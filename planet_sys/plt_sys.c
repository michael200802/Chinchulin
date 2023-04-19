#include "plt_sys.h"

void plt_sys_create(plt_sys_t * plt_sys, size_t len)
{
	*plt_sys = (plt_sys_t){.arr = malloc(len*sizeof(plt_sys_entity_t)), .len = len};
}

plt_sys_entity_t * plt_sys_get_ent(plt_sys_t * plt_sys, size_t index)
{
	if(index < plt_sys->len)
	{
		return plt_sys->arr+index;
	}
	return NULL;
}

void plt_sys_update(plt_sys_t * plt_sys, mpfr_t time)
{
	for(size_t i = 0; i < plt_sys->len; i++)
	{
		plt_sys_entity_t * ent = plt_sys_get_ent(plt_sys,i);
		ent->trajectory.update_coord(ent,time);
	}
}

void plt_sys_destroy(plt_sys_t * plt_sys)
{
	for(size_t i = 0; i < plt_sys->len;i++)
	{
		plt_sys_entity_destroy(
			plt_sys_get_ent(plt_sys,i)
		);
	}
	free(plt_sys->arr);
}


