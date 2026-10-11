#include "database.h"

#include <fstream>

#include "dlist.h"
#include "massiv.h"
#include "queue.h"
#include "slist.h"
#include "stack.h"
#include "tree.h"

using namespace std;

unique_ptr<Container> makeContainer(char t) {
    switch (t) {
        case 'M': return make_unique<Array>();
        case 'F': return make_unique<SList>();
        case 'L': return make_unique<DList>();
        case 'S': return make_unique<Stack>();
        case 'Q': return make_unique<Queue>();
        case 'T': return make_unique<CBTree>();
    }
    fail(string("unknown container type in file: ") + t);
}

void Database::load(const string& path) {
    ifstream in(path);
    for (string line; getline(in, line);) {
        Strs tok = tokenize(line);
        if (tok.size() < 2 || tok[0].empty()) continue;
        string name = tok[1];
        items[name] = makeContainer(tok[0][0]);
        items[name]->load(Strs(tok.begin() + 2, tok.end()));
    }
}

void Database::save(const string& path) const {
    ofstream out(path);
    if (!out) fail("cannot write file: " + path);
    for (auto& [name, c] : items) {
        out << c->type() << ' ' << quote(name);
        for (auto& s : c->dump()) out << ' ' << quote(s);  // строки с пробелами идут в кавычках
        out << '\n';
    }
}
