# Executable
EXEC=katrologger
SCREENSHOT=screenshot
RUN=/root/.katrologger/run
BIN=/usr/bin/
CONFIG=/root/.katrologger/
SYSTEMD=/etc/systemd/system/katrologger.service

# Compiler
CC=gcc
# .c sources
C_SRC=$(wildcard ./src/*.c)
# .h headers
H_SRC=$(wildcard ./src/*.h)
# Object files
OBJ=$(subst .c,.o,$(subst src,objs,$(C_SRC)))
# Flags
CC_FLAGS=-w          \
         -lquickmail \
				 -lX11

all: objFolder $(EXEC) $(SCREENSHOT)

$(EXEC): $(OBJ)
	@ echo '>>> Building binary using GCC linker: $@'
	$(CC) $^ -o $@ $(CC_FLAGS)
	@ echo 'Finished building binary: $@'
	@ echo ' '

./objs/%.o: ./src/%.c ./src/%.h
	@ echo '>>> Building target using GCC compiler: $<'
	$(CC) -c $< -o $@ -w
	@ echo ' '

./objs/main.o: ./src/main.c $(H_SRC)
	@ echo '>>> Building target using GCC compiler: $<'
	$(CC) -c $< -o $@ -w
	@ echo ' '

$(SCREENSHOT):
	@ echo '>>> Building target using GCC compiler: $@'
	$(CC) ./src/screenshot/screenshot.c -o $(RUN)/$@ -w -lcairo -lX11
	@ echo 'Finished building binary: $@'
	@ echo ' '

objFolder:
	@ mkdir -p objs

clean:
	@ rm -rf ./objs/*.o $(EXEC) $(SCREENSHOT)
	@ rmdir objs

.PHONY: all clean

make install:
	@ mv $(EXEC) $(BIN)
	@ echo "Installing: $(EXEC) -> $(BIN)"

uninstall:
	@ echo "remove binary -> $(BIN)$(EXEC)"
	@ rm -f $(BIN)$(EXEC)
	@ echo "remove directory -> $(CONFIG)"
	@ rm -rf $(CONFIG)
	@ echo "remove systemd unif-file -> $(SYSTEMD)"
	@ rm -f $(SYSTEMD)
