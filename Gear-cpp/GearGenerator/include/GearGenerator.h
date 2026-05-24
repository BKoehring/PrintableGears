#pragma once

#include <godot_cpp/classes/node.hpp>
#include <stl.h>
#include <GearSpecs.h>
#include <vector>

class GearGenerator : public godot::Node {
	GDCLASS(GearGenerator, godot::Node)

protected:
	static void _bind_methods();
public:
	GearGenerator();
	~GearGenerator();

	std::vector<openstl::Triangle> generate(GearSpecs gearSpecs);
};