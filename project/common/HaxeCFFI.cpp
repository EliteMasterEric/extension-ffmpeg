#include "HaxeCFFI.h"

/**
 * @brief A crash occurs when calling `alloc_buffer_len` from a thread.
 *        This provides a workaround.
 *        `Bad local allocator - requesting memory from unregistered thread!`
 */
buffer cffi_safe_alloc_buffer_len(int length)
{
    int base = 0;
    gc_set_top_of_stack(&base, true);
    buffer out_buffer = alloc_buffer_len(length);
    gc_set_top_of_stack((int *)0, true);
    return out_buffer;
}

/**
 * @brief Initialize a buffer from a pointer and length.
 *        The provided data is copied into the newly formed buffer.
 *
 * TODO: Can we improve the efficiency of this?
 */
buffer cffi_safe_build_buffer(unsigned char *data, int length)
{
    if (length <= 0) {
        // printf("[extension-ffmpeg] WARNING: Invalid input data.\n");
        return cffi_safe_alloc_buffer_len(0);
    }

    // Allocate a buffer of the appropriate size.
    // printf("Creating buffer for data...\n");
    buffer out_buffer = cffi_safe_alloc_buffer_len(length);

    // Get a pointer to the data.
    // printf("Retrieving pointer to buffer data...\n");
    unsigned char *out_data = (unsigned char *)buffer_data(out_buffer);

    // Copy the data into the buffer.
    // printf("[extension-ffmpeg] Copying %d bytes of data into buffer, SAFELY.\n", length);
    memcpy(out_data, data, length);

    return out_buffer;
}

/**
 * @brief Initialize a buffer from a pointer and length.
 *        The provided data is copied into the newly formed buffer.
 *
 * TODO: Can we improve the efficiency of this?
 */
buffer cffi_build_buffer(unsigned char *data, int length)
{
    if (length <= 0) {
        // printf("[extension-ffmpeg] WARNING: Invalid input data.\n");
        return alloc_buffer(0);
    }

    // Allocate a buffer of the appropriate size.
    // printf("Creating buffer for data...\n");
    buffer out_buffer = alloc_buffer_len(length);

    // Get a pointer to the data.
    // printf("Retrieving pointer to buffer data...\n");
    unsigned char *out_data = (unsigned char *)buffer_data(out_buffer);

    // Copy the data into the buffer.
    // printf("[extension-ffmpeg] Copying %d bytes of data into buffer, UNSAFELY.\n", length);
    memcpy(out_data, data, length);

    return out_buffer;
}

/**
 * @brief Call a callback function with one argument.
 *        A crash occurs when calling `val_call1` from a thread.
 *        This provides a hacky workaround.
 *
 * @param operation The operation to perform while being recognized by Haxe.
 * @param arg The argument to pass to the operation.
 * @see https://haxe.org/manual/target-cpp-ThreadsAndStacks.html
 * @see https://community.haxe.org/t/hxcpp-bad-local-allocator-requesting-memory-from-unregistered-thread/2675
 */
void cffi_safe_call_1(value operation, value arg)
{

    // Foreign threads must be attached-detached.
    // gc_set_top_of_stack(int * inTop,bool inPush)
    // - inTopOfStack - pointer to top of stack to attach, or 0 to remove stack.
    // - inForce - usually true. Recursive attachment/detachment.
    // printf("[extension-ffmpeg] Setting top of stack...\n");
    int base = 0;
    gc_set_top_of_stack(&base, true);

    // printf("[extension-ffmpeg] Calling safe operation...\n");
    // Call the operation.
    val_call1(operation, arg);

    // Set pointer to 0 to detach the foreign thread.
    // printf("[extension-ffmpeg] Clearing top of stack...\n");
    gc_set_top_of_stack((int *)0, true);
}

/**
    // This DOES NOT WORK because Haxe reports that the pointer does not belong to the Haxe context.
    // This means we have to perform a memory copy (hopefully there's a workaround because that's expensive)

    // A zany, wacky little hack.
    // There isn't a proper way to do this in CFFI.
    buffer out_buffer = alloc_buffer_len(0);
    Array_obj<unsigned char>* out_array = (Array_obj<unsigned char>*)out_buffer;
    out_array->setData((void*)data, length);
**/