
#include "input_line.h"

input_line_t input_line_create(size_t n_ctrls, ...)
{
	if(n_ctrls == 0)
	{
		return SEPARATOR_LINE;
	}
	va_list args;
	input_line_t line;
	line.hwnd_arr_len = n_ctrls;
	line.hwnd_arr = malloc(n_ctrls*sizeof(HWND));
	va_start(args,n_ctrls);
	for(size_t i = 0; i < n_ctrls; i++)
	{
		line.hwnd_arr[i] = va_arg(args,HWND);
	}
	va_end(args);
	return line;
}

void input_line_move(input_line_t line, ssize_t offset)
{
	offset *= LINE_HEIGHT;
	for(size_t i = 0; i < line.hwnd_arr_len; i++)
        {
                HWND hWnd = line.hwnd_arr[i];
		RECT rct; GetWindowRect(hWnd,&rct);
		POINT pt = {rct.left,rct.top};
		ScreenToClient(__hMainWnd,&pt);
		SetWindowPos(hWnd,NULL,pt.x,pt.y+offset,0,0,SWP_NOSIZE);
        }
}

void input_line_destroy(input_line_t*input)
{
	for(size_t i = 0; i < input->hwnd_arr_len; i++)
	{
		DestroyWindow(input->hwnd_arr[i]);
	}
	free(input->hwnd_arr);
}

