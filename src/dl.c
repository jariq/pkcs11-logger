/*
 *  Copyright 2011-2025 The Pkcs11Interop Project
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*
 *  Written for the Pkcs11Interop project by:
 *  Jaroslav IMRICH <jimrich@jimrich.sk>
 */


#include "pkcs11-logger.h"


 // Platform dependend function that loads dynamic library
DLHANDLE pkcs11_logger_dl_open(const char* library)
{
    DLHANDLE handle = NULL;

    pkcs11_logger_log_with_timestamp("Loading library \"%s\"", library);

#ifdef _WIN32

    DWORD flags = 0;
    DWORD error = 0;

    if (CK_TRUE == pkcs11_logger_utils_path_is_absolute(library))
        flags = LOAD_WITH_ALTERED_SEARCH_PATH;

    handle = LoadLibraryExA(library, NULL, flags);
    if (NULL == handle)
    {
        error = GetLastError();
        pkcs11_logger_log_with_timestamp("Unable to load library. Error: %0#10x", error);
    }
    else
    {
        pkcs11_logger_log_with_timestamp("Successfully loaded library");
    }

#else

    char* error = NULL;

    handle = dlopen(library, RTLD_NOW | RTLD_LOCAL);
    if (NULL == handle)
    {
        error = dlerror();
        if (NULL != error)
        {
            pkcs11_logger_log_with_timestamp("Unable to load library. Error: %s", error);
        }
        else
        {
            pkcs11_logger_log_with_timestamp("Unable to load library");
        }
    }
    else
    {
        pkcs11_logger_log_with_timestamp("Successfully loaded library");
    }

#endif

    return handle;
}


// Platform dependend function that gets function pointer from dynamic library
void *pkcs11_logger_dl_sym(DLHANDLE library, const char *function)
{
    void *address = NULL;

    pkcs11_logger_log_with_timestamp("Retrieving function pointer for %s", function);

#ifdef _WIN32

    DWORD error = 0;

    address = (void*) GetProcAddress(library, function);
    if (NULL == address)
    {
        error = GetLastError();
        pkcs11_logger_log_with_timestamp("Unable to retrieve function pointer. Error: %0#10x", error);
    }
    else
    {
        pkcs11_logger_log_with_timestamp("Successfully retrieved function pointer");
    }

#else

    char* error = NULL;

    address = dlsym(library, function);
    if (NULL == address)
    {
        error = dlerror();
        if (NULL != error)
        {
            pkcs11_logger_log_with_timestamp("Unable to retrieve function pointer. Error: %s", error);
        }
        else
        {
            pkcs11_logger_log_with_timestamp("Unable to retrieve function pointer");
        }
    }
    else
    {
        pkcs11_logger_log_with_timestamp("Successfully retrieved function pointer");
    }

#endif

    return address;
}


// Platform dependend function that unloads dynamic library
int pkcs11_logger_dl_close(DLHANDLE library)
{
    int rv = 0;

    pkcs11_logger_log_with_timestamp("Unloading library");

#ifdef _WIN32

    DWORD error = 0;

    rv = FreeLibrary(library);
    if (0 == rv)
    {
        error = GetLastError();
        pkcs11_logger_log_with_timestamp("Unable to unload library. Error: %0#10x", error);
    }
    else
    {
        pkcs11_logger_log_with_timestamp("Successfully unloaded library");
    }

#else

    char* error = NULL;

    rv = dlclose(library);
    if (0 == rv)
    {
        error = dlerror();
        if (NULL != error)
        {
            pkcs11_logger_log_with_timestamp("Unable to unload library. Error: %s", error);
        }
        else
        {
            pkcs11_logger_log_with_timestamp("Unable to unload library");
        }
    }
    else
    {
        pkcs11_logger_log_with_timestamp("Successfully unloaded library");
    }

#endif

    return rv;
}
