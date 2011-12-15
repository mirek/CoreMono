//
//  CMAssembly.c
//  CoreMono
//
//  Copyright (c) 2011 Mirek Rusin. All rights reserved.
//

#include "CMAssembly.h"

CMAssemblyRef CMAssemblyCreateWithDomain(CMDomainRef domain) {
    CMAssemblyRef assembly = NULL;
    if (domain) {
        assembly = CMAssemblyCreateWithDomainAndPath(domain, domain->path);
    }
    return assembly;
}

CMAssemblyRef CMAssemblyCreateWithDomainAndPath(CMDomainRef domain, CFStringRef path) {
    CMAssemblyRef assembly = NULL;
    if (domain) {
        if ((assembly = CFAllocatorAllocate(domain->allocator, sizeof(__CMAssemblyType), 0))) {
            memset(assembly, 0, sizeof(__CMAssemblyType));
            assembly->allocator = domain->allocator ? CFRetain(domain->allocator) : NULL;
            assembly->retainCount = 1;
            assembly->domain = CMDomainRetain(domain);
            assembly->path = CFRetain(path);
            IF_UTF8(domain->allocator, path, pathUtf8, {
                assembly->monoAssembly = mono_domain_assembly_open(domain->monoDomain, pathUtf8);
                assembly->image = CMImageCreateWithMonoImage(assembly->allocator, mono_assembly_get_image(assembly->monoAssembly));
            });
        }
    }
    return assembly;
}

CMAssemblyRef CMAssemblyRetain(CMAssemblyRef assembly) {
    if (assembly) {
        assembly->retainCount++;
    }
    return assembly;
}

CMAssemblyRef CMAssemblyRelease(CMAssemblyRef assembly) {
    return assembly;
}

void CMAssemblyMain(CMAssemblyRef assembly, int argc, const char *argv[]) {
    if (assembly) {
        mono_jit_exec(assembly->domain->monoDomain, assembly->monoAssembly, argc, (char **) argv);
    }
}

MonoImage *CMAssemblyGetMonoImage(CMAssemblyRef assembly) {
    MonoImage *monoImage = NULL;
    if (assembly) {
        monoImage = mono_assembly_get_image(assembly->monoAssembly);
    }
    return monoImage;
}

CMImageRef CMAssemblyGetImage(CMAssemblyRef assembly) {
    return assembly ? assembly->image : NULL;
}

