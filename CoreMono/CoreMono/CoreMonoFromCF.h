//
// CoreMono Framework
//
// (c) 2011 Mirek Rusin <mirek@me.com>
//

#ifndef CORE_MONO_FROM_CF_H
#define CORE_MONO_FROM_CF_H

#include "CoreMonoHelpers.h"

MonoObject *CMMonoBooleanObjectWithBoolean   (CFBooleanRef boolean);
MonoObject *CMMonoObjectWithNumber           (CFNumberRef number);
MonoString *CMMonoStringWithString           (CFStringRef string);
MonoObject *CMMonoUriWithURL                 (CFURLRef url);
MonoObject *CMMonoDateTimeWithDate           (CFDateRef date);
MonoObject *CMMonoGuidWithUUID               (CFUUIDRef uuid);
MonoArray  *CMMonoInlineByteArrayWithData    (CFDataRef data);
MonoObject *CMMonoHashtableWithDictionary    (CFDictionaryRef dictionary);
MonoArray  *CMMonoInlineObjectArrayWithArray (CFArrayRef array);
MonoObject *CMMonoObjectWithObject           (CFTypeRef object);

#endif
