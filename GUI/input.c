#include "input.h"

def_unit_t meter_units[] = {{"m","1"},{"km","0.001"},{"Mm","0.000001"},{"Gm","0.000000001"},{"Tm","0.000000000001"},{"ua","149597870700"},{NULL,0}};
def_unit_t gr_units[] =    {{"g","1"},{"kg","0.001"},{"Mg","0.000001"},{"Gg","0.000000001"},{"Tg","0.000000000001"},{NULL,0}};
def_unit_t angle_units[] = {{"rad","1"},{L"°","0.017453293"},{NULL,0}};
def_unit_t long_time_units[] = {{"ano","1"},{NULL,0}};
def_unit_t short_time_units[] = {{"ms","1"},{"s","1000"},{NULL,0}};

HWND __hMainWnd;
HINSTANCE __hIns;

struct __line_arr __lines = {0,NULL};

void input_add_line(size_t pos, input_line_t line)
{
	__lines.n_lines++;
        __lines.line_arr = realloc(__lines.line_arr,__lines.n_lines*sizeof(input_line_t));

	size_t index = pos;
	input_line_t cur_line, prev_line = __lines.line_arr[index];
	for(size_t i = index+1; i < __lines.n_lines; i++)
	{
		cur_line = __lines.line_arr[i];
		__lines.line_arr[i] = prev_line;
		prev_line = cur_line;
	}
	__lines.line_arr[pos] = line;
}

void input_delete_line(size_t pos)
{
	size_t index = pos;
	input_line_destroy(__lines.line_arr+index);

	for(size_t i = index; i < __lines.n_lines-1; i++)
	{
		__lines.line_arr[i] = __lines.line_arr[i+1];
		input_line_move(__lines.line_arr[i],false);
	}

	__lines.n_lines--;
	__lines.line_arr = realloc(__lines.line_arr,__lines.n_lines*sizeof(input_line_t));
}


void input_add_line_attop(input_line_t line)
{
	__lines.n_lines++;
	__lines.line_arr = realloc(__lines.line_arr,__lines.n_lines);
	__lines.line_arr[input_get_n_lines()-1] = line;
}

ssize_t input_get_wnd_line(HWND hWnd)
{
	for(size_t i = 0; i < __lines.n_lines; i++)
	{
		size_t pos = i;
		input_line_t line = __lines.line_arr[i];
		for(size_t i = 0; i < line.hwnd_arr_len; i++)
		{
			if(line.hwnd_arr[i] == hWnd)
			{
				return pos;
			}
		}
	}
	return -1;
}

ssize_t input_get_wnd_tab(HWND hWnd)
{
	RECT rct;
	POINT pt;
	ssize_t tab;
	GetWindowRect(hWnd,&rct);
	pt.x = rct.left;
	pt.y = rct.top;
	ScreenToClient(__hMainWnd,&pt);
	tab = pt.x-LEFT_MARGIN;
	tab /= TAB_WIDTH;
	return tab;
}

size_t input_get_str_width(const char* str)
{
	size_t len = 0;
	while(*str != '\0')
	{
		len++;
		str++;
	}
	size_t width = len*10;
	return width;
}

void input_repaint()
{
       for(ssize_t i = 0; i < __lines.n_lines; i++)
        {
                input_line_t line = __lines.line_arr[i];
		if(line.hwnd_arr_len != 0)
		{
	                ssize_t pos;
			{
				RECT rct;
				GetWindowRect(line.hwnd_arr[0],&rct);
				POINT pt = {rct.right,rct.bottom};
				ScreenToClient(__hMainWnd,&pt);
				pos = pt.y/LINE_HEIGHT-1;
			}
			ssize_t offset = i-pos;
			if(offset != 0)
			{
				input_line_move(line,offset);
			}
		}
        }
	InvalidateRect(__hMainWnd,NULL,TRUE);
}
