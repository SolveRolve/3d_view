CC=gcc
STDFLAGS=-std=c11 -Wall -Werror -Wextra #-g -fsanitize=address
GCOV_FLAGS=-fprofile-arcs -ftest-coverage
LINUX_FLAGS=-lpthread -pthread -lrt -lm
LIBS=-lcheck

TARGET=s21_3dviewer.a
SRC=$(wildcard ./backend/*.c) $(wildcard ./backend/matrix/*.c)
OBJ=$(patsubst ./backend/%.c, ./backend/%.o, ${SRC})

NEW_TEST_SRC=$(wildcard ./backend/tests/*.c)
NEW_TEST_OBJ=$(patsubst ./backend/tests/%.c, %.o, $(NEW_TEST_SRC))

OS = $(shell uname)
ifeq ($(OS),Linux)
  OPEN_CMD = xdg-open
endif
ifeq ($(OS),Darwin)
	OPEN_CMD = open
endif



all: clean install dist test dvi run 

install: uninstall
	mkdir build
	cd s21_3d-viewer/ && qmake && make && make clean && rm Makefile && cd ../ && mv s21_3d-viewer/s21_3d-viewer.app build

uninstall:
	rm -rf build

run:
	open ./build/s21_3d-viewer.app/Contents/MacOS/s21_3d-viewer

dvi:
	open ./readme.html

dist:
	mkdir 3DViewer/
	mkdir 3DViewer/src
	cp -R backend s21_3d-viewer backend/tests build Makefile 3DViewer/src
	tar cvzf 3DViewer.tgz 3DViewer/
	rm -rf 3DViewer

$(TARGET): clean ${SRC}
	$(CC) -c $(STDFLAGS) $(SRC)
	mv ./s21_matrix.o ./backend/matrix
	mv ./*.o ./backend
	ar rcs  $@ $(OBJ)

test: $(TARGET)
ifeq ($(OS), Darwin)
	${CC} $(STDFLAGS) ${NEW_TEST_SRC} $(TARGET) -o lib_test $(LIBS) 
else
	${CC} $(STDFLAGS) ${NEW_TEST_SRC} $(TARGET) -o lib_test $(LIBS) $(LINUX_FLAGS)
endif

	./lib_test

gcov_report: clean_test
ifeq ($(OS), Darwin)
	$(CC) $(STDFLAGS) $(GCOV_FLAGS)  ${NEW_TEST_SRC} ${SRC} -o test $(LIBS)
else
	$(CC) $(STDFLAGS) $(GCOV_FLAGS)  ${NEW_TEST_SRC} ${SRC} -o test $(LIBS) $(LINUX_FLAGS)
endif
	./test
	lcov -t "stest" -o s21_test.info -c -d .
	genhtml -o report s21_test.info
	$(OPEN_CMD) ./report/index.html


clean: uninstall clean_test
	rm -rf *.a
	rm -rf *.o
	rm -rf s21_3d-viewer/*.o
	rm -rf backend/*.o
	rm -rf backend/matrix/*.o 
	rm -rf lib_test
	rm -rf 3DViewer.tgz

clean_test:
	rm -rf *.gcda
	rm -rf *.gcno
	rm -rf *.info
	rm -rf test
	rm -rf report
