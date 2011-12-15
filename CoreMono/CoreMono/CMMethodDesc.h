//
//  CMMethodDesc.h
//  CoreMono
//
//  Created by Mirek Rusin on 12/12/2011.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef CORE_MONO_CMMETHODDESC_H
#define CORE_MONO_CMMETHODDESC_H

#include "CoreMono.h"

CMMethodDescRef CMMethodDescCreate  (CFAllocatorRef allocator, CFStringRef name, bool useNamespace);
CMMethodDescRef CMMethodDescRetain  (CMMethodDescRef methodDesc);
CMMethodDescRef CMMethodDescRelease (CMMethodDescRef methodDesc);

MonoMethodDesc *CMMethodDescGetMonoMethodDesc(CMMethodDescRef methodDesc);

#endif
