#include "hydrosheds/hydrosheds.hh"
#include <iostream>

using namespace hydrosheds;

int main(int argc, char** argv)
{
    if (argc != 2) 
    {
        std::cerr << "Usage: ./hydrosheds_app <path-to-gdb-directory>" << std::endl;
        exit(1);
    }

    // Initialise the data set.
    HydroshedsDataSet D(argv[1]);

    // Initial testing
    std::cout << "Shape of dataset: " << "(" << D.shape()[0] << ", "  << D.shape()[1] << ")" << std::endl;

    // Fields in the dataset.
    // D.FeatureAttributes();

    // Initialise a river segment object.
    // RiverSegment R = D.ConstructSegment(-100.0, -50.0, 100.0, 50.0, false, 3);
    // std::cout << "Shape: " << "(" << D.shape()[0] 
    //             << ", "  << D.shape()[1] << ")" << std::endl;

    std::cout << "Dataset iteration." << std::endl;
    for (const auto& seg : D)
    {
        std::cout << seg.GetFeatureIndex() << "\n";
    }

   
                
    // std::cout << "SUMMARY" << std::endl;
    // R.summary(true);
    
    RiverSegment R = D.ConstructSegment(50);
    std::cout << "LENGTHS ------" << std::endl;
    std::cout << R.GetLength() << std::endl;
    std::cout << R.GetTotalLength() << std::endl;
    std::cout << R.GetGeologicalLength() << std::endl;
    
    std::cout << "DOWNSTREAM ITERATION ------" << std::endl;
    RiverSegment R1 = R;
    for(int i = 1; i < 20; i++)
    {
        std::cout << "Getting downstream segments..." << std::endl;
        R1 = R1.GetDownstreamSegment();
        std::cout << "Feature index: " << R1.GetFeatureIndex() << std::endl;
        std::cout << "Current subsegment: " << R1.GetCurrentSegment() << std::endl; 
    }

    return 0;
}

