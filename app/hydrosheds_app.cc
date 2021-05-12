#include "hydrosheds/hydrosheds.hh"

#include "ogrsf_frmts.h"

#include<iostream>


int main(int argc, char** argv)
{
    if (argc != 2) 
    {
    std::cerr << "Usage: ./hydrosheds_app <path-to-gdb-directory>" << std::endl;
    return 1;
    }

    GDALAllRegister();

    GDALDataset* data;
    data = (GDALDataset*) GDALOpenEx(argv[1], GDAL_OF_VECTOR, NULL, NULL, NULL );

    if(data == NULL)
    {
        std::cerr << "Opening geodatabase failed." << std::endl;
        return 1;
    }

    OGRLayer* layer;
    OGRFeature* feature;

    layer = data->GetLayer(0);


  // Extract layers
    for(auto&& oField: *(layer->GetNextFeature()))
    {
        switch(oField.GetType())
        {
            case OFTInteger:
                printf( "%d,", oField.GetInteger());
                break;
            case OFTInteger64:
                printf( CPL_FRMT_GIB ",", oField.GetInteger64());
                break;
            case OFTReal:
                printf( "%.3f,", oField.GetDouble());
                break;
            case OFTString:
                printf( "%s,", oField.GetString());
                break;
            default:
                printf( "%s,", oField.GetAsString());
                break;
        }
    }

    return 0;
}