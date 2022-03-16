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

.. doxygenclass:: hydrosheds::HydroshedsDataSet
   :members:

.. doxygenclass:: hydrosheds::RiverSegment
   :members:

Examples
========

Iterating Over the Dataset
--------------------------

In order for iterating over the Dataset, you have to initialize the Dataset, which can be done as follows:

.. code-block:: C++

   HydroshedsDataSet D(<path_to_dataset>);

For iterating over the dataset do the following:

.. code-block:: C++

   for(const auto& it : D)
    {
        // do something...
        std::cout << it.getGeologicalLength() << std::endl;
    }


.. warning::
    It is important that the usage of the iterator class should be restricted to the case that the iteration object is called by reference (when using ranged-based loops). You are allowed to use the fully C++ implementation of the for loop.

    Otherwise error can and probably will occur!

Other possibilites are:

.. code-block:: C++

   for(auto& it : D)
    {
        std::cout << it.getGeologicalLength() << std::endl;
    }

or

.. code-block:: C++

   for(auto it = D.begin(); it!=D.end(); it++)
    {
        std::cout << it->getGeologicalLength() << std::endl;
    }

Iterating downstream
--------------------
.. code-block:: C++

   HydroshedsDataSet D(<path_to_dataset>);
   RiverSegment R = D.ConstructSegment(100);
   RiverSegment R1 = R;
   for (int i = 1; i < 20; i++)
   {
      std::cout << "Getting downstream segments..." << std::endl;
      R1 = R1.GetDownstreamSegment();
      std::cout << "Feature index: " << R1.GetFeatureIndex() << std::endl;
      std::cout << "Current subsegment: " << R1.GetCurrentSegment() << std::endl; 
   }



