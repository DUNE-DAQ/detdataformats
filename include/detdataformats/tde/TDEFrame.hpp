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


class TDEFrame
{
public:

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
    word_t tde_header : 10, tde_errors : 22;

    // Print functions for debugging.
    std::ostream& print_hex(std::ostream& o) const
    {
      return o << std::hex << "version:" << version << '\n'
                           << "det_id:" << det_id << '\n'
                           << "crate:" << crate << '\n'
                           << "slot:" << slot << '\n'
                           << "link:" << link << '\n'
                           << "timestamp: " << get_timestamp() << '\n'
                           << "TAItime: " << get_TAItime() << '\n'
                           << "tde_header:" << tde_header << '\n'
                           << "tde_errors:" << tde_errors << std::dec << '\n';
    }

    std::ostream& print_bits(std::ostream& o) const
    {
      return o << "version:" << std::bitset<6>(version) << '\n'
               << "det_id:" << std::bitset<6>(det_id) << '\n'
               << "crate:" << std::bitset<10>(crate) << '\n'
               << "slot:" << std::bitset<4>(slot) << '\n'
               << "link:" << std::bitset<6>(link) << '\n'
               << "timestamp: " << get_timestamp() << '\n'
               << "TAItime: " << get_TAItime() << '\n'
               << "tde_header:" << std::bitset<10>(tde_header) << '\n'
               << "tde_errors:" << std::bitset<16>(tde_errors) << '\n';
    }
  };

  struct ADCData
  {
    adc_t sample : 12, reserved : 4;

    // Print functions for debugging.
    std::ostream& print_hex(std::ostream& o) const
    {
      return o << std::hex << "sample:" << sample << '\n'
                           << "reserved:" << reserved << std::dec << '\n';
    }

    std::ostream& print_bits(std::ostream& o) const
    {
      return o << "sample:" << std::bitset<12>(sample) << '\n'
               << "reserved:" << std::bitset<4>(reserved) << '\n';
    }
  };

  TDEHeader tdeheader;
  ADCData adcdata;
  adc_t adc_samples[tot_adc_samples];

  uint64_t get_timestamp() const // NOLINT(build/unsigned)
  {
    return (uint64_t)tdeheader.timestamp_1 | ((uint64_t)tdeheader.timestamp_2 << 32); 
  }

  uint64_t get_TAItime() const // NOLINT(build/unsigned)
  {
    return (uint64_t)tdeheader.TAItime_1 | ((uint64_t)tdeheader.TAItime_2 << 32);
  }

  void set_timestamp(const uint64_t new_timestamp) // NOLINT(build/unsigned)
  {
    timestamp_1 = new_timestamp;
    timestamp_2 = new_timestamp >> 32;
  }

  uint16_t get_adc_samples(int i) // NOLINT(build/unsigned)
  {
    if (i < 0 || i >= tot_adc_samples)
        throw std::out_of_range("ADC sample index out of range");

    return (uint16_t)(adc_samples[i].sample);
  }

  const WIBHeader* get_tde_header() const { return &tde_head; }
  WIBHeader* get_tde_header() { return &tde_head; }

  // WIBHeader mutators
  void set_tde_errors(const uint16_t new_tde_errors) { tde_head.tde_errors = new_tde_errors; } // NOLINT(build/unsigned)
  void set_timestamp(const uint64_t new_timestamp) { tde_head.set_timestamp(new_timestamp); }  // NOLINT(build/unsigned)

  friend std::ostream& operator<<(std::ostream& o, TDEFrame const& f);

private:
  TDEFrame tde_head;

};

inline std::ostream&
operator<<(std::ostream& o, TDEHeader const& h)
{
  return o << "version:" << unsigned(h.version) << '\n'
           << "det_id:" << unsigned(h.det_id) << '\n'
           << "crate:" << unsigned(h.crate) << '\n'
           << "slot:" << unsigned(h.slot) << '\n'
           << "link:" << unsigned(h.link) << '\n'
           << "timestamp: " << h.get_timestamp() << '\n'
           << "TAItime: " << h.get_TAItime() << '\n'
           << "tde_header:" << unsigned(h.tde_header) << '\n'
           << "tde_errors:" << unsigned(h.tde_errors) << '\n';
}

inline std::ostream&
operator<<(std::ostream& o, ADCData const& h)
{
  return o << "sample:" << unsigned(h.sample) << '\n'
           << "reserved:" << unsigned(h.reserved) << '\n';
}

inline std::ostream&
operator<<(std::ostream& o, TDEFrame const& f)
{
  o << "Printing frame:" << '\n';
  o << f.tde_head << '\n';
  return o;
}



} // namespace tde
} // namespace detdataformats
} // namespace dunedaq

#endif // DETDATAFORMATS_INCLUDE_DATAFORMATS_TDE_TDEFRAME_HPP_
