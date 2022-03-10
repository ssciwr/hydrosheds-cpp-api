.. mdinclude:: ../README.md

Introduction
------------
The HydroSheds C++ API is interface that is built on top of the GDAL interface. The interface provides easier control over vector geographic data. It abstracts away the need for dealing with C style pointers and unnecssary code that one must read extensively about in the GDAL documentation. 

.. doxygenclass:: hydrosheds::HydroshedsDataSet
   :members:

.. doxygenclass:: hydrosheds::RiverSegment
   :members:

How to iterate over the Dataset?
------------

In order for iterating over the Dataset, you have to initialize the Dataset, which can be done as follows:

.. code-block:: C++

   HydroshedsDataSet D(argv[1], 0); // where argv[1] should be the passed path to the Dataset

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


