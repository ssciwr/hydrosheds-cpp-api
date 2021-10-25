#include "hydrosheds/hydrosheds.hh"
#include <iostream>
#include <thread>
#include <iomanip>
//#include "hydrosheds/hydroshedsTest.h"

using namespace hydrosheds;

/*
int main(int argc, char** argv) {


    std::string s = "/Users/halilibrahim/Desktop/HydroRIVERS_v10_eu/HydroRIVERS_v10_eu.gdb";
    HydroshedsDataSet D(s, 0);
*/
int main(int argc, char** argv)
{
        if(argc != 2)
        {
            std::cerr << "Usage: ./hydrosheds_app <path-to-gdb-directory>" << std::endl;
            exit(1);
        }

        // Initialise the data set.
        HydroshedsDataSet D(argv[1], 0);


    // works just fine
    auto itend = D.end();
    for(auto it = D.begin(); it!=itend; it++)
        std::cout << it->getfeature_index() << std::endl;

    // result in a segmentation fault
    /*
    for(auto it : D)
        std::cout << it.getGeologicalLength() << std::endl;
        */
    return 0;
}