//
//  CMDomain.h
//  CoreMono
//
//  Copyright (c) 2011 Mirek Rusin. All rights reserved.
//

#ifndef CORE_MONO_CMDOMAIN_H
#define CORE_MONO_CMDOMAIN_H

#include "CoreMono.h"

CMDomainRef __CMDomainCreate                 (CFAllocatorRef allocator);
CMDomainRef CMDomainCreateWithPath           (CFAllocatorRef allocator, CFStringRef path);
CMDomainRef CMDomainCreateWithNameAndVersion (CFAllocatorRef allocator, CFStringRef name, CFStringRef version);

CMDomainRef CMDomainRetain                   (CMDomainRef domain);
CMDomainRef CMDomainRelease                  (CMDomainRef domain);

//CMAssemblyRef CMDomainCreateAssembly         (CMDomainRef domain, CFStringRef path);

//@interface CMDomain : NSObject {
//@private
//    CMDomainRef domain;
//}
//@end
//
//- (CMDomain *) initWithPath: (NSString *) path NS_RETURNS_RETAINED;
//- (CMDomain *) initWithName: (NSString *) name version: (NSString *) version NS_RETURNS_RETAINED;
//
//+ (CMDomain *) domainWithPath: (NSString *) path;
//+ (CMDomain *) domainWithName: (NSString *) name version: (NSString *) version;

#endif
