#include "../include/hydrosheds/hydrosheds.hh"

#include "ogrsf_frmts.h"

#include <iostream>
#include <vector>
#include <iomanip>

using namespace hydrosheds;

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
RiverSegment::RiverSegment(OGRFeature* f): feature(f) {}

void RiverSegment::test_geometry() const
{
    OGRGeometry* G;
    G = feature->GetGeometryRef();
    std::cout << "Number of geometrical attributes: " 
              << feature->GetGeomFieldCount() << std::endl;
    // std::cout << G->getGeometryName() << std::endl;
    // std::cout << G->getDimension() << std::endl;
    OGRMultiLineString* GLine = G->toMultiLineString();
    std::cout << std::setprecision(13);
    for(auto multi_line_string: *(GLine))
    {
        for(auto point: *(multi_line_string))
        {
            // point is an OGRPoint
            std::cout << point.getX() << ", " <<  point.getY() << std::endl;
        }
    }
    std::cout << std::setprecision(4);
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
    RiverSegment R = D.ConstructSegment(27005);
    R.test_geometry();

    return 0;
}