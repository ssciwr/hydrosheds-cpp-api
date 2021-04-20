#include "hydrosheds-cpp-api/hydrosheds-cpp-api.hpp"
#include <iostream>

int main(){
  int result = hydroshedscppapi::add_one(1);
  std::cout << "1 + 1 = " << result << std::endl;
}