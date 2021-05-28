#include "hydrosheds/rasterize.hh"


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

  void RasterizedHydroshedsDataset::writeToFile(const std::string&) const
  {
    // TODO (lower priority, we should use a library like libtiff)
  }

}