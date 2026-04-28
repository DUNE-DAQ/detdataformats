
#include <bit>
#include <cstddef>
#include <type_traits>

namespace dunedaq::detdataformats {

static_assert(std::is_trivially_copyable_v<HSIFrame>,
              "HSIFrame isn't trivially copyable and can't be safely std::memcpy'd");
static_assert(std::is_standard_layout_v<HSIFrame>,
              "HSIFrame isn't standard layout; reinterpret_cast and offsetof can't safely be used with it");
static_assert(std::endian::native == std::endian::little,
              "The HSIFrame bitfield layout assumes little-endian architecture");

static_assert(sizeof(HSIFrame) == 28, "HSIFrame struct size different than expected!");
static_assert(offsetof(HSIFrame, timestamp_low) == 4, "HSIFrame timestamp_low field not at expected offset");
static_assert(offsetof(HSIFrame, timestamp_high) == 8, "HSIFrame timestamp_high field not at expected offset");
static_assert(offsetof(HSIFrame, input_low) == 12, "HSIFrame input_low field not at expected offset");
static_assert(offsetof(HSIFrame, input_high) == 16, "HSIFrame input_high field not at expected offset");
static_assert(offsetof(HSIFrame, trigger) == 20, "HSIFrame trigger field not at expected offset");
static_assert(offsetof(HSIFrame, sequence) == 24, "HSIFrame sequence field not at expected offset");

} // namespace dunedaq::detdataformats
