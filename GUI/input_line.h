#pragma once

#include "types.h"
#include "input.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>

input_line_t input_line_create(size_t n_ctrls, ...);

void input_line_move(input_line_t line, ssize_t offset);

void input_line_destroy(input_line_t*line);
