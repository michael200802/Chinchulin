#pragma once

#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>

typedef struct
{
	HWND * hwnd_arr;
	size_t hwnd_arr_len;
}input_line_t;

typedef double num_t;

typedef struct
{
        const char * name;
        num_t constant;
}def_unit_t;

typedef struct
{
	HWND hStatic_name;//name of the quantity
	HWND hEdit_num;
	HWND hStatic_tenlabel;//10^exp
	HWND hEdit_exp;//exp
	HWND hCB_unit;
	HWND hStatic_Error;//shows errors
	def_unit_t* units_arr;//constants to convert num to default unit (1/100 for km to m)
}input_quantity_t;

typedef struct
{
	HWND hStatic_name;
	input_quantity_t mass;
	input_quantity_t radius;
}input_body_t;

//OBJ
typedef struct
{
	//name of the element
	HWND hEdit_name;

	//def attr
        input_quantity_t aphelion;
        input_quantity_t perihelion;
	input_quantity_t inclination_angle;
	input_quantity_t phase_angle;
	input_body_t body;

	//self destruct
	HWND hButton_destroy;
}input_planet_t;

typedef struct __input_planet_sys_child
{
	//name of the element
	HWND hEdit_name;

	//self destruct
	HWND hButton_destroy;

	//orbit
	input_quantity_t aphelion;
	input_quantity_t perihelion;
	input_quantity_t inclination_angle;
	input_quantity_t phase_angle;

	//def attr
	input_body_t star;
	HWND hButton_new_planet, hButton_new_sys;
	input_planet_t* planets_arr;
	size_t planets_arr_len;
	struct __input_planet_sys_child* planet_sys_childs_arr;
	size_t planet_sys_childs_arr_len;

}input_planet_sys_child_t;

typedef struct
{
	//name of the element
	HWND hEdit_name;

	//self destruct
	HWND hButton_destroy;

	//coords
	input_quantity_t x_pos;
	input_quantity_t y_pos;

	//def attr
	input_body_t star;
	HWND hButton_new_planet, hButton_new_sys;
	input_planet_t* planets_arr;
	size_t planets_arr_len;
	input_planet_sys_child_t* planet_sys_childs_arr;
	size_t planet_sys_childs_arr_len;

}input_planet_sys_t;
//OBJ

