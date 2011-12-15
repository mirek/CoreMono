//
//  CMMethod.c
//  CoreMono
//
//  Created by Mirek Rusin on 12/12/2011.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include "CMMethod.h"

//CMObjectRef CMMethodInvoke(CMMethodRef method, MonoObject *object, void *args, CFErrorRef *error) {
//    if (method) {
//        MonoObject *monoExceptionObject = NULL;
//        MonoObject *monoResultObject = mono_runtime_invoke(method->monoMethod, object, args, &monoExceptionObject);
//    }
//}

CMMethodRef CMMethodCreate(CFAllocatorRef allocator) {
    CMMethodRef method = CFAllocatorAllocate(allocator, sizeof(__CMMethodType), 0);
    if (method) {
        memset(method, 0, sizeof(__CMMethodType));
        method->allocator = allocator ? CFRetain(allocator) : NULL;
        method->retainCount = 1;
    }
    return method;
}

//CMMethodRef CMMethodCreateWithName(CFAllocatorRef allocator, CFStringRef name, bool useNamespace) {
//    CMMethodRef method = NULL;
//    if (name) {
//        if ((method = CMMethodCreate(allocator))) {
//            method->useNamespace = useNamespace;
//            if (name) {
//                method->name = CFRetain(name);
//                IF_UTF8(method->allocator, name, nameUtf8, {
//                    method->monoMethod = mono_method_
//                });
//            }
//        }
//    }
//    return method;
//}

MonoMethod *CMMethodGetMonoMethod(CMMethodRef method) {
    MonoMethod *monoMethod = NULL;
    if (method) {
        monoMethod = method->monoMethod;
    }
    return monoMethod;
}

CFArrayRef CMCreateArrayWithMonoStringArray(CFAllocatorRef allocator, MonoArray *monoArray) {
    CFArrayRef array = NULL;
    if (monoArray) {
        uintptr_t n = mono_array_length(monoArray);
        CFTypeRef *values = CFAllocatorAllocate(allocator, n * sizeof(CFTypeRef), 0);
        if (values) {
            
            for (uintptr_t i = 0; i < n; i++) {
                values[i] = kCFNull;
                MonoString *monoString = mono_array_get(monoArray, MonoString*, i);
                if (monoString) {
                    char *utf8MonoString = mono_string_to_utf8(monoString);
                    if (utf8MonoString) {
                        CFStringRef string = CFStringCreateWithCString(allocator, utf8MonoString, kCFStringEncodingUTF8);
                        if (string) {
                            values[i] = string;
                        }
                        mono_free(utf8MonoString);
                    }
                }
            }
            
            array = CFArrayCreate(allocator, values, n, &kCFTypeArrayCallBacks);
            
            for (uintptr_t i = 0; i < n; i++) {
                if (values[i] != kCFNull) {
                    CFRelease(values[i]);
                }
            }
            
            CFAllocatorDeallocate(allocator, values);
        }
    }
    return array;
}
