#include "../include/hydrosheds/hydrosheds.hh"

#include "ogrsf_frmts.h"

#include <iostream>
#include <cmath>
#include <iomanip>

using namespace hydrosheds;

/* -- FREE FUNCTIONS -- */
void print_coordinate(Coordinate p)
{
    std::cout << std::setprecision(13);
    std::cout << "(" << p[0] << ", " << p[1] << ")" << std::endl; 
    std::cout << std::setprecision(2);
}

/* -- CLASS HYDROSHEDS DATA SET -- */
HydroshedsDataSet::HydroshedsDataSet(const std::string& path)
{
    GDALAllRegister();
    GDALDataset* data;
    data = (GDALDataset*) GDALOpenEx(path.c_str(), GDAL_OF_VECTOR, NULL, NULL, NULL);
    if(data == NULL)
    {
        std::cerr << "Opening geodatabase failed." << std::endl;
        exit(1);
    }
    layer = data->GetLayer(0);
}

std::array <unsigned long long, 2> HydroshedsDataSet::shape() const
{
    OGRFeature* feature;
    feature = layer->GetFeature(1);
    // check this count may return nothing as feature maybe pointing to nullptr
    std::array <unsigned long long, 2> shape = {(unsigned long long) layer->GetFeatureCount(), (unsigned long long) feature->GetFieldCount()};
    return shape;
}

void HydroshedsDataSet::FeatureAttributes() const
{
    OGRFeature* feature;
    feature = layer->GetNextFeature();
    for(int i = 0; i < feature->GetFieldCount(); i++)
    {
        // GetDefnRef() returns an object of the OGRFeatureDefn class. GetFieldDefn() returns an object of the class OGRFeatureDefn
        std::cout << feature->GetDefnRef()->GetFieldDefn(i)->GetNameRef() << " ";
    }
}

RiverSegment HydroshedsDataSet::ConstructSegment() const
{
    OGRFeature* f = layer->GetFeature(1);
    RiverSegment s(f, layer, 0);
    return s;
}

/* -- CLASS RIVER SEGMENT -- */
RiverSegment::RiverSegment(OGRFeature* f, OGRLayer* l, int seg_num)
    : feature(f), layer(l), segment(seg_num)
{
    // set vector of subsegments of current feature
    OGRGeometry* geometry = feature->GetGeometryRef();
    OGRMultiLineString* GLine = geometry->toMultiLineString();
    for(auto multi_line_string: *(GLine))
    {
        for(auto point: *(multi_line_string))
        {
            // set segment start and end points
            // point is an OGRPoint 
            Coordinate p = {point.getX(), point.getY()};
            segment_points.push_back(p);
        }
    }
}

void RiverSegment::test_geometry() const
{
    std::cout << std::setprecision(13);
    for(auto point: segment_points)
    {
        std::cout << point[0] << ", " <<  point[1] << std::endl;
    }
    std::cout << std::setprecision(2);
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
        if (count == seg)
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
    /* Know the subsegment from construction */ 
    int count = 0;
    Coordinate p;
    for(int i = 1; i < segment_points.size(); i += 2)
    {
        if (count == seg)
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
    if(segment == segment_points.size() - 1)
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
    OGRFeature* f_iter;
    OGRFeature* f_ret;
    layer->ResetReading();
    while((f_iter = layer->GetNextFeature()) != NULL)
    {
        if(f_iter->GetFieldAsInteger("HYRIV_ID") == NEXT_DOWN_ID)
        {
            f_ret = f_iter;
            break;
        }
    }
    return f_ret;
}

RiverSegment RiverSegment::getDownstreamSegment()
{
    if(this->hasDownstreamSegment() == false)
    {
        std::cerr << "No downstream segments for current segment." << std::endl;
        exit(1);
    }

    if(segment == segment_points.size() - 1)
    {
       segment++;
    }

    OGRFeature* f = search_feature(feature->GetFieldAsInteger("NEXT_DOWN"));
    RiverSegment s(f, layer, 0);
    return s;
}


int main(int argc, char** argv)
{
    if (argc != 2) 
    {
    std::cerr << "Usage: ./hydrosheds_app <path-to-gdb-directory>" << std::endl;
    return 1;
    }
    // Initialise the data set.
    HydroshedsDataSet D(argv[1]);

    // Initialise a river segment object.
    RiverSegment R = D.ConstructSegment();
    R.test_geometry();
    std::cout << std::setprecision(13);
    std::cout << "Shape: " << D.shape()[0] << " " << D.shape()[1] << std::endl;
    std::cout << R.getLength() << std::endl;
    std::cout << R.getTotalLength() << std::endl;
    std::cout << R.getGeologicalLength() << std::endl;

    /* // See number of subsegments in the chosen feature
    std::cout << "Number of subsegments: " << R.get_number_of_subsegments() << std::endl;


    // Start and end points test
    print_coordinate(R.getStartingPoint(1));
    print_coordinate(R.getEndPoint(1));
    std::cout << std::setprecision(4);
    std::cout << "Length of 1st subsegment of current river segment: " << R.getLength(1) << " Km" << std::endl;
    std::cout << "Total length of current river segment: " << R.getTotalLength() << "Km" << std::endl;
    std::cout << "Geological length of current river segment: " << R.getGeologicalLength() << " Km" << std::endl;
    std::cout << "Discharge of current river segment: " << R.getDischarge() << " m^3/s" << std::endl;

    RiverSegment R1 = R.getDownstreamSegment();
    std::cout << "Number of subsegments in downstream segment: " << R1.get_number_of_subsegments() << std::endl;
     */
    return 0;
}