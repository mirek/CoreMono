//
//  CoreMono.h
//  CoreMono Framework
//
//  Copyright (c) 2011 Mirek Rusin <mirek@me.com>
//

#include <CoreFoundation/CoreFoundation.h>

#include <Mono/mono-2.0/mono/jit/jit.h>
#include <Mono/mono-2.0/mono/metadata/object.h>
#include <Mono/mono-2.0/mono/metadata/environment.h>
#include <Mono/mono-2.0/mono/metadata/assembly.h>
#include <Mono/mono-2.0/mono/metadata/debug-helpers.h>

#include "CoreMonoFromCF.h"
#include "CoreMonoToCF.h"

#include "CMTypes.h"
#include "CMDomain.h"
#include "CMAssembly.h"
#include "CMImage.h"
#include "CMClass.h"
#include "CMMethodDesc.h"
#include "CMMethod.h"
#include "CMJIT.h"

void        CMAddInternalCall (CFStringRef name, const void *function);
MonoObject *CMRuntimeInvoke   (CMMethodRef method, MonoObject *monoObject, void *params[], CFErrorRef *error);

