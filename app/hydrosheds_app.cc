#include "hydrosheds/hydrosheds.hh"

#include<iostream>



int main(int argc, char** argv)
{
    /*
    if (argc != 2) {
        std::cerr << "Usage: ./hydrosheds_app <path-to-gdb-directory>" << std::endl;
        return 1;
    }
*/
    std::string tmp ="/Users/halilibrahim/Desktop/HydroRIVERS_v10_eu/HydroRIVERS_v10_eu.gdb";
    // Instantiate the data set
    hydrosheds::HydroshedsDataSet dataset(tmp);

    // ./hydrosheds_app /Users/halilibrahim/Desktop/HydroRIVERS_v10_eu/HydroRIVERS_v10_eu.gdb
    /*
    for(auto it = dataset.followbegin(); it != dataset.followend(); ++it)
        std::cout << it->getLength() << std::endl;
        */
    hydrosheds::Coordinate x = {49, 12};
   /* for(auto it = dataset.followbegin(); it != dataset.followend(); ++it)
    {
        std::cout << it.getLength() << std::endl;
    }
*/
   for(auto it : dataset)
   {
       std::cout << it.getLength() << std::endl;
   }
        }

/* TODO: -implement followend() und followbegin()
 *      - implement followstream()
 *      - implement begin() and end() in DownstreamIteratorHelper
 *      - MAKE IT WORK
*/