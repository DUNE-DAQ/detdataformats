
/**
 * @file TDEFrame_test.cxx TDEFrame class Unit Tests
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2022.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "detdataformats/tde/TDEFrame.hpp"

/**
 * @brief Name of this test module
 */
#define BOOST_TEST_MODULE TDEFrame_test // NOLINT

#include "boost/test/unit_test.hpp"

#include <string>
#include <vector>

using namespace dunedaq::detdataformats::tde;

BOOST_AUTO_TEST_SUITE(TDEFrame_test)

BOOST_AUTO_TEST_CASE(TDEHeader_TimestampMethods)
{
  TDEHeader tdeheader;
  tdeheader.timestamp_1 = 0x11111111;
  tdeheader.timestamp_2 = 0x2222;

  BOOST_REQUIRE_EQUAL(tdeheader.get_timestamp(), 0x222211111111);
}

BOOST_AUTO_TEST_CASE(TDEHeader_StreamMethods)
{
  TDEHeader tdeheader;

  std::ostringstream ostr;
  tdeheader.print_hex(ostr);
  std::string output = ostr.str();
  BOOST_TEST_MESSAGE("Print hex: " << output);
  BOOST_REQUIRE(!output.empty());

  ostr.str("");
  ostr.clear();

  tdeheader.print_bits(ostr);
  output = ostr.str();
  BOOST_TEST_MESSAGE("Print bits: " << output);
  BOOST_REQUIRE(!output.empty());

  ostr.str("");
  ostr.clear();

  ostr << tdeheader;
  output = ostr.str();
  BOOST_TEST_MESSAGE("Stream operator: " << output);
  BOOST_REQUIRE(!output.empty());
}

BOOST_AUTO_TEST_CASE(TDEFrame_StructMethods)
{
  TDEFrame frame;

  BOOST_REQUIRE(frame.get_tde_header() != nullptr);
}

BOOST_AUTO_TEST_CASE(TDEFrame_TDEHeaderMutators)
{
  TDEFrame frame;
  frame.set_tde_errors(0xFC5F);

  BOOST_REQUIRE_EQUAL(frame.get_tde_header()->tde_errors, 0xFC5F);
}

BOOST_AUTO_TEST_CASE(TDEFrame_StreamOperator)
{
  TDEFrame frame;

  std::ostringstream ostr;
  ostr << frame;
  auto output = ostr.str();
  BOOST_TEST_MESSAGE("Stream operator: " << output);
  BOOST_REQUIRE(!output.empty());
}

BOOST_AUTO_TEST_CASE(TDEFrame_ADCDataMutators)
{
  TDEFrame frame;
  frame.set_adc_samples(0x9, 0x5);

  BOOST_REQUIRE_EQUAL(frame.get_adc_samples(0x5), 0x9);
}
/*
BOOST_AUTO_TEST_CASE(TDEFrame_FromRawData)
{
  TDEHeader tdeheader;
  tdeheader.timestamp_1 = 0x11111111;
  tdeheader.timestamp_2 = 0x2222;
  TDEFrame frame;
  frame.set_adc_samples(0x9, 0x5);

  ADCData samplesinfo[tot_adc_samples];

  uint8_t* buff = static_cast<uint8_t*>(malloc(sizeof(tdeheader) + sizeof(samplesinfo))); 
  memcpy(buff, &tdeheader, sizeof(tdeheader));
  memcpy(buff + sizeof(tdeheader), samplesinfo, sizeof(ADCData) * tot_adc_samples);

  TDEFrame* from_raw_data = reinterpret_cast<TDEFrame*>(buff); // NOLINT

  BOOST_REQUIRE_EQUAL(from_raw_data->get_tde_header()->get_timestamp(), 0x222211111111);
  BOOST_REQUIRE_EQUAL(from_raw_data->get_adc_samples(0x5), 0x9);

  from_raw_data = nullptr;
  free(buff);
}
*/

BOOST_AUTO_TEST_SUITE_END()
