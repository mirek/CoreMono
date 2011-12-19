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

// TODO: Doesn't support unsigned actually
CFNumberRef CMCreateNumberWithMonoUInt32Object(CFAllocatorRef allocator, MonoObject *monoObject) {
    CFNumberRef number = NULL;
    if (monoObject) {
        int32_t value = (int32_t) (* (uint32_t *) mono_object_unbox(monoObject));
        number = CFNumberCreate(allocator, kCFNumberSInt32Type, &value);
    }
    return number;
}

// TODO: Doesn't support unsigned actually
CFNumberRef CMCreateNumberWithMonoUInt64Object(CFAllocatorRef allocator, MonoObject *monoObject) {
    CFNumberRef number = NULL;
    if (monoObject) {
        int64_t value = (int64_t) (* (uint64_t *) mono_object_unbox(monoObject));
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
        // TODO: This is wrong, offset abs time - unix epoch
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
                        keys[i] = CMNullify(CMCreateObjectWithMonoObject(allocator, mono_array_get(monoKeysArray, MonoObject *, i)));
                        values[i] = CMNullify(CMCreateObjectWithMonoObject(allocator, mono_array_get(monoValuesArray, MonoObject *, i)));
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

CFDataRef CMCreateDataWithMonoInlineByteArrayObject(CFAllocatorRef allocator, MonoObject *monoObject) {
    CFDataRef data = NULL;
    if (monoObject) {
        uintptr_t n = mono_array_length((MonoArray *) monoObject);
        char *bytes = CFAllocatorAllocate(allocator, n, 0);
        if (bytes) {
//            mono_array_memcpy_refs(bytes, 0, (MonoArray *) monoObject, 0, n);
            for (uintptr_t i = 0; i < n; i++) {
                bytes[i] = mono_array_get((MonoArray *) monoObject, char, i);
            }
            data = CFDataCreateWithBytesNoCopy(allocator, (const UInt8 *) bytes, n, allocator);
        }
    }
    return data;
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

CFArrayRef CMCreateArrayWithMonoInlineNumberArrayObject(CFAllocatorRef allocator, MonoObject *monoObject, CFNumberType type, int size) {
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

CFArrayRef CMCreateArrayWithMonoInlineInt32ArrayObject(CFAllocatorRef allocator, MonoObject *monoObject) {
    return CMCreateArrayWithMonoInlineNumberArrayObject(allocator, monoObject, kCFNumberSInt32Type, sizeof(int32_t));
}

CFArrayRef CMCreateArrayWithMonoInlineInt64ArrayObject(CFAllocatorRef allocator, MonoObject *monoObject) {
    return CMCreateArrayWithMonoInlineNumberArrayObject(allocator, monoObject, kCFNumberSInt64Type, sizeof(int64_t));
}

// TODO: Unsigned
CFArrayRef CMCreateArrayWithMonoInlineUInt32ArrayObject(CFAllocatorRef allocator, MonoObject *monoObject) {
    return CMCreateArrayWithMonoInlineNumberArrayObject(allocator, monoObject, kCFNumberSInt32Type, sizeof(int32_t));
}

// TODO: Unsigned
CFArrayRef CMCreateArrayWithMonoInlineUInt64ArrayObject(CFAllocatorRef allocator, MonoObject *monoObject) {
    return CMCreateArrayWithMonoInlineNumberArrayObject(allocator, monoObject, kCFNumberSInt64Type, sizeof(int64_t));
}

CFArrayRef CMCreateArrayWithMonoInlineDoubleArrayObject(CFAllocatorRef allocator, MonoObject *monoObject) {
    return CMCreateArrayWithMonoInlineNumberArrayObject(allocator, monoObject, kCFNumberDoubleType, sizeof(double));
}

CFArrayRef CMCreateArrayWithMonoInlineSingleArrayObject(CFAllocatorRef allocator, MonoObject *monoObject) {
    return CMCreateArrayWithMonoInlineNumberArrayObject(allocator, monoObject, kCFNumberFloatType, sizeof(float));
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

CFArrayRef CMCreateArrayWithMonoInlineArrayObject(CFAllocatorRef allocator, MonoArray *monoArray) {
    CFArrayRef array = NULL;
    if (monoArray) {
        uintptr_t n = mono_array_length(monoArray);
        if (n > 0) {
            CFTypeRef *values = CFAllocatorAllocate(allocator, sizeof(CFTypeRef) * n, 0);
            if (values) {
                for (uintptr_t i = 0; i < n; i++) {
                    values[i] = CMNullify(CMCreateObjectWithMonoObject(allocator, mono_array_get(monoArray, MonoObject *, i)));
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

CFArrayRef CMCreateArrayWithMonoArrayListObject(CFAllocatorRef allocator, MonoObject *monoObject) {
    CFArrayRef array = NULL;
    if (monoObject) {
        MonoClass *monoClass = mono_object_get_class(monoObject);
        int n = * (int *) mono_object_unbox(mono_property_get_value(mono_class_get_property_from_name(monoClass, "Count"), monoObject, NULL, NULL));
        if (n > 0) {
            MonoArray *monoArray = mono_array_new(mono_domain_get(), mono_get_object_class(), n);
            if (monoArray) {
                
                int zero = 0;
                void *params[2] = { monoArray, &zero };
                
                // TODO: Check for exceptions
                (void) mono_runtime_invoke(mono_class_get_method_from_name(monoClass, "CopyTo", 2), monoObject, params, NULL);
                array = CMCreateArrayWithMonoInlineArrayObject(allocator, monoArray);
            }
        } else {
            array = CFArrayCreate(allocator, NULL, 0, &kCFTypeArrayCallBacks);
        }
    }
    return array;
}

#define IS_CLASS(klass, namespace, name) (monoClass ? ( strcmp(mono_class_get_namespace(klass), namespace) == 0 && strcmp(mono_class_get_name(klass), name) == 0 ) : 0)

// Char
// Guid
// IntPtr
// Nullable<T>
// SByte
// TimeSpan
// UIntPtr
// Void

// System.Collections
//BitArray Class
//Hashtable Class
//ICollection Interface
//IDictionary Interface
//IEnumerable Interface
//IList Interface
//Queue Class
//SortedList Class
//Stack Class

// System.Collections.Generic
//Dictionary<TKey, TValue>
//HashSet<T>
//LinkedList<T>
//LinkedListNode<T>
//List<T>
//Queue<T>
//Stack<T>

// interfaces:
//ICollection<T>	Defines methods to manipulate generic collections.
//IComparer<T>	Defines a method that a type implements to compare two objects.
//IDictionary<TKey, TValue>	Represents a generic collection of key/value pairs.
//IEnumerable<T>	Exposes the enumerator, which supports a simple iteration over a collection of a specified type.
//IEnumerator<T>	Supports a simple iteration over a generic collection.
//IEqualityComparer<T>	Defines methods to support the comparison of objects for equality.
//IList<T>	Represents a collection of objects that can be individually accessed by index.
//ISet<T> Provides the base interface for the abstraction of sets.

CFTypeRef CMCreateObjectWithMonoObject(CFAllocatorRef allocator, MonoObject *monoObject) {
    static MonoClass *kCMMonoClassSystemCollectionsIDictionary = NULL;
    static MonoClass *kCMMonoClassSystemArray = NULL;
    
    if (kCMMonoClassSystemCollectionsIDictionary == NULL)
        kCMMonoClassSystemCollectionsIDictionary = mono_class_from_name(mono_get_corlib(), "System.Collections", "IDictionary");
    
    if (kCMMonoClassSystemArray == NULL)
        kCMMonoClassSystemArray = mono_class_from_name(mono_get_corlib(), "System", "Array");
    
    CFTypeRef result = NULL;
    if (monoObject) {
        MonoClass *monoClass = mono_object_get_class(monoObject);
        if      (IS_CLASS(monoClass, "System", "Boolean[]" )) result = CMCreateArrayWithMonoInlineBooleanArrayObject (allocator, monoObject);
        else if (IS_CLASS(monoClass, "System", "Byte[]"    )) result = CMCreateDataWithMonoInlineByteArrayObject     (allocator, monoObject);
        else if (IS_CLASS(monoClass, "System", "Int32[]"   )) result = CMCreateArrayWithMonoInlineInt32ArrayObject   (allocator, monoObject);
        else if (IS_CLASS(monoClass, "System", "Int64[]"   )) result = CMCreateArrayWithMonoInlineInt64ArrayObject   (allocator, monoObject);
        else if (IS_CLASS(monoClass, "System", "UInt32[]"  )) result = CMCreateArrayWithMonoInlineUInt32ArrayObject  (allocator, monoObject);
        else if (IS_CLASS(monoClass, "System", "UInt64[]"  )) result = CMCreateArrayWithMonoInlineUInt64ArrayObject  (allocator, monoObject);
        else if (IS_CLASS(monoClass, "System", "Single[]"  )) result = CMCreateArrayWithMonoInlineSingleArrayObject  (allocator, monoObject);
        else if (IS_CLASS(monoClass, "System", "Double[]"  )) result = CMCreateArrayWithMonoInlineDoubleArrayObject  (allocator, monoObject);
        else if (IS_CLASS(monoClass, "System", "String[]"  )) result = CMCreateArrayWithMonoInlineStringArrayObject  (allocator, monoObject);
        
        else if (IS_CLASS(monoClass, "System", "Boolean"  )) result = CMCreateBooleanWithMonoBooleanObject          (allocator, monoObject);
        else if (IS_CLASS(monoClass, "System", "Int32"    )) result = CMCreateNumberWithMonoInt32Object             (allocator, monoObject);
        else if (IS_CLASS(monoClass, "System", "Int64"    )) result = CMCreateNumberWithMonoInt64Object             (allocator, monoObject);
        else if (IS_CLASS(monoClass, "System", "UInt32"   )) result = CMCreateNumberWithMonoUInt32Object            (allocator, monoObject);
        else if (IS_CLASS(monoClass, "System", "UInt64"   )) result = CMCreateNumberWithMonoUInt64Object            (allocator, monoObject);

        else if (IS_CLASS(monoClass, "System", "Single"   )) result = CMCreateNumberWithMonoSingleObject            (allocator, monoObject);
        else if (IS_CLASS(monoClass, "System", "Double"   )) result = CMCreateNumberWithMonoDoubleObject            (allocator, monoObject);

        else if (IS_CLASS(monoClass, "System", "String"   )) result = CMCreateStringWithMonoStringObject            (allocator, monoObject);
        
        else if (IS_CLASS(monoClass, "System", "DateTime" )) result = CMCreateDateWithMonoDateTimeObject            (allocator, monoObject);
        
        else if (IS_CLASS(monoClass, "System.Collections", "ArrayList" )) result = CMCreateArrayWithMonoArrayListObject (allocator, monoObject);

        else if (mono_class_is_subclass_of(monoClass, kCMMonoClassSystemCollectionsIDictionary, 1)) result = CMCreateDictionaryWithMonoSubclassOfIDictionaryObject(allocator, monoObject);

        else if (mono_class_is_subclass_of(monoClass, kCMMonoClassSystemArray, 0)) result = CMCreateArrayWithMonoSubclassOfArrayObject(allocator, monoObject);
        
        else if (IS_CLASS(monoClass, "System", "")) {
            
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

MonoString *CMMonoStringWithString(CFStringRef string) {
    MonoString *monoString = NULL;
    if (string) {
        IF_UTF8(kCFAllocatorDefault, string, utf8, {
            monoString = mono_string_new(mono_domain_get(), utf8);
        });
    }
    return monoString;
}

MonoObject *CMMonoObjectWithNumber(CFNumberRef number) {
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

MonoObject *CMMonoDateTimeWithDate(CFDateRef date) {
    MonoObject *monoObject = NULL;
    if (date) {
        CFAbsoluteTime absoluteTime = CFDateGetAbsoluteTime(date);
        int64_t ticks = (int64_t) absoluteTime;
        MonoClass *monoClass = mono_class_from_name(mono_get_corlib(), "System", "DateTime");
        MonoObject *monoExceptionObject = NULL;
        monoObject = mono_object_init(mono_domain_get(), monoClass, 1, (const char *[]) { "System.Int64" }, (void *[]) { &ticks }, &monoExceptionObject);
        if (monoExceptionObject) {
            fprintf(stderr, "ERROR: exception in %p", monoExceptionObject);
        }
    }
    return monoObject;
}

MonoArray *CMMonoArrayWithArray(CFArrayRef array) {
    MonoArray *monoArray = NULL;
    if (array) {
        CFIndex count = CFArrayGetCount(array);
        monoArray = mono_array_new(mono_domain_get(), mono_get_object_class(), count);
        // TODO: we need to invoke .ctor
        for (CFIndex i = 0; i < count; i++) {
            mono_array_set(monoArray, MonoObject *, i, CMMonoObjectWithObject(CFArrayGetValueAtIndex(array, i)));
        }
    }
    return monoArray;
}

//MonoMethod *CMMethodSearchWithClass()

//bool mono_signature_has_param_type_names2(MonoMethodSignature *sig, uint32_t count, ...) {
//    bool result = 0;
//    MonoType *p = NULL;
//    void *iter = NULL;
//    va_list vp;
//    
//    va_start(vp, count);
//    
//    if (mono_signature_get_param_count(sig) == count) {
//        result = 1;
//        while ((p = mono_signature_get_params(sig, &iter))) {
//            const char *p2 = va_arg(vp, const char *);
//            if (strcmp(mono_type_get_name(p), p2) != 0) {
//                result = 0;
//                break;
//            }
//        }
//    }
//
//    va_end(vp);
//    
//    return result;
//}

//bool CMMonoMethodSignatureMatchesMonoParamTypes(MonoMethodSignature *monoMethodSignature, MonoType **monoTypes, CFIndex n) {
//    bool result = 0;
//    if (monoMethodSignature) {
//        void *paramsIterator = NULL;
//        MonoType *monoType = NULL;
//        if (mono_signature_get_param_count(monoMethodSignature) == n) {
//            
//        }
//        
//        for () {
//            
//        }
//        
//        MONO_TYPE_U2
//        
//        while ((monoType = mono_signature_get_params(monoMethodSignature, &paramsIterator))) {
//            if (strcmp(mono_type_get_name(monoType), "Int32")) {
//                monoObject = mono_object_new(mono_domain_get(), monoClass);
//                
//                int32_t n_ = (int32_t) n;
//                void *params[1] = { &n_ };
//                MonoObject *monoExceptionObject = NULL;
//                (void) mono_runtime_invoke(monoMethod, monoObject, params, &monoExceptionObject);
//                if (monoExceptionObject) {
//                    fprintf(stderr, "ERROR: Exception in constructor, %p\n", monoExceptionObject);
//                }
//                
//                break;
//            }
//        }
//    }
//    return result;
//}

//bool CMMonoMethodHasMonoParamTypes(MonoMethod *monoMethod, MonoType **types, CFIndex n) {
//    bool result = 0;
//    if (monoMethod) {
//        
//    }
//    return result;
//}

//MonoObject *CMMonoObjectInit(MonoImage *monoImageOrNull, MonoClass *monoClass, uint32_t MonoType **paramTypes, void **paramValues) {
//    MonoObject *monoObject = NULL;
//    MonoImage *monoImage = monoImageOrNull ? monoImageOrNull : mono_get_corlib();
//    if (monoImage) {
//        if (monoClass) {
//            MonoMethod *monoMethod = NULL;
//            void *iterator = NULL;
//            while ((monoMethod = mono_class_get_methods(monoClass, &iterator))) {
//                if (strcmp(mono_method_get_name(monoMethod), ".ctor") == 0) {
//                    if (mono_signature_has_param_type_names(mono_method_signature(monoMethod), 1, "Int32")) {
//                        monoObject = mono_object_new(mono_domain_get(), monoClass);
//                        
//                        int32_t n_ = (int32_t) n;
//                        void *params[1] = { &n_ };
//                        MonoObject *monoExceptionObject = NULL;
//                        (void) mono_runtime_invoke(monoMethod, monoObject, params, &monoExceptionObject);
//                        if (monoExceptionObject) {
//                            fprintf(stderr, "ERROR: Exception in constructor, %p\n", monoExceptionObject);
//                        }
//                        
//                        break;
//                    }
//                }
//            }
//        }
//    }
//    return monoObject;
//}

MonoObject *CMMonoGuidWithUUID(CFUUIDRef uuid) {
    MonoObject *monoObject = NULL;
    
    static MonoClass *monoClass = NULL; if (monoClass == NULL) monoClass = mono_class_from_name(mono_get_corlib(), "System", "Guid");
    static MonoMethod *monoCtor = NULL; if (monoCtor == NULL) monoCtor = mono_class_find_ctor_matching_param_type_names(monoClass, 11, (const char *[11]) { "System.UInt32", "System.UInt16", "System.UInt16", "System.Byte", "System.Byte", "System.Byte", "System.Byte", "System.Byte", "System.Byte", "System.Byte", "System.Byte" });

    if (uuid) {
        CFUUIDBytes bytes = CFUUIDGetUUIDBytes(uuid);
        uint32_t a = (bytes.byte0 << 24) | (bytes.byte1 << 16) | (bytes.byte2 << 8) | bytes.byte3;
        uint16_t b = (bytes.byte4 << 8) | bytes.byte5;
        uint16_t c = (bytes.byte6 << 8) | bytes.byte7;
        monoObject = mono_object_new(mono_domain_get(), monoClass);
        (void) mono_runtime_invoke(monoCtor, monoObject, (void *[11]) { &a, &b, &c, &bytes.byte8, &bytes.byte9, &bytes.byte10, &bytes.byte11, &bytes.byte12, &bytes.byte13, &bytes.byte14, &bytes.byte15 }, NULL);
    }
    return monoObject;
}

MonoObject *CMMonoBooleanObjectWithBoolean(CFBooleanRef boolean) {
    MonoObject *monoObject = NULL;
    if (boolean) {
        MonoBoolean monoBoolean = boolean == kCFBooleanTrue ? 1 : 0;
        monoObject = CM_BOX_BOOLEAN(monoBoolean);
    }
    return monoObject;
}

MonoArray *CMMonoInlineByteArrayWithData(CFDataRef data) {
    MonoArray *monoArray = NULL;
    
    static MonoClass *monoClass = NULL; if (monoClass == NULL) monoClass = mono_class_from_name(mono_get_corlib(), "System.Runtime.InteropServices", "Marshal");
    static MonoMethod *monoMethod = NULL; if (monoMethod == NULL) monoMethod = mono_class_find_method_matching_param_type_names(monoClass, "Copy", 4, (const char *[]) { "System.IntPtr", "System.Byte[]", "System.Int32", "System.Int32" });
    
    if (data) {
        CFIndex n = CFDataGetLength(data);
        monoArray = mono_array_new(mono_domain_get(), mono_get_byte_class(), n);
        
        int32_t source = (int32_t) CFDataGetBytePtr(data);
        int32_t index = 0;
        int32_t length = (int32_t) n;
        
        MonoObject *monoExceptionObject = NULL;
        (void) mono_runtime_invoke(monoMethod, NULL, (void *[]) { &source, monoArray, &index, &length }, &monoExceptionObject);
        if (monoExceptionObject) {
            fprintf(stderr, "ERROR: exception at %p\n", monoExceptionObject);
        }
    }
    return monoArray;
}

MonoObject *CMMonoHashtableWithDictionary(CFDictionaryRef dictionary) {
    MonoObject *monoObject = NULL;
    if (dictionary) {
        CFIndex n = CFDictionaryGetCount(dictionary);
        if (n > 0) {
            CFTypeRef *keys = CFAllocatorAllocate(kCFAllocatorDefault, sizeof(CFTypeRef) * n, 0);
            if (keys) {
                CFTypeRef *values = CFAllocatorAllocate(kCFAllocatorDefault, sizeof(CFTypeRef) * n, 0);
                if (values) {
                    CFDictionaryGetKeysAndValues(dictionary, keys, values);
                    
                    MonoClass *monoClass = mono_class_from_name(mono_get_corlib(), "System.Collections", "Hashtable");
                    
                    int32_t n_ = n;
                    monoObject = mono_object_init(mono_domain_get(), monoClass, 1, (const char *[]) { "System.Int32" }, (void *[]) { &n_ }, NULL);
                    
                    // Set key/values
                    {
                        if (monoObject) {
                            MonoMethodDesc *monoMethodDesc = mono_method_desc_new("System.Collections.Hashtable:Add", 1);
                            if (monoMethodDesc) {
                                MonoMethod *monoMethod = mono_method_desc_search_in_class(monoMethodDesc, mono_object_get_class(monoObject));
                                if (monoMethod) {
                                    for (CFIndex i = 0; i < n; i++) {
                                        void *params[] = { CMMonoObjectWithObject(keys[i]), CMMonoObjectWithObject(values[i]) };
                                        if (params[0] && params[1]) {
                                            MonoObject *monoExceptionObject = NULL;
                                            (void) mono_runtime_invoke(monoMethod, monoObject, params, &monoExceptionObject);
                                            if (monoExceptionObject) {
                                                fprintf(stderr, "ERROR: exception %p\n", monoExceptionObject);
                                            }
                                        } else {
                                            fprintf(stderr, "ERROR: Unexpected null in monoKeyObject = %p or monoValueObject = %p\n", params[0], params[1]);
                                        }
                                    }
                                }
                                mono_method_desc_free(monoMethodDesc);
                            }
                        }
                    }
                    CFAllocatorDeallocate(kCFAllocatorDefault, values);
                }
                CFAllocatorDeallocate(kCFAllocatorDefault, keys);
            }
        } else {
        }
        //        monoObject = mono_object_new(mono_domain_get(), mono_class_from_name(mono_assembly_get_image(mono_assembly_get_main()), "System.Collections.Generic", "Dictionary"))
    }
    return monoObject;
}

//MonoObject *CMMonoDictionaryOfStringAndObjectWithDictionary(CFDictionaryRef dictionary) {
//    MonoObject *monoObject = NULL;
//    if (dictionary) {
//        CFIndex n = CFDictionaryGetCount(dictionary);
//        monoObject
//        if (n > 0) {
//            CFTypeRef *keys = CFAllocatorAllocate(kCFAllocatorDefault, sizeof(CFTypeRef) * n, 0);
//            CFTypeRef *values = CFAllocatorAllocate(kCFAllocatorDefault, sizeof(CFTypeRef) * n, 0);
//        } else {
//        }
////        monoObject = mono_object_new(mono_domain_get(), mono_class_from_name(mono_assembly_get_image(mono_assembly_get_main()), "System.Collections.Generic", "Dictionary"))
//    }
//    return monoObject;
//}

MonoObject *CMMonoObjectWithObject(CFTypeRef object) {
    MonoObject *monoObject = NULL;
    if (object) {
             if (CFGetTypeID(object) == CFStringGetTypeID())     monoObject = (MonoObject *) CMMonoStringWithString         ((CFStringRef)     object);
        else if (CFGetTypeID(object) == CFNumberGetTypeID())     monoObject =                CMMonoObjectWithNumber         ((CFNumberRef)     object);
        else if (CFGetTypeID(object) == CFArrayGetTypeID())      monoObject = (MonoObject *) CMMonoArrayWithArray           ((CFArrayRef)      object);
        else if (CFGetTypeID(object) == CFDictionaryGetTypeID()) monoObject =                CMMonoHashtableWithDictionary  ((CFDictionaryRef) object);
        else if (CFGetTypeID(object) == CFDateGetTypeID())       monoObject =                CMMonoDateTimeWithDate         ((CFDateRef)       object);
        else if (CFGetTypeID(object) == CFDataGetTypeID())       monoObject = (MonoObject *) CMMonoInlineByteArrayWithData  ((CFDataRef)       object);
        else if (CFGetTypeID(object) == CFBooleanGetTypeID())    monoObject = (MonoObject *) CMMonoBooleanObjectWithBoolean ((CFBooleanRef)    object);
        else if (CFGetTypeID(object) == CFNullGetTypeID())       monoObject = (MonoObject *) NULL;
        else if (CFGetTypeID(object) == CFUUIDGetTypeID())       monoObject = (MonoObject *) CMMonoGuidWithUUID             ((CFUUIDRef)       object);
        //        CFNullGetTypeID();
        

        //CFUUIDCreateWithBytes(<#CFAllocatorRef alloc#>, <#UInt8 byte0#>, <#UInt8 byte1#>, <#UInt8 byte2#>, <#UInt8 byte3#>, <#UInt8 byte4#>, <#UInt8 byte5#>, <#UInt8 byte6#>, <#UInt8 byte7#>, <#UInt8 byte8#>, <#UInt8 byte9#>, <#UInt8 byte10#>, <#UInt8 byte11#>, <#UInt8 byte12#>, <#UInt8 byte13#>, <#UInt8 byte14#>, <#UInt8 byte15#>)
        
//        else if (CFGetTypeID(object) == CFBagGetTypeID())        monoObject =                CMMonoBagWithBag              ((CFBagRef)        object);
//        else if (CFGetTypeID(object) == CFBinaryHeapGetTypeID())        monoObject =                CMMonoBagWithBag              ((CFBagRef)        object);

//        CFBitVectorGetTypeID();
//        CFErrorGetTypeID();
//        CFMutableSetRef;
//        CFPropertyListRef
//        CFSocketGetTypeID();
//        CFTimeZoneGetTypeID();
//        CFTreeGetTypeID();
//        CFURLGetTypeID();
//        CFUUIDGetTypeID();
//        CFXMLNodeGetTypeID();
    }
    return monoObject;
}

