//
//  CMImage.c
//  CoreMono
//
//  Created by Mirek Rusin on 12/12/2011.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include "CMImage.h"

CMImageRef CMImageCreate(CFAllocatorRef allocator) {
    CMImageRef image = CFAllocatorAllocate(allocator, sizeof(__CMImageType), 0);
    if (image) {
        memset(image, 0, sizeof(__CMImageType));
        image->allocator = allocator ? CFRetain(allocator) : NULL;
        image->retainCount = 1;
    }
    return image;
}

CMImageRef CMImageCreateWithMonoImage(CFAllocatorRef allocator, MonoImage *monoImage) {
    CMImageRef image = NULL;
    if (monoImage) {
        if ((image = CMImageCreate(allocator))) {
            image->monoImage = monoImage;
        }
    }
    return image;
}

CMImageRef CMImageCreateWithAssembly(CMAssemblyRef assembly) {
    CMImageRef image = NULL;
    if (assembly) {
        image = CFAllocatorAllocate(assembly->allocator, sizeof(__CMImageType), 0);
        if (image) {
            image->retainCount = 1;
            image->monoImage = mono_assembly_get_image(assembly->monoAssembly);
        }
    }
    return image;
}

CMClassRef CMImageCreateClass(CMImageRef image, CFStringRef namespace_, CFStringRef name) {
    return CMClassCreateWithImage(image, namespace_, name);
}

MonoImage *CMImageGetMonoImage(CMImageRef image) {
    MonoImage *monoImage = NULL;
    if (image) {
        monoImage = image->monoImage;
    }
    return monoImage;
}

MonoMethod *CMImageGetMonoMethodWithMethodDesc(CMImageRef image, CMMethodDescRef methodDesc) {
    MonoMethod *monoMethod = NULL;
    if (image) {
        if (methodDesc) {
            monoMethod = mono_method_desc_search_in_image(CMMethodDescGetMonoMethodDesc(methodDesc), CMImageGetMonoImage(image));
        }
    }
    return monoMethod;
}
