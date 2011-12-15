//
//  main.c
//  test
//
//  Created by Mirek Rusin on 11/12/2011.
//  Copyright (c) 2011 Mirek Rusin. All rights reserved.
//

#include <Foundation/Foundation.h>
#include <CoreFoundation/CoreFoundation.h>
#include "CoreMono.h"

int main (int argc, const char *argv[]) {
    CMDomainRef domain = CMJITCreateDomainByInitializingWithPath(kCFAllocatorDefault, CFSTR("test1.exe"));
    if (domain) {
        CMAssemblyRef assembly = CMDomainCreateAssemblyByOpeningPath(domain, CFSTR("test1.exe"));
        if (assembly) {
            CMJITExec(domain, assembly, argc, (char **) argv);
            
            CMClassRef klass = CMClassCreateWithAssembly(assembly, CFSTR("test"), CFSTR("test1"));
            if (klass) {

//                CFTypeRef result = CMImageCreateObjectByInvokingMethodWithName(CMAssemblyGetImage(assembly), CFSTR("test.test1:ReturnInt32WithInt32"), TRUE, &i);
//                CFShow(result);

                CMMethodDescRef methodDesc = CMMethodDescCreate(kCFAllocatorDefault, CFSTR("test.test1:PrintDictionary"), TRUE);
                if (methodDesc) {
                    
                    MonoMethod *monoMethod = CMImageGetMonoMethodWithMethodDesc(CMAssemblyGetImage(assembly), methodDesc);
                    if (monoMethod) {
//                        int32_t i = 2011;
                        
                        NSDictionary *dictionary = [NSDictionary dictionaryWithObjectsAndKeys: @"foo", @"bar", nil];

                        void *params[1] = { CMMonoObjectWithObject(dictionary) };
                        MonoObject *monoObject = mono_runtime_invoke(monoMethod, NULL, params, NULL);
                        CFTypeRef object = CMCreateObjectWithMonoObject(NULL, monoObject);
                        CFShow(object);
                        if (object) {
                            CFRelease(object);
                        }
                    } else {
                        printf("Error creating method\n");
                    }
                    
                } else {
                    printf("Error creating method desc\n");
                }

                
//                CMMethodRef method = CMMethodCreateWithName(kCFAllocatorDefault, CFSTR(":ReturnInt32WithInt32"), 0);
//                if (method) {
//                    
//                    int32_t i = 2011;
//                    void *args[1] = { CM_BOX_INT32(i) };
//                    CMRuntimeInvoke(method, NULL, args, NULL);
////                    CMMethodCreateObjectByInvoking(method, CM_BOX_INT32(2011));
//                }
//                
//                CMMethodCreate(klass);
                
//                CMClassRelease(klass);
            } else {
                printf("Error creating class\n");
            }
            
//            CMMethodDescRef methodDesc = CMMethodDescCreate(kCFAllocatorDefault, CFSTR("test1:ReturnInt32WithInt32"), 0);
//            if (methodDesc) {
//                CMMethodRef method = CMMethodCreateWithName(CFSTR(":ReturnInt32WithInt32"), 0
//                if (method) {
//                    CMMethodCreateObjectByInvoking(method, CM_BOX_INT32(2011));
//                }
//            } else {
//                printf("Error getting method desc\n");
//            }
//            
//            CMRuntimeInvoke(method)
//            
//            "ReturnInt32WithInt32"
            
//            [NSNumber numberWithInt: 2011];
            
//            CMObjectRef string = CMObjectAllocateAndInitWithString(CFSTR("foo"));
//            CMObjectRef i = CMObjectAllocateWithSInt32(-32);
            
            //CMMethodInvoke("test1:foo", 3, 1, 2, "foo");
            
            CMAssemblyRelease(assembly);
        } else {
            printf("Error creating assembly\n");
        }
        CMDomainRelease(domain);
    } else {
        printf("Error creating domain\n");
    }
    return 0;
}

