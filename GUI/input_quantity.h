#pragma once

#include "types.h"
#include "input.h"
#include "input_line.h"
#include <stdbool.h>
#include <math.h>
#include <stdio.h>

#define INPUT_QUANTITY_EMPTY_ERROR "No se ha ingresado nada"
#define INPUT_QUANTITY_EMPTY_EXP_ERROR "No se ha ingresado el exponente"
#define INPUT_QUANTITY_EMPTY_NUM_ERROR "No se ha ingresado el numero"

#define INPUT_QUANTITY_NOBOTH_ERROR "Exponente y numero no validos"
#define INPUT_QUANTITY_NONUM_ERROR "Numero no valido"
#define INPUT_QUANTITY_NOEXP_ERROR "Exponente no valido"

#define INPUT_QUANTITY_NOERROR ""

#define INPUT_QUANTITY_EXP_WIDTH 50
#define INPUT_QUANTITY_NUM_WIDTH 100
#define INPUT_QUANTITY_EXPLABEL_WIDTH 35
#define INPUT_QUANTITY_CB_WIDTH 50

//line (0-n_lines)
//n_tab (0-n_tabs)
input_quantity_t input_quantity_create(const char* name, def_unit_t* unit_arr, size_t line, size_t n_tab);

bool input_quantity_check_val(input_quantity_t*input);

bool input_quantity_get_num(input_quantity_t* input, num_t* number);

void input_quantity_destroy(input_quantity_t* input);
