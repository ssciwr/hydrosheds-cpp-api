#pragma once

#include "ogrsf_frmts.h"

#include<array>
#include<string>

namespace hydrosheds {

  using Coordinate = std::array<double, 2>;
  using HydroshedsID = int;

  // Forward declaration
  class RiverSegment;

  class HydroshedsDataSet
  {
    public:
    HydroshedsDataSet(const std::string&);

    // Not intended to stay - just get some segment
    RiverSegment getSegment() const;

    private:
    OGRLayer* layer;
  };


  class RiverSegment
  {
    public:
    double getLength() const;
    double getGeologicalLength() const;
    double getDischarge() const;

    Coordinate getStartingPoint() const;
    Coordinate getEndPoint() const;

    bool hasDownstreamSegment() const;
    RiverSegment getDownstreamSegment() const;
    // What about upstream segments - is it possible?

    private:
    friend class HydroshedsDataSet;

    RiverSegment(OGRFeature*, unsigned int);
    OGRFeature* feature;
    unsigned int segment;
  };

} // namespace hydrosheds
