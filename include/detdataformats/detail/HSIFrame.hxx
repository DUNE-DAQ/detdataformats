
#include <cstddef>
#include <type_traits>
#include <bit>

namespace dunedaq::detdataformats {

  static_assert(std::endian::native == std::endian::little);

  static_assert(std::is_standard_layout_v<HSIFrame>, "HSIFrame isn't standard layout; reinterpret_cast and offsetof can't safely be used with it");

  static_assert(sizeof(HSIFrame) == 28);
  static_assert(offsetof(HSIFrame, timestamp_low) == 4, "timestamp_low field not at expected offset");
  static_assert(offsetof(HSIFrame, timestamp_high) == 8, "timestamp_high field not at expected offset");
  static_assert(offsetof(HSIFrame, input_low) == 12, "input_low field not at expected offset");
  static_assert(offsetof(HSIFrame, input_high) == 16, "input_high field not at expected offset");
  static_assert(offsetof(HSIFrame, trigger) == 20, "trigger field not at expected offset");
  static_assert(offsetof(HSIFrame, sequence) == 24, "sequence field not at expected offset");
  
} // namespace dunedaq::detdataformats
