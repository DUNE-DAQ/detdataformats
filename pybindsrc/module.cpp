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

namespace wib {
namespace python {
extern void register_wib(py::module &);
}
}

namespace wib2 {
namespace python {
  extern void register_wib2(py::module &);
}
}

namespace daphne {
namespace python {
  extern void register_daphne(py::module &);
}
}

namespace ssp {
namespace python {
extern void register_ssp(py::module &);    
}
}

namespace python {

PYBIND11_MODULE(_daq_detdataformats_py, m) {

    m.doc() = "c++ implementation of the dunedaq detdataformats modules"; // optional module docstring

    py::module_ wib_module = m.def_submodule("wib");
    wib::python::register_wib(wib_module);

    py::module_ wib2_module = m.def_submodule("wib2");
    wib2::python::register_wib2(wib2_module);

    py::module_ daphne_module = m.def_submodule("daphne");
    daphne::python::register_daphne(daphne_module);

    py::module_ ssp_module = m.def_submodule("ssp");
    ssp::python::register_ssp(ssp_module);
}

} // namespace python
} // namespace detdataformats
} // namespace dunedaq
