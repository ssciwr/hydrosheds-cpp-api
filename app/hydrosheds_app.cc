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

  // Printing some example data
  auto segment = dataset.getSegment();
  auto start = segment.getDownstreamSegment().getStartingPoint();
  auto end = segment.getDownstreamSegment().getEndPoint();

  std::cout << "Start point: " << start[0] << " " << start[1] << std::endl;
  std::cout << "End point: " << end[0] << " " << end[1] << std::endl;
  std::cout << "Has downstream: " << (segment.hasDownstreamSegment() ? "yes" : "no") << std::endl;
  std::cout << "Approximation Length: " << segment.getLength() << std::endl;
  std::cout << "Geological Length: " << segment.getGeologicalLength() << std::endl;

  return 0;
}