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
			"",
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
			"",
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
                        350,
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

	SetWindowPos(input.hCB_unit,NULL,0,0,INPUT_QUANTITY_CB_WIDTH,LINE_HEIGHT*(i+1),SWP_NOMOVE);

	return input;
}

enum input_number_type {NUM_IS_INT, NUM_IS_FLOAT, NUM_INVALID};
static inline enum input_number_type input_quantity_is_num(char * str)
{
	char * str_original = str;
	while(*str != '\0' && (*str !='.' || *str != ','))
	{
		if(!isdigit(*str))
		{
			return NUM_INVALID;
		}
		str++;
	}

	//it's empty
	if(str == str_original)
	{
		return NUM_INVALID;
	}

	//it's not an int
	if(*str == '.' || *str == ',')
	{
		str++;
		enum input_number_type num_type = input_quantity_is_num(str);
		if(num_type != NUM_IS_INT)
		{
			return NUM_INVALID;
		}
		return NUM_IS_FLOAT;
	}

	return NUM_IS_INT;
}

bool input_quantity_check_val(input_quantity_t*input)
{
	bool num_valid = true, num_empty = false;
	bool exp_valid = true, exp_empty = false;

	size_t num_len = Edit_GetTextLength(input->hEdit_num)+1;
	if(num_len == 1)
	{
		num_empty = true;
	}
	else
	{
		char * num_str = malloc(num_len*sizeof(char));
		Edit_GetText(input->hEdit_num,num_str,num_len);
		num_valid = (input_quantity_is_num(num_str) != NUM_INVALID);
		free(num_str);
	}

	size_t exp_len = Edit_GetTextLength(input->hEdit_exp)+1;
	if(exp_len == 1)
	{
		exp_empty = true;
	}
	else
	{
		char * exp_str = malloc(exp_len*sizeof(char));
		Edit_GetText(input->hEdit_exp,exp_str,exp_len);
		exp_valid = (input_quantity_is_num(exp_str) != NUM_INVALID);
		free(exp_str);
	}


	if(num_empty && exp_empty)
	{
		Static_SetText(input->hStatic_Error,INPUT_QUANTITY_EMPTY_ERROR);
		return false;
	}
	else
	{
		if(!num_valid && !exp_valid)
		{
			Static_SetText(input->hStatic_Error,INPUT_QUANTITY_NOBOTH_ERROR);
			return false;
		}
		else
		{
			char buffer[100];
			size_t buffer_len = 0;
			printf("num%d exp%d\n",num_valid,exp_valid);
			if(!num_valid)
			{
				buffer_len+=sprintf(buffer+buffer_len,INPUT_QUANTITY_NONUM_ERROR", ");
			}
			else if(!exp_valid)
			{
				buffer_len+=sprintf(buffer+buffer_len,INPUT_QUANTITY_NOEXP_ERROR", ");
			}
			if(num_empty)
			{
				buffer_len+=sprintf(buffer+buffer_len,INPUT_QUANTITY_EMPTY_NUM_ERROR);
			}
			else if(exp_empty)
			{
				buffer_len+=sprintf(buffer+buffer_len,INPUT_QUANTITY_EMPTY_EXP_ERROR);
			}
			if(buffer_len != 0)
			{
				Static_SetText(input->hStatic_Error,buffer);
				return false;
			}
		}
	}

	Static_SetText(input->hStatic_Error,INPUT_QUANTITY_NOERROR);

	return true;
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
