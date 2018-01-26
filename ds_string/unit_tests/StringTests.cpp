#include "..\src\catch.hpp"
#include "..\ds_string.h"

TEST_CASE("Empty string", "[ds_string]") {
	ds::string str;
	REQUIRE(str.is_small());
	REQUIRE(str.is_empty());
	REQUIRE(str.capacity() == 16);
}

TEST_CASE("Build small string", "[ds_string]") {
	ds::string str("Hello world");
	REQUIRE(str.is_small());
	REQUIRE_FALSE(str.is_empty());
	REQUIRE(str.capacity() == 16);
}

TEST_CASE("Build long string", "[ds_string]") {
	ds::string str("Hello world - 1234567890 - Test");
	REQUIRE_FALSE(str.is_small());
	REQUIRE_FALSE(str.is_empty());
	REQUIRE(str.capacity() == 48);
}

TEST_CASE("Clear string", "[ds_string]") {
	ds::string str("Hello world - 1234567890 - Test");
	REQUIRE_FALSE(str.is_small());
	REQUIRE_FALSE(str.is_empty());
	str.clear();
	REQUIRE_FALSE(str.is_small());
	REQUIRE(str.is_empty());
}

TEST_CASE("Resize string", "[ds_string]") {
	ds::string str("Hello world");
	REQUIRE(str.is_small());
	str.resize(64);
	REQUIRE_FALSE(str.is_small());
	REQUIRE(str.at(32) == '\0');
	REQUIRE(str.capacity() == 64);
}

TEST_CASE("Find string", "[ds_string]") {
	ds::string str("Hello world");
	size_t idx = 0;
	bool ret = str.find("ello", 0, &idx);
	REQUIRE(ret);
	REQUIRE(idx == 1);
	ret = str.find("xxx");
	REQUIRE_FALSE(ret);
	ret = str.find("Hello worldxxxxxxx");
	REQUIRE_FALSE(ret);
	ret = str.find("rld", 0, &idx);
	REQUIRE(ret);
	REQUIRE(idx == 8);
	ret = str.find("ello", 6, &idx);
	REQUIRE_FALSE(ret);
}