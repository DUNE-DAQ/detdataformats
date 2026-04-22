
namespace dunedaq::detdataformats {

  static_assert(std::endian::native == std::endian::little, "The DAQEthHeader bitfield layout assumes little-endian architecture");

  static_assert(std::is_trivially_copyable_v<DAQEthHeader>, "DAQEthHeader isn't trivially copyable and can't be safely std::memcpy'd");
  static_assert(std::is_standard_layout_v<DAQEthHeader>, "DAQEthHeader isn't standard layout; reinterpret_cast and offsetof can't safely be used with it");

  static_assert(sizeof(DAQEthHeader) == 16, "DAQEthHeader not the expected size");
  static_assert(offsetof(DAQEthHeader, timestamp) == 8, "timestamp field not at expected offset");

  inline std::ostream&
  operator<<(std::ostream& o, DAQEthHeader const& h)
  {
    return o << "Version:" << unsigned(h.version) << " DetID:" << unsigned(h.det_id) << " CrateID:" << unsigned(h.crate_id)
           << " SlotID:" << unsigned(h.slot_id) << " StreamID:" << unsigned(h.stream_id)
           << " SequenceID: " << unsigned(h.seq_id) << " Block length: " << unsigned(h.block_length)
	   << " Timestamp: " << h.get_timestamp() << '\n';
  }

} // namespace dunedaq::detdataformats
