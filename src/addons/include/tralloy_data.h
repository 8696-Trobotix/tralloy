/*

Could not get IDBFS file system to function as desired.
These functions provide a "kind of normal" file-based storage when native, 
and uses synchronous calls to IndexedDB when in WebAssembly.

*/
#ifndef TRALLOY_DATA
#define TRALLOY_DATA

#include <string>

#define TRALLOY_DB "DATA"

namespace tralloy {
    namespace data {
        // Check if key exists.
        bool exists(const char *key);

        // Loads data. Return true on success, false otherwise. Data is written into its respective arguments.
        bool load(const char *key, std::string *buff, int *bytes);

        // Stores data. Creates the file / key if it does not exist. Returns true on success, false otherwise.
        bool store(const char *key, const char *buff, int bytes);

        // Deletes a key. Returns true on success, false otherwise.
        // Native file deletion needs to be revised to not use system commands.
        // Clarification: Returns true even if key / file doesn't exist. Returns false when an error is encountered.
        bool remove(const char *key);
    }
}

#endif

// Copyright (c) 2022, Trobotix 8696
// All rights reserved.
// Use of this source code is governed by the BSD 3-Clause License that can be found in the LICENSE file.
