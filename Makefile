CC = gcc
CFLAGS = -Wall -O3

# Исходные файлы
SOURCES = src/main.c src/vector.c

# Генерация имен объектных файлов из имен исходных файлов
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = bin/csv_parser

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(EXECUTABLE)

# Обобщенное правило для компиляции .c файлов в .o файлы в подкаталоге src
src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@