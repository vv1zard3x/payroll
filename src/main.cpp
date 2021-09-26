
#include <locale.h> //подключение библиотеки для локализации на русский язык
#include <stdlib.h> //подключение стандартной библиотеки
#include <stdio.h> //подключение стандартной библиотеки ввода/вывода
#include <string.h> //подключение бибилиотеки для работы со строками

const char* FILE_NAME = "data"; //имя открываемого файла (указывать в кавычках)
const int MAX_COUNT_PERSONS  = 100; //максимально возможное количество работников
const double TAX = 0.12; //подоходный налог
const double PENSION = 0.01; //пенсионные отчисления
const double MINIMUM_WAGE = 10000; //Минимальный размер оплаты труда
const double MINIMUM_DEPENDENT_WAGE = 7000; //минимальный размер оплаты труда для инждевенцев

const char* ERROR_SAVE_FILE = "Ошибка чтения файла";

#define clear system("clear")

int COUNT_PERSONS = 0; //количество работников (сколько было в файле и сколько добавили)
double all_tax = 0, all_pension = 0;

//структура работника
struct Person{
    char name[100]; //имя
    char surname[100]; //фамилия
    char patronymic[100]; //отчество
    char personnel_number[100]; //персональный номер
    double hours_worked; //количество отработанных часов
    double overtime_worked; //количество отработанных часов сверхурочно
    bool dependent; //инждевенец он или нет
    double salary;
}persons[MAX_COUNT_PERSONS]; //массив из максимального количества работников

struct List{
    Person* person;
    List* next;
} *list = NULL;

struct Tree{
    Person* person;
    Tree* left, * right;
} *tree = NULL;

void push_to_tree(Person* person, Tree** position){
    if(!*position){
        *position = new Tree{person, NULL, NULL};
        return;
    }
    if(person->overtime_worked && person->overtime_worked > (*position)->person->overtime_worked){
        push_to_tree(person, &(*position)->left);
    }
    else if(person->overtime_worked){
        push_to_tree(person, &(*position)->right);
    }
}

void push_to_list(Person *person){
    List* current = list, *buffer = NULL, *previous = NULL;
    if(!list){
        list = new List{person, NULL};
        return;
    }
    else{
        bool ready = false;
        while(current != NULL){
            if(person->hours_worked + person->overtime_worked >=
               current->person->overtime_worked + current->person->hours_worked){
                buffer = new List{person, current};
                if(!previous){
                    list = buffer;
                }
                else{
                    previous->next = buffer;
                }
                return;
            }
            else{
                
            }
            previous = current;
            current = current->next;
            
        }
        if(current == NULL){
            current = new List{person, NULL};
            if(previous){
                previous->next = current;
            }
        }
    }
}

//функция чтения файла (возвращает количество прочитанных работников)
int read_file(const char* name){
    FILE* in = NULL; //создаем указатель на файл
    COUNT_PERSONS = 0;
    in = fopen(name, "r+b");//открываем файл
    if(!in){
        return 0;
    }
    
    //читаем все записи из файла
    while(!feof(in)){
        if(fread(&persons[COUNT_PERSONS], sizeof(Person), 1, in)){
            push_to_list(&persons[COUNT_PERSONS]);
            push_to_tree(&persons[COUNT_PERSONS], &tree);
            COUNT_PERSONS++;
        }
    }
    fclose(in);//закрываем файл
    return COUNT_PERSONS;
}

//сохранение всех данных в файл
void save_file(const char* name){
    FILE* out = NULL;
    out = fopen(name, "w+b");
    if(!out){
        throw ERROR_SAVE_FILE;//если не удалось открыть файл для записи, выбрасываем ошибку
    }
    
    //иначе записываем все данные из массива в файл
    for (int i = 0; i < COUNT_PERSONS; ++i) {
        fwrite(&persons[i], sizeof(Person), 1, out);
    }
    fclose(out);
}


//добавить работника (возвращает true или false в зависимости от того, удалось ли добавить сотрудника)
bool add_person(){
    if(COUNT_PERSONS == MAX_COUNT_PERSONS){
        return false;
    }
    else{
        fflush(stdout);//это значит очистить буфер
        getchar();
        printf("Имя: ");
        gets(persons[COUNT_PERSONS].name);
        fflush(stdout);
        printf("фамилия: ");
        gets(persons[COUNT_PERSONS].surname);
        fflush(stdout);
        printf("Отчество: ");
        gets(persons[COUNT_PERSONS].patronymic);
        printf("Персональный номер: ");
        gets(persons[COUNT_PERSONS].personnel_number);
        printf("Количество отработанных часов: ");
        scanf("%lf", &persons[COUNT_PERSONS].hours_worked);
        printf("Количество cверхурочных часов: ");
        scanf("%lf", &persons[COUNT_PERSONS].overtime_worked);
        fflush(stdin);
        printf("Иждевенец (yes/no) : ");
        char dependent[100];
        gets(dependent);
        !strcmp(dependent, "yes") ? persons[COUNT_PERSONS].dependent = true : persons[COUNT_PERSONS].dependent = false;
        printf("Зарплата (руб/час): ");
        scanf("%lf", &persons[COUNT_PERSONS].salary);
        push_to_list(&persons[COUNT_PERSONS]);
        push_to_tree(&persons[COUNT_PERSONS], &tree);
        ++COUNT_PERSONS;
    }
    return true;
}

void report(const Person* person){
    if(!person){
        return;
    }
    else{
        double total_salary, pension, tax;
        printf("ФИО:                           %s %s %s\n",
               person->surname,
               person->name,
               person->patronymic);
        printf("Персональный номер:            %s\n", person->personnel_number);
        printf("Количество отработанных часов: %.2lf\n", person->hours_worked);
        printf("Количество cверхурочных часов: %.2lf\n", person->overtime_worked);
        printf("Иждевенец:                     %s\n", person->dependent ? "да" : "нет");
        printf("Зарплата (руб/час):            %.2lf\n", person->salary);
        total_salary = person->salary *( person->hours_worked + 1.5 * person->overtime_worked);
        pension = total_salary * PENSION;
        if(person->dependent){
            tax =(total_salary - MINIMUM_WAGE - MINIMUM_DEPENDENT_WAGE) * TAX;
        }
        else{
            tax = (total_salary - MINIMUM_WAGE) * TAX ;
        }
        printf("Пенсионное отчисление:         %.2lf\n", pension);
        printf("Подоходный налог :             %.2lf\n", tax > 0 ? tax : 0.0);
        all_tax += tax; all_pension += pension;
        printf("Итоговая зарплата:             %.2lf\n", total_salary - tax - pension);
    }
}

void show_all_persons(){
    all_tax = 0, all_pension = 0;
    for(int i = 0; i < COUNT_PERSONS; ++i){
        printf("-----------------------------------------------------\n");
        report(&persons[i]);
        printf("-----------------------------------------------------\n");
    }
    printf("Общий подоходный налог: %.2lf\n", all_tax);
    printf("Общие пенсионные отчисления: %.2lf\n", all_pension);
}

//функция ищет работника по ФИО и возвращает указатель на него
void find(const char* surname, const char* n, const char* p){
    for (int i = 0; i < COUNT_PERSONS; ++i) {
        if(!strcmp(persons[i].surname, surname)){
            if(!strncmp(persons[i].name, n, 1) && !strncmp(persons[i].patronymic, p, 1)){
                report( &persons[i]);
            }
        }
    }
    return NULL;
}

//функция ищет работника по номеру и возвращает указатель на него
void find(const char* personnel_number){
    for (int i = 0; i < COUNT_PERSONS; ++i) {
        if(!strcmp(persons[i].personnel_number, personnel_number)){
            report( &persons[i]);
        }
    }
    return NULL;
}

void show_list(bool (*comp)(Person*) = NULL){
    List* current = list;
    while(current != NULL){
        printf("-----------------------------------------------------\n");
        if(comp){
            if(comp(current->person)){
                report(current->person);
            }
        }
        else{
            report(current->person);
        }
        printf("-----------------------------------------------------\n");
        current = current->next;
    }
    getchar();
}

void show_tree(Tree* tree){
    if(!tree)
        return;
    if(tree->left){
        show_tree(tree->left);
    }
    if(tree->right){
        show_tree(tree->right);
    }
    if(tree->person){
        printf("-----------------------------------------------------\n");
        report(tree->person);
        printf("-----------------------------------------------------\n");
    }
}

void show_definite_person(){
    char request[100], surname[100], n[2], p[2];
    while(true){
        fflush(stdout);
        printf("(Важно!) Символы нужно вводить точь в точь в таком же регистре как записаны работники.\n\n");
        printf("1. Найти работника по персональному номеру\n");
        printf("2. Найти работника по фамилии и инициалам\n");
        char key[2];
        gets(key);
        clear;
        switch (key[0]) {
            case '1':
                printf("Персональный номер : ");
                gets(request);
                find(request);
                printf("Нажмите Enter, чтобы продолжить\n");
                getchar();
                return;
                break;
            case '2':
                printf("Фамилия и инициалы в формате \"Иванов И И\" (без точек через пробелы): ");
                scanf("%s %s %s", surname, n, p);
                find(surname, n, p);
                printf("Нажмите Enter, чтобы продолжить\n");
                getchar();
                return;
                break;
            default:
                break;
        }
    }
}

void print_menu(){
    printf("1. Показать всех сотрудников\n");
    printf("2. Найти сотрудника\n");
    printf("3. Добавить сотрудника\n");
    printf("4. Показать весь линейный список\n");
    printf("5. Показать тех, кто отработал больше нормы\n");
    printf("6. Показать бинарное дерево\n");
    printf("7. Выйти\n");
}

bool menu(){
    clear;
    print_menu();
    char key[2];
    fflush(stdout);
    gets(key);
    switch (key[0]) {
        case '1':
            clear;
            show_all_persons();
            getchar();
            break;
        case '2':
            clear;
            show_definite_person();
            getchar();
            break;
        case '3':
            clear;
            add_person();
            break;
        case '4':
            clear;
            show_list();
            break;
        case '5':
            clear;
            show_list([](Person* p)->bool{
                return p->overtime_worked;
            });
            break;
        case '6':
            clear;
            show_tree(tree);
            getchar();
            break;
        case '7':
            clear;
            return false;
            break;
        default:
            break;
    }
    return true;
}

//основная исполняемая функция
int main(){
    setlocale(LC_ALL, "ru");
    try {
        list = NULL;
        read_file(FILE_NAME);
        while (menu());
        save_file(FILE_NAME);
    }
    catch (const char* error){
        printf("%s", error);
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
