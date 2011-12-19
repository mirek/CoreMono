//
//  CoreMono.c
//  CoreMono Framework
//
//  Copyright (c) 2011 Mirek Rusin <mirek@me.com>
//

#include "CoreMono.h"

void CMAddInternalCall(CFStringRef name, const void *function) {
    IF_UTF8(kCFAllocatorDefault, name, nameUtf8, {
        mono_add_internal_call(nameUtf8, function);
    });
}

MonoObject *CMRuntimeInvoke(CMMethodRef method, MonoObject *monoObject, void *params[], CFErrorRef *error) {
    return mono_runtime_invoke(CMMethodGetMonoMethod(method), monoObject, params, NULL);
}


