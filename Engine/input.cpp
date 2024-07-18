#include "input.hpp"

void register_key(Input* input, KeyCode key_code)
{
	input->key_downs[key_code] = true;
	input->key_presses[key_code] = true;
}

void reset_key_downs(Input* input)
{
	for (size_t i = 0; i < 26; i++)
	{
		input->key_downs[i] = false;
	}
}

void reset_key_press(Input* input, KeyCode key_code)
{
	input->key_presses[key_code] = false;
}

bool is_key_pressed(const Input* input, KeyCode key_code)
{
	return input->key_presses[key_code];
}

bool is_key_down(const Input* input, KeyCode key_code)
{
	return input->key_downs[key_code];
}

void update_mouse_position(Input* input, Vector2 mouse_position)
{
	input->mouse_position = mouse_position;
}

void register_mouse_button_down(Input* input, MouseButton mouse_button)
{
	input->mouse_button_downs[mouse_button] = true;
	input->mouse_button_presses[mouse_button] = true;
}

void reset_mouse_button_downs(Input* input)
{
	for (size_t i = 0; i < 3; i++)
	{
		input->mouse_button_downs[i] = false;
	}
}

void reset_mouse_button_press(Input* input, MouseButton mouse_button)
{
	input->mouse_button_presses[mouse_button] = false;
}

bool is_mouse_button_pressed(const Input* input, MouseButton mouse_button)
{
	return input->mouse_button_presses[mouse_button];
}

bool is_mouse_button_down(const Input* input, MouseButton mouse_button)
{
	return input->mouse_button_downs[mouse_button];
}