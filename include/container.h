// Общий интерфейс всех структур: запись в файл (dump), чтение (load), вывод (show).
#pragma once
#include "common.h"

struct Container {
    virtual ~Container() {}
    virtual char type() const = 0;
    virtual Strs dump() const = 0;
    virtual void load(const Strs& tokens) = 0;
    virtual std::string show() const = 0;
};
