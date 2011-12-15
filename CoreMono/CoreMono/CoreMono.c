//
//  CoreMono.c
//  CoreMono
//
//  Created by Mirek Rusin on 12/12/2011.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include "CoreMono.h"

CFBooleanRef CMCreateBooleanWithMonoBooleanObject(CFAllocatorRef allocator, MonoObject *monoObject) {
    CFBooleanRef boolean = NULL;
    if (monoObject) {
        MonoBoolean value = * (MonoBoolean *) mono_object_unbox(monoObject);
        boolean = value ? kCFBooleanTrue : kCFBooleanFalse;
    }
    return boolean;
}

CFNumberRef CMCreateNumberWithMonoIntObject(CFAllocatorRef allocator, MonoObject *monoObject) {
    CFNumberRef number = NULL;
    if (monoObject) {
        int value = * (int *) mono_object_unbox(monoObject);
        number = CFNumberCreate(allocator, kCFNumberIntType, &value);
    }
    return number;
}

CFNumberRef CMCreateNumberWithMonoInt32Object(CFAllocatorRef allocator, MonoObject *monoObject) {
    CFNumberRef number = NULL;
    if (monoObject) {
        int32_t value = * (int32_t *) mono_object_unbox(monoObject);
        number = CFNumberCreate(allocator, kCFNumberSInt32Type, &value);
    }
    return number;
}

CFNumberRef CMCreateNumberWithMonoInt64Object(CFAllocatorRef allocator, MonoObject *monoObject) {
    CFNumberRef number = NULL;
    if (monoObject) {
        int64_t value = * (int64_t *) mono_object_unbox(monoObject);
        number = CFNumberCreate(allocator, kCFNumberSInt64Type, &value);
    }
    return number;
}

CFNumberRef CMCreateNumberWithMonoDoubleObject(CFAllocatorRef allocator, MonoObject *monoObject) {
    CFNumberRef number = NULL;
    if (monoObject) {
        double value = * (double *) mono_object_unbox(monoObject);
        number = CFNumberCreate(allocator, kCFNumberDoubleType, &value);
    }
    return number;
}

CFNumberRef CMCreateNumberWithMonoSingleObject(CFAllocatorRef allocator, MonoObject *monoObject) {
    CFNumberRef number = NULL;
    if (monoObject) {
        float value = * (float *) mono_object_unbox(monoObject);
        number = CFNumberCreate(allocator, kCFNumberFloatType, &value);
    }
    return number;
}

CFDateRef CMCreateDateWithMonoDateTimeObject(CFAllocatorRef allocator, MonoObject *monoObject) {
    CFDateRef date = NULL;
    if (monoObject) {
        // Ticks represent the number of 100-nanosecond intervals that have elapsed since 12:00:00 midnight, January 1, 0001.
        uint64_t ticks = * (uint64_t *) mono_object_unbox(monoObject);
        
        // Absolute time is the first instant of the 1 January 2001, GMT
        date = CFDateCreate(allocator, ((ticks) - 621355968000000000LL) / 10000000);
    }
    return date;
}

CFStringRef CMCreateStringWithMonoStringObject(CFAllocatorRef allocator, MonoObject *monoObject) {
    CFStringRef string = NULL;
    if (monoObject) {
        char *utf8 = mono_string_to_utf8((MonoString *) monoObject);
        if (utf8) {
            string = CFStringCreateWithCString(allocator, (const char *) utf8, kCFStringEncodingUTF8);
            mono_free(utf8);
        }
    }
    return string;
}

CFDictionaryRef CMCreateDictionaryWithMonoSubclassOfIDictionaryObject(CFAllocatorRef allocator, MonoObject *monoObject) {
    CFDictionaryRef dictionary = NULL;
    if (monoObject) {
        MonoClass *monoClass = mono_object_get_class(monoObject);
        int count = * (int *) mono_object_unbox(mono_property_get_value(mono_class_get_property_from_name(monoClass, "Count"), monoObject, NULL, NULL));
        if (count > 0) {
            MonoArray *monoKeysArray = mono_array_new(mono_domain_get(), mono_get_object_class(), count);
            MonoArray *monoValuesArray = mono_array_new(mono_domain_get(), mono_get_object_class(), count);
            
            MonoObject *monoKeysObject = mono_property_get_value(mono_class_get_property_from_name(monoClass, "Keys"), monoObject, NULL, NULL);
            MonoObject *monoValuesObject = mono_property_get_value(mono_class_get_property_from_name(monoClass, "Values"), monoObject, NULL, NULL);
            
            int zero = 0;
            
            void *keysParams[2] = { monoKeysArray, &zero };
            void *valuesParams[2] = { monoValuesArray, &zero };
            
            (void) mono_runtime_invoke(mono_class_get_method_from_name(mono_object_get_class(monoKeysObject), "CopyTo", 2), monoKeysObject, keysParams, NULL);
            (void) mono_runtime_invoke(mono_class_get_method_from_name(mono_object_get_class(monoValuesObject), "CopyTo", 2), monoValuesObject, valuesParams, NULL);
            
            CFTypeRef *keys = CFAllocatorAllocate(allocator, sizeof(CFTypeRef) * count, 0);
            if (keys) {
                memset(keys, 0, sizeof(CFTypeRef) * count);
                CFTypeRef *values = CFAllocatorAllocate(allocator, sizeof(CFTypeRef) * count, 0);
                if (values) {
                    memset(values, 0, sizeof(CFTypeRef) * count);
                    
                    for (int i = 0; i < count; i++) {
                        // TODO: Set to kCFNull if == NULL
                        if (NULL == (keys[i] = CMCreateObjectWithMonoObject(allocator, mono_array_get(monoKeysArray, MonoObject *, i)))) keys[i] = kCFNull;
                        if (NULL == (values[i] = CMCreateObjectWithMonoObject(allocator, mono_array_get(monoValuesArray, MonoObject *, i)))) values[i] = kCFNull;
                    }
                    
                    dictionary = CFDictionaryCreate(allocator, keys, values, count, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
                    
                    for (int i = 0; i < count; i++) {
                        CFRelease(keys[i]);
                        CFRelease(values[i]);
                    }
                    
                    CFAllocatorDeallocate(allocator, values);
                }
                CFAllocatorDeallocate(allocator, keys);
            }
        } else {
            dictionary = CFDictionaryCreate(allocator, NULL, NULL, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
        }
    }
    return dictionary;
}

CFMutableDictionaryRef CMCreateDictionaryWithPropertiesOfMonoObject(CFAllocatorRef allocator, MonoObject *monoObject) {
    CFMutableDictionaryRef dictionary = NULL;
    if (monoObject) {
        dictionary = CFDictionaryCreateMutable(allocator, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
        MonoProperty *monoProperty = NULL;
        void *i = NULL;
        MonoClass *monoClass = mono_object_get_class(monoObject);
        while ((monoProperty = mono_class_get_properties(monoClass, &i))) {
            const char *keyUtf8 = mono_property_get_name(monoProperty);
            if (keyUtf8) {
                CFStringRef key = CFStringCreateWithCString(allocator, keyUtf8, kCFStringEncodingUTF8);
                if (key) {
                    CFTypeRef value = CMCreateObjectWithMonoObject(allocator, mono_property_get_value(monoProperty, monoObject, NULL, NULL));
                    if (value) {
                        CFDictionarySetValue(dictionary, key, value);
                        CFRelease(value);
                    }
                    CFRelease(key);
                }
            }
        }
    }
    return dictionary;
}

CFTypeRef CMNullify(CFTypeRef value) {
    return value ? value : kCFNull;
}

CFArrayRef CMCreateArrayWithMonoSubclassOfArrayObject(CFAllocatorRef allocator, MonoObject *monoObject) {
    CFArrayRef array = NULL;
    if (monoObject) {
        uintptr_t n = mono_array_length((MonoArray *) monoObject);
        if (n > 0) {
            CFTypeRef *values = CFAllocatorAllocate(allocator, sizeof(CFTypeRef) * n, 0);
            if (values) {
                for (uintptr_t i = 0; i < n; i++) {
                    values[i] = CMNullify(CMCreateObjectWithMonoObject(allocator, mono_array_get((MonoArray *) monoObject, MonoObject *, i)));
                }
                array = CFArrayCreate(allocator, values, n, &kCFTypeArrayCallBacks);
                for (uintptr_t i = 0; i < n; i++) {
                    CFRelease(values[i]);
                }
                CFAllocatorDeallocate(allocator, values);
            }
        } else {
            array = CFArrayCreate(allocator, NULL, 0, &kCFTypeArrayCallBacks);
        }
    }
    return array;
}

CFArrayRef CMCreateArrayWithMonoInlineBooleanArrayObject(CFAllocatorRef allocator, MonoObject *monoObject) {
    CFArrayRef array = NULL;
    if (monoObject) {
        uintptr_t n = mono_array_length((MonoArray *) monoObject);
        if (n > 0) {
            CFTypeRef *values = CFAllocatorAllocate(allocator, sizeof(CFTypeRef) * n, 0);
            if (values) {
                for (uintptr_t i = 0; i < n; i++) {
                    values[i] = mono_array_get((MonoArray *) monoObject, MonoBoolean, i) ? kCFBooleanTrue : kCFBooleanFalse;
                }
                array = CFArrayCreate(allocator, values, n, &kCFTypeArrayCallBacks);
                CFAllocatorDeallocate(allocator, values);
            }
        } else {
            array = CFArrayCreate(allocator, NULL, 0, &kCFTypeArrayCallBacks);
        }
    }
    return array;
}

CFArrayRef CMCreateArrayWithMonoInlineArrayObject(CFAllocatorRef allocator, MonoObject *monoObject, CFNumberType type, int size) {
    CFArrayRef array = NULL;
    if (monoObject) {
        uintptr_t n = mono_array_length((MonoArray *) monoObject);
        if (n > 0) {
            CFTypeRef *values = CFAllocatorAllocate(allocator, sizeof(CFTypeRef) * n, 0);
            if (values) {
                for (uintptr_t i = 0; i < n; i++) {
                    values[i] = CFNumberCreate(allocator, type, mono_array_addr_with_size((MonoArray *) monoObject, size, i));
                }
                array = CFArrayCreate(allocator, values, n, &kCFTypeArrayCallBacks);
                for (uintptr_t i = 0; i < n; i++) {
                    CFRelease(values[i]);
                }
                CFAllocatorDeallocate(allocator, values);
            }
        } else {
            array = CFArrayCreate(allocator, NULL, 0, &kCFTypeArrayCallBacks);
        }
    }
    return array;
}

CFArrayRef CMCreateArrayWithMonoInlineIntArrayObject(CFAllocatorRef allocator, MonoObject *monoObject) {
    return CMCreateArrayWithMonoInlineArrayObject(allocator, monoObject, kCFNumberIntType, sizeof(int));
}

CFArrayRef CMCreateArrayWithMonoInlineInt32ArrayObject(CFAllocatorRef allocator, MonoObject *monoObject) {
    return CMCreateArrayWithMonoInlineArrayObject(allocator, monoObject, kCFNumberSInt32Type, sizeof(int32_t));
}

CFArrayRef CMCreateArrayWithMonoInlineInt64ArrayObject(CFAllocatorRef allocator, MonoObject *monoObject) {
    return CMCreateArrayWithMonoInlineArrayObject(allocator, monoObject, kCFNumberSInt64Type, sizeof(int64_t));
}

CFArrayRef CMCreateArrayWithMonoInlineDoubleArrayObject(CFAllocatorRef allocator, MonoObject *monoObject) {
    return CMCreateArrayWithMonoInlineArrayObject(allocator, monoObject, kCFNumberDoubleType, sizeof(double));
}

CFArrayRef CMCreateArrayWithMonoInlineSingleArrayObject(CFAllocatorRef allocator, MonoObject *monoObject) {
    return CMCreateArrayWithMonoInlineArrayObject(allocator, monoObject, kCFNumberFloatType, sizeof(float));
}

CFArrayRef CMCreateArrayWithMonoInlineStringArrayObject(CFAllocatorRef allocator, MonoObject *monoObject) {
    CFArrayRef array = NULL;
    if (monoObject) {
        uintptr_t n = mono_array_length((MonoArray *) monoObject);
        if (n > 0) {
            CFTypeRef *values = CFAllocatorAllocate(allocator, sizeof(CFTypeRef) * n, 0);
            if (values) {
                for (uintptr_t i = 0; i < n; i++) {
                    values[i] = CMCreateStringWithMonoStringObject(allocator, mono_array_get((MonoArray *) monoObject, MonoObject *, i));
                }
                array = CFArrayCreate(allocator, values, n, &kCFTypeArrayCallBacks);
                for (uintptr_t i = 0; i < n; i++) {
                    CFRelease(values[i]);
                }
                CFAllocatorDeallocate(allocator, values);
            }
        } else {
            array = CFArrayCreate(allocator, NULL, 0, &kCFTypeArrayCallBacks);
        }
    }
    return array;
}

CFTypeRef CMCreateObjectWithMonoObject(CFAllocatorRef allocator, MonoObject *monoObject) {
    static MonoClass *kCMMonoClassSystemCollectionsIDictionary = NULL;
    static MonoClass *kCMMonoClassSystemArray = NULL;
    
    if (kCMMonoClassSystemCollectionsIDictionary == NULL)
        kCMMonoClassSystemCollectionsIDictionary = mono_class_from_name(mono_get_corlib(), "System.Collections", "IDictionary");
    
    if (kCMMonoClassSystemArray == NULL)
        kCMMonoClassSystemArray = mono_class_from_name(mono_get_corlib(), "System", "Array");
    
    CFTypeRef result = NULL;
    if (monoObject) {
//        mono_class_is_valuetype(<#MonoClass *klass#>)
        MonoClass *monoClass = mono_object_get_class(monoObject);
        const char *monoClassName = mono_class_get_name(monoClass);
        
        if (mono_class_is_subclass_of(monoClass, kCMMonoClassSystemCollectionsIDictionary, 1)) result = CMCreateDictionaryWithMonoSubclassOfIDictionaryObject(allocator, monoObject);
        
        else if (strcmp("Boolean[]", monoClassName) == 0) result = CMCreateArrayWithMonoInlineBooleanArrayObject(allocator, monoObject);
        else if (strcmp("int[]",     monoClassName) == 0) result = CMCreateArrayWithMonoInlineIntArrayObject(allocator, monoObject);
        else if (strcmp("Int32[]",   monoClassName) == 0) result = CMCreateArrayWithMonoInlineInt32ArrayObject(allocator, monoObject);
        else if (strcmp("Int64[]",   monoClassName) == 0) result = CMCreateArrayWithMonoInlineInt64ArrayObject(allocator, monoObject);
        
        else if (strcmp("Single[]",  monoClassName) == 0) result = CMCreateArrayWithMonoInlineSingleArrayObject(allocator, monoObject);
        else if (strcmp("Double[]",  monoClassName) == 0) result = CMCreateArrayWithMonoInlineDoubleArrayObject(allocator, monoObject);
        
        else if (strcmp("String[]",  monoClassName) == 0) result = CMCreateArrayWithMonoInlineStringArrayObject(allocator, monoObject);
        
        else if (strcmp("Boolean",   monoClassName) == 0) result = CMCreateBooleanWithMonoBooleanObject(allocator, monoObject);
        else if (strcmp("int",       monoClassName) == 0) result = CMCreateNumberWithMonoIntObject(allocator, monoObject);
        else if (strcmp("Int32",     monoClassName) == 0) result = CMCreateNumberWithMonoInt32Object(allocator, monoObject);
        else if (strcmp("Int64",     monoClassName) == 0) result = CMCreateNumberWithMonoInt64Object(allocator, monoObject);

        else if (strcmp("Single",    monoClassName) == 0) result = CMCreateNumberWithMonoSingleObject(allocator, monoObject);
        else if (strcmp("Double",    monoClassName) == 0) result = CMCreateNumberWithMonoDoubleObject(allocator, monoObject);

        else if (strcmp("String",    monoClassName) == 0) result = CMCreateStringWithMonoStringObject(allocator, monoObject);
        
        else if (strcmp("DateTime",  monoClassName) == 0) result = CMCreateDateWithMonoDateTimeObject(allocator, monoObject);
        
        else if (mono_class_is_subclass_of(monoClass, kCMMonoClassSystemArray, 0))
            result = CMCreateArrayWithMonoSubclassOfArrayObject(allocator, monoObject);
        else if (strcmp("", monoClassName) == 0) {
            
        } else if (strcmp("", monoClassName) == 0) {
            
        } else if (strcmp("", monoClassName) == 0) {
            
        } else if (strcmp("", monoClassName) == 0) {
            
        } else if (strcmp("", monoClassName) == 0) {
            
        } else if (strcmp("", monoClassName) == 0) {
            
        } else if (strcmp("", monoClassName) == 0) {
            
        } else if (strcmp("", monoClassName) == 0) {
            
        } else if (strcmp("", monoClassName) == 0) {
            
        } else if (strcmp("", monoClassName) == 0) {
            
        } else if (strcmp("", monoClassName) == 0) {
            
        } else if (strcmp("", monoClassName) == 0) {
            
        } else {
            result = CMCreateDictionaryWithPropertiesOfMonoObject(allocator, monoObject);
        }
        
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


