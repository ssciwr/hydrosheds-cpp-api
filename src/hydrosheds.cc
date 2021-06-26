#include "hydrosheds/hydrosheds.hh"

#include "ogrsf_frmts.h"

#include<iostream>
#include<string>



namespace hydrosheds {

    HydroshedsDataSet::HydroshedsDataSet(const std::string& path)
    {
        GDALAllRegister();
        auto data = (GDALDataset*) GDALOpenEx(path.c_str(), GDAL_OF_VECTOR, NULL, NULL, NULL);

        if(data == NULL)
        {
            std::cerr << "Opening geodatabase failed." << std::endl;
            throw std::exception{};
        }

        layer = data->GetLayer(0);
        this->count = layer->GetFeatureCount();
    }

    RiverSegment HydroshedsDataSet::getSegment()
    {
        // This is extracting just some random feature for debugging purposes.
        // When we move on to iterators, this will vanish.
        return RiverSegment(layer, layer->GetFeature(12));
    }

    RiverSegment::RiverSegment(OGRLayer* layer, OGRFeature* feature)
            : layer(layer)
            , feature(feature)
    {}

    RiverSegment RiverSegment::getDownstreamSegment() const
    {
        // The modulo operation here is weird, but necessary if the GetFeature(index)
        // method is supposed to be used: It removes the continent encoding prefix
        auto next_index = feature->GetFieldAsInteger("NEXT_DOWN") % 10000000;
        return RiverSegment(layer, layer->GetFeature(next_index));
    }

    double RiverSegment::getDischarge() const
    {
        return feature->GetFieldAsDouble("DIS_AV_CMS");
    }

    OGRLineString* getLineString(OGRFeature* feature)
    {
        OGRGeometry* geo = feature->GetGeometryRef();
        OGRMultiLineString* multiline = geo->toMultiLineString();
        OGRLineString* line = *(multiline->begin());
        return line;
    }

    Coordinate RiverSegment::getStartingPoint() const
    {
        // Extract the curve start point
        OGRPoint start;
        getLineString(feature)->StartPoint(&start);

        return Coordinate{start.getX(), start.getY()};
    }

    Coordinate RiverSegment::getEndPoint() const
    {
        // Extract the curve start point
        OGRPoint end;
        getLineString(feature)->EndPoint(&end);

        return Coordinate{end.getX(), end.getY()};
    }

    bool RiverSegment::hasDownstreamSegment() const
    {
        // If the NEXT_DOWN field is 0, there is no downstream segment
        if (feature->GetFieldAsInteger("NEXT_DOWN") == 0)
            return false;

        // If the ENDORHEIC field is 1, there is also no downstream segment
        // TODO: This is my limited understanding - might need to rethink
        if (feature->GetFieldAsInteger("ENDORHEIC") == 1)
            return false;

        // In all other cases, there is a downstream segment;
        return true;
    }

    double RiverSegment::getLength() const
    {
        return getLineString(feature)->get_Length();
    }

    double RiverSegment::getGeologicalLength() const
    {
        return feature->GetFieldAsDouble("LENGTH_KM");
    }

    // begin and end of the hydroshedsdata
    FullDatasetRiverSegmentIterator HydroshedsDataSet::begin() const
    {
        this->layer->ResetReading();
        auto feature = layer->GetNextFeature();
        FullDatasetRiverSegmentIterator start(feature, layer);
        return start;
    }

    FullDatasetRiverSegmentIterator HydroshedsDataSet::end() const
    {
        FullDatasetRiverSegmentIterator end(NULL, layer);
        return end;
    }


    // FullDataSetRiverSegmentIterator
    FullDatasetRiverSegmentIterator::FullDatasetRiverSegmentIterator(OGRFeature* ogrFeature, OGRLayer* ogrLayer)
            : segment(ogrLayer, ogrFeature)
    {
        // intializing the values
        feature = ogrFeature;
        layer = ogrLayer;
    }




    // Prefix increment
    FullDatasetRiverSegmentIterator FullDatasetRiverSegmentIterator::operator++()
    {
        // "incrementing" the Feature first
        //FullDatasetRiverSegmentIterator t(feature, layer);
        auto nextFeature = this->layer->GetNextFeature();
        this->segment.feature = nextFeature;
        this->feature = nextFeature;
        return (*this);
    }

    // Postfix increment
    FullDatasetRiverSegmentIterator FullDatasetRiverSegmentIterator::operator++(int)
    {
        // returning the feature first
        auto result = (*this);
        auto nextFeature = this->layer->GetNextFeature();
        this->segment.feature = nextFeature;
        this->feature = nextFeature;
        return result;
    }

    // Two implementations for the comparison operators

    bool FullDatasetRiverSegmentIterator::operator==(const FullDatasetRiverSegmentIterator& a)
    {
        bool result;
        //result = feature->Equal(a.feature);
        result = feature == a.feature;
        return result; // do pointer equality
    }


    bool FullDatasetRiverSegmentIterator::operator!=(const FullDatasetRiverSegmentIterator& a)
    {
        bool result;
        result = (*this) != a;
        return !result; // do pointer equality
    }


    RiverSegment* FullDatasetRiverSegmentIterator::operator->()
    {
        return &segment;
    }


    RiverSegment& FullDatasetRiverSegmentIterator::operator*()
    {
        return segment;
    }

    /// followbegin() and followend()
/*
    DownstreamIteratorHelper HydroshedsDataSet::followbegin() const
    {
        this->begin();
    }

    DownstreamIteratorHelper HydroshedsDataSet::followend() const
    {

        DownstreamIteratorHelper end();
        return end;
    }
*/


    // DowmstreamIterator
    DownstreamIterator DownstreamIterator::operator++()
    {
        auto next = this->segment.getDownstreamSegment();
        this->segment = next;
        return (*this);
    }

    DownstreamIterator DownstreamIterator::operator++(int)
    {
        auto result = (*this);
        auto next = this->segment.getDownstreamSegment();
        this->segment = next;
        return result;
    }
    bool DownstreamIterator::operator==(const DownstreamIterator& a)
    {
        auto feature = this->segment.feature;
        auto aFeature = a.segment.feature;
        bool result = feature == aFeature;
        return result;
    }
    bool DownstreamIterator::operator!=(const DownstreamIterator& a)
    {
        auto feature = this->segment.feature;
        auto aFeature = a.segment.feature;
        bool result = feature == aFeature;
        return !result;
    }

    RiverSegment* DownstreamIterator::operator->()
    {
        return &segment;
    }

    RiverSegment& DownstreamIterator::operator*()
    {
        return segment;
    }



    namespace impl{
        double norm(hydrosheds::Coordinate x, hydrosheds::Coordinate y)
        {
            double result = sqrt(pow(x[0]-y[0],2)+ pow(x[1]-y[1],2));
            return result;
        }
        DownstreamIterationHelper followstream(const hydrosheds::HydroshedsDataSet& dataset, hydrosheds::Coordinate x)
        {
            DownstreamIterationHelper result{dataset, x};
            return result;
        }

        // DownstreamIteratorHelper
        DownstreamIterator DownstreamIterationHelper::begin() const
        {
            auto feature = dataset.layer->GetFeature(0);
            RiverSegment segment(dataset.layer, feature);
            hydrosheds::Coordinate closest = segment.getStartingPoint();
            // iterate over all from the FullDatasetRiverSegmentIterator and find RiverSegment closest to x
            for(auto i : dataset)
            {
                auto start = i.getStartingPoint();
                if(impl::norm(x, start)<impl::norm(x, closest))
                {
                    segment = i;
                    closest = i.getStartingPoint();
                }
            }
            DownstreamIterator result(segment);
            return result;
        }

        // ----------------------------


        // DownstreamIteratorHelper end();
        DownstreamIterator DownstreamIterationHelper::end() const
        {
            auto feature = dataset.layer->GetFeature(0);
            RiverSegment segment(dataset.layer, feature);
            DownstreamIterator end(segment);
            if(end.isEndSegment == true)
            {
                return end;
            }
        }
    }

} // namespace hydrosheds