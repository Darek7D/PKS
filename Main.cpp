//Zalaczenie bibliotek
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glaux.lib")

#include "main.h"						
#include "camera.h"								
#include "models.h"			

bool  g_bFullScreen = true;
HWND  g_hWnd;									// Uchwyt okna
RECT  g_rRect;									
HDC   g_hDC;									// Uchwyt do kontekstu urzadzenia
HGLRC g_hRC;									// Kontekst rysowania dla OpenGL
HINSTANCE g_hInstance;							

PFNGLMULTITEXCOORD2FARBPROC		glMultiTexCoord2fARB	 = NULL;
PFNGLACTIVETEXTUREARBPROC		glActiveTextureARB		 = NULL;

// Kontroluje czy mamy wlaczone teksture detali (do pokrycia terenu)
bool g_bDetail = true;

// This handles the current scale for the texture matrix for the detail texture
int g_DetailScale = 16;

//Deklaracje klas
CCamera g_Camera;
models Models;

bool view=false;

BYTE g_HeightMap[MAP_SIZE*MAP_SIZE]; // Przechowywanie mapy wysokosci		
bool  g_bRenderMode = true;	// Czy chcemy tryb pelnego wypelnienia czy tylko siatke			
UINT g_Texture[MAX_TEXTURES] = {0}; //Przechowywanie tekstur				

/*Definicje numerow tekstur*/
#define BACK_ID		11	
#define FRONT_ID	12
#define BOTTOM_ID	13
#define TOP_ID		14
#define LEFT_ID		15
#define RIGHT_ID	16

GLuint	base;				// Display list dla czcionek
GLuint	loop;		

float wspc=0;

/*!Rysowanie SkyBox*/
void CreateSkyBox(float x, float y, float z, float width, float height, float length)
{
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, g_Texture[BACK_ID]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Ustawienie srodka 
	x = x - width  / 2;
	y = y - height / 2;
	z = z - length / 2;

	glBegin(GL_QUADS);		
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y,			z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,			y + height, z);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,			y,			z);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, g_Texture[FRONT_ID]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBegin(GL_QUADS);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,			y,			z + length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,			y + height, z + length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z + length); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,			z + length);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, g_Texture[BOTTOM_ID]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBegin(GL_QUADS);		
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,			y,			z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,			y,			z + length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y,			z + length); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,			z);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, g_Texture[TOP_ID]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBegin(GL_QUADS);		
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z + length); 
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,			y + height,	z + length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,			y + height,	z);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, g_Texture[LEFT_ID]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBegin(GL_QUADS);		
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,			y + height,	z);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,			y + height,	z + length); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,			y,			z + length);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,			y,			z);		
	glEnd();

	glBindTexture(GL_TEXTURE_2D, g_Texture[RIGHT_ID]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBegin(GL_QUADS);		
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,			z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y,			z + length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height,	z + length); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height,	z);
	glEnd();
}

GLvoid BuildFont(GLvoid)								
{
	float	cx;											
	float	cy;											

	base=glGenLists(256);								
	glBindTexture(GL_TEXTURE_2D, g_Texture[25]);		// Wybor czcionki
	for (loop=0; loop<256; loop++)						
	{
		cx=float(loop%16)/16.0f;						// Pozycja X obecnego znaku
		cy=float(loop/16)/16.0f;						// Pozycja Y obecnego znaku

		glNewList(base+loop,GL_COMPILE);				// Start budowania listy
			glBegin(GL_QUADS);							// Uzyj GL_QUADS do jednego znaku
				glTexCoord2f(cx,1-cy-0.0625f);			
				glVertex2i(0,0);						
				glTexCoord2f(cx+0.0625f,1-cy-0.0625f);	
				glVertex2i(16,0);						
				glTexCoord2f(cx+0.0625f,1-cy);			
				glVertex2i(16,16);						
				glTexCoord2f(cx,1-cy);					
				glVertex2i(0,16);						
			glEnd();								
			glTranslated(10,0,0);						// Przesun na prawa strone znaku
		glEndList();								
	}													
}

GLvoid KillFont(GLvoid)									// Usuniecie czcionki z pamieci
{
	glDeleteLists(base,256);							
}

GLvoid glPrint(GLint x, GLint y, char *string, int set)	// Where The Printing Happens
{
	glEnable(GL_BLEND);
	if (set>1)
	{
		set=1;
	}
	glBindTexture(GL_TEXTURE_2D, g_Texture[25]);		// Wybierz teksture czcionek
	glDisable(GL_DEPTH_TEST);							
	glMatrixMode(GL_PROJECTION);						// Wybranie macierza projekcji
	glPushMatrix();										// Zachowanie macierza
	glLoadIdentity();									// Restart macierza
	glOrtho(0,640,0,480,-1,1);							// Set Up An Ortho Screen
	glMatrixMode(GL_MODELVIEW);							
	glPushMatrix();										// Zachowanie macierza
	glLoadIdentity();									// Restart macierza
	glTranslated(x,y,0);								// Ustawienie pozycji tekstu
	glListBase(base-32+(128*set));						// Wybranie czcionki
	glCallLists(strlen(string),GL_BYTE,string);			// Wypisanie tekstu na ekran
	glMatrixMode(GL_PROJECTION);						// Wybranie macierza projekcji
	glPopMatrix();										// Przywrocenie zapisanego macierza
	glMatrixMode(GL_MODELVIEW);							
	glPopMatrix();										// Przywrocenie zapisanego macierza
	glEnable(GL_DEPTH_TEST);							
    glDisable(GL_BLEND);
}

void Init(HWND hWnd)
{
	g_hWnd = hWnd;										
	GetClientRect(g_hWnd, &g_rRect);					
	InitializeOpenGL(g_rRect.right, g_rRect.bottom);	

	glActiveTextureARB		= (PFNGLACTIVETEXTUREARBPROC)		wglGetProcAddress("glActiveTextureARB");
	glMultiTexCoord2fARB	= (PFNGLMULTITEXCOORD2FARBPROC)		wglGetProcAddress("glMultiTexCoord2fARB");

	// Upewnienie sie czy wersja OpenGl w systemie ma wspomaganie multitexturing
	if(!glActiveTextureARB || !glMultiTexCoord2fARB)
	{
		MessageBox(g_hWnd, "Twoje obecne ustawienie nie wspomaga multitexturing", "Error", MB_OK);
		PostQuitMessage(0);
	}

	// Wczytanie danych z pliku z "mapa wysokosci" do tablicy g_HeightMap
	LoadRawFile("data/Terrain.raw", MAP_SIZE * MAP_SIZE, g_HeightMap);	

	glEnable(GL_DEPTH_TEST);							// Wlacz test glebokosci
	glEnable(GL_TEXTURE_2D);							// Wlacz mapowanie tekstur
	glEnable(GL_CULL_FACE);								// Niewyswietlanie tylnej strony tekstur

	int tmpnbr;
	char tmpstring[40];
	FILE *fp = fopen("data/textures.dat", "r");
	if(!fp)	MessageBox(NULL, "Nie mozna otworzyc pliku z definicjami tekstur", "Error", MB_OK);
	while(1)
	{
		int result = fscanf(fp, "%d %s\n",&tmpnbr,&tmpstring);
		if(result == EOF) 
			break;
		CreateTexture(g_Texture[tmpnbr],tmpstring);
	}
	fclose(fp);

	glBlendFunc(GL_SRC_ALPHA,GL_ONE);

    BuildFont();

	g_Camera.PositionCamera( 280, 35, 225,  281, 35, 225,  0, 1, 0);
	//Models.LoadModels();
	Models.LoadASEModels();
}

WPARAM MainLoop()
{
	MSG msg;

	while(1)										
 	{													
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
        { 
			if(msg.message == WM_QUIT)					
				break;
            TranslateMessage(&msg);						
            DispatchMessage(&msg);						
        }
		else									
		{ 
			g_Camera.Update();					
			RenderScene();		//Rysowanie sceny				
        } 
	}
	DeInit();											// Wyczysc i zwolnij pamiec
	return(msg.wParam);									// Wyjdz z programu
}


void RenderScene() 
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Wyczyszczenie ekranu i bufora glebokosci
	glLoadIdentity();									// Restartowanie macierza
	
	// Wez wspolrzedne kamery
	CVector3 vPos		= g_Camera.Position();
	CVector3 vNewPos    = vPos;
	int strobj=0; // Objekt ktorym sterujemy
    char string[512]; //temp do napisow

/*	if (view==1)
	{
		// Sprawdzanie czy kamera jest nad powiezchnia terenu i uniemozliwienie zapadniecia sie pod teren
		//if(vPos.y < Height(g_HeightMap, (int)vPos.x, (int)vPos.z ) + 10)
		{
			//vNewPos.y = (float)Height(g_HeightMap, (int)vPos.x, (int)vPos.z ) + 10;
			if(vPos.y+0.9 < Height(g_HeightMap, (int)vPos.x, (int)vPos.z ) + 10)
			{   if(Height(g_HeightMap, (int)vPos.x, (int)vPos.z )+10-vPos.y+0.9>5.0)
					vNewPos.y=(float)vNewPos.y+5.0; 
				else 
					vNewPos.y=(float)vNewPos.y+0.9;
			}else{
			if(vPos.y-0.9 > Height(g_HeightMap, (int)vPos.x, (int)vPos.z ) + 10)
				vNewPos.y=(float)vNewPos.y-0.9;
			}
			// Get the difference of the y that the camera was pushed back up
			float temp = vNewPos.y - vPos.y;
			//  Get the current view and increase it by the different the position was moved
			CVector3 vView = g_Camera.View();
			//vView.y += temp;
			// Ustaw pozycje kamery

				g_Camera.PositionCamera(vNewPos.x,  vNewPos.y,  vNewPos.z,
			 					vView.x,	vView.y,	vView.z,	0, 1, 0);								
//				g_Camera.PositionCamera(Models.getSceneMX(strobj), Models.getSceneMY(strobj)+20, Models.getSceneMZ(strobj),
//							vView.x, vView.y, vView.z,	0, 1, 0);	

		}
/*	}else
	{
		float bx,by,bz,alph;
		alph =  ((Models.BaseOfRctVrtx[strobj].bndbx[0].y+Models.BaseOfRctVrtx[strobj].bndbx[3].y)/2)-
				((Models.BaseOfRctVrtx[strobj].bndbx[1].y+Models.BaseOfRctVrtx[strobj].bndbx[2].y)/ 2);
			bx=Models.GetX(strobj)+1*20*-1*sin(Models.BaseOfRctVrtx[strobj].angle*3.1415/180);
			by=Models.GetY(strobj)+20+1*20*-1*sin((alph)*3.1415/180);
			bz=Models.GetZ(strobj)+1*20*-1*cos(Models.BaseOfRctVrtx[strobj].angle*3.1415/180);
									
	}
*/	
	
						if (view==true)
						{   g_Camera.SetPos(Models.getSceneMX(strobj), Models.getSceneMY(strobj)+30, Models.getSceneMZ(strobj),Models.getSAngle(strobj)*3.1415/180); 
							//g_Camera.PositionCamera(Models.getSceneMX(strobj), Models.getSceneMY(strobj)+20, Models.getSceneMZ(strobj),
 	 						//	vView.x, vView.y, vView.z,	0, 1, 0);	
						}
	
	g_Camera.Look();

	// Rysowanie mapy
	glPushMatrix();
	glScalef(SCALEXY, 1.0, SCALEXY);
	RenderHeightMap(g_HeightMap);						
	glLoadIdentity();
	glPopMatrix();
	// Utworzenie SkyBox
	CreateSkyBox(Models.getSceneMX(strobj), Models.getSceneMY(strobj), Models.getSceneMZ(strobj), 5000, 5000, 5000 );

	//Ustawienie wysokosci "kol"
	/*for (int obj=0;obj<100;obj++)
	{
		if (!Models.BaseOfRctVrtx[obj].in_use)
			break;
		for (int kk=0;kk<4;kk++)
		{
//			if (Models.BaseOfRctVrtx[obj].speed<0) Models.BaseOfRctVrtx[0].speed*=-1;

			float rozn =  Models.BaseOfRctVrtx[obj].bndbx[kk].y-(float)Height(g_HeightMap, floor(Models.GetX(obj)+Models.BaseOfRctVrtx[obj].bndbx[kk].x), floor(Models.GetZ(obj)+Models.BaseOfRctVrtx[obj].bndbx[kk].z) );
			if (rozn+0.1>0)   Models.BaseOfRctVrtx[obj].bndbx[kk].y-=0.1;//+Models.BaseOfRctVrtx[obj].speed*0.07;
			if (rozn>5)   Models.BaseOfRctVrtx[obj].bndbx[kk].y-=0.1+Models.BaseOfRctVrtx[obj].speed*0.55;
			if (rozn-0.1<0)   Models.BaseOfRctVrtx[obj].bndbx[kk].y+=0.1;//+Models.BaseOfRctVrtx[obj].speed*0.07;
			if (rozn<-5)  Models.BaseOfRctVrtx[obj].bndbx[kk].y+=0.1+Models.BaseOfRctVrtx[obj].speed*0.55;
	        //Models.BaseOfRctVrtx[obj].bndbx[kk].y= Height(g_HeightMap, Models.GetX(obj)+Models.BaseOfRctVrtx[obj].bndbx[kk].x, Models.GetZ(obj)+Models.BaseOfRctVrtx[obj].bndbx[kk].z ) + 3;
			//Models.BaseOfRctVrtx[obj].bndbx[kk].y= 150;
		}
	}*/

	//grawitacja
	for (int obj=0;obj<Models.ScenesCount();obj++)
	{
		if (!Models.InUse(obj))
			break;
		//Models.Grawitacja(obj, 0.4,(float)Height(g_HeightMap, (int)Models.getSceneMX(obj), (int)Models.getSceneMZ(obj) )+2);
		Models.Grawitacja(obj, 0.4,(float)Heightf(g_HeightMap, Models.getSceneMX(obj), Models.getSceneMZ(obj) )+2);
		//	float wylx,wyly,wylz;
		//    wylx=Models.GetX(obj); wyly=Height(g_HeightMap, (int)Models.GetX(obj), (int)Models.GetZ(obj) ); wylz=Models.GetZ(obj);
		//	Models.ZmienXYZ(obj, wylx, floor(wyly),wylz);
	}

	//Models.DrawModels(g_Texture);
	//Models.ObrocBB(0,5.0);

	//Napisy
	sprintf((char*)string,"%f km/h",Models.getSSpeed(strobj)*17);
	glPrint(2,20,string,0);
	glPrint(2,2,"PKS",0);

	if(GetKeyState('I') & 0x80) {			
		if (Models.getSSpeed(strobj)>0)
			Models.setSSpeed(strobj,Models.getSSpeed(strobj)+0.004);
		else
			Models.setSSpeed(strobj,Models.getSSpeed(strobj)+0.01);
	}
	if(GetKeyState('K') & 0x80) {	
		if (Models.getSSpeed(strobj)>0)
		   Models.setSSpeed(strobj,Models.getSSpeed(strobj)-0.01);
		else
		   Models.setSSpeed(strobj,Models.getSSpeed(strobj)-0.004);
	}

	if (Models.getSSpeed(strobj)>0.01 || Models.getSSpeed(strobj) <-0.02)
	{	if(GetKeyState('J') & 0x80) {			
			if (wspc<1) wspc+=0.015;
		}
		if(GetKeyState('L')  & 0x80) {			
			if (wspc>-1) wspc-=0.015;
		}
	}

	if (wspc>0) wspc-=0.005;  
	if (wspc<0) wspc+=0.005;  
 	Models.setSAngle(strobj,Models.getSAngle(strobj)+wspc);


	/*Models.ZmienXYZ(strobj,Models.GetX(strobj)+1*Models.BaseOfRctVrtx[strobj].speed*-1*sin(Models.BaseOfRctVrtx[strobj].angle*3.1415/180),
		Models.GetY(strobj),
		Models.GetZ(strobj)+1*Models.BaseOfRctVrtx[strobj].speed*-1*cos(Models.BaseOfRctVrtx[strobj].angle*3.1415/180));
	*/
	Models.setSceneMX(strobj,Models.getSceneMX(strobj)+1*Models.getSSpeed(strobj)*-1*sin(Models.getSAngle(strobj)*3.1415/180));
	Models.setSceneMZ(strobj,Models.getSceneMZ(strobj)+1*Models.getSSpeed(strobj)*-1*cos(Models.getSAngle(strobj)*3.1415/180));

	if (Models.getSSpeed(strobj)>0.002) {
	    Models.setSSpeed(strobj,Models.getSSpeed(strobj)-(0.0002+(Models.getSSpeed(strobj)*0.0009)));
	}else
	{
		if (Models.getSSpeed(strobj)<-0.002){
			Models.setSSpeed(strobj,Models.getSSpeed(strobj)+0.0005-(Models.getSSpeed(strobj)*0.0020));
		}else
		{
			Models.setSSpeed(strobj,0);
		}
	}

	if(GetKeyState('C') & 0x80) {			
       view=false;
	}

	if(GetKeyState('V') & 0x80) {			
       view=true;
	}


	Models.DrawASEModels(g_Texture);
	SwapBuffers(g_hDC);		
}

LRESULT CALLBACK WinProc(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT    ps;

    switch (uMsg)
	{ 
    case WM_SIZE:										
		if(!g_bFullScreen)								
		{
			SizeOpenGLScreen(LOWORD(lParam),HIWORD(lParam));
			GetClientRect(hWnd, &g_rRect);				
		}
        break; 
 
	case WM_PAINT:										
		BeginPaint(hWnd, &ps);									
		EndPaint(hWnd, &ps);							
		break;


	case WM_LBUTTONDOWN:
		g_bDetail = !g_bDetail;
		break;

	case WM_RBUTTONDOWN:								
		
		g_bRenderMode = !g_bRenderMode;

		//Zmiana trybu renderowania
		if(g_bRenderMode) {	
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	
		}else{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	
		}
		break;

	case WM_KEYDOWN:
		switch(wParam) 
		{
			case VK_ESCAPE:								
				PostQuitMessage(0);						
				break;

			case VK_SPACE:								
				g_DetailScale = (g_DetailScale * 2) % 128;
				if(g_DetailScale == 0)
					g_DetailScale = 1;

				break;
		}
		break;
 
    case WM_CLOSE:	
        PostQuitMessage(0);
        break; 
    } 
    return DefWindowProc (hWnd, uMsg, wParam, lParam); 
}
