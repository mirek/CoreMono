//
//  CMClass.c
//  CoreMono
//
//  Created by Mirek Rusin on 14/12/2011.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include "CMClass.h"

CMClassRef CMClassCreate(CFAllocatorRef allocator) {
    CMClassRef klass = CFAllocatorAllocate(allocator, sizeof(__CMClassType), 0);
    if (klass) {
        memset(klass, 0, sizeof(__CMClassType));
        klass->allocator = allocator ? CFRetain(allocator) : NULL;
        klass->retainCount = 1;
    }
    return klass;
}

CMClassRef CMClassCreateWithAssembly(CMAssemblyRef assembly, CFStringRef namespace_, CFStringRef name) {
    CMClassRef klass = NULL;
    if (assembly) {
        if ((klass = CMClassCreate(assembly->allocator))) {
            IF_UTF8(klass->allocator, name, nameUtf8, {
                if (namespace_) {
                    IF_UTF8(klass->allocator, namespace_, namespaceUtf8, {
                        printf("%p %s %s\n", CMAssemblyGetMonoImage(assembly), namespaceUtf8, nameUtf8);
                        klass->monoClass = mono_class_from_name(CMAssemblyGetMonoImage(assembly), namespaceUtf8, nameUtf8);
                    });
                } else {
                    //klass->monoClass = mono_class_from_name(CMAssemblyGetMonoImage(assembly), NULL, nameUtf8);
                }
            });
        }
    }
    return klass;
}

CMClassRef CMClassCreateWithImage(CMImageRef image, CFStringRef namespace_, CFStringRef name) {
    CMClassRef klass = NULL;
    if (image) {
        klass = CFAllocatorAllocate(image->allocator, sizeof(__CMClassType), 0);
        memset(klass, 0, sizeof(__CMClassType));
        if (klass) {
            klass->allocator = image->allocator ? CFRetain(image->allocator) : NULL;
            klass->retainCount = 1;
            IF_UTF8(image->allocator, name, nameUtf8, {
                if (namespace_) {
                    IF_UTF8(image->allocator, namespace_, namespaceUtf8, {
                        klass->monoClass = mono_class_from_name(image->monoImage, namespaceUtf8, nameUtf8);
                    });
                } else {
                    klass->monoClass = mono_class_from_name(image->monoImage, NULL, nameUtf8);
                }
            });
        }
    }
    return klass;
}

MonoClass *CMClassGetMonoClass(CMClassRef klass) {
    MonoClass *monoClass = NULL;
    if (klass) {
        monoClass = klass->monoClass;
    }
    return monoClass;
}