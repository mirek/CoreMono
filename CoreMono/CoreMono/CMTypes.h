//
//  CMTypes.h
//  CoreMono
//
//  Created by Mirek Rusin on 12/12/2011.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef CoreMono_CMTypes_h
#define CoreMono_CMTypes_h

typedef struct __CMImageStruct {
    CFAllocatorRef allocator;
    CFIndex retainCount;
    MonoImage *monoImage;
} __CMImageType;

typedef __CMImageType* CMImageRef;


typedef struct __CMDomainStruct {
    CFAllocatorRef allocator;
    CFIndex retainCount;
    MonoDomain *monoDomain;
    CFStringRef path;
} __CMDomainType;

typedef __CMDomainType* CMDomainRef;


typedef struct __CMAssemblyStruct {
    CFAllocatorRef allocator;
    CFIndex retainCount;
    MonoAssembly *monoAssembly;
    CMDomainRef domain;
    CMImageRef image;
    CFStringRef path;
} __CMAssemblyType;

typedef __CMAssemblyType* CMAssemblyRef;


typedef struct __CMClassStruct {
    CFAllocatorRef allocator;
    CFIndex retainCount;
    CMDomainRef domain;
    MonoClass *monoClass;
} __CMClassType;

typedef __CMClassType* CMClassRef;


typedef struct __CMMethodDescStruct {
    CFAllocatorRef allocator;
    CFIndex retainCount;
    CFStringRef name;
    bool useNamespace;
    MonoMethodDesc *monoMethodDesc;
} __CMMethodDescType;

typedef __CMMethodDescType* CMMethodDescRef;


typedef struct __CMMethodStruct {
    CFAllocatorRef allocator;
    CFIndex retainCount;
    CFStringRef name;
    bool useNamespace;
    MonoMethod *monoMethod;
} __CMMethodType;

typedef __CMMethodType* CMMethodRef;


typedef struct __CMJITStruct {
    CFAllocatorRef allocator;
    CFIndex retainCount;
} __CMJITType;

typedef __CMJITType* CMJITRef;

#endif
