#include "GUI/input_planet_sys.h"

input_planet_sys_t * plt_sys_arr = NULL;
size_t plt_sys_arr_len = 0;

void __handle_command_msg(HWND hWnd, input_planet_sys_child_t * plt_sys_child_arr)
{
        input_planet_sys_child_t * cur_plt_sys = plt_sys_child_arr;
        if(hWnd == cur_plt_sys->hButton_new_sys)
        {
                input_planet_sys_child_add_sys(cur_plt_sys);
                return;
        }
        else if(hWnd == cur_plt_sys->hButton_new_planet)
        {
                input_planet_sys_child_add_planet(cur_plt_sys);
                return;
        }
	else
	{
		input_planet_t* planets_arr = cur_plt_sys->planets_arr;
	        size_t planets_arr_len = cur_plt_sys->planets_arr_len;
		for(size_t i = 0; i < planets_arr_len; i++)
		{
			input_planet_t cur_plt = planets_arr[i];
			if(cur_plt.hButton_destroy == hWnd)
			{
				input_planet_sys_child_remove_planet(cur_plt_sys,i);
				return;
			}
		}

		input_planet_sys_child_t* plt_sys_child_arr = cur_plt_sys->planet_sys_childs_arr;
		size_t plt_sys_child_arr_len = cur_plt_sys->planet_sys_childs_arr_len;
		for(size_t i = 0; i < plt_sys_child_arr_len; i++)
		{
			if(plt_sys_child_arr[i].hButton_destroy == hWnd)
			{
				input_planet_sys_child_remove_sys(cur_plt_sys,i);
				return;
			}
			else
			{
				__handle_command_msg(hWnd,plt_sys_child_arr+i);
			}
		}
	}
}

void handle_command_msg(LPARAM lParam)
{
	HWND hWnd = (HWND)lParam;

	for(size_t i = 0; i < plt_sys_arr_len; i++)
	{
		input_planet_sys_t * cur_plt_sys = &plt_sys_arr[i];
		if(hWnd == cur_plt_sys->hButton_new_sys)
		{
			input_planet_sys_add_sys(cur_plt_sys);
			return;
		}
		else if(hWnd == cur_plt_sys->hButton_new_planet)
		{
			input_planet_sys_add_planet(cur_plt_sys);
			return;
		}
		else if(hWnd == cur_plt_sys->hButton_destroy)
		{
			input_planet_sys_destroy(cur_plt_sys);
			size_t index = i;
			for(; index < plt_sys_arr_len-1; index++)
			{
				plt_sys_arr[index] = plt_sys_arr[index+1];
			}
			plt_sys_arr_len--;
			plt_sys_arr = realloc(plt_sys_arr, plt_sys_arr_len*sizeof(input_planet_sys_t));
			return;
		}
		else
		{
	        	input_planet_t* planets_arr = cur_plt_sys->planets_arr;
		        size_t planets_arr_len = cur_plt_sys->planets_arr_len;

			for(size_t i = 0; i < planets_arr_len; i++)
			{
				input_planet_t cur_plt = planets_arr[i];
				if(cur_plt.hButton_destroy == hWnd)
				{
					puts("Nayn");

					input_planet_sys_remove_planet(cur_plt_sys,i);
					return;
				}
			}

			input_planet_sys_child_t* plt_sys_child_arr = cur_plt_sys->planet_sys_childs_arr;
			size_t plt_sys_child_arr_len = cur_plt_sys->planet_sys_childs_arr_len;
			for(size_t i = 0; i < plt_sys_child_arr_len; i++)
			{
				if(plt_sys_child_arr[i].hButton_destroy == hWnd)
				{
					input_planet_sys_remove_sys(cur_plt_sys,i);
					return;
				}
				else
				{
					__handle_command_msg(hWnd,plt_sys_child_arr+i);
				}
			}
		}
		break;
	}
}

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	static HINSTANCE hIns;
	static HWND hBtnAddSys;
	static size_t wnd_height;
	static BOOL scroll_shown = TRUE;
	switch(Msg)
	{
		case WM_CREATE:

			hIns = ((CREATESTRUCT*)lParam)->hInstance;
			input_set_mainwnd(hWnd);
			input_set_ins(hIns);

			input_add_line(input_get_n_lines(),input_line_create(
				1,
			       	hBtnAddSys = CreateWindow(
			                WC_BUTTON,
			                "Registrar sistema",
			                WS_VISIBLE|WS_CHILD|WS_BORDER|BS_PUSHBUTTON,
			                ntab_to_screen(0),
			                line_to_screen(0),
			                input_get_str_width("Registrar sistema"),
			                LINE_HEIGHT,
			                hWnd,
			                NULL,
			                hIns,
			                NULL
			                )
				)
			);

			{
				SCROLLINFO sif = {.cbSize = sizeof(SCROLLINFO), .fMask = SIF_PAGE, .nPage = LINE_HEIGHT};
				SetScrollInfo(hWnd,SB_VERT,&sif,FALSE);
			}

			break;
		case WM_COMMAND:
			//Handle buttons
			{
				SCROLLINFO sif = {.cbSize = sizeof(SCROLLINFO), .fMask = SIF_POS};
                                GetScrollInfo(hWnd,SB_VERT,&sif);
				if(sif.nPos != 0) {ScrollWindow(hWnd,0,sif.nPos,NULL,NULL);}
				if((HWND)lParam == hBtnAddSys)
				{
					char buff[100]; sprintf(buff,"Sistema planetario numero %zu",plt_sys_arr_len);
					input_planet_sys_t new_plt_sys = input_planet_sys_create(buff,input_get_wnd_line(hBtnAddSys)+input_get_n_lines(),1);
					plt_sys_arr_len++;
					plt_sys_arr = realloc(plt_sys_arr,plt_sys_arr_len*sizeof(input_planet_sys_t));
					plt_sys_arr[plt_sys_arr_len-1] = new_plt_sys;
				}
				else
				{
					handle_command_msg(lParam);
				}
				if(sif.nPos != 0) {ScrollWindow(hWnd,0,-1*sif.nPos,NULL,NULL);}
			}
		case WM_SIZE:
			if(Msg == WM_SIZE) {wnd_height = HIWORD(lParam);}

			printf("%d -> %d\n", wnd_height/LINE_HEIGHT, input_get_n_lines());
			if(wnd_height/LINE_HEIGHT < input_get_n_lines())
			{
				if(scroll_shown == FALSE)
				{
					scroll_shown = TRUE;
					ShowScrollBar(hWnd,SB_VERT,TRUE);
				}
				SCROLLINFO scinf = {
						.cbSize = sizeof(SCROLLINFO),
						.fMask = SIF_RANGE,
						.nMin = 0,
						.nMax = (input_get_n_lines())*LINE_HEIGHT-LINE_HEIGHT
				};
				SetScrollInfo(hWnd,SB_VERT,&scinf,TRUE);
			}
			else if(scroll_shown == TRUE)
			{
				scroll_shown = FALSE;
				ShowScrollBar(hWnd,SB_VERT,FALSE);
			}
			input_repaint();
			UpdateWindow(hWnd);
			break;
		case WM_VSCROLL:
			{
				size_t prev_nPos;
				SCROLLINFO sif = {.cbSize = sizeof(SCROLLINFO), .fMask = SIF_POS};
				GetScrollInfo(hWnd,SB_VERT,&sif);
				prev_nPos = sif.nPos;
				switch(LOWORD(wParam))
				{
					case SB_THUMBPOSITION:
					case SB_THUMBTRACK:
						sif.nPos = HIWORD(wParam);
						break;
					case SB_LINEUP:
						sif.nPos++;
						break;
					case SB_LINEDOWN:
						sif.nPos--;
						break;

					case SB_PAGEUP:
						sif.nPos++;
						break;
					case SB_PAGEDOWN:
						sif.nPos--;
						break;
				}
				if(prev_nPos != sif.nPos)
				{
					size_t new_nPos = sif.nPos;
					ScrollWindow(hWnd,0,-1*(new_nPos-prev_nPos),NULL,NULL);
					sif = (SCROLLINFO){.cbSize = sizeof(SCROLLINFO), .fMask = SIF_POS, .nPos = new_nPos};
					SetScrollInfo(hWnd,SB_VERT,&sif,TRUE);
				}
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd,Msg,wParam,lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hCurInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iShowCmd)
{
	WNDCLASS WndClass = {};
	WndClass.hInstance = hCurInstance;
	WndClass.hIcon = LoadIcon(NULL,IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL,IDC_ARROW);
	WndClass.hbrBackground = CreateSolidBrush(RGB(255,255,255));
	WndClass.lpszClassName = "MainWndClass";
	WndClass.lpfnWndProc = MainWndProc;
	WndClass.style = CS_HREDRAW|CS_VREDRAW;

	RegisterClass(&WndClass);

	HWND hWnd = CreateWindow(
			WndClass.lpszClassName,
			"Chinchulin",
			WS_VISIBLE|WS_OVERLAPPEDWINDOW|WS_VSCROLL,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			NULL,
			NULL,
			hCurInstance,
			NULL
		);

	MSG Msg;
	while(GetMessage(&Msg,NULL,0,0) == TRUE)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return 0;
}
