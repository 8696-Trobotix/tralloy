#include "../include/tralloy_data.h"

#if defined(__EMSCRIPTEN__)
#include <emscripten.h>
#endif
#include <fstream>
#include <cstdlib>
#include <string>

// Please see header file for details.
// Note: Assumes text data, for now...

bool tralloy::data::exists(const char *key) {
    #if defined(__EMSCRIPTEN__)
    int exists, err;
    emscripten_idb_exists(TRALLOY_DB, key, &exists, &err);
    if (exists == 0 || err != 0) return false;
    return true;
    #else
    std::ifstream fin {key};
    if (!fin.is_open()) {
        fin.close();
        return false;
    }
    fin.close();
    return true;
    #endif
}

bool tralloy::data::load(const char *key, std::string *buff, int *bytes) {
    #if defined(__EMSCRIPTEN__)
    void **b {nullptr};
    int err;
    emscripten_idb_load(TRALLOY_DB, key, b, bytes, &err);
    if (err != 0) return false;
    buff->clear();
    for (int i {}; i < *bytes; ++i) *buff += ((char *)(*b))[i];
    free(*b);
    return true;
    #else
    if (!tralloy::data::exists(key)) return false;
    std::ifstream fin {key};
    std::string b;
    while (std::getline(fin, b)) *buff += b;
    *bytes = buff->length();
    fin.close();
    return true;
    #endif
}

bool tralloy::data::store(const char *key, const char *buff, int bytes) {
    #if defined(__EMSCRIPTEN__)
    int err;
    emscripten_idb_store(TRALLOY_DB, key, (void *)buff, bytes, &err);
    if (err != 0) return false;
    return true;
    #else
    std::ofstream fout {key};
    for (int i {}; i < bytes; ++i) fout << ((char *)(buff))[i];
    fout.close();
    return true;
    #endif
}

bool tralloy::data::remove(const char *key) {
    #if defined(__EMSCRIPTEN__)
    int err;
    emscripten_idb_delete(TRALLOY_DB, key, &err);
    if (err != 0) return false;
    return true;
    #else
        #if !defined(WIN32)
        std::string cmd {"rm -f "};
        cmd += key;
        if (system(cmd.c_str()) != 0); // Assume Linux or like OS. Redundant comparison to silent unused return warning.
        #else
        std::string cmd {"if exist "};
        cmd += key;
        cmd += " del /q ";
        cmd += key;
        if (system(cmd.c_str()) != 0); // Assume Windows. Don't know if this command works.
        #endif
    return true;
    #endif
}

// Copyright (c) 2022, Trobotix 8696
// All rights reserved.
// Use of this source code is governed by the BSD 3-Clause License that can be found in the LICENSE file.
