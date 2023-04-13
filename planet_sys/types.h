#pragma once

#include <mpfr.h>

#define DEFAULT_PREC 200

typedef struct __coord_t
{
	mpfr_t X, Y;
	mpfr_t cur_angle;

	mpfr_t inclination_angle;

	struct __coord_t * origin;
}plt_sys_coord_t;

#define ORIGIN_COORD NULL


typedef enum entity_direction {
	ENTDIR_PLUS/*sentido antihorario*/,
	ENTDIR_NEG/*sentido horario*/,
	ENTDIR_NOMOVE
}entity_direction_t;

typedef struct
{
	mpfr_t radius;
	mpfr_t mass;
}plt_sys_body_attr_t;

typedef struct
{
	entity_direction_t direction;
	mpfr_t aphelion, perihelion;
	void (*update_coord)(void*, mpfr_t stime);
}plt_sys_trajectory_t;

typedef struct
{
	plt_sys_body_attr_t body;
	plt_sys_trajectory_t trajectory;
	plt_sys_coord_t coord;
}plt_sys_entity_t;
