#pragma once
#include "tree.h"

class MapVisualizerS {
public:
    MapVisualizerS(const HashMap& hashMap, QTextEdit* textEdit)
        : hashMap_(hashMap), textEdit_(textEdit) {}

    void display() const;

private:
    const HashMap& hashMap_;
    QTextEdit* textEdit_;
};


class TreeVisualizerS {
public:
    TreeVisualizerS(Tree* tree, QTextEdit* textEdit) : tree_(tree), textEdit_(textEdit) {}
    void print();

private:
    void printHelper(Leaf* node, QString indent, bool last);

    Tree* tree_;
    QTextEdit* textEdit_;
};

