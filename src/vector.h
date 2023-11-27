#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "main.h"

typedef struct {
    struct FormulaType* data;
    size_t size;
    size_t capacity;
} Vector_formula;

void initVector(Vector_formula* vector, size_t capacity);

void pushBack(Vector_formula* vector, struct FormulaType fdata);

void freeVector(Vector_formula* vector);
