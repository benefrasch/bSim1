#pragma once
#include <vector>
struct wire_part
{
	long double x;
	long double y;
	long double z;
	long double rotation_x; //from x to y axis
	long double rotation_z; //from y to z axis
	long double length;
};

static class Wire_parts {

public:
	static std::vector<wire_part> ring();
};