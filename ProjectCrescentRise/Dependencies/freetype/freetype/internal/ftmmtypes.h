/****************************************************************************
 *
 * ftmmtypes.h
 *
 *   OpenType Variations type definitions for internal use
 *   with the multi-masters service (specification).
 *
 * Copyright (C) 2022-2025 by
 * David Turner, Robert Wilhelm, Werner Lemberg, George Williams, and
 * Dominik Röttsches.
 *
 * This file is part of the FreeType project, and may only be used,
 * modified, and distributed under the terms of the FreeType project
 * license, LICENSE.TXT.  By continuing to use, modify, or distribute
 * this file you indicate that you have read the license and
 * understand and accept it fully.
 *
 */


#ifndef FTMMTYPES_H_
#define FTMMTYPES_H_

FT_BEGIN_HEADER


  typedef FT_Int32  FT_ItemVarDelta;

  typedef struct  GX_ItemVarDataRec_
  {
    FT_UInt            itemCount;      
    FT_UInt            regionIdxCount; 
    FT_UInt*           regionIndices;  
                                       
    FT_Byte*           deltaSet;       
                                       
    FT_UShort          wordDeltaCount; 
                                       
                                       
    FT_Bool            longWords;      
                                       
                                       
                                       
  } GX_ItemVarDataRec, *GX_ItemVarData;


  
  typedef struct  GX_AxisCoordsRec_
  {
    FT_Fixed  startCoord;
    FT_Fixed  peakCoord;      
    FT_Fixed  endCoord;

  } GX_AxisCoordsRec, *GX_AxisCoords;


  typedef struct  GX_VarRegionRec_
  {
    GX_AxisCoords  axisList;               

  } GX_VarRegionRec, *GX_VarRegion;


  
  typedef struct  GX_ItemVarStoreRec_
  {
    FT_UInt         dataCount;
    GX_ItemVarData  varData;            
                                        
    FT_UShort     axisCount;
    FT_UInt       regionCount;          
    GX_VarRegion  varRegionList;

  } GX_ItemVarStoreRec, *GX_ItemVarStore;


  typedef struct  GX_DeltaSetIdxMapRec_
  {
    FT_ULong  mapCount;
    FT_UInt*  outerIndex;               
    FT_UInt*  innerIndex;               

  } GX_DeltaSetIdxMapRec, *GX_DeltaSetIdxMap;


FT_END_HEADER

#endif 



