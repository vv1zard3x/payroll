
#include <locale.h> //подключение библиотеки для локализации на русский язык
#include <stdlib.h> //подключение стандартной библиотеки
#include <stdio.h> //подключение стандартной библиотеки ввода/вывода

#define FILE_NAME "data" //имя открываемого файла (указывать в кавычках)
#define MAX_LENGTH_NAME 100 //максимально возможная длина имени
#define MAX_LENGTH_SURNAME 100 //максимально возможная длина фамилии
#define MAX_LENGTH_PARTRONYMIC 100 //максимально возможная длина отчества
#define MAX_LENGTH_PERSONNEL_NUMBER 100 //максимально возможная длина персонального номера
#define MAX_COUNT_PERSONS 100 //максимально возможное количество работников

#define ERROR_SAVE_FILE "Ошибка чтения файла"

int COUNT_PERSONS = 0; //количество работников (сколько было в файле и сколько добавили)

//структура работника
struct Person{
    char name[MAX_LENGTH_NAME + 1]; //имя
    char surname[MAX_LENGTH_SURNAME + 1]; //фамилия
    char patronymic[MAX_LENGTH_PARTRONYMIC + 1]; //отчество
    char presonnel_number[MAX_LENGTH_PERSONNEL_NUMBER + 1]; //персональный номер
    double hours_worked; //количество отработанных часов
    double overtime_worked; //количество отработанных часов сверхурочно
    bool dependent; //инждевенец он или нет
}persons[MAX_COUNT_PERSONS]; //массив из максимального количества работников


//функция чтения файла (возвращает количество прочитанных работников)
int read_file(const char* name){
    FILE* in = NULL;
    int count = 0;
    in = fopen(name, "rb");
    if(!in){
        return 0;
    }
    while(feof(in)){
        fread(&persons[count++], sizeof(Person), 1, in);
    }
    fclose(in);
    return count;
}

//сохранение всех данных в файл
void save_file(const char* name){
    FILE* out = NULL;
    out = fopen(name, "wb");
    if(!out){
        throw ERROR_SAVE_FILE;
    }
    fwrite(persons, sizeof(Person), COUNT_PERSONS, out);
    fclose(out);
}

//основная исполняемая функция
int main(){
    setlocale(LC_ALL, "ru");
    try {
        read_file(FILE_NAME);
        
        
        
        save_file(FILE_NAME);
    }
    catch (const char* error){
        printf(error);
    }
    catch (...) {
        printf("Что-то пошло не так :(\n");
        exit(EXIT_SUCCESS);
    }
    return 0;
}


//⣿⣿⣿⣻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
//⣿⣿⣿⣵⣿⣿⣿⠿⡟⣛⣧⣿⣯⣿⣝⡻⢿⣿⣿⣿⣿⣿⣿⣿
//⣿⣿⣿⣿⣿⠋⠁⣴⣶⣿⣿⣿⣿⣿⣿⣿⣦⣍⢿⣿⣿⣿⣿⣿
//⣿⣿⣿⣿⢷⠄⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣏⢼⣿⣿⣿⣿
//⢹⣿⣿⢻⠎⠔⣛⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡏⣿⣿⣿⣿
//⢸⣿⣿⠇⡶⠄⣿⣿⠿⠟⡛⠛⠻⣿⡿⠿⠿⣿⣗⢣⣿⣿⣿⣿
//⠐⣿⣿⡿⣷⣾⣿⣿⣿⣾⣶⣶⣶⣿⣁⣔⣤⣀⣼⢲⣿⣿⣿⣿
//⠄⣿⣿⣿⣿⣾⣟⣿⣿⣿⣿⣿⣿⣿⡿⣿⣿⣿⢟⣾⣿⣿⣿⣿ ----  НА УДАЧУ
//⠄⣟⣿⣿⣿⡷⣿⣿⣿⣿⣿⣮⣽⠛⢻⣽⣿⡇⣾⣿⣿⣿⣿⣿
//⠄⢻⣿⣿⣿⡷⠻⢻⡻⣯⣝⢿⣟⣛⣛⣛⠝⢻⣿⣿⣿⣿⣿⣿
//⠄⠸⣿⣿⡟⣹⣦⠄⠋⠻⢿⣶⣶⣶⡾⠃⡂⢾⣿⣿⣿⣿⣿⣿
//⠄⠄⠟⠋⠄⢻⣿⣧⣲⡀⡀⠄⠉⠱⣠⣾⡇⠄⠉⠛⢿⣿⣿⣿
//⠄⠄⠄⠄⠄⠈⣿⣿⣿⣷⣿⣿⢾⣾⣿⣿⣇⠄⠄⠄⠄⠄⠉⠉
//⠄⠄⠄⠄⠄⠄⠸⣿⣿⠟⠃⠄⠄⢈⣻⣿⣿⠄⠄⠄⠄⠄⠄
//⠄⠄⠄⠄⠄⠄⠄⠸⣿⣿⠃⠄⠈⢿⣿⣿⠄⠄⠄⠄⠄⠄⠄        НУ И ГУГЛ В ПОМОЩЬ
