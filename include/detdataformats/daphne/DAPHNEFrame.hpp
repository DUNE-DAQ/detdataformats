/**
 * @file DAPHNEFrame.hpp
 *
 *  Contains declaration of DAPHNEFrame, a class for accessing raw DAPHNE frames, as produced by the DAPHNE boards
 *
 *  The canonical definition of the PDS DAPHNE format is given in EDMS document 2088726:
 *  https://edms.cern.ch/document/2088726/3
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef DETDATAFORMATS_INCLUDE_DATAFORMATS_DAPHNE_DAPHNEFRAME_HPP_
#define DETDATAFORMATS_INCLUDE_DATAFORMATS_DAPHNE_DAPHNEFRAME_HPP_

#include <algorithm> // For std::min
#include <cassert>   // For assert()
#include <cstdio>
#include <cstdlib>
#include <stdexcept> // For std::out_of_range
#include <cstdint>  // For uint32_t etc

namespace dunedaq {
namespace detdataformats {
namespace daphne {

class DAPHNEFrame
{
public:
  // ===============================================================
  // Preliminaries
  // ===============================================================

  // The definition of the format is in terms of 32-bit words
  typedef uint32_t word_t; // NOLINT

  static constexpr int s_bits_per_adc = 14;
  static constexpr int s_bits_per_word = 8 * sizeof(word_t);
  static constexpr int s_t_channels_per_daphne = 320;
  static constexpr int s_channels_per_daphne = s_t_channels_per_daphne;
  static constexpr int s_daphnes_per_frame = 1;
  static constexpr int s_num_channels = s_daphnes_per_frame * s_channels_per_daphne;
  static constexpr int s_num_adc_words = s_num_channels * s_bits_per_adc / s_bits_per_word;

  struct Header
  {
    word_t start_frame;
    word_t data_version : 4, daphne : 8, channel : 6, trigger_peak_height : 14;
    word_t wf_length_in_words : 12, pds_reserved_bits : 20;
    word_t packet_counter : 32;
    word_t timestamp_wf_1 : 32;
    word_t timestamp_wf_2 : 32;
  };

  struct Trailer
  {
    word_t crc20 : 20;
    word_t flex_word_12 : 12;
    word_t eof : 8;
    word_t flex_word_24 : 24;
  };

  // ===============================================================
  // Data members
  // ===============================================================
  Header header;
  word_t adc_words[s_num_adc_words]; // NOLINT
  
  // THIS DATA IS STRIPPED DURING TRANSMISSION
  // Trailer trailer;

  // ===============================================================
  // Accessors
  // ===============================================================

  /**
   * @brief Get the ith ADC value in the frame
   *
   * The ADC words are 14 bits long, stored packed in the data structure. The order is:
   *
   * - 320 values from DAPHNE T channels
   */
  uint16_t get_adc(int i) const // NOLINT
  {
    if (i < 0 || i >= s_num_channels)
      throw std::out_of_range("ADC index out of range");

    // The index of the first (and sometimes only) word containing the required ADC value
    int word_index = s_bits_per_adc * i / s_bits_per_word;
    assert(word_index < s_num_adc_words);
    // Where in the word the lowest bit of our ADC value is located
    int first_bit_position = (s_bits_per_adc * i) % s_bits_per_word;
    // How many bits of our desired ADC are located in the `word_index`th word
    int bits_from_first_word = std::min(s_bits_per_adc, s_bits_per_word - first_bit_position);
    uint16_t adc = adc_words[word_index] >> first_bit_position; // NOLINT
    // If we didn't get the full 14 bits from this word, we need the rest from the next word
    if (bits_from_first_word < s_bits_per_adc) {
      assert(word_index + 1 < s_num_adc_words);
      adc |= adc_words[word_index + 1] << bits_from_first_word;
    }
    // Mask out all but the lowest 14 bits;
    return adc & 0x3FFFu;
  }

  /**
   * @brief Set the ith ADC value in the frame to @p val
   */
  void set_adc(int i, uint16_t val) // NOLINT
  {
    if (i < 0 || i >= s_num_channels)
      throw std::out_of_range("ADC index out of range");
    if (val >= (1 << s_bits_per_adc))
      throw std::out_of_range("ADC value out of range");

    // The index of the first (and sometimes only) word containing the required ADC value
    int word_index = s_bits_per_adc * i / s_bits_per_word;
    assert(word_index < s_num_adc_words);
    // Where in the word the lowest bit of our ADC value is located
    int first_bit_position = (s_bits_per_adc * i) % s_bits_per_word;
    // How many bits of our desired ADC are located in the `word_index`th word
    int bits_in_first_word = std::min(s_bits_per_adc, s_bits_per_word - first_bit_position);
    uint32_t mask = (1 << (first_bit_position)) - 1;
    adc_words[word_index] = ((val << first_bit_position) & ~mask) | (adc_words[word_index] & mask);
    // If we didn't put the full 14 bits in this word, we need to put the rest in the next word
    if (bits_in_first_word < s_bits_per_adc) {
      assert(word_index + 1 < s_num_adc_words);
      mask = (1 << (s_bits_per_adc - bits_in_first_word)) - 1;
      adc_words[word_index + 1] = ((val >> bits_in_first_word) & mask) | (adc_words[word_index + 1] & ~mask);
    }
  }

  /** @brief Get the ith T ADC in the given DAPHNE
   */
  uint16_t get_t(int i) const { return get_adc(i); } // NOLINT(build/unsigned)

  /** @brief Set the ith U-channel ADC in the given femb to val
   */
  void set_t(int i, uint16_t val) { return set_adc(i, val); } // NOLINT(build/unsigned)

  /** @brief Get the 64-bit timestamp of the frame
   */
  uint64_t get_timestamp() const // NOLINT(build/unsigned)
  {
    return (uint64_t)header.timestamp_wf_1 | ((uint64_t)header.timestamp_wf_2 << 32); // NOLINT(build/unsigned)
  }
};

} // namespace daphne
} // namespace detdataformats
} // namespace dunedaq

#endif // DETDATAFORMATS_INCLUDE_DATAFORMATS_DAPHNE_DAPHNEFRAME_HPP_

// Local Variables:
// c-basic-offset: 2
// End:
