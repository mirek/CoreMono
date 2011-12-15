//
//  CoreMono.h
//  CoreMono
//
//  Created by Mirek Rusin on 11/12/2011.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include <CoreFoundation/CoreFoundation.h>

#include <Mono/mono-2.0/mono/jit/jit.h>
#include <Mono/mono-2.0/mono/metadata/object.h>
#include <Mono/mono-2.0/mono/metadata/environment.h>
#include <Mono/mono-2.0/mono/metadata/assembly.h>
#include <Mono/mono-2.0/mono/metadata/debug-helpers.h>

#include "CMTypes.h"
#include "CMDomain.h"
#include "CMAssembly.h"
#include "CMImage.h"
#include "CMClass.h"
#include "CMMethodDesc.h"
#include "CMMethod.h"
#include "CMJIT.h"

#define CM_UNBOX_DOUBLE(x)  (* (double *)      mono_object_unbox(x))
#define CM_UNBOX_FLOAT(x)   (* (float *)       mono_object_unbox(x))
#define CM_UNBOX_INT64(x)   (* (int64_t *)     mono_object_unbox(x))
#define CM_UNBOX_INT32(x)   (* (int32_t *)     mono_object_unbox(x))
#define CM_UNBOX_INT16(x)   (* (int16_t *)     mono_object_unbox(x))
#define CM_UNBOX_INT8(x)    (* (int8_t *)      mono_object_unbox(x))
#define CM_UNBOX_UINT64(x)  (* (uint64_t *)    mono_object_unbox(x))
#define CM_UNBOX_UINT32(x)  (* (uint32_t *)    mono_object_unbox(x))
#define CM_UNBOX_UINT16(x)  (* (uint16_t *)    mono_object_unbox(x))
#define CM_UNBOX_UINT8(x)   (* (uint8_t *)     mono_object_unbox(x))
#define CM_UNBOX_BOOLEAN(x) (* (MonoBoolean *) mono_object_unbox(x))

#define CM_BOX_DOUBLE(x)  mono_value_box(mono_domain_get(), mono_get_double_class(),  &x)
#define CM_BOX_FLOAT(x)   mono_value_box(mono_domain_get(), mono_get_single_class(),  &x)
#define CM_BOX_INT64(x)   mono_value_box(mono_domain_get(), mono_get_int64_class(),   &x)
#define CM_BOX_INT32(x)   mono_value_box(mono_domain_get(), mono_get_int32_class(),   &x)
#define CM_BOX_INT16(x)   mono_value_box(mono_domain_get(), mono_get_int16_class(),   &x)
#define CM_BOX_INT8(x)    mono_value_box(mono_domain_get(), mono_get_sbyte_class(),   &x)
#define CM_BOX_UINT64(x)  mono_value_box(mono_domain_get(), mono_get_uint64_class(),  &x)
#define CM_BOX_UINT32(x)  mono_value_box(mono_domain_get(), mono_get_uint32_class(),  &x)
#define CM_BOX_UINT16(x)  mono_value_box(mono_domain_get(), mono_get_uint16_class(),  &x)
#define CM_BOX_UINT8(x)   mono_value_box(mono_domain_get(), mono_get_byte_class(),    &x)
#define CM_BOX_BOOLEAN(x) mono_value_box(mono_domain_get(), mono_get_boolean_class(), &x)

CFTypeRef              CMNullify(CFTypeRef value);

CFDictionaryRef        CMCreateDictionaryWithMonoSubclassOfIDictionaryObject(CFAllocatorRef allocator, MonoObject *monoObject);
CFMutableDictionaryRef CMCreateDictionaryWithPropertiesOfMonoObject(CFAllocatorRef allocator, MonoObject *monoObject);
CFArrayRef             CMCreateArrayWithMonoSubclassOfArrayObject(CFAllocatorRef allocator, MonoObject *monoObject);

CFDataRef CMCreateDataWithMonoInlineByteArrayObject(CFAllocatorRef allocator, MonoObject *monoObject);

CFArrayRef             CMCreateArrayWithMonoInlineBooleanArrayObject (CFAllocatorRef allocator, MonoObject *monoObject);

CFArrayRef CMCreateArrayWithMonoInlineNumberArrayObject        (CFAllocatorRef allocator, MonoObject *monoObject, CFNumberType type, int size);

CFArrayRef CMCreateArrayWithMonoInlineArrayObject(CFAllocatorRef allocator, MonoArray *monoArray);
CFArrayRef CMCreateArrayWithMonoArrayListObject(CFAllocatorRef allocator, MonoObject *monoObject);


CFArrayRef CMCreateArrayWithMonoInlineInt32ArrayObject   (CFAllocatorRef allocator, MonoObject *monoObject);
CFArrayRef CMCreateArrayWithMonoInlineInt64ArrayObject   (CFAllocatorRef allocator, MonoObject *monoObject);
CFArrayRef CMCreateArrayWithMonoInlineUInt32ArrayObject  (CFAllocatorRef allocator, MonoObject *monoObject);
CFArrayRef CMCreateArrayWithMonoInlineUInt64ArrayObject  (CFAllocatorRef allocator, MonoObject *monoObject);
CFArrayRef CMCreateArrayWithMonoInlineSingleArrayObject  (CFAllocatorRef allocator, MonoObject *monoObject);
CFArrayRef CMCreateArrayWithMonoInlineDoubleArrayObject  (CFAllocatorRef allocator, MonoObject *monoObject);

CFArrayRef CMCreateArrayWithMonoInlineStringArrayObject  (CFAllocatorRef allocator, MonoObject *monoObject);

CFBooleanRef CMCreateBooleanWithMonoBooleanObject (CFAllocatorRef allocator, MonoObject *monoObject);
CFNumberRef  CMCreateNumberWithMonoInt32Object    (CFAllocatorRef allocator, MonoObject *monoObject);
CFNumberRef  CMCreateNumberWithMonoInt64Object    (CFAllocatorRef allocator, MonoObject *monoObject);
CFNumberRef  CMCreateNumberWithMonoUInt32Object   (CFAllocatorRef allocator, MonoObject *monoObject);
CFNumberRef  CMCreateNumberWithMonoUInt64Object   (CFAllocatorRef allocator, MonoObject *monoObject);
CFNumberRef  CMCreateNumberWithMonoSingleObject   (CFAllocatorRef allocator, MonoObject *monoObject);
CFNumberRef  CMCreateNumberWithMonoDoubleObject   (CFAllocatorRef allocator, MonoObject *monoObject);
CFDateRef    CMCreateDateWithMonoDateTimeObject   (CFAllocatorRef allocator, MonoObject *monoObject);
CFStringRef  CMCreateStringWithMonoStringObject   (CFAllocatorRef allocator, MonoObject *monoObject);

void CMAddInternalCall(CFStringRef name, const void *function);

MonoObject *CMRuntimeInvoke(CMMethodRef method, MonoObject *monoObject, void *params[], CFErrorRef *error);

MonoString *CMCreateMonoStringWithString (CFAllocatorRef allocator, CFStringRef string);
MonoObject *CMCreateMonoObjectWithNumber (CFAllocatorRef allocator, CFNumberRef number);
MonoArray  *CMCreateMonoArrayWithArray   (CFAllocatorRef allocator, CFArrayRef array);
MonoObject *CMCreateMonoObjectWithObject (CFAllocatorRef allocator, CFTypeRef object);

CFTypeRef CMCreateObjectWithMonoObject(CFAllocatorRef allocator, MonoObject *monoObject);
