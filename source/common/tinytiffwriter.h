/*
     Copyright (c) 2008-220 Jan W. Krieger (<jan@jkrieger.de>), German Cancer Research Center (DKFZ) & IWR, University of Heidelberg
  
     This software is free software: you can redistribute it and/or modify
     it under the terms of the GNU Lesser General Public License (LGPL) as published by
     the Free Software Foundation, either version 3 of the License, or
     (at your option) any later version.
  
     This program is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
     GNU General Public License for more details.
  
     You should have received a copy of the GNU General Public License
     along with this program.  If not, see <http://www.gnu.org/licenses/>.
  
  
 */
  
  
  
  
 #ifndef TINYTIFFWRITER_H
 #define TINYTIFFWRITER_H
  
// #include "tinytiff_export.h"
 #include "tinytiff_defs.h"
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
// #include "inttypes.h"
  
 typedef struct TinyTIFFWriterFile TinyTIFFWriterFile; // forward
#define uint32_t unsigned __int32
#define uint16_t unsigned __int16
#define uint8_t unsigned __int8
#define uint64_t unsigned __int64

  
  
 #ifdef __cplusplus
 extern "C" {
 #endif
  
#define TINYTIFF_EXPORT

     TINYTIFF_EXPORT const char* TinyTIFFWriter_getVersion();
  
     TINYTIFF_EXPORT int TinyTIFFWriter_getMaxDescriptionTextSize();
  
     TINYTIFF_EXPORT const char* TinyTIFFWriter_getLastError(TinyTIFFWriterFile* tiff);
  
     TINYTIFF_EXPORT int TinyTIFFWriter_wasError(TinyTIFFWriterFile* tiff);
  
     TINYTIFF_EXPORT int TinyTIFFWriter_success(TinyTIFFWriterFile* tiff);
  
     enum TinyTIFFWriterSampleInterpretation {
         TinyTIFFWriter_AutodetectSampleInterpetation, 
         TinyTIFFWriter_Greyscale, 
         TinyTIFFWriter_GreyscaleAndAlpha, 
         TinyTIFFWriter_RGB, 
         TinyTIFFWriter_RGBA 
     };
  
     enum TinyTIFFWriterSampleFormat {
         TinyTIFFWriter_UInt, 
         TinyTIFFWriter_Int, 
         TinyTIFFWriter_Float 
     };
  
     TINYTIFF_EXPORT TinyTIFFWriterFile* TinyTIFFWriter_open(const char* filename, unsigned __int16 bitsPerSample,
		 enum TinyTIFFWriterSampleFormat sampleFormat,  unsigned __int16 samples,  unsigned __int32 width,
		  unsigned __int32 height, enum TinyTIFFWriterSampleInterpretation sampleInterpretation);
 
     TINYTIFF_EXPORT int TinyTIFFWriter_writeImageMultiSample(TinyTIFFWriterFile* tiff, const void* data, enum TinyTIFFSampleLayout inputOrganisation, enum TinyTIFFSampleLayout outputOrganization);
  
     TINYTIFF_EXPORT int TinyTIFFWriter_writeImagePlanarReorder(TinyTIFFWriterFile* tiff, const void* data);
  
  
     TINYTIFF_EXPORT int TinyTIFFWriter_writeImageChunkyReorder(TinyTIFFWriterFile* tiff, const void* data);
  
  
     TINYTIFF_EXPORT int TinyTIFFWriter_writeImage(TinyTIFFWriterFile* tiff, const void* data);
  
     TINYTIFF_EXPORT void TinyTIFFWriter_close_withmetadatadescription(TinyTIFFWriterFile* tiff, double pixel_width, double pixel_height, double frametime, double deltaz);
  
     TINYTIFF_EXPORT void TinyTIFFWriter_close(TinyTIFFWriterFile* tiff);
  
  
     TINYTIFF_EXPORT void TinyTIFFWriter_close_withdescription(TinyTIFFWriterFile* tiff, const char* imageDescription);
 #ifdef __cplusplus
 }
 #endif
  
 #endif // TINYTIFFWRITER_H
  
