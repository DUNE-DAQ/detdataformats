
/**
 * @file TDE12Frame_test.cxx TDE12Frame class Unit Tests
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2022.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "detdataformats/tde/TDE12Frame.hpp"

#define BOOST_TEST_MODULE TDE12Frame_test 

#include "boost/test/unit_test.hpp"

#include <string>
#include <vector>

using namespace dunedaq::detdataformats::tde;

BOOST_AUTO_TEST_SUITE(TDE12Frame_test)

BOOST_AUTO_TEST_CASE(TDE12Header_TimestampMethods)
{
  TDE12Header tde12header;
  tde12header.timestamp_1 = 0x11111111;
  tde12header.timestamp_2 = 0x2222;

  BOOST_REQUIRE_EQUAL(tde12header.get_timestamp(), 0x222211111111);

  tde12header.set_timestamp(0x444455555555); 
  BOOST_REQUIRE_EQUAL(tde12header.get_timestamp(), 0x444455555555);
  BOOST_REQUIRE_EQUAL(tde12header.timestamp_1, 0x55555555);
  BOOST_REQUIRE_EQUAL(tde12header.timestamp_2, 0x4444);
}

BOOST_AUTO_TEST_CASE(TDE12Header_StreamMethods)
{
  TDE12Header tde12header;

  std::ostringstream ostr;
  tde12header.print_hex(ostr);
  std::string output = ostr.str();
  BOOST_TEST_MESSAGE("Print hex: " << output);
  BOOST_REQUIRE(!output.empty());

  ostr.str("");
  ostr.clear();

  tde12header.print_bits(ostr);
  output = ostr.str();
  BOOST_TEST_MESSAGE("Print bits: " << output);
  BOOST_REQUIRE(!output.empty());

  ostr.str("");
  ostr.clear();

  ostr << tde12header;
  output = ostr.str();
  BOOST_TEST_MESSAGE("Stream operator: " << output);
  BOOST_REQUIRE(!output.empty());
}

BOOST_AUTO_TEST_CASE(TDE12Frame_StructMethods)
{
  TDE12Frame tde12frame;

  BOOST_REQUIRE(tde12frame.get_tde_header() != nullptr);
}

BOOST_AUTO_TEST_CASE(TDE12Frame_HeaderMutators)
{
  TDE12Frame tde12frame;
  tde12frame.set_tde_errors(0xFC5F);
  tde12frame.set_timestamp(0x444455555555);

  BOOST_REQUIRE_EQUAL(tde12frame.get_tde_header()->tde_errors, 0xFC5F);
  BOOST_REQUIRE_EQUAL(tde12frame.get_tde_header()->timestamp_1, 0x55555555);
  BOOST_REQUIRE_EQUAL(tde12frame.get_tde_header()->timestamp_2, 0x4444);
}

BOOST_AUTO_TEST_CASE(TDE12Frame_StreamOperator)
{
  TDE12Frame tde12frame;

  std::ostringstream ostr;
  ostr << tde12frame;
  auto output = ostr.str();
  BOOST_TEST_MESSAGE("Stream operator: " << output);
  BOOST_REQUIRE(!output.empty());
}

BOOST_AUTO_TEST_CASE(TDE12Frame_ADCDataMutators)
{
  TDE12Frame tde12frame;
  for(int i=0; i<tot_adc12_samples; i++) { tde12frame.set_adc_samples(0x9, i); }

  BOOST_REQUIRE_EQUAL(tde12frame.get_adc_samples(0x1), 0x9);
  BOOST_REQUIRE_EQUAL(tde12frame.get_adc_samples(0x2), 0x9);
  BOOST_REQUIRE_EQUAL(tde12frame.get_adc_samples(0x63), 0x9);
}

BOOST_AUTO_TEST_CASE(TDE12Frame_FromRawData)
{
  TDE12Header tde12header;
  tde12header.timestamp_1 = 0x11111111;
  tde12header.timestamp_2 = 0x2222;
  uint32_t words_info[num_adc_words] {};

  uint8_t* buff = static_cast<uint8_t*>(malloc(sizeof(tde12header) + sizeof(words_info))); 
  memcpy(buff, &tde12header, sizeof(tde12header));
  memcpy(buff + sizeof(tde12header), words_info, sizeof(uint32_t) * num_adc_words);
  TDE12Frame* from_raw_data = reinterpret_cast<TDE12Frame*>(buff); 

  BOOST_REQUIRE_EQUAL(from_raw_data->get_tde_header()->get_timestamp(), 0x222211111111);

  from_raw_data = nullptr;
  free(buff);
}

BOOST_AUTO_TEST_SUITE_END()
