/**
 * @file hsi.cpp
 *
 * This is part of the DUNE DAQ Software Suite, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */
#include "detdataformats/hsi/TimingHSIFrame.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace dunedaq {
namespace detdataformats {
namespace hsi {
namespace python {

void
register_hsi(py::module& m)
{

  py::class_<TimingHSIFrame> (m, "TimingHSIFrame", py::buffer_protocol())
    .def(py::init())
    .def(py::init([](py::capsule capsule) {
        auto hsfp = *static_cast<TimingHSIFrame*>(capsule.get_pointer());
        return hsfp;
    } ))
    .def("get_timestamp", &TimingHSIFrame::get_timestamp)
  ;
}

} // namespace python
} // namespace hsi
} // namespace detdataformats
} // namespace dunedaq
