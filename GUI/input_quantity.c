#include "input_quantity.h"

input_quantity_t input_quantity_create(const char* name, def_unit_t* unit_arr, size_t line, size_t n_tab)
{
	const HINSTANCE hIns = __hIns;
	const HWND hMainWnd = __hMainWnd;
	input_quantity_t input;

	input_add_line(line,SEPARATOR_LINE);

	line++;
	input.hStatic_name = CreateWindow(
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
	input_add_line(line,input_line_create(1,input.hStatic_name));

	line++;
        input.hEdit_num = CreateWindow(
			WC_EDIT,
			"0",
			WS_VISIBLE|WS_CHILD|WS_BORDER|ES_CENTER|ES_AUTOHSCROLL,
			ntab_to_screen(n_tab),
			line_to_screen(line),
			INPUT_QUANTITY_NUM_WIDTH,
			LINE_HEIGHT,
			hMainWnd,
			NULL,
			hIns,
			NULL
		);
        input.hStatic_tenlabel = CreateWindow(
			WC_STATIC,
			"*10^",
			WS_VISIBLE|WS_CHILD|WS_BORDER|SS_CENTER,
			ntab_to_screen(n_tab) + INPUT_QUANTITY_NUM_WIDTH,
			line_to_screen(line),
			INPUT_QUANTITY_EXPLABEL_WIDTH,
			LINE_HEIGHT,
			hMainWnd,
			NULL,
			hIns,
			NULL
		);
        input.hEdit_exp = CreateWindow(
			WC_EDIT,
			"0",
			WS_VISIBLE|WS_CHILD|WS_BORDER|ES_LEFT,
			ntab_to_screen(n_tab) + INPUT_QUANTITY_NUM_WIDTH + INPUT_QUANTITY_EXPLABEL_WIDTH,
			line_to_screen(line),
			INPUT_QUANTITY_EXP_WIDTH,
			LINE_HEIGHT,
			hMainWnd,
			NULL,
			hIns,
			NULL
		);
        input.hCB_unit = CreateWindow(
			WC_COMBOBOX,
			"",
			WS_VISIBLE|WS_CHILD|WS_BORDER|CBS_HASSTRINGS|CBS_DROPDOWNLIST,
			ntab_to_screen(n_tab) + INPUT_QUANTITY_NUM_WIDTH + INPUT_QUANTITY_EXPLABEL_WIDTH + INPUT_QUANTITY_EXP_WIDTH,
			line_to_screen(line),
			INPUT_QUANTITY_CB_WIDTH,
			LINE_HEIGHT,
			hMainWnd,
			NULL,
			hIns,
			NULL
		);
	input_add_line(line,input_line_create(4,input.hCB_unit,input.hEdit_exp,input.hStatic_tenlabel,input.hEdit_num));

	line++;
	input.hStatic_Error = CreateWindow(
                        WC_STATIC,
                        INPUT_QUANTITY_EMPTY_ERROR,
                        WS_VISIBLE|WS_CHILD|WS_BORDER|SS_LEFT,
                        ntab_to_screen(n_tab),
                        line_to_screen(line),
                        300,
                        LINE_HEIGHT,
                        hMainWnd,
                        NULL,
                        hIns,
                        NULL
                );
	input_add_line(line,input_line_create(1,input.hStatic_Error));

	line++;
	input_add_line(line,SEPARATOR_LINE);

	def_unit_t unit;
	size_t i = 0;
	do
	{
		unit = unit_arr[i++];
		ComboBox_AddString(input.hCB_unit,unit.name);
	}while(unit_arr[i].name != NULL);
	ComboBox_SetCurSel(input.hCB_unit,0);
	input.units_arr = unit_arr;

	SetWindowPos(input.hCB_unit,NULL,0,0,INPUT_QUANTITY_CB_WIDTH,LINE_HEIGHT*i,SWP_NOMOVE);

	return input;
}

bool input_quantity_get_num(input_quantity_t* input, num_t* number)
{
	char buf[100];
	double num;
	double exp;

	Edit_GetText(input->hEdit_num,buf,100);
	int err = sscanf(buf,"%f",&num);
	if(err != 1)
	{
		return false;
	}

	Edit_GetText(input->hEdit_exp,buf,100);
	err = sscanf(buf,"%f",&exp);
	if(err != 1)
	{
		return false;
	}

	int index = ComboBox_GetCurSel(input->hCB_unit);

	*number = num*pow(10,exp)*input->units_arr[index].constant;
	return true;
}

void input_quantity_destroy(input_quantity_t* input)
{
	size_t pos = input_get_wnd_line(input->hStatic_name)-1;
	for(int i = 0; i < INPUT_QUANTITY_N_LINES; i++)
	{
		input_delete_line(pos);
	}
}
