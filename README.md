ОТЧЁТ
по лабораторной работе № 4 по курсу «Алгоритмизация и программирование»

Тема работы: Использование объектов своих классов в последовательных контейнерах библиотеки STL.

Вариант: № 11

Выполнил: студент группы ИУ8-14/6, Калявкин Родион Алексеевич
Проверил: преподаватель Барыкин Дмитрий Васильевич

Москва, МГТУ — 2026
1. ЦЕЛЬ РАБОТЫ
Изучение последовательных контейнеров стандартной библиотеки шаблонов STL, получение навыков хранения объектов собственного класса в контейнерах, копирования контейнеров с помощью алгоритмов STL и сортировки объектов.
2. УСЛОВИЕ ЗАДАЧИ
Для варианта № 11 необходимо разработать класс «Студент» с полями: ФИО, группа, номер зачетной книжки и массив из четырех оценок за сессию. Исходные данные необходимо поместить в контейнер std::vector, затем скопировать исходный контейнер в std::deque с помощью алгоритма std::copy. Исходный контейнер vector требуется отсортировать по возрастанию среднего балла с помощью алгоритма std::sort.

Объекты должны храниться непосредственно в контейнерах, без использования указателей. Класс должен иметь необходимые конструкторы, конструктор копирования и перемещения, операции копирующего и перемещающего присваивания, а также перегруженную операцию вывода в поток <<. Исходные данные считываются из файла input.txt, результаты записываются в output.txt.
3. ОПИСАНИЕ РЕАЛИЗАЦИИ
Класс Student содержит следующие поля: fio — ФИО студента, group — учебная группа, recordBook — номер зачетной книжки, grades — массив из четырех оценок. Для вычисления среднего балла реализован метод average().

Копирующий и перемещающий конструкторы, а также соответствующие операции присваивания объявлены со значением default. Самостоятельное управление памятью не требуется: поля std::string и std::array самостоятельно управляют своими ресурсами, поэтому стандартных операций копирования и перемещения достаточно.

Операция operator< сравнивает студентов сначала по среднему баллу. Если средние баллы совпадают, для сохранения разных студентов в set дополнительно сравниваются остальные поля: ФИО, группа, номер зачетной книжки и оценки. Для данной лабораторной это также задает порядок сортировки по среднему баллу с дополнительным критерием при равенстве.

Операция operator>> считывает одну запись из строки файла формата ФИО;группа;номер_зачетной_книжки;оценка1 оценка2 оценка3 оценка4. Операция operator<< выводит все поля студента и рассчитанный средний балл.

В main.cpp записи считываются в vector<Student>. Затем с помощью std::copy и std::back_inserter исходные объекты копируются в deque<Student>. После копирования исходный vector сортируется алгоритмом std::sort. Дополнительно программа демонстрирует работу конструкторов копирования и перемещения, а также копирующего и перемещающего присваивания.
4. ТЕКСТ ПРОГРАММЫ С КОММЕНТАРИЯМИ
Листинг 1 — student.h
#ifndef STUDENT_H
#define STUDENT_H

#include <array>
#include <iostream>
#include <string>

using namespace std;

// Класс для хранения информации о студенте
class Student {
private:
    string fio;                 // Фамилия, имя и отчество
    string group;               // Учебная группа
    int recordBook;             // Номер зачетной книжки
    array<int, 4> grades;       // Оценки за четыре экзамена

public:
    Student();
    Student(const string& f, const string& g, int r, const array<int, 4>& a);

    // Все поля самостоятельно управляют своими ресурсами,
    // поэтому достаточно стандартных операций копирования и перемещения.
    Student(const Student& s) = default;
    Student(Student&& s) noexcept = default;
    Student& operator=(const Student& s) = default;
    Student& operator=(Student&& s) noexcept = default;

    double average() const;

    // Для set порядок определяется средним баллом.
    bool operator<(const Student& s) const;

    // Для unordered_set сравниваются все поля объекта.
    bool operator==(const Student& s) const;

    friend struct StudentHash;
    friend ostream& operator<<(ostream& os, const Student& s);
    friend istream& operator>>(istream& is, Student& s);
};

struct StudentHash {
    size_t operator()(const Student& s) const;
};

#endif
Листинг 2 — student.cpp
#include "student.h"

#include <functional>
#include <sstream>
#include <tuple>
#include <utility>

using namespace std;

// Создаем студента с пустыми данными
Student::Student() : fio(""), group(""), recordBook(0), grades{} {}

// Создаем студента с заданными данными
Student::Student(const string& f, const string& g, int r, const array<int, 4>& a)
    : fio(f), group(g), recordBook(r), grades(a) {}

// Вычисляем средний балл по четырем оценкам
double Student::average() const {
    int sum = 0;

    for (int grade : grades) {
        sum += grade;
    }

    return static_cast<double>(sum) / grades.size();
}

// Сравниваем студентов по среднему баллу
bool Student::operator<(const Student& s) const {
    // Сначала сравниваем средний балл, затем остальные поля.
    // Это сохраняет разных студентов с одинаковым средним баллом в set.
    if (average() != s.average()) {
        return average() < s.average();
    }

    return tie(fio, group, recordBook, grades) <
           tie(s.fio, s.group, s.recordBook, s.grades);
}

// Сравниваем все поля студентов
bool Student::operator==(const Student& s) const {
    return fio == s.fio &&
           group == s.group &&
           recordBook == s.recordBook &&
           grades == s.grades;
}

// Выводим данные студента в поток
ostream& operator<<(ostream& os, const Student& s) {
    os << "ФИО: " << s.fio << endl;
    os << "Группа: " << s.group << endl;
    os << "Номер зачетной книжки: " << s.recordBook << endl;
    os << "Оценки: ";

    for (int grade : s.grades) {
        os << grade << " ";
    }

    os << endl;
    os << "Средний балл: " << s.average() << endl;

    return os;
}

// Считываем одну запись из строки файла.
// Формат: ФИО;группа;номер_зачетной_книжки;оценка1 оценка2 оценка3 оценка4
istream& operator>>(istream& is, Student& s) {
    string line;

    if (!getline(is >> ws, line)) {
        return is;
    }

    stringstream ss(line);
    string f, g, record, gradeLine;

    if (!getline(ss, f, ';') || !getline(ss, g, ';') ||
        !getline(ss, record, ';') || !getline(ss, gradeLine)) {
        is.setstate(ios::failbit);
        return is;
    }

    int r;
    stringstream recordStream(record);
    if (!(recordStream >> r)) {
        is.setstate(ios::failbit);
        return is;
    }

    array<int, 4> a{};
    stringstream gradeStream(gradeLine);

    for (int i = 0; i < 4; i++) {
        if (!(gradeStream >> a[i]) || a[i] < 2 || a[i] > 5) {
            is.setstate(ios::failbit);
            return is;
        }
    }

    // Сначала заполняем временный объект, затем присваиваем его результату.
    Student tmp(f, g, r, a);
    s = move(tmp);

    return is;
}

// Вычисляем хеш по всем полям студента
size_t StudentHash::operator()(const Student& s) const {
    size_t h = hash<string>()(s.fio);
    h ^= hash<string>()(s.group) + 0x9e3779b9 + (h << 6) + (h >> 2);
    h ^= hash<int>()(s.recordBook) + 0x9e3779b9 + (h << 6) + (h >> 2);

    for (int grade : s.grades) {
        h ^= hash<int>()(grade) + 0x9e3779b9 + (h << 6) + (h >> 2);
    }

    return h;
}
Листинг 3 — main.cpp
#include <algorithm>
#include <deque>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <utility>
#include <vector>

#include "student.h"

using namespace std;

int main() {
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
5. КОНТРОЛЬНЫЙ ПРИМЕР И РЕЗУЛЬТАТЫ РАБОТЫ
Для проверки используется шесть записей студентов. Содержимое файла input.txt:
Иванов Иван Иванович;ИУ8-14/6;240011;3 3 4 3
Петров Петр Сергеевич;ИУ8-14/6;240012;3 4 3 4
Сидорова Анна Павловна;ИУ8-14/6;240013;4 4 4 4
Кузнецов Дмитрий Олегович;ИУ8-14/6;240014;4 4 5 4
Смирнова Мария Андреевна;ИУ8-14/6;240015;5 4 5 5
Орлов Максим Игоревич;ИУ8-14/6;240016;5 5 5 5
Средние баллы студентов рассчитываются как среднее арифметическое четырех оценок:
Студент	Расчет	Средний балл
Иванов Иван Иванович	(3 + 3 + 4 + 3) / 4	3,25
Петров Петр Сергеевич	(3 + 4 + 3 + 4) / 4	3,50
Сидорова Анна Павловна	(4 + 4 + 4 + 4) / 4	4,00
Кузнецов Дмитрий Олегович	(4 + 4 + 5 + 4) / 4	4,25
Смирнова Мария Андреевна	(5 + 4 + 5 + 5) / 4	4,75
Орлов Максим Игоревич	(5 + 5 + 5 + 5) / 4	5,00


После запуска в output.txt последовательно выводятся исходный контейнер vector, копия исходного контейнера deque и отсортированный vector. Контейнер deque сохраняет исходный порядок записей, поскольку копирование выполняется до сортировки исходного vector. После сортировки студенты располагаются по возрастанию среднего балла: от 3,25 до 5,00.

Дополнительно в выходной файл выводятся результаты проверки конструктора копирования, конструктора перемещения, копирующего присваивания и перемещающего присваивания.

Пример фрагмента результата сортировки:
Иванов Иван Иванович — 3.25
Петров Петр Сергеевич — 3.50
Сидорова Анна Павловна — 4.00
Кузнецов Дмитрий Олегович — 4.25
Смирнова Мария Андреевна — 4.75
Орлов Максим Игоревич — 5.00
6. ВЫВОД
В ходе лабораторной работы разработан класс Student и реализовано хранение его объектов в последовательных контейнерах STL. Исходные данные считаны из текстового файла input.txt в контейнер std::vector. С помощью алгоритма std::copy объекты скопированы в std::deque. Исходный vector отсортирован по возрастанию среднего балла с помощью std::sort. Также продемонстрированы копирующий и перемещающий конструкторы, а также соответствующие операции присваивания. Поставленные задачи выполнены, программа проверена на контрольных данных.
7. СБОРКА И ЗАПУСК ПРОГРАММЫ
Проект собирается с использованием CMake и стандарта C++11. Для запуска в CLion необходимо открыть папку проекта, дождаться загрузки CMake и запустить конфигурацию Lab4_Variant11. Рабочая директория должна быть установлена в корень проекта ($PROJECT_DIR$), чтобы программа могла найти input.txt.

Сборка из терминала:
cmake -S . -B build
cmake --build build
После запуска результаты записываются в output.txt.
8. СОСТАВ ПРОЕКТА
- main.cpp — основная функция программы;
- student.h — объявление класса Student;
- student.cpp — реализация методов и перегруженных операций класса;
- input.txt — исходные данные;
- output.txt — результаты работы программы (создается при запуске);
- CMakeLists.txt — настройки сборки проекта.
