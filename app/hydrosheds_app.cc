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
    : feature(f), layer(l)
{
    this->segment = seg_num;
    // std::cout << "Constructing segment: " << std::endl;
    // std::cout << "\t";
    // std::cout << "Feature HYRIV: " << feature->GetFID() << "\n";
    // std::cout << "\t";
    // std::cout << "segment num: " << segment << "\n";
    // set vector of subsegments of current feature
    OGRGeometry* geometry = f -> GetGeometryRef();
    OGRMultiLineString* GLine = geometry->toMultiLineString();
    this->segment_points.clear();
    for(auto multi_line_string: *(GLine))
    {
        for(auto point: *(multi_line_string))
        {
            // set segment start and end points
            // point is an OGRPoint 
            Coordinate p = {point.getX(), point.getY()};
            this->segment_points.push_back(p);
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
    // What happens if feature is endorheic should go back to main river.
    if(this->hasDownstreamSegment() == false)
    {
        std::cerr << "No downstream segments for current segment." << std::endl;
        exit(1);
    }

    // Need better memory management.
    OGRFeature* f;
    if(this->segment < this->get_number_of_subsegments() - 1)
    {
        f = feature;
    } 
    else
    {
        f = this->search_feature(feature->GetFieldAsInteger("NEXT_DOWN"));
    }
    // std::cout << "From downstream feature ID: " << f->GetFID() << std::endl;
    
    int new_segment_id = this->segment + 1;
    RiverSegment s(f, layer, new_segment_id);
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
    // Initial testing
    std::cout << "TEST GEOMETRY" << std::endl;
    R.test_geometry();
    std::cout << std::setprecision(13);
    std::cout << "Shape: " << D.shape()[0] << " " << D.shape()[1] << std::endl;
    // std::cout << R.getLength() << std::endl;
    // std::cout << R.getTotalLength() << std::endl;
    // std::cout << R.getGeologicalLength() << std::endl;
    std::cout << std::setprecision(2);

    std::cout << "Current subsegment: " << R.get_segment() << std::endl;
    std::cout << "Getting downstream segments..." << std::endl;
    RiverSegment R1 = R.getDownstreamSegment();
    std::cout << "Feature index: " << R1.getfeature_index() << std::endl;
    std::cout << "Current subsegment: " << R1.get_segment() << std::endl;

    std::cout << "Getting downstream segments..." << std::endl;
    RiverSegment R2 = R1.getDownstreamSegment();
    std::cout << "Feature index: " << R2.getfeature_index() << std::endl;
    std::cout << "Current subsegment: " << R2.get_segment() << std::endl;

    std::cout << "Getting downstream segments..." << std::endl;
    RiverSegment R3 = R2.getDownstreamSegment();
    std::cout << "Feature index: " << R3.getfeature_index() << std::endl;
    std::cout << "Current subsegment: " << R3.get_segment() << std::endl;

    std::cout << "Getting downstream segments..." << std::endl;
    RiverSegment R4 = R3.getDownstreamSegment();
    std::cout << "Feature index: " << R4.getfeature_index() << std::endl;
    std::cout << "Current subsegment: " << R4.get_segment() << std::endl;

    std::cout << "Getting downstream segments..." << std::endl;
    RiverSegment R5 = R4.getDownstreamSegment();
    std::cout << "Feature index: " << R5.getfeature_index() << std::endl;
    std::cout << "Current subsegment: " << R5.get_segment() << std::endl;
    
    return 0;
}