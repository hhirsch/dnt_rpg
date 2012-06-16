/* 
 DNT: a satirical post-apocalyptical RPG.
 Copyright (C) 2005-2012 DNTeam <dnt@dnteam.org>
 
 This file is part of DNT.
 
 DNT is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 DNT is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with DNT.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "config_mac.h"
#include <CoreFoundation/CoreFoundation.h>
using namespace std;

/************************************************************************	
 *                              macBundlePath                           *
 ************************************************************************/	
string macBundlePath()
{
   char path[1024];
   CFBundleRef mainBundle = CFBundleGetMainBundle();

   assert(mainBundle);
   CFURLRef mainBundleURL = CFBundleCopyBundleURL(mainBundle);
   assert(mainBundleURL);
   
   CFStringRef cfStringRef = CFURLCopyFileSystemPath(mainBundleURL, 
                                                     kCFURLPOSIXPathStyle);
   assert(cfStringRef);

   CFStringGetCString(cfStringRef, path, 1024, kCFStringEncodingASCII);

   CFRelease(mainBundleURL);
   CFRelease(cfStringRef);

   return((string)(path));
}

