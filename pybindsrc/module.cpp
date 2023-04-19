/**
 * @file module.cpp
 *
 * This is part of the DUNE DAQ Software Suite, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "submodules.hpp"

namespace py = pybind11;

namespace dunedaq {
namespace detdataformats {
namespace python {

PYBIND11_MODULE(_daq_detdataformats_py, m) {

    m.doc() = "c++ implementation of the dunedaq detdataformats modules"; // optional module docstring

    python::register_detid(m);
    python::register_daqheader(m);
    python::register_daqethheader(m);

    py::module_ trigger_primitive_module = m.def_submodule("trigger_primitive");
    trigger::python::register_trigger_primitive(trigger_primitive_module);

    py::module_ hsi_module = m.def_submodule("hsi");
    hsi::python::register_hsi(hsi_module);

}

} // namespace python
} // namespace detdataformats
} // namespace dunedaq
