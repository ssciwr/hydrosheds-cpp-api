#pragma once

#include "ogrsf_frmts.h"

#include <array>
#include <string>

namespace hydrosheds 
{

  using Coordinate = std::array <double, 2>;
  using HydroshedsID = int;

  // Forward declaration
  class RiverSegment;

  class HydroshedsDataSet
  {
    public:
		// GDALOpenex accepts a const char *
		HydroshedsDataSet() = default;
		HydroshedsDataSet(const char* s);
		std::array <int, 2> shape() const;
		void FeatureAttributes() const;
		RiverSegment ConstructSegment(const int& i) const;

    private:
		OGRLayer* layer;
  };


  class RiverSegment
  {
    public:
		void test_geometry() const;
		double getLength() const;
		double getGeologicalLength() const;
		double getDischarge() const;

		Coordinate getStartingPoint() const;
		Coordinate getEndPoint() const;

		bool hasDownstreamSegment() const;
		RiverSegment getDownstreamSegment() const;
		// What about upstream segments - is it possible?

    private:
		RiverSegment(OGRFeature* feature);
		OGRFeature* feature;
		unsigned int segment;
		
	friend class HydroshedsDataSet;
  };

} 
