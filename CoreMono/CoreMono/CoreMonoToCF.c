//
// CoreMono Framework
//
// (c) 2011 Mirek Rusin <mirek@me.com>
//

#include "CoreMonoToCF.h"

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

// TODO: HACK: FIXME: Should be unsigned, but how?
CFNumberRef CMCreateNumberWithMonoUInt32Object(CFAllocatorRef allocator, MonoObject *monoObject) {
    CFNumberRef number = NULL;
    if (monoObject) {
        int32_t value = (int32_t) (* (uint32_t *) mono_object_unbox(monoObject));
        number = CFNumberCreate(allocator, kCFNumberSInt32Type, &value);
    }
    return number;
}

// TODO: HACK: FIXME: Should be unsigned, but how?
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

CFDictionaryRef CMCreateDictionaryWithPropertiesOfMonoObject(CFAllocatorRef allocator, MonoObject *monoObject) {
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
    return (CFDictionaryRef) dictionary;
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

// TODO: HACK: FIXME: Should be unsigned, but how?
CFArrayRef CMCreateArrayWithMonoInlineUInt32ArrayObject(CFAllocatorRef allocator, MonoObject *monoObject) {
    return CMCreateArrayWithMonoInlineNumberArrayObject(allocator, monoObject, kCFNumberSInt32Type, sizeof(int32_t));
}

// TODO: HACK: FIXME: Should be unsigned, but how?
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
