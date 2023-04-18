compiler=x86_64-w64-mingw32-gcc
flags= -mwindows -lmpfr -lgmp

GUI_objs=GUI/input.o GUI/input_line.o GUI/input_quantity.o GUI/input_body.o GUI/input_planet.o GUI/input_planet_sys_child.o  GUI/input_planet_sys.o

PLT_SYS_objs=planet_sys/plt_sys_entity.o planet_sys/update_coord.o planet_sys/model_kepler.o

GUI/%.o: GUI/%.c GUI/%.h
	$(compiler) -c $(@:.o=.c) -o $@

planet_sys/%.o: planet_sys/%.c planet_sys/%.h
	$(compiler) -c $(@:.o=.c) -o $@

wmain.exe: $(GUI_objs) $(PLT_SYS_objs) main.c
	$(compiler) $? -o $@ $(flags)

all: $(GUI_objs) wmain.exe
	

clean:
	rm $(GUI_objs)
