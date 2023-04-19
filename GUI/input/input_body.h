#pragma once

#include "types.h"
#include "input.h"
#include "input_quantity.h"

#define MASS_INPUT_NAME "Masa"
#define RADIUS_INPUT_NAME "Radio del cuerpo"

input_body_t input_body_create(const char* name, size_t line, size_t n_tab);

void input_body_destroy(input_body_t * input);
