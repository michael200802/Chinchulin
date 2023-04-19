#pragma once

#include "plt_sys_entity.h"

#include <stdlib.h>

void plt_sys_create(plt_sys_t * plt_sys, size_t len);

plt_sys_entity_t * plt_sys_get_ent(plt_sys_t * plt_sys, size_t index);

void plt_sys_update(plt_sys_t * plt_sys, mpfr_t time);

void plt_sys_destroy(plt_sys_t * plt_sys);

