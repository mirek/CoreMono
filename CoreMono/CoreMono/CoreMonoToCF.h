//
// CoreMono Framework
//
// (c) 2011 Mirek Rusin <mirek@me.com>
//

#ifndef CORE_MONO_TO_CF_H
#define CORE_MONO_TO_CF_H

#include "CoreMonoHelpers.h"

CFDictionaryRef CMCreateDictionaryWithPropertiesOfMonoObject  (CFAllocatorRef allocator, MonoObject *monoObject);

CFDictionaryRef CMCreateDictionaryWithMonoSubclassOfIDictionaryObject (CFAllocatorRef allocator, MonoObject *monoObject);

CFArrayRef      CMCreateArrayWithMonoSubclassOfArrayObject    (CFAllocatorRef allocator, MonoObject *monoObject);

CFDataRef       CMCreateDataWithMonoInlineByteArrayObject     (CFAllocatorRef allocator, MonoObject *monoObject);

CFArrayRef      CMCreateArrayWithMonoInlineBooleanArrayObject (CFAllocatorRef allocator, MonoObject *monoObject);

CFArrayRef      CMCreateArrayWithMonoInlineArrayObject        (CFAllocatorRef allocator, MonoArray *monoArray);

CFArrayRef      CMCreateArrayWithMonoInlineNumberArrayObject  (CFAllocatorRef allocator, MonoObject *monoObject, CFNumberType type, int size);

CFArrayRef      CMCreateArrayWithMonoInlineInt32ArrayObject   (CFAllocatorRef allocator, MonoObject *monoObject);
CFArrayRef      CMCreateArrayWithMonoInlineInt64ArrayObject   (CFAllocatorRef allocator, MonoObject *monoObject);
CFArrayRef      CMCreateArrayWithMonoInlineUInt32ArrayObject  (CFAllocatorRef allocator, MonoObject *monoObject);
CFArrayRef      CMCreateArrayWithMonoInlineUInt64ArrayObject  (CFAllocatorRef allocator, MonoObject *monoObject);
CFArrayRef      CMCreateArrayWithMonoInlineSingleArrayObject  (CFAllocatorRef allocator, MonoObject *monoObject);
CFArrayRef      CMCreateArrayWithMonoInlineDoubleArrayObject  (CFAllocatorRef allocator, MonoObject *monoObject);
CFArrayRef      CMCreateArrayWithMonoInlineStringArrayObject  (CFAllocatorRef allocator, MonoObject *monoObject);

CFArrayRef      CMCreateArrayWithMonoArrayListObject          (CFAllocatorRef allocator, MonoObject *monoObject);

CFBooleanRef    CMCreateBooleanWithMonoBooleanObject (CFAllocatorRef allocator, MonoObject *monoObject);

CFNumberRef     CMCreateNumberWithMonoInt32Object    (CFAllocatorRef allocator, MonoObject *monoObject);
CFNumberRef     CMCreateNumberWithMonoInt64Object    (CFAllocatorRef allocator, MonoObject *monoObject);
CFNumberRef     CMCreateNumberWithMonoUInt32Object   (CFAllocatorRef allocator, MonoObject *monoObject);
CFNumberRef     CMCreateNumberWithMonoUInt64Object   (CFAllocatorRef allocator, MonoObject *monoObject);
CFNumberRef     CMCreateNumberWithMonoSingleObject   (CFAllocatorRef allocator, MonoObject *monoObject);
CFNumberRef     CMCreateNumberWithMonoDoubleObject   (CFAllocatorRef allocator, MonoObject *monoObject);

CFDateRef       CMCreateDateWithMonoDateTimeObject   (CFAllocatorRef allocator, MonoObject *monoObject);

CFStringRef     CMCreateStringWithMonoStringObject   (CFAllocatorRef allocator, MonoObject *monoObject);

CFTypeRef       CMCreateObjectWithMonoObject         (CFAllocatorRef allocator, MonoObject *monoObject);


#endif
