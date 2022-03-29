/**
 * @file TDEFrame.hpp TDE1 bit fields and accessors
 * Originally FelixFrame.hpp from protodune.
 * Original authors M. Vermeulen, R.Sipos 2018
 * Modified by P. Rodrigues on June 2020
 *
 * This is part of the DUNE DAQ , copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */
#ifndef DETDATAFORMATS_INCLUDE_DATAFORMATS_TDE_TDEFRAME_HPP_
#define DETDATAFORMATS_INCLUDE_DATAFORMATS_TDE_TDEFRAME_HPP_

#include <bitset>
#include <iostream>
#include <vector>
#include <stdexcept>

namespace dunedaq {
namespace detdataformats {
namespace tde {

using word_t = uint32_t; // NOLINT(build/unsigned)
using adc_t = uint16_t;  // NOLINT(build/unsigned)
static constexpr int tot_adc_samples = 4474;
static constexpr int tot_adc_bits = tot_adc_samples * sizeof(adc_t);

struct TDEHeader
{
  word_t version : 6, det_id : 6, crate : 10, slot : 4, link : 6;
  word_t timestamp_1 : 32;
  word_t timestamp_2 : 32;
  word_t TAItime_1 : 32;
  word_t TAItime_2 : 32;
  word_t tdeheaderer : 10, tde_errors : 22;

  uint64_t get_timestamp() const // NOLINT(build/unsigned)
  {
    //return (uint64_t)h.timestamp_1 | ((uint64_t)h.timestamp_2 << 32);
    uint64_t timestamp = timestamp_1 | ((uint64_t)(timestamp_2) << 32);
    return timestamp;
  }

  uint64_t get_TAItime() const // NOLINT(build/unsigned)
  {
    uint64_t TAItime = TAItime_1 | ((uint64_t)TAItime_2 << 32); 
    return TAItime;
  }

  // Print functions for debugging.
  std::ostream& print_hex(std::ostream& o) const
  {
    return o << std::hex << "version:" << version << "det_id:" << det_id << "crate:" << crate << "slot:" << slot 
                         << "link:" << link << "timestamp: " << get_timestamp() << "TAItime: " << get_TAItime() 
                         << "tdeheaderer:" << tdeheaderer << "tde_errors:" << tde_errors << std::dec << '\n';
  }

  std::ostream& print_bits(std::ostream& o) const
  {
    return o << "version:" << std::bitset<6>(version) << "det_id:" << std::bitset<6>(det_id) 
             << "crate:" << std::bitset<10>(crate) << "slot:" << std::bitset<4>(slot) 
             << "link:" << std::bitset<6>(link) << "timestamp: " << get_timestamp() 
             << "TAItime: " << get_TAItime() << "tdeheaderer:" << std::bitset<10>(tdeheaderer) 
             << "tde_errors:" << std::bitset<16>(tde_errors) << '\n';
  }
};

inline std::ostream&
operator<<(std::ostream& o, TDEHeader const& h)
{
  return o << "version:" << unsigned(h.version) << "det_id:" << unsigned(h.det_id) << "crate:" << unsigned(h.crate) 
           << "slot:" << unsigned(h.slot) << "link:" << unsigned(h.link) << "timestamp: " << h.get_timestamp() 
           << "TAItime: " << h.get_TAItime() << "tdeheaderer:" << unsigned(h.tdeheaderer) 
           << "tde_errors:" << unsigned(h.tde_errors) << '\n';
}

struct ADCData
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
operator<<(std::ostream& o, ADCData const& d)
{
  return o << "sample:" << unsigned(d.sample) << "reserved:" << unsigned(d.reserved) << '\n';
}

class TDEFrame
{
public:
  adc_t adc_samples[tot_adc_samples];

  uint16_t get_adc_samples(int i) // NOLINT(build/unsigned)
  {
    if (i < 0 || i >= tot_adc_samples){throw std::out_of_range("ADC sample index out of range");}
    //return (uint16_t)(adc_samples[i].sample);
    return (uint16_t)(adc_samples[i]);
  }

  const TDEHeader* get_tde_header() const { return &tdeheader; }
  TDEHeader* get_tde_header() { return &tdeheader; }

  // TDEHeader mutators
  void set_tde_errors(const uint16_t new_tde_errors) { tdeheader.tde_errors = new_tde_errors; } // NOLINT(build/unsigned)
  uint64_t get_timestamp() const { return tdeheader.get_timestamp(); } // NOLINT(build/unsigned)
  friend std::ostream& operator<<(std::ostream& o, TDEFrame const& frame);

private:
  TDEHeader tdeheader;
  ADCData adcdata;
};

inline std::ostream&
operator<<(std::ostream& o, TDEFrame const& frame)
{
  o << "Printing frame:" << '\n';
  o << frame.tdeheader << '\n';
  return o;
}

} // namespace tde
} // namespace detdataformats
} // namespace dunedaq

#endif // DETDATAFORMATS_INCLUDE_DATAFORMATS_TDE_TDEFRAME_HPP_