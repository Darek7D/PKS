#pragma once

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <gl\gl.h>									
#include <gl\glu.h>							
#include <gl\glaux.h>	
#include <math.h>
#include <mbstring.h>

#define MAX_SCENES 20
#define MAX_GEOMOBJECT 50
#define MAX_MESH_VERTEX 4300
#define MAX_MESH_FACE_LIST 8600

class models
{
public:
	models(void);
	~models(void);
	void Grawitacja(int obj, float G, float Y);
	void CalculateBB(int obj);
	void ObrocBB(int obj, float angle);
	void SetBndbx(int obj, float ile, int rog);
	float GetBndbx(int obj, int rog);

	struct Bndbx
    {
		float x, y, z;
	};
	struct Vertex
    {
		float u, v, x, y, z;
    };
	struct Texture
	{
		int nr;
		int opt;
		int adt;
	};
	struct Model
	{
		bool in_use;
		float angle;
		float speed;
		Vertex vrtx[100];
		Bndbx bndbx[4];
    	Texture textures[100];
		int g_NumberOfVerts;
	};
	Model BaseOfRctVrtx[100];

    /*ASE models poczatek*/
	void LoadASEModels(void);
	int LoadASEModel(int nr, char nazwa[32]);
	void DrawASEModels(UINT g_Texture[100]);
	void setModelVX(int scene, int obj, int vertex, float value);
	void setModelVY(int scene, int obj, int vertex, float value);
	void setModelVZ(int scene, int obj, int vertex, float value);
	void setSceneMX(int scene, float value);
	void setSceneMY(int scene, float value);
	void setSceneMZ(int scene, float value);
	void setSSpeed(int scene, float value);
	void setSAngle(int scene, float value);
	float getSSpeed(int scene);
	float getSceneMX(int scene);
	float getSceneMY(int scene);
	float getSceneMZ(int scene);
	float getSAngle(int scene);
	int ScenesCount(void);
	bool InUse(int scene);

	struct MESH_VERTEX
	{
		float x,y,z;
	};

	struct MESH_FACE
	{
		int A;
		int B;
		int C;
		int AB;
		int BC;
		int CA;
	};

	struct MESH_TFACE
	{
		int A;
		int B;
		int C;
	};

	struct MESH
	{
		int MESH_NUMVERTEX;
		int MESH_NUMFACES;
		int MESH_NUMTVERTEX;
		int MESH_NUMTVFACES;
		int MATERIAL_REF;
		MESH_VERTEX mesh_vertex_list[MAX_MESH_VERTEX];
		MESH_VERTEX mesh_normal_list[MAX_MESH_FACE_LIST];
		MESH_FACE mesh_face_list[MAX_MESH_FACE_LIST];
		MESH_TFACE mesh_tface_list[MAX_MESH_FACE_LIST];
		MESH_VERTEX mesh_tvertex_list[MAX_MESH_VERTEX*3];
	};

	struct GEOMOBJECT
	{
		char NODE_NAME[10];
		MESH_VERTEX position;
		float angle;
        MESH mesh;
	};

	struct MATERIAL
	{
		char name[32];
		int our_index;
	};

	struct SCENE
	{
		bool in_use;
		int objects;
		float speed;
		float angle;
		int MATERIAL_COUNT;
		MESH_VERTEX position;
		MATERIAL material[20];
		GEOMOBJECT geomobjects[MAX_GEOMOBJECT];
	};

  private:
	SCENE *scenes;
	int scenes_count;
	/*ASE models koniec*/
};



