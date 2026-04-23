
/**
 * @file DetID_test.cxx DetID class Unit Tests
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2022.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "detdataformats/DetID.hpp"

#define BOOST_TEST_MODULE DetID_test  // NOLINT

#include "boost/test/unit_test.hpp"

#include <string>
#include <vector>

using namespace dunedaq::detdataformats;

namespace {
  inline DetID from_string(const std::string& str)
  {
    return DetID(DetID::string_to_subdetector(str));
  }
}

BOOST_AUTO_TEST_SUITE(DetID_test)

BOOST_AUTO_TEST_CASE(DefaultConstruction)
{
  DetID detid;
  BOOST_REQUIRE_EQUAL(detid.subdetector, DetID::Subdetector::kUnknown);
}

BOOST_AUTO_TEST_CASE(ExplicitConstruction)
{
  DetID detid(DetID::Subdetector::kND_GAr);
  BOOST_REQUIRE_EQUAL(detid.subdetector, DetID::Subdetector::kND_GAr);
}

BOOST_AUTO_TEST_CASE(ImplicitConstruction)
{
  // Test implicit conversion from Subdetector to DetID
  DetID detid = DetID::Subdetector::kHD_TPC;
  BOOST_REQUIRE_EQUAL(detid.subdetector, DetID::Subdetector::kHD_TPC);
}

BOOST_AUTO_TEST_CASE(AllSubdetectorValues)
{
  // Test all defined subdetector enum values
  const std::vector<DetID::Subdetector> all_subdetectors = {
    DetID::Subdetector::kUnknown,
    DetID::Subdetector::kDAQ,
    DetID::Subdetector::kHD_PDS,
    DetID::Subdetector::kHD_TPC,
    DetID::Subdetector::kHD_CRT,
    DetID::Subdetector::kVD_CathodePDS,
    DetID::Subdetector::kVD_MembranePDS,
    DetID::Subdetector::kVD_BottomTPC,
    DetID::Subdetector::kVD_TopTPC,
    DetID::Subdetector::kVD_BernCRT,
    DetID::Subdetector::kVD_GrenobleCRT,
    DetID::Subdetector::kNDLAr_TPC,
    DetID::Subdetector::kNDLAr_PDS,
    DetID::Subdetector::kND_GAr
  };

  for (const auto& subdet : all_subdetectors) {
    DetID detid(subdet);
    BOOST_REQUIRE_EQUAL(detid.subdetector, subdet);
  }
}

BOOST_AUTO_TEST_CASE(SubdetectorToString)
{
  BOOST_REQUIRE_EQUAL(DetID::subdetector_to_string(DetID::Subdetector::kUnknown), "Unknown");
  BOOST_REQUIRE_EQUAL(DetID::subdetector_to_string(DetID::Subdetector::kDAQ), "DAQ");
  BOOST_REQUIRE_EQUAL(DetID::subdetector_to_string(DetID::Subdetector::kHD_PDS), "HD_PDS");
  BOOST_REQUIRE_EQUAL(DetID::subdetector_to_string(DetID::Subdetector::kHD_TPC), "HD_TPC");
  BOOST_REQUIRE_EQUAL(DetID::subdetector_to_string(DetID::Subdetector::kHD_CRT), "HD_CRT");
  BOOST_REQUIRE_EQUAL(DetID::subdetector_to_string(DetID::Subdetector::kVD_CathodePDS), "VD_CathodePDS");
  BOOST_REQUIRE_EQUAL(DetID::subdetector_to_string(DetID::Subdetector::kVD_MembranePDS), "VD_MembranePDS");
  BOOST_REQUIRE_EQUAL(DetID::subdetector_to_string(DetID::Subdetector::kVD_BottomTPC), "VD_BottomTPC");
  BOOST_REQUIRE_EQUAL(DetID::subdetector_to_string(DetID::Subdetector::kVD_TopTPC), "VD_TopTPC");
  BOOST_REQUIRE_EQUAL(DetID::subdetector_to_string(DetID::Subdetector::kVD_BernCRT), "VD_BernCRT");
  BOOST_REQUIRE_EQUAL(DetID::subdetector_to_string(DetID::Subdetector::kVD_GrenobleCRT), "VD_GrenobleCRT");
  BOOST_REQUIRE_EQUAL(DetID::subdetector_to_string(DetID::Subdetector::kNDLAr_TPC), "NDLAr_TPC");
  BOOST_REQUIRE_EQUAL(DetID::subdetector_to_string(DetID::Subdetector::kNDLAr_PDS), "NDLAr_PDS");
  BOOST_REQUIRE_EQUAL(DetID::subdetector_to_string(DetID::Subdetector::kND_GAr), "ND_GAr");
}

BOOST_AUTO_TEST_CASE(StringToSubdetector)
{
  BOOST_REQUIRE_EQUAL(DetID::string_to_subdetector("Unknown"), DetID::Subdetector::kUnknown);
  BOOST_REQUIRE_EQUAL(DetID::string_to_subdetector("DAQ"), DetID::Subdetector::kDAQ);
  BOOST_REQUIRE_EQUAL(DetID::string_to_subdetector("HD_PDS"), DetID::Subdetector::kHD_PDS);
  BOOST_REQUIRE_EQUAL(DetID::string_to_subdetector("HD_TPC"), DetID::Subdetector::kHD_TPC);
  BOOST_REQUIRE_EQUAL(DetID::string_to_subdetector("HD_CRT"), DetID::Subdetector::kHD_CRT);
  BOOST_REQUIRE_EQUAL(DetID::string_to_subdetector("VD_CathodePDS"), DetID::Subdetector::kVD_CathodePDS);
  BOOST_REQUIRE_EQUAL(DetID::string_to_subdetector("VD_MembranePDS"), DetID::Subdetector::kVD_MembranePDS);
  BOOST_REQUIRE_EQUAL(DetID::string_to_subdetector("VD_BottomTPC"), DetID::Subdetector::kVD_BottomTPC);
  BOOST_REQUIRE_EQUAL(DetID::string_to_subdetector("VD_TopTPC"), DetID::Subdetector::kVD_TopTPC);
  BOOST_REQUIRE_EQUAL(DetID::string_to_subdetector("VD_BernCRT"), DetID::Subdetector::kVD_BernCRT);
  BOOST_REQUIRE_EQUAL(DetID::string_to_subdetector("VD_GrenobleCRT"), DetID::Subdetector::kVD_GrenobleCRT);
  BOOST_REQUIRE_EQUAL(DetID::string_to_subdetector("NDLAr_TPC"), DetID::Subdetector::kNDLAr_TPC);
  BOOST_REQUIRE_EQUAL(DetID::string_to_subdetector("NDLAr_PDS"), DetID::Subdetector::kNDLAr_PDS);
  BOOST_REQUIRE_EQUAL(DetID::string_to_subdetector("ND_GAr"), DetID::Subdetector::kND_GAr);
}

BOOST_AUTO_TEST_CASE(StringToSubdetectorInvalid)
{
  // Invalid strings should fall back to kUnknown
  BOOST_REQUIRE_EQUAL(DetID::string_to_subdetector("InvalidName"), DetID::Subdetector::kUnknown);
  BOOST_REQUIRE_EQUAL(DetID::string_to_subdetector(""), DetID::Subdetector::kUnknown);
  BOOST_REQUIRE_EQUAL(DetID::string_to_subdetector("hd_tpc"), DetID::Subdetector::kUnknown);  // case sensitive
}

BOOST_AUTO_TEST_CASE(ConversionRoundTrip)
{
  // Test that conversion to/from string is reversible for all values
  const std::vector<DetID::Subdetector> all_subdetectors = {
    DetID::Subdetector::kUnknown,
    DetID::Subdetector::kDAQ,
    DetID::Subdetector::kHD_PDS,
    DetID::Subdetector::kHD_TPC,
    DetID::Subdetector::kHD_CRT,
    DetID::Subdetector::kVD_CathodePDS,
    DetID::Subdetector::kVD_MembranePDS,
    DetID::Subdetector::kVD_BottomTPC,
    DetID::Subdetector::kVD_TopTPC,
    DetID::Subdetector::kVD_BernCRT,
    DetID::Subdetector::kVD_GrenobleCRT,
    DetID::Subdetector::kNDLAr_TPC,
    DetID::Subdetector::kNDLAr_PDS,
    DetID::Subdetector::kND_GAr
  };

  for (const auto& subdet : all_subdetectors) {
    std::string str = DetID::subdetector_to_string(subdet);
    DetID::Subdetector recovered = DetID::string_to_subdetector(str);
    BOOST_REQUIRE_EQUAL(recovered, subdet);
  }
}

BOOST_AUTO_TEST_CASE(StreamOperatorOutput)
{
  DetID detid(DetID::Subdetector::kND_GAr);
  std::ostringstream ostr;
  ostr << detid;
  std::string output = ostr.str();
  BOOST_TEST_MESSAGE("Stream output: " << output);

  BOOST_REQUIRE(!output.empty());
  BOOST_REQUIRE(output.find("subdetector: ND_GAr") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(StreamOperatorRoundTrip)
{
  DetID detid(DetID::Subdetector::kHD_TPC);
  std::ostringstream ostr;
  ostr << detid;

  std::istringstream iss(ostr.str());
  DetID detid_from_stream;
  iss >> detid_from_stream;
  
  BOOST_REQUIRE_EQUAL(detid_from_stream.subdetector, detid.subdetector);
}

BOOST_AUTO_TEST_CASE(StreamOperatorRoundTripAll)
{
  // Test round-trip for all subdetector values:
  // DetID -> (<<) -> string -> (>>) -> DetID', verify equal
  const std::vector<DetID::Subdetector> all_subdetectors = {
    DetID::Subdetector::kUnknown,
    DetID::Subdetector::kDAQ,
    DetID::Subdetector::kHD_PDS,
    DetID::Subdetector::kHD_TPC,
    DetID::Subdetector::kHD_CRT,
    DetID::Subdetector::kVD_CathodePDS,
    DetID::Subdetector::kVD_MembranePDS,
    DetID::Subdetector::kVD_BottomTPC,
    DetID::Subdetector::kVD_TopTPC,
    DetID::Subdetector::kVD_BernCRT,
    DetID::Subdetector::kVD_GrenobleCRT,
    DetID::Subdetector::kNDLAr_TPC,
    DetID::Subdetector::kNDLAr_PDS,
    DetID::Subdetector::kND_GAr
  };

  for (const auto& subdet : all_subdetectors) {
    DetID original(subdet);
    
    // Serialize to string using <<
    std::ostringstream oss;
    oss << original;
    std::string serialized = oss.str();
    
    // Deserialize from string using >>
    std::istringstream iss(serialized);
    DetID recovered;
    iss >> recovered;
    
    // Verify they're equal
    BOOST_REQUIRE_EQUAL(recovered.subdetector, original.subdetector);
  }
}

BOOST_AUTO_TEST_CASE(SubdetectorEnumOperator)
{
  // Test stream operator on Subdetector enum directly
  DetID::Subdetector subdet = DetID::Subdetector::kHD_TPC;
  std::ostringstream ostr;
  ostr << subdet;
  std::string output = ostr.str();
  BOOST_REQUIRE_EQUAL(output, "HD_TPC");
}

BOOST_AUTO_TEST_SUITE_END()
