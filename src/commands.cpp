#include "commands.h"

#include "dlist.h"
#include "massiv.h"
#include "queue.h"
#include "slist.h"
#include "stack.h"
#include "tree.h"

using namespace std;

// Односвязный (F) и двусвязный (L) списки имеют одинаковые операции
template <typename L>
static string listCommand(L& l, const string& op, const Strs& a) {
    if (op == "PUSH" || op == "PUSHTAIL") { l.pushTail(arg(a, 2)); return a[2]; }
    if (op == "PUSHHEAD") { l.pushHead(arg(a, 2)); return a[2]; }
    if (op == "PUSHBEFORE") { l.pushBefore(arg(a, 2), arg(a, 3)); return a[3]; }  // name target value
    if (op == "PUSHAFTER") { l.pushAfter(arg(a, 2), arg(a, 3)); return a[3]; }
    if (op == "DEL") { if (!l.delValue(arg(a, 2))) fail("value not found: " + a[2]); return a[2]; }
    if (op == "DELHEAD") return l.popHead();
    if (op == "DELTAIL") return l.popTail();
    if (op == "DELBEFORE") return l.delBefore(arg(a, 2));
    if (op == "DELAFTER") return l.delAfter(arg(a, 2));
    if (op == "GET") return a.size() > 2 ? l.at(toIndex(a[2])) : spaced(l.items());
    if (op == "FIND") { int i = l.find(arg(a, 2)); return i < 0 ? "NOT FOUND" : "FOUND at index " + to_string(i); }
    fail("unknown command: " + a[0]);
}

string execute(Database& db, const Strs& a) {
    string cmd = upper(a[0]);
    const string& name = arg(a, 1);
    if (cmd == "PRINT") {
        auto it = db.items.find(name);
        if (it == db.items.end()) fail("no such container: " + name);
        return it->second->show();
    }
    if (cmd == "ISMEMBER") return fetch<CBTree>(db, name, false).contains(arg(a, 2)) ? "TRUE" : "FALSE";

    bool create = cmd.find("PUSH") != string::npos || cmd == "TINSERT";
    switch (cmd[0]) {
        case 'M': {
            Array& m = fetch<Array>(db, name, create);
            if (cmd == "MPUSH") {  // MPUSH name value [index]
                if (a.size() > 3) m.insert(toIndex(a[3]), arg(a, 2)); else m.push(arg(a, 2));
                return a[2];
            }
            if (cmd == "MGET") return a.size() > 2 ? m.get(toIndex(a[2])) : spaced(m.dump());  
            if (cmd == "MSET") { m.set(toIndex(arg(a, 2)), arg(a, 3)); return a[3]; }         
            if (cmd == "MDEL") return m.erase(toIndex(arg(a, 2)));                     
            if (cmd == "MLEN") return to_string(m.size());
            break;
        }
        case 'F': return listCommand(fetch<SList>(db, name, create), cmd.substr(1), a);
        case 'L': {
            DList& l = fetch<DList>(db, name, create);
            if (cmd == "LGETREV") return spaced(l.itemsReversed());  // чтение с конца - только у двусвязного
            return listCommand(l, cmd.substr(1), a);
        }
        case 'S': {
            Stack& s = fetch<Stack>(db, name, create);
            if (cmd == "SPUSH") { s.push(arg(a, 2)); return a[2]; }
            if (cmd == "SPOP") return s.pop();
            if (cmd == "SGET") return spaced(s.items());  // чтение без удаления
            break;
        }
        case 'Q': {
            Queue& q = fetch<Queue>(db, name, create);
            if (cmd == "QPUSH") { q.push(arg(a, 2)); return a[2]; }
            if (cmd == "QPOP") return q.pop();
            if (cmd == "QGET") return spaced(q.items());  // чтение без удаления
            break;
        }
        case 'T': {
            CBTree& t = fetch<CBTree>(db, name, create);
            if (cmd == "TINSERT") return t.insert(arg(a, 2));
            if (cmd == "TGET") return spaced(t.traverse(a.size() > 2 ? upper(a[2]) : "BFS"));
            if (cmd == "TISCOMPLETE") return t.isComplete() ? "TRUE" : "FALSE"; 
            break;
        }
    }
    fail("unknown command: " + a[0]);
}

bool readOnly(const string& c) {
    return c == "PRINT" || c == "ISMEMBER" || endsWith(c, "GET") || endsWith(c, "FIND") || endsWith(c, "LEN") ||
           endsWith(c, "COMPLETE") || endsWith(c, "GETREV");
}
