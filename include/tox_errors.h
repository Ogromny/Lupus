#pragma once

#include "toxcore/tox.h"
#include "toxencryptsave/toxencryptsave.h"

#define tox_error_message(err)                               \
    _Generic((err), Tox_Err_New                              \
             : tox_error_message_err_new, Tox_Err_Encryption \
             : tox_error_message_err_encryption)(err)

char const *tox_error_message_err_new(Tox_Err_New err);
char const *tox_error_message_err_encryption(Tox_Err_Encryption err);
