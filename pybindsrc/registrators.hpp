/**
 * @file registrators.hpp
 *
 * Declaration of functions to register Python bindings to C++ objects
 *
 * This is part of the DUNE DAQ Software Suite, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */


#ifndef DETDATAFORMATS_PYBINDSRC_REGISTRATORS_HPP_
#define DETDATAFORMATS_PYBINDSRC_REGISTRATORS_HPP_

#include <pybind11/pybind11.h>

namespace dunedaq::detdataformats::python {

  void register_detid(pybind11::module&);
  void register_daqheader(pybind11::module&);
  void register_daqethheader(pybind11::module&);
  void register_hsi(pybind11::module&);
}

#endif // DETDATAFORMATS_PYBINDSRC_REGISTRATORS_HPP_
