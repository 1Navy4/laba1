#pragma once
#include "slist.h"

struct Stack : public Container {
private:
    SList l;
public:
    void push(const std::string& v);   // O(1)
    std::string pop();                 // O(1)
    Strs items() const;                // от вершины к низу

    char type() const override { return 'S'; }
    Strs dump() const override;        // от низа к вершине
    void load(const Strs& t) override;
    std::string show() const override;
};
