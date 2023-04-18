#include "update_coord.h"

void update_coord_def(void* _ent, mpfr_t time)
{
	plt_sys_entity_t * ent = _ent;
	model_kepler_ellipse_t ell;
	MPFR_DECL_INIT(r_val,MPFR_DEFAULT_PRECISION);

	model_kepler_get_ellipse(&ell,ent->trajectory.aphelion, ent->trajectory.perihelion);

	model_kepler_get_r_val(r_val,&ell,ent->coord.cur_angle);

	model_kepler_get_new_angle(ent->coord.cur_angle,&ell,ent->coord.cur_angle, ent->trajectory.big_mass, time);

	mpfr_cos(ent->coord.X,ent->coord.cur_angle,MPFR_RNDN);
	mpfr_sin(ent->coord.Y,ent->coord.cur_angle,MPFR_RNDN);

	mpfr_mul(ent->coord.X,ent->coord.X,r_val,MPFR_RNDN);
	mpfr_mul(ent->coord.Y,ent->coord.Y,r_val,MPFR_RNDN);

	mpfr_sub(ent->coord.X,ent->coord.X,ell.c,MPFR_RNDN);

	mpfr_clears(ell.a,ell.b,ell.e,ell.c,(mpfr_ptr)0);
}

void update_coord_nomove(void* _ent, mpfr_t time)
{
}

void update_coord_exp(void* _ent, mpfr_t time)
{
	
}
