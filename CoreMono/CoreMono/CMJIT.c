//
//  CMJIT.c
//  CoreMono
//
//  Created by Mirek Rusin on 15/12/2011.
//  Copyright (c) 2011 Inteliv Ltd. All rights reserved.
//

#include "CMJIT.h"

//CMJITRef CMJITCreate(CFAllocatorRef allocator) {
//    mono_jit
//}


//CMJITRef CMJITRetain  (CMJITRef jit);
//CMJITRef CMJITRelease (CMJITRef jit);
//
//CMDomainRef CMJITCreateDomainByInitializingWithPath(CMJITRef jit, CFStringRef path);

//void CMJITCleanup(CMDomainRef domain) {
//    if (domain) {
//        mono_jit_cleanup(domain->monoDomain);
//    }
//}

int CMJITExec(CMDomainRef domain, CMAssemblyRef assembly, int argc, char **argv) {
    int result = -1;
    if (domain) {
        if (assembly) {
            result = mono_jit_exec(domain->monoDomain, assembly->monoAssembly, argc, argv);
        }
    }
    return result;
}

CMDomainRef CMJITCreateDomainByInitializingWithPath(CFAllocatorRef allocator, CFStringRef path) {
    CMDomainRef domain = NULL;
    if (path) {
        IF_UTF8(allocator, path, utf8, {
            domain = CMDomainCreateWithMonoDomain(allocator, mono_jit_init(utf8));
        });
    }
    return domain;
}
