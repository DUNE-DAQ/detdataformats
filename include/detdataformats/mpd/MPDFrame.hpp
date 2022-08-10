/**
 * @file MPDFrame.hpp MPD 
 *
 * Contains declaration of MPDFrame. This class access raw MPD frames as produced by the MPD boards 
 *
 * Dataformat: https://docs.google.com/spreadsheets/d/1b9ohCndIZcYHEPiRDz4TMbaf6CJNlXrEuMnt3XS2BZ8/edit#gid=1694585828
 * 
 * This is part of the DUNE DAQ , copyright 2022.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef DETDATAFORMATS_INCLUDE_DATAFORMATS_MPDFRAME_HPP_
#define DETDATAFORMATS_INCLUDE_DATAFORMATS_MPDFRAME_HPP_

#include <bitset>
#include <iostream>
#include <stdint.h>
#include <vector>

namespace dunedaq {
namespace detdataformats {
namespace mpd {

/**
 * @brief MPD frame
 */
class MPDFrame
{
  public:

  typedef uint32_t word_t ; // NOLINT(build/unsigned)
  static constexpr int num_blocks = 1 ; // only one for now
  static constexpr int num_adc_samples = 2048 ;
  
  struct MPDHeader {
    uint32_t timestamp_sync = 0x3f60b8a8 ;
    uint32_t lenght = 0x00000008 ;
    uint64_t timestamp ; // timestamp taken from the operating system
    uint32_t SyncMagic = 0x2A502A50 ;
    uint32_t length ;
    uint32_t event_num ; 
  };
    
  struct MPDDeviceEventBlock {
    //MpdDeviceHeader
    uint32_t device_serial_num : 32 ;
    uint32_t device_model_id : 8 , length: 24 ; 
    //MStreamTriggerHeader
    uint32_t trigger_type: 2, trigger_length: 22, trigger_channel_number: 8 ; 
    //Trigger Data
    word_t event_timestamp_1: 32 ; 
    word_t event_timestamp_2: 30, flags: 2 ; 
    uint64_t channel_bit_mask ; 
    //Data header
    word_t type: 2;
    word_t data_length: 32 ; 
    word_t channel_number: 8 ;
    //MStream Data
    uint16_t _null[4]; // unused words
    uint16_t data[num_adc_samples];
  };  

  // =======================================================
  // Data members 
  // =======================================================
  MPDHeader header ; 
  MPDDeviceEventBlock block[num_blocks] ;

  uint16_t get_sample( const unsigned int block_id, const unsigned i ) const {

    if ( block_id >= num_blocks ) throw std::out_of_range("Block index out of range");
    if ( i >= num_adc_samples   ) throw std::out_of_range("ADC index out of range");
    else if ( i >= 2*block[block_id].data_length ) return 0;

    return block[block_id].data[i];
  }

  void set_adc(const unsigned int block_id, const unsigned int i, uint16_t val) {

    if ( block_id >= num_blocks ) throw std::out_of_range("Block index out of range");
    if ( i >= num_adc_samples   ) throw std::out_of_range("ADC index out of range");

    block[block_id].data[i] = val ; 
    return; 
  }


  /** @brief Get the 64-bit timestamp of the frame
   */
  uint64_t get_timestamp() const {
    return (uint64_t)block[0].event_timestamp_1 | ((uint64_t)block[0].event_timestamp_2 << 32 ) ; 
  }

};

} // namespace mpd
} // namespace detdataformats
} // namespace dunedaq

#endif // DETDATAFORMATS_INCLUDE_DATAFORMATS_MPDFRAME_HPP_
