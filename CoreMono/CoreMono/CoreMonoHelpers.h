//
// CoreMono Framework
//
// (c) 2011 Mirek Rusin <mirek@me.com>
//

#ifndef CORE_MONO_HELPERS_H
#define CORE_MONO_HELPERS_H

#include <CoreFoundation/CoreFoundation.h>

#include <stdlib.h>
#include <string.h>

#include <Mono/mono-2.0/mono/jit/jit.h>
#include <Mono/mono-2.0/mono/metadata/object.h>
#include <Mono/mono-2.0/mono/metadata/environment.h>
#include <Mono/mono-2.0/mono/metadata/assembly.h>
#include <Mono/mono-2.0/mono/metadata/debug-helpers.h>

#define CM_UNBOX_DOUBLE(x)  (* (double *)      mono_object_unbox(x))
#define CM_UNBOX_FLOAT(x)   (* (float *)       mono_object_unbox(x))
#define CM_UNBOX_INT64(x)   (* (int64_t *)     mono_object_unbox(x))
#define CM_UNBOX_INT32(x)   (* (int32_t *)     mono_object_unbox(x))
#define CM_UNBOX_INT16(x)   (* (int16_t *)     mono_object_unbox(x))
#define CM_UNBOX_INT8(x)    (* (int8_t *)      mono_object_unbox(x))
#define CM_UNBOX_UINT64(x)  (* (uint64_t *)    mono_object_unbox(x))
#define CM_UNBOX_UINT32(x)  (* (uint32_t *)    mono_object_unbox(x))
#define CM_UNBOX_UINT16(x)  (* (uint16_t *)    mono_object_unbox(x))
#define CM_UNBOX_UINT8(x)   (* (uint8_t *)     mono_object_unbox(x))
#define CM_UNBOX_BOOLEAN(x) (* (MonoBoolean *) mono_object_unbox(x))

#define CM_BOX_DOUBLE(x)  mono_value_box(mono_domain_get(), mono_get_double_class(),  &x)
#define CM_BOX_FLOAT(x)   mono_value_box(mono_domain_get(), mono_get_single_class(),  &x)
#define CM_BOX_INT64(x)   mono_value_box(mono_domain_get(), mono_get_int64_class(),   &x)
#define CM_BOX_INT32(x)   mono_value_box(mono_domain_get(), mono_get_int32_class(),   &x)
#define CM_BOX_INT16(x)   mono_value_box(mono_domain_get(), mono_get_int16_class(),   &x)
#define CM_BOX_INT8(x)    mono_value_box(mono_domain_get(), mono_get_sbyte_class(),   &x)
#define CM_BOX_UINT64(x)  mono_value_box(mono_domain_get(), mono_get_uint64_class(),  &x)
#define CM_BOX_UINT32(x)  mono_value_box(mono_domain_get(), mono_get_uint32_class(),  &x)
#define CM_BOX_UINT16(x)  mono_value_box(mono_domain_get(), mono_get_uint16_class(),  &x)
#define CM_BOX_UINT8(x)   mono_value_box(mono_domain_get(), mono_get_byte_class(),    &x)
#define CM_BOX_BOOLEAN(x) mono_value_box(mono_domain_get(), mono_get_boolean_class(), &x)

#define __CORE_MONO_JOIN(x, y)       __CORE_MONO_JOIN_AGAIN(x, y)
#define __CORE_MONO_JOIN_AGAIN(x, y) x ## y

#define IF_UTF8(allocator, string, utf8, whatever)                                                                                      \
    size_t __CORE_MONO_JOIN(__length_of_, utf8) = -1;                                                                                   \
    char *utf8 = NULL;                                                                                                                  \
    if (CFStringGetLength(string) == 0) {                                                                                               \
        __CORE_MONO_JOIN(__length_of_, utf8) = 0;                                                                                       \
        utf8 = "";                                                                                                                      \
    } else {                                                                                                                            \
        if (NULL == (utf8 = (char *) CFStringGetCStringPtr(string, kCFStringEncodingUTF8))) {                                           \
            __CORE_MONO_JOIN(__length_of_, utf8) = CFStringGetMaximumSizeForEncoding(CFStringGetLength(string), kCFStringEncodingUTF8); \
            if ((utf8 = CFAllocatorAllocate(allocator, __CORE_MONO_JOIN(__length_of_, utf8) , 0))) {                                    \
                if (0 == CFStringGetCString(string, utf8, __CORE_MONO_JOIN(__length_of_, utf8), kCFStringEncodingUTF8)) {               \
                    /* todo */                                                                                                          \
                }                                                                                                                       \
            }                                                                                                                           \
        }                                                                                                                               \
    }                                                                                                                                   \
    do { whatever } while (0);                                                                                                          \
    if (__CORE_MONO_JOIN(__length_of_, utf8) > 0) {                                                                                     \
        if (utf8) {                                                                                                                     \
            CFAllocatorDeallocate(allocator, utf8);                                                                                     \
        }                                                                                                                               \
    }                                                                                                                                   \

// kCFNull'ifies provided value. Returned values is guaranteed not to be NULL. Returned value can be used as a value to be inserted
// in to the collection and/or CFRelease(...).
//
// Return kCFNull if the value is equal to NULL, value otherwise.
CFTypeRef CMNullify(CFTypeRef value);

/*
 * Example:
 *
 *     if (mono_signature_has_param_type_names(sig, 2, (const char *[]) { "System.Int32", "System.String" })) { 
 *         // ...do something with the signature that has int32 and string params...
 *     }
 *
 * Return 1 if the signature matches, 0 otherwise.
 */
int mono_signature_has_param_type_names(MonoMethodSignature *sig, uint32_t param_count, const char **param_names);

/*
 * Example:
 *
 *     if (mono_signature_has_return_type_name(sig, "System.UInt64")) { 
 *         // ...do something with method signature that returns unsigned int64...
 *     }
 *
 * Return 1 if the return type of the signature matches provided type name, 0 otherwise.
 */
int mono_signature_has_return_type_name(MonoMethodSignature *sig, const char *return_type_name);

/*
 * Example:
 *
 *     if (mono_signature_has_return_and_param_type_names(sig, "System.UInt64", 2, (const char *[]) { "System.Int32", "System.String" })) { 
 *         // ...do something with method signature that returns unsigned int64 and has specified parameter types...
 *     }
 *
 * Return 1 if the return type and parameter types match provided type names, 0 otherwise.
 */
int mono_signature_has_return_and_param_type_names(MonoMethodSignature *sig, const char *return_type_name, uint32_t param_count, const char **param_type_names);

/*
 * Perform linear search for a method matching provided name, return type name and param type names. 
 *
 * Example:
 *
 *     int32_t my_number = 65535;
 *     if ((method = mono_class_find_method_matching_return_and_param_type_names(klass, "GetMyString", "System.String", 1, (const char *[]) { "System.Int32" })) { 
 *         MonoObject *string = mono_runtime_invoke(method, NULL, (void *[1]) { &my_number }, NULL);
 *     }
 */
MonoMethod *mono_class_find_method_matching_return_and_param_type_names(MonoClass *klass, const char *name, const char *return_type_name, uint32_t param_count, const char **param_type_names);

/*
 * Perform linear search for a method matching provided name and param type names. 
 */
MonoMethod *mono_class_find_method_matching_param_type_names(MonoClass *klass, const char *name, uint32_t param_count, const char **param_type_names);

/*
 * Perform linear search for a constructor matching specified parameter names.
 */
MonoMethod *mono_class_find_ctor_matching_param_type_names(MonoClass *klass, uint32_t param_count, const char **param_type_names);

/*
 * Get property's get method matching return type name
 */
MonoMethod *mono_class_find_get_method_for_property_matching_type_name(MonoClass *klass, const char *name, const char *type_name);

/*
 * Create object and invoke it's constructor with specified paramer type names and values.
 */
MonoObject *mono_object_init(MonoDomain *domain, MonoClass *klass, uint32_t param_count, const char **param_names, void **params, MonoObject **exc);

#endif
