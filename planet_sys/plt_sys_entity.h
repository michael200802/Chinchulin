#pragma once

#include "types.h"

#include "update_coord.h"

void plt_sys_entity_init(
		plt_sys_entity_t * ent,
		mpfr_t radius, mpfr_t mass,//plt_sys_body_attr_t
		entity_direction_t direction, mpfr_t aphelion, mpfr_t perihelion, void (*update_coord)(void*,mpfr_t),//plt_sys_trajectory_t
		mpfr_t inclination_angle, mpfr_t phase_angle, plt_sys_coord_t * origin//plt_sys_coord_t
	);
void plt_sys_entity_nomove_init(
                plt_sys_entity_t * ent,
                mpfr_t radius, mpfr_t mass,//plt_sys_body_attr_t
		mpfr_t x_pos, mpfr_t y_pos
        );
void plt_sys_entity_destroy(
		plt_sys_entity_t * ent
	);
