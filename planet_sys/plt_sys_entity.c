#include "plt_sys_entity.h"

void __plt_sys_entity_init(plt_sys_entity_t * ent)
{
	mpfr_inits2(MPFR_DEFAULT_PRECISION,
		ent->coord.cur_angle,ent->coord.inclination_angle,
		ent->coord.X,ent->coord.Y,
		ent->body.radius,ent->body.mass,
		ent->trajectory.aphelion,ent->trajectory.perihelion,ent->trajectory.big_mass,
		(mpfr_ptr)0);
}

void plt_sys_entity_init(
        plt_sys_entity_t * ent,
        mpfr_t radius, mpfr_t mass,//plt_sys_body_attr_t
        entity_direction_t direction, mpfr_t aphelion, mpfr_t perihelion, mpfr_t big_mass, void (*update_coord)(void*, mpfr_t),//plt_sys_trajectory_t
        mpfr_t inclination_angle, mpfr_t phase_angle, plt_sys_coord_t * origin//plt_sys_coord_t
)
{
	__plt_sys_entity_init(ent);

	mpfr_set(ent->body.radius,radius,MPFR_RNDN);
	mpfr_set(ent->body.mass,mass,MPFR_RNDN);

	ent->trajectory.direction = direction;
	mpfr_set(ent->trajectory.aphelion,aphelion,MPFR_RNDN);
	mpfr_set(ent->trajectory.perihelion,perihelion,MPFR_RNDN);
	mpfr_set(ent->trajectory.big_mass,big_mass,MPFR_RNDN);
	ent->trajectory.update_coord = update_coord;

	mpfr_set(ent->coord.inclination_angle,inclination_angle,MPFR_RNDN);
	mpfr_set(ent->coord.cur_angle,phase_angle,MPFR_RNDN);
	ent->coord.origin = origin;

	//get initial x and y
	MPFR_DECL_INIT(stime,DEFAULT_PREC);
	mpfr_set_ui(stime,0,MPFR_RNDN);
	update_coord(ent,stime);

}

void plt_sys_get_real_coord(
		mpfr_t x_val, mpfr_t y_val,
                plt_sys_coord_t * coord
        )
{
	MPFR_DECL_INIT(module,MPFR_DEFAULT_PRECISION);
	MPFR_DECL_INIT(new_angle,MPFR_DEFAULT_PRECISION);
	MPFR_DECL_INIT(x_val_aux,MPFR_DEFAULT_PRECISION);
	MPFR_DECL_INIT(y_val_aux,MPFR_DEFAULT_PRECISION);

	mpfr_set_ui(x_val,0,MPFR_RNDN);
	mpfr_set_ui(y_val,0,MPFR_RNDN);

	for(int i = 0;coord;i++)
	{
		mpfr_pow_ui(x_val_aux,coord->X,2,MPFR_RNDN);
		mpfr_pow_ui(y_val_aux,coord->Y,2,MPFR_RNDN);
		mpfr_add(module,x_val_aux,y_val_aux,MPFR_RNDN);
		mpfr_sqrt(module,module,MPFR_RNDN);

		mpfr_add(new_angle,coord->cur_angle,coord->inclination_angle,MPFR_RNDN);

		mpfr_cos(x_val_aux,new_angle,MPFR_RNDN);
		mpfr_sin(y_val_aux,new_angle,MPFR_RNDN);

		mpfr_mul(x_val_aux,x_val_aux,module,MPFR_RNDN);
		mpfr_mul(y_val_aux,y_val_aux,module,MPFR_RNDN);

		mpfr_add(x_val,x_val,x_val_aux,MPFR_RNDN);
		mpfr_add(y_val,y_val,y_val_aux,MPFR_RNDN);

                coord = coord->origin;

	}
}

void plt_sys_entity_nomove_init(
                plt_sys_entity_t * ent,
                mpfr_t radius, mpfr_t mass,//plt_sys_body_attr_t
                mpfr_t x_pos, mpfr_t y_pos
        )
{
	__plt_sys_entity_init(ent);

        mpfr_set(ent->body.radius,radius,MPFR_RNDN);
        mpfr_set(ent->body.mass,mass,MPFR_RNDN);

	ent->trajectory.direction = ENTDIR_NOMOVE;
	ent->trajectory.update_coord = update_coord_nomove;

	mpfr_set_ui(ent->coord.inclination_angle,0,MPFR_RNDN);
	mpfr_set_ui(ent->coord.cur_angle,0,MPFR_RNDN);
	mpfr_set(ent->coord.X,x_pos,MPFR_RNDN);
	mpfr_set(ent->coord.Y,y_pos,MPFR_RNDN);
	ent->coord.origin = NULL;
}


void plt_sys_entity_destroy(
        plt_sys_entity_t * ent
)
{
	mpfr_clear(ent->body.radius);
	mpfr_clear(ent->body.mass);

	mpfr_clear(ent->trajectory.aphelion);
	mpfr_clear(ent->trajectory.perihelion);
	mpfr_clear(ent->trajectory.big_mass);

	mpfr_clear(ent->coord.X);
	mpfr_clear(ent->coord.Y);
	mpfr_clear(ent->coord.cur_angle);
	mpfr_clear(ent->coord.inclination_angle);
}

