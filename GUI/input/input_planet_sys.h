#include "types.h"
#include "input.h"
#include "input_quantity.h"
#include "input_body.h"
#include "input_planet.h"
#include "input_planet_sys_child.h"

input_planet_sys_t input_planet_sys_create(const char* defname, size_t line, size_t n_tab);

//after the user clicked hButton_new_elem
void input_planet_sys_add_planet(input_planet_sys_t * input);
void input_planet_sys_add_sys(input_planet_sys_t * input);

//after the user clicked hButton_destroy
void input_planet_sys_remove_planet(input_planet_sys_t * input, size_t i);
void input_planet_sys_remove_sys(input_planet_sys_t * input, size_t i);

void input_planet_sys_destroy(input_planet_sys_t * input);



