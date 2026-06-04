#pragma once

#include <stl.h>
#include <GearSpecs.h>
#include <vector>
#include <godot_cpp/variant/packed_vector3_array.hpp>
#include <godot_cpp/variant/packed_int32_array.hpp>

class GearGenerator {

public:
	GearGenerator();
	~GearGenerator();

	void generate(GearSpecs gearSpecs);

	const std::vector<openstl::Triangle>& GetTriangles();
	const godot::PackedVector3Array& GetVerts();
	const godot::PackedVector3Array& GetNormals();
	const godot::PackedInt32Array& GetIndices();

private:
	std::vector<openstl::Triangle> triangles;
	godot::PackedVector3Array verts;
	godot::PackedVector3Array normals;
	godot::PackedInt32Array indices;

	// Returns the next index
	unsigned int SetVert(unsigned int index, godot::Vector3 vert, godot::Vector3 normal);
	openstl::Vec3 ConvertVec3(godot::Vector3 vec);
};