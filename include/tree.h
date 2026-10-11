#pragma once
#include <map>
#include <utility>
#include <vector>

#include "container.h"

struct TNode { std::string key; TNode *left, *right; };

struct CBTree : public Container {
private:
    TNode* root = nullptr;
    static void destroy(TNode* x);
    std::vector<TNode*> nodes() const;
    void preorder(TNode* x, Strs& o) const;
    void inorder(TNode* x, Strs& o) const;
    void postorder(TNode* x, Strs& o) const;
    void dumpRec(TNode* x, Strs& o) const;
    TNode* build(const Strs& t, size_t& i);
    // Раскладка для вертикального вывода: узел -> (глубина, номер колонки по симметричному обходу)
    void place(TNode* x, int depth, int& col, std::map<TNode*, std::pair<int, int>>& pos) const;
public:
    CBTree() {}
    CBTree(const CBTree&) = delete;
    CBTree& operator=(const CBTree&) = delete;
    ~CBTree() { destroy(root); }

    std::string insert(const std::string& k);      
    bool contains(const std::string& k) const;      
    bool isComplete() const;                      
    Strs traverse(const std::string& order) const;  
    char type() const override { return 'T'; }
    Strs dump() const override;
    void load(const Strs& t) override;
    std::string show() const override;
};
