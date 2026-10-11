#pragma once
#include "container.h"

struct Array : public Container {
private:
    std::string* data = new std::string[4];
    size_t len = 0, cap = 4;
    void check(size_t i) const;
public:
    Array() {}
    Array(const Array&) = delete;
    Array& operator=(const Array&) = delete;
    ~Array() { delete[] data; }

    size_t size() const { return len; }
    void insert(size_t idx, const std::string& v);   // O(n)
    void push(const std::string& v);                 // O(1) амортизированно
    const std::string& get(size_t i) const;          // O(1)
    void set(size_t i, const std::string& v);        // O(1)
    std::string erase(size_t i);                     // O(n)

    char type() const override { return 'M'; }
    Strs dump() const override;
    void load(const Strs& t) override;
    std::string show() const override;
};
