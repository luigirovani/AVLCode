#include <iostream>
#include <memory>
#include <algorithm>
#include <typeinfo>
#include <limits>

using namespace std;

template <typename T>
class AVLTree {
private:
    class Node {
    public:
        T key;
        int height;
        unique_ptr<Node> left;
        unique_ptr<Node> right;

        Node(T k)
            : key(k), height(1), left(nullptr), right(nullptr) {}
    };

    using NodePtr = unique_ptr<Node>;
    NodePtr root;

    NodePtr insertNode(NodePtr node, T key) {
        if (!node)
            return make_unique<Node>(key);

        if (key < node->key) {
            node->left = insertNode(move(node->left), key);
        }
        else if (key > node->key) {
            node->right = insertNode(move(node->right), key);
        }
        else {
            return node;
        }

        node->height = 1 + max(getHeight(node->left), getHeight(node->right));
        return balanceTree(move(node), key);
    }

    NodePtr balanceTree(NodePtr node, T key) {
        int balance = getBalance(node);

        if (balance > 1 && key < node->left->key) {
            cout << "RSD " << node->key << endl;
            return rightRotate(move(node));
        }

        if (balance < -1 && key > node->right->key) {
            cout << "RSE " << node->key << endl;
            return leftRotate(move(node));
        }

        if (balance > 1 && key > node->left->key) {
            cout << "RDD " << node->left->key << endl;
            node->left = leftRotate(move(node->left));
            return rightRotate(move(node));
        }

        if (balance < -1 && key < node->right->key) {
            cout << "RDE " << node->right->key << endl;
            node->right = rightRotate(move(node->right));
            return leftRotate(move(node));
        }

        return node;
    }

    NodePtr leftRotate(NodePtr z) {
        auto y = move(z->right);
        z->right = move(y->left);
        y->left = move(z);

        y->left->height = 1 + max(getHeight(y->left->left), getHeight(y->left->right));
        y->height = 1 + max(getHeight(y->left), getHeight(y->right));

        return y;
    }

    NodePtr rightRotate(NodePtr z) {
        auto y = move(z->left);
        z->left = move(y->right);
        y->right = move(z);

        y->right->height = 1 + max(getHeight(y->right->left), getHeight(y->right->right));
        y->height = 1 + max(getHeight(y->left), getHeight(y->right));

        return y;
    }

    int getHeight(const NodePtr& node) const {
        return node ? node->height : 0;
    }

    int getBalance(const NodePtr& node) const {
        return node ? getHeight(node->left) - getHeight(node->right) : 0;
    }

    void printTree(const NodePtr& node, int space = 0, int level_gap = 5) const {
        if (!node)
            return;

        space += level_gap;

        printTree(node->right, space);

        cout << endl;
        for (int i = level_gap; i < space; ++i)
            cout << " ";
        cout << node->key << "\n";

        printTree(node->left, space);
    }

public:
    AVLTree() : root(nullptr) {}

    bool isEmpty() const {
        return !root;
    }

    void print() const {
        if (isEmpty()) {
            cout << "The tree is empty" << endl;
            return;
        }
        printTree(root);
    }

    int getBalance() const {
        return getBalance(root);
    }

    void insert(T key) {
        root = insertNode(move(root), key);
    }

    void clear() {
        root.reset();
        cout << "Tree cleared!\n";
    }

    T getInput() {
        cout << "Enter the value to insert: ";
        T input;
        cin >> input;

        if (cin.fail()) {
            cerr << "Invalid value!\nInstantiate a tree with another type\n" << endl;
            printTip();
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return getInput();
        }

        cout << endl;
        return input;
    }

    void printTip() const {
        cout << "Enter a value of type <" << typeid(T).name() << "> to insert into the tree.\n";
        cout << "Enter 0 to clear the tree.\n";
        cout << "Enter -1 to exit.\n";
        cout << "Enter -2 to print the tree.\n";
        cout << endl;
    }
};

int main() {
    AVLTree<int> tree;
    tree.printTip();

    while (true) {
        auto input = tree.getInput();

        if (input == 0) {
            cout << "Balance = " << tree.getBalance() << endl;
            tree.clear();
        }
        else if (input == -1)
            break;
        else if (input == -2)
            tree.print();
        else
            tree.insert(input);
    }

    cout << "End" << endl;
    return 0;
}
