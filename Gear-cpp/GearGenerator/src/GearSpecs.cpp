#include <GearSpecs.h>
#include <cmath>
#include <Util.h>

GearSpecs::GearSpecs() : 
	_module{ 10 },
	_number_of_teeth{24},
	_width{ 15 },
	_profile_shift{ 0.0 },
	_pressure_angle{ 20.0 },
	_reference_diameter{ _module * _number_of_teeth },
	_base_diameter{ _reference_diameter * cos(_pressure_angle * static_cast<float>(std::numbers::pi) / 180.0f) },
	_base_radius{ _base_diameter / 2.0f },
	_involute_steps{ 4 },
	_tip_diameter{ _reference_diameter + 2 * _module * (1 + _profile_shift) },
	_tip_radius{ _tip_diameter / 2.0f },
	_u{ static_cast<float>(sqrt((pow(_tip_radius, 2) / pow(_base_radius, 2)) - 1) )},
	_tip_pressure_angle{ static_cast<float>(acos(_base_diameter / _tip_diameter) * 180.0 / std::numbers::pi) },
	_inv_alpha{ static_cast<float>(tan(_pressure_angle * std::numbers::pi / 180.0) - _pressure_angle * std::numbers::pi / 180.0) },
	_inv_alpha_a{ static_cast<float>(tan(_tip_pressure_angle * std::numbers::pi / 180.0) - _tip_pressure_angle * std::numbers::pi / 180.0) },
	_top_thickness{ static_cast<float>(std::numbers::pi) / (2.0f * _number_of_teeth) + _inv_alpha - _inv_alpha_a },
	_end_x{ _base_radius * static_cast<float>(cos(_u) + _u * sin(_u)) },
	_end_y{ _base_radius * static_cast<float>(sin(_u) - _u * cos(_u)) },
	_distance{ static_cast<float>(sqrt(pow(_base_radius - _end_x, 2) + pow(_end_y, 2))) },
	_cosx{ (static_cast<float>(pow(_base_radius, 2) + pow(_tip_radius, 2) - pow(_distance, 2))) / 2.0f / _base_radius / _tip_radius },
	_tooth_thickness_rad{ static_cast < float>(2.0 * _top_thickness + 2.0 * acos(_cosx)) },
	_spacing_arc_length{ Util::DegreesToRadians(360.0 / _number_of_teeth) - _tooth_thickness_rad },
	_max_width{ _width / 2.0f },
	_min_width{ _width / -2.0f },
	_radial_segments{ static_cast<int>(_number_of_teeth * _involute_steps * SECTIONS_PER_TOOTH) }
{
	// Do nothing
}

GearSpecs::~GearSpecs() {

}

void GearSpecs::UpdateRefDiameter() {
	_reference_diameter = _module * _number_of_teeth;
	UpdateBaseDiameter();
	UpdateTipDiameter();
}

void GearSpecs::UpdateBaseDiameter() {
	_base_diameter = _reference_diameter * cos(_pressure_angle * std::numbers::pi / 180.0);
	UpdateBaseRadius();
	UpdateTipPressureAngle();
}

void GearSpecs::UpdateBaseRadius() {
	_base_radius = _base_diameter / 2.0;
	UpdateU();
	UpdateEndX();
	UpdateEndY();
	UpdateDistance();
	UpdateCosX();
}

void GearSpecs::UpdateTipDiameter() {
	_tip_diameter = _reference_diameter + 2.0 * _module * (1.0 + _profile_shift);
	UpdateTipRadius();
	UpdateTipPressureAngle();
}

void GearSpecs::UpdateTipRadius() {
	_tip_radius = _tip_diameter / 2.0;
	UpdateU();
	UpdateCosX();
}

void GearSpecs::UpdateU() {
	_u = sqrt((pow(_tip_radius, 2) / pow(_base_radius, 2)) - 1);
	UpdateEndX();
	UpdateEndY();
}

void GearSpecs::UpdateTipPressureAngle() {
	_tip_pressure_angle = acos(_base_diameter / _tip_diameter) * 180.0 / std::numbers::pi;
	UpdateInverseAngleA();
}

void GearSpecs::UpdateInverseAngle() {
	_inv_alpha = tan(_pressure_angle * std::numbers::pi / 180.0) - _pressure_angle * std::numbers::pi / 180.0;
	UpdateTopThickness();
}

void GearSpecs::UpdateInverseAngleA() {
	_inv_alpha_a = tan(_tip_pressure_angle * std::numbers::pi / 180.0) - _tip_pressure_angle * std::numbers::pi / 180.0;
	UpdateTopThickness();
}

void GearSpecs::UpdateTopThickness() {
	_top_thickness = std::numbers::pi / (2.0 * _number_of_teeth) + _inv_alpha - _inv_alpha_a;
	UpdateToothThicknessRad();
}

void GearSpecs::UpdateEndX() {
	_end_x = _base_radius * (cos(_u) + _u * sin(_u));
	UpdateDistance();
}

void GearSpecs::UpdateEndY() {
	_end_y = _base_radius * (sin(_u) - _u * cos(_u));
	UpdateDistance();
}

void GearSpecs::UpdateDistance() {
	_distance = sqrt(pow(_base_radius - _end_x, 2) + pow(_end_y, 2));
	UpdateCosX();
}

void GearSpecs::UpdateCosX() {
	_cosx = (pow(_base_radius, 2) + pow(_tip_radius, 2) - pow(_distance, 2)) / 2.0 / _base_radius / _tip_radius;
	UpdateToothThicknessRad();
}

void GearSpecs::UpdateToothThicknessRad() {
	_tooth_thickness_rad = 2.0 * _top_thickness + 2.0 * acos(_cosx);
	UpdateSpacingArcLength();
}

void GearSpecs::UpdateSpacingArcLength() {
	_spacing_arc_length = Util::DegreesToRadians(360.0 / _number_of_teeth) - _tooth_thickness_rad;
}

void GearSpecs::UpdateMaxWidth() {
	_max_width = _width / 2.0;
}

void GearSpecs::UpdateMinWidth() {
	_min_width = _width / -2.0;
}

void GearSpecs::UpdateRadialSegments() {
	_radial_segments = _number_of_teeth * _involute_steps * SECTIONS_PER_TOOTH;
}

void GearSpecs::SetModule(float gear_module) {
	_module = gear_module;
	UpdateRefDiameter();
	UpdateTipDiameter();
}

float GearSpecs::GetModule() {
	return _module;
}

void GearSpecs::SetNumberOfTeeth(unsigned int teeth_num) {
	_number_of_teeth = teeth_num;
	UpdateRefDiameter();
	UpdateTopThickness();
	UpdateSpacingArcLength();
	UpdateRadialSegments();
}

unsigned int GearSpecs::GetNumberOfTeeth() {
	return _number_of_teeth;
}

void GearSpecs::SetWidth(float width) {
	_width = width;
	UpdateMaxWidth();
	UpdateMinWidth();
}

float GearSpecs::GetWidth() {
	return _width;
}

void GearSpecs::SetProfileShift(float profileShift) {
	_profile_shift = profileShift;
	UpdateTipDiameter();
}

float GearSpecs::GetProfileShift() {
	return _profile_shift;
}

void GearSpecs::SetPressureAngle(float pressureAngle) {
	_pressure_angle = pressureAngle;
	UpdateBaseDiameter();
	UpdateInverseAngle();
}

float GearSpecs::GetPressureAngle() {
	return _pressure_angle;
}

void GearSpecs::SetInvoluteSteps(unsigned int involuteSteps) {
	_involute_steps = involuteSteps;
	UpdateRadialSegments();
}

unsigned int GearSpecs::GetInvoluteSteps() {
	return _involute_steps;
}

float GearSpecs::GetReferenceDiameter() {
	return _reference_diameter;
}

float GearSpecs::GetBaseDiameter() {
	return _base_diameter;
}

float GearSpecs::GetBaseRadius() {
	return _base_radius;
}

float GearSpecs::GetTipDiameter() {
	return _tip_diameter;
}

float GearSpecs::GetTipRadius() {
	return _tip_radius;
}

float GearSpecs::GetU() {
	return _u;
}

float GearSpecs::GetTipPressureAngle() {
	return _tip_pressure_angle;
}

float GearSpecs::GetInverseAlpha() {
	return _inv_alpha;
}

float GearSpecs::GetInverseAlphaA() {
	return _inv_alpha_a;
}

float GearSpecs::GetTopThickness() {
	return _top_thickness;
}

float GearSpecs::GetEndX() {
	return _end_x;
}

float GearSpecs::GetEndY() {
	return _end_y;
}

float GearSpecs::GetDistance() {
	return _distance;
}

float GearSpecs::GetCosX() {
	return _cosx;
}

float GearSpecs::GetToothThicknessRed() {
	return _tooth_thickness_rad;
}

float GearSpecs::GetSpacingArcLength() {
	return _spacing_arc_length;
}

float GearSpecs::GetMaxWidth() {
	return _max_width;
}

float GearSpecs::GetMinWidth() {
	return _min_width;
}

int GearSpecs::GetRadialSegments() {
	return _radial_segments;
}