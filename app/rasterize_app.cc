#include "hydrosheds/rasterize.hh"

#include<iostream>


int main(int argc, char** argv)
{
  if (argc != 2) {
    std::cerr << "Usage: ./rasterize_app <path-to-gdb-directory>" << std::endl;
    return 1;
  }

  // Instantiate the data set
  hydrosheds::HydroshedsDataSet dataset(argv[1]);

  // And the rasterized version of it
  hydrosheds::RasterizedHydroshedsDataset rasterdataset(dataset, {10, 10}, {0.0, 0.0}, {1.0, 1.0});

  // Print one pixel value
  std::cout << "Value of first pixel: " << (rasterdataset[{0, 0}] ? "true" : "false" ) << std::endl;
}
