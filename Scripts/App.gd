extends Node

@onready var printable_gear = $PrintableGear

@onready var module_input = $MainUI/MarginContainer/VBoxContainer/PanelContainer/MarginContainer/VBoxContainer/HBoxContainer/ModuleInput
@onready var num_teeth_input = $MainUI/MarginContainer/VBoxContainer/PanelContainer/MarginContainer/VBoxContainer/HBoxContainer2/NumTeethInput
@onready var width_input = $MainUI/MarginContainer/VBoxContainer/PanelContainer/MarginContainer/VBoxContainer/HBoxContainer3/WidthInput
@onready var profile_shift_input = $MainUI/MarginContainer/VBoxContainer/PanelContainer/MarginContainer/VBoxContainer/HBoxContainer4/ProfileShiftInput
@onready var pressure_angle_input = $MainUI/MarginContainer/VBoxContainer/PanelContainer/MarginContainer/VBoxContainer/HBoxContainer5/PressureAngleInput
@onready var involute_steps_input = $MainUI/MarginContainer/VBoxContainer/PanelContainer/MarginContainer/VBoxContainer/HBoxContainer6/InvoluteStepsInput

@onready var file_dialog = $FileDialog

func _ready():
	module_input.set_value_no_signal(printable_gear.GetModule())
	num_teeth_input.set_value_no_signal(printable_gear.GetNumberOfTeeth())
	width_input.set_value_no_signal(printable_gear.GetWidth())
	profile_shift_input.set_value_no_signal(printable_gear.GetProfileShift())
	pressure_angle_input.set_value_no_signal(printable_gear.GetPressureAngle())
	involute_steps_input.set_value_no_signal(printable_gear.GetInvoluteSteps())


func _on_module_input_value_changed(value):
	printable_gear.SetModule(value)


func _on_num_teeth_input_value_changed(value):
	printable_gear.SetNumberOfTeeth(value)


func _on_width_input_value_changed(value):
	printable_gear.SetWidth(value)


func _on_profile_shift_input_value_changed(value):
	printable_gear.SetProfileShift(value)


func _on_pressure_angle_input_value_changed(value):
	printable_gear.SetPressureAngle(value)


func _on_involute_steps_input_value_changed(value):
	printable_gear.SetInvoluteSteps(value)


func _on_file_id_pressed(id):
	match id:
		0:
			file_dialog.popup_centered()


func _on_file_dialog_confirmed():
	var filename: String = file_dialog.current_path
	printable_gear.SaveFile(filename)
