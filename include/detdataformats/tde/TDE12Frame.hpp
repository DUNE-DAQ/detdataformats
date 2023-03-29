/**
 * @file TDE12Frame.hpp TDE 12 bit fields and accessors
 *
 * This is part of the DUNE DAQ, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */
#ifndef DETDATAFORMATS_INCLUDE_DATAFORMATS_TDE_TDE12FRAME_HPP_
#define DETDATAFORMATS_INCLUDE_DATAFORMATS_TDE_TDE12FRAME_HPP_

#include <bitset>
#include <iostream>
#include <vector>
#include <stdexcept> 
#include <cassert> 
#include <algorithm> 
#include <cmath>
#include <cstdint>   
#include <cstdio>
#include <cstdlib>

namespace dunedaq {
namespace detdataformats {
namespace tde {

static constexpr int ticks_between_adc_samples = 32;
static constexpr int tot_adc12_samples = 5960;
static constexpr int bits_per_adc = 12;
static constexpr int n_channels_per_amc = 64;
static constexpr int bits_per_word = 8 * sizeof(uint32_t);
//static constexpr int tot_num_words = tot_adc12_samples * bits_per_adc/bits_per_word;
static constexpr int tot_num_words = 2235;
static constexpr int payload12 = 8964;

struct TDE12Header
{
  uint32_t version : 6, det_id : 6, crate : 10, slot : 4, link : 6;
  uint32_t timestamp_1 : 32;
  uint32_t timestamp_2 : 32;
  uint32_t TAItime_1 : 32;
  uint32_t TAItime_2 : 32;
  uint32_t tde_header : 10, tde_errors : 22;

  uint64_t get_timestamp() const { return (uint64_t)(timestamp_1) | ((uint64_t)(timestamp_2) << 32); }
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
    return o << std::hex << " version: " << version << " det_id: " << det_id << " crate: " << crate << " slot: " << slot << " link: " << link 
                         << "timestamp: " << get_timestamp() 
                         << "TAItime: " << get_TAItime() 
                         << "tde_header: " << tde_header << " tde_errors: " << tde_errors << std::dec << '\n';
  }

  std::ostream& print_bits(std::ostream& o) const
  {
    return o << "version: " << std::bitset<6>(version) << " det_id: " << std::bitset<6>(det_id) << " crate: " << std::bitset<10>(crate) << " slot: " << std::bitset<4>(slot) << " link: " << std::bitset<6>(link) 
             << "timestamp: " << get_timestamp() 
             << "TAItime: " << get_TAItime() 
             << "tde_header:" << std::bitset<10>(tde_header) << " tde_errors:" << std::bitset<16>(tde_errors) << '\n';
  }
};

struct Wordset
{
  uint32_t sample_0 : 12, sample_1 : 12, sample_2_0 : 8;
  uint32_t sample_2_1 : 4, sample_3 : 12, sample_4 : 12, sample_5_0 : 4;
  uint32_t sample_5_1 : 8, sample_6 : 12, sample_7 : 12;
};

struct ADC12Data
{
  uint32_t adc_words[tot_num_words];

  uint16_t get_adc(int i) const
  {
    if (i < 0 || i >= tot_adc12_samples) { throw std::out_of_range("ADC sample index out of range"); }

    // The index of the first (and sometimes only) word containing the required ADC value
    int word_index = bits_per_adc * i / bits_per_word;
    assert(word_index < tot_num_words);
    // Where in the word the lowest bit of our ADC value is located
    int first_bit_position = (bits_per_adc * i) % bits_per_word;
    
    // How many bits of our desired ADC are located in the `word_index`th word
    int bits_from_first_word = std::min(bits_per_adc, bits_per_word - first_bit_position);
    uint16_t adc = adc_words[word_index] >> first_bit_position;
    
    // If we didn't get the full 12 bits from this word, we need the rest from the next word
    if (bits_from_first_word < bits_per_adc) 
    {
      assert(word_index + 1 < tot_num_words);
      adc |= adc_words[word_index + 1] << bits_from_first_word;
    }

    return adc& 0x0FFFu;
  }

  void set_adc(int i, const uint16_t val)
  { 
    if (i < 0 || i >= tot_adc12_samples)
      throw std::out_of_range("ADC sample index out of range");
    if (val >= (1 << bits_per_adc))
      throw std::out_of_range("ADC bits value out of range");
    
    // The index of the first (and sometimes only) word containing the required ADC value
    int word_index = bits_per_adc * i / bits_per_word;
    assert(word_index < tot_num_words);

    // Where in the word the lowest bit of our ADC value is located
    int first_bit_position = (bits_per_adc * i) % bits_per_word;

    // How many bits of our desired ADC are located in the `word_index`th word
    int bits_in_first_word = std::min(bits_per_adc, bits_per_word - first_bit_position);
    uint32_t mask = (1 << (first_bit_position)) - 1;
    adc_words[word_index] = (( val << first_bit_position) & ~mask) | (adc_words[word_index] & mask);

    // If we didn't put the full 12 bits in this word, we need to put the rest in the next word
    if (bits_in_first_word < bits_per_adc) 
    {
      assert(word_index + 1 < tot_num_words);
      mask = (1 << (bits_per_adc - bits_in_first_word)) - 1;
      adc_words[word_index + 1] = (( val >> bits_in_first_word) & mask) | (adc_words[word_index + 1] & ~mask);
    }
  }

};

class TDE12Frame
{
public:
  const TDE12Header* get_tde_header() const { return &tde12header; }
  TDE12Header* get_tde_header() { return &tde12header; }

  // TDE12Header mutators
  void set_tde_errors(const uint16_t new_tde_errors) { tde12header.tde_errors = new_tde_errors; } 
  void set_timestamp(const uint64_t new_timestamp) { tde12header.set_timestamp(new_timestamp); } 
  void set_TAItime(const uint64_t new_TAItime) { tde12header.set_TAItime(new_TAItime); } 
  uint64_t get_timestamp() const { return tde12header.get_timestamp(); } 

  // ADC12Data mutators
  void set_adc(int i, const uint16_t val) {return adc12data.set_adc(i, val); }
  uint16_t get_adc(int i) { return adc12data.get_adc(i); }

  friend std::ostream& operator<<(std::ostream& o, TDE12Frame const& tde12frame);

private:
  TDE12Header tde12header;
  ADC12Data adc12data;
};

inline std::ostream&
operator<<(std::ostream& o, TDE12Header const& tde12header)
{
  return o << "version: " << unsigned(tde12header.version) << " det_id: " << unsigned(tde12header.det_id) << " crate: " << unsigned(tde12header.crate) << " slot: " << unsigned(tde12header.slot) << " link: " << unsigned(tde12header.link) 
           << "timestamp: " << tde12header.get_timestamp() 
           << "TAItime: " << tde12header.get_TAItime() 
           << "tde_header: " << unsigned(tde12header.tde_header) << " tde_errors: " << unsigned(tde12header.tde_errors) << '\n';
}

inline std::ostream&
operator<<(std::ostream& o, TDE12Frame const& tde12frame)
{
  o << "Printing frame:" << '\n';
  o << tde12frame.tde12header << '\n';
  return o;
}

} // namespace tde
} // namespace detdataformats
} // namespace dunedaq

#endif // DETDATAFORMATS_INCLUDE_DATAFORMATS_TDE_TDE12FRAME_HPP_
