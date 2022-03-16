#pragma once

#include "ogrsf_frmts.h"

#include <array>
#include <string>
#include <tuple>
#include <vector>
#include <stdexcept>
#include <cmath>
#include <iostream>

/** @brief hydrosheds primary namespace
 * Used to construct an object of the hydrosheds data.
 * Requires the dataset to be in .gdb format.
 * Can be downloaded from https://www.hydrosheds.org/.
 */

namespace hydrosheds 
{
    using Coordinate = std::array <double, 2>;
    using HydroshedsID = int;

    class RiverSegment;
    class FullDatasetRiverSegmentIterator;

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
            HydroshedsDataSet(const std::string&, const int = 0);

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

            /** @brief Access to the subsegments of each feature.
             * Constructs an instance of the subsegment interface.
             * By defeault the contructor holds the passes the first subsegment  
             * of the first feature in the dataset. 
             * @param FeatureIndex can be used to specify which feature (according  * to the FID (defined in the GDAL documentation)) will be constructed. 
             * ( @param XMin, @param YMin) and ( @param XMax, @param YMax)
             * are used to specify the lower left and upper right 
             * corners of the rectangle. They are by default @c 0.
             * @param restriction is used to specify whether a
             * restriction to a rectangular region
             * should be applied. It is by default @c false.
             */
            RiverSegment ConstructSegment(const int = 1, bool = false, double = 0.0, double = 0.0, double = 0.0, double = 0.0) const;

            /** @brief returns the first Iterator of the HydroShedsDataSet
             * `important:` calling this method multiple times may lead to different result
             * @returns FullDatasetRiverSegmentIterator.
             */
            FullDatasetRiverSegmentIterator begin() const;

            /** @brief This method returns the last iterator of the HydroShedsDataSet class
             * @returns FullDatasetRiverSegmentIterator.
             */
            FullDatasetRiverSegmentIterator end() const;

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

    
            /** @brief Segment Number.
             * @returns the segment number at 
             * which the current instance of RiverSegment
             * has been initialised.
             */
            unsigned int GetCurrentSegment() const
            {
                return this->segment;
            }

            /** @brief Feature Number.
             * @returns the current feature number of  
             * the current instance of RiverSegment.
             */
            unsigned int GetFeatureIndex() const
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
        friend class FullDatasetRiverSegmentIterator;
    };

    struct FullDatasetRiverSegmentIterator
    {
        RiverSegment segment;
        OGRFeature* feature;

        /**
         * @brief This constructor builds an object of the FullDatasetRiverSegment class using the @c HydroshedsDataSet
         * as a parameter. It is only be used by the end() method from the HydroshedsDataSet class
         */
        FullDatasetRiverSegmentIterator();


        /** @brief  Constructor using a @c HydroshedsDataSet object and @c OGRFeature for constructing.
         * The main purpose of this constructor is for using this for the @c end method of the @c HydroshedsDataSet
         *
         *  @param HydroshedsDataSet
         *
         *  @param OGRFeature
         */
        FullDatasetRiverSegmentIterator(HydroshedsDataSet, OGRFeature*);


        /**
         * @brief Prefix incrementation for incrementing the current object and getting the NextFeature.
         * Note: If trying to use this for the last Feature for the dataset pointing to NULL, errors may occur
         * @returns the incremented FullDatasetRiverSegmentIterator
         */
        FullDatasetRiverSegmentIterator operator++();


        /**
         * @brief Postfix incrementation for incrementing the current object and return the the object afterwards.
         *
         * Note: If trying to use this for the last Feature for the dataset pointing to NULL, errors may occur
         * @returns the current FullDatasetRiverSegmentIterator first and then increments the object afterwards
         */
        FullDatasetRiverSegmentIterator operator++(int);


        /**
         * @brief points to segment of the @c FullDatasetRiverSegmentIterator
         * @return pointer to the segment
         */
        RiverSegment* operator->();

        /** @brief A reference to the segment of the FullDatasetRiverSegmentIterator
         *
         * @return reference to reference to the segment
         */
        RiverSegment& operator*();


        /** @brief comparison operator
         *
         * @param a an Iterator from the class FullDatasetRiverSegmentIterator
         * @returns whether or not this Iterator is equal to a or not
         */
        bool operator==(const FullDatasetRiverSegmentIterator& a);


        /** @brief comparison operator
         *
         * @param a an Iterator from the class FullDatasetRiverSegmentIterator
         * @returns whether or not this Iterator is equal to a or not
         */

        bool operator!=(const FullDatasetRiverSegmentIterator& a);
    };

} 
