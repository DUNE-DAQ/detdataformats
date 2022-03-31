
/**
 * @file TDE2Frame_test.cxx TDE2Frame class Unit Tests
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2022.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "detdataformats/tde/TDE2Frame.hpp"

/**
 * @brief Name of this test module
 */
#define BOOST_TEST_MODULE TDE2Frame_test // NOLINT

#include "boost/test/unit_test.hpp"

#include <string>
#include <vector>

using namespace dunedaq::detdataformats::tde2;

BOOST_AUTO_TEST_SUITE(TDE2Frame_test)

BOOST_AUTO_TEST_CASE(TDE2Header_TimestampMethods)
{
  TDE2Header tde2header;
  tde2header.timestamp_1 = 0x11111111;
  tde2header.timestamp_2 = 0x2222;

  BOOST_REQUIRE_EQUAL(tde2header.get_timestamp(), 0x222211111111);
}

BOOST_AUTO_TEST_CASE(TDE2Header_StreamMethods)
{
  TDE2Header tde2header;

  std::ostringstream ostr;
  tde2header.print_hex(ostr);
  std::string output = ostr.str();
  BOOST_TEST_MESSAGE("Print hex: " << output);
  BOOST_REQUIRE(!output.empty());

  ostr.str("");
  ostr.clear();

  tde2header.print_bits(ostr);
  output = ostr.str();
  BOOST_TEST_MESSAGE("Print bits: " << output);
  BOOST_REQUIRE(!output.empty());

  ostr.str("");
  ostr.clear();

  ostr << tde2header;
  output = ostr.str();
  BOOST_TEST_MESSAGE("Stream operator: " << output);
  BOOST_REQUIRE(!output.empty());
}

BOOST_AUTO_TEST_CASE(TDE2Frame_StructMethods)
{
  TDE2Frame frame;

  BOOST_REQUIRE(frame.get_tde2_header() != nullptr);
}

BOOST_AUTO_TEST_CASE(TDE2Frame_TDE2HeaderMutators)
{
  TDE2Frame frame;
  frame.set_tde2_errors(0xFC5F);

  BOOST_REQUIRE_EQUAL(frame.get_tde2_header()->tde2_errors, 0xFC5F);
}

BOOST_AUTO_TEST_CASE(TDE2Frame_StreamOperator)
{
  TDE2Frame frame;

  std::ostringstream ostr;
  ostr << frame;
  auto output = ostr.str();
  BOOST_TEST_MESSAGE("Stream operator: " << output);
  BOOST_REQUIRE(!output.empty());
}

BOOST_AUTO_TEST_CASE(TDE2Frame_ADCDataMutators)
{
  TDE2Frame frame;
  frame.set_adc_samples(0x9, 0x5);

  BOOST_REQUIRE_EQUAL(frame.get_adc_samples(0x5), 0x9);
}
/*
BOOST_AUTO_TEST_CASE(TDE2Frame_FromRawData)
{
  TDE2Header tde2header;
  tde2header.timestamp_1 = 0x11111111;
  tde2header.timestamp_2 = 0x2222;
  TDE2Frame frame;
  frame.set_adc_samples(0x9, 0x5);

  ADCData samplesinfo[tot_adc_samples];

  uint8_t* buff = static_cast<uint8_t*>(malloc(sizeof(tde2header) + sizeof(samplesinfo))); 
  memcpy(buff, &tde2header, sizeof(tde2header));
  memcpy(buff + sizeof(tde2header), samplesinfo, sizeof(ADCData) * tot_adc_samples);

  TDE2Frame* from_raw_data = reinterpret_cast<TDE2Frame*>(buff); // NOLINT

  BOOST_REQUIRE_EQUAL(from_raw_data->get_tde2_header()->get_timestamp(), 0x222211111111);
  BOOST_REQUIRE_EQUAL(from_raw_data->get_adc_samples(0x5), 0x9);

  from_raw_data = nullptr;
  free(buff);
}
*/

BOOST_AUTO_TEST_SUITE_END()
