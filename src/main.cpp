#include <iostream>

#include "commands.h"

using namespace std;

int main(int argc, char** argv) {
    string file = "file.data", query;
    bool hasQuery = false;
    for (int i = 1; i + 1 < argc; i = i + 2) {
        string key = argv[i];
        if (key == "--file") file = argv[i + 1];
        if (key == "--query") { query = argv[i + 1]; hasQuery = true; }
    }
    if (!hasQuery) { cerr << "usage: " << argv[0] << " --file file.data --query 'COMMAND name args'\n"; return 2; }
    try {
        Database db;
        db.load(file);
        Strs a = tokenize(query);
        if (a.empty()) fail("empty query");
        cout << execute(db, a) << endl;
        if (!readOnly(upper(a[0]))) db.save(file); 
    } catch (const exception& e) {
        cerr << "ERROR: " << e.what() << endl;
        return 1;
    }
}
