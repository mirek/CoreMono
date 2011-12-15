//
//  CMAssembly.c
//  CoreMono
//
//  Copyright (c) 2011 Mirek Rusin. All rights reserved.
//

#include "CMAssembly.h"

//CMAssemblyRef CMAssemblyCreateWithDomain(CMDomainRef domain) {
//    CMAssemblyRef assembly = NULL;
//    if (domain) {
//        assembly = CMAssemblyCreateWithDomainAndPath(domain, domain->path);
//    }
//    return assembly;
//}

CMAssemblyRef CMAssemblyCreate(CFAllocatorRef allocator) {
    CMAssemblyRef assembly = CFAllocatorAllocate(allocator, sizeof(__CMAssemblyType), 0);
    if (assembly) {
        memset(assembly, 0, sizeof(__CMAssemblyType));
        assembly->retainCount = 1;
    }
    return assembly;
}

CMAssemblyRef CMAssemblyCreateWithMonoAssembly(CFAllocatorRef allocator, MonoAssembly *monoAssembly) {
    CMAssemblyRef assembly = NULL;
    if (monoAssembly) {
        if ((assembly = CMAssemblyCreate(allocator))) {
            assembly->monoAssembly = monoAssembly;
            assembly->image = CMImageCreateWithMonoImage(assembly->allocator, mono_assembly_get_image(assembly->monoAssembly));
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

//void CMAssemblyMain(CMAssemblyRef assembly, int argc, const char *argv[]) {
//    if (assembly) {
//        mono_jit_exec(assembly->domain->monoDomain, assembly->monoAssembly, argc, (char **) argv);
//    }
//}

MonoImage *CMAssemblyGetMonoImage(CMAssemblyRef assembly) {
    MonoImage *monoImage = NULL;
    if (assembly) {
        monoImage = mono_assembly_get_image(assembly->monoAssembly);
    }
    return monoImage;
}

CMImageRef CMAssemblyGetImage(CMAssemblyRef assembly) {
    CMImageRef image = NULL;
    if (assembly) {
        image = assembly->image;
    }
    return image;
}
