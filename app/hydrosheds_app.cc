#include "hydrosheds/hydrosheds.hh"
#include <iostream>
//#include "hydrosheds/hydroshedsTest.h"

using namespace hydrosheds;

int main(int argc, char** argv)
{

    std::string s = "/Users/halilibrahim/Desktop/HydroRIVERS_v10_eu/HydroRIVERS_v10_eu.gdb";

    HydroshedsDataSet D(s, 0);


    RiverSegment S = D.ConstructSegment();



    FullDatasetRiverSegmentIterator data(D);
    std::cout << "Constructing the first Data" << std::endl;
    FullDatasetRiverSegmentIterator data2(D);
    std::cout << "Constructing the second Data" << std::endl;



    bool tmp2 = data==data2;
    std::cout << tmp2 << std::endl;
    std::cout << "After ==" << std::endl;

    FullDatasetRiverSegmentIterator test1(D);
    FullDatasetRiverSegmentIterator test2(D);



    std::cout << "After D.end()" << std::endl;



    for(auto it= D.begin(); it!=D.end(); ++it) {
        std::cout << "Get feature ID: " << it->getfeature_index() << " get the geological Length: "
                  << it->getGeologicalLength() << std::endl;
    }
/*
    hydrosheds::Coordinate x = {12, 40};
    for(auto i : hydrosheds::impl::followstream(D, x))
    {
        std::cout << i.getGeologicalLength() << std::endl;
    }*/
    return 0;
}