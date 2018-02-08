/****************************************************************************

  Copyright (C) 2010 Blake W. Ford

  This file is part of the roughdraft project.


  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.


  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it

  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  zlib style

************************************************************************/
#ifndef bmpconstants_h
#define bmpconstants_h

//BMP Offsets as established by standard
#define BSpace 0
#define MSpace 1
#define BEGINBMPSZ 2
#define ENDBMPSZ 5	
#define BEGINAPPSPACE 6
#define ENDAPPSPACE 9
#define BEGINPXLOFFSET 10
#define ENDPXLOFFSET 13
#define BEGINHDRREM 14
#define ENDHDRREM 17
#define BEGINWIDTH 18
#define ENDWIDTH 21
#define BEGINHEIGHT 22
#define ENDHEIGHT 25
#define BEGINPLANES 26
#define ENDPLANES 27
#define BEGINDEPTH 28
#define ENDDEPTH 29
#define BEGINBI_RGB 30
#define ENDBI_RGB 33
#define BEGINRAWDATASZ 34 
#define ENDRAWDATASZ 37
#define BEGINHORRES 38
#define ENDHORRES 41
#define BEGINVERRES 42
#define ENDVERRES 45
#define BEGINPALCOLORS 46
#define ENDPALCOLORS 49
#define BEGINIMPCOLORS 50
#define ENDIMPCOLORS 53
#define HEADERSIZE 54


#endif
