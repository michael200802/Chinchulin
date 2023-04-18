#include "input_planet.h"

input_planet_t input_planet_create(const char* defname, size_t line, size_t n_tab)
{
	input_planet_t input;

	SCROLLINFO scinf = {.cbSize = sizeof(SCROLLINFO), .fMask = SIF_POS};
	GetScrollInfo(__hMainWnd,SB_VERT,&scinf);
	ScrollWindow(__hMainWnd,0,scinf.nPos,NULL,NULL);

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

	input.body = input_body_create("Radio y masa",line,n_tab);

	input_repaint();

	ScrollWindow(__hMainWnd,0,-1*scinf.nPos,NULL,NULL);

	return input;
}

void input_planet_destroy(input_planet_t* input)
{
	SCROLLINFO scinf = {.cbSize = sizeof(SCROLLINFO), .fMask = SIF_POS};
	GetScrollInfo(__hMainWnd,SB_VERT,&scinf);
	ScrollWindow(__hMainWnd,0,scinf.nPos,NULL,NULL);

	size_t n_lines = INPUT_PLANET_N_LINES;
	input_quantity_destroy(&input->aphelion);n_lines-=INPUT_QUANTITY_N_LINES;
	input_quantity_destroy(&input->perihelion);n_lines-=INPUT_QUANTITY_N_LINES;
	input_quantity_destroy(&input->inclination_angle);n_lines-=INPUT_QUANTITY_N_LINES;
	input_quantity_destroy(&input->phase_angle);n_lines-=INPUT_QUANTITY_N_LINES;
	input_body_destroy(&input->body);n_lines-=INPUT_BODY_N_LINES;
	size_t pos = input_get_wnd_line(input->hEdit_name);
	for(int i = 0; i < n_lines; i++)
	{
		input_delete_line(pos);
	}
	input_repaint();

	ScrollWindow(__hMainWnd,0,-1*scinf.nPos,NULL,NULL);
}
