#include "hydrosheds/hydrosheds.hh"

#include<iostream>



int main(int argc, char** argv)
{
    if (argc != 2) {
        std::cerr << "Usage: ./hydrosheds_app <path-to-gdb-directory>" << std::endl;
        return 1;
    }

    // Instantiate the data set
    hydrosheds::HydroshedsDataSet dataset(argv[1]);

    // ./hydrosheds_app /Users/halilibrahim/Desktop/HydroRIVERS_v10_eu/HydroRIVERS_v10_eu.gdb
    std::cout << "Checking some things: " << std::endl;
/**
    /// running it with prefix
    for(auto it = dataset.begin(); it!=dataset.end(); ++it)
        std::cout << it->getLength() << std::endl;
*/
/**
    /// running it with postfix
    for(auto it = dataset.begin(); it!=dataset.end(); it++)
        std::cout << it->getLength() << std::endl;
*/



    /// ranged based loop
    for(auto it : dataset)
        std::cout << it.getLength() << std::endl;

    return 0;
}