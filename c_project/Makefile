CC = mpicc
UTILS = image_utils
CFLAGS = -Wall -I$(UTILS)
LDFLAGS = -lm
SRC = src/main.c $(UTILS)/image.c $(UTILS)/label.c $(UTILS)/threshold.c $(UTILS)/distance_transform.c
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
	@echo "[INFO] Usage: make run p=<number of process> img=<path/to/image.pgm> th=<threshold value>"
	mpirun -np $(p) ./main ${th} $(img)

clean:
	@echo "[INFO] Cleaning environment..."
	rm -rf $(OBJ_DIR) main


