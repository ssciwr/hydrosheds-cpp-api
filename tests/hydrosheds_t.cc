#include "hydrosheds/hydrosheds.hh"
#include "catch2/catch.hpp"


using namespace hydrosheds;


TEST_CASE("test shape", "[dataset]")
{
	std::string path = "/Users/anirudh/Documents/Internship-Hydrosheds/HydroRIVERS_v10_as.gdb";
	HydroshedsDataSet D(path, 0);
	REQUIRE(D.shape()[0] > 0);
	REQUIRE(D.shape()[1] > 0);
}
