#include "hydrosheds/hydrosheds.hh"
#include <iostream>

using namespace hydrosheds;

int main(int argc, char** argv)
{
    if (argc != 2) 
    {
        std::cerr << "Usage: ./hydrosheds_app <path-to-gdb-directory>" << std::endl;
        return 1;
    }

    // Initialise the data set.
    HydroshedsDataSet D(argv[1], 0);

    // Initialise a river segment object.
    // RiverSegment R = D.ConstructSegment(-100.0, -50.0, 100.0, 50.0);
    RiverSegment R = D.ConstructSegment();
    // Initial testing
    std::cout << "Shape: " << "(" << D.shape()[0] 
                << ", "  << D.shape()[1] << ")" << std::endl;
                
    std::cout << "SUMMARY" << std::endl;
    R.summary(true);
    
    std::cout << "LENGTHS ------" << std::endl;
    std::cout << R.getLength() << std::endl;
    std::cout << R.getTotalLength() << std::endl;
    std::cout << R.getGeologicalLength() << std::endl;
    
    std::cout << "DOWNSTREAM ITERATION ------" << std::endl;
    RiverSegment R1 = R;
    for(int i = 1; i < 100; i++)
    {
        std::cout << "Getting downstream segments..." << std::endl;
        R1 = R1.getDownstreamSegment();
        std::cout << "Feature index: " << R1.getfeature_index() << std::endl;
        std::cout << "Current subsegment: " << R1.get_segment() << std::endl; 
    }
    return 0;
}
