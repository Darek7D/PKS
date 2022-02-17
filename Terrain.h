#ifndef _TERRAIN_H
#define _TERRAIN_H

#define MAP_SIZE		1024				
#define STEP_SIZE		16				
#define HEIGHT_RATIO	15.0f
#define SCALEXY         15.0f


/*!Zwraca wysokosc terenu w podanym miejscu*/
int Height(BYTE *pHeightMap, int X, int Y);
float Heightf(BYTE *pHeightMap, float X, float Y);
void LoadRawFile(LPSTR strName, int nSize, BYTE *pHeightMap);
void RenderHeightMap(BYTE *pHeightMap);

#endif