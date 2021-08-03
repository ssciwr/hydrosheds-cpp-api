#include "hydrosheds/hydrosheds.hh"

#include <iostream>
#include <cmath>
#include <iomanip>

using namespace hydrosheds;

// static layer declaration
OGRLayer* RiverSegment::layer;

/* -- CLASS HYDROSHEDS DATA SET -- */
HydroshedsDataSet::HydroshedsDataSet(const std::string& path, int l_num = 0)
{
    GDALAllRegister();
    data = (GDALDataset*) GDALOpenEx(path.c_str(), GDAL_OF_VECTOR, NULL, NULL, NULL);

    if(data == NULL)
    {
        throw std::invalid_argument("Opening geodatabase failed. Check path or file format.");
    }
    
    if(data->GetLayerCount() > 1)
    {
        std::cout << "Dataset contains more than one layer." << "\n";
        std::cout << "Initialising to layer " << l_num << "." << std::endl;
    }
    layer = data->GetLayer(l_num);
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
    for(int i = 0; i < feature->GetFieldCount(); i++)
    {
        std::cout << feature->GetDefnRef()->GetFieldDefn(i)->GetNameRef() << " ";
    }
    std::cout << std::endl;
    return feature->GetFieldCount();
}

RiverSegment HydroshedsDataSet::ConstructSegment(double x_min, double y_min, 
    double x_max, double y_max, bool restriction, const int feature_index) const
{
    layer->SetSpatialFilter(NULL);
    layer->ResetReading();
    
    if(restriction == true)
    {
        if(x_min < 0.0 || y_min < 0.0 || x_max < 0.0 || y_max < 0.0)
        {
            throw std::domain_error("Coordinates should be positive.");
        }

        layer->SetSpatialFilterRect(x_min, y_min, x_max, y_max);
        std::cout << "Added spatial filter to dataset. To remove, call HydroshedsDataSet::ConstructSegment()." << std::endl;
    }

    if(feature_index >= this->shape()[0])
    {
        throw std::domain_error("Specified feature index exceeds the total number of features in the layer.");
    }

    RiverSegment::layer = layer;
    OGRFeature* f = layer->GetFeature(feature_index);
    RiverSegment s(f, 0);
    return s;
}

/* -- CLASS RIVER SEGMENT -- */
RiverSegment::RiverSegment(OGRFeature* f, int seg_num)
    : feature(f), segment(seg_num)
{
    OGRGeometry* geometry = f->GetGeometryRef();
    segment_points.clear();
    std::string ls = "LINESTRING";
    std::string mls = "MULTILINESTRING";
    std::string current_geometry_name = geometry->getGeometryName();

    if(current_geometry_name == ls)
    {
        OGRLineString* GLine = geometry->toLineString();
        for(auto point: *(GLine))
        {
            Coordinate p = {point.getX(), point.getY()};
            segment_points.push_back(p);
        }
    }
    else if(current_geometry_name == mls)
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
    std::string current_geometry_name = geometry->getGeometryName();

    this->segment_points.clear();

    if(current_geometry_name == ls)
    {
        OGRLineString* GLine = geometry->toLineString();
        for(auto point: *(GLine))
        {
            Coordinate p = {point.getX(), point.getY()};
            segment_points.push_back(p);
        }
    }
    else if(current_geometry_name == mls)
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
    std::tuple <const char*, int, double> inf(this->feature->GetGeometryRef()->getGeometryName(), this->get_number_of_subsegments(), feature->GetFieldAsDouble("LENGTH_KM"));
    if(verbose == true)
    {
        std::cout << "Geometry type of current feature: " << std::get <const char*>(inf) << "\n";
        std::cout << "Number of subsegments in feature: " << std::get <int>(inf) << "\n";
        std::cout << "Geological length of feature (in Km): " << std::get <double>(inf) << std::endl;
    }
    return inf;
}

int RiverSegment::get_number_of_subsegments() const
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
    double total_length = 0.0;
    for(int i = 0; i < this->get_number_of_subsegments(); i++)
    {
        Coordinate p1 = this->getStartingPoint(i);
        Coordinate p2 = this->getEndPoint(i); 
        total_length += std::sqrt(std::pow(71.5 * (p1[0] - p2[0]), 2) + std::pow(111.3 * (p1[1] - p2[1]), 2)); 
    }
    return total_length;
}

double RiverSegment::getGeologicalLength() const
{
    double geological_length_of_feature = feature->GetFieldAsDouble("LENGTH_KM");
    double length_of_current_segment = this->getLength();
    double total_length = this->getTotalLength(); 
    return (length_of_current_segment / total_length) * geological_length_of_feature;
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
    if(segment == this->get_number_of_subsegments() - 1)
    {
        if(feature->GetFieldAsInteger("NEXT_DOWN") == 0 || feature->GetFieldAsInteger("ENDORHEIC") == 1)
        {
            return false;
        } 
    }
    return true;
}

OGRFeature* RiverSegment::search_feature(unsigned int NEXT_DOWN_ID) const
{
    layer->ResetReading();
    // RiverSegment::layer->SetAttributeFilter(NULL);
    std::string query = "HYRIV_ID = " + std::to_string(NEXT_DOWN_ID);
    layer->SetAttributeFilter(query.c_str());
    // std::cout << RiverSegment::layer->GetNextFeature()->GetFieldAsDouble("LENGTH_KM") << std::endl;
    return layer->GetNextFeature(); 
}

RiverSegment RiverSegment::getDownstreamSegment()
{
    if(this->hasDownstreamSegment() == false)
    {
        throw std::runtime_error("No downstream segments for current segment.");
    }

    OGRFeature* f;
    int new_segment_id;
    // std::cout << "next down id: " << feature->GetFieldAsInteger("NEXT_DOWN") << std::endl;
    if(this->segment < this->get_number_of_subsegments() - 1)
    {
        f = feature;
        new_segment_id = this->segment + 1;
    } 
    else
    {
        f = this->search_feature(feature->GetFieldAsInteger("NEXT_DOWN"));
        new_segment_id = 0;
    }
    
    RiverSegment s(f, new_segment_id);
    std::cout << "Constructed downstream segment." << std::endl;
    return s;
}
