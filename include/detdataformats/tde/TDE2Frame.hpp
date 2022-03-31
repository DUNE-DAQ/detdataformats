/**
 * @file TDE2Frame.hpp TDE2 bit fields and accessors
 * Originally FelixFrame.hpp from protodune.
 * Original authors M. Vermeulen, R.Sipos 2018
 * Modified by P. Rodrigues on June 2020
 *
 * This is part of the DUNE DAQ , copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */
#ifndef DETDATAFORMATS_INCLUDE_DATAFORMATS_TDE2_TDE2FRAME_HPP_
#define DETDATAFORMATS_INCLUDE_DATAFORMATS_TDE2_TDE2FRAME_HPP_

#include <bitset>
#include <iostream>
#include <vector>
#include <stdexcept>

namespace dunedaq {
namespace detdataformats {
namespace tde2 {

using word_t = uint32_t; 
using adc_t  = uint16_t;  

static constexpr int tot_adc_samples = 4474;

struct TDE2Header
{
  word_t version : 6, det_id : 6, crate : 10, slot : 4, link : 6;
  word_t timestamp_1 : 32;
  word_t timestamp_2 : 32;
  word_t TAItime_1 : 32;
  word_t TAItime_2 : 32;
  word_t tde2_header : 10, tde2_errors : 22;

  uint64_t get_timestamp() const { return (uint64_t)timestamp_1 | ((uint64_t)(timestamp_2) << 32); }

  uint64_t get_TAItime() const { return (uint64_t)TAItime_1 | ((uint64_t)TAItime_2 << 32); }

  // Print functions for debugging.
  std::ostream& print_hex(std::ostream& o) const
  {
    return o << std::hex << "version:" << version << "det_id:" << det_id << "crate:" << crate << "slot:" << slot 
                         << "link:" << link << "timestamp: " << get_timestamp() << "TAItime: " << get_TAItime() 
                         << "tde2_header:" << tde2_header << "tde2_errors:" << tde2_errors << std::dec << '\n';
  }

  std::ostream& print_bits(std::ostream& o) const
  {
    return o << "version:" << std::bitset<6>(version) << "det_id:" << std::bitset<6>(det_id) 
             << "crate:" << std::bitset<10>(crate) << "slot:" << std::bitset<4>(slot) 
             << "link:" << std::bitset<6>(link) << "timestamp: " << get_timestamp() 
             << "TAItime: " << get_TAItime() << "tde2_header:" << std::bitset<10>(tde2_header) 
             << "tde2_errors:" << std::bitset<16>(tde2_errors) << '\n';
  }
};

inline std::ostream&
operator<<(std::ostream& o, TDE2Header const& h)
{
  return o << "version:" << unsigned(h.version) << "det_id:" << unsigned(h.det_id) << "crate:" << unsigned(h.crate) 
           << "slot:" << unsigned(h.slot) << "link:" << unsigned(h.link) << "timestamp: " << h.get_timestamp() 
           << "TAItime: " << h.get_TAItime() << "tde2_header:" << unsigned(h.tde2_header) 
           << "tde2_errors:" << unsigned(h.tde2_errors) << '\n';
}

struct Sample 
{
  adc_t sample : 12, reserved : 4;

  // Print functions for debugging.
  std::ostream& print_hex(std::ostream& o) const
  {
    return o << std::hex << "sample:" << sample << "reserved:" << reserved << std::dec << '\n';
  }

  std::ostream& print_bits(std::ostream& o) const
  {
    return o << "sample:" << std::bitset<12>(sample) << "reserved:" << std::bitset<4>(reserved) << '\n';
  }
};

inline std::ostream&
operator<<(std::ostream& o, Sample const& s)
{
  return o << "sample:" << unsigned(s.sample) << "reserved:" << unsigned(s.reserved) << '\n';
}

struct ADCData
{
  Sample samplesinfo[tot_adc_samples];

  uint16_t get_adc_samples(int i) 
  {
    if (i < 0 || i >= tot_adc_samples) { throw std::out_of_range("ADC sample index out of range"); }
    
    return (uint64_t)samplesinfo[i].sample;
  }
};

class TDE2Frame
{
public:
  const TDE2Header* get_tde2_header() const { return &tde2header; }
  TDE2Header* get_tde2_header() { return &tde2header; }

  // TDE2Header mutators
  void set_tde2_errors(const uint16_t new_tde2_errors) { tde2header.tde2_errors = new_tde2_errors; } 
  uint64_t get_timestamp() const { return tde2header.get_timestamp(); } 

  // ADCData mutators
  void set_adc_samples(const uint16_t new_adc_samples, int sample_no) { adcdata.samplesinfo[sample_no].sample = new_adc_samples; } 
  uint16_t get_adc_samples(int sample_no) { return adcdata.get_adc_samples(sample_no); } 

  friend std::ostream& operator<<(std::ostream& o, TDE2Frame const& frame);

private:
  TDE2Header tde2header;
  ADCData adcdata;
};

inline std::ostream&
operator<<(std::ostream& o, TDE2Frame const& frame)
{
  o << "Printing frame:" << '\n';
  o << frame.tde2header << '\n';
  return o;
}

} // namespace tde2
} // namespace detdataformats
} // namespace dunedaq

#endif // DETDATAFORMATS_INCLUDE_DATAFORMATS_TDE2_TDE2FRAME_HPP_
