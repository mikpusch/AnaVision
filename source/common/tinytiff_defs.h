/*
     Copyright (c) 2008-2020 Jan W. Krieger (<jan@jkrieger.de>), German Cancer Research Center (DKFZ) & IWR, University of Heidelberg
  
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
 #ifndef TIFYTIFF_DEFS_H
 #define TIFYTIFF_DEFS_H
  
  
 #ifndef TINYTIFF_TRUE
  
 #  define TINYTIFF_TRUE 1
 #endif
 #ifndef TINYTIFF_FALSE
  
 #  define TINYTIFF_FALSE 0
 #endif
  
  
 #define TINYTIFF_SAMPLEFORMAT_UINT 1
  
 #define TINYTIFF_SAMPLEFORMAT_INT 2
  
 #define TINYTIFF_SAMPLEFORMAT_FLOAT 3
  
 #define TINYTIFF_SAMPLEFORMAT_UNDEFINED 4
  
  
  
 enum TinyTIFFSampleLayout {
     TinyTIFF_Interleaved,                 
     TinyTIFF_Chunky=TinyTIFF_Interleaved, 
     TinyTIFF_Separate,                    
     TinyTIFF_Planar=TinyTIFF_Separate     
 };
 #endif // TIFYTIFF_DEFS_H