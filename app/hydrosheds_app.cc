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



    for(auto it = dataset.begin(); 1; ++it)
    {
        std::cout << it->getLength() << std::endl;
    }

    std::cout << "Before the loop " << std::endl;
    auto start = dataset.begin();
    std::cout << start->getLength() << std::endl;

    /// using the free function
/*
    while(1)
    {
        auto next = increment(start);
        std::cout << start->getLength() << std::endl;
        start = next;
    }*/
/*
    while(1) {
        auto next = start.layer->GetNextFeature();
        hydrosheds::FullDatasetRiverSegmentIterator it1(next, start.layer);
        if(dataset.end() == it1)
        {
            break;
        }
        std::cout << it1->getLength() << std::endl;
    }*/

/*
    while(1)
    {
        start++;
        std::cout << start->getLength() << std::endl;
    }
    std::cout << "After loop" << std::endl;
*/


  return 0;
}
