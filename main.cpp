#include <iostream>
#include <vector>
#include <fstream>

/*########################################################################################*/
/*################################# Глобальные переменные ################################*/
/*########################################################################################*/
#define clear system("clear")

const std::string FILE_NAME = "data";          //имя открываемого файла (указывать в кавычках)
const double TAX = 0.12;                        //подоходный налог (коэффициент)
const double PENSION = 0.01;                    //пенсионные отчисления (коэффициент)
const int MINIMUM_WAGE = 10000;                 //Минимальный размер оплаты труда
const int MINIMUM_DEPENDENT_WAGE = 7000;        //минимальный размер оплаты труда для инждевенцев
double all_tax;
double all_pension;
/*########################################################################################*/
/*########################################################################################*/


/*########################################################################################*/
/*########################### Прототипы структуры и функций ##############################*/
/*########################################################################################*/
struct Person;
size_t count_persons_in_file(std::ifstream& file);
void read_file(std::vector<Person>& persons);
void save_file(std::vector<Person>& persons);
void add_person(std::vector<Person>& persons);
void show_all_persons(std::vector<Person>& persons);
std::vector<Person>::iterator find(std::vector<Person>& persons, Person& person);
std::vector<Person>::iterator find(std::vector<Person>& persons, std::string& number);
void show_definite_person(std::vector<Person>& persons);
void print_menu();
bool menu(std::vector<Person>& persons);
/*########################################################################################*/
/*########################################################################################*/



/*########################################################################################*/
/*################################## САМА ПРОГРАММА ######################################*/
/*########################################################################################*/
int main(){
    setlocale(LC_ALL, "ru");
    all_tax = 0.0;
    all_pension = 0.0;
    try {
        std::vector<Person> persons;
        read_file(persons);
        while (menu(persons));
        save_file(persons);
    }
    catch(std::exception& ex){
        ex.what();
    }
    catch (...) {
        printf("Что-то пошло не так :(\n");
        exit(EXIT_SUCCESS);
    }
    return 0;
}


/*########################################################################################*/
/*################################ Структура и ее методы ################################*/
/*########################################################################################*/
struct Person{
    /*FILEDS*/
    char name[100], surname[100], patronymic[100];
    char personnel_number[100];
    double hours_worked;
    double overtime_worked;
    bool dependent;
    double salary, total_salary, pension, tax;
    
    /*METHODS*/
    void calc_tax();
    void report() const;
};

void Person::calc_tax(){
    total_salary = salary * (hours_worked + 1.5 * overtime_worked);
    pension = total_salary * PENSION;
    if(dependent){
        tax =(total_salary - MINIMUM_WAGE - MINIMUM_DEPENDENT_WAGE) * TAX;
    }
    else{
        tax = (total_salary - MINIMUM_WAGE) * TAX ;
    }
    total_salary -= (tax + pension);
    all_tax += tax; all_pension += pension;
}

void Person::report() const{
    std::cout << "ФИО: " << this->surname << " " << this->name << " " << this->patronymic << std::endl;
    std::cout << "Персональный номер: " << this->personnel_number << std::endl;
    printf("Количество отработанных часов: %.2lf\n", this->hours_worked);
    printf("Количество cверхурочных часов: %.2lf\n", this->overtime_worked);
    printf("Иждевенец: %s\n", this->dependent ? "да" : "нет");
    printf("Зарплата (руб/час): %.2lf\n", this->salary);
    printf("Пенсионное отчисление: %.2lf\n", this->pension);
    printf("Подоходный налог : %.2lf\n", this->tax > 0 ? this->tax : 0.0);
    printf("Итоговая зарплата: %.2lf\n", total_salary );
}
/*########################################################################################*/
/*########################################################################################*/


/*########################################################################################*/
/*############################# Основные фукнции в программе #############################*/
/*########################################################################################*/
bool menu(std::vector<Person>& persons){
    clear;
    print_menu();
    char key;
    std::cin >> key;
    switch (key) {
        case '1':
            clear;
            show_all_persons(persons);
            std::cin.ignore();
            std::cin.get();
            break;
        case '2':
            clear;
            show_definite_person(persons);
            std::cin.ignore();
            std::cin.get();
            break;
        case '3':
            clear;
            add_person(persons);
            break;
        case '4':
            clear;
            return false;
            break;
        default:
            break;
    }
    return true;
}

void print_menu(){
    printf("1. Показать всех сотрудников\n");
    printf("2. Найти сотрудника\n");
    printf("3. Добавить сотрудника\n");
    printf("4. Выйти\n");
}

void show_all_persons(std::vector<Person>& persons){
    for(auto &person : persons){
        std::cout << "-----------------------------------------------------" << std::endl;
        person.report();
        std::cout << "-----------------------------------------------------" << std::endl;
    }
    printf("Общий подоходный налог: %.2lf\n", all_tax);
    printf("Общие пенсионные отчисления: %.2lf\n", all_pension);
}

void show_definite_person(std::vector<Person>& persons){
    char key;
    std::string number;
    Person person;
    std::vector<Person>::iterator result;
    while(true){
        printf("(Важно!) Символы нужно вводить точь в точь в таком же регистре как записаны работники.\n\n");
        printf("1. Найти работника по персональному номеру\n");
        printf("2. Найти работника по фамилии и инициалам\n");
        std::cin >> key;
        clear;
        switch (key) {
            case '1':
                printf("Персональный номер : ");
                std::cin >> number;
                result = find(persons, number);
                if(result != persons.end()){
                    result->report();
                }
                printf("Нажмите Enter, чтобы продолжить\n");
                std::cin.get();
                return;
                break;
            case '2':
                printf("Фамилия и инициалы: ");
                std::cin >> person.surname >> person.name >> person.patronymic;
                result = find(persons, person);
                if(result != persons.end()){
                    result->report();
                }
                printf("Нажмите Enter, чтобы продолжить\n");
                std::cin.get();
                return;
                break;
            default:
                break;
        }
    }
}

std::vector<Person>::iterator find(std::vector<Person>& persons, Person& person){
    return std::find_if(persons.begin(), persons.end(), [&](Person &p)->bool{
        return person.surname == p.surname &&
        person.name[0] == p.name[0] &&
        person.patronymic[0] == p.patronymic[0];
    });
}

std::vector<Person>::iterator find(std::vector<Person>& persons, std::string& number){
    return std::find_if(persons.begin(), persons.end(), [&](Person &p)->bool{
        return p.personnel_number == number;
    });
}

void add_person(std::vector<Person>& persons){
    Person person;
    std::string dependent;
    std::cout << "ФИО: ";
    std::cin >> person.surname >> person.name >> person.patronymic;
    std::cout << "Gерсональный номер: ";
    std::cin >> person.personnel_number;
    std::cout << "Отработанные часы: ";
    std::cin >> person.hours_worked;
    std::cout << "Отработанные сверхурочные часы: ";
    std::cin >> person.overtime_worked;
    std::cout << "Иждевенец (yes/no): ";
    std::cin >> dependent;
    person.dependent = dependent == "yes" ? true : false;
    std::cout << "Зарплата (руб/час): ";
    std::cin >> person.salary;
    person.calc_tax();
    persons.push_back(person);
}
/*########################################################################################*/
/*########################################################################################*/


/*########################################################################################*/
/*################################### Работа с файлами ###################################*/
/*########################################################################################*/
size_t count_persons_in_file(std::ifstream& file){
    file.seekg(0, file.end);
    size_t length = file.tellg();
    file.seekg(0, file.beg);
    return length / sizeof(Person);
}

void read_file(std::vector<Person>& persons){
    std::ifstream file;
    file.open(FILE_NAME, std::ios::binary);
    Person person;
    if(file.is_open())
    {
        size_t count = count_persons_in_file(file);
        for (size_t i = 0; i < count; ++i) {
            file.read((char*)&person, sizeof(Person));
            person.calc_tax();
            persons.push_back(person);
        }
    }
    file.close();
}

void save_file(std::vector<Person>& persons){
    std::ofstream file(FILE_NAME, std::ios::binary | std::ios::trunc);
    if(not file.is_open()){
        std::cout << "Ошибка записи в файл :(" << std::endl;
    }
    for(auto item : persons){
        file.write((char*)&item, sizeof(Person));
    }
    file.close();
}
/*########################################################################################*/
/*########################################################################################*/
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
