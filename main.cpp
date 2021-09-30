#include <iostream>
#include <vector>
#include <fstream>

/*########################################################################################*/
/*################################# Глобальные переменные ################################*/
/*########################################################################################*/
#define clear system("clear")

const std::string FILE_NAME = "data1";          //имя открываемого файла (указывать в кавычках)
const double TAX = 0.12;                        //подоходный налог (коэффициент)
const double PENSION = 0.01;                    //пенсионные отчисления (коэффициент)
const int MINIMUM_WAGE = 10000;                 //Минимальный размер оплаты труда
const int MINIMUM_DEPENDENT_WAGE = 7000;        //минимальный размер оплаты труда для инждевенцев
double all_tax;
double all_pension;
/*########################################################################################*/
/*########################################################################################*/


/*########################################################################################*/
/*##################################### Структуры ########################################*/
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
    double total_time() const;
};

struct List{
private:
    struct Item{
        Person person;
        Item* next;
    };
    Item* start;
public:
    List();
    List(const Person& person);
    void insert(const Person& person);
    void show() const;
};

struct Tree{
private:
    struct Node{
        Person person;
        Node* left, * right;
    };
    Node* root;
    void insert(Node** root, const Person& person);
    void show(Node* root) const;
public:
    Tree();
    Tree(const Person& person);
    void insert(const Person& person);
    void show() const;
};

/*########################################################################################*/
/*########################################################################################*/


/*########################################################################################*/
/*############################### Прототипы функций ######################################*/
/*########################################################################################*/
size_t count_persons_in_file(std::ifstream& file);
void read_file(std::vector<Person>& persons, List& list, Tree& tree);
void save_file(std::vector<Person>& persons);
void add_person(std::vector<Person>& persons, List& list, Tree& tree);
void show_all_persons(std::vector<Person>& persons);
std::vector<Person>::iterator find(std::vector<Person>& persons, Person& person);
std::vector<Person>::iterator find(std::vector<Person>& persons, std::string& number);
void show_definite_person(std::vector<Person>& persons);
void print_menu();
bool menu(std::vector<Person>& persons, List& list, Tree& tree);
void qs(int left, int right, std::vector<Person>& persons);
void quicksort(std::vector<Person>& persons);
std::vector<Person>::iterator binary_search(std::vector<Person>& persons, Person& person);
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
        List list; Tree tree;
        read_file(persons, list, tree);
        while (menu(persons, list, tree));
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
/*################################### Методы структур ####################################*/
/*########################################################################################*/

//посчитать налоги
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

//вывести информацию о работнике на экран
void Person::report() const{
    std::cout << "-----------------------------------------------------" << std::endl;
    std::cout << "ФИО: " << this->surname << " " << this->name << " " << this->patronymic << std::endl;
    std::cout << "Персональный номер: " << this->personnel_number << std::endl;
    printf("Количество отработанных часов: %.2lf\n", this->hours_worked);
    printf("Количество cверхурочных часов: %.2lf\n", this->overtime_worked);
    printf("Иждевенец: %s\n", this->dependent ? "да" : "нет");
    printf("Зарплата (руб/час): %.2lf\n", this->salary);
    printf("Пенсионное отчисление: %.2lf\n", this->pension);
    printf("Подоходный налог : %.2lf\n", this->tax > 0 ? this->tax : 0.0);
    printf("Итоговая зарплата: %.2lf\n", total_salary );
    std::cout << "-----------------------------------------------------" << std::endl;
}

double Person::total_time() const{
    return this->hours_worked + this->total_time();
}

List::List() : start(nullptr){}

List::List(const Person& person){
    start = new Item{person, nullptr};
};

void List::insert(const Person& person){
    if(this->start == nullptr){
        this->start = new Item{person, nullptr};
        return;
    }
    Item* current = start;
    Item* buffer = nullptr;
    Item* previous = nullptr;
    while (current != nullptr) {
        if( person.total_time() >= current->person.total_time()){
            buffer = new Item{person, current};
            previous == nullptr ? start = buffer : previous->next = buffer;
            return;
        }
        previous = current;
        current = current->next;
    }
    if(current == nullptr){
        current = new Item{person, nullptr};
        if(previous != nullptr){
            previous->next = current;
        }
        
    }
}

void List::show() const{
    Item* current = start;
    while(current != nullptr){
        current->person.report();
        current = current->next;
    }
}

Tree::Tree() : root(nullptr){}

Tree::Tree(const Person& person) {
    root = new Node{person, nullptr, nullptr};
}

void Tree::insert(Node** root, const Person& person){
    if(!*root){
        *root = new Node{person, NULL, NULL};
        return;
    }
    if(person.overtime_worked && person.overtime_worked > (*root)->person.overtime_worked){
        insert(&(*root)->left, person);
    }
    else if(person.overtime_worked){
        insert(&(*root)->right, person);
    }
}

void Tree::insert(const Person& person) {
    this->insert(&root, person);
}

void Tree::show(Node *root) const{
    if(root == nullptr)
           return;
    if(root->left != nullptr){
       show(root->left);
    }
    if(root->right != nullptr){
       show(root->right);
    }
    root->person.report();
}

void Tree::show() const{
    this->show(this->root);
}


/*########################################################################################*/
/*########################################################################################*/


/*########################################################################################*/
/*############################# Основные фукнции в программе #############################*/
/*########################################################################################*/

//реализация меню
bool menu(std::vector<Person>& persons, List& list, Tree& tree){
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
            add_person(persons, list, tree);
            std::cin.ignore();
            break;
        case '4':
            clear;
            tree.show();
            std::cin.ignore();
            std::cin.get();
            break;
        case '5':
            clear;
            list.show();
            std::cin.ignore();
            std::cin.get();
            break;
        case '6':
            clear;
            return false;
            break;
        default:
            break;
    }
    return true;
}

//вывести меню на экран
void print_menu(){
    printf("1. Показать всех сотрудников\n");
    printf("2. Найти сотрудника\n");
    printf("3. Добавить сотрудника\n");
    printf("4. Показать бинарное дерево\n");
    printf("5. Показать линейный список\n");
    printf("6. Выйти\n");
}

//показать всех работников
void show_all_persons(std::vector<Person>& persons){
    for(auto &person : persons){
        person.report();
    }
    printf("Общий подоходный налог: %.2lf\n", all_tax);
    printf("Общие пенсионные отчисления: %.2lf\n", all_pension);
}

//найти определенных работников
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
                std::cin.ignore();
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
                std::cin.ignore();
                std::cin.get();
                return;
                break;
            default:
                break;
        }
    }
}

//найти работника по фамилии
std::vector<Person>::iterator find(std::vector<Person>& persons, Person& person){
    if(persons.empty()){
        return persons.end();
    }
    return binary_search(persons, person);
}

//найти работника по номеру
std::vector<Person>::iterator find(std::vector<Person>& persons, std::string& number){
    return std::find_if(persons.begin(), persons.end(), [&](Person &p)->bool{
        return p.personnel_number == number;
    });
}

//бинарный поиск по фамилии
std::vector<Person>::iterator binary_search(std::vector<Person>& persons, Person& person){
    int middle, left = 0, right = persons.size() - 1;
    while(true){
        middle = (left + right) / 2;
        if(strcmp(person.surname, persons[middle].surname) < 0){
            right = middle - 1;
        }
        else if(strcmp(person.surname, persons[middle].surname) > 0){
            left = middle + 1;
        }
        else{
            std::vector<Person>::iterator result = persons.begin() + middle;
            return result;
        }
        
        if(left > right){
            return persons.end();
        }
    }
}

//доавбить нового работника
void add_person(std::vector<Person>& persons, List& list, Tree& tree){
    Person person;
    std::cin.ignore();
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
    list.insert(person);
    tree.insert(person);
    quicksort(persons);
}

//быстрая сотировка
void qs(int left, int right, std::vector<Person>& persons){
    int i = left, j = right;
    Person x = persons[(left+ right)/2];
    do{
        while(strcmp(persons[i].surname, x.surname) < 0) ++i;
        while(strcmp(x.surname, persons[j].surname) < 0) --j;
        if(i <=j){
            Person y = persons[i];
            persons[i] = persons[j];
            persons[j] = y;
            ++i; --j;
        }
    }while(i <=j);
    if(left < j) qs(left, j, persons);
    if(i < right) qs(i, right, persons);
}

//вызов быстрой сортировки
void quicksort(std::vector<Person>& persons){
    if(persons.size() > 1){
        qs(0, persons.size()-1, persons);
    }
}

/*########################################################################################*/
/*########################################################################################*/


/*########################################################################################*/
/*################################### Работа с файлами ###################################*/
/*########################################################################################*/
//количество записей о работниках в файле
size_t count_persons_in_file(std::ifstream& file){
    file.seekg(0, file.end);
    size_t length = file.tellg();
    file.seekg(0, file.beg);
    return length / sizeof(Person);
}

//прочитать данные из файла
void read_file(std::vector<Person>& persons, List& list, Tree& tree){
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
            list.insert(person);
            tree.insert(person);
        }
    }
    quicksort(persons);
    file.close();
}

//записать данные в файл
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

//⣿⣿⣿⣻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
//⣿⣿⣿⣵⣿⣿⣿⠿⡟⣛⣧⣿⣯⣿⣝⡻⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣵⣿⣿⣿⠿⡟⣛⣧⣿⣯⣿⣝⡻⢿⣿⣿⣿⣿⣿⣿⣿
//⣿⣿⣿⣿⣿⠋⠁⣴⣶⣿⣿⣿⣿⣿⣿⣿⣦⣍⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠋⠁⣴⣶⣿⣿⣿⣿⣿⣿⣿⣦⣍⢿⣿⣿⣿⣿⣿
//⣿⣿⣿⣿⢷⠄⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣏⢼⣿⣿⣿⣿⣿⣿⣿⣿⢷⠄⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣏⢼⣿⣿⣿⣿
//⢹⣿⣿⢻⠎⠔⣛⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡏⣿⣿⣿⣿⢹⣿⣿⢻⠎⠔⣛⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡏⣿⣿⣿⣿
//⢸⣿⣿⠇⡶⠄⣿⣿⠿⠟⡛⠛⠻⣿⡿⠿⠿⣿⣗⢣⣿⣿⣿⣿⢸⣿⣿⠇⡶⠄⣿⣿⠿⠟⡛⠛⠻⣿⡿⠿⠿⣿⣗⢣⣿⣿⣿⣿
//⠐⣿⣿⡿⣷⣾⣿⣿⣿⣾⣶⣶⣶⣿⣁⣔⣤⣀⣼⢲⣿⣿⣿⣿⠐⣿⣿⡿⣷⣾⣿⣿⣿⣾⣶⣶⣶⣿⣁⣔⣤⣀⣼⢲⣿⣿⣿⣿
//⠄⣿⣿⣿⣿⣾⣟⣿⣿⣿⣿⣿⣿⣿⡿⣿⣿⣿⢟⣾⣿⣿⣿⣿ ⠄⣿⣿⣿⣿⣾⣟⣿⣿⣿⣿⣿⣿⣿⡿⣿⣿⣿⢟⣾⣿⣿⣿⣿----  НА УДАЧУ
//⠄⣟⣿⣿⣿⡷⣿⣿⣿⣿⣿⣮⣽⠛⢻⣽⣿⡇⣾⣿⣿⣿⣿⣿⠄⣟⣿⣿⣿⡷⣿⣿⣿⣿⣿⣮⣽⠛⢻⣽⣿⡇⣾⣿⣿⣿⣿⣿
//⠄⢻⣿⣿⣿⡷⠻⢻⡻⣯⣝⢿⣟⣛⣛⣛⠝⢻⣿⣿⣿⣿⣿⣿⠄⢻⣿⣿⣿⡷⠻⢻⡻⣯⣝⢿⣟⣛⣛⣛⠝⢻⣿⣿⣿⣿⣿⣿
//⠄⠸⣿⣿⡟⣹⣦⠄⠋⠻⢿⣶⣶⣶⡾⠃⡂⢾⣿⣿⣿⣿⣿⣿⠄⠸⣿⣿⡟⣹⣦⠄⠋⠻⢿⣶⣶⣶⡾⠃⡂⢾⣿⣿⣿⣿⣿⣿
//⠄⠄⠟⠋⠄⢻⣿⣧⣲⡀⡀⠄⠉⠱⣠⣾⡇⠄⠉⠛⢿⣿⣿⣿⠄⠄⠟⠋⠄⢻⣿⣧⣲⡀⡀⠄⠉⠱⣠⣾⡇⠄⠉⠛⢿⣿⣿⣿
//⠄⠄⠄⠄⠄⠈⣿⣿⣿⣷⣿⣿⢾⣾⣿⣿⣇⠄⠄⠄⠄⠄⠉⠉⠄⠄⠄⠄⠄⠈⣿⣿⣿⣷⣿⣿⢾⣾⣿⣿⣇⠄⠄⠄⠄⠄⠉⠉
//⠄⠄⠄⠄⠄⠄⠸⣿⣿⠟⠃⠄⠄⢈⣻⣿⣿⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠸⣿⣿⠟⠃⠄⠄⢈⣻⣿⣿⠄⠄⠄⠄⠄⠄
//⠄⠄⠄⠄⠄⠄⠄⠸⣿⣿⠃⠄⠈⢿⣿⣿⠄⠄⠄⠄⠄⠄⠄ ⠄⠄⠄⠄⠄⠄⠄⠸⣿⣿⠃⠄⠈⢿⣿⣿⠄⠄⠄⠄⠄⠄⠄       НУ И ГУГЛ В ПОМОЩЬ
