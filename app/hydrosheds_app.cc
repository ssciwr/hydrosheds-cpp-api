// #include "../include/hydrosheds/hydrosheds.hh"

// #include "ogrsf_frmts.h"

// #include <iostream>
// #include <cmath>
// #include <iomanip>

// using namespace hydrosheds;

// // static layer declaration
// OGRLayer* RiverSegment::layer;

// /* -- CLASS HYDROSHEDS DATA SET -- */
// HydroshedsDataSet::HydroshedsDataSet(const std::string& path, int l_num = 0)
// {
//     GDALAllRegister();
//     data = (GDALDataset*) GDALOpenEx(path.c_str(), GDAL_OF_VECTOR, NULL, NULL, NULL);
    
//     if(data == NULL)
//     {
//         std::cerr << "Opening geodatabase failed. Check path or file format." << std::endl;
//         exit(1);
//     }
    
//     if(data->GetLayerCount() > 1)
//     {
//         std::cout << "Dataset contains more than one layer." << "\n";
//         std::cout << "Initialising by default to the first layer." << std::endl;
//     }
//     layer = data->GetLayer(l_num);
// }

// std::array <unsigned long long, 2> HydroshedsDataSet::shape() const
// {
//     OGRFeature* feature;
//     feature = layer->GetFeature(1);
//     if(layer->GetFeatureCount() == -1)
//     {
//         std::cout << "Number of features unknown (-1).";
//     }
//     std::array <unsigned long long, 2> shape = {(unsigned long long) layer->GetFeatureCount(), (unsigned long long) feature->GetFieldCount()};
//     return shape;
// }

// void HydroshedsDataSet::FeatureAttributes() const
// {
//     OGRFeature* feature;
//     feature = layer->GetNextFeature();
//     for(int i = 0; i < feature->GetFieldCount(); i++)
//     {
//         std::cout << feature->GetDefnRef()->GetFieldDefn(i)->GetNameRef() << " ";
//     }
//     layer->ResetReading();
//     std::cout << std::endl;
// }

// RiverSegment HydroshedsDataSet::ConstructSegment() const
// {
//     RiverSegment::layer = this->layer;
//     OGRFeature* f = this->layer->GetFeature(1);
//     RiverSegment s(f, 0);
//     return s;
// }

// /* -- CLASS RIVER SEGMENT -- */
// RiverSegment::RiverSegment(OGRFeature* f, int seg_num)
//     : feature(f), segment(seg_num)
// {
//     OGRGeometry* geometry = f->GetGeometryRef();
//     OGRMultiLineString* GLine = geometry->toMultiLineString();
//     this->segment_points.clear();
//     for(auto multi_line_string: *(GLine))
//     {
//         for(auto point: *(multi_line_string))
//         {
//             Coordinate p = {point.getX(), point.getY()};
//             this->segment_points.push_back(p);
//         }
//     }
// }

// RiverSegment::RiverSegment(const RiverSegment& R)
// {
//     this->feature = R.feature;
//     this->segment = R.segment;
//     OGRGeometry* geometry = R.feature->GetGeometryRef();
//     OGRMultiLineString* GLine = geometry->toMultiLineString();
//     this->segment_points.clear();
//     for(auto multi_line_string: *(GLine))
//     {
//         for(auto point: *(multi_line_string))
//         {
//             Coordinate p = {point.getX(), point.getY()};
//             this->segment_points.push_back(p);
//         }
//     }
// }

// auto RiverSegment::summary(bool verbose = false) const
// {
//     std::tuple <const char*, int, double> inf(this->feature->GetGeometryRef()->getGeometryName(), this->get_number_of_subsegments(), feature->GetFieldAsDouble("LENGTH_KM"));
//     if(verbose == true)
//     {
//         std::cout << "Geometry type of current feature: " << std::get <const char*>(inf) << "\n";
//         std::cout << "Number of subsegments in feature: " << std::get <int>(inf) << "\n";
//         std::cout << "Geological length of feature (in Km): " << std::get <double>(inf) << std::endl;
//     }
//     return inf;
// }

// int RiverSegment::get_number_of_subsegments() const
// {
//     return segment_points.size() % 2 == 0 ? segment_points.size() / 2 : segment_points.size() / 2 + 1;
// }

// double RiverSegment::getLength() const
// {
//     Coordinate p1 = this->getStartingPoint(segment);
//     Coordinate p2 = this->getEndPoint(segment); 
//     return std::sqrt(std::pow(71.500 * (p1[0] - p2[0]), 2) + std::pow(111.300 * (p1[1] - p2[1]), 2));
// }

// double RiverSegment::getTotalLength() const
// {
//     double total_length = 0.0;
//     for(int i = 0; i < this->get_number_of_subsegments(); i++)
//     {
//         Coordinate p1 = this->getStartingPoint(i);
//         Coordinate p2 = this->getEndPoint(i); 
//         total_length += std::sqrt(std::pow(71.5 * (p1[0] - p2[0]), 2) + std::pow(111.3 * (p1[1] - p2[1]), 2)); 
//     }
//     return total_length;
// }

// double RiverSegment::getGeologicalLength() const
// {
//     double geological_length_of_feature = feature->GetFieldAsDouble("LENGTH_KM");
//     double length_of_current_segment = this->getLength();
//     double total_length = this->getTotalLength(); 
//     return (length_of_current_segment / total_length) * geological_length_of_feature;
// }

// double RiverSegment::getDischarge() const
// {
//     return feature->GetFieldAsDouble("DIS_AV_CMS");
// }

// Coordinate RiverSegment::getStartingPoint(int seg) const
// {
//     int count = 0;
//     Coordinate p;
//     for(int i = 0; i < segment_points.size(); i += 2)
//     {
//         if (count == seg)
//         {
//             p = segment_points[i];
//             break;
//         }
//         count += 1;
//     }
//     return p;
// }

// Coordinate RiverSegment::getEndPoint(int seg) const
// {
//     int count = 0;
//     Coordinate p;
//     for(int i = 1; i < segment_points.size(); i += 2)
//     {
//         if (count == seg)
//         {
//             p = segment_points[i];
//             break;
//         }
//         count += 1;
//     }
//     return p;
// }

// bool RiverSegment::hasDownstreamSegment() const
// {
//     if(segment == segment_points.size() - 1)
//     {
//         if(feature->GetFieldAsInteger("NEXT_DOWN") == 0 || feature->GetFieldAsInteger("ENDORHEIC") == 1)
//         {
//             return false;
//         } 
//     }
//     return true;
// }

// OGRFeature* RiverSegment::search_feature(unsigned int NEXT_DOWN_ID) const
// {

//     RiverSegment::layer->ResetReading(); 
//     std::string query = "HYRIV_ID = " + std::to_string(NEXT_DOWN_ID);
//     layer->SetAttributeFilter(query.c_str());
//     return RiverSegment::layer->GetNextFeature(); 
// }

// RiverSegment RiverSegment::getDownstreamSegment()
// {
//     if(this->hasDownstreamSegment() == false)
//     {
//         std::cerr << "No downstream segments for current segment." << std::endl;
//         exit(1);
//     }

//     OGRFeature* f;
//     int new_segment_id;
//     if(this->segment < this->get_number_of_subsegments() - 1)
//     {
//         f = feature;
//         new_segment_id = this->segment + 1;
//     } 
//     else
//     {
//         f = this->search_feature(feature->GetFieldAsInteger("NEXT_DOWN"));
//         new_segment_id = 0;
//     }
    
//     RiverSegment s(f, new_segment_id);
//     return s;
// }

// int main(int argc, char** argv)
// {
//     if (argc < 2 or argc > 2) 
//     {
//         std::cerr << "Usage: ./hydrosheds_app <path-to-gdb-directory>" << std::endl;
//         exit(1);
//     }

//     // Initialise the data set.
//     HydroshedsDataSet D(argv[1]);

//     // Initialise a river segment object.
//     RiverSegment R = D.ConstructSegment();
//     // Initial testing
//     std::cout << "SUMMARY" << std::endl;
//     R.summary(true);
//     std::cout << "Shape: " << "(" << D.shape()[0] 
//             << ", "  << D.shape()[1] << ")" << std::endl;
//     std::cout << "LENGTHS ------" << std::endl;
//     std::cout << R.getLength() << std::endl;
//     std::cout << R.getTotalLength() << std::endl;
//     std::cout << R.getGeologicalLength() << std::endl;
    
//     std::cout << "DOWNSTREAM ITERATION ------" << std::endl;
//     RiverSegment R1 = R;
//     for(int i = 1; i < 100; i++)
//     {
//         std::cout << "Getting downstream segments..." << std::endl;
//         R1 = R1.getDownstreamSegment(); 
//         std::cout << "Feature index: " << R1.getfeature_index() << std::endl;
//         std::cout << "Current subsegment: " << R1.get_segment() << std::endl; 
//     }
    
//     return 0;
// }