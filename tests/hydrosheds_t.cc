#include "hydrosheds/hydrosheds.hh"
#include "catch2/catch.hpp"


using namespace hydrosheds;

std::string path = "/Users/anirudh/Documents/Internship-Hydrosheds/HydroRIVERS_v10_as.gdb";
HydroshedsDataSet D(path, 0);
RiverSegment R = D.ConstructSegment();	

/* -- DATASET TESTS -- */
TEST_CASE("Test shape", "[dataset]")
{
	REQUIRE(D.shape()[0] > 0);
	REQUIRE(D.shape()[1] > 0);
}

TEST_CASE("Field count", "[dataset]")
{
	REQUIRE(D.FeatureAttributes() > 0);
	REQUIRE(D.FeatureAttributes() == 15);
}

TEST_CASE("River segment subsegments.", "[RiverSegment]")
{
	// REQUIRE(std::is_same <decltype(R.getLength()), decltype(num)> == 1);
    RiverSegment R1 = R;
    for(int i = 1; i < 100; i++)
    {
		R1 = R1.getDownstreamSegment();
		SECTION("Different lengths.")
		{
			REQUIRE(R.getLength() >= 0.0);
			REQUIRE(R.getTotalLength() >= 0.0);
			REQUIRE(R.getGeologicalLength() >= 0.0);
		}
		
		SECTION("Checking number of subsegments")
		{
			CAPTURE(R.get_number_of_subsegments());
			REQUIRE(R1.get_number_of_subsegments() > 0);
		}

       
       
    }
}


