#include <PrintableGear.h>
#include <fstream>

void PrintableGear::_bind_methods() {
	godot::ClassDB::bind_method(godot::D_METHOD("SetModule", "gear_module"), &PrintableGear::SetModule);
	godot::ClassDB::bind_method(godot::D_METHOD("GetModule"), &PrintableGear::GetModule);

	godot::ClassDB::bind_method(godot::D_METHOD("SetNumberOfTeeth", "teeth_num"), &PrintableGear::SetNumberOfTeeth);
	godot::ClassDB::bind_method(godot::D_METHOD("GetNumberOfTeeth"), &PrintableGear::GetNumberOfTeeth);

	godot::ClassDB::bind_method(godot::D_METHOD("SetWidth", "width"), &PrintableGear::SetWidth);
	godot::ClassDB::bind_method(godot::D_METHOD("GetWidth"), &PrintableGear::GetWidth);

	godot::ClassDB::bind_method(godot::D_METHOD("SetProfileShift", "profileShift"), &PrintableGear::SetProfileShift);
	godot::ClassDB::bind_method(godot::D_METHOD("GetProfileShift"), &PrintableGear::GetProfileShift);

	godot::ClassDB::bind_method(godot::D_METHOD("SetPressureAngle", "pressureAngle"), &PrintableGear::SetPressureAngle);
	godot::ClassDB::bind_method(godot::D_METHOD("GetPressureAngle"), &PrintableGear::GetPressureAngle);

	godot::ClassDB::bind_method(godot::D_METHOD("SetInvoluteSteps", "involuteSteps"), &PrintableGear::SetInvoluteSteps);
	godot::ClassDB::bind_method(godot::D_METHOD("GetInvoluteSteps"), &PrintableGear::GetInvoluteSteps);

	godot::ClassDB::bind_method(godot::D_METHOD("SaveFile", "fileName"), &PrintableGear::SaveFile);

	godot::ClassDB::bind_method(godot::D_METHOD("GetVerts"), &PrintableGear::GetVerts);
	godot::ClassDB::bind_method(godot::D_METHOD("GetNormals"), &PrintableGear::GetNormals);
	godot::ClassDB::bind_method(godot::D_METHOD("GetIndices"), &PrintableGear::GetIndices);

	ADD_SIGNAL(godot::MethodInfo("gear_property_changed", godot::PropertyInfo(godot::Variant::OBJECT, "node")));

	ADD_PROPERTY(godot::PropertyInfo(godot::Variant::FLOAT, "_module", godot::PROPERTY_HINT_RANGE, "0.1,50.0,0.01"), "SetModule", "GetModule");
	ADD_PROPERTY(godot::PropertyInfo(godot::Variant::INT, "_number_of_teeth", godot::PROPERTY_HINT_RANGE, "8,100,1"), "SetNumberOfTeeth", "GetNumberOfTeeth");
	ADD_PROPERTY(godot::PropertyInfo(godot::Variant::FLOAT, "_width", godot::PROPERTY_HINT_RANGE, "0.01,700.0,0.01"), "SetWidth", "GetWidth");
	ADD_PROPERTY(godot::PropertyInfo(godot::Variant::FLOAT, "_profile_shift", godot::PROPERTY_HINT_RANGE, "-1.0,1.0,0.01"), "SetProfileShift", "GetProfileShift");
	ADD_PROPERTY(godot::PropertyInfo(godot::Variant::FLOAT, "_pressure_angle", godot::PROPERTY_HINT_RANGE, "14.5,25.0,0.01"), "SetPressureAngle", "GetPressureAngle");
	ADD_PROPERTY(godot::PropertyInfo(godot::Variant::INT, "_involute_steps", godot::PROPERTY_HINT_RANGE, "4,50,1"), "SetInvoluteSteps", "GetInvoluteSteps");
}

PrintableGear::PrintableGear() {
	generator.generate(specs);
}

PrintableGear::~PrintableGear() {

}

void PrintableGear::SetModule(float gear_module) {
	specs.SetModule(gear_module);
	RefreshGearMesh();
}

float PrintableGear::GetModule() {
	return specs.GetModule();
}

void PrintableGear::SetNumberOfTeeth(unsigned int teeth_num) {
	specs.SetNumberOfTeeth(teeth_num);
	RefreshGearMesh();
}

unsigned int PrintableGear::GetNumberOfTeeth() {
	return specs.GetNumberOfTeeth();
}

void PrintableGear::SetWidth(float width) {
	specs.SetWidth(width);
	RefreshGearMesh();
}

float PrintableGear::GetWidth() {
	return specs.GetWidth();
}

void PrintableGear::SetProfileShift(float profileShift) {
	specs.SetProfileShift(profileShift);
	RefreshGearMesh();
}

float PrintableGear::GetProfileShift() {
	return specs.GetProfileShift();
}

void PrintableGear::SetPressureAngle(float pressureAngle) {
	specs.SetPressureAngle(pressureAngle);
	RefreshGearMesh();
}

float PrintableGear::GetPressureAngle() {
	return specs.GetPressureAngle();
}

void PrintableGear::SetInvoluteSteps(unsigned int involuteSteps) {
	specs.SetInvoluteSteps(involuteSteps);
	RefreshGearMesh();
}

unsigned int PrintableGear::GetInvoluteSteps() {
	return specs.GetInvoluteSteps();
}

bool PrintableGear::SaveFile(godot::String fileName)
{
	const auto& triangles = generator.GetTriangles();

	std::string name = std::string(fileName.utf8().get_data());

	std::ofstream file(name, std::ios::binary);
	if (!file.is_open()) {
		std::cerr << "Error: Unable to open file '" << name << "'" << std::endl;
	}

	openstl::serialize(triangles, file, openstl::StlFormat::Binary);

	if (file.fail()) {
		std::cerr << "Error: Failed to write to file " << name << std::endl;
	}
	else {
		std::cout << "File " << name << " has been successfully written." << std::endl;
	}
	file.close();

	return true;
}

void PrintableGear::RefreshGearMesh()
{
	generator.generate(specs);
	emit_signal("gear_property_changed", this);
}

const godot::PackedVector3Array& PrintableGear::GetVerts()
{
	return generator.GetVerts();
}

const godot::PackedVector3Array& PrintableGear::GetNormals()
{
	return generator.GetNormals();
}

const godot::PackedInt32Array& PrintableGear::GetIndices()
{
	return generator.GetIndices();
}
