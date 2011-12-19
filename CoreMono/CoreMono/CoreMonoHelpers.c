//
// CoreMono Framework
//
// (c) 2011 Mirek Rusin <mirek@me.com>
//

#include "CoreMonoHelpers.h"

CFTypeRef CMNullify(CFTypeRef value) {
    return value ? value : kCFNull;
}

int mono_signature_has_param_type_names(MonoMethodSignature *sig, uint32_t param_count, const char **param_type_names) {
    int result = 0;
    MonoType *p = NULL;
    void *iter = NULL;
    int i = 0;
    
    if (param_type_names) {
        if (mono_signature_get_param_count(sig) == param_count) {
            result = 1;
            while (((p = mono_signature_get_params(sig, &iter)))) {
                if (param_type_names[i] == NULL || strcmp(mono_type_get_name(p), param_type_names[i]) != 0) {
                    result = 0;
                    break;
                }
                i++;
            }
        }
    }
    
    return result;
}

int mono_signature_has_return_type_name(MonoMethodSignature *sig, const char *return_type_name) {
    return return_type_name != NULL && strcmp(mono_type_get_name(mono_signature_get_return_type(sig)), return_type_name) == 0; 
}

int mono_signature_has_return_and_param_type_names(MonoMethodSignature *sig, const char *return_type_name, uint32_t param_count, const char **param_type_names) {
    return mono_signature_has_return_type_name(sig, return_type_name) && mono_signature_has_param_type_names(sig, param_count, param_type_names);
}

MonoMethod *mono_class_find_method_matching_param_type_names(MonoClass *klass, const char *name, uint32_t param_count, const char **param_type_names) {
    MonoMethod *method = NULL, *tmp = NULL;
    MonoMethodSignature *sig = NULL;
    void *iter = NULL;
    
    while ((tmp = mono_class_get_methods(klass, &iter))) {
        if (strcmp(mono_method_get_name(tmp), name) == 0) {
            if ((sig = mono_method_signature(tmp))) {
                if (mono_signature_has_param_type_names(sig, param_count, param_type_names)) {
                    method = tmp;
                    break;
                }
            }
        }
    }
    
    return method;
}

MonoMethod *mono_class_find_method_matching_return_and_param_type_names(MonoClass *klass, const char *name, const char *return_type_name, uint32_t param_count, const char **param_type_names) {
    MonoMethod *method = NULL, *tmp = NULL;
    MonoMethodSignature *sig = NULL;
    void *iter = NULL;
    
    while ((tmp = mono_class_get_methods(klass, &iter))) {
        if (strcmp(mono_method_get_name(tmp), name) == 0) {
            if ((sig = mono_method_signature(tmp))) {
                if (mono_signature_has_return_and_param_type_names(sig, return_type_name, param_count, param_type_names)) {
                    method = tmp;
                    break;
                }
            }
        }
    }
    
    return method;
}

MonoMethod *mono_class_find_ctor_matching_param_type_names(MonoClass *klass, uint32_t param_count, const char **param_type_names) {
    MonoMethod *method = NULL, *tmp = NULL;
    void *iter = NULL;
    
    while ((tmp = mono_class_get_methods(klass, &iter))) {
        if (strcmp(mono_method_get_name(tmp), ".ctor") == 0) {
            if (mono_signature_has_param_type_names(mono_method_signature(tmp), param_count, param_type_names)) {
                method = tmp;
                break;
            }
        }
    }
    
    return method;
}

MonoObject *mono_object_init(MonoDomain *domain, MonoClass *klass, uint32_t param_count, const char **param_type_names, void **params, MonoObject **exc) {
    MonoObject *object = NULL;
    MonoMethod *method = NULL;
    
    if ((method = mono_class_find_ctor_matching_param_type_names(klass, param_count, param_type_names))) {
        if ((object = mono_object_new(domain, klass))) {
            mono_runtime_invoke(method, object, params, exc);
        }
    }
    
    return object;
}
