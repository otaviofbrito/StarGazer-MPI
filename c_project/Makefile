CC = mpicc
CFLAGS = -Wall -Iimage
LDFLAGS = -lm
SRC = src/main.c image/image.c image/rotulacao.c image/limiarizacao.c
OBJ_DIR = object
OBJ = $(SRC:src/%.c=$(OBJ_DIR)/%.o)

all: main

# Cria o diretório de objetos, se não existir
$(OBJ_DIR)/%.o: src/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

main: $(OBJ)
	$(CC) -o main $(OBJ) $(LDFLAGS)


run: 
	@echo "Executing the program with $(p) processes and image file <$(img)>"
	mpirun -np $(p) ./main $(img)

clean:
	rm -rf $(OBJ_DIR) main
	rm -rf ./data/*.csv


