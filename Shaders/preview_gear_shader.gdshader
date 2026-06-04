shader_type spatial;
render_mode unshaded;

uniform vec4 color : source_color = vec4(.8, .8, .8, 1.);

void vertex() {
	COLOR = color;
}

void fragment() {
	ALBEDO = COLOR.rgb * vec3(NORMAL.b);
}

//void light() {
//	// Called for every pixel for every light affecting the material.
//	// Uncomment to replace the default light processing function with this one.
//}
