#include <algorithm>
#include <deque>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <utility>
#include <vector>
#include <windows.h>
#include "student.h"

using namespace std;

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    ifstream in("input.txt");
    ofstream out("output.txt");

    if (!in || !out) {
        cout << "Ошибка открытия файлов!" << endl;
        return 1;
    }

    vector<Student> students;
    Student s;

    // Считываем записи из файла в vector
    while (in >> s) {
        students.push_back(s);
    }

    if (!in.eof()) {
        cout << "Ошибка чтения данных из файла!" << endl;
        out << "Ошибка чтения данных из файла!" << endl;
        return 1;
    }

    if (students.empty()) {
        cout << "Входной файл не содержит данных." << endl;
        out << "Входной файл не содержит данных." << endl;
        return 1;
    }

    out << fixed << setprecision(2);

    // Выводим исходный контейнер
    out << "ИСХОДНЫЙ КОНТЕЙНЕР vector:" << endl << endl;
    for (const Student& st : students) {
        out << st << endl;
    }

    // Копируем vector в deque с помощью std::copy
    deque<Student> copiedStudents;
    copy(students.begin(), students.end(), back_inserter(copiedStudents));

    out << "КОПИЯ ИСХОДНОГО КОНТЕЙНЕРА deque:" << endl << endl;
    for (const Student& st : copiedStudents) {
        out << st << endl;
    }

    // Сортируем vector по возрастанию среднего балла.
    // operator< класса Student сравнивает объекты именно по этому полю.
    sort(students.begin(), students.end());

    out << "КОНТЕЙНЕР vector ПОСЛЕ СОРТИРОВКИ:" << endl << endl;
    for (const Student& st : students) {
        out << st << endl;
    }

    // Проверяем конструкторы копирования и перемещения
    Student copyStudent(students[0]);
    out << "ПРОВЕРКА КОНСТРУКТОРА КОПИРОВАНИЯ:" << endl;
    out << copyStudent << endl;

    Student moveStudent(move(copyStudent));
    out << "ПРОВЕРКА КОНСТРУКТОРА ПЕРЕМЕЩЕНИЯ:" << endl;
    out << moveStudent << endl;

    // Проверяем копирующее и перемещающее присваивание
    Student assignedStudent;
    assignedStudent = students[0];
    out << "ПРОВЕРКА КОПИРУЮЩЕГО ПРИСВАИВАНИЯ:" << endl;
    out << assignedStudent << endl;

    Student moveAssignedStudent;
    moveAssignedStudent = move(assignedStudent);
    out << "ПРОВЕРКА ПЕРЕМЕЩАЮЩЕГО ПРИСВАИВАНИЯ:" << endl;
    out << moveAssignedStudent << endl;

    cout << "Работа завершена. Результаты записаны в output.txt." << endl;
    return 0;
}
