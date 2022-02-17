#ifndef _MAIN_H
#define _MAIN_H

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <gl\gl.h>										
#include <gl\glu.h>										
#include <gl\glaux.h>									
#include <math.h>

#include "Terrain.h"									

#define GL_TEXTURE0_ARB                     0x84C0
#define GL_TEXTURE1_ARB                     0x84C1

#define GL_COMBINE_ARB						0x8570
#define GL_RGB_SCALE_ARB					0x8573

typedef void (APIENTRY * PFNGLMULTITEXCOORD2FARBPROC) (GLenum target, GLfloat s, GLfloat t);
typedef void (APIENTRY * PFNGLACTIVETEXTUREARBPROC) (GLenum target);

extern PFNGLMULTITEXCOORD2FARBPROC			glMultiTexCoord2fARB;
extern PFNGLACTIVETEXTUREARBPROC			glActiveTextureARB;

#define MAX_TEXTURES 1000

extern UINT g_Texture[MAX_TEXTURES];	

#define GL_CLAMP_TO_EDGE	0x812F

#define SCREEN_WIDTH 800								
#define SCREEN_HEIGHT 600							
#define SCREEN_DEPTH 16									

extern bool  g_bFullScreen;								
extern HWND  g_hWnd;									
extern RECT  g_rRect;									
extern HDC   g_hDC;										
extern HGLRC g_hRC;					
extern HINSTANCE g_hInstance;				


struct CVector3
{
public:
	CVector3() {}

	CVector3(float X, float Y, float Z) 
	{ 
		x = X; y = Y; z = Z;
	}

	CVector3 operator+(CVector3 vVector)
	{
		return CVector3(vVector.x + x, vVector.y + y, vVector.z + z);
	}

	CVector3 operator-(CVector3 vVector)
	{
		return CVector3(x - vVector.x, y - vVector.y, z - vVector.z);
	}
	
	CVector3 operator*(float num)
	{
		return CVector3(x * num, y * num, z * num);
	}

	CVector3 operator/(float num)
	{
		return CVector3(x / num, y / num, z / num);
	}

	float x, y, z;						
};

		int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprev, PSTR cmdline, int ishow);
		LRESULT CALLBACK WinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
		WPARAM MainLoop();
		bool CreateTexture(UINT &texture, LPSTR strFileName);
		void ChangeToFullScreen();
		HWND CreateMyWindow(LPSTR strWindowName, int width, int height, DWORD dwStyle, bool bFullScreen, HINSTANCE hInstance);
		bool bSetupPixelFormat(HDC hdc);
		void SizeOpenGLScreen(int width, int height);
		void InitializeOpenGL(int width, int height);
		void Init(HWND hWnd);
		void RenderScene();
		void DeInit();
		GLvoid KillFont(GLvoid);
		GLvoid glPrint(GLint x, GLint y, char *string, int set);
		GLvoid BuildFont(GLvoid);

#endif 
