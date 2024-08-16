#pragma once
#include "tree.h"

class MapVisualizer {
public:
    MapVisualizer(const HashMap& hashMap, QTextEdit* textEdit)
        : hashMap_(hashMap), textEdit_(textEdit) {}

    void display() const;
    // void setSize(size_t size){
    //     hashMap_.setSize(size);
    // }
private:
    const HashMap& hashMap_;
    QTextEdit* textEdit_;
};


class TreeVisualizer {
public:
    TreeVisualizer(Tree* tree, QTextEdit* textEdit) : tree_(tree), textEdit_(textEdit) {}
    void print();

private:
    void printHelper(Leaf* node, QString indent, bool last);

    Tree* tree_;
    QTextEdit* textEdit_;
};

