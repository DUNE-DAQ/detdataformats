/**
 * @file WIB2Frame.hpp
 *
 * Contains declaration of WIB2Frame, a class for accessing raw WIB v2 frames, as used in ProtoDUNE-SP-II * *  The
 * canonical definition of the WIB format is given in EDMS document 2088713: *  https://edms.cern.ch/document/2088713/4
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef DETDATAFORMATS_INCLUDE_DATAFORMATS_WIB2_WIB2FRAME_HPP_
#define DETDATAFORMATS_INCLUDE_DATAFORMATS_WIB2_WIB2FRAME_HPP_

#include <algorithm> // For std::min
#include <cassert>   // For assert()
#include <cstdint>   // For uint32_t etc
#include <cstdio>
#include <cstdlib>
#include <stdexcept> // For std::out_of_range

namespace dunedaq {
namespace detdataformats {
namespace wib2 {

/**
 *  @brief Class for accessing raw WIB v2 frames, as used in ProtoDUNE-SP-II
 *
 *  The canonical definition of the WIB format is given in EDMS document 2088713:
 *  https://edms.cern.ch/document/2088713/4
 */
class WIB2Frame
{
public:
  // ===============================================================
  // Preliminaries
  // ===============================================================

  // The definition of the format is in terms of 32-bit words
  typedef uint32_t word_t; // NOLINT

  static constexpr int s_bits_per_adc = 14;
  static constexpr int s_bits_per_word = 8 * sizeof(word_t);
  static constexpr int s_u_channels_per_femb = 40;
  static constexpr int s_v_channels_per_femb = 40;
  static constexpr int s_x_channels_per_femb = 48;
  static constexpr int s_channels_per_femb = s_u_channels_per_femb + s_v_channels_per_femb + s_x_channels_per_femb;
  static constexpr int s_fembs_per_frame = 2;
  static constexpr int s_num_channels = s_fembs_per_frame * s_channels_per_femb;
  static constexpr int s_num_adc_words = s_num_channels * s_bits_per_adc / s_bits_per_word;

  struct Header
  {
    word_t version : 6, detector_id : 6, crate : 10, slot : 4, link : 6;
    word_t timestamp_1 : 32;
    word_t timestamp_2 : 32;
    word_t tbd_1 : 13, colddata_timestamp_id : 3, femb_valid : 2, link_mask : 8, lock_output_status : 1, tbd_2 : 5;
    word_t femb_pulser_frame_bits : 8, femb_sync_flags : 8, colddata_timestamp : 15, tbd_3 : 1;
  };

  struct Trailer
  {
    word_t flex_bits : 16, tbd_1 : 1, tbd_2 : 1, ws : 1, psr_cal : 4, ready : 1, context_code : 8;
  };

  // ===============================================================
  // Data members
  // ===============================================================
  Header header;
  word_t adc_words[s_num_adc_words]; // NOLINT
  Trailer trailer;

  // ===============================================================
  // Accessors
  // ===============================================================

  /**
   * @brief Get the ith ADC value in the frame
   *
   * The ADC words are 14 bits long, stored packed in the data structure. The order is:
   *
   * - 40 values from FEMB0 U channels
   * - 40 values from FEMB0 V channels
   * - 48 values from FEMB0 X channels (collection)
   * - 40 values from FEMB1 U channels
   * - 40 values from FEMB1 V channels
   * - 48 values from FEMB1 X channels (collection)
   */
  uint16_t get_adc(int i) const // NOLINT(build/unsigned)
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
    uint16_t adc = adc_words[word_index] >> first_bit_position; // NOLINT(build/unsigned)
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
  void set_adc(int i, uint16_t val) // NOLINT(build/unsigned)
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

  /** @brief Get the ith U-channel ADC in the given femb
   */
  uint16_t get_u(int femb, int i) const { return get_adc(get_adc_index(kU, femb, i)); } // NOLINT(build/unsigned)

  /** @brief Get the ith V-channel ADC in the given femb
   */
  uint16_t get_v(int femb, int i) const { return get_adc(get_adc_index(kV, femb, i)); } // NOLINT(build/unsigned)

  /** @brief Get the ith X-channel (ie, collection) ADC in the given femb
   */
  uint16_t get_x(int femb, int i) const { return get_adc(get_adc_index(kX, femb, i)); } // NOLINT(build/unsigned)

  /** @brief Set the ith U-channel ADC in the given femb to val
   */
  void set_u(int femb, int i, uint16_t val) // NOLINT(build/unsigned)
  {
    return set_adc(get_adc_index(kU, femb, i), val);
  }

  /** @brief Set the ith V-channel ADC in the given femb to val
   */
  void set_v(int femb, int i, uint16_t val) // NOLINT(build/unsigned)
  {
    return set_adc(get_adc_index(kV, femb, i), val);
  }

  /** @brief Set the ith X-channel (ie, collection) ADC in the given femb to val
   */
  void set_x(int femb, int i, uint16_t val) // NOLINT(build/unsigned)
  {
    return set_adc(get_adc_index(kX, femb, i), val);
  }

  /** @brief Get the 64-bit timestamp of the frame
   */
  uint64_t get_timestamp() const // NOLINT(build/unsigned)
  {
    return (uint64_t)header.timestamp_1 | ((uint64_t)header.timestamp_2 << 32); // NOLINT(build/unsigned)
  }

private:
  enum View
  {
    kU,
    kV,
    kX
  };

  int get_adc_index(View view, int femb, int i) const
  {
    if (femb < 0 || femb >= s_fembs_per_frame) {
      throw std::out_of_range("FEMB index out of range");
    }

    int offset = 0;
    int n_channels = 0;

    switch (view) {
      case kU:
        offset = 0;
        n_channels = s_u_channels_per_femb;
        break;
      case kV:
        offset = s_u_channels_per_femb;
        n_channels = s_v_channels_per_femb;
        break;
      case kX:
        offset = s_u_channels_per_femb + s_v_channels_per_femb;
        n_channels = s_x_channels_per_femb;
        break;
    }

    if (i < 0 || i >= n_channels) {
      throw std::out_of_range("Channel index out of range");
    }

    return s_channels_per_femb * femb + offset + i;
  }
};

} // namespace wib2
} // namespace detdataformats
} // namespace dunedaq

#endif // DETDATAFORMATS_INCLUDE_DATAFORMATS_WIB2_WIB2FRAME_HPP_

// Local Variables:
// c-basic-offset: 2
// End:
