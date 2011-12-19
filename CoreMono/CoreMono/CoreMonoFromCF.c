//
// CoreMono Framework
//
// (c) 2011 Mirek Rusin <mirek@me.com>
//

#include "CoreMonoFromCF.h"

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

MonoArray *CMMonoInlineObjectArrayWithArray(CFArrayRef array) {
    MonoArray *monoArray = NULL;
    if (array) {
        CFIndex n = CFArrayGetCount(array);
        monoArray = mono_array_new(mono_domain_get(), mono_get_object_class(), n);
        for (CFIndex i = 0; i < n; i++) {
            mono_array_set(monoArray, MonoObject *, i, CMMonoObjectWithObject(CFArrayGetValueAtIndex(array, i)));
        }
    }
    return monoArray;
}

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

MonoObject *CMMonoUriWithURL(CFURLRef url) {
    MonoObject *monoObject = NULL;
    
    static MonoClass *monoClass = NULL; if (monoClass == NULL) monoClass = mono_class_from_name(mono_get_corlib(), "System", "Uri");
    static MonoMethod *monoCtor = NULL; if (monoCtor == NULL) monoCtor = mono_class_find_ctor_matching_param_type_names(monoClass, 1, (const char *[1]) { "System.String" });
    
    if (url) {
        CFStringRef string = CFURLGetString(url);
        IF_UTF8(kCFAllocatorDefault, string, utf8, {
            monoObject = mono_object_new(mono_domain_get(), monoClass);
            (void) mono_runtime_invoke(monoCtor, monoObject, (void *[1]) { utf8 }, NULL);
        });
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

// Create mono object with Core Foundation object.
// TODO: CFBagGetTypeID, CFBinaryHeapGetTypeID, CFBitVectorGetTypeID, CFErrorGetTypeID, CFSetRef, CFPropertyListRef, CFSocketGetTypeID, CFTimeZoneGetTypeID, CFTreeGetTypeID, CFXMLNodeGetTypeID
MonoObject *CMMonoObjectWithObject(CFTypeRef object) {
    MonoObject *monoObject = NULL;
    if (object) {
        if      (CFGetTypeID(object) == CFNullGetTypeID())       monoObject = (MonoObject *) NULL;
        else if (CFGetTypeID(object) == CFStringGetTypeID())     monoObject = (MonoObject *) CMMonoStringWithString           ((CFStringRef)     object);
        else if (CFGetTypeID(object) == CFNumberGetTypeID())     monoObject =                CMMonoObjectWithNumber           ((CFNumberRef)     object);
        else if (CFGetTypeID(object) == CFArrayGetTypeID())      monoObject = (MonoObject *) CMMonoInlineObjectArrayWithArray ((CFArrayRef)      object);
        else if (CFGetTypeID(object) == CFDictionaryGetTypeID()) monoObject =                CMMonoHashtableWithDictionary    ((CFDictionaryRef) object);
        else if (CFGetTypeID(object) == CFDateGetTypeID())       monoObject =                CMMonoDateTimeWithDate           ((CFDateRef)       object);
        else if (CFGetTypeID(object) == CFDataGetTypeID())       monoObject = (MonoObject *) CMMonoInlineByteArrayWithData    ((CFDataRef)       object);
        else if (CFGetTypeID(object) == CFBooleanGetTypeID())    monoObject = (MonoObject *) CMMonoBooleanObjectWithBoolean   ((CFBooleanRef)    object);
        else if (CFGetTypeID(object) == CFUUIDGetTypeID())       monoObject = (MonoObject *) CMMonoGuidWithUUID               ((CFUUIDRef)       object);
        else if (CFGetTypeID(object) == CFURLGetTypeID())        monoObject = (MonoObject *) CMMonoUriWithURL                 ((CFURLRef)        object);
    }
    return monoObject;
}
