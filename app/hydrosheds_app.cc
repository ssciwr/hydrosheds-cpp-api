#include "hydrosheds/hydrosheds.hh"

#include<iostream>


int main(int argc, char** argv)
{
  if (argc != 2) {
    std::cerr << "Usage: ./hydrosheds_app <path-to-gdb-directory>" << std::endl;
    return 1;
  }

  hydrosheds::HydroshedsDataSet dataset(std::string(argv[1]));
  //std::cout << dataset.getSegment().getLength() << std::endl;

  return 0;
}