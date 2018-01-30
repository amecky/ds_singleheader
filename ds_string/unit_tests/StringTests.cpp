#include "catch.hpp"
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

TEST_CASE("Compare string", "[ds_string]") {	
	ds::string str("Hello world");
	bool ret = str.compare("Hello");
	REQUIRE_FALSE(ret);
	ds::string next("Hello world");
	REQUIRE(str.compare(next.c_str()));
	REQUIRE(str.compare(next));
	REQUIRE(str.compare("hELLo WORLD",true));	
}

TEST_CASE("Append string", "[ds_string]") {
	ds::string str("Hello world");
	REQUIRE(str.is_small());
	str.append(" here is more to come");
	REQUIRE_FALSE(str.is_small());
	REQUIRE(strcmp(str.c_str(), "Hello world here is more to come") == 0);
}

TEST_CASE("Append float to string", "[ds_string]") {
	ds::string nr;
	nr.append(123.45f);
	REQUIRE(nr.is_small());
	REQUIRE(strcmp(nr.c_str(), "123.45") == 0);
}

TEST_CASE("Append int to string", "[ds_string]") {
	ds::string nr;
	nr.append(1024);
	REQUIRE(nr.is_small());
	REQUIRE(strcmp(nr.c_str(), "1024") == 0);
}

TEST_CASE("Append double to string", "[ds_string]") {
	ds::string nr;
	nr.append(1024.66);
	REQUIRE(nr.is_small());
	REQUIRE(strcmp(nr.c_str(), "1024.66") == 0);
}

TEST_CASE("Operator +=", "[ds_string]") {
	ds::string nr("Hello");
	nr += ds::string(" world");
	REQUIRE(nr.is_small());
	REQUIRE(strcmp(nr.c_str(), "Hello world") == 0);
}

TEST_CASE("sprintf string", "[ds_string]") {
	ds::string nr;
	nr.sprintf("%d = %g and %s", 100, 34.56f, "Hello world");
	REQUIRE_FALSE(nr.is_small());
	REQUIRE(strcmp(nr.c_str(), "100 = 34.56 and Hello world") == 0);
	ds::string str = nr.sprintf("%d / %d / %d", 100, 200, 300);
	REQUIRE(strcmp(str.c_str(), "100 / 200 / 300") == 0);
}

TEST_CASE("Substr - basic", "[ds_string]") {
	ds::string nr("Hello");
	ds::string sub = nr.substr(0, 2);
	REQUIRE(sub.is_small());
	REQUIRE(strcmp(sub.c_str(), "Hel") == 0);
}

TEST_CASE("Substr - too large", "[ds_string]") {
	ds::string nr("Hello");
	ds::string sub = nr.substr(0, 20);
	REQUIRE(sub.is_small());
	REQUIRE(strcmp(sub.c_str(), "Hello") == 0);
}

TEST_CASE("Substr - offset and too large", "[ds_string]") {
	ds::string nr("Hello");
	ds::string sub = nr.substr(3, 20);
	REQUIRE(sub.is_small());
	REQUIRE(strcmp(sub.c_str(), "lo") == 0);
}

TEST_CASE("Substr - offset", "[ds_string]") {
	ds::string nr("Hello");
	ds::string sub = nr.substr(3);
	REQUIRE(sub.is_small());
	REQUIRE(strcmp(sub.c_str(), "lo") == 0);
}

TEST_CASE("Substr - offset too large", "[ds_string]") {
	ds::string nr("Hello");
	ds::string sub = nr.substr(30);
	REQUIRE(sub.is_small());
	REQUIRE(sub.size() == 0);
	REQUIRE(sub.is_empty());
	REQUIRE(strcmp(sub.c_str(), "") == 0);
}

TEST_CASE("Erase - one char", "[ds_string]") {
	ds::string nr("Hello");
	nr.erase(2);
	REQUIRE(strcmp(nr.c_str(), "Helo") == 0);
}

TEST_CASE("Erase - one char out of scope", "[ds_string]") {
	ds::string nr("Hello");
	nr.erase(20);
	REQUIRE(strcmp(nr.c_str(), "Hello") == 0);
}

TEST_CASE("Erase - 3 chars", "[ds_string]") {
	ds::string nr("Hello");
	nr.erase(1,3);
	REQUIRE(strcmp(nr.c_str(), "H") == 0);
}

TEST_CASE("Erase - too much chars", "[ds_string]") {
	ds::string nr("Hello");
	nr.erase(1, 30);
	REQUIRE(strcmp(nr.c_str(), "H") == 0);
}

TEST_CASE("Replace one char", "[ds_string]") {
	ds::string nr("Hello");
	nr.replace('l', 'm');
	REQUIRE(strcmp(nr.c_str(), "Hemmo") == 0);
}

TEST_CASE("Replace one char case sensitive", "[ds_string]") {
	ds::string nr("Hello");
	nr.replace('L', 'm', true);
	REQUIRE(strcmp(nr.c_str(), "Hello") == 0);
}

TEST_CASE("Insert string", "[ds_string]") {
	ds::string nr("Hello");
	ds::string is("world");
	nr.insert(3, is);
	printf("nr: %s\n", nr.c_str());
	REQUIRE(strcmp(nr.c_str(), "Hello") == 0);
}