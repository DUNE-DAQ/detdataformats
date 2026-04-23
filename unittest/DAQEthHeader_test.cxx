/**
 * @file DAQEthHeader_test.cxx DAQEthHeader class Unit Tests
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2022.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "detdataformats/DAQEthHeader.hpp"

#define BOOST_TEST_MODULE DAQEthHeader_test  // NOLINT

#include "boost/test/unit_test.hpp"

#include <cstring>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

using namespace dunedaq::detdataformats;

namespace {
using word_t = DAQEthHeader::word_t;

DAQEthHeader
make_header(word_t version,
            word_t det_id,
            word_t crate_id,
            word_t slot_id,
            word_t stream_id,
            word_t reserved,
            word_t seq_id,
            word_t block_length,
            word_t timestamp)
{
  DAQEthHeader header;
  header.version = version;
  header.det_id = det_id;
  header.crate_id = crate_id;
  header.slot_id = slot_id;
  header.stream_id = stream_id;
  header.reserved = reserved;
  header.seq_id = seq_id;
  header.block_length = block_length;
  header.timestamp = timestamp;
  return header;
}

DAQEthHeader
header_from_stream_output(const std::string& output)
{
  std::istringstream iss(output);

  std::string version_token;
  std::string det_id_token;
  std::string crate_id_token;
  std::string slot_id_token;
  std::string stream_id_token;
  std::string sequence_label;
  unsigned int sequence_value{};
  std::string block_label;
  std::string block_length_label;
  unsigned int block_length_value{};
  std::string timestamp_label;
  uint64_t timestamp_value{};

  iss >> version_token >> det_id_token >> crate_id_token >> slot_id_token >> stream_id_token >> sequence_label >> sequence_value >> block_label >> block_length_label >> block_length_value >> timestamp_label >> timestamp_value;

  BOOST_REQUIRE_EQUAL(sequence_label, "SequenceID:");
  BOOST_REQUIRE_EQUAL(block_label, "Block");
  BOOST_REQUIRE_EQUAL(block_length_label, "length:");
  BOOST_REQUIRE_EQUAL(timestamp_label, "Timestamp:");

  return make_header(
    static_cast<word_t>(std::stoull(version_token.substr(std::string("Version:").size()))),
    static_cast<word_t>(std::stoull(det_id_token.substr(std::string("DetID:").size()))),
    static_cast<word_t>(std::stoull(crate_id_token.substr(std::string("CrateID:").size()))),
    static_cast<word_t>(std::stoull(slot_id_token.substr(std::string("SlotID:").size()))),
    static_cast<word_t>(std::stoull(stream_id_token.substr(std::string("StreamID:").size()))),
    0,
    static_cast<word_t>(sequence_value),
    static_cast<word_t>(block_length_value),
    static_cast<word_t>(timestamp_value));
}
} // namespace

BOOST_AUTO_TEST_SUITE(DAQEthHeader_test)

BOOST_AUTO_TEST_CASE(DefaultConstruction)
{
  DAQEthHeader header;

  BOOST_REQUIRE_EQUAL(header.timestamp, std::numeric_limits<word_t>::max());
  BOOST_REQUIRE_EQUAL(header.get_timestamp(), std::numeric_limits<uint64_t>::max());
}

BOOST_AUTO_TEST_CASE(TimestampAccess)
{
  const DAQEthHeader header = make_header(0, 0, 0, 0, 0, 0, 0, 0, 0x0123456789ABCDEFULL);

  BOOST_REQUIRE_EQUAL(header.get_timestamp(), 0x0123456789ABCDEFULL);
}

BOOST_AUTO_TEST_CASE(TimestampRoundTripFromGetTimestamp)
{
  const std::vector<word_t> values = {
    0x0000000000000000ULL,
    0x0000000000000001ULL,
    0x0123456789ABCDEFULL,
    0xFEDCBA9876543210ULL,
    0xFFFFFFFFFFFFFFFFULL
  };

  for (const auto& value : values) {
    const DAQEthHeader original = make_header(0, 0, 0, 0, 0, 0, 0, 0, value);
    const uint64_t timestamp = original.get_timestamp();

    BOOST_REQUIRE_EQUAL(static_cast<word_t>(timestamp), original.timestamp);
  }
}

BOOST_AUTO_TEST_CASE(BitfieldUpperBounds)
{
  const DAQEthHeader header = make_header(63, 63, 1023, 15, 255, 63, 4095, 4095, 0);

  BOOST_REQUIRE_EQUAL(header.version, 63);
  BOOST_REQUIRE_EQUAL(header.det_id, 63);
  BOOST_REQUIRE_EQUAL(header.crate_id, 1023);
  BOOST_REQUIRE_EQUAL(header.slot_id, 15);
  BOOST_REQUIRE_EQUAL(header.stream_id, 255);
  BOOST_REQUIRE_EQUAL(header.reserved, 63);
  BOOST_REQUIRE_EQUAL(header.seq_id, 4095);
  BOOST_REQUIRE_EQUAL(header.block_length, 4095);
}

BOOST_AUTO_TEST_CASE(BitfieldMasking)
{
  DAQEthHeader header = make_header(0, 0, 0, 0, 0, 0, 0, 0, 0);

  header.version = 99;         // 99 & 0x3F = 35
  header.det_id = 127;         // 127 & 0x3F = 63
  header.crate_id = 2048;      // 2048 & 0x3FF = 0
  header.slot_id = 31;         // 31 & 0x0F = 15
  header.stream_id = 511;      // 511 & 0xFF = 255
  header.reserved = 127;       // 127 & 0x3F = 63
  header.seq_id = 5000;        // 5000 & 0xFFF = 904
  header.block_length = 5000;  // 5000 & 0xFFF = 904

  BOOST_REQUIRE_EQUAL(header.version, 35);
  BOOST_REQUIRE_EQUAL(header.det_id, 63);
  BOOST_REQUIRE_EQUAL(header.crate_id, 0);
  BOOST_REQUIRE_EQUAL(header.slot_id, 15);
  BOOST_REQUIRE_EQUAL(header.stream_id, 255);
  BOOST_REQUIRE_EQUAL(header.reserved, 63);
  BOOST_REQUIRE_EQUAL(header.seq_id, 904);
  BOOST_REQUIRE_EQUAL(header.block_length, 904);
}

BOOST_AUTO_TEST_CASE(StreamOperatorRoundTrip)
{
  const DAQEthHeader original = make_header(
    7,
    3,
    22,
    4,
    9,
    0,
    1234,
    88,
    0x0123456789ABCDEFULL);

  std::ostringstream oss;
  oss << original;
  const std::string output = oss.str();
  const DAQEthHeader recovered = header_from_stream_output(output);

  BOOST_REQUIRE(!output.empty());
  BOOST_REQUIRE(output.back() == '\n');

  BOOST_REQUIRE_EQUAL(recovered.version, original.version);
  BOOST_REQUIRE_EQUAL(recovered.det_id, original.det_id);
  BOOST_REQUIRE_EQUAL(recovered.crate_id, original.crate_id);
  BOOST_REQUIRE_EQUAL(recovered.slot_id, original.slot_id);
  BOOST_REQUIRE_EQUAL(recovered.stream_id, original.stream_id);
  BOOST_REQUIRE_EQUAL(recovered.reserved, original.reserved);
  BOOST_REQUIRE_EQUAL(recovered.seq_id, original.seq_id);
  BOOST_REQUIRE_EQUAL(recovered.block_length, original.block_length);
  BOOST_REQUIRE_EQUAL(recovered.timestamp, original.timestamp);
}

BOOST_AUTO_TEST_CASE(ByteRoundTrip)
{
  const DAQEthHeader original = make_header(
    13,
    21,
    777,
    10,
    55,
    6,
    2048,
    1024,
    0x1122334455667788ULL);

  uint8_t buffer[sizeof(DAQEthHeader)]{};
  std::memcpy(buffer, &original, sizeof(DAQEthHeader));

  DAQEthHeader recovered;
  std::memcpy(&recovered, buffer, sizeof(DAQEthHeader));

  BOOST_REQUIRE_EQUAL(recovered.version, original.version);
  BOOST_REQUIRE_EQUAL(recovered.det_id, original.det_id);
  BOOST_REQUIRE_EQUAL(recovered.crate_id, original.crate_id);
  BOOST_REQUIRE_EQUAL(recovered.slot_id, original.slot_id);
  BOOST_REQUIRE_EQUAL(recovered.stream_id, original.stream_id);
  BOOST_REQUIRE_EQUAL(recovered.reserved, original.reserved);
  BOOST_REQUIRE_EQUAL(recovered.seq_id, original.seq_id);
  BOOST_REQUIRE_EQUAL(recovered.block_length, original.block_length);
  BOOST_REQUIRE_EQUAL(recovered.timestamp, original.timestamp);
  BOOST_REQUIRE_EQUAL(recovered.get_timestamp(), original.get_timestamp());
}

BOOST_AUTO_TEST_SUITE_END()
