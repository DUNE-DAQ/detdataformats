
/**
 * @file WIBEthFrame_test.cxx WIBEthFrame class Unit Tests
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2022.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "detdataformats/wibeth/WIBEthFrame.hpp"

#define BOOST_TEST_MODULE WIBEthFrame_test

#include "boost/test/unit_test.hpp"

#include <string>
#include <vector>
#include <random>

using namespace dunedaq::detdataformats::wibeth;

BOOST_AUTO_TEST_SUITE(WIBEthFrame_test)

BOOST_AUTO_TEST_CASE(WIBEthFrame_ADCDataMutators)
{
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> dist(1,1<<14-1);
  std::vector<int> v;
  for(int i=0; i<64 * 64; i++) {
    v.push_back(dist(rng));
  }

  WIBEthFrame wibethframe {};
  for(int i=0; i<64; i++) {
    for(int j=0; j<64; j++) {
      wibethframe.set_adc(i, j, v[i + 64*j]);
    }
  }

  for(int i=0; i<64; i++) {
    for(int j=0; j<64; j++) {
      BOOST_REQUIRE_EQUAL(wibethframe.get_adc(i, j), v[i + 64 * j]);
    }
  }

}

BOOST_AUTO_TEST_SUITE_END()
