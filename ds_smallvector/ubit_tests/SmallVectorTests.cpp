#include "..\..\ds_string\unit_tests\catch.hpp"
#include "..\ds_smallvector.h"

TEST_CASE("ds_smallvector - Push single value", "[ds_smallvector]") {
	ds::smallvector<int> v;
	v.push_back(100);
	REQUIRE(v.size() == 1);	
}

TEST_CASE("ds_smallvector - Erase value", "[ds_smallvector]") {
	ds::smallvector<int> v;
	v.push_back(100);
	v.push_back(200);
	v.push_back(300);
	v.push_back(400);
	v.push_back(500);
	REQUIRE(v.size() == 5);
	REQUIRE_FALSE(v.is_empty());
	bool ret = v.remove(2);
	REQUIRE(ret);
	REQUIRE(v.size() == 4);
	REQUIRE(v[2] == 400);
	ret = v.remove(200);
	REQUIRE_FALSE(ret);
}

TEST_CASE("ds_smallvector - Erase last value", "[ds_smallvector]") {
	ds::smallvector<int> v;
	v.push_back(100);
	REQUIRE(v.size() == 1);
	bool ret = v.remove(0);
	REQUIRE(ret);
	REQUIRE(v.size() == 0);
	REQUIRE(v.is_empty());
	
}