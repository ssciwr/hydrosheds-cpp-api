#include "hydrosheds/hydrosheds.hh"


using namespace hydrosheds;

// static layer declaration for RiverSegment.
OGRLayer* RiverSegment::layer;

/* -- CLASS HYDROSHEDS DATA SET -- */
HydroshedsDataSet::HydroshedsDataSet(const std::string& path, const int LayerNumber)
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
        std::cout << "Initialising to layer " << LayerNumber << "." << std::endl;
    }

    layer = data->GetLayer(LayerNumber);

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

std::string HydroshedsDataSet::GetSpatialReferenceName() const
{
    return std::string(layer->GetSpatialRef()->GetName());
}

RiverSegment HydroshedsDataSet::ConstructSegment(const int FeatureIndex, 
                                                bool restriction, 
                                                double XMin, double YMin, 
    	                                        double XMax, double YMax) const
{
    layer->SetSpatialFilter(NULL);
    layer->ResetReading();

    if (restriction)
    {
        layer->SetSpatialFilterRect(XMin, YMin, XMax, YMax);
        std::cout << "Added spatial filter to current instance of layer." << std::endl;
    }

    if (FeatureIndex >= this->shape()[0] || FeatureIndex < 0)
    {
        throw std::domain_error("Specified feature index exceeds the total number of features in the layer or is negative.");
    }

    RiverSegment::layer = this->layer;
    OGRFeature* f = this->layer->GetFeature(FeatureIndex);
    RiverSegment s(f, 0);
    return s;
}

/* -- CLASS RIVER SEGMENT -- */
RiverSegment::RiverSegment(OGRFeature* f, int SegNum)
    : feature(f), segment(SegNum)
{
    OGRGeometry* geometry = f->GetGeometryRef();

    if (!SegmentPoints.empty())
    {
        SegmentPoints.clear();
    }
    std::string ls = "LINESTRING";
    std::string mls = "MULTILINESTRING";
    std::string CurrentGeometryName = geometry->getGeometryName();

    if (CurrentGeometryName == ls)
    {
        OGRLineString* GLine = geometry->toLineString();
        for (auto point: *(GLine))
        {
            Coordinate p = {point.getX(), point.getY()};
            SegmentPoints.push_back(p);
        }
    }
    else if (CurrentGeometryName == mls)
    {
        OGRMultiLineString* GLine = geometry->toMultiLineString();
        for (auto LineString: *(GLine))
        {
            for (auto point: *(LineString))
            {
                Coordinate p = {point.getX(), point.getY()};
                SegmentPoints.push_back(p);
            }
        }
    }
    else
    {
        throw std::invalid_argument("Could not construct segment, need LINESTRING or MULTILINESTRING as geometry type.");
    }
}

// RiverSegment::RiverSegment(const RiverSegment& R)
// {
//     this->feature = R.feature;
//     this->segment = R.segment;
//     OGRGeometry* geometry = R.feature->GetGeometryRef();
//     std::string ls = "LINESTRING";
//     std::string mls = "MULTILINESTRING";
//     std::string currentGeometryName = geometry->getGeometryName();

//     this->segment_points.clear();

//     if (currentGeometryName == ls)
//     {
//         OGRLineString* GLine = geometry->toLineString();
//         for(auto point: *(GLine))
//         {
//             Coordinate p = {point.getX(), point.getY()};
//             segment_points.push_back(p);
//         }
//     }
//     else if (currentGeometryName == mls)
//     {
//         OGRMultiLineString* GLine = geometry->toMultiLineString();
//         for(auto line_string: *(GLine))
//         {
//             for(auto point: *(line_string))
//             {
//                 Coordinate p = {point.getX(), point.getY()};
//                 segment_points.push_back(p);
//             }
//         }
//     }
// }

std::tuple <const char*, int, double> RiverSegment::summary(bool verbose) const
{
    std::tuple <const char*, int, double> inf(this->feature->GetGeometryRef()->getGeometryName(),this->GetNumberOfSegments(), 
    feature->GetFieldAsDouble("LENGTH_KM"));
    if (verbose)
    {
        std::cout << "Geometry type of current feature: " << std::get <const char*>(inf) << "\n";
        std::cout << "Number of subsegments in feature: " << std::get <int>(inf) << "\n";
        std::cout << "Geological length of feature (in Km): " << std::get <double>(inf) << std::endl;
    }
    return inf;
}

int RiverSegment::GetNumberOfSegments() const
{
    return SegmentPoints.size() % 2 == 0 ? SegmentPoints.size() / 2 : SegmentPoints.size() / 2 + 1;
}

double RiverSegment::GetLength() const
{
    Coordinate p1 = this->GetStartingPoint(segment);
    Coordinate p2 = this->GetEndPoint(segment); 
    return std::sqrt(std::pow(71.500 * (p1[0] - p2[0]), 2) + std::pow(111.300 * (p1[1] - p2[1]), 2));
}

double RiverSegment::GetTotalLength() const
{
    double TotalLength = 0.0;
    for (int i = 0; i < this->GetNumberOfSegments(); i++)
    {
        Coordinate p1 = this->GetStartingPoint(i);
        Coordinate p2 = this->GetEndPoint(i); 
        TotalLength += std::sqrt(std::pow(71.5 * (p1[0] - p2[0]), 2) + std::pow(111.3 * (p1[1] - p2[1]), 2)); 
    }
    return TotalLength;
}

double RiverSegment::GetGeologicalLength() const
{
    double GeologicalLengthFeature = feature->GetFieldAsDouble("LENGTH_KM");
    double LengthCurrentSegment = this->GetLength();
    double TotalLength = this->GetTotalLength(); 
    return (LengthCurrentSegment / TotalLength) * GeologicalLengthFeature;
}

double RiverSegment::GetDischarge() const
{
    return feature->GetFieldAsDouble("DIS_AV_CMS");
}

Coordinate RiverSegment::GetStartingPoint(int seg) const
{
    int count = 0;
    Coordinate p;
    for (int i = 0; i < SegmentPoints.size(); i += 2)
    {
        if (count == seg)
        {
            p = SegmentPoints[i];
            break;
        }
        count += 1;
    }
    return p;
}

Coordinate RiverSegment::GetEndPoint(int seg) const
{
    int count = 0;
    Coordinate p;
    for (int i = 1; i < SegmentPoints.size(); i += 2)
    {
        if (count == seg)
        {
            p = SegmentPoints[i];
            break;
        }
        count += 1;
    }
    return p;
}

bool RiverSegment::HasDownstreamSegment() const
{
    if (segment == this->GetNumberOfSegments() - 1)
    {
        if (feature->GetFieldAsInteger("NEXT_DOWN") == 0 || feature->GetFieldAsInteger("ENDORHEIC") == 1)
        {
            return false;
        } 
    }
    return true;
}

OGRFeature* RiverSegment::SearchFeature(unsigned int NextDownID) const
{
    layer->ResetReading();
    std::string query = "HYRIV_ID = " + std::to_string(NextDownID);
    layer->SetAttributeFilter(query.c_str());
    return layer->GetNextFeature(); 
}

RiverSegment RiverSegment::GetDownstreamSegment()
{
    if (!this->HasDownstreamSegment())
    {
        throw std::runtime_error("No more downstream segments for current segment.");
    }

    OGRFeature* f;
    int NewSegmentId;
    if (this->segment < this->GetNumberOfSegments() - 1)
    {
        f = feature;
        NewSegmentId = this->segment + 1;
    } 
    else
    {
        f = this->SearchFeature(feature->GetFieldAsInteger("NEXT_DOWN"));
        NewSegmentId = 0;
    }
    
    RiverSegment s(f, NewSegmentId);
    return s;
}
