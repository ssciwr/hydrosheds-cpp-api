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


        /** @brief This method is for iterating over all RiverSegment of the Dataset using the DownstreamIterator class. It starts with the Segment
         *          which is the closest to parameter x.
         *
         * @param x coordinate for the closest RiverSegment
         * @return the first iterator
         */
        DownstreamIterator followbegin(Coordinate x) const;


        /** @brief This method returns the last Iterator of the DownstreamIterator class.
         *
         * @return the last iterator of the DownstreamIterator class
         */
        DownstreamIterator followend() const;

        // Not intended to stay - just get some segment
        RiverSegment getSegment();

    private:
        friend class RiverSegment;
        friend class FullDatasetRiverSegmentIterator;
        friend class DownstreamIterator;
        friend class DownstreamIteratorHelper;
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

        bool hasDownstreamSegment() const;
        RiverSegment getDownstreamSegment() const;
        // What about upstream segments - is it possible?


    private:
        friend class HydroshedsDataSet;
        friend class FullDatasetRiverSegmentIterator;
        friend class DownstreamIterator;
        friend class DownstreamIteratorHelper;
        RiverSegment(OGRLayer*, OGRFeature*);
        OGRLayer* layer;
        OGRFeature* feature;
    };


    class FullDatasetRiverSegmentIterator
    {
    private:
        OGRFeature *feature;
        OGRLayer *layer;
        RiverSegment segment;
    public:

        /** @brief Constructs an iterator with a given OGRFeature and OGRLayer
        *
        *   This Constructor initializes the segment with the two given parameters
        *
        *
        *   @param ogrFeature that this iterator should be validating against
        *   @param ogrLayer that this layer should be validating the layer
        */
        FullDatasetRiverSegmentIterator(OGRFeature* ogrFeature, OGRLayer* ogrLayer);



        /** @brief Points to the segment of the FullDatasetRiverSegmentIterator
         *
         *
         * @returns a pointer to the segment instances form the Itertor instacne
         */
        RiverSegment* operator->();


        /** @brief A reference to the segment of the FullDatasetRiverSegmentIterator
         *
         * @return a reference to reference to the segment
         */
        RiverSegment& operator*();



        /** @brief Prefix incrementation
         *
         *  This increment operator increments the iterator by assiging its feature to
         *  the next upcoming feature defined by the layer. It will be incremented first
         *  and then returned
         *
         * @returns an incremented version of the iterator
         */
        FullDatasetRiverSegmentIterator operator++();


        /** @brief Postfix incrementation
         *
         * This increment operator returns the current Iterator first and
         * then increments the Iterator
         *
         * @return an incremented version of the iterator
         */
        FullDatasetRiverSegmentIterator operator++(int);

        /** @brief comparison operator
         *
         * @param a an Iterator from the class FullDatasetRiverSegmentIterator
         * @returns whether or not this Iterator is equal to a or not
         */
        bool operator==(const FullDatasetRiverSegmentIterator& a);
        // equality operations

        /** @brief comparison operator
         *
         * @param a an Iterator from the class FullDatasetRiverSegmentIterator
         * @returns whether or not this Iterator is equal to a or not
         */
        bool operator!=(const FullDatasetRiverSegmentIterator& a);


    };


    struct DownstreamIterator{
    public:
        bool isEndSegment;
        RiverSegment segment;

        /** @brief This constructor requires a RiverSegment for instantiating a DownstreamIterator.
         *          It assigns the segment of the DownstreamIterator to the riverSegment which is
         *          provided as a parameter. It also initializes the isEndSegment member if the
         *          given segment has a DownstreamSegment or not.
         *
         * @param riverSegment
         */
        DownstreamIterator(RiverSegment riverSegment): segment(riverSegment), isEndSegment(!riverSegment.hasDownstreamSegment()){};

        /** @brief This constructor is needed only for initializing the end() method of the DownstreamIteratorHelper struct. It initializes
         *          the given RiverSegment with the OGRLayer of the dataset and initializes the OGRFeature as NULL.
         *
         * @param dataset
         */
        DownstreamIterator(HydroshedsDataSet dataset): segment(dataset.layer, NULL){};


        /** @brief This method increments the given DownstreamIterator in a prefix style
         *
         * @return Incremented DownstreamIterator
         */
        DownstreamIterator operator++();

        /** @brief This method increments the given DownstreamIterator in a postfix style
         *
         * @return Incremented DownstreamIterator
         */
        DownstreamIterator operator++(int);

        /** @brief This is a comparison operator checking on pointer equality from their RiverSegments
         *
         * @param a the DownstreamIterator to be compared with
         * @return true if the DownstreamIterator are the same
         */
        bool operator==(const DownstreamIterator& a);


        /** @brief This is a comparison operator checking on pointer equality from their RiverSegments
         *
         * @param a the DownstreamIterator to be compared with
         * @return false if the DownstreamIterator are the same
         */
        bool operator!=(const DownstreamIterator& a);

        /** @brief Points to the DownstreamIterator
         *
         * @return a RiverSegment pointer
         */
        RiverSegment* operator->();

        /** @brief Reference to the DownstreamIterator
         *
         * @returns a reference to the DownstreamIterator
         */
        RiverSegment& operator*();
    };


    namespace impl{

        /** @brief This method computes the euclidean distances between to Coordinates.
         *
         * @param x
         * @param y
         * @return the distance between two Coordinates
         */
        double norm(hydrosheds::Coordinate x, hydrosheds::Coordinate y);


        struct DownstreamIterationHelper
        {
        public:
            const hydrosheds::HydroshedsDataSet& dataset;
            hydrosheds::Coordinate x;
            bool hasDownSegment;


            /** @brief This constructor initializes the given data members.
             *
             * @param dataset_
             * @param x_
             */
            DownstreamIterationHelper(const hydrosheds::HydroshedsDataSet& dataset_, hydrosheds::Coordinate x_)
            : dataset(dataset_), x(x_) {};

            /** @brief This method returns the first iterator of the dataset nearest to Coordinate x
             *
             * @return first iterator of the given dataset
             */
            DownstreamIterator begin() const;

            /** @brief This method returns the last iterator of the dataset
             *
             * @return last iterator of the given dataset
             */
            DownstreamIterator end() const;
        };

        /** @brief This method is basically for initializing the DownstreamIteratorHelper struct for iterating over
         *          the dataset via a ranged based loop
         *
         *
         * @param dataset
         * @param x
         * @return a DownstreamIteratonHelper with a given dataset and a Coordinate
         */
        DownstreamIterationHelper followstream(const hydrosheds::HydroshedsDataSet& dataset, hydrosheds::Coordinate x);


    }

    

} // namespace hydrosheds