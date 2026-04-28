/**
 * @file HSIFrame_test.cxx HSIFrame class Unit Tests
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2022.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "detdataformats/HSIFrame.hpp"

#define BOOST_TEST_MODULE HSIFrame_test  // NOLINT

#include "boost/test/unit_test.hpp"

#include <cstring>
#include <limits>
#include <vector>

using namespace dunedaq::detdataformats;

// Unit tests for a data formats library can expect to work with a lot of unsigned integers
// NOLINTBEGIN(build/unsigned)

namespace {
  // Helper to create HSIFrame from individual components
  HSIFrame make_frame(uint32_t version, uint32_t detector_id, uint32_t crate, 
                      uint32_t slot, uint32_t link,
                      uint64_t timestamp, uint32_t input_low, uint32_t input_high,
                      uint32_t trigger, uint32_t sequence)
  {
    HSIFrame frame;
    frame.version = version;
    frame.detector_id = detector_id;
    frame.crate = crate;
    frame.slot = slot;
    frame.link = link;
    frame.set_timestamp(timestamp);
    frame.input_low = input_low;
    frame.input_high = input_high;
    frame.trigger = trigger;
    frame.sequence = sequence;
    return frame;
  }
} // namespace ""

BOOST_AUTO_TEST_SUITE(HSIFrame_test)

BOOST_AUTO_TEST_CASE(DefaultConstruction)
{
  HSIFrame frame;
  
    // Note: bitfield members are uninitialized by default, so we don't test them here
  BOOST_REQUIRE_EQUAL(frame.get_timestamp(), std::numeric_limits<uint64_t>::max());
  BOOST_REQUIRE_EQUAL(frame.input_low, std::numeric_limits<uint32_t>::max());
  BOOST_REQUIRE_EQUAL(frame.input_high, std::numeric_limits<uint32_t>::max());
  BOOST_REQUIRE_EQUAL(frame.trigger, std::numeric_limits<uint32_t>::max());
  BOOST_REQUIRE_EQUAL(frame.sequence, std::numeric_limits<uint32_t>::max());
}

BOOST_AUTO_TEST_CASE(GetTimestamp)
{
  HSIFrame frame;
  
  uint64_t ts = frame.get_timestamp();
  BOOST_REQUIRE_EQUAL(ts, std::numeric_limits<uint64_t>::max());
}

BOOST_AUTO_TEST_CASE(SetTimestampMax)
{
  HSIFrame frame;
  
  uint64_t max_ts = std::numeric_limits<uint64_t>::max();
  frame.set_timestamp(max_ts);
  BOOST_REQUIRE_EQUAL(frame.get_timestamp(), max_ts);
  BOOST_REQUIRE_EQUAL(frame.timestamp_low, std::numeric_limits<uint32_t>::max());
  BOOST_REQUIRE_EQUAL(frame.timestamp_high, std::numeric_limits<uint32_t>::max());
}

BOOST_AUTO_TEST_CASE(SetTimestampSplit)
{
  HSIFrame frame;
  
  // Test a 64-bit value that crosses the 32-bit boundary
  uint64_t ts = 0x0123456789ABCDEF;
  frame.set_timestamp(ts);
  
  BOOST_REQUIRE_EQUAL(frame.get_timestamp(), ts);
  BOOST_REQUIRE_EQUAL(frame.timestamp_low, 0x89ABCDEF);      // lower 32 bits
  BOOST_REQUIRE_EQUAL(frame.timestamp_high, 0x01234567);     // upper 32 bits
}

BOOST_AUTO_TEST_CASE(TimestampRoundTrip)
{
  // Test a range of timestamp values
  const std::vector<uint64_t> test_timestamps = {
    0,
    1,
    0xFF,
    0xFFFFFFFF,  // max 32-bit
    0x100000000,  // just above 32-bit
    0x123456789ABCDEF0,
    std::numeric_limits<uint64_t>::max()
  };

  for (const auto& ts : test_timestamps) {
    HSIFrame frame;
    frame.set_timestamp(ts);
    BOOST_REQUIRE_EQUAL(frame.get_timestamp(), ts);
  }
}

BOOST_AUTO_TEST_CASE(BitfieldMembers)
{
  HSIFrame frame;
  
  frame.version = 5;
  frame.detector_id = 3;
  frame.crate = 7;
  frame.slot = 2;
  frame.link = 42;
  
  BOOST_REQUIRE_EQUAL(frame.version, 5);
  BOOST_REQUIRE_EQUAL(frame.detector_id, 3);
  BOOST_REQUIRE_EQUAL(frame.crate, 7);
  BOOST_REQUIRE_EQUAL(frame.slot, 2);
  BOOST_REQUIRE_EQUAL(frame.link, 42);
}

BOOST_AUTO_TEST_CASE(BitfieldMask)
{
  HSIFrame frame;
  
  // version is 6 bits: max value 63
  frame.version = 63;
  BOOST_REQUIRE_EQUAL(frame.version, 63);
  
  // detector_id is 6 bits: max value 63
  frame.detector_id = 63;
  BOOST_REQUIRE_EQUAL(frame.detector_id, 63);
  
  // crate is 10 bits: max value 1023
  frame.crate = 1023;
  BOOST_REQUIRE_EQUAL(frame.crate, 1023);
  
  // slot is 4 bits: max value 15
  frame.slot = 15;
  BOOST_REQUIRE_EQUAL(frame.slot, 15);
  
  // link is 6 bits: max value 63
  frame.link = 63;
  BOOST_REQUIRE_EQUAL(frame.link, 63);
}

BOOST_AUTO_TEST_CASE(RegularMembers)
{
  HSIFrame frame;
  
  frame.input_low = 0x11223344;
  frame.input_high = 0x55667788;
  frame.trigger = 0xDEADBEEF;
  frame.sequence = 0xCAFEBABE;
  
  BOOST_REQUIRE_EQUAL(frame.input_low, 0x11223344);
  BOOST_REQUIRE_EQUAL(frame.input_high, 0x55667788);
  BOOST_REQUIRE_EQUAL(frame.trigger, 0xDEADBEEF);
  BOOST_REQUIRE_EQUAL(frame.sequence, 0xCAFEBABE);
}

BOOST_AUTO_TEST_CASE(CompleteFrameSetup)
{
  HSIFrame frame = make_frame(
    5,                    // version
    3,                    // detector_id
    7,                    // crate
    2,                    // slot
    42,                   // link
    0x0123456789ABCDEF,   // timestamp
    0x11111111,           // input_low
    0x22222222,           // input_high
    0x33333333,           // trigger
    0x44444444            // sequence
  );
  
  BOOST_REQUIRE_EQUAL(frame.version, 5);
  BOOST_REQUIRE_EQUAL(frame.detector_id, 3);
  BOOST_REQUIRE_EQUAL(frame.crate, 7);
  BOOST_REQUIRE_EQUAL(frame.slot, 2);
  BOOST_REQUIRE_EQUAL(frame.link, 42);
  BOOST_REQUIRE_EQUAL(frame.get_timestamp(), 0x0123456789ABCDEF);
  BOOST_REQUIRE_EQUAL(frame.input_low, 0x11111111);
  BOOST_REQUIRE_EQUAL(frame.input_high, 0x22222222);
  BOOST_REQUIRE_EQUAL(frame.trigger, 0x33333333);
  BOOST_REQUIRE_EQUAL(frame.sequence, 0x44444444);
}

BOOST_AUTO_TEST_CASE(ByteRoundTrip)
{
  // Create a frame with specific values
  HSIFrame original = make_frame(
    5, 3, 7, 2, 42,
    0x0123456789ABCDEF,
    0x11111111, 0x22222222,
    0x33333333, 0x44444444
  );
  
  // Serialize to bytes
  uint8_t buffer[ sizeof(HSIFrame) ]; // NOLINT(modernize-avoid-c-arrays)
  std::memcpy(buffer, &original, sizeof(HSIFrame));

  // Deserialize from bytes
  HSIFrame recovered;
  std::memcpy(&recovered, buffer, sizeof(HSIFrame));
  
  // Verify all fields match
  BOOST_REQUIRE_EQUAL(recovered.version, original.version);
  BOOST_REQUIRE_EQUAL(recovered.detector_id, original.detector_id);
  BOOST_REQUIRE_EQUAL(recovered.crate, original.crate);
  BOOST_REQUIRE_EQUAL(recovered.slot, original.slot);
  BOOST_REQUIRE_EQUAL(recovered.link, original.link);
  BOOST_REQUIRE_EQUAL(recovered.get_timestamp(), original.get_timestamp());
  BOOST_REQUIRE_EQUAL(recovered.input_low, original.input_low);
  BOOST_REQUIRE_EQUAL(recovered.input_high, original.input_high);
  BOOST_REQUIRE_EQUAL(recovered.trigger, original.trigger);
  BOOST_REQUIRE_EQUAL(recovered.sequence, original.sequence);
}

BOOST_AUTO_TEST_CASE(TimestampEndianness)
{
  HSIFrame frame;
  
  // Set timestamp to a value where endianness matters
  uint64_t ts = 0x0102030405060708;
  frame.set_timestamp(ts);
  
  // On little-endian: low 32 bits should be 0x05060708, high 32 bits should be 0x01020304
  BOOST_REQUIRE_EQUAL(frame.timestamp_low, 0x05060708);
  BOOST_REQUIRE_EQUAL(frame.timestamp_high, 0x01020304);
  
  // Verify get_timestamp reconstructs it correctly
  BOOST_REQUIRE_EQUAL(frame.get_timestamp(), ts);
}

BOOST_AUTO_TEST_CASE(MultipleFramesIndependent)
{
  HSIFrame frame1 = make_frame(1, 1, 1, 1, 1, 0x1111111111111111, 0x11, 0x11, 0x11, 0x11);
  HSIFrame frame2 = make_frame(2, 2, 2, 2, 2, 0x2222222222222222, 0x22, 0x22, 0x22, 0x22);
  
  // Verify they're independent
  BOOST_REQUIRE_EQUAL(frame1.version, 1);
  BOOST_REQUIRE_EQUAL(frame2.version, 2);
  
  BOOST_REQUIRE_EQUAL(frame1.get_timestamp(), 0x1111111111111111);
  BOOST_REQUIRE_EQUAL(frame2.get_timestamp(), 0x2222222222222222);
  
  // Modify frame1, verify frame2 unchanged
    frame1.version = 5;  // Use a value that fits in 6 bits
      BOOST_REQUIRE_EQUAL(frame1.version, 5);
    BOOST_REQUIRE_EQUAL(frame1.version, 5);
  BOOST_REQUIRE_EQUAL(frame2.version, 2);
}

BOOST_AUTO_TEST_CASE(EdgeCaseHighBitfieldValues)
{
  HSIFrame frame;
  
  // Set all bitfields to maximum valid values for their bit widths
  frame.version = (1u << 6) - 1;         // 6 bits: 0x3F = 63
  frame.detector_id = (1u << 6) - 1;     // 6 bits: 0x3F = 63
  frame.crate = (1u << 10) - 1;          // 10 bits: 0x3FF = 1023
  frame.slot = (1u << 4) - 1;            // 4 bits: 0xF = 15
  frame.link = (1u << 6) - 1;            // 6 bits: 0x3F = 63
  
  // Verify all are at max
  BOOST_REQUIRE_EQUAL(frame.version, 63);
  BOOST_REQUIRE_EQUAL(frame.detector_id, 63);
  BOOST_REQUIRE_EQUAL(frame.crate, 1023);
  BOOST_REQUIRE_EQUAL(frame.slot, 15);
  BOOST_REQUIRE_EQUAL(frame.link, 63);
}

BOOST_AUTO_TEST_SUITE_END()

// NOLINTEND(build/unsigned)
