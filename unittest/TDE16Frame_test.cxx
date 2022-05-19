
/**
 * @file TDE16Frame_test.cxx TDE16Frame class Unit Tests
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2022.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "detdataformats/tde/TDE16Frame.hpp"

#define BOOST_TEST_MODULE TDE16Frame_test 

#include "boost/test/unit_test.hpp"

#include <string>
#include <vector>

using namespace dunedaq::detdataformats::tde;

BOOST_AUTO_TEST_SUITE(TDE16Frame_test)

BOOST_AUTO_TEST_CASE(TDE16Header_TimestampMethods)
{
  TDE16Header tde16header {};
  tde16header.timestamp_1 = 0x11111111;
  tde16header.timestamp_2 = 0x2222;

  BOOST_REQUIRE_EQUAL(tde16header.get_timestamp(), 0x222211111111);

  tde16header.set_timestamp(0x444455555555); 
  BOOST_REQUIRE_EQUAL(tde16header.get_timestamp(), 0x444455555555);
  BOOST_REQUIRE_EQUAL(tde16header.timestamp_1, 0x55555555);
  BOOST_REQUIRE_EQUAL(tde16header.timestamp_2, 0x4444);
}

BOOST_AUTO_TEST_CASE(TDE16Header_StreamMethods)
{
  TDE16Header tde16header {};

  std::ostringstream ostr;
  tde16header.print_hex(ostr);
  std::string output = ostr.str();
  BOOST_TEST_MESSAGE("Print hex: " << output);
  BOOST_REQUIRE(!output.empty());

  ostr.str("");
  ostr.clear();

  tde16header.print_bits(ostr);
  output = ostr.str();
  BOOST_TEST_MESSAGE("Print bits: " << output);
  BOOST_REQUIRE(!output.empty());

  ostr.str("");
  ostr.clear();

  ostr << tde16header;
  output = ostr.str();
  BOOST_TEST_MESSAGE("Stream operator: " << output);
  BOOST_REQUIRE(!output.empty());
}

BOOST_AUTO_TEST_CASE(TDE16Frame_StructMethods)
{
  TDE16Frame tde16frame {};

  BOOST_REQUIRE(tde16frame.get_tde_header() != nullptr);
}

BOOST_AUTO_TEST_CASE(TDE16Frame_HeaderMutators)
{
  TDE16Frame tde16frame {};
  tde16frame.set_tde_errors(0xFC5F);
  tde16frame.set_timestamp(0x444455555555);

  BOOST_REQUIRE_EQUAL(tde16frame.get_tde_header()->tde_errors, 0xFC5F);
  BOOST_REQUIRE_EQUAL(tde16frame.get_tde_header()->timestamp_1, 0x55555555);
  BOOST_REQUIRE_EQUAL(tde16frame.get_tde_header()->timestamp_2, 0x4444);
}

BOOST_AUTO_TEST_CASE(TDE16Frame_StreamOperator)
{
  TDE16Frame tde16frame {};

  std::ostringstream ostr;
  ostr << tde16frame;
  auto output = ostr.str();
  BOOST_TEST_MESSAGE("Stream operator: " << output);
  BOOST_REQUIRE(!output.empty());
}

BOOST_AUTO_TEST_CASE(TDE16Frame_ADCDataMutators)
{
  TDE16Frame tde16frame {};
  for(int i=0; i<tot_adc16_samples; i++) { tde16frame.set_adc_samples(0x63, i); }

  for(int i=0; i<30; i++) { BOOST_REQUIRE_EQUAL(tde16frame.get_adc_samples(i), 0x63); }
  for(int i=tot_adc16_samples-20; i<tot_adc16_samples; i++) { BOOST_REQUIRE_EQUAL(tde16frame.get_adc_samples(i), 0x63); }
}

BOOST_AUTO_TEST_CASE(TDE16Frame_PayloadSize)
{
  TDE16Frame tde16frame {};
  
  BOOST_REQUIRE_EQUAL(sizeof(tde16frame), payload16);
}

BOOST_AUTO_TEST_CASE(TDE16Frame_FromRawData)
{
  TDE16Header tde16header {};
  tde16header.timestamp_1 = 0x11111111;
  tde16header.timestamp_2 = 0x2222;
  Sample samples_info[tot_adc16_samples] {};
  
  uint8_t* buff = static_cast<uint8_t*>(malloc(sizeof(tde16header) + sizeof(samples_info))); 
  memcpy(buff, &tde16header, sizeof(tde16header));
  memcpy(buff + sizeof(tde16header), samples_info, sizeof(Sample) * tot_adc16_samples);
  TDE16Frame* from_raw_data = reinterpret_cast<TDE16Frame*>(buff); 

  BOOST_REQUIRE_EQUAL(from_raw_data->get_tde_header()->get_timestamp(), 0x222211111111);

  from_raw_data = nullptr;
  free(buff);
}

BOOST_AUTO_TEST_SUITE_END()
