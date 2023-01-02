/*

Could not get IDBFS file system to function as desired.
These functions provide a "kind of normal" file-based storage when native, 
and uses synchronous calls to IndexedDB when in WebAssembly.

Update: IDBFS probably didn't work because the boolean, which signals when the file system has asyncronously loaded, was not declared volatile.

*/
#ifndef TRALLOY_DATA
#define TRALLOY_DATA

#include <string>

#define TRALLOY_DB "DATA"
#define DEFAULT_FILE_EXTENSION ".txt"

namespace tralloy {
    namespace data {
        // Check if key exists.
        bool exists(const std::string &key);

        // Loads data. Return true on success, false otherwise. Data is written into its respective arguments.
        bool load(const std::string &key, std::string &buff);

        // Stores data. Creates the file / key if it does not exist. Returns true on success, false otherwise.
        bool store(const std::string &key, const std::string &data);

        // Deletes a key. Returns true on success, false otherwise.
        // Native file deletion needs to be revised to not use system commands.
        // Clarification: Returns true even if key / file doesn't exist. Returns false when an error is encountered.
        bool remove(const std::string &key);
    }
}

#endif

// Copyright (c) 2022, Trobotix 8696
// All rights reserved.
// Use of this source code is governed by the BSD 3-Clause License that can be found in the LICENSE file.
