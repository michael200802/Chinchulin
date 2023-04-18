#pragma once

#include <mpfr.h>

#include "plt_sys_entity.h"

typedef struct
{
	mpfr_t a, b, e, c;
}model_kepler_ellipse_t;

void model_kepler_get_c_val(mpfr_t c_val, mpfr_t a_val, mpfr_t b_val);

void model_kepler_get_e_val(mpfr_t e_val, mpfr_t a_val, mpfr_t b_val);

void model_kepler_get_ellipse(model_kepler_ellipse_t * ell, mpfr_t aphelion, mpfr_t perihelion);

void model_kepler_get_r_val(mpfr_t r_val, model_kepler_ellipse_t * ell, mpfr_t angle);

void model_kepler_get_vel_val(mpfr_t vel_val, model_kepler_ellipse_t * ell, mpfr_t angle, mpfr_t big_mass);

void model_kepler_get_angle_vel(mpfr_t angle_vel, model_kepler_ellipse_t * ell, mpfr_t angle, mpfr_t big_mass);

void model_kepler_get_new_angle(mpfr_t new_angle, model_kepler_ellipse_t * ell, mpfr_t _old_angle, mpfr_t big_mass, mpfr_t time_diff);
