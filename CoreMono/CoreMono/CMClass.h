//
//  CMClass.h
//  CoreMono
//
//  Created by Mirek Rusin on 14/12/2011.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef CORE_MONO_CMCLASS_H
#define CORE_MONO_CMCLASS_H

#include "CoreMono.h"

CMClassRef CMClassCreate(CFAllocatorRef allocator);
CMClassRef CMClassCreateWithAssembly(CMAssemblyRef assembly, CFStringRef namespace_, CFStringRef name);
CMClassRef CMClassCreateWithImage(CMImageRef image, CFStringRef namespace_, CFStringRef name);

MonoClass *CMClassGetMonoClass(CMClassRef klass);

#endif
