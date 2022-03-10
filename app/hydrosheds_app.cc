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

    for(const auto& seg : D)
      std::cout << seg.getfeature_index() << std::endl;

    return 0;
}