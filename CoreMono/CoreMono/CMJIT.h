//
//  CMJIT.h
//  CoreMono
//
//  Created by Mirek Rusin on 15/12/2011.
//  Copyright (c) 2011 Inteliv Ltd. All rights reserved.
//

#ifndef CORE_MONO_CMJIT_H
#define CORE_MONO_CMJIT_H

#include "CoreMono.h"

CMJITRef CMJITCreate  (CFAllocatorRef allocator);
CMJITRef CMJITRetain  (CMJITRef jit);
CMJITRef CMJITRelease (CMJITRef jit);

//CMDomainRef CMJITCreateDomainByInitializingWithPath(CMJITRef jit, CFStringRef path);

CMDomainRef CMJITCreateDomainByInitializingWithPath(CFAllocatorRef allocator, CFStringRef path);
int CMJITExec(CMDomainRef domain, CMAssemblyRef assembly, int argc, char **argv);

#endif
