#pragma once

#include "ogrsf_frmts.h"

#include<array>
#include<string>
#include <cmath>

namespace hydrosheds {

    using Coordinate = std::array<double, 2>;
    using HydroshedsID = int;

    // Forward declarations
    class RiverSegment;
    class FullDatasetRiverSegmentIterator;

    struct DownstreamIterator;
    struct DownstreamIteratorHelper;


    class HydroshedsDataSet
    {
    public:
        HydroshedsDataSet(const std::string&);
        /** @brief main constructor
         * Defauult constructor.
         */
        HydroshedsDataSet() = default;

        /** @brief Primary constructor
         * Initialises the dataset using an input path from the command line.
         * @param path Path to file.
         * @param l_num is the layer number to be initailised.
         * Defaults to 0 (first layer).
         */
        HydroshedsDataSet(const std::string&, int l_num);

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
        * @param seg_num can be used to specify which subsegment
        * of the feature will be constructed. This may
        * throw an error as @c seg_num may exceed the number
        * of subsegments in the feature.
        * ( @param x_min, @param y_min) and ( @param x_max, @param y_max)
        * are used to specify the lower left and upper right
        * corners of the rectangle. They are by default @c NULL .
        * @param restriction is used to specify whether a
        * restriction to a rectangular region
        * should be applied. It is by default @c false .
        */
        RiverSegment ConstructSegment(double x_min = 0.0, double y_min = 0.0,
                                      double x_max = 0.0, double y_max = 0.0, bool restriction = false, int seg_num = 0) const;

        // Iterator access for Feature Iteration

        /** @brief returns the first Iterator of the HydroShedsDataSet
         *
         *          important: calling this method multiple times may lead to different result
         *
         * @return the first iterator of the HydroShedsDataSet
         */
        FullDatasetRiverSegmentIterator begin() const;

        /** @brief This method returns the last iterator of the HydroShedsDataSet class
         *
         * @return the last iterator of the HydroShedsDataSet
         */
        FullDatasetRiverSegmentIterator end() const;


        // Not intended to stay - just get some segment
        RiverSegment getSegment();

    private:
        friend class RiverSegment;
        friend class FullDatasetRiverSegmentIterator;
        friend class DownstreamIterator;
        friend class DownstreamIteratorHelper;
        OGRLayer* layer;
        GDALDataset* data;
        long count;
    };


    class RiverSegment
    {
    public:
        /** @brief Default Constructor
          */
        RiverSegment() = default;

        /** @brief Copy Constructor
         * Implements deep copies.
         * Copies entities in @param R.
         * Deals with the feature pointer.
          */
        RiverSegment(const RiverSegment&);

        /** @brief Summary of Current Feature
         * Produces a summary of the basic attributes
         * of the current feature stored in @c feature .
         * @param verbose allows for a more detailed
         * description. The function returns a tuple
         * @c (geometry_type, @c number_of_subsegments, @c feature_length) .
         */
        std::tuple <const char*, int, double> summary(bool) const;


        /** @brief Length of current subsegment
        * Returns the calculated length of the subsegment (who's index, starting 0,
        * is stored in @c segment ) in Km. The length is calculated
        * using the start and end points of segment @c segment.
        */
        double getLength() const;

        /** @brief Sum length of all subsegments
         * Returns the sum of the calculated length of all subsegments (who's index, starting 0,
         * is stored in @c segment ) in Km. The length is calculated
         * using the start and end points of segment @c segment.
         */
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
         * @param seg starts at 0 (for the first segment).
         */
        Coordinate getStartingPoint(int) const;

        /** @brief End point of current subsegment
    * Returns the End point of the subsegment who's index (starting 0)
    * is stored in @c segment in geological coordinates.
    * @param seg starts at 0 (for the first segment).
    */
        Coordinate getEndPoint(int) const;

        /** @brief Existence of next downstream segment
         * Returns @c bool depending on whether a
         * downstream segment/feature exists. Returns to true even if the
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
        friend class HydroshedsDataSet;
        friend class FullDatasetRiverSegmentIterator;
        friend class DownstreamIterator;
        friend class DownstreamIteratorHelper;
        /** @brief Private constructor
     * Constructs by default the first subsegment
     * of the first feature. Construction
     */
        RiverSegment(OGRFeature*, int);

        /** @brief Search feature
         * Search the feature with the HYRIV_ID
         * @param NEXT_DOWN_ID .
         * Uses an SQL Query to search a
         * feature with a particular @c HYRIV_ID
         * by setting the search criterion of
         * @c OGRLayer::GetNextFeature() .
         */
        OGRFeature* search_feature(unsigned int) const;

        static OGRLayer* layer;
        OGRFeature* feature;
        unsigned long int segment;
        std::vector <Coordinate> segment_points;
    };




    class FullDatasetRiverSegmentIterator{
    public:
        RiverSegment segment;
        OGRFeature *feature;


        /**
         * @brief This constructor builds an object of the FullDatasetRiverSegment class using the @c HydroshedsDataSet
         * as a parameter. It is only be used by the end() method from the HydroshedsDataSet class
         */
         FullDatasetRiverSegmentIterator();


        /** @brief  Constructor using a @c HydroshedsDataSet object and @c OGRFeature for constructing.
         *  The main purpose of this constructor is for using this for the @c end method of the @c HydroshedsDataSet
         *
         *  @param HydroshedsDataSet
         *
         *  @param OGRFeature
         */
        FullDatasetRiverSegmentIterator(HydroshedsDataSet, OGRFeature* );


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
    } // namespace hydrosheds