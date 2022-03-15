#pragma once

#include "ogrsf_frmts.h"

#include <array>
#include <string>
#include <tuple>
#include <vector>
#include <stdexcept>

/** @brief hydrosheds primary namespace
 *
 * Used to construct an object of the hydrosheds data.
 * Requires the dataset to be in .gdb format.
 * Can be downloaded from https://www.hydrosheds.org/.
 */

namespace hydrosheds 
{
	using Coordinate = std::array <double, 2>;
	using HydroshedsID = int;

	class RiverSegment;

	/** @brief HydroshedsDataSet class
	 * Main interface that deals with the .gdb data
	 */
	class HydroshedsDataSet
	{
		public:
		/** @brief main constructor
		 * Default constructor.
		 */ 
		HydroshedsDataSet() = default;

		/** @brief Primary constructor
		 * Initialises the dataset using an input path from the command line.
		 * @param path Path to file.
		 * @param LayerNumber is the layer number to be initailised.
		 * Defaults to 0 (first layer).
		 */  
		HydroshedsDataSet(const std::string&, const int LayerNumber = 0);

		/** @brief Get dataset dimensions
		 * Returns the size of the dataset as an 
		 * array (#features, #column_fields).
		 */  
		std::array <unsigned long long, 2> shape() const;

		/** @brief Field names in dataset
		 * Prints the column names in the dataset.
		 * @returns number of columns in the dataset.
		 */  
		int FeatureAttributes() const;


		/** @brief Gives the spatial reference format name
		 * of the initialised layer.
		 * @returns string as the format name.
		 */ 
		std::string GetSpatialReferenceName() const;

		/** @brief Access to the subsegments of each feature
		 * Constructs an instance of the subsegment interface.
		 * By defeault the contructor holds the passes the first subsegment  
		 * of the first feature in the dataset. 
		 * @param FeatureIndex can be used to specify which feature (according  * to the FID (defined in the GDAL documentation)) will be constructed. 
		 * ( @param x_min, @param y_min) and ( @param x_max, @param y_max)
		 * are used to specify the lower left and upper right 
		 * corners of the rectangle. They are by default @c NULL .
		 * @param restriction is used to specify whether a
		 * restriction to a rectangular region
		 * should be applied. It is by default @c false.
		 */
		RiverSegment ConstructSegment(const int FeatureIndex = 1, bool restriction = false, double XMin = 0.0, double YMin = 0.0, double XMax = 0.0, double YMax = 0.0) const;

		private:
			GDALDataset* data;
			OGRLayer* layer;
	};

	/** @brief RiverSegment class
	 * Interface deals with all features using 
	 * the subsegments.
	 */	
	class RiverSegment
	{
	public:
		/** @brief Default Constructor
	 	 */
		RiverSegment() = default;

		// /** @brief Copy Constructor
		//  * Implements deep copies.
		//  * Copies entities in @param R.
		//  * Deals with the feature pointer.
	 	//  */	
		// RiverSegment(const RiverSegment&);

		/** @brief Summary of Current Feature
		 * Produces a summary of the basic attributes
		 * of the current feature stored in @c feature .
		 * @param verbose allows for a more detailed
		 * description. The function returns a tuple
		 * @c (geometry_type, @c number_of_subsegments, @c feature_length) .
		 */
		std::tuple <const char*, int, double> summary(bool = false) const;

		/** @brief Total subsegments in a feature
		 * Gives the total number of subsegments 
		 * in a feature. The count is based on the
		 * start and end points of one subsegment.
		 */
		int GetNumberOfSegments() const;

		/** @brief Length of current subsegment.
		 * @returns the calculated length of the subsegment (who's index, starting 0, 
		 * is stored in @c segment ) in Km. 
		 * The length is calculated using the start and 
		 * end points of segment @c segment. 
		 */
		double GetLength() const;

		/** @brief Sum length of all subsegments.
		 * @returns the sum of the calculated length of all subsegments (who's index, starting 0, 
		 * is stored in @c segment ) in Km. The length is calculated
		 * using the start and end points of segment @c segment.
		 */
		double GetTotalLength() const;

		/** @brief Geological Length of current subsegment.
		 * @returns the length of the subsegment (who's index, starting 0, 
		 * is stored in @c segment ) as a fraction of the
		 * geological length of the feature in Km. 
		 * @returns double.
		 */
		double GetGeologicalLength() const; // distribute by ratio length / total_length * geo_length 

		/** @brief Feature flow rate
		 * Gives the flow rate of the feature stored 
		 * in the class in \f$ m^3/s \f$.
		 */ 
		double GetDischarge() const;

		/** @brief Starting point of current subsegment
		 * Returns the starting point of the subsegment (who's index, starting 0, 
		 * is stored in @c segment ) in geological coordinates.
		 * @param seg starts at 0 (for the first segment).
		 */
		Coordinate GetStartingPoint(int) const;

		/** @brief End point of current subsegment
		 * Returns the End point of the subsegment who's index (starting 0) 
		 * is stored in @c segment in geological coordinates.
		 * @param seg starts at 0 (for the first segment).
		 */
		Coordinate GetEndPoint(int) const;

		/** @brief Existence of next downstream segment
		 * Returns @c bool depending on whether a 
		 * downstream segment/feature exists. Returns to true even if the 
		 * @c feature needs to be shifted.
		 */
		bool HasDownstreamSegment() const; 

		/** @brief Constructs next downstream segment
		 * Uses @c bool from @c hasDownStreamSegment() to
		 * check whether segment exists. Returns a 
		 * @c RiverSegment object.
		 */	
		RiverSegment GetDownstreamSegment();

 
		long get_segment() const
		{
			return this->segment;
		}

		int getfeature_index() const
		{
			return this->feature->GetFID();
		}
		

	private:
		/** @brief Custom Constructor.
		 * Used for constructing river segments
		 * called from @c HydroshedsDataSet::ConstructSegment.
		 * @param f is the OGRFeature pointer.
		 * @param SegNum is the segment number of the subsegment 
		 * to which the river segment object is initialised.
		 */
		RiverSegment(OGRFeature*, int);

		/** @brief Search Feature.
		 * Search the feature with the HYRIV_ID
		 * @param NextDownID is the HRIV_ID 
		 * of the next downstream feature. 
		 * Uses an SQL Query to search a
		 * feature with a particular @c HYRIV_ID
		 * by setting the search criterion of 
		 * @c OGRLayer::GetNextFeature() .
		 */
		OGRFeature* SearchFeature(unsigned int) const;

		static OGRLayer* layer;
		OGRFeature* feature;
		unsigned long int segment; 
		std::vector <Coordinate> SegmentPoints;  
		
	friend class HydroshedsDataSet;
	};

} 

/* -- CONSIDERATIONS -- */
// Fuzzy conversion from coordinates to ID.
// Use a map as a data structure to keep track of vertices.
// Inflow and Outflow defined in boundary vertices. 
// 
