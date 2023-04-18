#pragma once

#include <stdio.h>

#include "types.h"

#include "model_kepler.h"

void update_coord_def(void* _ent, mpfr_t time);
void update_coord_nomove(void* _ent, mpfr_t time);
void update_coord_exp(void* _ent, mpfr_t time);

