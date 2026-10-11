#pragma once
#include "database.h"

std::string execute(Database& db, const Strs& a);  // a - токены запроса, a[0] - команда
bool readOnly(const std::string& cmd);             // true - команда не меняет файл
