#include "hydrosheds/hydrosheds.hh"

#include<iostream>
//"/Users/halilibrahim/Desktop/HydroRIVERS_v10_eu/HydroRIVERS_v10_eu.gdb"


int main(int argc, char** argv)
{
    if (argc != 2) {
        std::cerr << "Usage: ./hydrosheds_app <path-to-gdb-directory>" << std::endl;
        return 1;
    }
    // Instantiate the data set
    hydrosheds::HydroshedsDataSet dataset(argv[1]);
    hydrosheds::Coordinate x = {49, 13};

    // funktioniert
    auto begin = dataset.begin();
    while(1)
    {
        begin++;
        if(begin==dataset.end()) {
            break;
        }
        std::cout << begin->getLength() << std::endl;
    }

    // funktioniert nicht
    /*
    for(auto it : dataset)
        std::cout << it.getLength() << std::endl;
*/
}