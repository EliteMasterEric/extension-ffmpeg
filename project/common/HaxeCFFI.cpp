#include "HaxeCFFI.h"

/**
 * @brief Initialize a buffer from a pointer and length.
 */
buffer cffi_build_buffer(unsigned char* data, int length)
{
    // A zany, wacky little hack.
    // There isn't a proper way to do this in CFFI.
    buffer out_buffer = alloc_buffer_len(0);
    Array_obj<unsigned char>* out_array = (Array_obj<unsigned char>*)out_buffer;
    out_array->setData((void*)data, length);

    return out_buffer;
}
