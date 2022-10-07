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
#include <fmt/format.h>

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
  static constexpr unsigned int num_adc_samples = 2048 ;
  
  struct MPDHeader {
    word_t timestamp_sync = 0x3f60b8a8 ;
    word_t lenght = 0x00000008 ;
    uint64_t timestamp_OS ; // timestamp taken from the operating system
    word_t SyncMagic = 0x2A502A50 ;
    word_t length ;
    word_t event_num ; 
    // THIS SHOULD NOT BE HERE
    //MpdDeviceHeader
    word_t device_serial_num ;
    word_t device_length : 24, device_model_id : 8;  
    //MStreamTriggerHeader
    word_t trigger_type: 2, trigger_length: 22, trigger_channel_number: 8 ; 
    //Trigger Data
    word_t event_timestamp_1: 32 ; 
    word_t flags: 2, event_timestamp_2: 30 ; 
    uint64_t channel_bit_mask ; 
    //Data header
    word_t data_type: 2, data_length: 22, channel_number: 8 ;

  };
    

  // =======================================================
  // Data members 
  // =======================================================
  MPDHeader header ; 
  //MStream Data
  uint64_t _null; // unused words
  uint16_t data[num_adc_samples];

  uint16_t get_sample( const unsigned i ) const {
    if ( i >= num_adc_samples   ) throw std::out_of_range("Index out of range");
    return data[i];
  }

  void set_value( const unsigned int i, uint16_t val ) {
    if ( i >= num_adc_samples   ) throw std::out_of_range("Index out of range");
    data[i] = val ; 
    return; 
  }

  /** @brief Get the 64-bit timestamp of the frame
   */
  uint64_t get_timestamp() const {
    // in nanoseconds
    return (uint64_t)header.event_timestamp_1 | ((uint64_t)header.event_timestamp_2 << 32 ) ; 
  }

  /** Adding some helper functions **/
  void print_content_header() const {
    TLOG_DEBUG(1) << " Time stamp syncronization number "<< fmt::format("{:#x}", header.timestamp_sync );// 0x3f60b8a8	 
    TLOG_DEBUG(1) << " Time stamp Operation System "<< header.timestamp_OS ;
    TLOG_DEBUG(1) << " Sync Magic = " << fmt::format("{:#x}", header.SyncMagic) ; //0x2A502A50
    TLOG_DEBUG(1) << " Length = " << header.length ; 
    TLOG_DEBUG(1) << " Event Number = "<< header.event_num ; 
    TLOG_DEBUG(1) << " Device serial number = " << header.device_serial_num ; 
    TLOG_DEBUG(1) << " Block length = " << header.device_length ;
    TLOG_DEBUG(1) << " Device model ID = " << header.device_model_id ;
    TLOG_DEBUG(1) << " Trigger Type = " << header.trigger_type ;
    TLOG_DEBUG(1) << " Trigger Length = " << header.trigger_length ; 
    TLOG_DEBUG(1) << " Trigger channel number = " << header.trigger_channel_number ; 
    TLOG_DEBUG(1) << " Event Timestamp 1 = "<< header.event_timestamp_1 ; 
    TLOG_DEBUG(1) << " Event Timestamp 2 = " << header.event_timestamp_2 ; 
    TLOG_DEBUG(1) << " Flags = " << header.flags ; 
    TLOG_DEBUG(1) << " Channel bit mask = " << header.channel_bit_mask ; 
    TLOG_DEBUG(1) << " Type of data = " << header.data_type ; 
    TLOG_DEBUG(1) << " Data length = " << header.data_length ; 
    TLOG_DEBUG(1) << " Channel number = " << header.channel_number ;  
  }

  void print_content_data(const unsigned int id) const {
    TLOG_DEBUG(1) << " Data " << id << ": " << data[id] ; 
  }
 
  void print_content_data() const {
    for ( unsigned int i = 0 ; i < num_adc_samples ; ++i ) {
      print_content_data(i);
    }
  }  
};

} // namespace mpd
} // namespace detdataformats
} // namespace dunedaq

#endif // DETDATAFORMATS_INCLUDE_DATAFORMATS_MPDFRAME_HPP_
