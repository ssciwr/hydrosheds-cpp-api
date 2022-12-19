# Welcome to HydroSheds C++ API

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![GitHub Workflow Status](https://img.shields.io/github/actions/workflow/status/ssciwr/hydrosheds-cpp-api/ci.yml?branch=main)](https://github.com/dokempf/hydrosheds-cpp-api/actions/workflows/ci.yml)
[![Documentation Status](https://readthedocs.org/projects/hydrosheds-cpp-api/badge/)](https://hydrosheds-cpp-api.readthedocs.io/)

*This repository is currently under development. If you are interested in the result, check back in a few months.*
# Prerequisites

Building the HydroShed C++ API requires the following software installed:

* A C++11-compliant compiler
* CMake `>= 3.9`
* Doxygen (optional, documentation building is skipped if missing)
* [GDAL](https://gdal.org/), e.g. by doing
  *  `sudo apt install libgdal-dev` on Debian/Ubuntu
  *  `brew install gdal` on MacOS

# Building HydroSheds C++ API

The following sequence of commands downloads and builds HydroShed C++ API:

```
git clone --recursive https://github.com/ssciwr/hydrosheds-cpp-api.git
cd hydrosheds-cpp-api
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

The build process can be customized with the following CMake variables,
which can be set by adding `-D<var>={ON, OFF}` to the `cmake` call:

* `BUILD_TESTING`: Enable building of the test suite (default: `ON`)
* `BUILD_DOCS`: Enable building the documentation (default: `ON`)

# Documentation

HydroShed C++ API provides a Sphinx-based documentation, that can
be browsed [online at readthedocs.org](https://hydrosheds-cpp-api.readthedocs.io).
