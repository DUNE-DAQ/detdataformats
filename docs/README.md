# DUNE Detectors Data Formats

This repository contains bitfields of detector raw data and utilities used to decode them. In general, each detector's data can be conveniently decoded via a class with the name of the form `<datatype>Frame` found in a public header file called `<datatype>Frame.hpp`, where values like timestamps and ADC values can be easily retrieved via getter functions. Additionally, for a subset of the detector data, Python utilities are available. 

For those who work on ProtoDUNE (otherwise skip this paragraph): this package is very similar to the `dune-raw-data` package which contains overlay classes for ProtoDUNE datatypes. In fact, in some cases, the code is essentially cut-and-pasted over; e.g., the `anlTypes.hh` header from `dune-raw-data` which helps SSP data analyses has been copied into `SSPTypes.hpp`. 

Each section below describes the utilities available for different detectors. Links are provided to the code; be aware, however, that the code you're linked to is taken from the head of this package's `develop` branch and consequently may differ from the code you may be using. 

_Sections not yet filled in_

## WIB

## WIB2

## DAPHNE

## SSP

## PACMAN



