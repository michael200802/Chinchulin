#include "output_render.h"

void*output_render_routine(void*arg)
{
	output_render_t * render = arg;

        output_drawing_t new_drawing = { malloc(sizeof(output_img_t)*render->plt_sys.len), render->plt_sys.len};

        for(size_t i = 0; i < render->plt_sys.len; i++)
        {
                plt_sys_entity_t * ent = plt_sys_get_ent(&render->plt_sys,i);

                //set x and y
                MPFR_DECL_INIT(x_pos,MPFR_DEFAULT_PRECISION);
                MPFR_DECL_INIT(y_pos,MPFR_DEFAULT_PRECISION);
                plt_sys_get_real_coord(x_pos,y_pos,&ent->coord);

                mpfr_div(x_pos,x_pos,render->dis_unit,MPFR_DEFAULT_RND);
                mpfr_div(y_pos,y_pos,render->dis_unit,MPFR_DEFAULT_RND);
                output_img_t img = {
                                .screen_x = mpfr_get_si(x_pos,MPFR_DEFAULT_RND),
                                .screen_y = -1*mpfr_get_si(y_pos,MPFR_DEFAULT_RND),
                        };

                //set radius
                MPFR_DECL_INIT(radius,MPFR_DEFAULT_PRECISION);
                mpfr_div(radius,ent->body.radius,render->dis_unit,MPFR_DEFAULT_RND);
                img.radius = mpfr_get_ui(radius,MPFR_DEFAULT_RND)+1;

                //set line x and y
                if(ent->coord.origin)
                {
                        plt_sys_get_real_coord(x_pos,y_pos,ent->coord.origin);
                        mpfr_div(x_pos,x_pos,render->dis_unit,MPFR_DEFAULT_RND);
                        mpfr_div(y_pos,y_pos,render->dis_unit,MPFR_DEFAULT_RND);
                        img.lend_x = mpfr_get_si(x_pos,MPFR_DEFAULT_RND);
                        img.lend_y = -1*mpfr_get_si(y_pos,MPFR_DEFAULT_RND);
                }
                else
                {
                        img.lend_x = 0;
                        img.lend_y = 0;
                }

                //color of the circle
                float cur_angle = mpfr_get_flt(ent->coord.cur_angle,MPFR_DEFAULT_RND);
                unsigned char green_val, blue_val;
                green_val = blue_val = 255-255 * sqrt( (cos(cur_angle)+1)/2 );
                img.rgb = RGB(255,green_val,blue_val);

                new_drawing.img_arr[i] = img;
        }
	plt_sys_update(&render->plt_sys,render->time_unit);

	output_drawing_queue_enqueue(render->queue,new_drawing);

        usleep(50*1000);
	return NULL;
}

void output_render_create(output_worker_t * render, output_drawing_queue_t * queue, plt_sys_t plt_sys, mpfr_t time_unit, mpfr_t dis_unit)
{
	output_render_t * arg = malloc(sizeof(output_render_t));
	*arg = (output_render_t){.queue = queue, .plt_sys = plt_sys};
	mpfr_init2(arg->time_unit,MPFR_DEFAULT_PRECISION);
	mpfr_init2(arg->dis_unit,MPFR_DEFAULT_PRECISION);
	mpfr_set(arg->time_unit,time_unit,MPFR_DEFAULT_RND);
	mpfr_set(arg->dis_unit,dis_unit,MPFR_DEFAULT_RND);

	output_worker_create(render,output_render_routine,arg);
}



