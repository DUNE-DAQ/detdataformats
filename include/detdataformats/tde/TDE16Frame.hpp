/**
 * @file TDE16Frame.hpp TDE 16 bit fields and accessors
 *
 * This is part of the DUNE DAQ, copyright 2022.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */
#ifndef DETDATAFORMATS_INCLUDE_DATAFORMATS_TDE_TDE16FRAME_HPP_
#define DETDATAFORMATS_INCLUDE_DATAFORMATS_TDE_TDE16FRAME_HPP_

#include <bitset>
#include <iostream>
#include <vector>
#include <stdexcept> 

#include <ostream>
#include <string>
#include <fstream>
#include <iterator>

namespace dunedaq {
namespace detdataformats {
namespace tde {

static constexpr int ticks_between_adc_samples = 32;
static constexpr int tot_adc16_samples = 4474;
static constexpr int n_channels_per_amc = 64;
static constexpr int payload16 = 8972;

struct TDE16Header
{
  uint32_t version : 6, det_id : 6, crate : 10, slot : 4, link : 6;
  uint32_t timestamp_1 : 32;
  uint32_t timestamp_2 : 32;
  uint32_t TAItime_1 : 32;
  uint32_t TAItime_2 : 32;
  uint32_t tde_header : 10, tde_errors : 22;

  uint64_t get_timestamp() const { return (uint64_t)(timestamp_1) | ((uint64_t)(timestamp_2) << 32); }
  uint16_t get_channel() const { return link; }

  uint64_t get_TAItime() const { return (uint64_t)(TAItime_1) | ((uint64_t)(TAItime_2) << 32); }
  void set_timestamp(const uint64_t new_timestamp) 
  {
    timestamp_1 = new_timestamp;
    timestamp_2 = new_timestamp >> 32;
  }
  void set_TAItime(const uint64_t new_TAItime) 
  {
    TAItime_1 = new_TAItime;
    TAItime_2 = new_TAItime >> 32;
  }

  // Print functions for debugging.
  std::ostream& print_hex(std::ostream& o) const
  {
    return o << std::hex << "version: " << version << " det_id: " << det_id << " crate: " << crate << " slot: " << slot << " link: " << link 
                         << "timestamp: " << get_timestamp() 
                         << "TAItime: " << get_TAItime() 
                         << "tde_header: " << tde_header << " tde_errors: " << tde_errors << std::dec << '\n';
  }

  std::ostream& print_bits(std::ostream& o) const
  {
    return o << "version: " << std::bitset<6>(version) << " det_id: " << std::bitset<6>(det_id) << " crate: " << std::bitset<10>(crate) << " slot: " << std::bitset<4>(slot) << " link: " << std::bitset<6>(link) 
             << "timestamp: " << get_timestamp() 
             << "TAItime: " << get_TAItime() 
             << "tde_header: " << std::bitset<10>(tde_header) << " tde_errors: " << std::bitset<16>(tde_errors) << '\n';
  }

};

struct Sample 
{
  uint16_t sample : 12, reserved : 4;

  // Print functions for debugging.
  std::ostream& print_hex(std::ostream& o) const
  {
    return o << std::hex << "sample: " << sample << " reserved: " << reserved << std::dec << '\n';
  }

  std::ostream& print_bits(std::ostream& o) const
  {
    return o << "sample: " << std::bitset<12>(sample) << " reserved: " << std::bitset<4>(reserved) << '\n';
  }
};

struct ADC16Data
{
  Sample samples_info[tot_adc16_samples];

  uint16_t get_adc_sample(int i) const
  {
    if (i < 0 || i >= tot_adc16_samples) { throw std::out_of_range("ADC sample index out of range"); }
    
    return (uint16_t)samples_info[i].sample;
  }
};

class TDE16Frame
{
public:
  const TDE16Header* get_tde_header() const { return &tde16header; }
  TDE16Header* get_tde_header() { return &tde16header; }

  // TDE16Header mutators
  void set_tde_errors(const uint16_t new_tde_errors) { tde16header.tde_errors = new_tde_errors; } 
  void set_timestamp(const uint64_t new_timestamp) { tde16header.set_timestamp(new_timestamp); } 
  void set_channel(const uint16_t new_channel) { tde16header.link=new_channel; } 
  void set_TAItime(const uint64_t new_TAItime) { tde16header.set_TAItime(new_TAItime); } 
  uint64_t get_timestamp() const { return tde16header.get_timestamp(); } 
  uint16_t get_channel() const { return tde16header.get_channel(); } 

  // ADC16Data mutators
  void set_adc_sample(const uint16_t new_adc_val, int sample_no) { adc16data.samples_info[sample_no].sample = new_adc_val; } 
  uint16_t get_adc_sample(int sample_no) const { return adc16data.get_adc_sample(sample_no); } 

  friend std::ostream& operator<<(std::ostream& o, TDE16Frame const& tde16frame);

private:
  TDE16Header tde16header;
  ADC16Data adc16data;
};

inline std::ostream&
operator<<(std::ostream& o, TDE16Header const& tde16header)
{
  return o << "version: " << unsigned(tde16header.version) << " det_id: " << unsigned(tde16header.det_id) << " crate: " << unsigned(tde16header.crate) << " slot: " << unsigned(tde16header.slot) << " link: " << unsigned(tde16header.link) 
           << "timestamp: " << tde16header.get_timestamp() 
           << "TAItime: " << tde16header.get_TAItime() 
           << "tde_header: " << unsigned(tde16header.tde_header) << " tde_errors: " << unsigned(tde16header.tde_errors) << '\n';
}

inline std::ostream&
operator<<(std::ostream& o, Sample const& sampleinfo)
{
  return o << "sample: " << unsigned(sampleinfo.sample) << " reserved: " << unsigned(sampleinfo.reserved) << '\n';
}

inline std::ostream&
operator<<(std::ostream& o, TDE16Frame const& tde16frame)
{
  o << "Printing frame:" << '\n';
  o << tde16frame.tde16header << '\n';
  return o;
}



} // namespace tde
} // namespace detdataformats
} // namespace dunedaq

#endif // DETDATAFORMATS_INCLUDE_DATAFORMATS_TDE_TDE16FRAME_HPP_
