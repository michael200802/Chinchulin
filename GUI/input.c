#include "input.h"

def_unit_t meter_units[] = {{"m",1},{"km",1/1000},{"Mm",1/1000000},{"Gm",1/1000000000},{"Tm",1/1000000000000},{NULL,0}};
def_unit_t gr_units[] = {{"g",1},{"kg",1/1000},{"Mg",1/1000000},{"Gg",1/1000000000},{"Tg",1/1000000000000},{NULL,0}};

HWND __hMainWnd;
HINSTANCE __hIns;

struct __line_arr __lines = {0,NULL};

void input_add_line(size_t pos, input_line_t line)
{
	__lines.n_lines++;
        __lines.line_arr = realloc(__lines.line_arr,__lines.n_lines*sizeof(input_line_t));

	size_t index = pos;
	input_line_t cur_line, prev_line = __lines.line_arr[index];
	printf("%d -> %d-------------------\n",pos,__lines.n_lines);
	for(size_t i = index+1; i < __lines.n_lines; i++)
	{

		puts("moving---");
		input_line_move(prev_line,true);

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
	InvalidateRect(__hMainWnd,NULL,TRUE);
}
