/**
 *  * @file toad.cpp Python bindings for the TOADFrame format
 *   *
 *    * This is part of the DUNE DAQ Software Suite, copyright 2020.
 *     * Licensing/copyright details are in the COPYING file that you should have
 *      * received with this code.
 *       */

#include "detdataformats/toad/TOADFrameOverlay.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <cstdint>
#include <cstddef>

namespace py = pybind11;

namespace dunedaq {
namespace detdataformats {
namespace toad {
namespace python {

void
register_toad(py::module& m)
{

  py::class_<TOADFrameOverlay>(m, "TOADFrameOverlay", py::buffer_protocol())
    .def(py::init())
    .def(py::init([](py::capsule capsule) {
        auto wfp = *static_cast<TOADFrameOverlay*>(capsule.get_pointer());
        return wfp;
    } ))
    .def_property_readonly("tstmp", [](TOADFrameOverlay& self) -> uint64_t {return self.tstmp;})
    .def_property_readonly("fec", [](TOADFrameOverlay& self) -> uint32_t {return self.fec;})
    .def_property_readonly("hdr_par_check", [](TOADFrameOverlay& self) -> uint32_t {return self.hdr_par_check;})
    .def_property_readonly("n_samples", [](TOADFrameOverlay& self) -> uint32_t {return self.n_samples;});
}

} // namespace python
} // namespace toad
} // namespace detdataformats
} // namespace dunedaq
