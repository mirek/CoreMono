//
//  CMAssembly.h
//  CoreMono
//
//  Copyright (c) 2011 Mirek Rusin. All rights reserved.
//

#ifndef CORE_MONO_CMASSEMBLY_H
#define CORE_MONO_CMASSEMBLY_H

#include "CoreMono.h"

CMAssemblyRef CMAssemblyCreate(CFAllocatorRef allocator);
CMAssemblyRef CMAssemblyCreateWithMonoAssembly(CFAllocatorRef allocator, MonoAssembly *monoAssembly);

//CMAssemblyRef CMAssemblyCreateWithDomainAndPath (CMDomainRef domain, CFStringRef path) CF_RETURNS_RETAINED;
//CMAssemblyRef CMAssemblyCreateWithDomain        (CMDomainRef domain) CF_RETURNS_RETAINED;
CMAssemblyRef CMAssemblyRetain                  (CMAssemblyRef assembly);
CMAssemblyRef CMAssemblyRelease                 (CMAssemblyRef assembly);

//void          CMAssemblyMain                    (CMAssemblyRef assembly, int argc, const char *argv[]);

CMImageRef    CMAssemblyGetImage                (CMAssemblyRef assembly);

MonoImage    *CMAssemblyGetMonoImage            (CMAssemblyRef assembly);

#ifdef __OBJC__
//
//@interface CMAssembly : NSObject {
//@private
//    CMAssemblyRef assembly;
//}
//
//- (CMAssembly *) initWithDomain: (CMDomain *) domain;
//
//+ (CMAssembly *) assemblyWithDomain: (CMDomain *) domain;
//
//- (void) mainWithArgc: (int) argc argv: (const char *[]) argv;
//
//- (CMImage *) image;
//
//@end
//
#endif

#endif
