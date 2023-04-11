#pragma once

#include "types.h"
#include "input_line.h"

extern def_unit_t meter_units[10];
extern def_unit_t gr_units[10];
extern def_unit_t angle_units[3];

extern HWND __hMainWnd;
extern HINSTANCE __hIns;

#define LEFT_MARGIN 15

#define LINE_HEIGHT 22

#define TAB_WIDTH 15

#define ntab_to_screen(_ntab) ((_ntab)*TAB_WIDTH+LEFT_MARGIN)
#define line_to_screen(_line) ((_line)*LINE_HEIGHT)

#define INPUT_QUANTITY_N_LINES (5)
#define INPUT_BODY_N_LINES (1+INPUT_QUANTITY_N_LINES*2)
#define INPUT_PLANET_N_LINES (3+4*INPUT_QUANTITY_N_LINES+INPUT_BODY_N_LINES)

#define INPUT_PLANET_SYS_CHILD_MIN_N_LINES (INPUT_PLANET_N_LINES + 2/*Separator line + hButton_new_elem line*/)
#define INPUT_PLANET_SYS_MIN_N_LINES ((3+2*INPUT_QUANTITY_N_LINES+INPUT_BODY_N_LINES) + 2/*Separator line + hButton_new_elem line*/)

#define SEPARATOR_LINE (input_line_t){NULL,0}

#define input_set_mainwnd(hwnd) __hMainWnd = hwnd;
#define input_set_ins(hIns) __hIns = hIns;

struct __line_arr
{
	size_t n_lines;
	input_line_t * line_arr;
};
extern struct __line_arr __lines;

#define input_get_n_lines() __lines.n_lines

//pos (0 - __lines.n_lines)

void input_add_line(size_t pos, input_line_t line);

void input_add_line_attop(input_line_t line);

void input_delete_line(size_t pos);

ssize_t input_get_wnd_line(HWND hWnd);

ssize_t input_get_wnd_tab(HWND hWnd);

size_t input_get_str_width(const char* str);

void input_repaint();

#define DESTROY_BUTTON_TEXT "Eliminar"
#define DESTROY_BUTTON_WIDTH 70

#define input_obj_set_defctrls(obj,name,_line_start,n_tab) \
	(obj).hEdit_name = CreateWindow(WC_EDIT,name,WS_VISIBLE|WS_BORDER|WS_CHILD|ES_LEFT,ntab_to_screen(n_tab),line_to_screen(_line_start),input_get_str_width(name),LINE_HEIGHT,__hMainWnd,NULL,__hIns,NULL); \
	(obj).hButton_destroy = CreateWindow(WC_BUTTON,DESTROY_BUTTON_TEXT,WS_VISIBLE|WS_BORDER|WS_CHILD|BS_LEFT|BS_PUSHBUTTON,ntab_to_screen(n_tab+1),line_to_screen(_line_start+2),DESTROY_BUTTON_WIDTH,LINE_HEIGHT,__hMainWnd,NULL,__hIns,NULL); \
	input_add_line(_line_start,input_line_create(1,(obj).hEdit_name));\
	input_add_line(_line_start+1,SEPARATOR_LINE);\
	input_add_line(_line_start+2,input_line_create(1,(obj).hButton_destroy));
