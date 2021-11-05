/**
 * @file module.cpp
 *
 * This is part of the DUNE DAQ Software Suite, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace dunedaq {
namespace detdataformats {
namespace python {

    extern void register_wib(py::module &);
    extern void register_ssp(py::module &);

PYBIND11_MODULE(_daq_detdataformats_py, m) {

    m.doc() = "c++ implementation of the dunedaq detdataformats modules"; // optional module docstring

    register_wib(m);
    register_ssp(m);
}

} // namespace python
} // namespace detdataformats
} // namespace dunedaq