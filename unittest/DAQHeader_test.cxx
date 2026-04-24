/**
 * @file DAQHeader_test.cxx DAQHeader class Unit Tests
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2022.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "detdataformats/DAQHeader.hpp"

#define BOOST_TEST_MODULE DAQHeader_test  // NOLINT

#include "boost/test/unit_test.hpp"

#include <cstring>
#include <limits>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

#pragma GCC diagnostic ignored "-Woverflow" // intentional overflows are performed as part of testing

using namespace dunedaq::detdataformats;

namespace {
using word_t = DAQHeader::word_t;

DAQHeader
make_header(word_t version,
            word_t det_id,
            word_t crate_id,
            word_t slot_id,
            word_t link_id,
            word_t timestamp_1,
            word_t timestamp_2)
{
  DAQHeader header;
  header.version = version;
  header.det_id = det_id;
  header.crate_id = crate_id;
  header.slot_id = slot_id;
  header.link_id = link_id;
  header.timestamp_1 = timestamp_1;
  header.timestamp_2 = timestamp_2;
  return header;
}

std::pair<word_t, word_t>
split_timestamp(uint64_t timestamp)
{
  return {
    static_cast<word_t>(timestamp),
    static_cast<word_t>(timestamp >> 32)
  };
}

DAQHeader
header_from_stream_output(const std::string& output)
{
  std::istringstream iss(output);

  std::string version_token;
  std::string det_id_token;
  std::string crate_id_token;
  std::string slot_id_token;
  std::string link_id_token;
  std::string timestamp_label;
  uint64_t timestamp_value{};

  iss >> version_token >> det_id_token >> crate_id_token >> slot_id_token >> link_id_token >> timestamp_label >> timestamp_value;

  const auto [timestamp_1, timestamp_2] = split_timestamp(timestamp_value);

  return make_header(
    static_cast<word_t>(std::stoul(version_token.substr(std::string("Version:").size()))),
    static_cast<word_t>(std::stoul(det_id_token.substr(std::string("DetID:").size()))),
    static_cast<word_t>(std::stoul(crate_id_token.substr(std::string("CrateID:").size()))),
    static_cast<word_t>(std::stoul(slot_id_token.substr(std::string("SlotID:").size()))),
    static_cast<word_t>(std::stoul(link_id_token.substr(std::string("LinkID:").size()))),
    timestamp_1,
    timestamp_2);
}
}

BOOST_AUTO_TEST_SUITE(DAQHeader_test)

BOOST_AUTO_TEST_CASE(DefaultConstruction)
{
  DAQHeader header;

  BOOST_REQUIRE_EQUAL(header.timestamp_1, std::numeric_limits<word_t>::max());
  BOOST_REQUIRE_EQUAL(header.timestamp_2, std::numeric_limits<word_t>::max());
  BOOST_REQUIRE_EQUAL(header.get_timestamp(), std::numeric_limits<uint64_t>::max());
}

BOOST_AUTO_TEST_CASE(TimestampAssembly)
{
  DAQHeader header = make_header(0, 0, 0, 0, 0, 0x89ABCDEF, 0x01234567);

  BOOST_REQUIRE_EQUAL(header.get_timestamp(), 0x0123456789ABCDEFULL);
}

BOOST_AUTO_TEST_CASE(TimestampRoundTripFromGetTimestamp)
{
  const std::vector<std::pair<word_t, word_t>> values = {
    { 0x00000000u, 0x00000000u },
    { 0xFFFFFFFFu, 0x00000000u },
    { 0x00000000u, 0xFFFFFFFFu },
    { 0x01234567u, 0x89ABCDEFu },
    { 0xFFFFFFFFu, 0xFFFFFFFFu }
  };

  for (const auto& value : values) {
    const DAQHeader original = make_header(0, 0, 0, 0, 0, value.first, value.second);
    const uint64_t timestamp = original.get_timestamp();
    const auto [timestamp_1, timestamp_2] = split_timestamp(timestamp);

    BOOST_REQUIRE_EQUAL(timestamp_1, original.timestamp_1);
    BOOST_REQUIRE_EQUAL(timestamp_2, original.timestamp_2);
  }
}

BOOST_AUTO_TEST_CASE(BitfieldUpperBounds)
{
  DAQHeader header = make_header(63, 63, 1023, 15, 63, 0, 0);

  BOOST_REQUIRE_EQUAL(header.version, 63);
  BOOST_REQUIRE_EQUAL(header.det_id, 63);
  BOOST_REQUIRE_EQUAL(header.crate_id, 1023);
  BOOST_REQUIRE_EQUAL(header.slot_id, 15);
  BOOST_REQUIRE_EQUAL(header.link_id, 63);
}

BOOST_AUTO_TEST_CASE(BitfieldMasking)
{
  DAQHeader header = make_header(0, 0, 0, 0, 0, 0, 0);

  header.version = 99;     // 99 & 0x3F = 35
  header.det_id = 127;     // 127 & 0x3F = 63
  header.crate_id = 2048;  // 2048 & 0x3FF = 0
  header.slot_id = 31;     // 31 & 0x0F = 15
  header.link_id = 65;     // 65 & 0x3F = 1

  BOOST_REQUIRE_EQUAL(header.version, 35);
  BOOST_REQUIRE_EQUAL(header.det_id, 63);
  BOOST_REQUIRE_EQUAL(header.crate_id, 0);
  BOOST_REQUIRE_EQUAL(header.slot_id, 15);
  BOOST_REQUIRE_EQUAL(header.link_id, 1);
}

BOOST_AUTO_TEST_CASE(StreamOperatorRoundTrip)
{
  const DAQHeader original = make_header(7, 3, 22, 4, 9, 0x01234567, 0x89ABCDEF);

  std::ostringstream oss;
  oss << original;
  const std::string output = oss.str();
  const DAQHeader recovered = header_from_stream_output(output);

  BOOST_REQUIRE(!output.empty());
  BOOST_REQUIRE(output.back() == '\n');

  BOOST_REQUIRE_EQUAL(recovered.version, original.version);
  BOOST_REQUIRE_EQUAL(recovered.det_id, original.det_id);
  BOOST_REQUIRE_EQUAL(recovered.crate_id, original.crate_id);
  BOOST_REQUIRE_EQUAL(recovered.slot_id, original.slot_id);
  BOOST_REQUIRE_EQUAL(recovered.link_id, original.link_id);
  BOOST_REQUIRE_EQUAL(recovered.timestamp_1, original.timestamp_1);
  BOOST_REQUIRE_EQUAL(recovered.timestamp_2, original.timestamp_2);
}

BOOST_AUTO_TEST_CASE(ByteRoundTrip)
{
  const DAQHeader original = make_header(13, 21, 777, 10, 55, 0x11111111, 0x22222222);

  uint8_t buffer[sizeof(DAQHeader)]{};
  std::memcpy(buffer, &original, sizeof(DAQHeader));

  DAQHeader recovered;
  std::memcpy(&recovered, buffer, sizeof(DAQHeader));

  BOOST_REQUIRE_EQUAL(recovered.version, original.version);
  BOOST_REQUIRE_EQUAL(recovered.det_id, original.det_id);
  BOOST_REQUIRE_EQUAL(recovered.crate_id, original.crate_id);
  BOOST_REQUIRE_EQUAL(recovered.slot_id, original.slot_id);
  BOOST_REQUIRE_EQUAL(recovered.link_id, original.link_id);
  BOOST_REQUIRE_EQUAL(recovered.timestamp_1, original.timestamp_1);
  BOOST_REQUIRE_EQUAL(recovered.timestamp_2, original.timestamp_2);
  BOOST_REQUIRE_EQUAL(recovered.get_timestamp(), original.get_timestamp());
}

BOOST_AUTO_TEST_SUITE_END()
