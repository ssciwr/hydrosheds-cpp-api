#include "hydrosheds/rasterize.hh"

#include<iostream>


int main(int argc, char** argv)
{
    if (argc != 2) {
        std::cerr << "Usage: ./rasterize_app <path-to-gdb-directory>" << std::endl;
        return 1;
    }
    // Instantiate the data set
    hydrosheds::HydroshedsDataSet dataset(argv[1]);
}