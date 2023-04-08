#pragma once

#include "input.h"
#include "input_line.h"
#include "input_body.h"

input_planet_t input_planet_create(const char* defname, size_t line, size_t n_tab);

void input_planet_destroy(input_planet_t* input);
