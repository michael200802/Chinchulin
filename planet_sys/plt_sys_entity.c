#include "plt_sys_entity.h"

void plt_sys_entity_init(
        plt_sys_entity_t * ent,
        mpfr_t radius, mpfr_t mass,//plt_sys_body_attr_t
        entity_direction_t direction, mpfr_t aphelion, mpfr_t perihelion, void (*update_coord)(plt_sys_entity_t*, mpfr_t),//plt_sys_trajectory_t
        mpfr_t inclination_angle, mpfr_t phase_angle, plt_sys_coord_t * origin//plt_sys_coord_t
)
{
	mpfr_set(ent->body.radius,radius,MPFR_RNDN);
	mpfr_set(ent->body.mass,mass,MPFR_RNDN);

	ent->trajectory.direction = direction;
	mpfr_set(ent->trajectory.aphelion,aphelion,MPFR_RNDN);
	mpfr_set(ent->trajectory.perihelion,perihelion,MPFR_RNDN);
	ent->trajectory.update_coord = update_coord;


	mpfr_set(ent->coord.inclination_angle,inclination_angle,MPFR_RNDN);
	mpfr_set(ent->coord.cur_angle,phase_angle,MPFR_RNDN);
	ent->coord.origin = origin;

	//get initial x and y
	MPFR_DECL_INIT(stime,DEFAULT_PREC);
	mpfr_set_ui(stime,0,MPFR_RNDN);
	update_coord(ent,stime);
}

void plt_sys_entity_destroy(
        plt_sys_entity_t * ent
)
{
	mpfr_clear(ent->body.radius);
	mpfr_clear(ent->body.mass);

	mpfr_clear(ent->trajectory.aphelion);
	mpfr_clear(ent->trajectory.perihelion);

	mpfr_clear(ent->coord.X);
	mpfr_clear(ent->coord.Y)
	mpfr_clear(ent->coord.cur_angle);
	mpfr_clear(ent->coord.inclination_angle);
}

