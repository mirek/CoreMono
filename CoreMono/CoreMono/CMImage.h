//
//  CMImage.h
//  CoreMono
//
//  Created by Mirek Rusin on 11/12/2011.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef CORE_MONO_CMIMAGE_H
#define CORE_MONO_CMIMAGE_H

#include "CoreMono.h"

CMImageRef CMImageCreate(CFAllocatorRef allocator);
CMImageRef CMImageCreateWithMonoImage(CFAllocatorRef allocator, MonoImage *monoImage);

CMImageRef CMImageCreateWithAssembly(CMAssemblyRef assembly) CF_RETURNS_RETAINED;
CMClassRef CMImageCreateClass(CMImageRef image, CFStringRef namespace_, CFStringRef name);

MonoImage *CMImageGetMonoImage(CMImageRef image);
MonoMethod *CMImageGetMonoMethodWithMethodDesc(CMImageRef image, CMMethodDescRef methodDesc);

#endif
