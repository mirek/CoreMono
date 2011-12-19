//
//  mono_ext.h
//  CoreMono
//
//  Created by Mirek Rusin on 19/12/2011.
//  Copyright (c) 2011 Inteliv Ltd. All rights reserved.
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

int mono_signature_has_param_type_names(MonoMethodSignature *sig, uint32_t param_count, const char **param_names);
int mono_signature_has_return_type_name(MonoMethodSignature *sig, const char *return_type_name);
int mono_signature_has_return_and_param_type_names(MonoMethodSignature *sig, const char *return_type_name, uint32_t param_count, const char **param_type_names);

MonoMethod *mono_class_find_method_matching_return_and_param_type_names(MonoClass *klass, const char *name, const char *return_type_name, uint32_t param_count, const char **param_type_names);

MonoMethod *mono_class_find_method_matching_param_type_names(MonoClass *klass, const char *name, uint32_t param_count, const char **param_type_names);

MonoMethod *mono_class_find_ctor_matching_param_type_names(MonoClass *klass, uint32_t param_count, const char **param_type_names);

MonoObject *mono_object_init(MonoDomain *domain, MonoClass *klass, uint32_t param_count, const char **param_names, void **params, MonoObject **exc);

#endif
