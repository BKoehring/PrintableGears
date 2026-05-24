#include <GearGenerator.h>
#include <Util.h>
#include <godot_cpp/core/class_db.hpp>

void GearGenerator::_bind_methods() {
}

GearGenerator::GearGenerator() {
}

GearGenerator::~GearGenerator() {
}

std::vector<openstl::Triangle> GearGenerator::generate(GearSpecs gearSpecs) {
    std::vector<openstl::Vec3>verts;
    std::vector<int>indices;
    
    auto base_radius = gearSpecs.GetBaseRadius();
    auto tip_diameter = gearSpecs.GetTipDiameter();
    auto tip_radius = gearSpecs.GetTipRadius();
    auto u = gearSpecs.GetU();
    auto tip_pressure_angle = gearSpecs.GetTipPressureAngle();
    auto inv_alpha = gearSpecs.GetInverseAlpha();
    auto inv_alpha_a = gearSpecs.GetInverseAlphaA();
    auto top_thickness = gearSpecs.GetTopThickness();
    auto end_x = gearSpecs.GetEndX();
    auto end_y = gearSpecs.GetEndY();
    auto distance = gearSpecs.GetDistance();
    auto cosx = gearSpecs.GetCosX();
    auto tooth_thickness_rad = gearSpecs.GetToothThicknessRed();
    auto spacing_arc_length = gearSpecs.GetSpacingArcLength();
    
    auto max_width = gearSpecs.GetMaxWidth();
    auto min_width = gearSpecs.GetMinWidth();
    
    int radial_segments = gearSpecs.GetRadialSegments();
    
    //Create Center of Gear
    //Add Top center vertice
    auto vert = openstl::Vec3(0, max_width, 0);
    verts.push_back(vert);
    
    //Add bottom center vertice
    vert = openstl::Vec3(0, min_width, 0);
    verts.push_back(vert);
    
    auto center_radial_segments = gearSpecs.GetNumberOfTeeth();
    for (unsigned int ring = 0; ring < gearSpecs.CENTER_RINGS; ring++) {
    	auto ring_radius = float(ring + 1.0) / float(gearSpecs.CENTER_RINGS + 1.0) * base_radius;
    	for (unsigned int segment = 0; segment < center_radial_segments; segment++) {
    		auto radian = 2.0 * std::numbers::pi * (segment / float(center_radial_segments));
    		auto x = ring_radius * cos(radian);
    		auto z = ring_radius * sin(radian);
    
    		//Top ring
    		vert = openstl::Vec3(x, max_width, z);
    		verts.push_back(vert);
    
    		//Bottom ring
    		vert = openstl::Vec3(x, min_width, z);
    		verts.push_back(vert);
    
    		if (segment > 0) {
    			auto current_point = segment * 2 + 2;
    			if (ring == 0) {
    				//Add Top triangle around center point
    				indices.push_back(0);
    				indices.push_back(current_point - 2);
    				indices.push_back(current_point);
    
    				//Add bottom trianlge around center point
    				indices.push_back(1);
    				indices.push_back(current_point + 1);
    				indices.push_back(current_point - 1);
    			}
    			else {
    				//Connect Top rings
    				indices.push_back(center_radial_segments * (ring - 1) * 2 + (current_point - 2));
    				indices.push_back(center_radial_segments * ring * 2 + (current_point - 2));
    				indices.push_back(center_radial_segments * (ring - 1) * 2 + current_point);                    
    
    				indices.push_back(center_radial_segments * (ring - 1) * 2 + current_point);
    				indices.push_back(center_radial_segments * ring * 2 + (current_point - 2));
    				indices.push_back(center_radial_segments * ring * 2 + current_point);               

    				//Connect Bottom rings
    				indices.push_back(center_radial_segments * (ring - 1) * 2 + (current_point - 1));
    				indices.push_back(center_radial_segments * (ring - 1) * 2 + (current_point + 1));
    				indices.push_back(center_radial_segments * ring * 2 + (current_point - 1));
    
    				indices.push_back(center_radial_segments * (ring - 1) * 2 + (current_point + 1));
    				indices.push_back(center_radial_segments * ring * 2 + (current_point + 1));
    				indices.push_back(center_radial_segments * ring * 2 + (current_point - 1));
    			}
    		}
    	}
    
    	//Complete the circle
    	auto last_vert = (ring + 1) * center_radial_segments * 2;
    	if (ring == 0) {
    		//Add top triangle around center point
    		indices.push_back(0);
    		indices.push_back(last_vert);
    		indices.push_back(2);
    
    		//Add bottom triangle around center point
    		indices.push_back(1);
    		indices.push_back(3);
    		indices.push_back(last_vert + 1);
    	}
    	else {
    		//Add top triangles connecting rings
    		indices.push_back(center_radial_segments * ring * 2);
    		indices.push_back(last_vert);
    		indices.push_back(center_radial_segments * (ring - 1) * 2 + 2);
    
    		indices.push_back(center_radial_segments * (ring - 1) * 2 + 2);
    		indices.push_back(last_vert);
    		indices.push_back(center_radial_segments * ring * 2 + 2);
    
    		//Add bottom triangles connecting rings
    		indices.push_back(center_radial_segments * ring * 2 + 1);
    		indices.push_back(center_radial_segments * (ring - 1) * 2 + 3);
    		indices.push_back(last_vert + 1);
    
    		indices.push_back(center_radial_segments * (ring - 1) * 2 + 3);
    		indices.push_back(center_radial_segments * ring * 2 + 3);
    		indices.push_back(last_vert + 1);
    	}
    }
    
    for (unsigned int current_tooth = 0; current_tooth < gearSpecs.GetNumberOfTeeth(); current_tooth++) {
    	for (unsigned int segment = 0; segment < (gearSpecs.GetInvoluteSteps() * gearSpecs.SECTIONS_PER_TOOTH); segment++) {
    		auto t = u * (1.0 + (0.5 / (gearSpecs.GetInvoluteSteps() - 0.5))) / std::numbers::pi * acos(cos((segment + 0.5) * std::numbers::pi / gearSpecs.GetInvoluteSteps()));
    		auto x = base_radius;
    		auto z = base_radius;
    		auto offset = Util::DegreesToRadians(current_tooth * (360.0 / gearSpecs.GetNumberOfTeeth()));
    
    		if (segment < gearSpecs.GetInvoluteSteps()) {
    			x *= (cos(t + offset) + t * sin(t + offset));
    			z *= (sin(t + offset) - t * cos(t + offset));
    		}
    		else if (segment < (gearSpecs.GetInvoluteSteps() * 2)) {
    			x *= (cos(-t + offset + tooth_thickness_rad) - t * sin(-t + offset + tooth_thickness_rad));
    			z *= (sin(-t + offset + tooth_thickness_rad) + t * cos(-t + offset + tooth_thickness_rad));
    		}
    		else {
    			auto spacing_arc_start_rad = tooth_thickness_rad + offset;
    			auto spacing_arc_start_coord = Util::Vec2(x * cos(spacing_arc_start_rad), z * sin(spacing_arc_start_rad));
    			auto spacing_arc_end_rad = spacing_arc_start_rad + spacing_arc_length;
    			auto arc_end_coord = Util::Vec2(x * cos(spacing_arc_end_rad), z * sin(spacing_arc_end_rad));
    			auto spacing_center_rad = spacing_arc_start_rad + spacing_arc_length / 2.0;
    			auto spacing_center_coord = Util::Vec2(x * cos(spacing_center_rad), z * sin(spacing_center_rad));
    			auto spacing_circle_start_angle = atan2(abs(spacing_arc_start_coord.Y - spacing_center_coord.Y), abs(spacing_arc_start_coord.X - spacing_center_coord.X));
    			auto spacing_circle_end_angle = atan2(abs(arc_end_coord.Y - spacing_center_coord.Y), abs(arc_end_coord.X - spacing_center_coord.X));
    
    			double spacing_circle_start = spacing_circle_start_angle;
    			if (spacing_arc_start_coord.X > spacing_center_coord.X) {
    				if (spacing_arc_start_coord.Y < spacing_center_coord.Y) {
    					spacing_circle_start = 2 * std::numbers::pi - spacing_circle_start;
    				}
    			}
    			else {
    				if (spacing_arc_start_coord.Y > spacing_center_coord.Y) {
    					spacing_circle_start = std::numbers::pi - spacing_circle_start;
    				}
    				else {
    					spacing_circle_start += std::numbers::pi;
    				}
    			}
    
    			double spacing_circle_end = spacing_circle_end_angle;
    			if (arc_end_coord.X > spacing_center_coord.X) {
    				if (arc_end_coord.Y < spacing_center_coord.Y) {
    					spacing_circle_end = 2 * std::numbers::pi - spacing_circle_end;
    				}
    			}
    			else {
    				if (arc_end_coord.Y > spacing_center_coord.Y) {
    					spacing_circle_end = std::numbers::pi - spacing_circle_end;
    				}
    				else {
    					spacing_circle_end += std::numbers::pi;
    				}
    			}
    
    			if (spacing_circle_start < spacing_circle_end) {
    				spacing_circle_start += 2 * std::numbers::pi;
    			}
    
    			auto spacing_circle_radius = sqrt(pow(spacing_center_coord.X - spacing_arc_start_coord.X, 2) + pow(spacing_center_coord.Y - spacing_arc_start_coord.Y, 2));
    			auto spacing_circle_step = (spacing_circle_start - spacing_circle_end) / (gearSpecs.GetInvoluteSteps() + 1.0);
    			auto spacing_circle_radial = spacing_circle_start - spacing_circle_step * (segment % gearSpecs.GetInvoluteSteps() + 1);
    			x = spacing_circle_radius * cos(spacing_circle_radial) + spacing_center_coord.X;
    			z = spacing_circle_radius * sin(spacing_circle_radial) + spacing_center_coord.Y;
    		}
    
    		if (segment == (gearSpecs.GetInvoluteSteps() - 1) || segment == gearSpecs.GetInvoluteSteps()) {
    			//Add top vert
    			vert = openstl::Vec3(x, max_width * .8, z);
    			verts.push_back(vert);
    
    			//Add bottom vert
    			vert = openstl::Vec3(x, min_width * .8, z);
    			verts.push_back(vert);
    		}
    		else {
    			//Add top vert
    			vert = openstl::Vec3(x, max_width, z);
    			verts.push_back(vert);
    
    			//Add bottom vert
    			vert = openstl::Vec3(x, min_width, z);
    			verts.push_back(vert);
    		}
    	}
    
    	if (current_tooth > 0) {
    		auto first_point = (gearSpecs.CENTER_RINGS - 1) * center_radial_segments * 2 + (2 * current_tooth);
    		auto tooth_starting_point = (current_tooth - 1) * gearSpecs.GetInvoluteSteps() * gearSpecs.SECTIONS_PER_TOOTH * 2 + (gearSpecs.CENTER_RINGS * center_radial_segments * 2) + 2;
    		auto tooth_end_point = tooth_starting_point + gearSpecs.GetInvoluteSteps() * 4 - 2;
    
    		//Top triangles
    		indices.push_back(first_point);
    		indices.push_back(tooth_starting_point);
    		indices.push_back(tooth_end_point);
    
    		//Bottom triangles
    		indices.push_back(first_point + 1);
    		indices.push_back(tooth_end_point + 1);
    		indices.push_back(tooth_starting_point + 1);
    
    		for (unsigned int involute_step = 0; involute_step < (gearSpecs.GetInvoluteSteps() - 1); involute_step++) {
    			auto increment = involute_step * 2;
    
    			//Top Tooth Triangles
    			indices.push_back(tooth_starting_point + increment);
    			indices.push_back(tooth_starting_point + 2 + increment);
    			indices.push_back(tooth_end_point - increment);
    
    			indices.push_back(tooth_end_point - increment);
    			indices.push_back(tooth_starting_point + 2 + increment);
    			indices.push_back(tooth_end_point - 2 - increment);
    
    			//Bottom Tooth Triangles
    			indices.push_back(tooth_starting_point + increment + 1);
    			indices.push_back(tooth_end_point - increment + 1);
    			indices.push_back(tooth_starting_point + 3 + increment);
    
    			indices.push_back(tooth_end_point - increment + 1);
    			indices.push_back(tooth_end_point - 1 - increment);
    			indices.push_back(tooth_starting_point + 3 + increment);
    
    			//Connect Top and Bottom Teeth
    			indices.push_back(tooth_starting_point + increment);
    			indices.push_back(tooth_starting_point + increment + 1);
    			indices.push_back(tooth_starting_point + increment + 2);
    
    			indices.push_back(tooth_starting_point + increment + 2);
    			indices.push_back(tooth_starting_point + increment + 1);
    			indices.push_back(tooth_starting_point + increment + 3);
    
    			indices.push_back(tooth_end_point - increment);
    			indices.push_back(tooth_end_point - increment - 2);
    			indices.push_back(tooth_end_point - increment + 1);
    
    			indices.push_back(tooth_end_point - increment + 1);
    			indices.push_back(tooth_end_point - increment - 2);
    			indices.push_back(tooth_end_point - increment - 1);
    
    			//Spacing Triangles
    			if (involute_step < (gearSpecs.GetInvoluteSteps() / 2.0 - 0.5)) {
    				//Top
    				indices.push_back(first_point);
    				indices.push_back(tooth_end_point + (involute_step * 2));
    				indices.push_back(tooth_end_point + (involute_step * 2) + 2);
    
    				//Bottom
    				indices.push_back(first_point + 1);
    				indices.push_back(tooth_end_point + (involute_step * 2) + 3);
    				indices.push_back(tooth_end_point + (involute_step * 2) + 1);
    			}
    			else if (involute_step <= (gearSpecs.GetInvoluteSteps() / 2.0)) {
    				//Top
    				indices.push_back(first_point);
    				indices.push_back(tooth_end_point + (involute_step * 2));
    				indices.push_back(first_point + 2);
    
    				indices.push_back(first_point + 2);
    				indices.push_back(tooth_end_point + (involute_step * 2));
    				indices.push_back(tooth_end_point + (involute_step * 2) + 2);
    
    				//Bottom
    				indices.push_back(first_point + 1);
    				indices.push_back(first_point + 3);
    				indices.push_back(tooth_end_point + (involute_step * 2) + 1);
    
    				indices.push_back(first_point + 3);
    				indices.push_back(tooth_end_point + (involute_step * 2) + 3);
    				indices.push_back(tooth_end_point + (involute_step * 2) + 1);
    			}
    			else {
    				//Top
    				indices.push_back(first_point + 2);
    				indices.push_back(tooth_end_point + (involute_step * 2));
    				indices.push_back(tooth_end_point + (involute_step * 2) + 2);
    
    				//Bottom
    				indices.push_back(first_point + 3);
    				indices.push_back(tooth_end_point + (involute_step * 2) + 3);
    				indices.push_back(tooth_end_point + (involute_step * 2) + 1);
    			}
    
    
    			//Connect top and bottom Spacing
    			indices.push_back(tooth_end_point + (involute_step * 2));
    			indices.push_back(tooth_end_point + (involute_step * 2) + 1);
    			indices.push_back(tooth_end_point + (involute_step * 2) + 2);
    
    			indices.push_back(tooth_end_point + (involute_step * 2) + 2);
    			indices.push_back(tooth_end_point + (involute_step * 2) + 1);
    			indices.push_back(tooth_end_point + (involute_step * 2) + 3);
    
    			////Connect gaps
    			if (involute_step == gearSpecs.GetInvoluteSteps() - 2) {
    				//Connect tooth ends
    				indices.push_back(tooth_starting_point + (involute_step * 2) + 2);
    				indices.push_back(tooth_starting_point + (involute_step * 2) + 3);
    				indices.push_back(tooth_starting_point + (involute_step * 2) + 4);
    
    				indices.push_back(tooth_starting_point + (involute_step * 2) + 4);
    				indices.push_back(tooth_starting_point + (involute_step * 2) + 3);
    				indices.push_back(tooth_starting_point + (involute_step * 2) + 5);
    
    				//Connect top spacing to next tooth
    				indices.push_back(first_point + 2);
    				indices.push_back(tooth_end_point + (involute_step * 2) + 2);
    				indices.push_back(tooth_end_point + (involute_step * 2) + 4);
    
    				indices.push_back(first_point + 2);
    				indices.push_back(tooth_end_point + (involute_step * 2) + 4);
    				indices.push_back(tooth_end_point + (involute_step * 2) + 6);
    
    				//Connect bottom spacing to next tooth
    				indices.push_back(first_point + 3);
    				indices.push_back(tooth_end_point + (involute_step * 2) + 5);
    				indices.push_back(tooth_end_point + (involute_step * 2) + 3);
    
    				indices.push_back(first_point + 3);
    				indices.push_back(tooth_end_point + (involute_step * 2) + 7);
    				indices.push_back(tooth_end_point + (involute_step * 2) + 5);
    
    				//Connect top and bottom Spacing
    				indices.push_back(tooth_end_point + (involute_step * 2) + 2);
    				indices.push_back(tooth_end_point + (involute_step * 2) + 3);
    				indices.push_back(tooth_end_point + (involute_step * 2) + 4);
    
    				indices.push_back(tooth_end_point + (involute_step * 2) + 4);
    				indices.push_back(tooth_end_point + (involute_step * 2) + 3);
    				indices.push_back(tooth_end_point + (involute_step * 2) + 5);
    
    				indices.push_back(tooth_end_point + (involute_step * 2) + 4);
    				indices.push_back(tooth_end_point + (involute_step * 2) + 5);
    				indices.push_back(tooth_end_point + (involute_step * 2) + 6);
    
    				indices.push_back(tooth_end_point + (involute_step * 2) + 6);
    				indices.push_back(tooth_end_point + (involute_step * 2) + 5);
    				indices.push_back(tooth_end_point + (involute_step * 2) + 7);
    			}
    		}
    	}
    
    	//Add Last Tooth
    	if (current_tooth == gearSpecs.GetNumberOfTeeth() - 1) {
    		auto first_point = (gearSpecs.CENTER_RINGS - 1) * center_radial_segments * 2 + (2 * (current_tooth + 1));
    		auto next_point = (gearSpecs.CENTER_RINGS - 1) * center_radial_segments * 2 + 2;
    		auto tooth_starting_point = current_tooth * gearSpecs.GetInvoluteSteps() * gearSpecs.SECTIONS_PER_TOOTH * 2 + (gearSpecs.CENTER_RINGS * center_radial_segments * 2) + 2;
    		auto tooth_end_point = tooth_starting_point + gearSpecs.GetInvoluteSteps() * 4 - 2;
    		auto next_tooth = gearSpecs.CENTER_RINGS * center_radial_segments * 2 + 2;
    
    		//Top triangles
    		indices.push_back(first_point);
    		indices.push_back(tooth_starting_point);
    		indices.push_back(tooth_end_point);
    
    		//Bottom triangles
    		indices.push_back(first_point + 1);
    		indices.push_back(tooth_end_point + 1);
    		indices.push_back(tooth_starting_point + 1);
    
    		for (unsigned int involute_step = 0; involute_step < (gearSpecs.GetInvoluteSteps() - 1); involute_step++) {
    			auto increment = involute_step * 2;
    
    			//Top Tooth Triangles
    			indices.push_back(tooth_starting_point + increment);
    			indices.push_back(tooth_starting_point + 2 + increment);
    			indices.push_back(tooth_end_point - increment);
    
    			indices.push_back(tooth_end_point - increment);
    			indices.push_back(tooth_starting_point + 2 + increment);
    			indices.push_back(tooth_end_point - 2 - increment);
    
    			//Bottom Tooth Triangles
    			indices.push_back(tooth_starting_point + increment + 1);
    			indices.push_back(tooth_end_point - increment + 1);
    			indices.push_back(tooth_starting_point + 3 + increment);
    
    			indices.push_back(tooth_end_point - increment + 1);
    			indices.push_back(tooth_end_point - 1 - increment);
    			indices.push_back(tooth_starting_point + 3 + increment);
    
    			//Connect Top and Bottom Teeth
    			indices.push_back(tooth_starting_point + increment);
    			indices.push_back(tooth_starting_point + increment + 1);
    			indices.push_back(tooth_starting_point + increment + 2);
    
    			indices.push_back(tooth_starting_point + increment + 2);
    			indices.push_back(tooth_starting_point + increment + 1);
    			indices.push_back(tooth_starting_point + increment + 3);
    
    			indices.push_back(tooth_end_point - increment);
    			indices.push_back(tooth_end_point - increment - 2);
    			indices.push_back(tooth_end_point - increment + 1);
    
    			indices.push_back(tooth_end_point - increment + 1);
    			indices.push_back(tooth_end_point - increment - 2);
    			indices.push_back(tooth_end_point - increment - 1);
    
    			//Spacing Triangles
    			if (involute_step < (gearSpecs.GetInvoluteSteps() / 2.0 - 0.5)) {
    				//Top
    				indices.push_back(first_point);
    				indices.push_back(tooth_end_point + (involute_step * 2));
    				indices.push_back(tooth_end_point + (involute_step * 2) + 2);
    
    				//Bottom
    				indices.push_back(first_point + 1);
    				indices.push_back(tooth_end_point + (involute_step * 2) + 3);
    				indices.push_back(tooth_end_point + (involute_step * 2) + 1);
    			}
    			else if (involute_step <= (gearSpecs.GetInvoluteSteps() / 2.0)) {
    				//Top
    				indices.push_back(first_point);
    				indices.push_back(tooth_end_point + (involute_step * 2));
    				indices.push_back(next_point);
    
    				indices.push_back(next_point);
    				indices.push_back(tooth_end_point + (involute_step * 2));
    				indices.push_back(tooth_end_point + (involute_step * 2) + 2);
    
    				//Bottom
    				indices.push_back(first_point + 1);
    				indices.push_back(next_point + 1);
    				indices.push_back(tooth_end_point + (involute_step * 2) + 1);
    
    				indices.push_back(next_point + 1);
    				indices.push_back(tooth_end_point + (involute_step * 2) + 3);
    				indices.push_back(tooth_end_point + (involute_step * 2) + 1);
    			}
    			else {
    				//Top
    				indices.push_back(next_point);
    				indices.push_back(tooth_end_point + (involute_step * 2));
    				indices.push_back(tooth_end_point + (involute_step * 2) + 2);
    
    				//Bottom
    				indices.push_back(next_point + 1);
    				indices.push_back(tooth_end_point + (involute_step * 2) + 3);
    				indices.push_back(tooth_end_point + (involute_step * 2) + 1);
    			}
    
    
    			//Connect top and bottom Spacing
    			indices.push_back(tooth_end_point + (involute_step * 2));
    			indices.push_back(tooth_end_point + (involute_step * 2) + 1);
    			indices.push_back(tooth_end_point + (involute_step * 2) + 2);
    
    			indices.push_back(tooth_end_point + (involute_step * 2) + 2);
    			indices.push_back(tooth_end_point + (involute_step * 2) + 1);
    			indices.push_back(tooth_end_point + (involute_step * 2) + 3);
    
    			////Connect gaps
    			if (involute_step == gearSpecs.GetInvoluteSteps() - 2) {
    				//Connect tooth ends
    				indices.push_back(tooth_starting_point + (involute_step * 2) + 2);
    				indices.push_back(tooth_starting_point + (involute_step * 2) + 3);
    				indices.push_back(tooth_starting_point + (involute_step * 2) + 4);
    
    				indices.push_back(tooth_starting_point + (involute_step * 2) + 4);
    				indices.push_back(tooth_starting_point + (involute_step * 2) + 3);
    				indices.push_back(tooth_starting_point + (involute_step * 2) + 5);
    
    				//Connect top spacing to next tooth
    				indices.push_back(next_point);
    				indices.push_back(tooth_end_point + (involute_step * 2) + 2);
    				indices.push_back(tooth_end_point + (involute_step * 2) + 4);
    
    				indices.push_back(next_point);
    				indices.push_back(tooth_end_point + (involute_step * 2) + 4);
    				indices.push_back(next_tooth);
    
    				//Connect bottom spacing to next tooth
    				indices.push_back(next_point + 1);
    				indices.push_back(tooth_end_point + (involute_step * 2) + 5);
    				indices.push_back(tooth_end_point + (involute_step * 2) + 3);
    
    				indices.push_back(next_point + 1);
    				indices.push_back(next_tooth + 1);
    				indices.push_back(tooth_end_point + (involute_step * 2) + 5);
    
    				//Connect top and bottom Spacing
    				indices.push_back(tooth_end_point + (involute_step * 2) + 2);
    				indices.push_back(tooth_end_point + (involute_step * 2) + 3);
    				indices.push_back(tooth_end_point + (involute_step * 2) + 4);
    
    				indices.push_back(tooth_end_point + (involute_step * 2) + 4);
    				indices.push_back(tooth_end_point + (involute_step * 2) + 3);
    				indices.push_back(tooth_end_point + (involute_step * 2) + 5);
    
    				indices.push_back(tooth_end_point + (involute_step * 2) + 4);
    				indices.push_back(tooth_end_point + (involute_step * 2) + 5);
    				indices.push_back(next_tooth);
    
    				indices.push_back(next_tooth);
    				indices.push_back(tooth_end_point + (involute_step * 2) + 5);
    				indices.push_back(next_tooth + 1);
    			}
    		}
    	}
    }

    // Stitch verts into triangles
    std::vector<openstl::Triangle> triangles;
    openstl::Triangle triangle;
    for (unsigned int index = 0; index < indices.size(); index++) {
        unsigned int t_point = index % 3;
        switch(t_point) {
        case 0:
            triangle.v0 = verts[indices[index]];
            break;
        case 1:
            triangle.v1 = verts[indices[index]];
            break;
        case 2:
            triangle.v2 = verts[indices[index]];
            triangles.push_back(triangle);
            break;
        }
    }

    return triangles;
}