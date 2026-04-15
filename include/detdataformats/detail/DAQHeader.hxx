
namespace dunedaq::detdataformats {

  static_assert(std::endian::native == std::endian::little);

  static_assert(std::is_trivially_copyable_v<DAQHeader>, "DAQHeader isn't trivially copyable and can't be safely std::memcpy'd");
  static_assert(std::is_standard_layout_v<DAQHeader>, "DAQHeader isn't standard layout; reinterpret_cast and offsetof can't safely be used with it");

  static_assert(sizeof(DAQHeader) == 12);
  static_assert(offsetof(DAQHeader, timestamp_1) == 4, "timestamp_1 field not at expected offset");
  static_assert(offsetof(DAQHeader, timestamp_2) == 8, "timestamp_2 field not at expected offset");

} // namespace dunedaq::detdataformats
