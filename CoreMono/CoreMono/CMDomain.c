//
//  CMDomain.c
//  CoreMono
//
//  Created by Mirek Rusin on 11/12/2011.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include "CMDomain.h"

CMDomainRef __CMDomainCreate(CFAllocatorRef allocator) {
    CMDomainRef domain = CFAllocatorAllocate(allocator, sizeof(__CMDomainType), 0);
    if (domain) {
        memset(domain, 0, sizeof(__CMDomainType));
        domain->allocator = allocator ? CFRetain(allocator) : NULL;
        domain->retainCount = 1;
    }
    return domain;
}

CMDomainRef CMDomainCreateWithPath(CFAllocatorRef allocator, CFStringRef path) {
    CMDomainRef domain = __CMDomainCreate(allocator);
    if (domain) {
        domain->path = CFRetain(path);
        IF_UTF8(domain->allocator, path, pathUtf8, {
            if (NULL == (domain->monoDomain = mono_jit_init(pathUtf8))) {
                // TODO: Error
            }
        });
    }
    return domain;
}

CMDomainRef CMDomainCreateWithNameAndVersion(CFAllocatorRef allocator, CFStringRef name, CFStringRef version) {
    return NULL;
}

CMDomainRef CMDomainRetain(CMDomainRef domain) {
    if (domain) {
        domain->retainCount++;
    }
    return domain;
}

CMDomainRef CMDomainRelease(CMDomainRef domain) {
    return domain;
}


//CMAssemblyRef CMDomainCreateAssembly(CMDomainRef domain, CFStringRef path) {
//    CMAssemblyRef assembly = NULL;
//    if (domain) {
//        IF_UTF8(domain->allocator, path, pathUtf8, {
//            MonoAssembly *monoAssembly = mono_domain_assembly_open(domain->monoDomain, pathUtf8);
//            if (monoAssembly) {
//                assembly = __CMAssemblyCreate(domain->allocator, monoAssembly);
//            }
//        });
//    }
//    return assembly;
//}
//
