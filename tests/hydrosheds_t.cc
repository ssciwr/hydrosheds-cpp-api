#include "hydrosheds/hydrosheds.hh"
#include "catch2/catch.hpp"

using namespace hydrosheds;

TEST_CASE( "Range-based for", "[FullDatasetRiverSegment]" ){
}


/*
std::string s = "/Users/halilibrahim/Desktop/HydroRIVERS_v10_eu/HydroRIVERS_v10_eu.gdb";

HydroshedsDataSet D(s, 0);

TEST_CASE( "Range-based for", "[FullDatasetRiverSegment]" ){
    int count = 1;
    for(auto it : D)
{
        count++;
        auto index = it.getfeature_index();
        REQUIRE(count == index);
}
}

TEST_CASE("==operator", "[FullDatasetRiverSegmentIterator]")
{
    auto begin1 = D.begin();
auto begin2 = D.begin();
REQUIRE(begin1==begin2);
}


TEST_CASE("!=operator", "[FullDatasetRiverSegmentIterator]")
{
auto begin1 = D.begin();
auto begin2 = D.begin();
begin2++;
REQUIRE(begin1!=begin2);
}

TEST_CASE("->operator", "[FullDatasetRiverSegmentIterator]")
{
    auto tmp = D.begin();
    REQUIRE(tmp->getGeologicalLength()>0);
}


TEST_CASE("incrementing", "[FullDatasetRiverSegmentIterator]")
{
auto tmp = D.begin();
auto ID_1 = tmp->get_feature_index();
tmp++;
auto ID_2 = tmp->get_feature_index();
REQUIRE(ID_1+1==ID_2);
}

TEST_CASE("Range-based for", "[DownstreamIterator]")
{
    HydroshedsDataSet D(s);
    Coordinate x = {12, 40};
    for(auto i : followstream(D, x))
{
        REQUIRE(i.getGeologicalLength()>0);
}
}

TEST_CASE("begin-method", "[DownstreamIterator]")
{
HydroshedsDataSet D(s);
Coordinate x = {12, 40};
auto begin = D.followbegin(x);
for(auto i : D)
{
    REQUIRE(impl::norm(begin.x, x)<impl::norm(i.x, x));
}
}

TEST_CASE("== operator", "[DownstreamIterator]")
{
HydroshedsDataSet D(s);
Coordinate x = {12, 40};
auto begin1 = D.followbegin(x);
auto begin2 = D.followbegin(x);
REQUIRE(begin1 == begin2);
}

TEST_CASE("!= operator", "[DownstreamIterator]")
{
HydroshedsDataSet D(s);
Coordinate x = {12, 40};
auto begin1 = D.followbegin(x);
begin1++;
auto begin2 = D.followbegin(x);
REQUIRE(begin1 != begin2);
}


TEST_CASE("-> operator", "[DownstreamIterator]")
{
HydroshedsDataSet D(s);
Coordinate x = {12, 40};
auto begin1 = D.followbegin(x);
REQUIRE(begin1->getGeologicalLength()>0);
}
*/