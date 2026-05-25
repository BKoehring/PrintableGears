#pragma once

#include <stl.h>
#include <GearSpecs.h>
#include <vector>
#include <godot_cpp/variant/packed_vector3_array.hpp>

class GearGenerator {

public:
	GearGenerator();
	~GearGenerator();

	void generate(GearSpecs gearSpecs);

	const std::vector<openstl::Triangle>& GetTriangles();
	const godot::PackedVector3Array& GetVectors();

private:
	std::vector<openstl::Triangle> triangles;
	godot::PackedVector3Array ordered_vectors;
};