#include "../include/tralloy_data.h"

#if defined(__EMSCRIPTEN__)
#include <emscripten.h>
#endif
#include <fstream>
#include <string>

// Please see header file for details.

bool tralloy::data::exists(const std::string &key) {
    #if defined(__EMSCRIPTEN__)
    int exists, err;
    emscripten_idb_exists(TRALLOY_DB, 
                          (key + DEFAULT_FILE_EXTENSION).c_str(), 
                          &exists, 
                          &err);
    if (exists == 0 || err != 0) return false;
    return true;
    #else
    std::ifstream fin {key + DEFAULT_FILE_EXTENSION};
    if (!fin.is_open()) {
        fin.close();
        return false;
    }
    fin.close();
    return true;
    #endif
}

bool tralloy::data::load(const std::string &key, std::string &buff) {
    #if defined(__EMSCRIPTEN__)
    void **b {nullptr};
    int err, bytes;
    emscripten_idb_load(TRALLOY_DB, 
                        (key + DEFAULT_FILE_EXTENSION).c_str(), 
                        b, 
                        &bytes, 
                        &err);
    if (err != 0) return false;
    buff.assign((char *)(*b), bytes);
    free(*b);
    return true;
    #else
    if (!tralloy::data::exists(key)) return false;
    std::ifstream fin {key + DEFAULT_FILE_EXTENSION};
    std::string b;
    while (std::getline(fin, b)) buff += b;
    fin.close();
    return true;
    #endif
}

bool tralloy::data::store(const std::string &key, const std::string &buff) {
    #if defined(__EMSCRIPTEN__)
    int err;
    emscripten_idb_store(TRALLOY_DB, 
                         (key + DEFAULT_FILE_EXTENSION).c_str(), 
                         (void *)buff.c_str(), 
                         buff.length(), 
                         &err);
    if (err != 0) return false;
    return true;
    #else
    std::ofstream fout {key + DEFAULT_FILE_EXTENSION};
    fout << buff;
    fout.close();
    return true;
    #endif
}

bool tralloy::data::remove(const std::string &key) {
    #if defined(__EMSCRIPTEN__)
    int err;
    emscripten_idb_delete(TRALLOY_DB, 
                          (key + DEFAULT_FILE_EXTENSION).c_str(), 
                          &err);
    if (err != 0) return false;
    return true;
    #else
        #if !defined(WIN32)
        std::string cmd {"rm -f " + key + DEFAULT_FILE_EXTENSION};
        if (system(cmd.c_str()) != 0) {}; // Assume Linux or like OS. Redundant comparison to silent unused return warning.
        #else
        std::string cmd {"if exist " + key + DEFAULT_FILE_EXTENSION + " del /q" + key + DEFAULT_FILE_EXTENSION};
        if (system(cmd.c_str()) != 0) {}; // Assume Windows. Don't know if this command works.
        #endif
    return true;
    #endif
}

// Copyright (c) 2022, Trobotix 8696
// All rights reserved.
// Use of this source code is governed by the BSD 3-Clause License that can be found in the LICENSE file.
