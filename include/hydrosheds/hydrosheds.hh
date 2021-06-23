#pragma once

#include "ogrsf_frmts.h"

#include <array>
#include <string>
#include <vector>

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
		 * Defauult constructor.
		 */ 
		HydroshedsDataSet() = default;

		/** @brief Primary constructor
		 * Initialises the dataset using an input path from the command line.
		 * @param path Path to file.
		 */  
		HydroshedsDataSet(const std::string&);

		/** @brief Get dataset dimensions
		 * Returns the size of the dataset as an array (#features, #column_fields).
		 */  
		std::array <unsigned long long, 2> shape() const;

		/** @brief Field names in dataset
		 * Returns the column names in the dataset.
		 */  
		void FeatureAttributes() const;

		/** @brief Access to the subsegments of each feature
		 * Constructs an instance of the subsegment interface.
		 * By defeault the contructor holds the passes the first subsegment  
		 * of the first feature in the dataset. 
		 */
		RiverSegment ConstructSegment() const;

	private:
		OGRLayer* layer;
	};

	/** @brief RiverSegment class
	 * Interface deals with all features using the subsegments.
	 */	
	class RiverSegment
	{
	public:
		/** @brief Default Constructor
	 	 */
		RiverSegment() = default;

		/** @brief Copy Constructor
		 * Implements deep copies.
		 * Copies entitie in @param RiverSegment 
		 * has to deal with the feature pointer
	 	 */	
		RiverSegment(const RiverSegment&);

		/** @brief Assignment Operator
	 	 */		
		RiverSegment& operator=(RiverSegment&);

		void test_geometry() const;

		/** @brief Length of current subsegment
		 * Returns the calculated length of the subsegment (who's index, starting 0, 
		 * is stored in @c segment ) in Km. The length is calculated
		 * using the start and points of segment @c segment. 
		 */
		double getLength() const;

		double getTotalLength() const;

		/** @brief Geological Length of current subsegment
		 * Returns the length of the subsegment (who's index, starting 0, 
		 * is stored in @c segment ) as a fraction of the
		 * geological length of the feature in Km. 
		 */
		double getGeologicalLength() const; // distribute by ratio length / total_length * geo_length 

		/** @brief Feature flow rate
		 * Gives the flow rate of the feature stored 
		 * in the class in \f$ m^3/s \f$.
		 */ 
		double getDischarge() const;

		/** @brief Starting point of current subsegment
		 * Returns the starting point of the subsegment (who's index, starting 0, 
		 * is stored in @c segment ) in geological coordinates.
		 */
		Coordinate getStartingPoint(int) const;

		/** @brief End point of current subsegment
		 * Returns the End point of the subsegment who's index (starting 0) 
		 * is stored in @c segment in geological coordinates.
		 */
		Coordinate getEndPoint(int) const;

		/** @brief Existence of next downstream segment
		 * Returns @c bool depending on whether a 
		 * downstream exists. Returns to true even if the 
		 * @c feature needs to be shifted.
		 */
		bool hasDownstreamSegment() const; 

		/** @brief Constructs next downstream segment
		 * Uses @c bool from @c hasDownStreamSegment() to
		 * check whether segment exists. Returns a 
		 * @c RiverSegment object.
		 */	
		RiverSegment getDownstreamSegment();

		/** @brief Total subsegments in a feature
		 * Gives the total number of subsegments 
		 * in a feature. The count is based on the
		 * start and end points of one subsegment.
		 */
		int get_number_of_subsegments() const;
 
		long get_segment() const
		{
			return this->segment;
		}

		int getfeature_index() const
		{
			return this->feature->GetFID();
		}
		

	private:
		/** @brief Private constructor
		 * Constructs by default the first subsegment of the first feature.
		 */
		RiverSegment(OGRFeature*, OGRLayer*, int);

		/** @brief Search feature
		 * Search the feature with number (ID) 
		 * @c ID . The ID is the feature number and
		 * is not the HYRIV_ID. 
		 */
		OGRFeature* search_feature(unsigned int) const;

		OGRLayer* layer;
		OGRFeature* feature;
		unsigned long int segment; 
		std::vector <Coordinate> segment_points;  
		
		friend class HydroshedsDataSet;
	};

} 

