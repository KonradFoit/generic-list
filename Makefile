HDR_PATH := src
OBJ_PATH := _build/obj

all:
	mkdir -p ${OBJ_PATH}
	gcc -c -I${HDR_PATH} src/generic_list.c -o ${OBJ_PATH}/generic_list.o
	gcc -c -I${HDR_PATH} tests/check_generic_list.c -o ${OBJ_PATH}/check_generic_list.o -L/usr/local/lib -lcheck -lc
	gcc ${OBJ_PATH}/generic_list.o ${OBJ_PATH}/check_generic_list.o -o _build/check_generic_list -L/usr/local/lib -lcheck -lc