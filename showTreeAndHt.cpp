#include"showTreeAndHt.h"

void MapVisualizer::display() const {

    textEdit_->append("----------------------------------------------------------");
    textEdit_->append(QString("%1%2%3%4")
                          .arg("Bucket", -12)
                          .arg("Code", -12)
                          .arg("Group", -12)
                          .arg("Discipline", -12));
    textEdit_->append("----------------------------------------------------------");

    for (size_t i = 0; i < hashMap_.getSize(); ++i) {
        const Cell& record = hashMap_.getCell()[i];
        if (!record.isEmpty()) {
            const List& list = record.getValue().getList();
            Node* head = list.getHead();
            if (head != nullptr) {
                Node* current = head;
                bool firstNode = true;
                do {
                    QString line;
                    if (firstNode) {
                        line += QString("%1").arg(i, -12); // Print bucket index once
                        firstNode = false;
                    } else {
                        line += QString("%1").arg("", -12); // Align subsequent lines
                    }
                    line += QString("%1%2%3")
                                .arg(current->getCode().toString().c_str(), -10)
                                .arg(current->getGroup().ToString().c_str(), -12)
                                .arg(current->getDiscipline().c_str(), -12);
                    current = current->getNext();
                    if (current != head) {
                        line += " -> "; // Add arrow for all but the last element
                    }
                    textEdit_->append(line);
                } while (current != head);

            }
        } else {
            textEdit_->append(QString("%1%2%3%4")
                                  .arg(i, -12)
                                  .arg("-", -12)
                                  .arg("-", -12)
                                  .arg("-", -12));
        }
    }
    textEdit_->append("----------------------------------------------------------");
}

void TreeVisualizer::print() {
    if (tree_->getRoot() == tree_->getNullLeave()) {
        textEdit_->append("Tree is empty.");
        return;
    }
    printHelper(tree_->getRoot(), "", true);
}

void TreeVisualizer::printHelper(Leaf* node, QString indent, bool last) {
    if (node != tree_->getNullLeave()) {
        textEdit_->append(indent + (last ? "R----" : "L----") +
                          QString::fromStdString(node->getDate().toString()) +
                          "(" + (node->getColor() == COLOR_RED ? "RED" : "BLACK") + ")");

        indent += last ? "   " : "|  ";
        // это вывод хеш таблицы

        MapVisualizer mapVisualizer(*node->getMap(), textEdit_);
        mapVisualizer.display();

        printHelper(node->getLeft(), indent, false);
        printHelper(node->getRight(), indent, true);
    }
}
