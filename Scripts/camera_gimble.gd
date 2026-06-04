extends Node3D

var panning := false
var rotating := false
var rotation_speed : float = .01
var rotate_velocity := Vector2.ZERO
var pan_velocity := Vector3.ZERO
var zoom_speed : float = .1
var movement_speed : float = .1

@onready var vertical_pivot := $VerticalPivot
@onready var camera := $VerticalPivot/Camera3D

func _ready():
	camera.look_at(position)

func _input(event):
	if event.is_action_pressed("rotate") && !panning:
		rotating = true
		Input.mouse_mode = Input.MOUSE_MODE_CAPTURED
	
	if event.is_action_released("rotate") && rotating:
		rotating = false
		Input.mouse_mode = Input.MOUSE_MODE_VISIBLE
	
	if event.is_action_pressed("zoom_in"):
		if zoom_speed < camera.position.z:
			camera.position.z -= zoom_speed
	
	if event.is_action_pressed("zoom_out"):
		camera.position.z += zoom_speed
	
	if event.is_action_pressed("pan") && !rotating:
		panning = true
		Input.mouse_mode = Input.MOUSE_MODE_CAPTURED
		
	if event.is_action_released("pan") && panning:
		panning = false
		Input.mouse_mode = Input.MOUSE_MODE_VISIBLE
		pan_velocity = Vector3.ZERO
	
	if event is InputEventMouseMotion && rotating:
		var mouse_movement = event.relative * rotation_speed
		rotate_velocity -= mouse_movement
	elif rotating:
		rotate_velocity = Vector2.ZERO
	
	if event is InputEventMouseMotion && panning:
		var mouse_movement = event.relative * movement_speed
		pan_velocity = Vector3(-mouse_movement.x, mouse_movement.y, 0).rotated(Vector3.UP, rotation.y)
	elif panning:
		pan_velocity = Vector3.ZERO

func _process(delta):
	if rotating:
		rotation.y += rotate_velocity.x*delta
		vertical_pivot.rotation.x += rotate_velocity.y*delta
	if panning:
		var move_by = pan_velocity * delta
		position += move_by
