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

namespace wib {
namespace python {
extern void register_wib(pybind11::module &);
}
}  // namespace wib

namespace wib2 {
namespace python {
extern void register_wib2(pybind11::module &);
}
}  // namespace wib2

namespace wibeth {
namespace python {
extern void register_wibeth(pybind11::module &);
}
} // namespace wibeth

namespace daphne {
namespace python {
extern void register_daphne(pybind11::module &);
}
}  // namespace daphne

namespace ssp {
namespace python {
extern void register_ssp(pybind11::module &);
}
}  // namespace ssp

namespace trigger {
namespace python {
extern void register_trigger_primitive(pybind11::module &);
}
}  // namespace trigger

namespace hsi {
namespace python {
extern void register_hsi(pybind11::module &);
}
}  // namespace hsi


namespace tde {
namespace python {
extern void register_tde(pybind11::module &);
}
}  // namespace tde

namespace mpd {
namespace python {
extern void register_mpd(pybind11::module &);    
}
}  // namespace mpd

}  // namespace detdataformats
}  // namespace dunedaq
#endif /* __DUNEDAQ_DETDATAFORMATS_PYBINDSRC_SUBMODULES_HPP__ */
