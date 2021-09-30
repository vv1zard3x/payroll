#include "person.hpp"

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
