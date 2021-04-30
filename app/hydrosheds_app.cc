#include "hydrosheds/hydrosheds.hh"

#include "ogrsf_frmts.h"

#include<iostream>


int main(int argc, char** argv)
{
  if (argc != 2) {
    std::cerr << "Usage: ./hydrosheds_app <path-to-gdb-directory>" << std::endl;
    return 1;
  }

  GDALAllRegister();

  GDALDataset* data;
  data = (GDALDataset*) GDALOpenEx(argv[1], GDAL_OF_VECTOR, NULL, NULL, NULL );

  if(data == NULL)
  {
      std::cerr << "Opening geodatabase failed." << std::endl;
      return 1;
  }

  std::cout << "Layer count in GeoDataBase: " << data->GetLayerCount() << std::endl;

  // Extract the first layer
  auto layer = data->GetLayer(0);

  // Extract first feature
  auto feature = layer->GetNextFeature();

  return 0;
}