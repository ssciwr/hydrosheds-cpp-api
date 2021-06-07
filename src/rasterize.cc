#include "hydrosheds/rasterize.hh"

#include<tiffio.h>

namespace hydrosheds {

  RasterizedHydroshedsDataset::RasterizedHydroshedsDataset(
      const HydroshedsDataSet& dataset,
      std::array<unsigned int, 2> resolution,
      Coordinate lowerleft,
      Coordinate upperright
    )
  {
    // TODO: This is where the actual rasterization could be implemented
  }

  bool RasterizedHydroshedsDataset::operator[](const std::array<unsigned int, 2>&) const
  {
    // TODO
  }

  void RasterizedHydroshedsDataset::writeToFile(const std::string& filename) const
  {
    // TODO e.g. using code from this StackOverflow post:
    // https://stackoverflow.com/questions/50770906/32bit-tif-image-read-and-write-using-libtiff-library-c
  }

}