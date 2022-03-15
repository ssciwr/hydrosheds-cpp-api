#include "hydrosheds/hydrosheds.hh"
#include "catch2/catch.hpp"

using namespace hydrosheds;

std::string path = "../../tests/test_data/HydroRIVERS_v10_gr.gdb/HydroRIVERS_v10_gr.gdb";

HydroshedsDataSet D(path.c_str());
RiverSegment R = D.ConstructSegment(30);	

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
	
    RiverSegment R1 = R;
    for(int i = 1; i < 20; i++)
    {
		R1 = R1.getDownstreamSegment();
		
		SECTION("Different lengths type.")
		{
			REQUIRE(std::is_same <decltype(R1.getLength()), double>::value == true);
			REQUIRE(std::is_same <decltype(R1.getTotalLength()), double>::value == true);
			REQUIRE(std::is_same <decltype(R1.getGeologicalLength()), double>::value == true);
			
		}
		
		SECTION("Different lengths' value.")
		{
			REQUIRE(R1.getLength() >= 0.0);
			REQUIRE(R1.getTotalLength() >= 0.0);
			REQUIRE(R1.getGeologicalLength() >= 0.0);
		}
		
		SECTION("Check number of subsegments.")
		{
			// CAPTURE(R.get_number_of_subsegments());
			REQUIRE(R1.getNumberOfSegments() > 0);
		}

		SECTION("Discharge.")
		{
			REQUIRE(std::is_same <decltype(R1.getDischarge()), double>::value == true);
			REQUIRE(R1.getDischarge() >= 0.0);
		}

		SECTION("Starting point of subsegment.")
		{}  
    }
}


