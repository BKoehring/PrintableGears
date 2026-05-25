extends PrintableGear

@onready var previewMesh = $PreviewGearMesh

func _ready():
	RefreshPreview()

func RefreshPreview():
	var arrays = []
	arrays.resize(Mesh.ARRAY_MAX)
	arrays[Mesh.ARRAY_VERTEX] = GetVectors()
	
	previewMesh.mesh.clear_surfaces()
	previewMesh.mesh.add_surface_from_arrays(Mesh.PRIMITIVE_TRIANGLES, arrays)


func _on_gear_property_changed(_node):
	RefreshPreview()
