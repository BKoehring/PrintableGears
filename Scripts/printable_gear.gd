extends PrintableGear

@onready var previewMesh = $PreviewGearMesh

func _ready():
	RefreshPreview()

func RefreshPreview():
	#var arrays = []
	#arrays.resize(Mesh.ARRAY_MAX)
	#arrays[Mesh.ARRAY_VERTEX] = GetVerts()
	#arrays[Mesh.ARRAY_NORMAL] = GetNormals()
	#arrays[Mesh.ARRAY_INDEX] = GetIndices()
	
	var st = SurfaceTool.new()
	st.begin(Mesh.PRIMITIVE_TRIANGLES)
	st.set_smooth_group(-1)
	for vert in GetVerts():
		st.add_vertex(vert)
	for index in GetIndices():
		st.add_index(index)
	st.generate_normals()
	
	previewMesh.mesh.clear_surfaces()
	previewMesh.mesh.add_surface_from_arrays(Mesh.PRIMITIVE_TRIANGLES, st.commit_to_arrays())


func _on_gear_property_changed(_node):
	RefreshPreview()
