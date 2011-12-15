//
//  CMMethod.h
//  CoreMono
//
//  Created by Mirek Rusin on 11/12/2011.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef CORE_MONO_CMMETHOD_H
#define CORE_MONO_CMMETHOD_H

#include <CoreFoundation/CoreFoundation.h>

//const CFStringRef kCMMethodNameConstructor = CFSTR(".ctor");

#include "CoreMono.h"

CMMethodRef CMMethodCreate                  (CFAllocatorRef allocator);
//CMMethodRef CMMethodCreateWithName          (CFAllocatorRef allocator, CFStringRef name, bool useNamespace);

//CMObjectRef CMMethodInvoke                  (CMMethodRef method, CMObjectRef object, const void *args[], CFErrorRef *error);

MonoMethod *CMMethodGetMonoMethod            (CMMethodRef method);


CFArrayRef  CMCreateArrayWithMonoStringArray (CFAllocatorRef allocator, MonoArray *monoArray);


#endif
