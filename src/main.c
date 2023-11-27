#include "main.h"
#include "vector.h"

void countRowsAndColumns(const char* filename, int* rowCount, int* colCount) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
		fprintf(stderr, "The file could not be opened\n");
        exit(EXIT_FAILURE);
    }
    // ���������� ��� �������� �������� ������� � ����������� �������
    char currentChar;
    // ������������� ��������� ����� � ��������
    *rowCount = 0;
    *colCount = 0;
    bool checkfirstSymbol = 1;
    unsigned int firstline_count_comma = 0;   //���������� ������� � ������ ������
	unsigned int count_comma = 0;
    unsigned int string_length = 0; //����� ������ ��� �������� ������ ������ ��� ��� (������ ���������� ���� \n)
    while (1) {
        currentChar = fgetc(file);
		//�������� ������� ������� � �����
		if (checkfirstSymbol) {
			checkfirstSymbol = 0;
			if (currentChar != ',') {
				fprintf(stderr, "Error: invalid table\n");
				fprintf(stderr, "the first column of the first row must be empty\n");
				exit(EXIT_FAILURE);
			}
		}
		//������ ����� ������, ����������� ������� �����
        if (currentChar == '\n') {
			//�������� �� ������ ������
            if (string_length != 0) {
				//���������� ���������� ������� � ������ ������
				if (*rowCount == 0) {
					if (count_comma < 1) {
					fprintf(stderr, "Error: invalid table\n");
					fprintf(stderr, "the number of columns must be at least 2 in each row\n");
					fprintf(stderr, "The number of columns in the %d row is %d\n", *rowCount + 1, count_comma + 1);
					exit(EXIT_FAILURE);
					}
					firstline_count_comma = count_comma;
				}
				//��������� ���������� ������� � ������ (���������� � ������ �������)
				else if (count_comma != firstline_count_comma) {
					fprintf(stderr, "Error: invalid table, invalid number of columns\n");
					fprintf(stderr, "The number of columns in the first row is %d\n", firstline_count_comma+1);
					fprintf(stderr, "The number of columns in the %d row is %d\n", *rowCount + 1, count_comma+1);
					exit(EXIT_FAILURE);
				}

                (*rowCount)++;		//����������� ������� �����
				count_comma = 0;
				string_length = 0;
            }
			//���� ������ ������ �� ������ ������ (� ����� �� ������ ���� ������ �����)
			else {
				fprintf(stderr, "Error: invalid table\n");
				fprintf(stderr, "empty string\n");
				fprintf(stderr, "line number %d\n", *rowCount + 1);
				exit(EXIT_FAILURE);
			}
        } 

		// ������ ����� �����, ������� �� �����
		else if (currentChar == EOF) {
			if (string_length != 0) (*rowCount)++;
			break;
		}
        else {
			// ������ ����� ������, ����������� ������� ��������
			++string_length;
        }
		// ������� �������, ����������� ������� ��������
		if (currentChar == ',') {
			++count_comma;
		}
    }
    //���������� �������� = ���������� ������� + 1
    *colCount = firstline_count_comma + 1;
    // ��������� ����
    fclose(file);
}

//������� ��� ��������� ���� csv �����
int getField(FILE* file, char* field, const int fieldSize) {
	int i = 0;
	char currentChar;
	while (1) {
		currentChar = fgetc(file);
		if (currentChar == ',' || currentChar == '\n' || currentChar == EOF) {
			break;
		}
		//���������� �������
		if (currentChar == ' ') continue;

		field[i] = currentChar;
		i++;
		if (i >= fieldSize - 1) {
			break;
		}
	}
	field[i] = '\0';
	return i;
}

//������� ������ ��������� � ������� � ���� ������
void printExprn(const struct FormulaType* const formula, const struct MatrixElement** const matrix) {
	printf("=");
	printf("%s", (char*)matrix[0][formula->ind_col1].ptr);
	printf("%d", formula->value_ind_col1);
	printf("%c", formula->operation);
	printf("%s", (char*)matrix[0][formula->ind_col2].ptr);
	printf("%d", formula->value_ind_col2);
}

int findIndexCol(const char* index_str_name, const struct MatrixElement** const matrix, const int colCount) {
	bool found = false;
	int index = 0;
	for (int i = 1; i < colCount; i++) {
		//fprintf(stderr, "i=%d\n", i);
		if (strcmp(index_str_name, (char*)matrix[0][i].ptr) == 0) {
			found = true;
			index = i;
			break;
		}
	}
	if (!found) return -1;
	else return index;
}

//������� ��������� ���� �� �� ������� ������ (������������ �������)
//������ true - ���� ������� �������� ����������
bool checking_correct_character_order(const char const* exprField, const unsigned int fieldSize) {
	bool correct_order = true;
	bool found_digit = false;
	for (int i = 0; i < fieldSize; i++) {
		if (isdigit(exprField[i])) {
			found_digit = true;
		}
		else if (found_digit) {
			correct_order = false;
			break;
		}
	}
	return correct_order;
}

//������� ������� ���������� ������� ��������� ARG1 OP ARG2 � ��������� ��������� FormulaType
void parseExprn(const char const *exprField, const unsigned int fieldSize, struct FormulaType* formula, const struct MatrixElement** const matrix, const int colCount) {
    char letters[50] = {0};
    char digits[50] = {0};
    char operation;

    int lettersSize = 0;
    int digitsSize = 0;
    int shiftIndex = 0;

	bool skip_ch = false;

    //���������� ������� ������� ���������
    while (1) {
		if(exprField[1] == '-' && skip_ch == false) {
			shiftIndex = 2;
			digits[0] = '-';
			digitsSize = 1;
			skip_ch = true;
			continue;
		}
		if ((int)exprField[shiftIndex] >= 0 && (int)exprField[shiftIndex] <= 47 || shiftIndex >= fieldSize) {
			break;
		}
		else if (isalpha(exprField[shiftIndex])) {
			letters[lettersSize++] = exprField[shiftIndex];
		}
		else if (isdigit(exprField[shiftIndex])) {
			digits[digitsSize++] = exprField[shiftIndex];
		}
		++shiftIndex;
    }
	if (digitsSize==0) {
		fprintf(stderr, "Error: incorrect expression: %s\n", exprField);
		fprintf(stderr, "error in the FIRST argument\n");
		exit(EXIT_FAILURE);
	}
	//���� �������� �����
	if (lettersSize == 0 && digitsSize != 0) {
		formula->argOneIsNumber = true;
		formula->valueNumberArg1 = atoi(digits);
	}
	//���� �������� ������ �� ������
	else{
		if(!checking_correct_character_order(exprField, lettersSize + digitsSize)) {
			fprintf(stderr, "Error: incorrect expression: %s\n", exprField);
			fprintf(stderr, "incorrect character order in the first argument\n");
			exit(EXIT_FAILURE);
		}
		//���������� ����� ������� ������� ������� ��������, ��� ����� ���������� �� ������ ������ �������
		int index_col = findIndexCol(letters, matrix, colCount);
		if (index_col == -1) {
			fprintf(stderr, "Error: incorrect expression: %s\n", exprField);
			fprintf(stderr, "error in the FIRST argument\n");
			exit(EXIT_FAILURE);
		}
		else formula->ind_col1 = index_col;

		//���������� �������� ������ ������� ��������� (������ �� �������� ����� ������, ��������� ������� ��������� �� �� �����)
		formula->ind_row1 = 0;
		formula->value_ind_col1 = atoi(digits);
	}

    //���������� ������� ��������
    operation = exprField[shiftIndex++];
    if(operation != '+' && operation != '-' && operation != '*' && operation != '/') {
		fprintf(stderr, "Error: invalid operation\n");
		fprintf(stderr, "Operation must be one of the following: +, -, *, /\n");
		fprintf(stderr, "field: %s\n", exprField);
		exit(EXIT_FAILURE);
	}
    formula->operation = operation;
    //���������� ������� ������� ���������
	lettersSize = 0;
	digitsSize = 0;
	const unsigned int arg2_start_index = shiftIndex;
	while (1) {
		if((int)exprField[shiftIndex] >= 0 && (int)exprField[shiftIndex] <= 47 || shiftIndex >= fieldSize) {
			break;
		}
		else if (isalpha(exprField[shiftIndex])) {
			letters[lettersSize++] = exprField[shiftIndex];
		}
		else if (isdigit(exprField[shiftIndex])) {
			digits[digitsSize++] = exprField[shiftIndex];
		}
		++shiftIndex;
	}
	digits[digitsSize] = '\0';	//��������� � ����� ������ ���� ������
	if (digitsSize == 0) {
		fprintf(stderr, "Error: incorrect expression: %s\n", exprField);
		fprintf(stderr, "error in the SECOND argument\n");
		exit(EXIT_FAILURE);
	}
	//���� �������� �����
	if (lettersSize == 0 && digitsSize != 0) {
		formula->argTwoIsNumber = true;
		formula->valueNumberArg2 = atoi(digits);
	}
	else {
		if (!checking_correct_character_order(exprField + arg2_start_index, lettersSize+ digitsSize)) {
			fprintf(stderr, "Error: incorrect expression: %s\n", exprField);
			fprintf(stderr, "incorrect character order in the second argument\n");
			exit(EXIT_FAILURE);
		}
		//���������� ����� ������� ������� ������� ��������, ��� ����� ���������� �� ������ ������ �������
		int index_col = findIndexCol(letters, matrix, colCount);
		if (index_col == -1) {
			fprintf(stderr, "Error: incorrect expression: %s\n", exprField);
			fprintf(stderr, "error in the SECOND argument\n");
			exit(EXIT_FAILURE);
		}
		else formula->ind_col2 = index_col;
		//���������� �������� ������ ������� ��������� (������ �� �������� ����� ������, ��������� ������� ��������� �� �� �����)
		formula->ind_row2 = 0;
		formula->value_ind_col2 = atoi(digits);
	}
}

//������� ��������� ������� �� ������ ������ �� ���� (������ �������� ����������� "-")
bool isdigits(const char* const str, const int length) {
	int start_index = 0;
	if (str[0] == '-') start_index = 1;
	for (int i = start_index; i < length; i++) {
		if (!isdigit(str[i])) return false;  // ���� ���� �� ���� ������ �� �������� ������, ���������� 0
	}
	return true;  // ���� ��� ������� �������� �������, ���������� 1
}

//��������� ����������� ������� �� �����
void loadData(const char* filename, const int rowCount, const int colCount, struct MatrixElement** matrix, Vector_formula* f_vector) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
		fprintf(stderr, "The file could not be opened\n");
        exit(EXIT_FAILURE);
    }
    //��������� ���� ������ ������ � ��������� �� � ������� matrix � ���� �����
    const unsigned int fieldSize = 100;
    char* field = (char*)malloc(fieldSize * sizeof(char));
    if(field == NULL) {
		fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < colCount; i++) {
        unsigned int fieldLen = getField(file, field, fieldSize);
        //dbg ����� � �������
        //printf("%s\n", field);
        matrix[0][i].pointerType = pChar;
        matrix[0][i].ptr = malloc((fieldLen+1) * sizeof(char));
        if(matrix[0][i].ptr == NULL) {
			fprintf(stderr, "Memory allocation error\n");
			exit(EXIT_FAILURE);
		}
        strcpy(matrix[0][i].ptr, field);
    }
    //��������� ��������� ������ � ��������� �� � ������� matrix � ���� ����� ��� �������� FormulaType
    for (int i = 1; i < rowCount; i++) {
		    for (int j = 0; j < colCount; j++) {
			    unsigned int fieldLen = getField(file, field, fieldSize);
			    if (field[0] == '=') {
				    matrix[i][j].pointerType = pFormula;
					matrix[i][j].ptr = NULL;
                    struct FormulaType formula = {0};
                    formula.ind_retun_row = i;
                    formula.ind_retun_col = j;
                    //������ �������
                    parseExprn(field, fieldLen, &formula, (const struct MatrixElement**)matrix, colCount);
                    pushBack(f_vector, formula);
			    }
			    else {
				    matrix[i][j].pointerType = pInt;
				    matrix[i][j].ptr = malloc(sizeof(int));
				    if(matrix[i][j].ptr == NULL) {
						fprintf(stderr, "Memory allocation error\n");
			            exit(EXIT_FAILURE);
		            }
					if (isdigits(field, fieldLen)) {
						*(int*)matrix[i][j].ptr = atoi(field);
					}
					else {
						fprintf(stderr, "Error: invalid table\n");
						fprintf(stderr, "The field must contain a number or a formula\n");
						fprintf(stderr, "row: %d\t col: %d\n", i+1, j+1);
						fprintf(stderr, "now the field contains: %s\n", field);
						exit(EXIT_FAILURE);
					}
			    }
		    }
	    }
    free(field);
    fclose(file);
}

//������� ��� ������ ����������� ������� � �������
void printMatrix(const int rowCount, const int colCount, struct MatrixElement** matrix) {
	for (int i = 0; i < rowCount; i++) {
		for (int j = 0; j < colCount; j++) {
			if (matrix[i][j].pointerType == pChar) {
				printf("%s", (char*)matrix[i][j].ptr);
			}
			else if (matrix[i][j].pointerType == pInt) {
				printf("%d", *(int*)matrix[i][j].ptr);
			}
			else if (matrix[i][j].pointerType == pFormula) {
				printf("*F");
			}
			if (j != colCount - 1) {
				printf(",");
			}
		}
		printf("\n");
	}
}

//������� ��� ������������ ������ ����������� ������� matrix
void freeMatrix(const int rowCount, const int colCount, struct MatrixElement** matrix) {
	for (int i = 0; i < rowCount; i++) {
		for (int j = 0; j < colCount; j++) {
			if (matrix[i][j].ptr != NULL) {
				free(matrix[i][j].ptr);
			}   
		}
		free(matrix[i]);
	}
	free(matrix);
}

//������� ������������� ����������� ������� matrix �������� ����������
void initMatrix(const int rowCount, const int colCount, struct MatrixElement** matrix) {
	for (int i = 0; i < rowCount; i++) {
		for (int j = 0; j < colCount; j++) {
			matrix[i][j].pointerType = null;
			matrix[i][j].ptr = NULL;
		}
	}
}

void dbg_print_Formula(const struct FormulaType* const formula) {
	printf("ind_row1: %d\n", formula->ind_row1);
	printf("ind_col1: %d\n", formula->ind_col1);
	printf("ind_row2: %d\n", formula->ind_row2);
	printf("ind_col2: %d\n", formula->ind_col2);
	printf("value_ind_col1: %d\n", formula->value_ind_col1);
	printf("value_ind_col2: %d\n", formula->value_ind_col2);
	printf("operation: %c\n", formula->operation);
	printf("length_dependency: %d\n", formula->length_dependency);
}

int findIndexRow(const int index_value_name, const struct MatrixElement** const matrix, const int rowCount) {
	bool found = false;
	int index = 0;
	for (int i = 1; i < rowCount; i++) {
		if (*(int*)matrix[i][0].ptr == index_value_name) {
			found = true;
			index = i;
			break;
		}
	}
	if (!found) return -1;
	else return index;
}

//������� ���������� ������� �������� ����� � ��������� FormulaType
void calculateFormula(struct FormulaType* formula, const struct MatrixElement** const matrix, const int rowCount) {
	bool error_flag = false;
	if (formula->argOneIsNumber == false) {
		//���������� ����� ������� ������ ������� ��������, ��� ����� ���������� �� ������� ������� �������
		int index_row = findIndexRow(formula->value_ind_col1, matrix, rowCount);
		if (index_row == -1) error_flag = true;
		else formula->ind_row1 = index_row;
	}
	if (formula->argTwoIsNumber == false) {
		//���������� ����� ������� ������ ������� ��������, ��� ����� ���������� �� ������� ������� �������
		int index_row = findIndexRow(formula->value_ind_col2, matrix, rowCount);
		if (index_row == -1) error_flag = true;
		else formula->ind_row2 = index_row;
	}
	//��������� ������� ������
	if (error_flag) {
		fprintf(stderr, "Error: incorrect expression\n");
		fprintf(stderr, "index row: %d\t col: %d\n", formula->ind_retun_row+1, formula->ind_retun_col+1);
		printExprn(formula, matrix);
		exit(EXIT_FAILURE);
	}
}

//������� ������������� ��������� ������� �� ����� ��������� FormulaType � ������� f_vector
void setPointersMatrixOnVector(const Vector_formula* const f_vector, struct MatrixElement** matrix) {
	for (int i = 0; i < f_vector->size; i++) {
		matrix[f_vector->data[i].ind_retun_row][f_vector->data[i].ind_retun_col].ptr = &f_vector->data[i];
	}
}

//����������� ������� ��� ���������� ����� ������� ����������� ��� ���������� �������
//������������ ������� ������ ��������� ������ �� ������� ������
unsigned int calculateLengthDependecy(struct FormulaType* formula, const struct MatrixElement** const matrix, unsigned int lenDep) {
	unsigned int lenDep1 = 0, lenDep2 = 0;
	//��������� �������� 1
	if (formula->argOneIsNumber == false) {
		if (matrix[formula->ind_row1][formula->ind_col1].pointerType == pFormula) {
			lenDep1 = lenDep + 1;
			lenDep1 = calculateLengthDependecy((struct FormulaType*)matrix[formula->ind_row1][formula->ind_col1].ptr, matrix, lenDep1);
		}
	}
	else lenDep1 = lenDep;
	//��������� �������� 2
	if(formula->argTwoIsNumber == false) {
		if (matrix[formula->ind_row2][formula->ind_col2].pointerType == pFormula) {
			lenDep2 = lenDep + 1;
			lenDep2 = calculateLengthDependecy((struct FormulaType*)matrix[formula->ind_row2][formula->ind_col2].ptr, matrix, lenDep2);
		}
	} 
	else lenDep2 = lenDep;
	//�������� ������������ ��������
	if (lenDep1 > lenDep2) return lenDep1;
	else return lenDep2;
}

//������� ���������� ������� ����� ������� ����������� ��� ������ ������� � ������� f_vector
void runCalcLenDepInVector(Vector_formula* f_vector, const struct MatrixElement** const matrix) {
	for (int i = 0; i < f_vector->size; i++) {
		//�������� ��������� �� ����������� ��������� �� ���� ����
		if (
			(f_vector->data[i].ind_row1 == f_vector->data[i].ind_retun_row) && (f_vector->data[i].ind_col1 == f_vector->data[i].ind_retun_col)
			|| 
			(f_vector->data[i].ind_row2 == f_vector->data[i].ind_retun_row) && (f_vector->data[i].ind_col2 == f_vector->data[i].ind_retun_col)
			) 
		{
			fprintf(stderr, "Error: invalid expression\n");
			fprintf(stderr, "the expression refers to itself\n");
			fprintf(stderr, "row: %d\t col: %d\n", f_vector->data[i].ind_retun_row+1, f_vector->data[i].ind_col1+1);
			printExprn(&f_vector->data[i], matrix);
			exit(EXIT_FAILURE);
		}
		//���������� ������� ����� ������� ����������� ��� ���������
		f_vector->data[i].length_dependency = calculateLengthDependecy(&f_vector->data[i], matrix, 0);
	}
}

////������� ��� ������� ������� � ������� ������ ������
void dbg_print_vector_formula(Vector_formula* f_vector) {
	for (int i = 0; i < f_vector->size; i++) {
		dbg_print_Formula(&f_vector->data[i]);
		printf("\n");
	}
}

//������� ���������� ������� ������ �� ����� ������� ����������� (�� �����������)
void sortVectorFormula(Vector_formula* f_vector) {
	for (int i = 0; i < f_vector->size; i++) {
		for (int j = 0; j < f_vector->size - 1; j++) {
			if (f_vector->data[j].length_dependency > f_vector->data[j + 1].length_dependency) {
				struct FormulaType temp = f_vector->data[j];
				f_vector->data[j] = f_vector->data[j + 1];
				f_vector->data[j + 1] = temp;
			}
		}
	}
}


//������� ������������ ���������� �������
int calcFormula(struct FormulaType* formula, const struct MatrixElement** const matrix) {
	int arg1 = 0, arg2 = 0;
	if (formula->argOneIsNumber == true) arg1 = formula->valueNumberArg1;
	else arg1 = *(int*)matrix[formula->ind_row1][formula->ind_col1].ptr;

	if (formula->argTwoIsNumber == true) arg2 = formula->valueNumberArg2;
	else arg2 = *(int*)matrix[formula->ind_row2][formula->ind_col2].ptr;

	int result = 0;
	switch (formula->operation) {
		case '+':
			result = arg1 + arg2;
			break;
		case '-':
			result = arg1 - arg2;
			break;
		case '*':
			result = arg1 * arg2;
			break;
		case '/':
			//�������� ������� �� ����
			if (arg2 == 0) {
				fprintf(stderr,"Error: division by zero\n");
				exit(EXIT_FAILURE);   //���� ��� �������
			}
			result = arg1 / arg2;
			break;
	}
	return result;
}

//������� �������������� ������� �� ������� f_vector � ������������ ��������� � ������� matrix
void runCalcFormulaInVector(const Vector_formula* const f_vector, struct MatrixElement** matrix) {
	for (int i = 0; i < f_vector->size; i++) {
		int result = calcFormula(&f_vector->data[i], matrix);
		matrix[f_vector->data[i].ind_retun_row][f_vector->data[i].ind_retun_col].pointerType = pInt;
		//�������� ������ ��� ���������
		matrix[f_vector->data[i].ind_retun_row][f_vector->data[i].ind_retun_col].ptr = malloc(sizeof(int));
		if(matrix[f_vector->data[i].ind_retun_row][f_vector->data[i].ind_retun_col].ptr == NULL) {
			fprintf(stderr, "Memory allocation error\n");
			exit(EXIT_FAILURE);
		}
		*(int*)matrix[f_vector->data[i].ind_retun_row][f_vector->data[i].ind_retun_col].ptr = result;
	}
}

//������� ������������ ������ ������� � ����
void writeMatrixToFile(const char* filename, const int rowCount, const int colCount, const struct MatrixElement** const matrix) {
	FILE* file = fopen(filename, "w");
	if (file == NULL) {
		fprintf(stderr, "The output file could not be opened\n");
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < rowCount; i++) {
		for (int j = 0; j < colCount; j++) {
			if (matrix[i][j].pointerType == pChar) {
				fprintf(file, "%s", (char*)matrix[i][j].ptr);
			}
			else if (matrix[i][j].pointerType == pInt) {
				fprintf(file, "%d", *(int*)matrix[i][j].ptr);
			}
			else if (matrix[i][j].pointerType == pFormula) {
				fprintf(file, "*F");
			}
			if (j != colCount - 1) {
				fprintf(file, ",");
			}
		}
		fprintf(file, "\n");
	}
	fclose(file);
}

int main(int argc, char* argv[]) {
	const char* input_filename = argv[1];
	char* output_filename = NULL;
	bool activate_write_outputfile = false;
	if (argc <=1 || argc >3) {
		fprintf(stderr, "the number of arguments must be in the range from 1 to 2\n");
		exit(EXIT_FAILURE);
	}
	printf("input_filename: %s\n\n", input_filename);
	if (argc == 3) {
		activate_write_outputfile = true;
		output_filename = argv[2];
		printf("output_filename: %s\n\n", output_filename);
	 }

    int rowCount = 0, colCount = 0;

	//����� 1 ������� ���������� ����� � �������� � �����
    countRowsAndColumns(input_filename, &rowCount, &colCount);
    printf("count string's: %d\n", rowCount);
    printf("count column's: %d\n\n", colCount);

	//����� 2 �������� ���������� ������ ��������
    struct MatrixElement** matrix = (struct MatrixElement**)malloc(rowCount * sizeof(struct MatrixElement*));
    for (int i = 0; i < rowCount; i++) {
		matrix[i] = (struct MatrixElement*)malloc(colCount * sizeof(struct MatrixElement));
	}
    initMatrix(rowCount, colCount, matrix);

	//����� 3 �������� ������ ��� ��������� ����������� �������
    Vector_formula f_vector;
    initVector(&f_vector, 10);

    //����� 4 ���������� ���������� ������� �� �����
    loadData(input_filename, rowCount, colCount, matrix, &f_vector);

    //����� 5 ���������� ������� �������� ����� � ��������� FormulaType
    for (int i = 0; i < f_vector.size; i++) 
		calculateFormula(&f_vector.data[i], matrix, rowCount);
	
	//����� 6 ������������� ��������� ������� �� ����� ��������� FormulaType � ������� f_vector
	setPointersMatrixOnVector(&f_vector, matrix);
	 
	//����� 7 ���������� ������� ����� ������� ����������� ��� ������ ������� � ������� f_vector
	runCalcLenDepInVector(&f_vector, (const struct MatrixElement**)matrix);
	//dbg_print_vector_formula(&f_vector);

	//����� 8 ��������� ������ ������ �� ����� ������� ����������� (�� �����������)
	sortVectorFormula(&f_vector);

	//����� 9 ���������� ���������� ������
	runCalcFormulaInVector(&f_vector, matrix);

	//����� 10 ������� ������� � �������
    printMatrix(rowCount, colCount, matrix);
	
	//���������� ������ � �������� ���� ���� �� ������ ������ ����������
	if(activate_write_outputfile) 
		writeMatrixToFile(output_filename, rowCount, colCount, matrix);
	
	//����� 11 ����������� ������
    freeMatrix(rowCount, colCount, matrix);
    freeVector(&f_vector);
    return 0;
}