#pragma once
#include "container.h"

struct DNode { std::string val; DNode *prev, *next; };

struct DList : public Container {
private:
    DNode *head = nullptr, *tail = nullptr;
    size_t n = 0;
    DNode* findNode(const std::string& v) const;
    DNode* mustFind(const std::string& v) const;
    std::string unlink(DNode* x);               // O(1)
    void insertAfter(DNode* t, const std::string& v);
public:
    DList() {}
    DList(const DList&) = delete;
    DList& operator=(const DList&) = delete;
    ~DList();

    void pushHead(const std::string& v);
    void pushTail(const std::string& v);
    void pushAfter(const std::string& target, const std::string& v); 
    void pushBefore(const std::string& target, const std::string& v); 
    std::string popHead();
    std::string popTail();
    std::string delAfter(const std::string& target);
    std::string delBefore(const std::string& target);
    bool delValue(const std::string& v);
    int find(const std::string& v) const;
    const std::string& at(size_t i) const;
    Strs items() const;
    Strs itemsReversed() const;

    char type() const override { return 'L'; }
    Strs dump() const override { return items(); }
    void load(const Strs& t) override;
    std::string show() const override;
};
