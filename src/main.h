#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

enum pType
{
    null,
    pChar,
    pInt,
    pFormula
};

struct FormulaType
{
    unsigned int ind_row1;
    unsigned int ind_col1;
    unsigned int ind_row2;
    unsigned int ind_col2;
    int value_ind_col1;
    int value_ind_col2;
    unsigned int ind_retun_row;
    unsigned int ind_retun_col;
    unsigned int length_dependency; //длина цепочки зависимости (сколько формул предстоит решить до решения данной)
    int valueNumberArg1;
    int valueNumberArg2;
    char operation;
    bool argOneIsNumber;
    bool argTwoIsNumber;
};

//структура для хранения данных в двухмерном массиве
struct MatrixElement
{
    enum pType pointerType;
    void* ptr;
};