#pragma once

#define SINGLEGRAPHSIZE 6
#define DOUBLEGRAPHSIZE 6
#define MARKERSCALE 4
#define MATRIXCELLWIDTH 1.8
#define MATRIXCELLHEIGHT 1.8
#define TEX_COORD_OFFSET 0.25
#define TEX_COORD_STEP 1.25
#define DENSITY_MAX_DOTS 500
#define AREA_OFFSET 0.03

#define DATASET 0
#if DATASET == 0
#define NUMNODE 74
#define NUMGRAPH 27
#else
//#define NUMNODE 167
//#define NUMNODE 148
#define NUMNODE 100
#define NUMGRAPH 18
#endif