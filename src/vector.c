#include "vector.h"

void initVector(Vector_formula* vector, size_t capacity) {
    vector->data = malloc(capacity * sizeof(struct FormulaType));
    vector->size = 0;
    vector->capacity = capacity;
}

void pushBack(Vector_formula* vector, struct FormulaType fdata) {
    if (vector->size == vector->capacity) {
        // Увеличиваем capacity в два раза при необходимости
        vector->capacity *= 2;
        vector->data = realloc(vector->data, vector->capacity * sizeof(struct FormulaType));
    }
    if (vector->data == NULL) {
        // Обработка ошибки выделения памяти
        fprintf(stderr, "Memory allocation error in the vector_pushBack function\n");
        exit(EXIT_FAILURE);
    }
    vector->data[vector->size++] = fdata;
}

//void printVector(const Vector_formula* vector) {
//    for (size_t i = 0; i < vector->size; ++i) {
//        printf("%d ", vector->data[i]);
//    }
//    printf("\n");
//}

void freeVector(Vector_formula* vector) {
    free(vector->data);
    vector->size = 0;
    vector->capacity = 0;
}