// Очередь (Q) строк на основе односвязного списка (начало - голова, конец - хвост).
#pragma once
#include "slist.h"

struct Queue : public Container {
private:
    SList l;
public:
    void push(const std::string& v);   // O(1)
    std::string pop();                 // O(1)
    Strs items() const;                // от начала к концу

    char type() const override { return 'Q'; }
    Strs dump() const override { return l.items(); }
    void load(const Strs& t) override;
    std::string show() const override;
};
