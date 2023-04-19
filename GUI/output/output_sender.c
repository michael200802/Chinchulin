#include "output_sender.h"

void*output_sender_routine(void*arg)
{
	output_sender_t * sender = arg;

	HDC hDC = GetDC(sender->hWnd);
	size_t width, height;
	long wnd_x_center, wnd_y_center;
	long x_center, y_center;

	{
		SCROLLINFO sif = {.cbSize = sizeof(SCROLLINFO), .fMask = SIF_PAGE|SIF_POS|SIF_RANGE};
		GetScrollInfo(sender->hWnd,SB_HORZ,&sif);
		width = sif.nPage;
		wnd_x_center = width/2;
		x_center = sif.nPos-(sif.nMax-sif.nMin)/2;
	}

	{
		SCROLLINFO sif = {.cbSize = sizeof(SCROLLINFO), .fMask = SIF_PAGE|SIF_POS|SIF_RANGE};
		GetScrollInfo(sender->hWnd,SB_VERT,&sif);
		height = sif.nPage;
		wnd_y_center = height/2;
		y_center = sif.nPos-(sif.nMax-sif.nMin)/2;
	}

	BitBlt(hDC,0,0,width,height,0,0,0,BLACKNESS);

	HPEN hOldPen = SelectObject(hDC,CreatePen(PS_SOLID,1,RGB(255,255,255)));

        output_drawing_t drawing = output_drawing_queue_dequeue(sender->queue);
        for(size_t i = 0;i < drawing.img_arr_len; i++)
        {
                output_img_t cur_img = drawing.img_arr[i];
                HBRUSH hOldBrush = SelectObject(hDC,CreateSolidBrush(cur_img.rgb));
                Ellipse(hDC,
                        wnd_x_center+x_center+cur_img.screen_x-cur_img.radius,wnd_y_center+y_center+cur_img.screen_y-cur_img.radius,
                        wnd_x_center+x_center+cur_img.screen_x+cur_img.radius,wnd_y_center+y_center+cur_img.screen_y+cur_img.radius);

                MoveToEx(hDC,
                        wnd_x_center+x_center+cur_img.screen_x,
                        wnd_y_center+y_center+cur_img.screen_y,NULL);
                LineTo(hDC,
                        wnd_x_center+x_center+cur_img.lend_x,
                        wnd_y_center+y_center+cur_img.lend_y);

                DeleteObject(SelectObject(hDC,hOldBrush));

                output_img_t img = cur_img;
        }
	free(drawing.img_arr);

        DeleteObject(SelectObject(hDC,hOldPen));

	ReleaseDC(sender->hWnd,hDC);

	usleep(100*1000);
}

void output_sender_create(output_worker_t * sender, output_drawing_queue_t * queue, HWND hWnd)
{
	output_sender_t * arg = malloc(sizeof(output_sender_t));
	*arg = (output_sender_t){.queue = queue, .hWnd = hWnd};
	output_worker_create(sender, output_sender_routine, arg);
}
