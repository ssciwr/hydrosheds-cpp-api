# Welcome to HydroShed C++ API

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![GitHub Workflow Status](https://img.shields.io/github/workflow/status/dokempf/hydrosheds-cpp-api/CI)](https://github.com/dokempf/hydrosheds-cpp-api/actions?query=workflow%3ACI)
[![Documentation Status](https://readthedocs.org/projects/hydrosheds-cpp-api/badge/)](https://hydrosheds-cpp-api.readthedocs.io/)
[![codecov](https://codecov.io/gh/dokempf/hydrosheds-cpp-api/branch/main/graph/badge.svg)](https://codecov.io/gh/dokempf/hydrosheds-cpp-api)
[![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=dokempf_hydrosheds-cpp-api&metric=alert_status)](https://sonarcloud.io/dashboard?id=dokempf_hydrosheds-cpp-api)

# Prerequisites

Building HydroShed C++ API requires the following software installed:

* A C++17-compliant compiler
* CMake `>= 3.9`
* Doxygen (optional, documentation building is skipped if missing)




# Building HydroShed C++ API

The following sequence of commands builds HydroShed C++ API.
It assumes that your current working directory is the top-level directory
of the freshly cloned repository:

```
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

The build process can be customized with the following CMake variables,
which can be set by adding `-D<var>={ON, OFF}` to the `cmake` call:

* `BUILD_TESTING`: Enable building of the test suite (default: `ON`)
* `BUILD_DOCS`: Enable building the documentation (default: `ON`)

# Documentation

HydroShed C++ API provides a Sphinx-based documentation, that can
be browsed [online at readthedocs.org](https://hydrosheds-cpp-api.readthedocs.io).

