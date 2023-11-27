# Тестовое задание Yadro
## _C-csv-parser_
Написанная на языке "С" программа, читает произвольную CSV-форму из файла (количество строк и столбцов может быть любым), вычисляет значения ячеек, выводит получившуюся табличку в виде CSV-представления в консоль (и в файл, если он указан в аргментах). 

### Запуск программы
Запуск производится из консоли. Первый аргумент имя входного .csv файла с таблицей, второй аргумент (необязательный) имя выходного файла, в который будет записан результат работы программы.
Пример:
```
csv_parser.exe testFile.csv outputFile.csv
```
### Сборка из исходных файлов
#### Windows (ручная сборка)
В Windows для компиляции программы на языке C с использованием компилятора GCC, вам потребуется установить MinGW (Minimalist GNU for Windows) или другую подобную среду разработки, которая включает GCC. Вот шаги:
- Установите MinGW:
	- Скачайте установщик MinGW с официального сайта: https://www.mingw-w64.org/downloads/#mingw-builds
	- Запустите установщик и следуйте инструкциям по установке.
- Добавьте путь к компилятору в переменную PATH:
  	- Найдите путь к каталогу bin в установленной директории MinGW (например, C:\MinGW\bin).
	- Добавьте этот путь в переменную PATH.
- Откройте командную строку:
  	- Нажмите Win + R, введите cmd, и нажмите Enter.
- Перейдите в каталог с вашими исходными файлами программы
  	- Используйте команду cd, чтобы перейти в каталог с вашими файлами. Например:
```
cd путь\к\каталогу\csv_parser\src
```
- Выполните компиляцию:
-- Используйте gcc для компиляции. Например:
```
gcc -o csv_parser main.c vector.c -O3
```
- Запустите исполняемый файл:
-- Введите имя созданного исполняемого файла с расширением .exe:
```
csv_parser.exe [входной_файл.csv] [выходной_файл.csv]
```
#### Windows (сборка с помощью make)
- Убедитесь что MinGW корректно установлен и добавлен в path.
- Запустите консоль и перейдите в каталог проекта
- Выполните команду 'make' в командной строке. В результате в папке bin появится скомпилированный исполняемый файл

## Про логику работы программы
1. Производится подсчет кол-ва строк и столбцов в файле.
   Строки определяются по '\n' и EOF.
   Стобцы по количеству запятых в строке +1
3. Выделение динамического массива структур.  
	- структура содержит:
    	- enum pType pointerType;
    	- void* ptr;
    	
где перечисление pType описывает тип хранимых данных по указателю: null, pChar, pInt, pFormula
Стобцы первой строки описываются "си-строками"
Числа в остальных столбцах и строках - int

- Выражения описываются структурой FormulaType
	- содержит в себе:
		- индексы первого аргумента
		- индексы второго аргумента
		- оператор (действие)
		- значение строки (мы не можем знать ее индекс в двухмерном массиве на этапе заполенения)
		- индексы возврата расчитанного значения (ячейки в которую было вписано выражение)
		- длина цепочки зависимости (сколько формул предстоит решить до решения данной)

3. Выделение вектора под структуры описывающие формулы
4. Заполнение двухмерного массива и вектора выражений (формул) из файла
5. Просчет индексов строк в структурах вектора
6. Установка указателей в матрице на соответствующие элементы вектора
7. Измерение длины цепочки зависимостей
8. Сортировка вектора структур по увеличению длины цепочки зависимостей
9. Вычисление формул, запись результатов в матрицу
10. Вывод решеной матрицы в консоль и в файл
11. Освобождение памяти

В ходе работы программы производится проверка на:
- количество полей в каждой строке
- пустые строки
- лишние пробелы в полях и выражениях (игнорируются)
- проверяется корректность указанных аргументов в выражениях
- ссылка выражения на самого себя
- правильность порядка символов в аргументе (за цифрой не должно следовать букв)
- Первый столбец первой строки должен быть пустым
- Ошибочную запись символа в поле с числом
