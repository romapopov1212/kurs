#pragma once
#include <iomanip>
#include <vector>
#include "list.h"

using namespace std;
using Str = std::string;
using UInt = unsigned int;

class M_List {
public:
    M_List() : list_(new List()) {}
    ~M_List() { delete list_; }

    List& getList() { return *list_; }
    const List& getList() const { return *list_; }

    void setList(List* list) {
        delete list_;
        list_ = list;
    }

private:
    List* list_;
};

class Cell {
public:
    Cell() : is_empty_(true), hash_(0), value_(new M_List()) {}
    ~Cell() { delete value_; }

    bool isEmpty() const { return is_empty_; }
    UInt getHash() const { return hash_; }
    const M_List& getValue() const { return *value_; }
    M_List& getValue() { return *value_; }

    void set_Empty(bool is_empty) { is_empty_ = is_empty; }
    void setHash(UInt hash) { hash_ = hash; }
    void setValue(M_List* value) {
        delete value_;
        value_ = value;
        is_empty_ = false;
    }


private:
    bool is_empty_;
    UInt hash_;
    M_List* value_;
};

class HashMap {
public:
    HashMap(int size) : size_(size < 2 ? 2: static_cast<size_t>(size)),
        initial_size_(size < 2 ? 2 : static_cast<size_t>(size)),
        count_cell_(0) {
        cell_.resize(size_);
    }

    ~HashMap() = default;
    size_t getSize() const { return size_; }
    size_t getInitialSize() const { return initial_size_; }
    size_t getCountCell() const { return count_cell_; }
    const vector<Cell>& getCell() const { return cell_; }
    void setSize(size_t size){size_=size;}
    bool isEmpty() const { return count_cell_ == 0; }

    void insert(const Code& code, const Group& group, const Str& discipline) {
        Str key = code.toString();
        UInt hash = hashFunc(key);


        if (cell_[hash].isEmpty() || !cell_[hash].getValue().getList().is_real_node(code, group, discipline)) {
            if (cell_[hash].isEmpty()) {

                cell_[hash].setHash(hash);
                cell_[hash].setValue(new M_List());
                count_cell_++;
            }

            cell_[hash].getValue().getList().insert(code, group, discipline);
        }
    }


    bool remove(const Code& code, const Group& group, const Str& discipline) {
        Str key = group.ToString() + " " + discipline;
        UInt hash = hashFunc(key);

        if (!cell_[hash].isEmpty()) {
            List& list = cell_[hash].getValue().getList();
            list.remove(code, group, discipline);

            if (list.isEmpty()) {
                cell_[hash].set_Empty(true);
                count_cell_--;
            }
            return true; // Successfully removed
        }
        return false; // Element not found
    }

    bool Search(const Code&code) const {
        Str key = code.toString();
        UInt hash = hashFunc(key);
        size_t steps = 0;

        while (!cell_[hash].isEmpty()) {
            steps++;
            const List& list = cell_[hash].getValue().getList();
            Node* current = list.getHead();
            do {
                if (current->getCode().getFirst() == code.getFirst() && current->getCode().getSecond() == code.getSecond() && current->getCode().getThird() == code.getThird()) {
                    return true;
                }
                current = current->getNext();
            } while (current != list.getHead());

            hash = (hash + 1) % size_;
        }

        return false;
    }


    bool find(const Code&code, vector<Node*>& res) const {
        Str key = code.toString();
        UInt hash = hashFunc(key);
        size_t steps = 0;

        while (!cell_[hash].isEmpty()) {
            steps++;
            const List& list = cell_[hash].getValue().getList();
            Node* current = list.getHead();
            do {
                if (current->getCode().getFirst() == code.getFirst() && current->getCode().getSecond() == code.getSecond() && current->getCode().getThird() == code.getThird()) {
                    res.push_back(current);
                    return true;
                }
                current = current->getNext();
            } while (current != list.getHead());

            hash = (hash + 1) % size_;
        }

        return false;
    }
private:
    size_t size_;
    size_t initial_size_;
    size_t count_cell_;
    vector<Cell> cell_;

    int countInNumber(int number) const {
        int count = 0;
        while (number != 0) {
            number /= 10;
            count++;
        }
        return count;
    }

public:
    UInt hashFunc(const Str& key) const {
        int hashedValue = 0;
        for (char c : key) {
            hashedValue += static_cast<int>(c);
        }
        hashedValue = hashedValue * hashedValue;

        int size_a = countInNumber(hashedValue);
        int size_b = countInNumber(size_ / 10);
        int index = static_cast<int>(ceil(size_a / 2.0)) - 1;

        hashedValue = hashedValue / static_cast<int>(pow(10, size_a - index - size_b));
        hashedValue = hashedValue % static_cast<int>(pow(10, size_b));

        return static_cast<UInt>(hashedValue);
    }
    // UInt hashFunc(const Str& key) const {
    //     UInt hashedValue = 0;
    //     for (char c : key) {
    //         hashedValue = hashedValue * 31 + static_cast<UInt>(c);
    //     }
    //     return hashedValue % size_;
    // }

};



