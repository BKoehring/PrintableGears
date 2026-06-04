#include <GearGenerator.h>
#include <Util.h>
#include "GearGenerator.h"
#include "GearGenerator.h"
#include "GearGenerator.h"
#include "GearGenerator.h"
#include "GearGenerator.h"
#include "GearGenerator.h"
#include "GearGenerator.h"

GearGenerator::GearGenerator() {
}

GearGenerator::~GearGenerator() {
}

void GearGenerator::generate(GearSpecs gearSpecs) {    
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
    auto center_radial_segments = gearSpecs.GetNumberOfTeeth();

    std::size_t total_verts = 2 * (1 + gearSpecs.CENTER_RINGS * center_radial_segments + center_radial_segments * gearSpecs.GetInvoluteSteps() * gearSpecs.SECTIONS_PER_TOOTH);
    verts.resize(total_verts);
    normals.resize(total_verts);

    indices.clear();

    //Create Center of Gear
    //Add Top center vertice
    auto vert = godot::Vector3(0, max_width, 0);
    unsigned int vert_index = 0;
    vert_index = SetVert(vert_index, vert, godot::Vector3(0, 1, 0));
    
    //Add bottom center vertice
    vert.y = min_width;
    vert_index = SetVert(vert_index, vert, godot::Vector3(0, -1, 0));
    
    for (unsigned int ring = 0; ring < gearSpecs.CENTER_RINGS; ring++) {
    	auto ring_radius = float(ring + 1.0) / float(gearSpecs.CENTER_RINGS + 1.0) * base_radius;
    	for (unsigned int segment = 0; segment < center_radial_segments; segment++) {
    		auto radian = 2.0 * std::numbers::pi * (segment / float(center_radial_segments));
    		auto x = ring_radius * cos(radian);
    		auto z = ring_radius * sin(radian);
    
    		//Top ring
    		vert = godot::Vector3(x, max_width, z);
            vert_index = SetVert(vert_index, vert, godot::Vector3(0, 1, 0));
    
    		//Bottom ring
    		vert = godot::Vector3(x, min_width, z);
            vert_index = SetVert(vert_index, vert, godot::Vector3(0, -1, 0));
    
    		if (segment > 0) {
    			auto current_point = segment * 2 + 2;
    			if (ring == 0) {
    				//Add Top triangle around center point
    				indices.append(0);
    				indices.append(current_point - 2);
    				indices.append(current_point);
    
    				//Add bottom trianlge around center point
    				indices.append(1);
    				indices.append(current_point + 1);
    				indices.append(current_point - 1);
    			}
    			else {
    				//Connect Top rings
    				indices.append(center_radial_segments * (ring - 1) * 2 + (current_point - 2));
    				indices.append(center_radial_segments * ring * 2 + (current_point - 2));
    				indices.append(center_radial_segments * (ring - 1) * 2 + current_point);                    
    
    				indices.append(center_radial_segments * (ring - 1) * 2 + current_point);
    				indices.append(center_radial_segments * ring * 2 + (current_point - 2));
    				indices.append(center_radial_segments * ring * 2 + current_point);               

    				//Connect Bottom rings
    				indices.append(center_radial_segments * (ring - 1) * 2 + (current_point - 1));
    				indices.append(center_radial_segments * (ring - 1) * 2 + (current_point + 1));
    				indices.append(center_radial_segments * ring * 2 + (current_point - 1));
    
    				indices.append(center_radial_segments * (ring - 1) * 2 + (current_point + 1));
    				indices.append(center_radial_segments * ring * 2 + (current_point + 1));
    				indices.append(center_radial_segments * ring * 2 + (current_point - 1));
    			}
    		}
    	}
    
    	//Complete the circle
    	auto last_vert = (ring + 1) * center_radial_segments * 2;
    	if (ring == 0) {
    		//Add top triangle around center point
    		indices.append(0);
    		indices.append(last_vert);
    		indices.append(2);
    
    		//Add bottom triangle around center point
    		indices.append(1);
    		indices.append(3);
    		indices.append(last_vert + 1);
    	}
    	else {
    		//Add top triangles connecting rings
    		indices.append(center_radial_segments * ring * 2);
    		indices.append(last_vert);
    		indices.append(center_radial_segments * (ring - 1) * 2 + 2);
    
    		indices.append(center_radial_segments * (ring - 1) * 2 + 2);
    		indices.append(last_vert);
    		indices.append(center_radial_segments * ring * 2 + 2);
    
    		//Add bottom triangles connecting rings
    		indices.append(center_radial_segments * ring * 2 + 1);
    		indices.append(center_radial_segments * (ring - 1) * 2 + 3);
    		indices.append(last_vert + 1);
    
    		indices.append(center_radial_segments * (ring - 1) * 2 + 3);
    		indices.append(center_radial_segments * ring * 2 + 3);
    		indices.append(last_vert + 1);
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
    			vert = godot::Vector3(x, max_width * .8, z);
                vert_index = SetVert(vert_index, vert, godot::Vector3(0,1,0));
    
    			//Add bottom vert
    			vert = godot::Vector3(x, min_width * .8, z);
                vert_index = SetVert(vert_index, vert, godot::Vector3(0, -1, 0));
    		}
    		else {
    			//Add top vert
    			vert = godot::Vector3(x, max_width, z);
                vert_index = SetVert(vert_index, vert, godot::Vector3(0, 1, 0));
    
    			//Add bottom vert
    			vert = godot::Vector3(x, min_width, z);
                vert_index = SetVert(vert_index, vert, godot::Vector3(0, -1, 0));
    		}
    	}
    
    	if (current_tooth > 0) {
    		auto first_point = (gearSpecs.CENTER_RINGS - 1) * center_radial_segments * 2 + (2 * current_tooth);
    		auto tooth_starting_point = (current_tooth - 1) * gearSpecs.GetInvoluteSteps() * gearSpecs.SECTIONS_PER_TOOTH * 2 + (gearSpecs.CENTER_RINGS * center_radial_segments * 2) + 2;
    		auto tooth_end_point = tooth_starting_point + gearSpecs.GetInvoluteSteps() * 4 - 2;
    
    		//Top triangles
    		indices.append(first_point);
    		indices.append(tooth_starting_point);
    		indices.append(tooth_end_point);
    
    		//Bottom triangles
    		indices.append(first_point + 1);
    		indices.append(tooth_end_point + 1);
    		indices.append(tooth_starting_point + 1);
    
    		for (unsigned int involute_step = 0; involute_step < (gearSpecs.GetInvoluteSteps() - 1); involute_step++) {
    			auto increment = involute_step * 2;
    
    			//Top Tooth Triangles
    			indices.append(tooth_starting_point + increment);
    			indices.append(tooth_starting_point + 2 + increment);
    			indices.append(tooth_end_point - increment);
    
    			indices.append(tooth_end_point - increment);
    			indices.append(tooth_starting_point + 2 + increment);
    			indices.append(tooth_end_point - 2 - increment);
    
    			//Bottom Tooth Triangles
    			indices.append(tooth_starting_point + increment + 1);
    			indices.append(tooth_end_point - increment + 1);
    			indices.append(tooth_starting_point + 3 + increment);
    
    			indices.append(tooth_end_point - increment + 1);
    			indices.append(tooth_end_point - 1 - increment);
    			indices.append(tooth_starting_point + 3 + increment);
    
    			//Connect Top and Bottom Teeth
    			indices.append(tooth_starting_point + increment);
    			indices.append(tooth_starting_point + increment + 1);
    			indices.append(tooth_starting_point + increment + 2);
    
    			indices.append(tooth_starting_point + increment + 2);
    			indices.append(tooth_starting_point + increment + 1);
    			indices.append(tooth_starting_point + increment + 3);
    
    			indices.append(tooth_end_point - increment);
    			indices.append(tooth_end_point - increment - 2);
    			indices.append(tooth_end_point - increment + 1);
    
    			indices.append(tooth_end_point - increment + 1);
    			indices.append(tooth_end_point - increment - 2);
    			indices.append(tooth_end_point - increment - 1);
    
    			//Spacing Triangles
    			if (involute_step < (gearSpecs.GetInvoluteSteps() / 2.0 - 0.5)) {
    				//Top
    				indices.append(first_point);
    				indices.append(tooth_end_point + (involute_step * 2));
    				indices.append(tooth_end_point + (involute_step * 2) + 2);
    
    				//Bottom
    				indices.append(first_point + 1);
    				indices.append(tooth_end_point + (involute_step * 2) + 3);
    				indices.append(tooth_end_point + (involute_step * 2) + 1);
    			}
    			else if (involute_step <= (gearSpecs.GetInvoluteSteps() / 2.0)) {
    				//Top
    				indices.append(first_point);
    				indices.append(tooth_end_point + (involute_step * 2));
    				indices.append(first_point + 2);
    
    				indices.append(first_point + 2);
    				indices.append(tooth_end_point + (involute_step * 2));
    				indices.append(tooth_end_point + (involute_step * 2) + 2);
    
    				//Bottom
    				indices.append(first_point + 1);
    				indices.append(first_point + 3);
    				indices.append(tooth_end_point + (involute_step * 2) + 1);
    
    				indices.append(first_point + 3);
    				indices.append(tooth_end_point + (involute_step * 2) + 3);
    				indices.append(tooth_end_point + (involute_step * 2) + 1);
    			}
    			else {
    				//Top
    				indices.append(first_point + 2);
    				indices.append(tooth_end_point + (involute_step * 2));
    				indices.append(tooth_end_point + (involute_step * 2) + 2);
    
    				//Bottom
    				indices.append(first_point + 3);
    				indices.append(tooth_end_point + (involute_step * 2) + 3);
    				indices.append(tooth_end_point + (involute_step * 2) + 1);
    			}
    
    
    			//Connect top and bottom Spacing
    			indices.append(tooth_end_point + (involute_step * 2));
    			indices.append(tooth_end_point + (involute_step * 2) + 1);
    			indices.append(tooth_end_point + (involute_step * 2) + 2);
    
    			indices.append(tooth_end_point + (involute_step * 2) + 2);
    			indices.append(tooth_end_point + (involute_step * 2) + 1);
    			indices.append(tooth_end_point + (involute_step * 2) + 3);
    
    			////Connect gaps
    			if (involute_step == gearSpecs.GetInvoluteSteps() - 2) {
    				//Connect tooth ends
    				indices.append(tooth_starting_point + (involute_step * 2) + 2);
    				indices.append(tooth_starting_point + (involute_step * 2) + 3);
    				indices.append(tooth_starting_point + (involute_step * 2) + 4);
    
    				indices.append(tooth_starting_point + (involute_step * 2) + 4);
    				indices.append(tooth_starting_point + (involute_step * 2) + 3);
    				indices.append(tooth_starting_point + (involute_step * 2) + 5);
    
    				//Connect top spacing to next tooth
    				indices.append(first_point + 2);
    				indices.append(tooth_end_point + (involute_step * 2) + 2);
    				indices.append(tooth_end_point + (involute_step * 2) + 4);
    
    				indices.append(first_point + 2);
    				indices.append(tooth_end_point + (involute_step * 2) + 4);
    				indices.append(tooth_end_point + (involute_step * 2) + 6);
    
    				//Connect bottom spacing to next tooth
    				indices.append(first_point + 3);
    				indices.append(tooth_end_point + (involute_step * 2) + 5);
    				indices.append(tooth_end_point + (involute_step * 2) + 3);
    
    				indices.append(first_point + 3);
    				indices.append(tooth_end_point + (involute_step * 2) + 7);
    				indices.append(tooth_end_point + (involute_step * 2) + 5);
    
    				//Connect top and bottom Spacing
    				indices.append(tooth_end_point + (involute_step * 2) + 2);
    				indices.append(tooth_end_point + (involute_step * 2) + 3);
    				indices.append(tooth_end_point + (involute_step * 2) + 4);
    
    				indices.append(tooth_end_point + (involute_step * 2) + 4);
    				indices.append(tooth_end_point + (involute_step * 2) + 3);
    				indices.append(tooth_end_point + (involute_step * 2) + 5);
    
    				indices.append(tooth_end_point + (involute_step * 2) + 4);
    				indices.append(tooth_end_point + (involute_step * 2) + 5);
    				indices.append(tooth_end_point + (involute_step * 2) + 6);
    
    				indices.append(tooth_end_point + (involute_step * 2) + 6);
    				indices.append(tooth_end_point + (involute_step * 2) + 5);
    				indices.append(tooth_end_point + (involute_step * 2) + 7);
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
    		indices.append(first_point);
    		indices.append(tooth_starting_point);
    		indices.append(tooth_end_point);
    
    		//Bottom triangles
    		indices.append(first_point + 1);
    		indices.append(tooth_end_point + 1);
    		indices.append(tooth_starting_point + 1);
    
    		for (unsigned int involute_step = 0; involute_step < (gearSpecs.GetInvoluteSteps() - 1); involute_step++) {
    			auto increment = involute_step * 2;
    
    			//Top Tooth Triangles
    			indices.append(tooth_starting_point + increment);
    			indices.append(tooth_starting_point + 2 + increment);
    			indices.append(tooth_end_point - increment);
    
    			indices.append(tooth_end_point - increment);
    			indices.append(tooth_starting_point + 2 + increment);
    			indices.append(tooth_end_point - 2 - increment);
    
    			//Bottom Tooth Triangles
    			indices.append(tooth_starting_point + increment + 1);
    			indices.append(tooth_end_point - increment + 1);
    			indices.append(tooth_starting_point + 3 + increment);
    
    			indices.append(tooth_end_point - increment + 1);
    			indices.append(tooth_end_point - 1 - increment);
    			indices.append(tooth_starting_point + 3 + increment);
    
    			//Connect Top and Bottom Teeth
    			indices.append(tooth_starting_point + increment);
    			indices.append(tooth_starting_point + increment + 1);
    			indices.append(tooth_starting_point + increment + 2);
    
    			indices.append(tooth_starting_point + increment + 2);
    			indices.append(tooth_starting_point + increment + 1);
    			indices.append(tooth_starting_point + increment + 3);
    
    			indices.append(tooth_end_point - increment);
    			indices.append(tooth_end_point - increment - 2);
    			indices.append(tooth_end_point - increment + 1);
    
    			indices.append(tooth_end_point - increment + 1);
    			indices.append(tooth_end_point - increment - 2);
    			indices.append(tooth_end_point - increment - 1);
    
    			//Spacing Triangles
    			if (involute_step < (gearSpecs.GetInvoluteSteps() / 2.0 - 0.5)) {
    				//Top
    				indices.append(first_point);
    				indices.append(tooth_end_point + (involute_step * 2));
    				indices.append(tooth_end_point + (involute_step * 2) + 2);
    
    				//Bottom
    				indices.append(first_point + 1);
    				indices.append(tooth_end_point + (involute_step * 2) + 3);
    				indices.append(tooth_end_point + (involute_step * 2) + 1);
    			}
    			else if (involute_step <= (gearSpecs.GetInvoluteSteps() / 2.0)) {
    				//Top
    				indices.append(first_point);
    				indices.append(tooth_end_point + (involute_step * 2));
    				indices.append(next_point);
    
    				indices.append(next_point);
    				indices.append(tooth_end_point + (involute_step * 2));
    				indices.append(tooth_end_point + (involute_step * 2) + 2);
    
    				//Bottom
    				indices.append(first_point + 1);
    				indices.append(next_point + 1);
    				indices.append(tooth_end_point + (involute_step * 2) + 1);
    
    				indices.append(next_point + 1);
    				indices.append(tooth_end_point + (involute_step * 2) + 3);
    				indices.append(tooth_end_point + (involute_step * 2) + 1);
    			}
    			else {
    				//Top
    				indices.append(next_point);
    				indices.append(tooth_end_point + (involute_step * 2));
    				indices.append(tooth_end_point + (involute_step * 2) + 2);
    
    				//Bottom
    				indices.append(next_point + 1);
    				indices.append(tooth_end_point + (involute_step * 2) + 3);
    				indices.append(tooth_end_point + (involute_step * 2) + 1);
    			}
    
    
    			//Connect top and bottom Spacing
    			indices.append(tooth_end_point + (involute_step * 2));
    			indices.append(tooth_end_point + (involute_step * 2) + 1);
    			indices.append(tooth_end_point + (involute_step * 2) + 2);
    
    			indices.append(tooth_end_point + (involute_step * 2) + 2);
    			indices.append(tooth_end_point + (involute_step * 2) + 1);
    			indices.append(tooth_end_point + (involute_step * 2) + 3);
    
    			////Connect gaps
    			if (involute_step == gearSpecs.GetInvoluteSteps() - 2) {
    				//Connect tooth ends
    				indices.append(tooth_starting_point + (involute_step * 2) + 2);
    				indices.append(tooth_starting_point + (involute_step * 2) + 3);
    				indices.append(tooth_starting_point + (involute_step * 2) + 4);
    
    				indices.append(tooth_starting_point + (involute_step * 2) + 4);
    				indices.append(tooth_starting_point + (involute_step * 2) + 3);
    				indices.append(tooth_starting_point + (involute_step * 2) + 5);
    
    				//Connect top spacing to next tooth
    				indices.append(next_point);
    				indices.append(tooth_end_point + (involute_step * 2) + 2);
    				indices.append(tooth_end_point + (involute_step * 2) + 4);
    
    				indices.append(next_point);
    				indices.append(tooth_end_point + (involute_step * 2) + 4);
    				indices.append(next_tooth);
    
    				//Connect bottom spacing to next tooth
    				indices.append(next_point + 1);
    				indices.append(tooth_end_point + (involute_step * 2) + 5);
    				indices.append(tooth_end_point + (involute_step * 2) + 3);
    
    				indices.append(next_point + 1);
    				indices.append(next_tooth + 1);
    				indices.append(tooth_end_point + (involute_step * 2) + 5);
    
    				//Connect top and bottom Spacing
    				indices.append(tooth_end_point + (involute_step * 2) + 2);
    				indices.append(tooth_end_point + (involute_step * 2) + 3);
    				indices.append(tooth_end_point + (involute_step * 2) + 4);
    
    				indices.append(tooth_end_point + (involute_step * 2) + 4);
    				indices.append(tooth_end_point + (involute_step * 2) + 3);
    				indices.append(tooth_end_point + (involute_step * 2) + 5);
    
    				indices.append(tooth_end_point + (involute_step * 2) + 4);
    				indices.append(tooth_end_point + (involute_step * 2) + 5);
    				indices.append(next_tooth);
    
    				indices.append(next_tooth);
    				indices.append(tooth_end_point + (involute_step * 2) + 5);
    				indices.append(next_tooth + 1);
    			}
    		}
    	}
    }
}

const std::vector<openstl::Triangle>& GearGenerator::GetTriangles()
{
    // Stitch verts into triangles
    std::size_t triangle_count = indices.size() / 3;
    triangles.resize(triangle_count);
    openstl::Triangle triangle;
    for (unsigned int index = 0; index < indices.size(); index++) {
        unsigned int t_point = index % 3;
        auto vert = ConvertVec3(verts[indices[index]]);

        switch (t_point) {
        case 0:
            triangle.v0 = vert;
            break;
        case 1:
            triangle.v1 = vert;
            break;
        case 2:
            triangle.v2 = vert;
            triangles[index / 3] = triangle;
            break;
        }
    }

    return triangles;
}

const godot::PackedVector3Array& GearGenerator::GetVerts()
{
    return verts;
}

const godot::PackedVector3Array& GearGenerator::GetNormals()
{
    return normals;
}

const godot::PackedInt32Array& GearGenerator::GetIndices()
{
    return indices;
}

unsigned int GearGenerator::SetVert(unsigned int index, godot::Vector3 vert, godot::Vector3 normal)
{
    verts[index] = vert;
    normals[index] = normal;
    
    return ++index;
}

openstl::Vec3 GearGenerator::ConvertVec3(godot::Vector3 vec)
{
    return openstl::Vec3(vec.x, vec.y, vec.z);
}
