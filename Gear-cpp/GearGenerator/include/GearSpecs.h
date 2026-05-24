#pragma once

#include <godot_cpp/classes/node.hpp>

class GearSpecs : public godot::Node {
	GDCLASS(GearSpecs, godot::Node)

protected:
	static void _bind_methods();

public:
	const unsigned int CENTER_RINGS = 2;
	const unsigned int SECTIONS_PER_TOOTH = 3;

	GearSpecs();
	~GearSpecs();

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

	float GetReferenceDiameter();

	float GetBaseDiameter();

	float GetBaseRadius();

	float GetTipDiameter();

	float GetTipRadius();
	
	float GetU();

	float GetTipPressureAngle();

	float GetInverseAlpha();

	float GetInverseAlphaA();

	float GetTopThickness();

	float GetEndX();

	float GetEndY();

	float GetDistance();

	float GetCosX();

	float GetToothThicknessRed();

	float GetSpacingArcLength();

	float GetMaxWidth();

	float GetMinWidth();

	int GetRadialSegments();

private:
	//UPROPERTY(EditAnywhere, Category = "Gear Properties", Meta = (Units = "Millimeters", ClampMin = 0.1, ClampMax = 50.0));
	float _module;

	//UPROPERTY(EditAnywhere, Category = "Gear Properties", Meta = (ClampMin = 8, ClampMax = 100));
	unsigned int _number_of_teeth;

	//UPROPERTY(EditAnywhere, Category = "Gear Properties", Meta = (Units = "Millimeters", ClampMin = .01, ClampMax = 700.0));
	float _width;

	//UPROPERTY(EditAnywhere, Category = "Gear Properties", Meta = (Units = "Millimeters", ClampMin = 0.0, ClampMax = 10.0));
	float _profile_shift;

	//UPROPERTY(EditAnywhere, Category = "Gear Properties", Meta = (Units = "Degrees", ClampMin = 14.5, ClampMax = 25.0));
	float _pressure_angle;

	//UPROPERTY(VisibleAnywhere, Category = "Gear Properties", Meta = (Units = "Millimeters"));
	float _reference_diameter;

	//UPROPERTY(VisibleAnywhere, Category = "Gear Properties", Meta = (Units = "Millimeters"));
	float _base_diameter;

	//UPROPERTY(VisibleAnywhere, Category = "Gear Properties", Meta = (Units = "Millimeters"));
	float _base_radius;

	//UPROPERTY(EditAnywhere, AdvancedDisplay, Meta = (ClampMin = 4, ClampMax = 50));
	unsigned int _involute_steps;

	float _tip_diameter;
	float _tip_radius;
	float _u;
	float _tip_pressure_angle;
	float _inv_alpha;
	float _inv_alpha_a;
	float _top_thickness;
	float _end_x;
	float _end_y;
	float _distance;
	float _cosx;
	float _tooth_thickness_rad;
	float _spacing_arc_length;

	float _max_width;
	float _min_width;

	int _radial_segments;

	void UpdateRefDiameter();

	void UpdateBaseDiameter();

	void UpdateBaseRadius();

	void UpdateTipDiameter();

	void UpdateTipRadius();

	void UpdateU();

	void UpdateTipPressureAngle();

	void UpdateInverseAngle();

	void UpdateInverseAngleA();

	void UpdateTopThickness();

	void UpdateEndX();

	void UpdateEndY();

	void UpdateDistance();

	void UpdateCosX();

	void UpdateToothThicknessRad();

	void UpdateSpacingArcLength();

	void UpdateMaxWidth();

	void UpdateMinWidth();

	void UpdateRadialSegments();
};