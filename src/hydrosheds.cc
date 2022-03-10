#include "hydrosheds/hydrosheds.hh"

#include "ogrsf_frmts.h"

#include <iostream>
#include <cmath>
#include <iomanip>

using namespace hydrosheds;

// static layer declaration for RiverSegment.
OGRLayer* RiverSegment::layer;

/* -- CLASS HYDROSHEDS DATA SET -- */
HydroshedsDataSet::HydroshedsDataSet(const std::string& path, const int layerNumber)
{
    GDALAllRegister();
    data = (GDALDataset*) GDALOpenEx(path.c_str(), GDAL_OF_VECTOR, NULL, NULL, NULL);

    if (data == NULL)
    {
        throw std::invalid_argument("Opening geodatabase failed. Check path or file format.");
    }
    
    if (data->GetLayerCount() > 1)
    {
        std::cout << "Dataset contains more than one layer." << "\n";
        std::cout << "Initialising to layer " << layerNumber << "." << std::endl;
    }
    
    layer = data->GetLayer(layerNumber);
}

std::array <unsigned long long, 2> HydroshedsDataSet::shape() const
{
    OGRFeature* feature;
    feature = layer->GetFeature(1);
    std::array <unsigned long long, 2> shape = {(unsigned long long) layer->GetFeatureCount(), (unsigned long long) feature->GetFieldCount()};
    return shape;
}

int HydroshedsDataSet::FeatureAttributes() const
{
    OGRFeature* feature;
    feature = layer->GetNextFeature();
    for (int i = 0; i < feature->GetFieldCount(); i++)
    {
        std::cout << feature->GetDefnRef()->GetFieldDefn(i)->GetNameRef() << "\n";
    }
    std::cout << std::endl;
    return feature->GetFieldCount();
}

RiverSegment HydroshedsDataSet::ConstructSegment(const int featureIndex, 
                                                bool restriction, 
                                                double x_min, double y_min, 
                                                double x_max, double y_max) const
{
    layer->SetSpatialFilter(NULL);
    layer->ResetReading();
    if (restriction)
    {
        if (x_min < 0.0 || y_min < 0.0 || x_max < 0.0 || y_max < 0.0)
        {
            throw std::domain_error("Coordinates should be positive.");
        }

        layer->SetSpatialFilterRect(x_min, y_min, x_max, y_max);
        std::cout << "Added spatial filter to dataset. To remove, call HydroshedsDataSet::ConstructSegment()." << std::endl;
    }

    if (featureIndex >= this->shape()[0])
    {
        throw std::domain_error("Specified feature index exceeds the total number of features in the layer.");
    }

    RiverSegment::layer = this->layer;
    OGRFeature* f = this->layer->GetFeature(featureIndex);
    RiverSegment s(f, 0);
    return s;
}

/* -- CLASS RIVER SEGMENT -- */
RiverSegment::RiverSegment(OGRFeature* f, int seg_num)
    : feature(f), segment(seg_num)
{
    OGRGeometry* geometry = f->GetGeometryRef();
    if (!segment_points.empty())
    {
        segment_points.clear();
    }
    std::string ls = "LINESTRING";
    std::string mls = "MULTILINESTRING";
    std::string currentGeometryName = geometry->getGeometryName();

    if (currentGeometryName == ls)
    {
        OGRLineString* GLine = geometry->toLineString();
        for (auto point: *(GLine))
        {
            Coordinate p = {point.getX(), point.getY()};
            segment_points.push_back(p);
        }
    }
    else if (currentGeometryName == mls)
    {
        OGRMultiLineString* GLine = geometry->toMultiLineString();
        for (auto lineString: *(GLine))
        {
            for (auto point: *(lineString))
            {
                Coordinate p = {point.getX(), point.getY()};
                segment_points.push_back(p);
            }
        }
    }
    else
    {
        throw std::invalid_argument("Could not construct segment, need LINESTRING or MULTILINESTRING as geometry type.");
    }
}

RiverSegment::RiverSegment(const RiverSegment& R)
{
    this->feature = R.feature;
    this->segment = R.segment;
    OGRGeometry* geometry = R.feature->GetGeometryRef();
    std::string ls = "LINESTRING";
    std::string mls = "MULTILINESTRING";
    std::string currentGeometryName = geometry->getGeometryName();

    this->segment_points.clear();

    if (currentGeometryName == ls)
    {
        OGRLineString* GLine = geometry->toLineString();
        for(auto point: *(GLine))
        {
            Coordinate p = {point.getX(), point.getY()};
            segment_points.push_back(p);
        }
    }
    else if (currentGeometryName == mls)
    {
        OGRMultiLineString* GLine = geometry->toMultiLineString();
        for(auto line_string: *(GLine))
        {
            for(auto point: *(line_string))
            {
                Coordinate p = {point.getX(), point.getY()};
                segment_points.push_back(p);
            }
        }
    }
}

std::tuple <const char*, int, double> RiverSegment::summary(bool verbose = false) const
{
    std::tuple <const char*, int, double> inf(this->feature->GetGeometryRef()->getGeometryName(),this->getNumberOfSegments(), 
    feature->GetFieldAsDouble("LENGTH_KM"));
    if (verbose)
    {
        std::cout << "Geometry type of current feature: " << std::get <const char*>(inf) << "\n";
        std::cout << "Number of subsegments in feature: " << std::get <int>(inf) << "\n";
        std::cout << "Geological length of feature (in Km): " << std::get <double>(inf) << std::endl;
    }
    return inf;
}

int RiverSegment::getNumberOfSegments() const
{
    return segment_points.size() % 2 == 0 ? segment_points.size() / 2 : segment_points.size() / 2 + 1;
}

double RiverSegment::getLength() const
{
    Coordinate p1 = this->getStartingPoint(segment);
    Coordinate p2 = this->getEndPoint(segment); 
    return std::sqrt(std::pow(71.500 * (p1[0] - p2[0]), 2) + std::pow(111.300 * (p1[1] - p2[1]), 2));
}

double RiverSegment::getTotalLength() const
{
    double totalLength = 0.0;
    for(int i = 0; i < this->getNumberOfSegments(); i++)
    {
        Coordinate p1 = this->getStartingPoint(i);
        Coordinate p2 = this->getEndPoint(i); 
        totalLength += std::sqrt(std::pow(71.5 * (p1[0] - p2[0]), 2) + std::pow(111.3 * (p1[1] - p2[1]), 2)); 
    }
    return totalLength;
}

double RiverSegment::getGeologicalLength() const
{
    double geologicalLengthFeature = feature->GetFieldAsDouble("LENGTH_KM");
    double lengthCurrentSegment = this->getLength();
    double totalLength = this->getTotalLength(); 
    return (lengthCurrentSegment / totalLength) * geologicalLengthFeature;
}

double RiverSegment::getDischarge() const
{
    return feature->GetFieldAsDouble("DIS_AV_CMS");
}

Coordinate RiverSegment::getStartingPoint(int seg) const
{
    int count = 0;
    Coordinate p;
    for(int i = 0; i < segment_points.size(); i += 2)
    {
        if(count == seg)
        {
            p = segment_points[i];
            break;
        }
        count += 1;
    }
    return p;
}

Coordinate RiverSegment::getEndPoint(int seg) const
{
    int count = 0;
    Coordinate p;
    for(int i = 1; i < segment_points.size(); i += 2)
    {
        if(count == seg)
        {
            p = segment_points[i];
            break;
        }
        count += 1;
    }
    return p;
}

bool RiverSegment::hasDownstreamSegment() const
{
    if (segment == this->getNumberOfSegments() - 1)
    {
        if (feature->GetFieldAsInteger("NEXT_DOWN") == 0 || feature->GetFieldAsInteger("ENDORHEIC") == 1)
        {
            return false;
        } 
    }
    return true;
}

OGRFeature* RiverSegment::searchFeature(unsigned int NEXT_DOWN_ID) const
{
    layer->ResetReading();
    std::string query = "HYRIV_ID = " + std::to_string(NEXT_DOWN_ID);
    layer->SetAttributeFilter(query.c_str());
    return layer->GetNextFeature(); 
}

RiverSegment RiverSegment::getDownstreamSegment()
{
    if (!this->hasDownstreamSegment())
    {
        throw std::runtime_error("No more downstream segments for current segment.");
    }

    OGRFeature* f;
    int newSegmentId;
    if (this->segment < this->getNumberOfSegments() - 1)
    {
        f = feature;
        newSegmentId = this->segment + 1;
    } 
    else
    {
        f = this->searchFeature(feature->GetFieldAsInteger("NEXT_DOWN"));
        newSegmentId = 0;
    }
    
    RiverSegment s(f, newSegmentId);
    return s;
}
