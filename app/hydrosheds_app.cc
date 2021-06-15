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

RiverSegment HydroshedsDataSet::ConstructSegment(const int& i) const
{
    OGRFeature* f = layer->GetFeature(i);
    RiverSegment s(f, layer);
    return s;
}


/* -- CLASS RIVER SEGMENT -- */
RiverSegment::RiverSegment(OGRFeature* f, OGRLayer* l)
    : feature(f), layer(l)
{
    // set vector of subsegments
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
    Coordinate p;
    for(int i = 0; i < segment_points.size(); i += 2)
    {
        if (count == sub_segment)
        {
            p = segment_points[i];
            break;
        }
        count += 1;
    }
    return p;

}

Coordinate RiverSegment::getEndPoint(int sub_segment) const
{
    /* Know the subsegment from construction */ 
    int count = 0;
    Coordinate p;
    for(int i = 1; i < segment_points.size(); i += 2)
    {
        if (count == sub_segment)
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
    if(feature->GetFieldAsInteger("NEXT_DOWN") == 0 || feature->GetFieldAsInteger("ENDORHEIC") == 1)
    {
        return false;
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

RiverSegment RiverSegment::getDownstreamSegment() const
{
    /* Do for subsegments or move to next feature */
    if(this->hasDownstreamSegment() == false)
    {
        std::cerr << "No downstream segments for current segment." << std::endl;
        exit(1);
    }
    // int next_ID = feature->GetFieldAsInteger("NEXT_DOWN"); ->> ID
    // std::cout << feature->GetFieldAsInteger(1) << std::endl;
    // search_feature works fine. Segementation fault here
    OGRFeature* f = search_feature(feature->GetFieldAsInteger(1));
    RiverSegment s(f, layer);
    return s;
    // std::cout << feature->GetFID() << std::endl;
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
    RiverSegment R = D.ConstructSegment(5693);
    R.test_geometry();

    // See number of subsegments in the chosen feature
    std::cout << "Number of segments: " << R.get_number_of_subsegments() << std::endl;


    // Start and end points test
    print_coordinate(R.getStartingPoint(1));
    print_coordinate(R.getEndPoint(1));
    std::cout << std::setprecision(4);
    std::cout << "Length of 1st subsegment of current river segment: " << R.getLength(1) << " Km" << std::endl;
    std::cout << "Total length of current river segment: " << R.getTotalLength() << "Km" << std::endl;
    std::cout << "Geological length of current river segment: " << R.getGeologicalLength() << " Km" << std::endl;
    // std::cout << "Discharge of current river segment: " << R.getDischarge() << " m^3/s" << std::endl;

    RiverSegment R1 = R.getDownstreamSegment();
    std::cout << "Number of segments in downstream segment: " << R1.get_number_of_subsegments() << std::endl;
    
    return 0;
}