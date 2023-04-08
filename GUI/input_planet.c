#include "input_planet.h"

input_planet_t input_planet_create(const char* defname, size_t line, size_t n_tab)
{
	input_planet_t input;
	input_obj_set_defctrls(input,defname,line,n_tab);//+3 lines
	input.aphelion = input_quantity_create("Afelio",meter_units,line+3,n_tab);
	input.perihelion = input_quantity_create("Perihelio",meter_units,line+3+INPUT_QUANTITY_N_LINES,n_tab);
	input.body = input_body_create("Radio y masa",line+3+2*INPUT_QUANTITY_N_LINES,n_tab);
	return input;
}

void input_planet_destroy(input_planet_t* input)
{
	input_quantity_destroy(&input->aphelion);
	input_quantity_destroy(&input->perihelion);
	input_body_destroy(&input->body);
	size_t pos = input_get_wnd_line(input->hEdit_name);
	for(int i = 0; i < (INPUT_PLANET_N_LINES-INPUT_BODY_N_LINES-2*INPUT_QUANTITY_N_LINES); i++)
	{
		input_delete_line(pos);
	}
}
