#include "tree.h"

using namespace std;

void CBTree::destroy(TNode* x) {
    if (x) { destroy(x->left); destroy(x->right); delete x; }
}

vector<TNode*> CBTree::nodes() const {  // сам вектор служит очередью
    vector<TNode*> o;
    if (root) o.push_back(root);
    for (size_t i = 0; i < o.size(); ++i) {
        if (o[i]->left) o.push_back(o[i]->left);
        if (o[i]->right) o.push_back(o[i]->right);
    }
    return o;
}

// Три обхода в глубину отличаются только местом, где записывается сам узел.
void CBTree::preorder(TNode* x, Strs& o) const {  // прямой узел, левое, правое
    if (!x) return;
    o.push_back(x->key);
    preorder(x->left, o);
    preorder(x->right, o);
}

void CBTree::inorder(TNode* x, Strs& o) const {  // симметричный левое, узел, правое
    if (!x) return;
    inorder(x->left, o);
    o.push_back(x->key);
    inorder(x->right, o);
}

void CBTree::postorder(TNode* x, Strs& o) const {  // обратный левое, правое, узел
    if (!x) return;
    postorder(x->left, o);
    postorder(x->right, o);
    o.push_back(x->key);
}

void CBTree::dumpRec(TNode* x, Strs& o) const {
    o.push_back(x ? x->key : "#");
    if (x) { dumpRec(x->left, o); dumpRec(x->right, o); }
}

TNode* CBTree::build(const Strs& t, size_t& i) {
    if (i >= t.size()) return nullptr;
    string key = t[i];
    ++i;
    if (key == "#") return nullptr;
    TNode* x = new TNode{normInt(key), nullptr, nullptr};  // в дереве только числа
    x->left = build(t, i);
    x->right = build(t, i);
    return x;
}

void CBTree::place(TNode* x, int depth, int& col, map<TNode*, pair<int, int>>& pos) const {
    if (!x) return;
    place(x->left, depth + 1, col, pos);
    pos[x] = {depth, col++};  // колонка = номер узла в симметричном обходе поэтому узлы не наезжают друг на друга
    place(x->right, depth + 1, col, pos);
}

string CBTree::insert(const string& k) {
    TNode* z = new TNode{normInt(k), nullptr, nullptr};  // не число ошибка до изменения дерева
    string result = z->key;
    if (!root) { root = z; return result; }
    for (TNode* x : nodes()) {  // 1) место: первое свободное при обходе в ширину дерево остается complete
        if (!x->left) { x->left = z; return result; }
        if (!x->right) { x->right = z; return result; }
    }
    return result;  // O(n) на поиск места + O(log n) на всплытие
}

bool CBTree::contains(const string& k) const {
    string key = normInt(k);
    for (TNode* x : nodes()) if (x->key == key) return true;
    return false;
}

// Проверка complete обход в ширину вместе с nullptr eсли после первого пустого места встретился настоящий узел дерево не complete
bool CBTree::isComplete() const {
    vector<TNode*> o{root};
    bool gap = false;
    for (size_t i = 0; i < o.size(); ++i) {
        if (!o[i]) { gap = true; continue; }
        if (gap) return false;
        o.push_back(o[i]->left);
        o.push_back(o[i]->right);
    }
    return true;
}

Strs CBTree::traverse(const string& order) const {
    Strs o;
    if (order == "PREORDER") preorder(root, o);
    else if (order == "INORDER") inorder(root, o);
    else if (order == "POSTORDER") postorder(root, o);
    else if (order == "BFS") for (TNode* x : nodes()) o.push_back(x->key);
    else fail("unknown order: " + order + " (inorder, preorder, postorder, bfs)");
    return o;
}

Strs CBTree::dump() const { Strs o; dumpRec(root, o); return o; }

void CBTree::load(const Strs& t) { destroy(root); root = nullptr; size_t i = 0; root = build(t, i); }

// Вертикальный вывод
string CBTree::show() const {
    if (!root) return "(empty tree)";
    size_t w = 0;
    for (TNode* x : nodes()) w = max(w, x->key.size());
    size_t cw = w + 2;  // ширина одной колонки
    map<TNode*, pair<int, int>> pos;
    int cols = 0;
    place(root, 0, cols, pos);
    int maxDepth = 0;
    for (auto& p : pos) maxDepth = max(maxDepth, p.second.first);

    vector<string> rows(2 * (maxDepth + 1), string(cols * cw, ' '));  // строка узлов и строка перекладины на каждый уровень
    auto start = [&](TNode* x) { return pos[x].second * cw + (cw - x->key.size()) / 2; };
    auto center = [&](TNode* x) { return start(x) + x->key.size() / 2; };
    for (auto& p : pos) {
        TNode* x = p.first;
        int r = 2 * p.second.first;
        rows[r].replace(start(x), x->key.size(), x->key);
        if (!x->left && !x->right) continue;
        size_t c = center(x), a = c, b = c;
        if (x->left) a = center(x->left);
        if (x->right) b = center(x->right);
        for (size_t i = a; i <= b; ++i) rows[r + 1][i] = '-';
        rows[r + 1][a] = rows[r + 1][b] = rows[r + 1][c] = '+';
    }
    string out;
    for (auto& r : rows) {
        size_t e = r.find_last_not_of(' ');
        if (e == string::npos) continue;  // пустая нижняя строка
        out = out + r.substr(0, e + 1) + "\n";
    }
    out.pop_back();
    return out;
}
