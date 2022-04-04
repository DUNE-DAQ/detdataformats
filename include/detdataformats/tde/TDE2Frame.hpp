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
#include <algorithm> // For std::min
#include <cassert>   // For assert()
#include <cstdint>   // For uint32_t etc
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <stdexcept> // For std::out_of_range

namespace dunedaq {
namespace detdataformats {
namespace tde2 {

//using word_t = uint32_t; 
//using adc_t  = uint16_t;  

static constexpr int tot_adc_samples = 5965;
static constexpr int tot_adc_sets = 746;
static constexpr int adc_set_samples = 8;
static constexpr int bits_per_adc = 12;
static constexpr int bits_per_word = 3 * sizeof(uint32_t);
static constexpr int num_adc_words = tot_adc_samples * bits_per_adc / bits_per_word;

struct TDE2Header
{
  uint32_t version : 6, det_id : 6, crate : 10, slot : 4, link : 6;
  uint32_t timestamp_1 : 32;
  uint32_t timestamp_2 : 32;
  uint32_t TAItime_1 : 32;
  uint32_t TAItime_2 : 32;
  uint32_t tde2_header : 10, tde2_errors : 22;

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

struct Word
{
  uint32_t sample_0 : 12, sample_1 : 12, sample_2_0 : 8;
  uint32_t sample_2_1 : 4, sample_3 : 12, sample_4 : 12, sample_5_0 : 4;
  uint32_t sample_5_1 : 8, sample_6 : 12, sample_7 : 12;
  
  // Print functions for debugging.
  std::ostream& print_hex(std::ostream& o) const
  {
    return o << std::hex << "sample_0:" << sample_0    
                         << "sample_1:" << sample_1 
                         << "sample_2_0:" << sample_2_0 << "sample_2_1:" << sample_2_1
                         << "sample_3:" << sample_3 
                         << "sample_4:" << sample_4    
                         << "sample_5_0:" << sample_5_0 << "sample_5_1:" << sample_5_1 
                         << "sample_6:" << sample_6    
                         << "sample_7:" << sample_7 << std::dec << '\n';
  }

  std::ostream& print_bits(std::ostream& o) const
  {
    return o << "sample_0:" << std::bitset<12>(sample_0) 
             << "sample_1:" << std::bitset<12>(sample_1) 
             << "sample_2_0:" << std::bitset<8>(sample_2_0) << "sample_2_1:" << std::bitset<4>(sample_2_1)              
             << "sample_3:" << std::bitset<12>(sample_3) 
             << "sample_4:" << std::bitset<12>(sample_4) 
             << "sample_5_0:" << std::bitset<4>(sample_5_0) << "sample_5_1:" << std::bitset<8>(sample_5_1)
             << "sample_6:" << std::bitset<12>(sample_6) 
             << "sample_7:" << std::bitset<12>(sample_7) << '\n';
  }
};

inline std::ostream&
operator<<(std::ostream& o, Word const& w)
{
  return o << "sample_0:" << unsigned(w.sample_0) 
           << "sample_1:" << unsigned(w.sample_1) 
           << "sample_2_0:" << unsigned(w.sample_2_0) << "sample_2_1:" << unsigned(w.sample_2_1)         
           << "sample_3:" << unsigned(w.sample_3) 
           << "sample_4:" << unsigned(w.sample_4) 
           << "sample_5_0:" << unsigned(w.sample_5_0) << "sample_5_1:" << unsigned(w.sample_5_1) 
           << "sample_6:" << unsigned(w.sample_6) 
           << "sample_7:" << unsigned(w.sample_7) << '\n';
}

struct ADCData
{
  uint32_t samples_info[num_adc_words];

  uint16_t get_adc_samples(int i) const 
  {
    if (i < 0 || i >= tot_adc_samples) { throw std::out_of_range("ADC sample index out of range"); }

    int adc_set_no = round(i / 8);
    if (adc_set_no < 0 || adc_set_no >= tot_adc_sets) { throw std::out_of_range("ADC set index out of range"); }

    // The index of the first (and sometimes only) word containing the required ADC value
    int word_index = bits_per_adc * i / bits_per_word;
    assert(word_index < num_adc_words);

    // Where in the word the lowest bit of our ADC value is located
    int first_bit_position = (bits_per_adc * i) % bits_per_word;

    // How many bits of our desired ADC are located in the `word_index`th word
    int bits_from_first_word = std::min(bits_per_adc, bits_per_word - first_bit_position);
    uint16_t adc_info = samples_info[word_index] >> first_bit_position; 

    // If we didn't get the full 12 bits from this word, we need the rest from the next word
    if (bits_from_first_word < bits_per_adc) {
      assert(word_index + 1 < num_adc_words);
      adc_info |= samples_info[word_index + 1] << bits_from_first_word;
    }

    return adc_info;
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
  void set_adc_samples(const uint16_t new_adc_val, int sample_no) { 

    if (sample_no < 0 || sample_no >= tot_adc_samples)
      throw std::out_of_range("ADC sample index out of range");
    if (new_adc_val >= (1 << bits_per_adc))
      throw std::out_of_range("ADC bits value out of range");

    // The index of the first (and sometimes only) word containing the required ADC value
    int word_index = bits_per_adc * sample_no / bits_per_word;
    assert(word_index < num_adc_words);

    // Where in the word the lowest bit of our ADC value is located
    int first_bit_position = (bits_per_adc * sample_no) % bits_per_word;

    // How many bits of our desired ADC are located in the `word_index`th word
    int bits_in_first_word = std::min(bits_per_adc, bits_per_word - first_bit_position);
    adcdata.samples_info[word_index] |= (new_adc_val << first_bit_position);

    // If we didn't put the full 14 bits in this word, we need to put the rest in the next word
    if (bits_in_first_word < bits_per_adc) {
      assert(word_index + 1 < num_adc_words);
      adcdata.samples_info[word_index + 1] |= new_adc_val >> bits_in_first_word;
    }
  }

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
