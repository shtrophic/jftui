CC=clang
OFLAGS=-O2 -march=native
WFLAGS=-Wall -Wpedantic -Wextra -Wconversion -Wstrict-prototypes -Werror=implicit-function-declaration -Werror=implicit-int -Werror=incompatible-pointer-types -Werror=int-conversion
CFLAGS=`pkg-config --cflags libcurl yajl mpv` -pthread
LFLAGS=`pkg-config --libs libcurl yajl mpv`
DFLAGS=-g -O1 -fno-omit-frame-pointer -fno-optimize-sibling-calls -fsanitize=address -fsanitize=undefined

SOURCES=src/linenoise.c src/shared.c src/config.c src/disk_io.c src/json_parser.c src/menu.c src/network.c src/main.c

OBJECTS=build/linenoise.o build/menu.o build/shared.o build/config.o build/disk_io.o build/json_parser.o build/network.o build/main.o

BUILD_DIR := build

.PHONY: all debug install uninstall clean




all: ${BUILD_DIR}/jftui

debug: ${BUILD_DIR}/jftui_debug

install: jftui
	install -Dm555 ${BUILD_DIR}/$^ $(DESTDIR)/usr/bin/$^

uninstall:
	rm $(DESTDIR)/usr/bin/jftui

clean:
	rm -rf ${BUILD_DIR}



${BUILD_DIR}:
	mkdir -p ${BUILD_DIR}

${BUILD_DIR}/jftui: ${BUILD_DIR} $(SOURCES)
	$(CC) $(CFLAGS) $(LFLAGS) $(OFLAGS) $(SOURCES) -g -o $@

${BUILD_DIR}/jftui_debug: ${BUILD_DIR} $(OBJECTS) $(SOURCES)
	$(CC) $(WFLAGS) $(LFLAGS) $(DFLAGS) $(OBJECTS) -o $@

src/command_parser.c: src/command_grammar.leg
	leg -o $@ $^

${BUILD_DIR}/linenoise.o: src/linenoise.c
	$(CC) $(WFLAGS) $(CFLAGS) $(DFLAGS) -c -o $@ $^

${BUILD_DIR}/menu.o: src/menu.c src/command_parser.c
	$(CC) $(WFLAGS) $(CFLAGS) $(DFLAGS) -c -o $@ src/menu.c

${BUILD_DIR}/shared.o: src/shared.c
	$(CC) $(WFLAGS) $(CFLAGS) $(DFLAGS) -c -o $@ $^

${BUILD_DIR}/config.o: src/config.c
	$(CC) $(WFLAGS) $(CFLAGS) $(DFLAGS) -c -o $@ $^

${BUILD_DIR}/disk_io.o: src/disk_io.c
	$(CC) $(WFLAGS) $(CFLAGS) $(DFLAGS) -c -o $@ $^

${BUILD_DIR}/json_parser.o: src/json_parser.c
	$(CC) $(WFLAGS) $(CFLAGS) $(DFLAGS) -c -o $@ $^

${BUILD_DIR}/network.o: src/network.c
	$(CC) $(WFLAGS) $(CFLAGS) $(DFLAGS) -c -o $@ $^

${BUILD_DIR}/main.o: src/main.c
	$(CC) $(WFLAGS) $(CFLAGS) $(DFLAGS) -c -o $@ $^
