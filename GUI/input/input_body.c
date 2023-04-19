#include "input_body.h"

input_body_t input_body_create(const char* name, size_t line, size_t n_tab)
{
	const HINSTANCE hIns = __hIns;
        const HWND hMainWnd = __hMainWnd;

	HWND hStatic = CreateWindow(
                                WC_STATIC,
                                name,
                                WS_VISIBLE|WS_CHILD|WS_BORDER|SS_LEFT,
                                ntab_to_screen(n_tab),
                                line_to_screen(line),
                                input_get_str_width(name),
                                LINE_HEIGHT,
                                hMainWnd,
                                NULL,
                                hIns,
                                NULL
                        );
	input_add_line(line,input_line_create(1,hStatic));

	return (input_body_t){
			hStatic,
			input_quantity_create(MASS_INPUT_NAME,gr_units,line+1,n_tab),
			input_quantity_create(RADIUS_INPUT_NAME,meter_units,line+1+INPUT_QUANTITY_N_LINES,n_tab)
	};
}

void input_body_destroy(input_body_t * input)
{
	size_t pos = input_get_wnd_line(input->hStatic_name);
        input_delete_line(pos);
	input_quantity_destroy(&input->mass);
	input_quantity_destroy(&input->radius);
}

