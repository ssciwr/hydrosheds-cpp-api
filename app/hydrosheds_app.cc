#include "hydrosheds/hydrosheds.hh"
#include <iostream>
#include <thread>
#include <iomanip>

using namespace hydrosheds;

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
        std::cout << seg.getGeologicalLength() << std::endl;
    return 0;
}