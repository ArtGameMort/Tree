#ifndef LESSONS_CPP_AVLTREE_H
#define LESSONS_CPP_AVLTREE_H

#include <iostream>
#include <vector>

using namespace std;

// Дерево состоит из различных элементов. Каждый элемент дает после себя
// максимум две ветки, причем элементы в левой ветке меньше его, а элементы
// правой ветки больше
class node {
public:
    int data;  // int - 4 байта
    unsigned char height; // Высота поддерева, исходящего из данного узла (1 байт)
    node* left;
    node* right;

    node(int elem);
};

node::node(int elem) {
    height = 1;
    data = elem;
    left = nullptr;
    right = nullptr;
}

unsigned char height(node* p) {
    if (p == nullptr) {
        return 0;
    }
    else {
        return p->height;
    }
}

// Баланс фактор
int balance_factor(node* p) {
    return height(p->right) - height(p->left);
}

void fix_height(node* p) {
    unsigned char h1 = height(p->left);
    unsigned char h2 = height(p->right);
    p->height = std::max(h1, h2) + 1;
}

class tree {
public:
    tree(); // Конструктор по умолчанию

    ~tree();

    void add(const int& elem);

    bool remove(const int& elem);

    void print(node* current = nullptr);

    vector<node*> dfs();

    vector<int> dfs_inorder() const;

    vector<int> dfs_preorder() const;

    vector<int> dfs_postorder() const;

    vector<int> bfs() const;

    size_t getLevels() {
        level_count(root, 1);
        return levels;
    }

    bool find(const int& elem) const;


private:
    node* root;
    size_t size;
    size_t levels;

    node* rotate_left(node* p);

    node* rotate_right(node* p);

    node* balance(node* p);

    node* insert(node* p, const int& elem);

    node* find_min(node* p); // поиск минимального узла в дереве p

    node* remove_min(node* p); // Удаление узла с минимальным ключом из дерева p

    node* remove_node(node* p, const int& elem);

    node* _find(node* p, const int& elem) const;

    void level_count(node* current, int level);

    void dfs_(node* current, vector<node*>& res);

    void _dfs_inorder(node* current, vector<int>& res) const;

    void _bfs(node* current, vector<int>& res, int level) const;

    void _dfs_preorder(node* current, vector<int>& res) const;

    void _dfs_postorder(node* current, vector<int>& res) const;
};

std::vector<int> tree::bfs() const {
    int h = height(root);
    std::vector<int> res;
    for (int i = 1; i <= h; ++i) {
        _bfs(root, res, i);
    }
    return res;
}

void tree::_bfs(node* current, vector<int>& res, int level) const {
    if (current == nullptr) {
        return;
    }
    if (level == 1) {
        res.push_back(current->data);
    }
    else {
        _bfs(current->left, res, level - 1);
        _bfs(current->right, res, level - 1);
    }
}

std::vector<int> tree::dfs_postorder() const {
    std::vector<int> res;
    _dfs_postorder(root, res);
    return res;
}

void tree::_dfs_postorder(node* current, vector<int>& res) const {
    if (current == nullptr) {
        return;
    }
    _dfs_postorder(current->left, res);
    _dfs_postorder(current->right, res);
    res.push_back(current->data);
}


std::vector<int> tree::dfs_inorder() const {
    std::vector<int> res;
    _dfs_inorder(root, res);
    return res;
}

void tree::_dfs_inorder(node* current, vector<int>& res) const {
    if (current == nullptr) {
        return;
    }
    _dfs_inorder(current->left, res);
    res.push_back(current->data);
    _dfs_inorder(current->right, res);
}

std::vector<int> tree::dfs_preorder() const {
    std::vector<int> res;
    _dfs_preorder(root, res);
    return res;
}

void tree::_dfs_preorder(node* current, vector<int>& res) const {
    if (current == nullptr) {
        return;
    }
    res.push_back(current->data);
    _dfs_preorder(current->left, res);
    _dfs_preorder(current->right, res);
}

node* tree::rotate_left(node* p) {
    node* res = p->right;
    p->right = res->left;
    res->left = p;
    fix_height(res);
    fix_height(p);
    return res;
}

node* tree::rotate_right(node* p) {
    node* res = p->left;
    p->left = res->right;
    res->right = p;
    fix_height(res);
    fix_height(p);
    return res;
}

tree::tree() {
    root = nullptr;
    size = 0;
    levels = 0;
}

void tree::add(const int& elem) {
    root = insert(root, elem);
    ++size;
}

tree::~tree() {
    vector<node*> elems = dfs();
    for (auto& elem : elems) {
        delete elem;
    }
}

void tree::print(node* current) {
    if (current != nullptr) {
        std::cout << current->data << ' ';
        print(current->left);
        print(current->right);
    }
}

vector<node*> tree::dfs() {
    vector<node*> res;
    dfs_(root, res);
    return res;
}

void tree::dfs_(node* current, vector<node*>& other) {
    if (current != nullptr) {
        other.push_back(current);
        dfs_(current->left, other);
        dfs_(current->right, other);
    }
}

void tree::level_count(node* current, int current_level = 1) {
    if (current != nullptr) {
        if (current_level > levels) {
            levels = current_level;
        }
        level_count(current->left, current_level + 1);
        level_count(current->right, current_level + 1);
    }
}

node* tree::balance(node* p) {
    fix_height(p);
    if (balance_factor(p) == 2) {
        if (balance_factor(p->right) < 0) {
            p->right = rotate_right(p->right);
        }
        return rotate_left(p);
    }
    if (balance_factor(p) == -2) {
        if (balance_factor(p->left) > 0) {
            p->left = rotate_left(p->left);
        }
        return rotate_right(p);
    }
    return p;
}

node* tree::insert(node* p, const int& elem) {
    if (p == nullptr) {
        return new node(elem);
    }
    else if (elem < p->data) {
        p->left = insert(p->left, elem);
    }
    else {
        p->right = insert(p->right, elem);
    }
    return balance(p);
}

node* tree::find_min(node* p) {
    return p->left ? find_min(p->left) : p;
}

node* tree::remove_min(node* p) {
    if (p->left == nullptr) {
        return p->right;
    }
    p->left = find_min(p->left);
    return balance(p);
}

node* tree::remove_node(node* p, const int& elem) {
    if (p == nullptr) {
        return nullptr;
    }
    else if (elem < p->data) {
        p->left = remove_node(p->left, elem);
    }
    else if (elem > p->data) {
        p->right = remove_node(p->right, elem);
    }
    else {
        if (p->right == nullptr || p->left == nullptr) {
            auto* tmp = p->left ? p->left : p->right;
            if (tmp == nullptr) {
                tmp = p;
                p = nullptr;
            }
            else {
                *p = *tmp;
            }
            delete tmp;
        }
        else {
            auto* tmp = find_min(p->right);
            p->data = tmp->data;
            p->right = remove_node(p->right, tmp->data);
        }
    }
    if (p == nullptr) {
        return p;
    }

    p->height = 1 + max(height(p->left), height(p->right));
    int balance = balance_factor(p);

    if (balance > 1 && balance_factor(p->left) < 0) {
        p->left = rotate_left(p->left);
        return rotate_right(p);
    }

    if (balance < -1 && balance_factor(p->right) <= 0) {
        return rotate_left(p);
    }

    if (balance < -1 && balance_factor(p->right) > 0) {
        p->right = rotate_right(p->right);
        return rotate_left(p);
    }

    return p;
}

bool tree::remove(const int& elem) {
    return remove_node(root, elem) != nullptr;
}

bool tree::find(const int& elem) const {
    return _find(root, elem) != nullptr;
}

node* tree::_find(node* p, const int& elem) const {
    if (p == nullptr) {
        return nullptr;
    }
    else if (p->data < elem) {
        return _find(p->right, elem);
    }
    else if (p->data > elem) {
        return _find(p->left, elem);
    }
    else {
        return p;
    }
}


#endif //LESSONS_CPP_AVLTREE_H