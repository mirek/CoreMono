//
//  mono_ext.h
//  CoreMono Framework
//
//  Copyright (c) 2011 Mirek Rusin <mirek@me.com>
//

#ifndef CORE_MONO_MONO_EXT_H
#define CORE_MONO_MONO_EXT_H

#include <stdlib.h>
#include <string.h>

#include <Mono/mono-2.0/mono/jit/jit.h>
#include <Mono/mono-2.0/mono/metadata/object.h>
#include <Mono/mono-2.0/mono/metadata/environment.h>
#include <Mono/mono-2.0/mono/metadata/assembly.h>
#include <Mono/mono-2.0/mono/metadata/debug-helpers.h>

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
 * Create object and invoke it's constructor with specified paramer type names and values.
 */
MonoObject *mono_object_init(MonoDomain *domain, MonoClass *klass, uint32_t param_count, const char **param_names, void **params, MonoObject **exc);

#endif
