compiler=x86_64-w64-mingw32-gcc
flags= -mwindows -lmpfr -lgmp

GUI_DIR=../GUI
GUI_INPUT_DIR=$(GUI_DIR)/input
GUI_OUTPUT_DIR=$(GUI_DIR)/output
GUI_WORKER_DIR=$(GUI_DIR)/worker

GUI_WORKER_objs=$(GUI_WORKER_DIR)/worker.o
GUI_INPUT_objs=$(GUI_INPUT_DIR)/input.o $(GUI_INPUT_DIR)/input_line.o $(GUI_INPUT_DIR)/input_quantity.o $(GUI_INPUT_DIR)/input_body.o $(GUI_INPUT_DIR)/input_planet.o $(GUI_INPUT_DIR)/input_planet_sys_child.o  $(GUI_INPUT_DIR)/input_planet_sys.o
GUI_OUTPUT_objs=$(GUI_OUTPUT_DIR)/output_drawing_queue.o $(GUI_OUTPUT_DIR)/output_render.o $(GUI_OUTPUT_DIR)/output_sender.o $(GUI_OUTPUT_DIR)/output_wnd.o

PLT_SYS_DIR=../planet_sys
PLT_SYS_objs=$(PLT_SYS_DIR)/plt_sys.o $(PLT_SYS_DIR)/plt_sys_entity.o $(PLT_SYS_DIR)/update_coord.o $(PLT_SYS_DIR)/model_kepler.o

$(GUI_INPUT_DIR)/%.o: $(GUI_INPUT_DIR)/%.c $(GUI_INPUT_DIR)/%.h
	$(compiler) -c $(@:.o=.c) -o $@

$(GUI_OUTPUT_DIR)/%.o: $(GUI_OUTPUT_DIR)/%.c $(GUI_OUTPUT_DIR)/%.h
	$(compiler) -c $(@:.o=.c) -o $@

$(GUI_WORKER_DIR)/%.o: $(GUI_WORKER_DIR)/%.c $(GUI_WORKER_DIR)/%.h
	$(compiler) -c $(@:.o=.c) -o $@

$(PLT_SYS_DIR)/%.o: $(PLT_SYS_DIR)/%.c $(PLT_SYS_DIR)/%.h
	$(compiler) -c $(@:.o=.c) -o $@

OBJ_files=$(PLT_SYS_objs) $(GUI_INPUT_objs) $(GUI_OUTPUT_objs) $(GUI_WORKER_objs)

clean:
	rm $(OBJ_files)

../run/wmain.exe: $(OBJ_files) main.c
	$(compiler) $? -o $@ $(flags)

all:  ../run/wmain.exe
	

