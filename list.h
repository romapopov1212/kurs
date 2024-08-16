
#pragma once
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include<string>
#include<cmath>
using namespace std;
using Str = std::string;
using UInt = unsigned int;

class Code {
public:
    Code() : first_(0), second_(0), third_(0) {}
    Code(UInt first, UInt second, UInt third) : first_(first), second_(second), third_(third) {}
    ~Code() = default;

    UInt getFirst() const { return first_; }
    UInt getSecond() const { return second_; }
    UInt getThird() const { return third_; }

    void setFirst(UInt first) { first_ = first; }
    void setSecond(UInt second) { second_ = second; }
    void setThird(UInt third) { third_ = third; }

    Str toString() const {
        char buffer[9];
        snprintf(buffer, sizeof(buffer), "%02d.%02d.%02d", first_, second_, third_);
        return string(buffer);
    }

private:
    UInt first_;
    UInt second_;
    UInt third_;
};

class Group {
public:
    Group() : letter_(""), numbers_(0) {}
    Group(const Str& letter, UInt numbers) : letter_(letter), numbers_(numbers) {}
    ~Group() = default;

    const Str& getLetter() const { return letter_; }
    UInt getNumbers() const { return numbers_; }

    void setLetter(const Str& letter) { letter_ = letter; }
    void setNumbers(UInt numbers) { numbers_ = numbers; }

    Str ToString() const {
        return letter_ + to_string(numbers_);
    }
    bool isValid() const {
        // Проверяем, что строка не пустая и имеет длину хотя бы 2 символа
        if(letter_ == ""){
            return false;
        }
        else if(numbers_<1000){
            return false;
        }
        else {
            return true;
        }
    }
private:
    Str letter_;
    UInt numbers_;
};

class Date {
public:
    Date() : day_(0), month_(0), year_(0) {}
    Date(UInt day, UInt month, UInt year) : day_(day), month_(month), year_(year) {}
    ~Date() = default;

    UInt getDay() const { return day_; }
    UInt getMonth() const { return month_; }
    UInt getYear() const { return year_; }

    void setDay(UInt day) { day_ = day; }
    void setMonth(UInt month) { month_ = month; }
    void setYear(UInt year) { year_ = year; }

    Str toString() const {
        char buffer[11];
        snprintf(buffer, sizeof(buffer), "%02d.%02d.%04d", day_, month_, year_);
        return Str(buffer);
    }

    bool is_Date() const {
        if (year_ < 1)
            return false;

        if ((month_ < 1) || (month_ > 12))
            return false;

        bool leap_year = is_Date_Helper(year_);
        int days_in_month = getCountDays(month_, leap_year);

        return (day_ >= 1 && day_ <= days_in_month);
    }

    bool inRange(Date Date1, Date Date2) const
    {
        return (Date1 > *this and *this > Date2);
    }
    bool operator<(const Date& other) const {
        if (year_ != other.year_)
            return year_ < other.year_;
        if (month_ != other.month_)
            return month_ < other.month_;
        return day_ < other.day_;
    }

    bool operator>(const Date& other) const {
        return other < *this;
    }

    bool operator<=(const Date& other) const {
        return !(other < *this);
    }

    bool operator>=(const Date& other) const {
        return !(*this < other);
    }

private:
    UInt day_;
    UInt month_;
    UInt year_;

    bool is_Date_Helper(UInt year) const {
        return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
    }

    int getCountDays(UInt month, bool leap_year) const {
        switch (month) {
        case 1: // January
        case 3: // March
        case 5: // May
        case 7: // July
        case 8: // August
        case 10: // October
        case 12: // December
            return 31;
        case 4: // April
        case 6: // June
        case 9: // September
        case 11: // November
            return 30;
        case 2: // February
            return leap_year ? 29 : 28;
        default:
            return 0; // Invalid month
        }
    }
};

class Node {
public:
    Node(const Code& code, const Group& group, const Str& discipline)
        : code_(code), group_(group), discipline_(discipline), next_(nullptr) {}
    ~Node() = default;

    const Code& getCode() const { return code_; }
    const Group& getGroup() const { return group_; }
    const Str& getDiscipline() const { return discipline_; }
    Node* getNext() const { return next_; }

    void setCode(const Code& code) { code_ = code; }
    void setGroup(const Group& group) { group_ = group; }
    void setDiscipline(const Str& discipline) { discipline_ = discipline; }
    void setNext(Node* next) { next_ = next; }

private:
    Code code_;
    Group group_;
    Str discipline_;
    Node* next_;
};

class D
{
public:
    const Code& getCode() const { return code_; }
    const Group& getGroup() const { return group_; }
    const Str& getDiscipline() const { return dis_; }
    Date getDate() const { return date_; }

    void setCode(Code& code) {
        code_ = code;
    }
    void setGroup(Group& g) {
        group_ = g;
    }
    void setDis(Str& dis) {
        dis_ = dis;
    }
    void setDate(Date& date) {
        date_ = date;
    }
private:
    Code code_;
    Group group_;
    Str dis_;
    Date date_;
};

struct key
{
    Date date_;
    Str name_dis_;
    Code code_;
    Group group_;

    key(Date D, Str F, Code code, Group g) : date_(D), code_(code), group_(g) {};
    key() {};
};


class List {
public:
    List() : size_(0), head_(nullptr) {}
    ~List() { clear(); }

    size_t getSize() const { return size_; }
    bool isEmpty() const { return size_ == 0; }
    Node* getHead() const { return head_; }

    void insert(const Code& code, const Group& group, const Str& discipline) {
        if (is_real_node(code, group, discipline)) return;

        Node* newNode = new Node(code, group, discipline);
        if (isEmpty()) {
            head_ = newNode;
            head_->setNext(head_);
        }
        else {
            Node* tail = head_;
            while (tail->getNext() != head_) {
                tail = tail->getNext();
            }
            tail->setNext(newNode);
            newNode->setNext(head_);
        }
        size_++;
    }

    void remove(const Code& code, const Group& group, const Str& discipline) {
        if (isEmpty()) return;

        Node* current = head_;
        Node* previous = nullptr;

        do {
            if (current->getCode().getFirst() == code.getFirst() &&
                current->getCode().getSecond() == code.getSecond() &&
                current->getCode().getThird() == code.getThird() &&
                current->getGroup().getLetter() == group.getLetter() &&
                current->getGroup().getNumbers() == group.getNumbers() &&
                current->getDiscipline() == discipline) {

                if (previous == nullptr) {
                    Node* tail = head_;
                    while (tail->getNext() != head_) {
                        tail = tail->getNext();
                    }
                    if (tail == head_) {
                        delete head_;
                        head_ = nullptr;
                    }
                    else {
                        tail->setNext(head_->getNext());
                        Node* temp = head_;
                        head_ = head_->getNext();
                        delete temp;
                    }
                }
                else {
                    previous->setNext(current->getNext());
                    delete current;
                }
                size_--;
                return;
            }
            previous = current;
            current = current->getNext();
        } while (current != head_);
    }

    void clear() {
        while (!isEmpty()) {
            remove(head_->getCode(), head_->getGroup(), head_->getDiscipline());
        }
    }

    bool is_real_node(const Code& code, const Group& group, const Str& discipline) const {
        if (isEmpty()) return false;

        Node* current = head_;
        do {
            if (current->getCode().getFirst() == code.getFirst() &&
                current->getCode().getSecond() == code.getSecond() &&
                current->getCode().getThird() == code.getThird() &&
                current->getGroup().getLetter() == group.getLetter() &&
                current->getGroup().getNumbers() == group.getNumbers() &&
                current->getDiscipline() == discipline) {
                return true;
            }
            current = current->getNext();
        } while (current != head_);

        return false;
    }
    Node* findByCode(const Code& code) const {
        if (isEmpty()) return nullptr;

        Node* current = head_;
        do {
            if (current->getCode().getFirst() == code.getFirst() &&
                current->getCode().getSecond() == code.getSecond() &&
                current->getCode().getThird() == code.getThird()) {
                return current;
            }
            current = current->getNext();
        } while (current != head_);

        return nullptr;
    }

    void writeToFile(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filename << std::endl;
            return;
        }

        if (isEmpty()) {
            file.close();
            return;
        }

        Node* current = head_;
        do {
            file << current->getCode().toString() << " ";
            file << current->getGroup().ToString() << " ";
            file << current->getDiscipline() << "\n";
            current = current->getNext();
        } while (current != head_);

        file.close();
    }
private:
    size_t size_;
    Node* head_;
};

