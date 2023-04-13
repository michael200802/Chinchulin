#include "GUI/input_planet_sys.h"

#include "planet_sys/plt_sys_entity.h"

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

MPFR_DECL_INIT(radius,DEFAULT_PREC);
MPFR_DECL_INIT(mass,DEFAULT_PREC);
MPFR_DECL_INIT(aphelion,DEFAULT_PREC);
MPFR_DECL_INIT(perihelion,DEFAULT_PREC);
MPFR_DECL_INIT(inclination_angle,DEFAULT_PREC);
MPFR_DECL_INIT(phase_angle,DEFAULT_PREC);

MPFR_DECL_INIT(x_pos,DEFAULT_PREC);
MPFR_DECL_INIT(y_pos,DEFAULT_PREC);

plt_sys_entity_t* __get_entities_arr(input_planet_sys_child_t* plt_sys, plt_sys_entity_t* arr)
{
	plt_sys_entity_t* plt_sys_ent = arr-1;
	plt_sys_coord_t * origin = &plt_sys_ent->coord;
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
					ENTDIR_PLUS,aphelion,perihelion,update_coord_def,
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
					ENTDIR_PLUS,aphelion,perihelion,update_coord_def,
					inclination_angle,phase_angle,origin
				);
                arr++;
        }
	return arr;
}

plt_sys_entity_t* get_entities_arr(input_planet_sys_t* plt_sys, plt_sys_entity_t* arr)
{
	plt_sys_entity_t* arr_original = arr;
	plt_sys_coord_t * origin = NULL;
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
					ENTDIR_PLUS,aphelion,perihelion,update_coord_def,
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
					ENTDIR_PLUS,aphelion,perihelion,update_coord_def,
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
		else if(hWnd == cur_plt_sys->hButton_new_sys)
		{
			puts("adding sys");
			input_planet_sys_add_sys(cur_plt_sys);
			puts("sys added");
			return;
		}
		else if(hWnd == cur_plt_sys->hButton_new_planet)
		{
			puts("adding plt");
			input_planet_sys_add_planet(cur_plt_sys);
			puts("plt added");
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
				if(sif.nPos != 0) {ScrollWindow(hWnd,0,sif.nPos,NULL,NULL);}
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
							plt_sys_entity_t * ent_arr, * arr;
							size_t ent_arr_len = total;

							arr = ent_arr = malloc(total*sizeof(plt_sys_entity_t));

                                                        for(size_t i = 0; i < plt_sys_arr_len; i++)
                                                        {
						                input_quantity_get_num(&plt_sys_arr[i].star.radius,radius);
	        					        input_quantity_get_num(&plt_sys_arr[i].star.mass,mass);

						                input_quantity_get_num(&plt_sys_arr[i].x_pos,x_pos);
	        					        input_quantity_get_num(&plt_sys_arr[i].y_pos,y_pos);

								plt_sys_entity_nomove_init(arr,radius,mass,x_pos,y_pos);
								arr++;
								arr = get_entities_arr(plt_sys_arr+i,arr);
                                                        }
						}
						break;
					default:
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
				if(sif.nPos != 0) {ScrollWindow(hWnd,0,-1*sif.nPos,NULL,NULL);}

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
					SetScrollInfo(hWnd,SB_VERT,&scinf,TRUE);
				}
				else if(scroll_shown == TRUE)
				{
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
