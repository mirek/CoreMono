//
//  CMTypes.h
//  CoreMono
//
//  Created by Mirek Rusin on 12/12/2011.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef CoreMono_CMTypes_h
#define CoreMono_CMTypes_h

#define __CORE_MONO_JOIN(x, y)       __CORE_MONO_JOIN_AGAIN(x, y)
#define __CORE_MONO_JOIN_AGAIN(x, y) x ## y

#define IF_UTF8(allocator, string, utf8, whatever)                                                                                  \
    size_t __CORE_MONO_JOIN(__length_of_, utf8) = -1;                                                                               \
    char *utf8 = NULL; \
    if (CFStringGetLength(string) == 0) { \
        __CORE_MONO_JOIN(__length_of_, utf8) = 0; \
        utf8 = ""; \
    } else { \
        if (NULL == (utf8 = (char *) CFStringGetCStringPtr(string, kCFStringEncodingUTF8))) {                                           \
            __CORE_MONO_JOIN(__length_of_, utf8) = CFStringGetMaximumSizeForEncoding(CFStringGetLength(string), kCFStringEncodingUTF8); \
            if ((utf8 = CFAllocatorAllocate(allocator, __CORE_MONO_JOIN(__length_of_, utf8) , 0))) {                                    \
                if (0 == CFStringGetCString(string, utf8, __CORE_MONO_JOIN(__length_of_, utf8), kCFStringEncodingUTF8)) {               \
                    /* todo */                                                                                                          \
                }                                                                                                                       \
            }                                                                                                                           \
        }                                                                                                                               \
    } \
    do { whatever } while (0);                                                                                                      \
    if (__CORE_MONO_JOIN(__length_of_, utf8) > 0) {                                                                               \
        if (utf8) {                                                                                                                 \
            CFAllocatorDeallocate(allocator, utf8);                                                                                 \
        }                                                                                                                           \
    }                                                                                                                               \

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
