/**
 * @file PACMANFrame.hpp PACMAN bit fields and accessors (ND LAr).
 *
 * This is part of the DUNE DAQ , copyright 2021.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */
#ifndef DETDATAFORMATS_INCLUDE_DATAFORMATS_PACMANFRAME_HPP_
#define DETDATAFORMATS_INCLUDE_DATAFORMATS_PACMANFRAME_HPP_

#include <bitset>
#include <iostream>
#include <cstdint>
#include <vector>

namespace dunedaq {
namespace detdataformats {
namespace pacman {

using word_t = uint32_t; // NOLINT(build/unsigned)

/**
 * @brief PACMAN frame
 */
class PACMANFrame
{
  public:
  static constexpr int WORD_LEN = 16;  // bytes
  static constexpr int HEADER_LEN = 8; // bytes

  enum msg_type { // message type declarations
    DATA_MSG = 0x44,
    REQ_MSG = 0x3F,
    REP_MSG = 0x21
  };
  enum word_type { // word type declarations
    DATA_WORD  = 0x44,
    TRIG_WORD  = 0x54,
    SYNC_WORD  = 0x53,
    PING_WORD  = 0x50,
    WRITE_WORD = 0x57,
    READ_WORD = 0x52,
    TX_WORD = 0x44,
    ERR_WORD = 0x45
  };
  enum packet_type
  { // packet type declarations
    DATA_PACKET = 0x0,
    CONFIG_WRITE_PACKET = 0x2,
    CONFIG_READ_PACKET = 0x3
  };

  struct __attribute__((__packed__)) PACMANMessageHeader
  {
    msg_type type:8; // message type indicator
    uint32_t unix_ts; // unix timestamp from PACMAN dataserver
    uint8_t _null; // unused
    uint16_t words; // number of words in message
  };

  union LArPixPacket
  {
    struct __attribute__((__packed__)) LArPixGenericPacket
    { // A generic type applicable to all LArPix packets
      packet_type type:2; // packet type indicator
      uint64_t _null:62; // packet data (specific for each packet type)
    } packet;

    struct __attribute__((__packed__)) LArPixDataPacket
    { // Specific for LArPix data packets
      packet_type type:2; // packet type indicator
      uint16_t chipid:8; // asic chip id
      uint16_t channelid:6; // asic channel id
      uint32_t timestamp:31; // packet timestamp
      bool first_packet:1; // first packet indicator
      uint16_t dataword:8; // adc value
      uint16_t trigger_type:2; // trigger type indicator
      uint16_t local_fifo_status:2; // local fifo flag
      uint16_t shared_fifo_status:2; // shared fifo flag
      bool downstream_marker:1; // upstream/downstream indicator
      bool parity_bit:1; // packet parity
    } data_packet;

    struct LArPixConfigWritePacket { /* not implemented */ } write_packet;
    struct LArPixConfigReadPacket { /* not implemented */ } read_packet;
  };

  union PACMANMessageWord
  {
    struct __attribute__((__packed__)) PACMANGenericWord
    { // A generic type applicable to all PACMAN message words
      word_type type:8; // word type indicator
      uint8_t _null[15]; // word data (specific for each word type)
    } word;

    struct __attribute__((__packed__)) PACMANDataWord
    { // Specific for PACMAN data words
      word_type type:8; // word type indicator
      uint8_t channel_id; // uart channel number
      uint32_t receipt_timestamp; // PACMAN timestamp when LArPix word received
      uint8_t _null[2]; // unused
      LArPixPacket larpix_word; // LArPix data
    } data_word;

    struct PACMANTriggerWord { /* not implemented */ } trig_word;
    struct PACMANSyncWord { /* not implemented */ } sync_word;
  };

  PACMANMessageHeader* get_msg_header(void* msg) const
  {
    return static_cast<PACMANMessageHeader*>(msg);
  }

  PACMANMessageWord* get_msg_word(void* msg, const uint32_t i) const
  {
    return reinterpret_cast<PACMANMessageWord*>(static_cast<uint8_t*>(msg) + HEADER_LEN + WORD_LEN * i);
  }

  friend std::ostream& operator<<(std::ostream& o, PACMANFrame const& frame);

private:
};

} // namespace pacman
} // namespace detdataformats
} // namespace dunedaq

#endif // DETDATAFORMATS_INCLUDE_DATAFORMATS_PACMANFRAME_HPP_
