#include <iostream>
#include <fstream>
#include <string>
#include "avlTree.h"

struct node_binary {
    int data;
    node_binary* left;
    node_binary* right;
};

node_binary* createNode(int data) {
    return new node_binary{ data, nullptr, nullptr };
}

node_binary* insertNode(node_binary* root, int data) {
    if (root == nullptr) {
        root = createNode(data);
        return root;
    }
    if (data < root->data) {
        root->left = insertNode(root->left, data);
    }
    else if (data > root->data) {
        root->right = insertNode(root->right, data);
    }
    return root;
}


void traverseInDepth(node_binary* root, std::vector<int>& res) {
    if (root == nullptr) {
        return;
    }
    traverseInDepth(root->left, res);
    res.emplace_back(root->data);
    traverseInDepth(root->right, res);
}

node_binary* create_root(const std::string& src) {
    if (src.front() != '(') {
        throw std::runtime_error(
            src + " is not valid brace sequence. At index " + std::to_string(0) + " must be '('\n");
    }
    else if (src.back() != ')') {
        throw std::runtime_error(src + " is not valid brace sequence. At last position must be ')'\n");
    }
    else {
        auto current_data = src.find_first_not_of("0123456789", 1);
        auto last_open = src.find_last_of('(');
        if (last_open == 0) {
            if (current_data == 1) { return nullptr; }
            else { return new node_binary{ std::stoi(src.substr(1, current_data - 1)), nullptr, nullptr }; }
        }
        node_binary* left = create_root(src.substr(current_data + 1, last_open - (current_data + 1) - 1));
        node_binary* right = create_root(src.substr(last_open, src.length() - (last_open + 1)));
        return new node_binary{ std::stoi(src.substr(1, current_data - 1)), left, right };
    }
}

bool checkBinaryForCorrect(node_binary* root) {
    std::vector<int> to_check;
    traverseInDepth(root, to_check);

    node_binary* tmp_root = nullptr;
    for (const auto& x : to_check) {
        tmp_root = insertNode(tmp_root, x);
    }

    std::vector<int> tmp_to_check;
    traverseInDepth(tmp_root, tmp_to_check);

    for (size_t i = 0; i < to_check.size(); ++i) {
        if (to_check[i] != tmp_to_check[i]) {
            return false;
        }
    }
    return true;
}

void task_2() {
    std::string filename;
    std::cout << "Input filename to parse: ";
    std::getline(std::cin, filename);
    std::ifstream fin(filename);
    if (!fin.is_open()) {
        throw std::runtime_error("File " + filename + " can not be opened.\n");
    }
    std::string src;
    std::getline(fin, src);
    fin.close();

    node_binary* root = create_root(src);

    if (checkBinaryForCorrect(root)) {
        std::cout << "All is good!\n";
    }
    else {
        std::cout << "Something went wrong!\n";
    }
}

template<typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& rhs) {
    for (const auto& x : rhs) {
        out << x << ' ';
    }
    return out;
}

void task_1() {
    tree avl_tree;
    int n;
    std::cout << "Enter node's count: ";
    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        int number;
        std::cout << "node " << i + 1 << ": ";
        std::cin >> number;
        avl_tree.add(number);
    }

    std::string cmd;
    while (true) {
        std::cout << "1. DFS Inorder\n";
        std::cout << "2. DFS Preorder\n";
        std::cout << "3. DFS Postorder\n";
        std::cout << "4. BFS\n";
        std::cout << "0. Exit\nYour choice: ";
        std::getline(std::cin, cmd);
        if (cmd == "1") {
            std::cout << avl_tree.dfs_inorder() << '\n';
        }
        else if (cmd == "2") {
            std::cout << avl_tree.dfs_preorder() << '\n';
        }
        else if (cmd == "3") {
            std::cout << avl_tree.dfs_postorder() << '\n';
        }
        else if (cmd == "4") {
            std::cout << avl_tree.bfs() << '\n';
        }
        else if (cmd == "0") {
            return;
        }
        else {
            std::cout << "Unknown command!\n";
        }
    }
}

void menu() {
    std::string cmd;
    while (true) {
        std::cout << "1. AVL-tree\n";
        std::cout << "2. Parse file with tree\n";
        std::cout << "0. exit\n";
        std::getline(std::cin, cmd);
        if (cmd == "1") {
            task_1();
        }
        else if (cmd == "2") {
            task_2();
        }
        else if (cmd == "0") {
            std::cout << "Good bye!\n";
            return;
        }
        else {
            std::cout << "Unknown command!\n";
        }
    }
}

int main() {
    menu();
    return 0;
}