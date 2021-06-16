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
    FullDatasetRiverSegmentIterator begin() const; /// it compiles but
    FullDatasetRiverSegmentIterator end() const;  /// stops after one iteration
    // Not intended to stay - just get some segment
    RiverSegment getSegment() const;

    private:
    OGRLayer* layer;
    long count;
  };


  class RiverSegment
  {
    public:
    double getLength() const;
    double getGeologicalLength() const;
    double getDischarge() const;

    Coordinate getStartingPoint() const;
    Coordinate getEndPoint() const;

    OGRFeature* getFeature(){return feature;};
    OGRLayer* getLayer(){return layer;};

    bool hasDownstreamSegment() const;
    RiverSegment getDownstreamSegment() const;
    // What about upstream segments - is it possible?


    private:
    friend class HydroshedsDataSet;
    friend class FullDatasetRiverSegmentIterator;

    RiverSegment(OGRLayer*, OGRFeature*);
    OGRLayer* layer;
    OGRFeature* feature;
  };



  class FullDatasetRiverSegmentIterator
  {
  public:
    OGRFeature *feature;
    OGRLayer *layer;
    RiverSegment segment;

    FullDatasetRiverSegmentIterator(OGRFeature* ogrFeature, OGRLayer* ogrLayer);


      /// to get a pointer and reference access
      RiverSegment* operator->();
      RiverSegment& operator*();

      /// incrementing via prefix
      FullDatasetRiverSegmentIterator operator++();

      /// incrementing via postfix
      FullDatasetRiverSegmentIterator operator++(int);

      /// equality operations
      bool operator==(const FullDatasetRiverSegmentIterator& a);
      bool operator!=(const FullDatasetRiverSegmentIterator& a);


      /// Not intendent to say
      OGRFeature* getFeature() const {return feature;};
      OGRLayer* getLayer() const {return layer;};

  };

    FullDatasetRiverSegmentIterator increment(hydrosheds::FullDatasetRiverSegmentIterator it);

} // namespace hydrosheds
