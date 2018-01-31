#include "..\..\ds_string\unit_tests\catch.hpp"
#include "..\ds_stretchbuffer.h"

struct TestData {
	int i;
	float v;
};

TEST_CASE("Push single value", "[ds_stretchbuffer]") {
	TestData* data = NULL;
	TestData td = { 100,1.23f };
	array_push(data, td);
	REQUIRE(array_size(data) == 1);
	REQUIRE(array_capacity(data) == 8);
	array_header* h = array_header(data);
	REQUIRE(array_size(data) == h->size);
	REQUIRE(array_capacity(data) == h->capacity);
	REQUIRE_FALSE(array_full(data));
	array_free(data);
}

TEST_CASE("Remove single value", "[ds_stretchbuffer]") {
	TestData* data = NULL;
	TestData td = { 100,1.23f };
	array_push(data, td);
	TestData nd = { 200, 2.34f };
	array_push(data, nd);
	REQUIRE(array_size(data) == 2);
	REQUIRE(array_capacity(data) == 8);
	array_remove(data, 0);
	REQUIRE(array_size(data) == 1);
	REQUIRE(data[0].i == 200);
	array_free(data);
}
