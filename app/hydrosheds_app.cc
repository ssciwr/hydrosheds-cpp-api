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
    }
    layer = data->GetLayer(0);
}

std::array <int, 2> HydroshedsDataSet::shape() const
{
    OGRFeature* feature;
    int count = 0;
    while((feature = layer->GetNextFeature()) != nullptr)
    {
        count++;
    }
    // check this count may return nothing as feature maybe pointing to nullptr
    std::array <int, 2> shape = {count, feature->GetFieldCount()};
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

OGRFeature* HydroshedsDataSet::search_feature(unsigned int ID) const
{
    int count = 0;
    while(layer->GetNextFeature()->GetFieldAsInteger("HYRIV_ID") != ID)
    {
        count++;
    }
    return layer->GetFeature(count);
}

RiverSegment HydroshedsDataSet::ConstructSegment(const int& i) const
{
    OGRFeature* f = layer->GetFeature(i);
    RiverSegment s(f);
    return s;
}


/* -- CLASS RIVER SEGMENT -- */
RiverSegment::RiverSegment(OGRFeature* f)
    : feature(f)
{
    // set geometry object
    geometry = feature->GetGeometryRef();
    GLine = geometry->toMultiLineString();
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

double RiverSegment::getLength(int sub_segment) const
{
    Coordinate p1 = this->getStartingPoint(sub_segment);
    Coordinate p2 = this->getEndPoint(sub_segment); 
    return std::sqrt(std::pow(71.5 * (p1[0] - p2[0]), 2) + std::pow(111.3 * (p1[1] - p2[1]), 2));
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
    return feature->GetFieldAsDouble("LENGTH_KM");
}

double RiverSegment::getDischarge() const
{
    return feature->GetFieldAsDouble("DIS_A V_CMS");
}

Coordinate RiverSegment::getStartingPoint(int sub_segment) const
{
    int count = 0;
    for(int i = 0; i < segment_points.size(); i += 2)
    {
        if (count == sub_segment)
        {
            return segment_points[i];
        }
        count += 1;

    }
}

Coordinate RiverSegment::getEndPoint(int sub_segment) const
{
    int count = 0;
    for(int i = 1; i < segment_points.size(); i += 2)
    {
        if (count == sub_segment)
        {
            return segment_points[i];
        }
        count += 1;
    }
}

bool RiverSegment::hasDownstreamSegment() const
{
    if(feature->GetFieldAsInteger("NEXT_DOWN") == 0 || feature->GetFieldAsInteger("ENDORHEIC") == 1)
    {
        return false;
    }
    return true;
}

void RiverSegment::get_ID() const
{
    std::cout << feature->GetFID() << std::endl;
}


RiverSegment RiverSegment::getDownstreamSegment() const
{
    if(this->hasDownstreamSegment() == false)
    {
        std::cerr << "No downstream segments for current segment." << std::endl;
    }

    int current_ID = feature->GetFID();

    OGRFeature* f = this->search_feature(current_ID);

}


int main(int argc, char** argv)
{
    if (argc != 2) 
    {
    std::cerr << "Usage: ./hydrosheds_app <path-to-gdb-directory>" << std::endl;
    return 1;
    }
    // Initialise the data set
    HydroshedsDataSet D(argv[1]);
    
    // Initialise a river segment object specified by the river by index
    // See feature 27005 using ogrinfo -al <path_to_data> 
    RiverSegment R = D.ConstructSegment(3127);
    R.test_geometry();

    // See number of subsegments in the chosen feature
    std::cout << "Number of segments: " << R.get_number_of_subsegments() << std::endl;


    // Start and end points test
    print_coordinate(R.getStartingPoint(5));
    print_coordinate(R.getEndPoint(5));
    std::cout << std::setprecision(4);
    std::cout << "Length of 6th subsegment of current river segment: " << R.getLength(5) << " Km" << std::endl;
    std::cout << "Total length of current river segment: " << R.getTotalLength() << "Km" << std::endl;
    std::cout << "Geological length of current river segment: " << R.getGeologicalLength() << " Km" << std::endl;
    std::cout << "Discharge of current river segment: " << R.getDischarge() << " m3/s" << std::endl;
    R.get_ID();
    return 0;
}