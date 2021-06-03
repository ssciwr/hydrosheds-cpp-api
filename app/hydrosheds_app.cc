#include "../include/hydrosheds/hydrosheds.hh"

#include "ogrsf_frmts.h"

#include <iostream>
#include <vector>
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
    RiverSegment s(f);
    return s;
}


/* -- CLASS RIVER SEGMENT -- */
RiverSegment::RiverSegment(OGRFeature* f)
    : feature(f) 
    {
        geometry = feature->GetGeometryRef();
    }

void RiverSegment::test_geometry() const
{
    // std::cout << "Number of geometrical attributes: " 
    //           << feature->GetGeomFieldCount() << std::endl;
    OGRMultiLineString* GLine = geometry->toMultiLineString();
    std::cout << std::setprecision(13);
    for(auto multi_line_string: *(GLine))
    {
        for(auto point: *(multi_line_string))
        {
            // point is an OGRPoint
            std::cout << point.getX() << ", " <<  point.getY() << std::endl;
        }
    }
    std::cout << std::setprecision(2);
}

int RiverSegment::get_number_of_subsegments() const
{
    OGRMultiLineString* GLine = geometry->toMultiLineString();
    int count = 0;
    for(auto multi_line_string: *(GLine))
    {
        for(auto point: *(multi_line_string))
        {
            count++;
        }
    }
    return count;
}

Coordinate RiverSegment::getStartingPoint(int sub_segment) const
{
    OGRMultiLineString* GLine = geometry->toMultiLineString();
    int count = 0;
    Coordinate p;
    for(auto multi_line_string: *(GLine))
    {
        for(auto point: *(multi_line_string))
        {
            if(count == sub_segment)
            {
                p = {point.getX(), point.getY()};
            }
            count++;
        }
    }
    return p;
}


Coordinate RiverSegment::getEndPoint(int sub_segment) const
{
    OGRMultiLineString* GLine = geometry->toMultiLineString();
    int count = 0;
    Coordinate p;
    for(auto multi_line_string: *(GLine))
    {
        for(auto point: *(multi_line_string))
        {
            if(count == sub_segment + 1)
            {
                p = {point.getX(), point.getY()};
            }
            count++;
        }
    }
    return p; 
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
    RiverSegment R = D.ConstructSegment(27005);
    R.test_geometry();

    // See number of subsegments in the chosen feature
    std::cout << R.get_number_of_subsegments() << std::endl;


    // Start and end points test
    print_coordinate(R.getStartingPoint(0));
    print_coordinate(R.getEndPoint(0));
    return 0;
}