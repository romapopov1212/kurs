#pragma once
#include <iomanip>
#include <vector>
#include "map.h"
#include <QTextEdit>  // Включаем необходимый заголовок Qt для QTextEdit
#include <QString>
using namespace std;
using Str = std::string;
using UInt = unsigned int;

enum Color { COLOR_RED, COLOR_BLACK };

class Leaf {
public:
    Leaf(const Date& date, HashMap* map = nullptr, Color color = COLOR_RED) : date_(date), map_(map), color_(color), parent_(nullptr), left_(nullptr), right_(nullptr) {}

    ~Leaf() {
        delete map_;
    }

    const Date& getDate() const
    {
        return date_;
    }
    HashMap* getMap() const
    {
        return map_;
    }
    Color getColor() const
    {
        return color_;
    }
    Leaf* getParent() const
    {
        return parent_;
    }
    Leaf* getLeft() const
    {
        return left_;
    }
    Leaf* getRight() const
    {
        return right_;
    }

    void setDate(const Date& date)
    {
        date_ = date;
    }
    void setMap(HashMap* map)
    {
        map_ = map;
    }
    void setColor(Color color)
    {
        color_ = color;
    }
    void setParent(Leaf* parent)
    {
        parent_ = parent;
    }
    void setLeft(Leaf* left)
    {
        left_ = left;
    }
    void setRight(Leaf* right)
    {
        right_ = right;
    }

private:
    Date date_;
    HashMap* map_;
    Color color_;
    Leaf* parent_;
    Leaf* left_;
    Leaf* right_;

};

class Tree {
public:
    Tree() {
        n_leaf_ = new Leaf(Date(), nullptr, COLOR_BLACK);
        n_leaf_->setLeft(n_leaf_);
        n_leaf_->setRight(n_leaf_);
        root_ = n_leaf_;
    }

    ~Tree() {
        clearTree();
        delete n_leaf_;
    }

    Leaf* getRoot() const
    {
        return root_;
    }
    Leaf* getNullLeave() const
    {
        return n_leaf_;
    }
    void setSizeTable(size_t size){
        size_ = size;
    }
    void insert(const Code& code, const Group& group, const Str& discipline, const Date& date) {
        if (!date.is_Date()) {
            return;
        }

        Leaf* existing_node = searchTree(date);

        if (existing_node != n_leaf_) {
            existing_node->getMap()->insert(code, group, discipline);
            return;
        }

        HashMap* new_map = new HashMap(size_);
        new_map->insert(code, group, discipline);

        Leaf* new_node = new Leaf(date, new_map);
        new_node->setParent(nullptr);
        new_node->setLeft(n_leaf_);
        new_node->setRight(n_leaf_);
        new_node->setColor(COLOR_RED);

        Leaf* y = nullptr;
        Leaf* x = this->root_;

        while (x != n_leaf_) {
            y = x;
            if (new_node->getDate().getYear() < x->getDate().getYear() ||
                (new_node->getDate().getYear() == x->getDate().getYear() && new_node->getDate().getMonth() < x->getDate().getMonth()) ||
                (new_node->getDate().getYear() == x->getDate().getYear() && new_node->getDate().getMonth() == x->getDate().getMonth() && new_node->getDate().getDay() < x->getDate().getDay())) {
                x = x->getLeft();
            }
            else {
                x = x->getRight();
            }
        }

        new_node->setParent(y);
        if (y == nullptr) {
            root_ = new_node;
        }
        else if (new_node->getDate().getYear() < y->getDate().getYear() ||
                 (new_node->getDate().getYear() == y->getDate().getYear() && new_node->getDate().getMonth() < y->getDate().getMonth()) ||
                 (new_node->getDate().getYear() == y->getDate().getYear() && new_node->getDate().getMonth() == y->getDate().getMonth() && new_node->getDate().getDay() < y->getDate().getDay())) {
            y->setLeft(new_node);
        }
        else {
            y->setRight(new_node);
        }

        if (new_node->getParent() == nullptr) {
            new_node->setColor(COLOR_BLACK);
            return;
        }

        if (new_node->getParent()->getParent() == nullptr) {
            return;
        }

        balance_after_insert(new_node);
    }
    bool remove(const Code& code, const Group& group, const Str& discipline, const Date& date) {
        Leaf* node = searchTree(date);

        if (node == n_leaf_) {
            return false; // Node not found
        }

        if (node->getMap()->remove(code, group, discipline)) {
            if (node->getMap()->isEmpty()) {
                removeHelper(node, code, group, discipline, date);
            }
            return true; // Successfully removed
        }

        return false; // Remove from map failed
    }

    Leaf* find(const Date& date) {
        return searchTree(date);
    }

    vector<Leaf*> searchByCodeAndDateRange( const Code& code, const Date& startDate, const Date& endDate, vector<Node*>& res, int&count) {
        vector<Leaf*> result;
        count = 0;
        searchByCodeAndDateRangeHelper(this->getRoot(), this->getNullLeave(), code, startDate, endDate, result, res, count);
        return result;
    }

    void searchByCodeAndDateRangeHelper(Leaf* node, Leaf* null_leave, const Code& code, const Date& startDate, const Date& endDate, vector<Leaf*>& result, vector<Node*>& res, int&count) {
        if (node == null_leave) {
            return;
        }
        count++;
        if (node->getDate() > startDate && node->getDate() < endDate) {
            if (node->getMap() != nullptr && node->getMap()->find(code, res)) {
                result.push_back(node);
            }
        }

        if (node->getDate() >= startDate) {
            searchByCodeAndDateRangeHelper(node->getLeft(), null_leave, code, startDate, endDate, result, res, count);
        }

        if (node->getDate() <= endDate) {
            searchByCodeAndDateRangeHelper(node->getRight(), null_leave, code, startDate, endDate, result,res, count);
        }
    }

private:
    Leaf* root_;
    Leaf* n_leaf_;
    size_t size_;

private:
    void initNullLeave(Leaf* leave, Leaf* parent) {
        leave->setDate(Date());
        leave->setMap(nullptr);
        leave->setColor(COLOR_BLACK);
        leave->setParent(parent);
        leave->setLeft(nullptr);
        leave->setRight(nullptr);
    }

    void leftRotate(Leaf* leave) {
        Leaf* right = leave->getRight();
        leave->setRight(right->getLeft());

        if (right->getLeft() != n_leaf_) {
            right->getLeft()->setParent(leave);
        }

        right->setParent(leave->getParent());
        if (leave->getParent() == nullptr) {
            root_ = right;
        }
        else if (leave == leave->getParent()->getLeft()) {
            leave->getParent()->setLeft(right);
        }
        else {
            leave->getParent()->setRight(right);
        }

        right->setLeft(leave);
        leave->setParent(right);
    }

    void rightRotate(Leaf* leave) {
        Leaf* left = leave->getLeft();
        leave->setLeft(left->getRight());

        if (left->getRight() != n_leaf_) {
            left->getRight()->setParent(leave);
        }

        left->setParent(leave->getParent());
        if (leave->getParent() == nullptr) {
            root_ = left;
        }
        else if (leave == leave->getParent()->getRight()) {
            leave->getParent()->setRight(left);
        }
        else {
            leave->getParent()->setLeft(left);
        }

        left->setRight(leave);
        leave->setParent(left);
    }


    void removeHelper(Leaf* node, const Code& code, const Group& group, const Str& discipline, const Date& date) {
        Leaf* z = node;
        Leaf* x, * y;
        y = z;
        Color y_original_color = y->getColor();

        if (z->getLeft() == n_leaf_) {
            x = z->getRight();
            swap(z, z->getRight());
        }
        else if (z->getRight() == n_leaf_) {
            x = z->getLeft();
            swap(z, z->getLeft());
        }
        else {
            y = minimum(z->getRight());
            y_original_color = y->getColor();
            x = y->getRight();
            if (y->getParent() == z) {
                x->setParent(y);
            }
            else {
                swap(y, y->getRight());
                y->setRight(z->getRight());
                y->getRight()->setParent(y);
            }

            swap(z, y);
            y->setLeft(z->getLeft());
            y->getLeft()->setParent(y);
            y->setColor(z->getColor());
        }

        delete z;

        if (y_original_color == COLOR_BLACK) {
            balanceRemove(x);
        }
    }

    void swap(Leaf* u, Leaf* v) {
        if (u->getParent() == nullptr) {
            root_ = v;
        }
        else if (u == u->getParent()->getLeft()) {
            u->getParent()->setLeft(v);
        }
        else {
            u->getParent()->setRight(v);
        }
        v->setParent(u->getParent());
    }

    void balance_after_insert(Leaf* k) {
        Leaf* u;
        while (k->getParent()->getColor() == COLOR_RED) {
            if (k->getParent() == k->getParent()->getParent()->getRight()) {
                u = k->getParent()->getParent()->getLeft();
                if (u->getColor() == COLOR_RED) {
                    u->setColor(COLOR_BLACK);
                    k->getParent()->setColor(COLOR_BLACK);
                    k->getParent()->getParent()->setColor(COLOR_RED);
                    k = k->getParent()->getParent();
                }
                else {
                    if (k == k->getParent()->getLeft()) {
                        k = k->getParent();
                        rightRotate(k);
                    }
                    k->getParent()->setColor(COLOR_BLACK);
                    k->getParent()->getParent()->setColor(COLOR_RED);
                    leftRotate(k->getParent()->getParent());
                }
            }
            else {
                u = k->getParent()->getParent()->getRight();
                if (u->getColor() == COLOR_RED) {
                    u->setColor(COLOR_BLACK);
                    k->getParent()->setColor(COLOR_BLACK);
                    k->getParent()->getParent()->setColor(COLOR_RED);
                    k = k->getParent()->getParent();
                }
                else {
                    if (k == k->getParent()->getRight()) {
                        k = k->getParent();
                        leftRotate(k);
                    }
                    k->getParent()->setColor(COLOR_BLACK);
                    k->getParent()->getParent()->setColor(COLOR_RED);
                    rightRotate(k->getParent()->getParent());
                }
            }
            if (k == root_) {
                break;
            }
        }
        root_->setColor(COLOR_BLACK);
    }

    void balanceRemove(Leaf* x) {
        Leaf* s;
        while (x != root_ && x->getColor() == COLOR_BLACK) {
            if (x == x->getParent()->getLeft()) {
                s = x->getParent()->getRight();
                if (s->getColor() == COLOR_RED) {
                    s->setColor(COLOR_BLACK);
                    x->getParent()->setColor(COLOR_RED);
                    leftRotate(x->getParent());
                    s = x->getParent()->getRight();
                }

                if (s->getLeft()->getColor() == COLOR_BLACK && s->getRight()->getColor() == COLOR_BLACK) {
                    s->setColor(COLOR_RED);
                    x = x->getParent();
                }
                else {
                    if (s->getRight()->getColor() == COLOR_BLACK) {
                        s->getLeft()->setColor(COLOR_BLACK);
                        s->setColor(COLOR_RED);
                        rightRotate(s);
                        s = x->getParent()->getRight();
                    }

                    s->setColor(x->getParent()->getColor());
                    x->getParent()->setColor(COLOR_BLACK);
                    s->getRight()->setColor(COLOR_BLACK);
                    leftRotate(x->getParent());
                    x = root_;
                }
            }
            else {
                s = x->getParent()->getLeft();
                if (s->getColor() == COLOR_RED) {
                    s->setColor(COLOR_BLACK);
                    x->getParent()->setColor(COLOR_RED);
                    rightRotate(x->getParent());
                    s = x->getParent()->getLeft();
                }

                if (s->getLeft()->getColor() == COLOR_BLACK && s->getRight()->getColor() == COLOR_BLACK) {
                    s->setColor(COLOR_RED);
                    x = x->getParent();
                }
                else {
                    if (s->getLeft()->getColor() == COLOR_BLACK) {
                        s->getRight()->setColor(COLOR_BLACK);
                        s->setColor(COLOR_RED);
                        leftRotate(s);
                        s = x->getParent()->getLeft();
                    }

                    s->setColor(x->getParent()->getColor());
                    x->getParent()->setColor(COLOR_BLACK);
                    s->getLeft()->setColor(COLOR_BLACK);
                    rightRotate(x->getParent());
                    x = root_;
                }
            }
        }
        x->setColor(COLOR_BLACK);
    }

    Leaf* searchTreeHelper(Leaf* leave, const Date& date) {
        if (leave == n_leaf_ || (leave->getDate().getYear() == date.getYear() && leave->getDate().getMonth() == date.getMonth() && leave->getDate().getDay() == date.getDay())) {
            return leave;
        }

        if (date.getYear() < leave->getDate().getYear() ||
            (date.getYear() == leave->getDate().getYear() && date.getMonth() < leave->getDate().getMonth()) ||
            (date.getYear() == leave->getDate().getYear() && date.getMonth() == leave->getDate().getMonth() && date.getDay() < leave->getDate().getDay())) {
            return searchTreeHelper(leave->getLeft(), date);
        }
        return searchTreeHelper(leave->getRight(), date);
    }

    Leaf* searchTree(const Date& date) {
        return searchTreeHelper(this->root_, date);
    }

    Leaf* minimum(Leaf* leave) {
        while (leave->getLeft() != n_leaf_) {
            leave = leave->getLeft();
        }
        return leave;
    }

    Leaf* maximum(Leaf* leave) {
        while (leave->getRight() != n_leaf_) {
            leave = leave->getRight();
        }
        return leave;
    }

    void clearTreeHelper(Leaf* leave) {
        if (leave != n_leaf_) {
            clearTreeHelper(leave->getLeft());
            clearTreeHelper(leave->getRight());
            delete leave;
        }
    }

    void clearTree() {
        clearTreeHelper(root_);
        root_ = n_leaf_;
    }
};
