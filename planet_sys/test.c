#include <stdio.h>

#include "model_kepler.c"

int main()
{
	mpfr_t af, pe;
	mpfr_t r_val, vel_val, angle_vel;
	model_kepler_ellipse_t ellipse;

	mpfr_init2(af,MPFR_DEFAULT_PRECISION);
	mpfr_init2(pe,MPFR_DEFAULT_PRECISION);
	mpfr_init2(r_val,MPFR_DEFAULT_PRECISION);
	mpfr_init2(vel_val,MPFR_DEFAULT_PRECISION);
	mpfr_init2(angle_vel,MPFR_DEFAULT_PRECISION);

	mpfr_set_str(af,"152000000000",10,MPFR_RNDN);
	mpfr_set_str(pe,"147000000000",10,MPFR_RNDN);

	puts("afelio: ");
	mpfr_printf("%.128RNf",af);
	mpfr_out_str(stdout,10,0,af,MPFR_RNDN);
	puts("");

	puts("Perihelio:");
	mpfr_out_str(stdout,10,0,pe,MPFR_RNDN);

	MPFR_DECL_INIT(big_mass,MPFR_DEFAULT_PRECISION);
	mpfr_set_str(big_mass,"1989000000000000000000000000000",10,MPFR_RNDN);

	puts("");

	puts("Masa:");
	mpfr_out_str(stdout,10,0,big_mass,MPFR_RNDN);

	puts("\n\n");

	model_kepler_get_ellipse(&ellipse,af,pe);

	printf("a%f b%f c%f e%f\n",mpfr_get_flt(ellipse.a,MPFR_RNDN),mpfr_get_flt(ellipse.b,MPFR_RNDN),mpfr_get_flt(ellipse.c,MPFR_RNDN),mpfr_get_flt(ellipse.e,MPFR_RNDN));

	MPFR_DECL_INIT(cur_angle,MPFR_DEFAULT_PRECISION);
	MPFR_DECL_INIT(old_angle,MPFR_DEFAULT_PRECISION);
	MPFR_DECL_INIT(time_diff,MPFR_DEFAULT_PRECISION);

	mpfr_set_ui(cur_angle,0,MPFR_RNDN);
	mpfr_set_str(time_diff, "86400", 10,MPFR_RNDN);

	size_t i = 0;
	for(; i <= 365;i++)
	{
		model_kepler_get_r_val(r_val,&ellipse,cur_angle);
		model_kepler_get_vel_val(vel_val,&ellipse,cur_angle,big_mass);
		model_kepler_get_angle_vel(angle_vel,&ellipse,cur_angle,big_mass);

		printf("%zu: \n",i);

		printf("\tO=");
		mpfr_out_str(stdout,10,0,cur_angle,MPFR_RNDN);
		putchar('\n');

		printf("\t\tr=");
		mpfr_out_str(stdout,10,0,r_val,MPFR_RNDN);
		putchar('\n');

		printf("\t\tvt=");
		mpfr_out_str(stdout,10,0,vel_val,MPFR_RNDN);
		putchar('\n');

		printf("\t\tw=");
		mpfr_out_str(stdout,10,0,angle_vel,MPFR_RNDN);
		putchar('\n');

		mpfr_set(old_angle, cur_angle, MPFR_RNDN);
		model_kepler_get_new_angle(cur_angle,&ellipse,old_angle,big_mass,time_diff);
	}

	mpfr_printf("");

	return 0;
}
