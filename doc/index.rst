HydroSheds C++ API
==================

.. toctree::
   :maxdepth: 2
   :caption: Contents:


Introduction
------------
The HydroSheds C++ API is interface that is built on top of the GDAL interface. The interface provides easier control over vector geographic data. It abstracts away the need for dealing with C style pointers and unnecssary code that one must read extensively about in the GDAL documentation. 

The HydroSheds Dataset consists of information that permits the mapping of river networks across the world. A typical vector dataset consists of mulitple river lines (approximating the river network) that are in the form of either line strings or multiline strings coordinates that specify lines (approximating the river network). This is accompanied by another 15 features which among other things specifies the geological length, discharge, etc. of a particular entry in the dataset. One can read the documentation that is provided with each dataset for further gelogical information.

The main interface allows ones to interact with `.gdb` and `.shp` files. One can glean basic information about the dataset such as its shape and feature (river segment) attributes. One can also construct a `RiverSegment` object from the dataset for the purposes of iteration or gleaning information about individual features.

The rasterisation framework allows one to render the data specified by constructing the `RiverSegment`.

Add example of downstream iteration.

.. doxygenclass:: hydrosheds::HydroshedsDataSet
   :members:

.. doxygenclass:: hydrosheds::RiverSegment
   :members:

