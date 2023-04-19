#include <stdio.h>

#include <math.h>

#include "GUI/input_planet_sys.h"

#include "planet_sys/plt_sys_entity.h"

#include <pthread.h>

typedef struct
{
	mpfr_t esc_time, esc_dis;
	size_t len;
	plt_sys_entity_t * arr;
}popup_wnd_arg_t;

#define POPUPCLASS_NAME "Popup window class"

#include "GUI/input_drawing.h"

typedef struct
{
	popup_wnd_arg_t * pu_arg;
	input_drawing_queue_t * queue;
	pthread_mutex_t mutex;
	bool stop;
}render_routine_arg_t;

#include <unistd.h>

void* render_routine(void*_arg)
{
	render_routine_arg_t * render = _arg;

	input_drawing_queue_t ** link = &render->queue;

	for(;;)
	{
		pthread_mutex_lock(&render->mutex);

		if(render->stop)
		{
			return NULL;
		}

		input_drawing_queue_t * new_node = malloc(sizeof(input_drawing_queue_t));

		input_drawing_t new_drawing = { malloc(sizeof(input_img_t)*render->pu_arg->len), render->pu_arg->len};

		for(size_t i = 0; i < render->pu_arg->len; i++)
		{
			plt_sys_entity_t * ent = render->pu_arg->arr+i;

			//set x and y
			MPFR_DECL_INIT(x_pos,MPFR_DEFAULT_PRECISION);
			MPFR_DECL_INIT(y_pos,MPFR_DEFAULT_PRECISION);
			plt_sys_get_real_coord(x_pos,y_pos,&ent->coord);

			mpfr_div(x_pos,x_pos,render->pu_arg->esc_dis,MPFR_DEFAULT_RND);
			mpfr_div(y_pos,y_pos,render->pu_arg->esc_dis,MPFR_DEFAULT_RND);
			input_img_t img = {
					.screen_x = mpfr_get_si(x_pos,MPFR_DEFAULT_RND),
					.screen_y = -1*mpfr_get_si(y_pos,MPFR_DEFAULT_RND),
				};


			//set radius
			MPFR_DECL_INIT(radius,MPFR_DEFAULT_PRECISION);
			mpfr_div(radius,ent->body.radius,render->pu_arg->esc_dis,MPFR_DEFAULT_RND);
			img.radius = mpfr_get_ui(radius,MPFR_DEFAULT_RND)+1;

			//set line x and y
			if(ent->coord.origin)
			{
				plt_sys_get_real_coord(x_pos,y_pos,ent->coord.origin);
				mpfr_div(x_pos,x_pos,render->pu_arg->esc_dis,MPFR_DEFAULT_RND);
				mpfr_div(y_pos,y_pos,render->pu_arg->esc_dis,MPFR_DEFAULT_RND);
				img.lend_x = mpfr_get_si(x_pos,MPFR_DEFAULT_RND);
				img.lend_y = -1*mpfr_get_si(y_pos,MPFR_DEFAULT_RND);
			}
			else
			{
				img.lend_x = 0;
				img.lend_y = 0;
			}

			//color of the circle
			float cur_angle = mpfr_get_flt(ent->coord.cur_angle,MPFR_DEFAULT_RND);
			unsigned char green_val, blue_val;
			green_val = blue_val = 255-255 * sqrt( (cos(cur_angle)+1)/2 );
			img.rgb = RGB(255,green_val,blue_val);

			new_drawing.img_arr[i] = img;
		}
		for(size_t i = 0; i < render->pu_arg->len; i++)
		{
			plt_sys_entity_t * ent = render->pu_arg->arr+i;
			ent->trajectory.update_coord(ent,render->pu_arg->esc_time);
		}

		new_node->drawing = new_drawing;
		new_node->next = NULL;

		*link = new_node;
		link = &new_node->next;

		pthread_mutex_unlock(&render->mutex);

		usleep(25*1000);
	}
}

#define POPUP_WIDTH 10000
#define POPUP_CENTER (POPUP_WIDTH/2)

LRESULT CALLBACK PopUpWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	static size_t ent_arr_len;
	static plt_sys_entity_t * ent_arr;

	static render_routine_arg_t * render;

	static long x_max, x_min, y_max, y_min;
	static long x_center, y_center;
	static long wnd_x_center, wnd_y_center;

	switch(Msg)
	{
		case WM_CREATE:
			{
				SetTimer(hWnd,0,100,NULL);

				render = ((CREATESTRUCT*)lParam)->lpCreateParams;

				x_max = POPUP_WIDTH;
				x_min = 0;

				y_max = POPUP_WIDTH;
				y_min = 0;

				x_center = 0;
				y_center = 0;

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

				wnd_x_center = LOWORD(lParam)/2;
				wnd_y_center = HIWORD(lParam)/2;

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
				if(Msg == WM_VSCROLL)
				{
					y_center -= pos_dif;
				}
				else
				{
					x_center -= pos_dif;
				}
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
		case WM_TIMER:
			InvalidateRect(hWnd,NULL,TRUE);
			break;
		case WM_PAINT:
			{
				pthread_mutex_lock(&render->mutex);

				if(render->queue)
				{
					PAINTSTRUCT ps;
					HDC hDC = BeginPaint(hWnd,&ps);

					HPEN hOldPen = SelectObject(hDC,CreatePen(PS_SOLID,1,RGB(255,255,255)));

					input_drawing_t drawing = render->queue->drawing;

					for(size_t i = 0;i < drawing.img_arr_len; i++)
					{
						input_img_t cur_img = drawing.img_arr[i];
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

						input_img_t img = cur_img;
					}

					DeleteObject(SelectObject(hDC,hOldPen));

					EndPaint(hWnd,&ps);
					input_drawing_queue_t * node_to_kill = render->queue;
					render->queue = render->queue->next;
					free(node_to_kill);
				}

				pthread_mutex_unlock(&render->mutex);
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

void*popup_wnd_routine(void*__arg)
{
	popup_wnd_arg_t* pu_arg = __arg;
	render_routine_arg_t arg = {.pu_arg = pu_arg, NULL, PTHREAD_MUTEX_INITIALIZER, false};
	pthread_t thr;
	pthread_create(&thr,NULL,render_routine,&arg);
	Sleep(400);
	HWND hWnd = CreateWindow(
			POPUPCLASS_NAME,
			"",
			WS_OVERLAPPEDWINDOW|WS_VISIBLE|WS_VSCROLL|WS_HSCROLL,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			NULL,
			NULL,
			__hIns,
			&arg
		);
	MSG Msg;
	while(GetMessage(&Msg,NULL,0,0))
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	pthread_mutex_lock(&arg.mutex);
	arg.stop = true;
	pthread_mutex_unlock(&arg.mutex);
	Sleep(200);
	pthread_mutex_destroy(&arg.mutex);

	input_drawing_queue_t * cur_node = arg.queue, * next_node;
	while(cur_node)
	{
		next_node = cur_node->next;
		free(cur_node);
		cur_node = next_node;
	}

	return NULL;
}

input_planet_sys_t * plt_sys_arr = NULL;
size_t plt_sys_arr_len = 0;

size_t __get_total_n_entities(input_planet_sys_child_t* plt_sys)
{
	size_t total = plt_sys->planets_arr_len+plt_sys->planet_sys_childs_arr_len;
	for(size_t i = 0; i < plt_sys->planet_sys_childs_arr_len; i++)
	{
		total += __get_total_n_entities(plt_sys->planet_sys_childs_arr+i);
	}
	return total;
}

size_t get_total_n_entities(input_planet_sys_t* plt_sys)
{
	size_t total = plt_sys->planets_arr_len+plt_sys->planet_sys_childs_arr_len;
	for(size_t i = 0; i < plt_sys->planet_sys_childs_arr_len; i++)
	{
		total += __get_total_n_entities(plt_sys->planet_sys_childs_arr+i);
	}
	return total;
}

MPFR_DECL_INIT(radius,MPFR_DEFAULT_RND);
MPFR_DECL_INIT(mass,MPFR_DEFAULT_RND);
MPFR_DECL_INIT(aphelion,MPFR_DEFAULT_RND);
MPFR_DECL_INIT(perihelion,MPFR_DEFAULT_RND);
MPFR_DECL_INIT(inclination_angle,MPFR_DEFAULT_RND);
MPFR_DECL_INIT(phase_angle,MPFR_DEFAULT_RND);

MPFR_DECL_INIT(x_pos,MPFR_DEFAULT_RND);
MPFR_DECL_INIT(y_pos,MPFR_DEFAULT_RND);

plt_sys_entity_t* __get_entities_arr(input_planet_sys_child_t* plt_sys, plt_sys_entity_t* arr)
{
	plt_sys_entity_t* plt_sys_ent = arr-1;
	plt_sys_coord_t * origin = &plt_sys_ent->coord;
	MPFR_DECL_INIT(big_mass,MPFR_DEFAULT_PRECISION);mpfr_set(big_mass,plt_sys_ent->body.mass,MPFR_DEFAULT_RND);
        for(size_t i = 0; i < plt_sys->planet_sys_childs_arr_len; i++)
        {
		input_planet_sys_child_t* cur_sys = plt_sys->planet_sys_childs_arr+i;

		input_quantity_get_num(&cur_sys->star.radius,radius);
		input_quantity_get_num(&cur_sys->star.mass,mass);
		input_quantity_get_num(&cur_sys->aphelion,aphelion);
		input_quantity_get_num(&cur_sys->perihelion,perihelion);
		input_quantity_get_num(&cur_sys->inclination_angle,inclination_angle);
		input_quantity_get_num(&cur_sys->phase_angle,phase_angle);

		plt_sys_entity_init(
					arr,
					radius,mass,
					ENTDIR_PLUS,aphelion,perihelion,big_mass,update_coord_def,
					inclination_angle,phase_angle,origin
				);
		arr++;

		arr = __get_entities_arr(plt_sys->planet_sys_childs_arr+i,arr);
        }
	for(size_t i = 0; i < plt_sys->planets_arr_len; i++)
        {
		input_planet_t* cur_plt = plt_sys->planets_arr+i;

		input_quantity_get_num(&cur_plt->body.radius,radius);
		input_quantity_get_num(&cur_plt->body.mass,mass);
		input_quantity_get_num(&cur_plt->aphelion,aphelion);
		input_quantity_get_num(&cur_plt->perihelion,perihelion);
		input_quantity_get_num(&cur_plt->inclination_angle,inclination_angle);
		input_quantity_get_num(&cur_plt->phase_angle,phase_angle);

		plt_sys_entity_init(
					arr,
					radius,mass,
					ENTDIR_PLUS,aphelion,perihelion,big_mass,update_coord_def,
					inclination_angle,phase_angle,origin
				);
                arr++;
        }
	return arr;
}

plt_sys_entity_t* get_entities_arr(input_planet_sys_t* plt_sys, plt_sys_entity_t* arr)
{
	plt_sys_entity_t* arr_original = arr;
	plt_sys_entity_t* plt_sys_ent = arr-1;
	plt_sys_coord_t * origin = &plt_sys_ent->coord;
	MPFR_DECL_INIT(big_mass,MPFR_DEFAULT_PRECISION);mpfr_set(big_mass,plt_sys_ent->body.mass,MPFR_DEFAULT_RND);

        for(size_t i = 0; i < plt_sys->planet_sys_childs_arr_len; i++)
        {
		input_planet_sys_child_t* cur_sys = plt_sys->planet_sys_childs_arr+i;

		input_quantity_get_num(&cur_sys->star.radius,radius);
		input_quantity_get_num(&cur_sys->star.mass,mass);
		input_quantity_get_num(&cur_sys->aphelion,aphelion);
		input_quantity_get_num(&cur_sys->perihelion,perihelion);
		input_quantity_get_num(&cur_sys->inclination_angle,inclination_angle);
		input_quantity_get_num(&cur_sys->phase_angle,phase_angle);

		plt_sys_entity_init(
					arr,
					radius,mass,
					ENTDIR_PLUS,aphelion,perihelion,big_mass,update_coord_def,
					inclination_angle,phase_angle,origin
				);
		arr++;

		arr = __get_entities_arr(plt_sys->planet_sys_childs_arr+i,arr);
        }

	for(size_t i = 0; i < plt_sys->planets_arr_len; i++)
        {
		input_planet_t* cur_plt = plt_sys->planets_arr+i;

		input_quantity_get_num(&cur_plt->body.radius,radius);
		input_quantity_get_num(&cur_plt->body.mass,mass);
		input_quantity_get_num(&cur_plt->aphelion,aphelion);
		input_quantity_get_num(&cur_plt->perihelion,perihelion);
		input_quantity_get_num(&cur_plt->inclination_angle,inclination_angle);
		input_quantity_get_num(&cur_plt->phase_angle,phase_angle);

		plt_sys_entity_init(
					arr,
					radius,mass,
					ENTDIR_PLUS,aphelion,perihelion,big_mass,update_coord_def,
					inclination_angle,phase_angle,origin
				);
                arr++;
        }
	printf("diff: %zu\n",(size_t)((intptr_t)arr-(intptr_t)arr_original)/sizeof(plt_sys_entity_t));
	return arr;
}

void __handle_command_msg(HWND hWnd, input_planet_sys_child_t * plt_sys_child)
{
        input_planet_sys_child_t * cur_plt_sys = plt_sys_child;
	if(hWnd == cur_plt_sys->aphelion.hEdit_num || hWnd == cur_plt_sys->aphelion.hEdit_exp)
	{
		input_quantity_check_val(&cur_plt_sys->aphelion);
		return;
	}
	else if(hWnd == cur_plt_sys->perihelion.hEdit_num || hWnd == cur_plt_sys->perihelion.hEdit_exp)
	{
		input_quantity_check_val(&cur_plt_sys->perihelion);
		return;
	}
	else if(hWnd == cur_plt_sys->inclination_angle.hEdit_num || hWnd == cur_plt_sys->inclination_angle.hEdit_exp)
	{
		input_quantity_check_val(&cur_plt_sys->inclination_angle);
		return;
	}
	else if(hWnd == cur_plt_sys->phase_angle.hEdit_num || hWnd == cur_plt_sys->phase_angle.hEdit_exp)
	{
		input_quantity_check_val(&cur_plt_sys->phase_angle);
		return;
	}
	else if(hWnd == cur_plt_sys->star.mass.hEdit_num || hWnd == cur_plt_sys->star.mass.hEdit_exp)
	{
		input_quantity_check_val(&cur_plt_sys->star.mass);
		return;
	}
	else if(hWnd == cur_plt_sys->star.radius.hEdit_num || hWnd == cur_plt_sys->star.radius.hEdit_exp)
	{
		input_quantity_check_val(&cur_plt_sys->star.radius);
		return;
	}
        else if(hWnd == cur_plt_sys->hButton_new_sys)
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
			else if(hWnd == cur_plt.aphelion.hEdit_num || hWnd == cur_plt.aphelion.hEdit_exp)
			{
				input_quantity_check_val(&cur_plt.aphelion);
				return;
			}
			else if(hWnd == cur_plt.perihelion.hEdit_num || hWnd == cur_plt.perihelion.hEdit_exp)
			{
				input_quantity_check_val(&cur_plt.perihelion);
				return;
			}
			else if(hWnd == cur_plt.inclination_angle.hEdit_num || hWnd == cur_plt.inclination_angle.hEdit_exp)
			{
				input_quantity_check_val(&cur_plt.inclination_angle);
				return;
			}
			else if(hWnd == cur_plt.phase_angle.hEdit_num || hWnd == cur_plt.phase_angle.hEdit_exp)
			{
				input_quantity_check_val(&cur_plt.phase_angle);
				return;
			}
			else if(hWnd == cur_plt.body.mass.hEdit_num || hWnd == cur_plt.body.mass.hEdit_exp)
			{
				input_quantity_check_val(&cur_plt.body.mass);
				return;
			}
			else if(hWnd == cur_plt.body.radius.hEdit_num || hWnd == cur_plt.body.radius.hEdit_exp)
			{
				input_quantity_check_val(&cur_plt.body.radius);
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
		if(hWnd == cur_plt_sys->x_pos.hEdit_num || hWnd == cur_plt_sys->x_pos.hEdit_exp)
		{
			input_quantity_check_val(&cur_plt_sys->x_pos);
			return;
		}
		else if(hWnd == cur_plt_sys->y_pos.hEdit_num || hWnd == cur_plt_sys->y_pos.hEdit_exp)
		{
			input_quantity_check_val(&cur_plt_sys->y_pos);
			return;
		}
		else if(hWnd == cur_plt_sys->star.mass.hEdit_num || hWnd == cur_plt_sys->star.mass.hEdit_exp)
		{
			input_quantity_check_val(&cur_plt_sys->star.mass);
			return;
		}
		else if(hWnd == cur_plt_sys->star.radius.hEdit_num || hWnd == cur_plt_sys->star.radius.hEdit_exp)
		{
			input_quantity_check_val(&cur_plt_sys->star.radius);
			return;
		}
		else if(hWnd == cur_plt_sys->hButton_new_sys)
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
					input_planet_sys_remove_planet(cur_plt_sys,i);
					return;
				}
				else if(hWnd == cur_plt.aphelion.hEdit_num || hWnd == cur_plt.aphelion.hEdit_exp)
				{
					input_quantity_check_val(&cur_plt.aphelion);
					return;
				}
				else if(hWnd == cur_plt.perihelion.hEdit_num || hWnd == cur_plt.perihelion.hEdit_exp)
				{
					input_quantity_check_val(&cur_plt.perihelion);
					return;
				}
				else if(hWnd == cur_plt.inclination_angle.hEdit_num || hWnd == cur_plt.inclination_angle.hEdit_exp)
				{
					input_quantity_check_val(&cur_plt.inclination_angle);
					return;
				}
				else if(hWnd == cur_plt.phase_angle.hEdit_num || hWnd == cur_plt.phase_angle.hEdit_exp)
				{
					input_quantity_check_val(&cur_plt.phase_angle);
					return;
				}
				else if(hWnd == cur_plt.body.mass.hEdit_num || hWnd == cur_plt.body.mass.hEdit_exp)
				{
					input_quantity_check_val(&cur_plt.body.mass);
					return;
				}
				else if(hWnd == cur_plt.body.radius.hEdit_num || hWnd == cur_plt.body.radius.hEdit_exp)
				{
					input_quantity_check_val(&cur_plt.body.radius);
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
				else if(hWnd == plt_sys_child_arr[i].aphelion.hEdit_num || hWnd == plt_sys_child_arr[i].aphelion.hEdit_exp)
				{
					input_quantity_check_val(&plt_sys_child_arr[i].aphelion);
					return;
				}
				else if(hWnd == plt_sys_child_arr[i].perihelion.hEdit_num || hWnd == plt_sys_child_arr[i].perihelion.hEdit_exp)
				{
					input_quantity_check_val(&plt_sys_child_arr[i].perihelion);
					return;
				}
				else if(hWnd == plt_sys_child_arr[i].inclination_angle.hEdit_num || hWnd == plt_sys_child_arr[i].inclination_angle.hEdit_exp)
				{
					input_quantity_check_val(&plt_sys_child_arr[i].inclination_angle);
					return;
				}
				else if(hWnd == plt_sys_child_arr[i].phase_angle.hEdit_num || hWnd == plt_sys_child_arr[i].phase_angle.hEdit_exp)
				{
					input_quantity_check_val(&plt_sys_child_arr[i].phase_angle);
					return;
				}
				else if(hWnd == plt_sys_child_arr[i].star.mass.hEdit_num || hWnd == plt_sys_child_arr[i].star.mass.hEdit_exp)
				{
					input_quantity_check_val(&plt_sys_child_arr[i].star.mass);
					return;
				}
				else if(hWnd == plt_sys_child_arr[i].star.radius.hEdit_num || hWnd == plt_sys_child_arr[i].star.radius.hEdit_exp)
				{
					input_quantity_check_val(&plt_sys_child_arr[i].star.radius);
					return;
				}
				else
				{
					__handle_command_msg(hWnd,plt_sys_child_arr+i);
				}
			}
		}
	}
}

#define MENU_ADD_ID (10)
#define MENU_START_ID (MENU_ADD_ID+1)

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	static HINSTANCE hIns;
	static input_quantity_t RealTime_To_ScreenTime, RealDis_To_ScreenPix;
	static size_t wnd_height, max_scroll;
	static BOOL scroll_shown = TRUE;
	switch(Msg)
	{
		case WM_CREATE:

			hIns = ((CREATESTRUCT*)lParam)->hInstance;
			input_set_mainwnd(hWnd);
			input_set_ins(hIns);

			{
				HMENU hMainMenu = CreateMenu();
				AppendMenu(hMainMenu,MF_STRING,MENU_ADD_ID,"Registrar sistema");
				AppendMenu(hMainMenu,MF_STRING,MENU_START_ID,"Empezar simulacion");
				SetMenu(hWnd,hMainMenu);

				SCROLLINFO sif = {.cbSize = sizeof(SCROLLINFO), .fMask = SIF_PAGE, .nPage = LINE_HEIGHT};
				SetScrollInfo(hWnd,SB_VERT,&sif,FALSE);
			}

			RealDis_To_ScreenPix = input_quantity_create("Cada pixel equivale a",meter_units,0,0);
			RealTime_To_ScreenTime = input_quantity_create("Cada 100 ms pasan",long_time_units,input_get_n_lines(),0);

			input_add_line(input_get_n_lines(),SEPARATOR_LINE);

			break;
		case WM_COMMAND:
			//Handle buttons
			{
				SCROLLINFO sif = {.cbSize = sizeof(SCROLLINFO), .fMask = SIF_POS};
                                GetScrollInfo(hWnd,SB_VERT,&sif);
				switch(LOWORD(wParam))
				{
					case MENU_ADD_ID:
						{
							char buff[100]; sprintf(buff,"Sistema planetario numero %zu",plt_sys_arr_len);
							input_planet_sys_t new_plt_sys = input_planet_sys_create(
								buff,
								input_get_n_lines(),
								1);
							plt_sys_arr_len++;
							plt_sys_arr = realloc(plt_sys_arr,plt_sys_arr_len*sizeof(input_planet_sys_t));
							plt_sys_arr[plt_sys_arr_len-1] = new_plt_sys;
						}
						break;
					case MENU_START_ID:
						{

							size_t total = plt_sys_arr_len;
							for(size_t i = 0; i < plt_sys_arr_len; i++)
							{
								total += get_total_n_entities(plt_sys_arr+i);
							}
							if(total == 0)
							{
								MessageBox(hWnd,"No se ha ingresado algo.","Error",MB_ICONERROR|MB_OK|MB_APPLMODAL);
								break;
							}
							plt_sys_entity_t * ent_arr, * arr;
							size_t ent_arr_len = total;

							arr = ent_arr = malloc(total*sizeof(plt_sys_entity_t));

							popup_wnd_arg_t __arg = {.arr = ent_arr, .len = total}, * arg = malloc(sizeof(popup_wnd_arg_t));
							*arg = __arg;
							mpfr_init2(arg->esc_dis,MPFR_DEFAULT_PRECISION);
							mpfr_init2(arg->esc_time,MPFR_DEFAULT_PRECISION);
							input_quantity_get_num(&RealDis_To_ScreenPix,arg->esc_dis);
							input_quantity_get_num(&RealTime_To_ScreenTime,arg->esc_time);

							quantity_errno = true;
                                                        for(size_t i = 0; i < plt_sys_arr_len; i++)
                                                        {
						                input_quantity_get_num(&plt_sys_arr[i].star.radius,radius);
	        					        input_quantity_get_num(&plt_sys_arr[i].star.mass,mass);

						                input_quantity_get_num(&plt_sys_arr[i].x_pos,x_pos);
	        					        input_quantity_get_num(&plt_sys_arr[i].y_pos,y_pos);

								plt_sys_entity_nomove_init(arr,radius,mass,x_pos,y_pos);
								arr++;
								arr = get_entities_arr(plt_sys_arr+i,arr);

								if(plt_sys_arr[i].planets_arr_len == 0 && plt_sys_arr[i].planet_sys_childs_arr_len == 0
									|| !quantity_errno
								)
								{
									MessageBox(hWnd,
										!quantity_errno ? "Informacion invalida" : "Sistema planetario sin planetas",
										"Error",MB_ICONERROR|MB_OK|MB_APPLMODAL);
									size_t j = i;
									for(size_t i = 0; i < j; i++)
									{
										plt_sys_entity_destroy(ent_arr+i);
									}
									free(ent_arr);
									ent_arr = NULL;
									mpfr_clears(arg->esc_dis,arg->esc_time,(mpfr_ptr)0);
									break;
								}
                                                        }
							if(ent_arr == NULL)
							{
								break;
							}

							pthread_t thr;
							pthread_create(&thr,NULL,popup_wnd_routine,arg);
						}
						break;
					default:
						quantity_errno = true;

						if((HWND)lParam == RealDis_To_ScreenPix.hEdit_num || (HWND)lParam == RealDis_To_ScreenPix.hEdit_exp)
						{
							input_quantity_check_val(&RealDis_To_ScreenPix);
						}
						else if((HWND)lParam == RealTime_To_ScreenTime.hEdit_num || (HWND)lParam == RealTime_To_ScreenTime.hEdit_exp)
						{
							input_quantity_check_val(&RealTime_To_ScreenTime);
						}
						else
						{
							handle_command_msg(lParam);
						}
						break;
				}
			}
		case WM_SIZE:
				if(Msg == WM_SIZE) {wnd_height = HIWORD(lParam);}

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
							.nMax = (input_get_n_lines())*LINE_HEIGHT-(wnd_height/LINE_HEIGHT)*LINE_HEIGHT
					};
					max_scroll = scinf.nMax;
					if(scinf.nPos < 0)
					{
						scinf.nPos = 0;
					}
					else if(scinf.nPos > max_scroll)
					{
						scinf.nPos = max_scroll;
					}
					SetScrollInfo(hWnd,SB_VERT,&scinf,TRUE);
				}
				else if(scroll_shown == TRUE)
				{
				        SCROLLINFO scinf = {.cbSize = sizeof(SCROLLINFO), .fMask = SIF_POS};
				        GetScrollInfo(hWnd,SB_VERT,&scinf);
				        ScrollWindow(hWnd,0,scinf.nPos,NULL,NULL);

					scroll_shown = FALSE;
					ShowScrollBar(hWnd,SB_VERT,FALSE);
				}
			break;
		case WM_VSCROLL:
			{
				size_t prev_nPos;
				static SCROLLINFO sif = {.cbSize = sizeof(SCROLLINFO), .fMask = SIF_POS, .nPos = 0};
				prev_nPos = sif.nPos;
				switch(LOWORD(wParam))
				{
					case SB_THUMBPOSITION:
					case SB_THUMBTRACK:
						sif.nPos = HIWORD(wParam);
						break;
					case SB_LINEUP:
						sif.nPos-=LINE_HEIGHT;
						break;
					case SB_LINEDOWN:
						sif.nPos+=LINE_HEIGHT;
						break;
					case SB_PAGEUP:
						sif.nPos-=LINE_HEIGHT;
						break;
					case SB_PAGEDOWN:
						sif.nPos+=LINE_HEIGHT;
						break;
				}
				if(sif.nPos < 0)
				{
					sif.nPos = 0;
				}
				else if(sif.nPos > max_scroll)
				{
					sif.nPos = max_scroll;
				}
				else if(prev_nPos != sif.nPos)
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

	//pop up
	WndClass.lpfnWndProc = PopUpWndProc;
	WndClass.style = CS_HREDRAW|CS_VREDRAW;
	WndClass.hInstance = hCurInstance;
	WndClass.hIcon = LoadIcon(NULL,IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL,IDC_ARROW);
	WndClass.hbrBackground = CreateSolidBrush(RGB(0,0,0));
	WndClass.lpszClassName = POPUPCLASS_NAME;
	RegisterClass(&WndClass);

	//main wnd
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
