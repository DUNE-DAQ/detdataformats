/**
 * @file GeoID_test.cxx GeoID struct Unit Tests
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2022.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "detdataformats/GeoID.hpp"

#define BOOST_TEST_MODULE GeoID_test // NOLINT

#include "boost/test/unit_test.hpp"

#include <algorithm>
#include <cstdint>
#include <string>
#include <vector>

using namespace dunedaq::detdataformats;

BOOST_AUTO_TEST_SUITE(GeoID_test)

BOOST_AUTO_TEST_CASE(DefaultConstruction)
{
  GeoID geo_id;
  BOOST_REQUIRE_EQUAL(geo_id.subdetector(), DetID::Subdetector::kUnknown);
  BOOST_REQUIRE_EQUAL(geo_id.encode(), 0);
}

BOOST_AUTO_TEST_CASE(ExplicitConstruction)
{
  GeoID geo_id(DetID::Subdetector::kHD_TPC, 1, 2, 3);
  BOOST_REQUIRE_EQUAL(geo_id.subdetector(), DetID::Subdetector::kHD_TPC);
  BOOST_REQUIRE_EQUAL(geo_id.detector_id, static_cast<uint16_t>(DetID::Subdetector::kHD_TPC)); // NOLINT(build/unsigned)
  BOOST_REQUIRE_EQUAL(geo_id.crate_id, 1);
  BOOST_REQUIRE_EQUAL(geo_id.slot_id, 2);
  BOOST_REQUIRE_EQUAL(geo_id.stream_id, 3);
}

BOOST_AUTO_TEST_CASE(EncodingMatchesFileFormat)
{
  // the layout already used in raw data files: stream<<48 | slot<<32 | crate<<16 | detector
  GeoID geo_id(DetID::Subdetector::kHD_TPC, 1, 0, 1);
  using encoded_t = GeoID::encoded_t;
  const encoded_t expected = (encoded_t{ 1 } << 48) | (encoded_t{ 0 } << 32) | (encoded_t{ 1 } << 16) | encoded_t{ 3 };
  BOOST_REQUIRE_EQUAL(geo_id.encode(), expected);
}

BOOST_AUTO_TEST_CASE(EncodeDecodeRoundTrip)
{
  const std::vector<GeoID> geo_ids = { GeoID(),
                                       GeoID(DetID::Subdetector::kHD_PDS, 4, 5, 6),
                                       GeoID(DetID::Subdetector::kND_GAr, 0xffff, 0xffff, 0xffff) };
  for (auto const& geo_id : geo_ids) {
    BOOST_REQUIRE(GeoID::decode(geo_id.encode()) == geo_id);
  }

  const uint64_t all_ones = ~uint64_t{ 0 }; // NOLINT(build/unsigned)
  BOOST_REQUIRE_EQUAL(GeoID::decode(all_ones).encode(), all_ones);
}

BOOST_AUTO_TEST_CASE(OrderingFollowsEncodedValue)
{
  std::vector<GeoID> geo_ids = { GeoID(DetID::Subdetector::kHD_TPC, 0, 0, 2),
                                 GeoID(DetID::Subdetector::kHD_TPC, 0, 1, 0),
                                 GeoID(DetID::Subdetector::kHD_PDS, 9, 0, 0) };
  std::sort(geo_ids.begin(), geo_ids.end());
  for (size_t idx = 1; idx < geo_ids.size(); ++idx) {
    BOOST_REQUIRE(geo_ids[idx - 1].encode() < geo_ids[idx].encode());
  }
  BOOST_REQUIRE(GeoID(DetID::Subdetector::kHD_TPC, 1, 2, 3) != GeoID(DetID::Subdetector::kHD_TPC, 1, 2, 4));
}

BOOST_AUTO_TEST_CASE(StreamOperator)
{
  GeoID geo_id(DetID::Subdetector::kHD_TPC, 1, 2, 3);
  BOOST_REQUIRE_EQUAL(geo_id.to_string(), "subdetector: HD_TPC, crate: 1, slot: 2, stream: 3");
}

BOOST_AUTO_TEST_SUITE_END()
