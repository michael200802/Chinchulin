#include "input_planet_sys_child.h"

input_planet_sys_child_t input_planet_sys_child_create(const char* defname, size_t line, size_t n_tab)
{
        input_planet_sys_child_t input = {};

	const HINSTANCE hIns = __hIns;
        const HWND hMainWnd = __hMainWnd;

        SCROLLINFO scinf = {.cbSize = sizeof(SCROLLINFO), .fMask = SIF_POS};
        GetScrollInfo(hMainWnd,SB_VERT,&scinf);
        ScrollWindow(hMainWnd,0,scinf.nPos,NULL,NULL);

	input_obj_set_defctrls(input,defname,line,n_tab);//+3 lines
	line+=3;
	n_tab+=2;

        input.aphelion = input_quantity_create("Afelio",meter_units,line,n_tab);
	line+=INPUT_QUANTITY_N_LINES;

        input.perihelion = input_quantity_create("Perihelio",meter_units,line,n_tab);
	line+=INPUT_QUANTITY_N_LINES;

        input.inclination_angle = input_quantity_create("Angulo de inclinacion",angle_units,line,n_tab);
        line+=INPUT_QUANTITY_N_LINES;

        input.phase_angle = input_quantity_create("Angulo de fase",angle_units,line,n_tab);
        line+=INPUT_QUANTITY_N_LINES;

	input.star = input_body_create("Radio y masa de la estrella",line,n_tab);
	line+=INPUT_BODY_N_LINES;

	input.hButton_new_planet = CreateWindow(
		WC_BUTTON,
		"Registrar planeta",
		WS_VISIBLE|WS_CHILD|WS_BORDER|BS_PUSHBUTTON,
		ntab_to_screen(n_tab),
                line_to_screen(line),
		input_get_str_width("Registrar planeta"),
		LINE_HEIGHT,
		hMainWnd,
		NULL,
		hIns,
		NULL
		);
	input.hButton_new_sys = CreateWindow(
		WC_BUTTON,
		"Registrar sistema",
		WS_VISIBLE|WS_CHILD|WS_BORDER|BS_PUSHBUTTON,
		ntab_to_screen(n_tab)+input_get_str_width("Registrar planeta"),
                line_to_screen(line),
		input_get_str_width("Registrar sistema"),
		LINE_HEIGHT,
		hMainWnd,
		NULL,
		hIns,
		NULL
		);
	input_add_line(line,input_line_create(2,input.hButton_new_planet,input.hButton_new_sys));
	line++;

	input_add_line(line,SEPARATOR_LINE);

	input_repaint();

        ScrollWindow(hMainWnd,0,-1*scinf.nPos,NULL,NULL);

        return input;
}

void input_planet_sys_child_add_planet(input_planet_sys_child_t * input)
{
	size_t pos = input_get_wnd_line(input->hEdit_name);
	size_t start_line = pos+INPUT_PLANET_SYS_CHILD_MIN_N_LINES;

	input->planets_arr_len++;
	input->planets_arr = realloc(input->planets_arr,input->planets_arr_len*sizeof(input_planet_t));

	char buffer[100];
	sprintf(buffer,"Planeta %zu",input->planets_arr_len-1);

	input->planets_arr[input->planets_arr_len-1] = input_planet_create(buffer,start_line,input_get_wnd_tab(input->hButton_new_planet)+1);
}

void input_planet_sys_child_add_sys(input_planet_sys_child_t * input)
{
        size_t pos = input_get_wnd_line(input->hEdit_name);
        size_t start_line = pos+INPUT_PLANET_SYS_CHILD_MIN_N_LINES;

        input->planet_sys_childs_arr_len++;
        input->planet_sys_childs_arr = realloc(input->planet_sys_childs_arr,input->planet_sys_childs_arr_len*sizeof(input_planet_sys_child_t));

        char buffer[100];
        sprintf(buffer,"Sistema planetario %zu",input->planet_sys_childs_arr_len-1);

        input->planet_sys_childs_arr[input->planet_sys_childs_arr_len-1] = input_planet_sys_child_create(buffer,start_line,input_get_wnd_tab(input->hButton_new_planet)+1);
}

//after the user clicked hButton_destroy
void input_planet_sys_child_remove_planet(input_planet_sys_child_t * input, size_t i)
{
	input_planet_destroy(&input->planets_arr[i]);
	for(; i < input->planets_arr_len-1; i++)
	{
		input->planets_arr[i] = input->planets_arr[i+1];
	}
	input->planets_arr_len--;
	input->planets_arr = realloc(input->planets_arr,input->planets_arr_len*sizeof(input_planet_t));
}

void input_planet_sys_child_remove_sys(input_planet_sys_child_t * input, size_t i)
{
	input_planet_sys_child_destroy(&input->planet_sys_childs_arr[i]);
	for(; i < input->planet_sys_childs_arr_len-1; i++)
	{
		input->planet_sys_childs_arr[i] = input->planet_sys_childs_arr[i+1];
	}
	input->planet_sys_childs_arr_len--;
	input->planet_sys_childs_arr = realloc(input->planet_sys_childs_arr,input->planet_sys_childs_arr_len*sizeof(input_planet_sys_child_t));
}

void input_planet_sys_child_destroy(input_planet_sys_child_t * input)
{
        SCROLLINFO scinf = {.cbSize = sizeof(SCROLLINFO), .fMask = SIF_POS};
        GetScrollInfo(__hMainWnd,SB_VERT,&scinf);
        ScrollWindow(__hMainWnd,0,scinf.nPos,NULL,NULL);

	size_t n_lines = INPUT_PLANET_SYS_CHILD_MIN_N_LINES;
	input_quantity_destroy(&input->aphelion);n_lines-=INPUT_QUANTITY_N_LINES;
	input_quantity_destroy(&input->perihelion);n_lines-=INPUT_QUANTITY_N_LINES;
        input_quantity_destroy(&input->inclination_angle);n_lines-=INPUT_QUANTITY_N_LINES;
        input_quantity_destroy(&input->phase_angle);n_lines-=INPUT_QUANTITY_N_LINES;
	input_body_destroy(&input->star);n_lines-=INPUT_BODY_N_LINES;
	for(size_t i = 0; i < input->planets_arr_len; i++)
	{
		input_planet_destroy(input->planets_arr+i);
	}
	for(size_t i = 0; i < input->planet_sys_childs_arr_len; i++)
	{
		input_planet_sys_child_destroy(input->planet_sys_childs_arr+i);
	}
	size_t pos = input_get_wnd_line(input->hEdit_name);
	for(size_t i = 0; i < n_lines; i++)
	{
		input_delete_line(pos);
	}

	input_repaint();

        ScrollWindow(__hMainWnd,0,-1*scinf.nPos,NULL,NULL);
}


