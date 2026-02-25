#include "rmproto/frame.hpp"

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <sstream>

namespace rmproto {

namespace {
    constexpr uint8_t kSof0 = 0xA5;
    constexpr uint8_t kSof1 = 0x5A;

    void AppendLe16(std::vector<uint8_t>& out, uint16_t v) {
        out.push_back(static_cast<uint8_t>(v & 0xFF));
        out.push_back(static_cast<uint8_t>((v >> 8) & 0xFF));
    }

    uint16_t ReadLe16(const std::vector<uint8_t>& in, size_t offset) {
        return static_cast<uint16_t>(in[offset]) | (static_cast<uint16_t>(in[offset + 1]) << 8);
    }

    bool IsHexChar(char c) {
        return std::isxdigit(static_cast<unsigned char>(c)) != 0;
    }

    int HexVal(char c) {
        if (c >= '0' && c <= '9')
            return c - '0';
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        if (c >= 'a' && c <= 'f')
            return 10 + (c - 'a');
        return -1;
    }

} // namespace

uint16_t Crc16Ccitt(const uint8_t* data, size_t len) {
    // TODO: 实现 CRC16-CCITT。
    // 参数：poly=0x1021, init=0xFFFF。
    // CRC 必须覆盖 version..payload 这些字节（即 SOF 之后、crc16 之前的全部内容）。
    // 单元测试依赖这一点。
    const uint16_t poly = 0x1021;
    uint16_t crc        = 0xFFFF;

    for (size_t i = 0; i < len; ++i) {
        crc ^= (static_cast<uint16_t>(data[i]) << 8);
        for (int j = 0; j < 8; ++j) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ poly;
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}

std::vector<uint8_t> Encode(const Frame& f) {
    // TODO: 将 Frame 序列化为线上的 wire 格式。
    // Wire 格式（字段为小端序 little-endian）：
    //   SOF[2] = 0xA5 0x5A
    //   version[1]
    //   payload_len[2]
    //   seq[2]
    //   type[1]
    //   payload[payload_len]
    //   crc16[2]（对 version..payload 计算 CRC16-CCITT）
    //
    // 这个占位实现会故意生成不正确的帧，从而让单元测试失败；
    // 但仍保证工程（以及 CLI）可以正常编译。
    std::vector<uint8_t> out;
    out.reserve(2 + 1 + 2 + 2 + 1 + f.payload.size() + 2);

    out.push_back(kSof0);
    out.push_back(kSof1);
    out.push_back(f.version);
    AppendLe16(out, static_cast<uint16_t>(f.payload.size()));
    AppendLe16(out, f.seq);
    out.push_back(f.type);
    out.insert(out.end(), f.payload.begin(), f.payload.end());
    const uint8_t* crc_data = out.data() + 2;
    size_t crc_len          = out.size() - 2;
    uint16_t crc            = Crc16Ccitt(crc_data, crc_len);
    // TODO: 替换为真实的 CRC。
    AppendLe16(out, crc);
    return out;
}

bool TryDecode(std::vector<uint8_t>& buffer, Frame& out) {
    // TODO: 流式解析器。
    // 需求（见 README + 测试）：
    // - buffer 是字节流，开头可能包含垃圾数据。
    // - 必须搜索 SOF（0xA5 0x5A）来重新同步。
    // - 若不足以组成完整帧：返回 false，并保持 buffer 不变。
    // - 若候选帧 CRC 错误 / 长度非法：丢弃部分字节并继续搜索（必须避免死循环）。
    // - 成功时：填充 out，从 buffer 中擦除已消费的字节，并返回 true。
    const size_t maxpayload_len = 1024 * 1024;
    while (true) {
        if (buffer.size() < 2) {
            return false;
        }
        size_t sof_pos = 0;
        bool found     = false;
        for (; sof_pos + 1 < buffer.size(); ++sof_pos) {
            if (buffer[sof_pos] == 0xA5 && buffer[sof_pos + 1] == 0x5A) {
                found = true;
                break;
            }
        }
        if (!found) {
            return false;
        }
        if (sof_pos > 0) {
            buffer.erase(buffer.begin(), buffer.begin() + sof_pos);
            continue;
        }
        uint16_t payload_len = ReadLe16(buffer, 3);
        size_t total_len =
            2 + 1 + 2 + 2 + 1 + payload_len + 2; // SOF(2) + version(1) + payload_len(2) + seq(2) + type(1)
        if (payload_len > maxpayload_len) {
            buffer.erase(buffer.begin(), buffer.begin() + 2); // 丢弃当前 SOF
            continue;
        }
        if (buffer.size() < total_len) {
            return false;
        }
        const uint8_t* crc_data = buffer.data() + 2;
        size_t crc_len          = total_len - 4;
        uint16_t expected_crc   = Crc16Ccitt(crc_data, crc_len);
        uint16_t actual_crc     = ReadLe16(buffer, total_len - 2);
        if (expected_crc != actual_crc) { // CRC 校验失败，丢弃当前 SOF，继续向后搜索
            size_t next_sof_pos = 1;
            bool found_next_sof = false;
            for (; next_sof_pos + 1 < buffer.size(); ++next_sof_pos) {
                if (buffer[next_sof_pos] == 0xA5 && buffer[next_sof_pos + 1] == 0x5A) {
                    found_next_sof = true;
                    break;
                }
            }
            size_t erase_len = found_next_sof ? next_sof_pos : 2;
            buffer.erase(buffer.begin(), buffer.begin() + erase_len);
            continue;
        }
        out.version = buffer[2];
        out.seq     = ReadLe16(buffer, 5);
        out.type    = buffer[7];
        out.payload.assign(buffer.begin() + 8, buffer.begin() + 8 + payload_len);
        buffer.erase(buffer.begin(), buffer.begin() + total_len);
        return true;
    }
}

bool ParseHexBytes(const std::string& text, std::vector<uint8_t>& out) {
    // 支持：
    // - "A5 5A 01 00"（空格分隔）
    // - "A5,5A,01,00"（逗号分隔）
    // - "0xA5 0x5A"（每个字节可选 0x 前缀）
    // - "a55a0100"（无分隔符）
    // - 输出： 每个字节一个元素的 vector<uint8_t>
    out.clear();

    size_t i      = 0;
    auto skip_sep = [&](void) {
        while (i < text.size()) {
            char c = text[i];
            if (std::isspace(static_cast<unsigned char>(c)) || c == ',') {
                ++i;
                continue;
            }
            break;
        }
    };

    while (true) {
        skip_sep();
        if (i >= text.size())
            break;

        // 可选的 0x 前缀（仅当其严格以 "0x"/"0X" 形式出现时）。
        if (text[i] == '0' && (i + 1) < text.size() && (text[i + 1] == 'x' || text[i + 1] == 'X')) {
            i += 2;
        }

        if ((i + 1) >= text.size())
            return false;
        if (!IsHexChar(text[i]) || !IsHexChar(text[i + 1]))
            return false;

        int hi = HexVal(text[i]);
        int lo = HexVal(text[i + 1]);
        if (hi < 0 || lo < 0)
            return false;

        out.push_back(static_cast<uint8_t>((hi << 4) | lo));
        i += 2;
    }

    return true;
}

std::string ToHex(const std::vector<uint8_t>& bytes) {
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    for (size_t i = 0; i < bytes.size(); ++i) {
        if (i)
            oss << ' ';
        oss << std::setw(2) << static_cast<int>(bytes[i]);
    }
    return oss.str();
}

} // namespace rmproto
