#include "hydrosheds/hydrosheds.hh"

#include "ogrsf_frmts.h"

#include<iostream>
#include<string>


namespace hydrosheds {

  HydroshedsDataSet::HydroshedsDataSet(const std::string& path)
  {
    GDALAllRegister();
    auto data = (GDALDataset*) GDALOpenEx(path.c_str(), GDAL_OF_VECTOR, NULL, NULL, NULL);

    if(data == NULL)
    {
      std::cerr << "Opening geodatabase failed." << std::endl;
      throw std::exception{};
    }

    layer = data->GetLayer(0);
  }

  RiverSegment HydroshedsDataSet::getSegment() const
  {
    // This is extracting just some random feature for debugging purposes.
    // When we move on to iterators, this will vanish.
    return RiverSegment(layer, layer->GetFeature(52));
  }

  RiverSegment::RiverSegment(OGRLayer* layer, OGRFeature* feature)
    : layer(layer)
    , feature(feature)
  {}

  RiverSegment RiverSegment::getDownstreamSegment() const
  {
    // The modulo operation here is weird, but necessary if the GetFeature(index)
    // method is supposed to be used: It removes the continent encoding prefix
    auto next_index = feature->GetFieldAsInteger("NEXT_DOWN") % 10000000;
    return RiverSegment(layer, layer->GetFeature(next_index));
  }

  double RiverSegment::getDischarge() const
  {
    return feature->GetFieldAsDouble("DIS_AV_CMS");
  }

  Coordinate RiverSegment::getStartingPoint() const
  {
    // Get the most specific GDAL object that we can find that represents our curve
    OGRGeometry* geo = feature->GetGeometryRef();
    OGRMultiLineString* multiline = geo->toMultiLineString();
    OGRLineString* line = *(multiline->begin());

    // Extract the curve start point
    OGRPoint start;
    line->StartPoint(&start);

    return Coordinate{start.getX(), start.getY()};
  }

  Coordinate RiverSegment::getEndPoint() const
  {
    // TODO: TASK
  }

  bool RiverSegment::hasDownstreamSegment() const
  {
    // TODO: TASK
  }

  double RiverSegment::getLength() const
  {
    // TODO: TASK
  }

  double RiverSegment::getGeologicalLength() const
  {
    // TODO: TASK
  }

} // namespace hydrosheds
