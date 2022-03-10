#include "hydrosheds/hydrosheds.hh"
#include "catch2/catch.hpp"
#include <iostream>

using namespace hydrosheds;

//std::string s = "/Users/halilibrahim/Desktop/HydroRIVERS_v10_eu/HydroRIVERS_v10_eu.gdb";

//Important: when trying to run the testcase, add the path
HydroshedsDataSet D(s, 0);

TEST_CASE( "Range-based for const&", "[FullDatasetRiverSegment]" ){
    for(const auto& it : D)
    {
        REQUIRE(it.getGeologicalLength()>0);
    }
}

TEST_CASE( "Range-based for &", "[FullDatasetRiverSegment]" ){
for(auto& it : D)
{
REQUIRE(it.getGeologicalLength()>0);
}
}

TEST_CASE("==operator", "[FullDatasetRiverSegmentIterator]")
{
    FullDatasetRiverSegmentIterator first;
    FullDatasetRiverSegmentIterator second;
    REQUIRE(first.operator==(second));
}


TEST_CASE("!=operator", "[FullDatasetRiverSegmentIterator]")
{
    FullDatasetRiverSegmentIterator first;
    auto second = D.begin();
    REQUIRE(first.operator!=(second));
}

auto test1 = D.begin();

TEST_CASE("->operator", "[FullDatasetRiverSegmentIterator]")
{
    REQUIRE(test1.operator->()->getGeologicalLength()>0);
}

TEST_CASE("pre fix incrementing", "[FullDatasetRiverSegmentIterator]")
{
    auto test2 = test1.operator++();
    REQUIRE(test1->getfeature_index() == test2->getfeature_index());
}


TEST_CASE("post fix incrementing", "[FullDatasetRiverSegmentIterator]")
{
    auto begin = D.begin();
    begin++;
    auto second = begin++;
    int second_index = second->getfeature_index();
    int first_index = begin->getfeature_index();
    bool result = second_index+1==first_index;
    REQUIRE(result);
}