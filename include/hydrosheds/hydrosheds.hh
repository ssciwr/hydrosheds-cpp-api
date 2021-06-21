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

    class DownstreamIterator;
    class DownstreamIteratorHelper;


    class HydroshedsDataSet
    {
    public:
        HydroshedsDataSet(const std::string&);

        // Iterator access for Feature Iteration

        FullDatasetRiverSegmentIterator begin() const;
        FullDatasetRiverSegmentIterator end() const;

        // Iterator access for Segment Iteration
        DownstreamIteratorHelper followbegin();
        DownstreamIteratorHelper followend();

        // Not intended to stay - just get some segment
        RiverSegment getSegment();

    private:
        friend class DownstreamIteratorHelper;
        friend class DownstreamIterator;
        friend class RiverSegment;
        friend class FullDatasetRiverSegmentIterator;
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
        friend class DownstreamIteratorHelper;
        friend class DownstreamIterator;

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
        *  @param ogrLayer that this layer should be validating the layer
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
         *  the next upcoming feature defined by the layer. It will first incremented and then
         *  it will be returned
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



    namespace impl{
        double norm(hydrosheds::Coordinate x, hydrosheds::Coordinate y);

        struct DownstreamIterator{

            bool isEndSegment;
            RiverSegment segment;
            DownstreamIterator(RiverSegment riverSegment): segment(riverSegment){};


            DownstreamIterator operator++();
            DownstreamIterator operator++(int);

            bool operator==(const DownstreamIterator& a);
            bool operator!=(const DownstreamIterator& a);

            RiverSegment* operator->();
            RiverSegment& operator*();
        };

        struct DownstreamIterationHelper
        {
            const hydrosheds::HydroshedsDataSet& dataset;
            hydrosheds::Coordinate x;
            bool hasDownSegment;


            DownstreamIterationHelper(const hydrosheds::HydroshedsDataSet& dataset_, hydrosheds::Coordinate x_)
            : dataset(dataset_), x(x_) {};

            DownstreamIterator begin();
            DownstreamIterator end();
        };


        DownstreamIterationHelper followstream(const hydrosheds::HydroshedsDataSet& dataset, hydrosheds::Coordinate x);

    }

    

} // namespace hydrosheds