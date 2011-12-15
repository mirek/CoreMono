//
//  CMMethodDesc.c
//  CoreMono
//
//  Created by Mirek Rusin on 12/12/2011.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include "CMMethodDesc.h"

CMMethodDescRef CMMethodDescCreate(CFAllocatorRef allocator, CFStringRef name, bool useNamespace) {
    CMMethodDescRef methodDesc = NULL;
    if (name) {
        methodDesc = CFAllocatorAllocate(allocator, sizeof(__CMMethodDescType), 0);
        if (methodDesc) {
            methodDesc->allocator = allocator ? CFRetain(allocator) : NULL;
            methodDesc->retainCount = 1;
            methodDesc->useNamespace = useNamespace;
            methodDesc->name = CFRetain(name);
            IF_UTF8(allocator, name, utf8, {
                if (NULL == (methodDesc->monoMethodDesc = mono_method_desc_new(utf8, useNamespace))) {
                    // TODO: error
                }
            });
        }
    }
    return methodDesc;
}

CMMethodDescRef CMMethodDescRetain(CMMethodDescRef methodDesc) {
    if (methodDesc) {
        methodDesc->retainCount++;
    }
    return methodDesc;
}

CMMethodDescRef CMMethodDescRelease(CMMethodDescRef methodDesc) {
    if (methodDesc) {
        if (--methodDesc->retainCount == 0) {
            mono_method_desc_free(methodDesc->monoMethodDesc);
            // TODO: dealloc
        }
    }
    return methodDesc;
}

MonoMethodDesc *CMMethodDescGetMonoMethodDesc(CMMethodDescRef methodDesc) {
    MonoMethodDesc *monoMethodDesc = NULL;
    if (methodDesc) {
        monoMethodDesc = methodDesc->monoMethodDesc;
    }
    return monoMethodDesc;
}
