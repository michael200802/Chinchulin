#include "model_kepler.h"

void model_kepler_get_c_val(mpfr_t c_val, mpfr_t a_val, mpfr_t b_val)
{
	MPFR_DECL_INIT(aux,MPFR_DEFAULT_PRECISION);

	mpfr_pow_ui(c_val,a_val,2,MPFR_RNDN);
	mpfr_pow_ui(aux,b_val,2,MPFR_RNDN);

	mpfr_sub(c_val,c_val,aux,MPFR_RNDN);

	mpfr_sqrt(c_val,c_val,MPFR_RNDN);
}

void model_kepler_get_e_val(mpfr_t e_val, mpfr_t a_val, mpfr_t b_val)
{
	MPFR_DECL_INIT(c_val,MPFR_DEFAULT_PRECISION);

	model_kepler_get_c_val(c_val,a_val,b_val);

	mpfr_div(e_val,c_val,a_val,MPFR_RNDN);
}

void model_kepler_get_ellipse(model_kepler_ellipse_t * ell, mpfr_t aphelion, mpfr_t perihelion)
{
	mpfr_init2(ell->a,MPFR_DEFAULT_PRECISION);
	mpfr_init2(ell->b,MPFR_DEFAULT_PRECISION);
	mpfr_init2(ell->c,MPFR_DEFAULT_PRECISION);
	mpfr_init2(ell->e,MPFR_DEFAULT_PRECISION);

	mpfr_add(ell->a,aphelion,perihelion,MPFR_RNDN);
	mpfr_div_ui(ell->a,ell->a,2,MPFR_RNDN);

	mpfr_set(ell->b,aphelion,MPFR_RNDN);
	mpfr_mul(ell->b,ell->b,perihelion,MPFR_RNDN);
	mpfr_sqrt(ell->b,ell->b,MPFR_RNDN);

	model_kepler_get_c_val(ell->c,ell->a,ell->b);
	model_kepler_get_e_val(ell->e,ell->a,ell->b);
}

void model_kepler_get_r_val(mpfr_t r_val, model_kepler_ellipse_t * ell, mpfr_t angle)
{
	MPFR_DECL_INIT(first,MPFR_DEFAULT_PRECISION);
	MPFR_DECL_INIT(second,MPFR_DEFAULT_PRECISION);

	mpfr_cos(first,angle,MPFR_RNDN);
	mpfr_sin(second,angle,MPFR_RNDN);

	mpfr_pow_ui(first,first,2,MPFR_RNDN);
	mpfr_pow_ui(second,second,2,MPFR_RNDN);

	mpfr_div(first,first,ell->a,MPFR_RNDN);
	mpfr_div(first,first,ell->a,MPFR_RNDN);

	mpfr_div(second,second,ell->b,MPFR_RNDN);
	mpfr_div(second,second,ell->b,MPFR_RNDN);

	mpfr_add(r_val,first,second,MPFR_RNDN);
	mpfr_sqrt(r_val,r_val,MPFR_RNDN);
	mpfr_pow_si(r_val,r_val,-1,MPFR_RNDN);
}

void model_kepler_get_vel_val(mpfr_t vel_val, model_kepler_ellipse_t * ell, mpfr_t angle, mpfr_t big_mass)
{
        MPFR_DECL_INIT(first,MPFR_DEFAULT_PRECISION);
        MPFR_DECL_INIT(second,MPFR_DEFAULT_PRECISION);

	MPFR_DECL_INIT(G_const_exp,MPFR_DEFAULT_PRECISION);
	MPFR_DECL_INIT(G_const,MPFR_DEFAULT_PRECISION);

	mpfr_ui_pow_ui(G_const_exp,10,11,MPFR_RNDN);
	mpfr_pow_si(G_const_exp,G_const_exp,-1,MPFR_RNDN);

	mpfr_set_str(G_const,"6.6743",10,MPFR_RNDN);
	mpfr_mul(G_const,G_const,G_const_exp,MPFR_RNDN);

	mpfr_cos(first,angle,MPFR_RNDN);
	mpfr_mul(first,first,ell->e,MPFR_RNDN);
	mpfr_add_ui(first,first,1,MPFR_RNDN);
	mpfr_div(first,first,ell->a,MPFR_RNDN);

	mpfr_pow_ui(second,ell->e,2,MPFR_RNDN);
	mpfr_ui_sub(second,1,second,MPFR_RNDN);

	mpfr_div(first,first,second,MPFR_RNDN);

	mpfr_ui_div(second,1,ell->a,MPFR_RNDN);
	mpfr_div_ui(second,second,2,MPFR_RNDN);

	mpfr_sub(vel_val,first,second,MPFR_RNDN);
	mpfr_mul(vel_val,vel_val,big_mass,MPFR_RNDN);
	mpfr_mul(vel_val,vel_val,G_const,MPFR_RNDN);
	mpfr_mul_ui(vel_val,vel_val,2,MPFR_RNDN);
	mpfr_sqrt(vel_val,vel_val,MPFR_RNDN);
}

void model_kepler_get_angle_vel(mpfr_t angle_vel, model_kepler_ellipse_t * ell, mpfr_t angle, mpfr_t big_mass)
{
	MPFR_DECL_INIT(vel_val,MPFR_DEFAULT_PRECISION);
        MPFR_DECL_INIT(r_val,MPFR_DEFAULT_PRECISION);

	model_kepler_get_vel_val(vel_val,ell,angle,big_mass);
	model_kepler_get_r_val(r_val,ell,angle);

	mpfr_div(angle_vel,vel_val,r_val,MPFR_RNDN);
}

#define TIME_DIFF_NSTEPS 100

void model_kepler_get_new_angle(mpfr_t new_angle, model_kepler_ellipse_t * ell, mpfr_t _old_angle, mpfr_t big_mass, mpfr_t time_diff)
{
	MPFR_DECL_INIT(old_angle,MPFR_DEFAULT_PRECISION);
	MPFR_DECL_INIT(angle_vel,MPFR_DEFAULT_PRECISION);
	MPFR_DECL_INIT(angle_diff,MPFR_DEFAULT_PRECISION);
	MPFR_DECL_INIT(time_diff_step,MPFR_DEFAULT_PRECISION);

	mpfr_set(old_angle,_old_angle,MPFR_RNDN);

	mpfr_set(new_angle,old_angle,MPFR_RNDN);

	mpfr_div_ui(time_diff_step,time_diff,TIME_DIFF_NSTEPS,MPFR_RNDN);

	for(size_t i = 0; i < TIME_DIFF_NSTEPS; i++)
	{
		model_kepler_get_angle_vel(angle_vel,ell,new_angle,big_mass);
		mpfr_mul(angle_diff,angle_vel,time_diff_step,MPFR_RNDN);
		mpfr_add(new_angle,new_angle,angle_diff,MPFR_RNDN);
	}

	MPFR_DECL_INIT(pi_val,MPFR_DEFAULT_PRECISION);
	MPFR_DECL_INIT(completed_angle,MPFR_DEFAULT_PRECISION);
	mpfr_const_pi(pi_val,MPFR_RNDN);
	mpfr_mul_ui(completed_angle,pi_val,2,MPFR_RNDN);
	if(mpfr_cmp(completed_angle,new_angle) < 0)
	{
		mpfr_sub(new_angle,new_angle,completed_angle,MPFR_RNDN);
	}

}
