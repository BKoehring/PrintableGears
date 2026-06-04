#pragma once

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <GearGenerator.h>

class PrintableGear : public godot::Node {
	GDCLASS(PrintableGear, godot::Node)

protected:
	static void _bind_methods();

public:
	PrintableGear();
	~PrintableGear();

	void SetModule(float gear_module);
	float GetModule();

	void SetNumberOfTeeth(unsigned int teeth_num);
	unsigned int GetNumberOfTeeth();

	void SetWidth(float width);
	float GetWidth();

	void SetProfileShift(float profileShift);
	float GetProfileShift();

	void SetPressureAngle(float pressureAngle);
	float GetPressureAngle();

	void SetInvoluteSteps(unsigned int involuteSteps);
	unsigned int GetInvoluteSteps();

	bool SaveFile(godot::String fileName);

	void RefreshGearMesh();

	const godot::PackedVector3Array& GetVerts();
	const godot::PackedVector3Array& GetNormals();
	const godot::PackedInt32Array& GetIndices();

private:
	GearSpecs specs;
	GearGenerator generator;
};