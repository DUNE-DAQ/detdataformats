
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
  TDEHeader header;
  header.timestamp_1 = 0x11111111;
  header.timestamp_2 = 0x2222;

  BOOST_REQUIRE_EQUAL(header.get_timestamp(), 0x7FFF222211111111);

  header.set_timestamp(0xF333444455555555); 
  BOOST_REQUIRE_EQUAL(header.get_timestamp(), 0x7333444455555555);
  BOOST_REQUIRE_EQUAL(header.timestamp_1, 0x55555555);
  BOOST_REQUIRE_EQUAL(header.timestamp_2, 0x4444);

}

BOOST_AUTO_TEST_CASE(TDEHeader_StreamMethods)
{
  TDEHeader header;

  std::ostringstream ostr;
  header.print_hex(ostr);
  std::string output = ostr.str();
  BOOST_TEST_MESSAGE("Print hex: " << output);
  BOOST_REQUIRE(!output.empty());

  ostr.str("");
  ostr.clear();

  header.print_bits(ostr);
  output = ostr.str();
  BOOST_TEST_MESSAGE("Print bits: " << output);
  BOOST_REQUIRE(!output.empty());

  ostr.str("");
  ostr.clear();

  ostr << header;
  output = ostr.str();
  BOOST_TEST_MESSAGE("Stream operator: " << output);
  BOOST_REQUIRE(!output.empty());
}

BOOST_AUTO_TEST_CASE(TDEFrame_StructMethods)
{
  TDEFrame frame;

  BOOST_REQUIRE(frame.get_tde_header() != nullptr);
}

BOOST_AUTO_TEST_CASE(TDEFrame_HeaderMutators)
{
  {
    TDEFrame frame;
    frame.set_tde_errors(0xFC5F);
    BOOST_REQUIRE_EQUAL(frame.get_tde_header()->tde_errors, 0xFC5F);
  }

  {
    TDEFrame frame;
    frame.set_timestamp(0x7333444455555555);
    BOOST_REQUIRE_EQUAL(frame.get_tde_header()->get_timestamp(), 0x444455555555);
  }
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

BOOST_AUTO_TEST_CASE(TDEFrame_FromRawData)
{
  TDEHeader header;

  header.timestamp_1 = 0x12345678;
  header.timestamp_2 = 0x9ABC;

  ADCData samples[TDEFrame::tot_adc_samples];

  uint8_t* buff = static_cast<uint8_t*>(malloc(sizeof(header) + sizeof(adc_t))); // NOLINT(build/unsigned)
  memcpy(buff, &header, sizeof(header));
  memcpy(buff + sizeof(header), samples, sizeof(ADCData) * TDEFrame::tot_adc_samples);

  TDEFrame* from_raw_data = reinterpret_cast<TDEFrame*>(buff); // NOLINT

  BOOST_REQUIRE_EQUAL(from_raw_data->get_tde_header()->get_timestamp(), 0x9ABC12345678);
  from_raw_data = nullptr;
  free(buff);
}

BOOST_AUTO_TEST_SUITE_END()
