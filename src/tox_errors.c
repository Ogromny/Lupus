#include "tox_errors.h"

#define ERR_NULL "One of the arguments to the function was NULL when it was not expected."

char const *
tox_error_message_err_new(Tox_Err_New err)
{
    switch (err) {
    case TOX_ERR_NEW_OK:
        return NULL;
    case TOX_ERR_NEW_NULL:
        return ERR_NULL;
    case TOX_ERR_NEW_MALLOC:
        return "The function was unable to allocate enough memory to store the events_alloc "
               "structures for the Tox object.";
    case TOX_ERR_NEW_PORT_ALLOC:
        return "The function was unable to bind to a port. This may mean that all ports have "
               "already been bound, e.g. by other Tox instances, or it may mean a permission "
               "error. You may be able to gather more information from errno.";
    case TOX_ERR_NEW_PROXY_BAD_TYPE:
        return "proxy_type was invalid.";
    case TOX_ERR_NEW_PROXY_BAD_HOST:
        return "proxy_type was valid but the proxy_host passed had an invalid format or was NULL.";
    case TOX_ERR_NEW_PROXY_BAD_PORT:
        return "proxy_type was valid, but the proxy_port was invalid.";
    case TOX_ERR_NEW_PROXY_NOT_FOUND:
        return "The proxy address passed could not be resolved.";
    case TOX_ERR_NEW_LOAD_ENCRYPTED:
        return "The byte array to be loaded contained an encrypted save.";
    case TOX_ERR_NEW_LOAD_BAD_FORMAT:
        return "The data format was invalid. This can happen when loading data that was saved by "
               "an older version of Tox, or when the data has been corrupted. When loading from "
               "badly formatted data, some data may have been loaded, and the rest is discarded. "
               "Passing an invalid length parameter also causes this error.";
    }

    return NULL;
}

char const *
tox_error_message_err_encryption(Tox_Err_Encryption err)
{
    switch (err) {
    case TOX_ERR_ENCRYPTION_OK:
        return NULL;
    case TOX_ERR_ENCRYPTION_NULL:
        return ERR_NULL;
    case TOX_ERR_ENCRYPTION_KEY_DERIVATION_FAILED:
        return "The crypto lib was unable to derive a key from the given passphrase, which is "
               "usually a lack of memory issue. The functions accepting keys do not produce this "
               "error.";
    case TOX_ERR_ENCRYPTION_FAILED:
        return "The encryption itself failed.";
    }

    return NULL;
}
