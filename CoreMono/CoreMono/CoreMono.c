//
//  CoreMono.c
//  CoreMono
//
//  Created by Mirek Rusin on 12/12/2011.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include "CoreMono.h"

CFTypeRef CMCreateObjectWithMonoObject(CFAllocatorRef allocator, MonoObject *monoObject) {
    CFTypeRef result = NULL;
    if (monoObject) {
        MonoClass *monoClass = mono_object_get_class(monoObject);
        if (mono_get_object_class() == monoClass) {
            
        } else if (false) {
            
        }
//        mono_get_object_class      (void);
//        mono_get_byte_class        (void);
//        mono_get_void_class        (void);
//        mono_get_boolean_class     (void);
//        mono_get_sbyte_class       (void);
//        mono_get_int16_class       (void);
//        mono_get_uint16_class      (void);
//        mono_get_int32_class       (void);
//        mono_get_uint32_class      (void);
//        mono_get_intptr_class         (void);
//        mono_get_uintptr_class        (void);
//        mono_get_int64_class       (void);
//        mono_get_uint64_class      (void);
//        mono_get_single_class      (void);
//        mono_get_double_class      (void);
//        mono_get_char_class        (void);
//        mono_get_string_class      (void);
//        mono_get_enum_class        (void);
//        mono_get_array_class       (void);
//        mono_get_thread_class       (void);
//        mono_get_exception_class    (void);
//        
//        if (monoClass == mono_get_int16_class()) {
//            
//        }
    }
    return result;
}

void CMAddInternalCall(CFStringRef name, const void *function) {
    IF_UTF8(kCFAllocatorDefault, name, nameUtf8, {
        mono_add_internal_call(nameUtf8, function);
    });
}

MonoObject *CMRuntimeInvoke(CMMethodRef method, MonoObject *monoObject, void *params[], CFErrorRef *error) {
    return mono_runtime_invoke(CMMethodGetMonoMethod(method), monoObject, params, NULL);
}

MonoString *CMCreateMonoStringWithString(CFAllocatorRef allocator, CFStringRef string) {
    MonoString *monoString = NULL;
    if (string) {
        IF_UTF8(allocator, string, utf8, {
            monoString = mono_string_new(mono_domain_get(), utf8);
        });
    }
    return monoString;
}

MonoObject *CMCreateMonoObjectWithNumber(CFAllocatorRef allocator, CFNumberRef number) {
    MonoObject *monoNumber = NULL;
    if (number) {
        switch (CFNumberGetType(number)) {
            case kCFNumberSInt8Type: {
                int8_t value = 0;
                CFNumberGetValue(number, kCFNumberSInt8Type, &value);
                monoNumber = CM_BOX_INT8(value);
                break;
            }
            case kCFNumberSInt16Type: {
                int16_t value = 0;
                CFNumberGetValue(number, kCFNumberSInt16Type, &value);
                monoNumber = CM_BOX_INT16(value);
                break;
            }
            case kCFNumberSInt32Type: {
                int32_t value = 0;
                CFNumberGetValue(number, kCFNumberSInt32Type, &value);
                monoNumber = CM_BOX_INT32(value);
                break;
            }
            case kCFNumberSInt64Type: {
                int64_t value = 0;
                CFNumberGetValue(number, kCFNumberSInt64Type, &value);
                monoNumber = CM_BOX_INT64(value);
                break;
            }
            case kCFNumberFloat32Type: {
                Float32 value = 0;
                CFNumberGetValue(number, kCFNumberFloat32Type, &value);
                float floatValue = (float) value;
                monoNumber = CM_BOX_FLOAT(floatValue);
                break;
            }
            case kCFNumberFloat64Type: {
                Float64 value = 0;
                CFNumberGetValue(number, kCFNumberFloat64Type, &value);
                double doubleValue = (double) value;
                monoNumber = CM_BOX_DOUBLE(doubleValue);
                break;
            }
            case kCFNumberCharType: {
                char value = 0;
                CFNumberGetValue(number, kCFNumberCharType, &value);
                int8_t int8Value = (int8_t) value;
                monoNumber = CM_BOX_INT8(int8Value);
                break;
            }
            case kCFNumberShortType: {
                short value = 0;
                CFNumberGetValue(number, kCFNumberShortType, &value);
                int16_t int16Value = (int16_t) value;
                monoNumber = CM_BOX_INT16(int16Value);
                break;
            }
            case kCFNumberIntType: {
                int value = 0;
                CFNumberGetValue(number, kCFNumberIntType, &value);
                int32_t int32Value = (int32_t) value;
                monoNumber = CM_BOX_INT32(int32Value);
                break;
            }
            case kCFNumberLongType: {
                long value = 0;
                CFNumberGetValue(number, kCFNumberLongType, &value);
                int32_t int32Value = (int32_t) value;
                monoNumber = CM_BOX_INT32(int32Value);
                break;
            }
            case kCFNumberLongLongType: {
                long long value = 0;
                CFNumberGetValue(number, kCFNumberLongLongType, &value);
                int64_t int64Value = (int64_t) value;
                monoNumber = CM_BOX_INT64(int64Value);
                break;
            }
            case kCFNumberFloatType: {
                float value = 0;
                CFNumberGetValue(number, kCFNumberFloatType, &value);
                monoNumber = CM_BOX_FLOAT(value);
                break;
            }
            case kCFNumberDoubleType: {
                double value = 0;
                CFNumberGetValue(number, kCFNumberDoubleType, &value);
                monoNumber = CM_BOX_DOUBLE(value);
                break;
            }
            case kCFNumberCFIndexType: {
                CFIndex value = 0;
                CFNumberGetValue(number, kCFNumberCFIndexType, &value);
                int32_t int32Value = (int32_t) value;
                monoNumber = CM_BOX_INT32(int32Value);
                break;
            }
//#if MAC_OS_X_VERSION_10_5 <= MAC_OS_X_VERSION_MAX_ALLOWED
//            case kCFNumberNSIntegerType: {
//                NSInteger value = 0;
//                CFNumberGetValue(number, kCFNumberNSIntegerType, &value);
//                int32_t int32Value = (int32_t) value;
//                monoNumber = CM_BOX_INT32(int32Value);
//                break;
//            }
//            case kCFNumberCGFloatType: {
//                CGFloat value = 0;
//                CFNumberGetValue(number, kCFNumberCGFloatType, &value);
//                float floatValue = (float) value;
//                monoNumber = CM_BOX_FLOAT(floatValue);
//                break;
//            }
//#endif
        }
    }
    return monoNumber;
}

MonoArray *CMCreateMonoArrayWithArray(CFAllocatorRef allocator, CFArrayRef array) {
    MonoArray *monoArray = NULL;
    if (array) {
        CFIndex count = CFArrayGetCount(array);
        monoArray = mono_array_new(mono_domain_get(), mono_get_object_class(), count);
        for (CFIndex i = 0; i < count; i++) {
            mono_array_set(monoArray, MonoObject *, i, CMCreateMonoObjectWithObject(allocator, CFArrayGetValueAtIndex(array, i)));
        }
    }
    return monoArray;
}

MonoObject *CMCreateMonoObjectWithObject(CFAllocatorRef allocator, CFTypeRef object) {
    MonoObject *monoObject = NULL;
         if (CFGetTypeID(object) == CFStringGetTypeID()) monoObject = (MonoObject *) CMCreateMonoStringWithString (allocator, (CFStringRef) object);
    else if (CFGetTypeID(object) == CFNumberGetTypeID()) monoObject = (MonoObject *) CMCreateMonoObjectWithNumber (allocator, (CFNumberRef) object);
    else if (CFGetTypeID(object) == CFArrayGetTypeID())  monoObject = (MonoObject *) CMCreateMonoArrayWithArray   (allocator, (CFArrayRef)  object);
    return monoObject;
}


