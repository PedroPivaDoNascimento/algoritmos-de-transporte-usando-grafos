# Variáveis de compilação
CC = gcc
CFLAGS = -Wall -Wextra -I./src/matriz -I./src/view -I./src/controller -I./src -I./src/utils

# Pastas do projeto
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Nome do executável final
TARGET = $(BIN_DIR)/algoritmo_transporte

OBJS = $(OBJ_DIR)/matriz/matriz.o \
       $(OBJ_DIR)/view/view.o \
       $(OBJ_DIR)/controller/controller.o \
       $(OBJ_DIR)/main.o \
	   $(OBJ_DIR)/utils/utils.o

# Regra padrão (compila o executável)
all: $(TARGET)

# Regra para linkar os objetos e criar o executável final
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(OBJS) -o $(TARGET)

# Regra específica para o main.c que está na raiz da pasta src
$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/main.c -o $(OBJ_DIR)/main.o

$(OBJ_DIR)/matriz/matriz.o: $(SRC_DIR)/matriz/matriz.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/controller/controller.o: $(SRC_DIR)/controller/controller.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/view/view.o: $(SRC_DIR)/view/view.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@
$(OBJ_DIR)/utils/utils.o: $(SRC_DIR)/utils/utils.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Executa o programa
run: $(TARGET)
	./$(TARGET)
	
# Limpa os arquivos gerados
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Declara alvos que não representam arquivos físicos
.PHONY: all run clean