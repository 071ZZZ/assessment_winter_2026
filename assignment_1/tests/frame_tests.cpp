#include "rmproto/frame.hpp"

#include <gtest/gtest.h>

TEST(RmProto, EncodeDecodeRoundTrip) {
    rmproto::Frame in;
    in.version = 1;
    in.seq     = 42;
    in.type    = 7;
    in.payload = { 0x01, 0x02, 0xA0, 0xFF };

    auto bytes = rmproto::Encode(in);

    std::vector<uint8_t> buf = bytes;
    rmproto::Frame out;
    ASSERT_TRUE(rmproto::TryDecode(buf, out));
    EXPECT_TRUE(buf.empty());

    EXPECT_EQ(out.version, in.version);
    EXPECT_EQ(out.seq, in.seq);
    EXPECT_EQ(out.type, in.type);
    EXPECT_EQ(out.payload, in.payload);
}
// 正常帧的编码与解码测试。
TEST(RmProto, ResyncOnGarbage) {
    rmproto::Frame in;
    in.seq     = 1;
    in.type    = 2;
    in.payload = { 0x10, 0x20 };

    auto bytes = rmproto::Encode(in);

    std::vector<uint8_t> stream = { 0x00, 0xFF, 0x13, 0x37 };
    stream.insert(stream.end(), bytes.begin(), bytes.end());

    rmproto::Frame out;
    ASSERT_TRUE(rmproto::TryDecode(stream, out));
    EXPECT_EQ(out.seq, in.seq);
    EXPECT_EQ(out.type, in.type);
    EXPECT_EQ(out.payload, in.payload);
}
// 帧前有垃圾数据，测试重新同步能力。
TEST(RmProto, RejectBadCrc) {
    rmproto::Frame in;
    in.seq     = 9;
    in.type    = 1;
    in.payload = { 0xAA };

    auto bytes = rmproto::Encode(in);
    bytes.back() ^= 0x01; // break crc

    std::vector<uint8_t> buf = bytes;
    rmproto::Frame out;
    EXPECT_FALSE(rmproto::TryDecode(buf, out));
    EXPECT_FALSE(buf.empty());
}
