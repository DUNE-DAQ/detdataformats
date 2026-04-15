
namespace dunedaq::detdataformats {

  static_assert(std::endian::native == std::endian::little);

  static_assert(std::is_trivially_copyable_v<DAQEthHeader>, "DAQEthHeader isn't trivially copyable and can't be safely std::memcpy'd");
  static_assert(std::is_standard_layout_v<DAQEthHeader>, "DAQEthHeader isn't standard layout; reinterpret_cast and offsetof can't safely be used with it");

  static_assert(sizeof(DAQEthHeader) == 16);
  static_assert(offsetof(DAQEthHeader, timestamp) == 8, "timestamp field not at expected offset");

} // namespace dunedaq::detdataformats
