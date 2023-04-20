#ifndef __DUNEDAQ_DETDATAFORMATS_PYBINDSRC_SUBMODULES_HPP__
#define __DUNEDAQ_DETDATAFORMATS_PYBINDSRC_SUBMODULES_HPP__ 

#include <pybind11/pybind11.h>

namespace dunedaq {
namespace detdataformats {

namespace python {
void register_detid(pybind11::module &);
void register_daqheader(pybind11::module &);
void register_daqethheader(pybind11::module &);
}  // namespace python

namespace hsi {
namespace python {
extern void register_hsi(pybind11::module &);
}
}  // namespace hsi

}  // namespace detdataformats
}  // namespace dunedaq
#endif /* __DUNEDAQ_DETDATAFORMATS_PYBINDSRC_SUBMODULES_HPP__ */
