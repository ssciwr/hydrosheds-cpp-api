#pragma once

#include "ogrsf_frmts.h"

#include <array>
#include <string>
#include <vector>

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
		HydroshedsDataSet(const std::string&);
		std::array <int, 2> shape() const;
		void FeatureAttributes() const;
		OGRFeature* search_feature(unsigned int) const;
		RiverSegment ConstructSegment(const int& i) const;

	private:
		OGRLayer* layer;
	};


	class RiverSegment
	{
	public:
		void test_geometry() const;
		int get_number_of_subsegments() const;
		double getLength(int) const;
		double getTotalLength() const;
		double getGeologicalLength() const;
		double getDischarge() const;

		Coordinate getStartingPoint(int) const;
		Coordinate getEndPoint(int) const;
		void get_ID() const;

		bool hasDownstreamSegment() const;
		RiverSegment getDownstreamSegment() const;
		// What about upstream segments - is it possible?

	private:
		RiverSegment(OGRFeature* feature);
		OGRFeature* feature;
		OGRGeometry* geometry;
		OGRMultiLineString* GLine;
		std::vector <Coordinate> segment_points;  
		unsigned long int segment;
		
		friend class HydroshedsDataSet;
	};

} 

