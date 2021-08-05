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
}