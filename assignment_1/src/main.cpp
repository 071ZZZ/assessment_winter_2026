#include "rmproto/frame.hpp"

#include <iostream>
#include <string>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage:\n"
                  << "  rmproto_cli encode <seq> <type> <payload_hex>\n"
                  << "  rmproto_cli decode <stream_hex>\n";
        return 2;
    }

    std::string cmd = argv[1];
    if (cmd == "encode") {
        if (argc != 5) {
            std::cerr << "encode requires: <seq> <type> <payload_hex>\n";
            return 2;
        }

        rmproto::Frame f;
        f.seq  = static_cast<uint16_t>(std::stoul(argv[2]));
        f.type = static_cast<uint8_t>(std::stoul(argv[3]));
        if (!rmproto::ParseHexBytes(argv[4], f.payload)) {
            std::cerr << "invalid payload hex\n";
            return 2;
        }

        auto bytes = rmproto::Encode(f);
        std::cout << rmproto::ToHex(bytes) << "\n";
        return 0;
    }

    if (cmd == "decode") {
        if (argc != 3) {
            std::cerr << "decode requires: <stream_hex>\n";
            return 2;
        }

        std::vector<uint8_t> stream;
        if (!rmproto::ParseHexBytes(argv[2], stream)) {
            std::cerr << "invalid stream hex\n";
            return 2;
        }

        rmproto::Frame f;
        if (!rmproto::TryDecode(stream, f)) {
            std::cerr << "no valid frame\n";
            return 1;
        }

        std::cout << "version=" << static_cast<int>(f.version) << " seq=" << f.seq
                  << " type=" << static_cast<int>(f.type) << " payload=" << rmproto::ToHex(f.payload) << "\n";
        return 0;
    }

    std::cerr << "unknown command\n";
    return 2;
}
