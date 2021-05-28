#pragma once

#include "ogrsf_frmts.h"

#include<array>
#include<string>

namespace hydrosheds {

  using Coordinate = std::array<double, 2>;
  using HydroshedsID = int;

  // Forward declarations
  class RiverSegment;
  class FullDatasetRiverSegmentIterator;

  class HydroshedsDataSet
  {
    public:
    HydroshedsDataSet(const std::string&);

    // Iterator access
    FullDatasetRiverSegmentIterator begin() const;
    FullDatasetRiverSegmentIterator end() const;

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

    RiverSegment(OGRLayer*, OGRFeature*);
    OGRLayer* layer;
    OGRFeature* feature;
  };

  class FullDatasetRiverSegmentIterator
  {
    // TODO: Add the iterator interface for an iterator that traverses
    //       the entire dataset and gives access to RiverSegment
  };

} // namespace hydrosheds
