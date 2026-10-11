// База данных набор именованных структур + загрузка/сохранение в файл
#pragma once
#include <map>
#include <memory>

#include "container.h"

struct Database {
    std::map<std::string, std::unique_ptr<Container>> items;  // имена хранятся отсортированными

    void load(const std::string& path);        // нет файла база пустая
    void save(const std::string& path) const;  // по строке на контейнер <тип> <имя> <элементы>
};

std::unique_ptr<Container> makeContainer(char type);

// Найти контейнер типа T; при create=true создать, если его нет.
template <typename T>
T& fetch(Database& db, const std::string& name, bool create) {
    auto it = db.items.find(name);
    if (it == db.items.end()) {
        if (!create) fail("no such container: " + name);
        it = db.items.emplace(name, std::make_unique<T>()).first;
    }
    T* p = dynamic_cast<T*>(it->second.get());
    if (!p) fail("container '" + name + "' has a different type");
    return *p;
}
