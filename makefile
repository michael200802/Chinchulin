compiler=x86_64-w64-mingw32-gcc
flags=-mwindows

GUI_objs=GUI/input.o GUI/input_line.o GUI/input_quantity.o GUI/input_body.o GUI/input_planet.o GUI/input_planet_sys_child.o  GUI/input_planet_sys.o

GUI/%.o: GUI/%.c GUI/%.h
	$(compiler) -c $(@:.o=.c) -o $@

wmain.exe: $(GUI_objs) main.c
	$(compiler) $(flags) $? -o $@

all: $(GUI_objs) wmain.exe
	

clean:
	rm $(GUI_objs)
