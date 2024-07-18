#pragma once

#include "math.hpp"

enum KeyCode
{
	KEY_A,
	KEY_B,
	KEY_C,
	KEY_D,
	KEY_E,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_I,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_M,
	KEY_N,
	KEY_O,
	KEY_P,
	KEY_Q,
	KEY_R,
	KEY_S,
	KEY_T,
	KEY_U,
	KEY_V,
	KEY_W,
	KEY_X,
	KEY_Y,
	KEY_Z
};

enum MouseButton
{
	MOUSE_BUTTON_LEFT,
	MOUSE_BUTTON_MIDDLE,
	MOUSE_BUTTON_RIGHT
};

struct Input
{
	bool key_downs[26];
	bool key_presses[26];
	Vector2 mouse_position;
	bool mouse_button_downs[3];
	bool mouse_button_presses[3];
};

void register_key(Input* input, KeyCode key_code);
void reset_key_downs(Input* input);
void reset_key_press(Input* input, KeyCode key_code);
bool is_key_pressed(const Input* input, KeyCode key_code);
bool is_key_down(const Input* input, KeyCode key_code);
void update_mouse_position(Input* input, Vector2 mouse_position);
void register_mouse_button_down(Input* input, MouseButton mouse_button);
void reset_mouse_button_downs(Input* input);
void reset_mouse_button_press(Input* input, MouseButton mouse_button);
bool is_mouse_button_pressed(const Input* input, MouseButton mouse_button);
bool is_mouse_button_down(const Input* input, MouseButton mouse_button);