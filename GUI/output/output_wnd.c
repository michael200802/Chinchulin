#include "output_wnd.h"

#define POPUP_WIDTH 10000
#define POPUP_CENTER (POPUP_WIDTH/2)
#define POPUPCLASS_NAME "Popup window class"

LRESULT CALLBACK PopUpWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
		case WM_CREATE:
			{
				SCROLLINFO sif = {.cbSize = sizeof(SCROLLINFO), .fMask = SIF_RANGE|SIF_POS};
				sif.nPos = POPUP_CENTER;
				sif.nMax = POPUP_WIDTH;
				sif.nMin = 0;
				SetScrollInfo(hWnd,SB_VERT,&sif,TRUE);
				SetScrollInfo(hWnd,SB_HORZ,&sif,TRUE);
			}
			break;
		case WM_SIZE:
			{
				SCROLLINFO sif = {.cbSize = sizeof(SCROLLINFO), .fMask = SIF_PAGE};

				long wnd_x_center = LOWORD(lParam)/2;
				long wnd_y_center = HIWORD(lParam)/2;

				sif.nPage = HIWORD(lParam);
				SetScrollInfo(hWnd,SB_VERT,&sif,TRUE);
				sif.nPage = LOWORD(lParam);
				SetScrollInfo(hWnd,SB_HORZ,&sif,TRUE);
			}
			break;
		case WM_VSCROLL:
		case WM_HSCROLL:
			{
				long pos;
				int sb_type = (Msg == WM_VSCROLL ? SB_VERT: SB_HORZ);
				SCROLLINFO sif = {.cbSize = sizeof(SCROLLINFO), .fMask = SIF_POS|SIF_PAGE|SIF_RANGE};
				GetScrollInfo(hWnd,sb_type,&sif);
				pos = sif.nPos;
				switch(LOWORD(wParam))
				{
					case SB_THUMBPOSITION:
					case SB_THUMBTRACK:
						pos = HIWORD(wParam);
						break;
					case SB_LINEUP:
						pos--;
						break;
					case SB_LINEDOWN:
						pos++;
						break;
					case SB_PAGEUP:
						pos-=sif.nPage;
						break;
					case SB_PAGEDOWN:
						pos+=sif.nPage;;
						break;
				}

				long pos_dif = pos-sif.nPos;
				if(pos_dif > 0)
				{
					sif = (SCROLLINFO){.cbSize = sizeof(SCROLLINFO), .fMask = SIF_POS|SIF_RANGE, .nPos = pos,
						.nMin = sif.nMin,
						.nMax = sif.nMax+pos_dif
					};
				}
				else
				{
					sif = (SCROLLINFO){.cbSize = sizeof(SCROLLINFO), .fMask = SIF_POS|SIF_RANGE, .nPos = pos,
						.nMin = sif.nMin+pos_dif,
						.nMax = sif.nMax
					};

				}

				SetScrollInfo(hWnd,sb_type,&sif,TRUE);
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

typedef struct
{
        plt_sys_t plt_sys;

        mpfr_t time_unit;
        mpfr_t dis_unit;

}output_wnd_routine_arg_t;

void*output_wnd_routine(void* _arg)
{
	HWND hWnd = CreateWindow(
		POPUPCLASS_NAME,
		"Chinchulin",
		WS_VISIBLE|WS_OVERLAPPEDWINDOW|WS_VSCROLL|WS_HSCROLL,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		GetModuleHandle(NULL),
		NULL
	);

	output_wnd_routine_arg_t * arg = _arg;

	output_drawing_queue_t queue;
	output_drawing_queue_create(&queue);

	output_worker_t sender;
	output_worker_t render;
	output_sender_create(&sender,&queue,hWnd);
	output_render_create(&render,&queue,arg->plt_sys,arg->time_unit,arg->dis_unit);

	mpfr_clears(arg->time_unit,arg->dis_unit,(mpfr_ptr)0);

	MSG Msg;
	while(GetMessage(&Msg,NULL,0,0))
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	output_worker_destroy(&sender);
	output_worker_destroy(&render);
	plt_sys_destroy(&arg->plt_sys);

	return NULL;
}

void output_start(plt_sys_t plt_sys, mpfr_t time_unit,mpfr_t dis_unit)
{
        static bool first_time = true;
        if(first_time)
        {
                WNDCLASS WndClass = {};
                WndClass.lpfnWndProc = PopUpWndProc;
                WndClass.style = CS_HREDRAW|CS_VREDRAW;
                WndClass.hInstance = GetModuleHandle(NULL);
                WndClass.hIcon = LoadIcon(NULL,IDI_APPLICATION);
                WndClass.hCursor = LoadCursor(NULL,IDC_ARROW);
                WndClass.hbrBackground = CreateSolidBrush(RGB(0,0,0));
                WndClass.lpszClassName = POPUPCLASS_NAME;

                RegisterClass(&WndClass);

		first_time = false;
        }

	output_wnd_routine_arg_t * arg = malloc(sizeof(output_wnd_routine_arg_t));
	*arg = (output_wnd_routine_arg_t){.plt_sys = plt_sys};
	mpfr_inits2(MPFR_DEFAULT_PRECISION,arg->time_unit,arg->dis_unit,(mpfr_ptr)0);
	mpfr_set(arg->time_unit,time_unit,MPFR_DEFAULT_RND);
	mpfr_set(arg->dis_unit,dis_unit,MPFR_DEFAULT_RND);
	output_worker_create_static(output_wnd_routine,arg);

}
