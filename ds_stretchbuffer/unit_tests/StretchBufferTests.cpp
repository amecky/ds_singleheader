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
	REQUIRE(array_count(data) == 100);
	array_free(data);
}