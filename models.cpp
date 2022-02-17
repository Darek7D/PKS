#include "models.h"

float i=0;

models::models(void)
{

}

models::~models(void)
{
}

/*!Grawitacja
G - "wspolczynnik przyciagania"
Y - pozycja ktorej wartosc ma byc wpolzedna wysokosci*/
void models::Grawitacja(int obj, float G, float Y)
{  
/*	if (BaseOfRctVrtx[obj].speed>0)  G=(float)0.1+G*BaseOfRctVrtx[obj].speed*2;
	else G=(float)0.1+G*BaseOfRctVrtx[obj].speed*2*-1;
*/
/*	if (BaseOfRctVrtx[obj].vrtx[0].y-G>Y) {
			BaseOfRctVrtx[obj].vrtx[0].y-=G;
		}else {
			if (BaseOfRctVrtx[obj].vrtx[0].y+G<Y)
			BaseOfRctVrtx[obj].vrtx[0].y+=G;
		}
*/
//BaseOfRctVrtx[obj].vrtx[0].y=((BaseOfRctVrtx[obj].bndbx[0].y+BaseOfRctVrtx[obj].bndbx[1].y+BaseOfRctVrtx[obj].bndbx[2].y+BaseOfRctVrtx[obj].bndbx[3].y)/4)+3;
scenes[obj].position.y=Y;
}


/*!Ustawienie rownolegle do podloza*/
void models::CalculateBB(int obj)
{	//przod tyl
	glRotatef((GLfloat)((BaseOfRctVrtx[obj].bndbx[0].y-BaseOfRctVrtx[obj].bndbx[1].y)*0.19), (GLfloat)(BaseOfRctVrtx[obj].bndbx[0].y), 0.0, 0.0);
	glRotatef((GLfloat)((BaseOfRctVrtx[obj].bndbx[1].y-BaseOfRctVrtx[obj].bndbx[0].y)*0.19), (GLfloat)(BaseOfRctVrtx[obj].bndbx[1].y*-1), 0.0, 0.0);
	glRotatef((GLfloat)((BaseOfRctVrtx[obj].bndbx[2].y-BaseOfRctVrtx[obj].bndbx[3].y)*0.19), (GLfloat)(BaseOfRctVrtx[obj].bndbx[2].y*-1), 0.0, 0.0);
	glRotatef((GLfloat)((BaseOfRctVrtx[obj].bndbx[3].y-BaseOfRctVrtx[obj].bndbx[2].y)*0.19), (GLfloat)(BaseOfRctVrtx[obj].bndbx[3].y), 0.0, 0.0);
	//lewo prawo
	glRotatef((BaseOfRctVrtx[obj].bndbx[0].y-BaseOfRctVrtx[obj].bndbx[3].y), 0.0, 0.0, BaseOfRctVrtx[obj].bndbx[0].y*-1);
	glRotatef((BaseOfRctVrtx[obj].bndbx[1].y-BaseOfRctVrtx[obj].bndbx[2].y), 0.0, 0.0, BaseOfRctVrtx[obj].bndbx[1].y*-1);
	glRotatef((BaseOfRctVrtx[obj].bndbx[2].y-BaseOfRctVrtx[obj].bndbx[1].y), 0.0, 0.0, BaseOfRctVrtx[obj].bndbx[2].y);
	glRotatef((BaseOfRctVrtx[obj].bndbx[3].y-BaseOfRctVrtx[obj].bndbx[0].y), 0.0, 0.0, BaseOfRctVrtx[obj].bndbx[3].y*-1);

}

/*!Odwrocenie punktow "rozmieszczenia kol"
  Nieuzywane
*/
void models::ObrocBB(int obj, float angle)
{
	float r=5;
	BaseOfRctVrtx[obj].bndbx[0].x=(float)(BaseOfRctVrtx[obj].bndbx[0].x+(r*sin(angle*3.1415/180)));
	BaseOfRctVrtx[obj].bndbx[0].z=(float)(BaseOfRctVrtx[obj].bndbx[0].z+(r*cos(angle*3.1415/180)));

	BaseOfRctVrtx[obj].bndbx[1].x=(float)(BaseOfRctVrtx[obj].bndbx[1].x+(r*sin(angle*3.1415/180)));
	BaseOfRctVrtx[obj].bndbx[1].z=(float)(BaseOfRctVrtx[obj].bndbx[1].z+(r*cos(angle*3.1415/180)));	

	BaseOfRctVrtx[obj].bndbx[2].x=(float)(BaseOfRctVrtx[obj].bndbx[2].x+(r*sin(angle*3.1415/180)));
	BaseOfRctVrtx[obj].bndbx[2].z=(float)(BaseOfRctVrtx[obj].bndbx[2].z+(r*cos(angle*3.1415/180)));

	BaseOfRctVrtx[obj].bndbx[3].x=(float)(BaseOfRctVrtx[obj].bndbx[3].x+(r*sin(angle*3.1415/180)));
	BaseOfRctVrtx[obj].bndbx[3].z=(float)(BaseOfRctVrtx[obj].bndbx[3].z+(r*cos(angle*3.1415/180)));
}

/*!Ustawienie wysokosci kola
   ktory objekt, jaka wartosc, ktory rog (0-lewy przedni, 1-lewy tylny itd.)*/
void models::SetBndbx(int obj, float ile, int rog)
{  BaseOfRctVrtx[obj].bndbx[rog].y=ile;
}

/*!Zwrocenie wysokosci kola*/
float models::GetBndbx(int obj, int rog)
{
   return BaseOfRctVrtx[obj].bndbx[rog].y;
}

/*#################################################
					ASE MODELS
#################################################*/

void models::LoadASEModels(void)
{
	scenes = new SCENE[3];
	scenes_count=3;
	//if (LoadASEModel(0,"box")) scenes[0].in_use=true;
	if (LoadASEModel(0,"autosan")) scenes[0].in_use=true;
	if (LoadASEModel(1,"drzewko")) scenes[1].in_use=true;
	if (LoadASEModel(2,"drzewko1")) scenes[2].in_use=true;

	/*	FILE *fps = fopen("data/models/lampa1/output.ASE", "w");
	for (int scs=0;scs<scenes_count ;scs++)
	{
		if (!scenes[scs].in_use)
			break;
	    fprintf(fps,"Scene\n");
		for (int obj=0;obj<scenes[scs].objects ;obj++)
		{
			fprintf(fps,"\tObject %d\n",obj);
			fprintf(fps,"\t\tMESH_NUMVERTEX %d\n",scenes[scs].geomobjects[obj].mesh.MESH_NUMVERTEX);
			fprintf(fps,"\t\tMESH_NUMFACES %d\n",scenes[scs].geomobjects[obj].mesh.MESH_NUMFACES);
			fprintf(fps,"\t\tMESH_NUMTVERTEX %d\n",scenes[scs].geomobjects[obj].mesh.MESH_NUMTVERTEX);
			fprintf(fps,"\t\tMESH_NUMTVFACES %d\n",scenes[scs].geomobjects[obj].mesh.MESH_NUMTVFACES);
			fprintf(fps,"\t\t*MESH_VERTEX_LIST {\n ");
			for (int nv=0;nv<scenes[scs].geomobjects[obj].mesh.MESH_NUMVERTEX;nv++)
			{
				fprintf(fps,"\t\t\t*MESH_VERTEX %d %f %f %f \n",
					nv,
					scenes[scs].geomobjects[obj].mesh.mesh_vertex_list[nv].x,
					scenes[scs].geomobjects[obj].mesh.mesh_vertex_list[nv].y,
					scenes[scs].geomobjects[obj].mesh.mesh_vertex_list[nv].z);
			}
			fprintf(fps,"\t\t}\n");
			fprintf(fps,"\t\t*MESH_FACE_LIST {\n ");
			for (int nf=0;nf<scenes[scs].geomobjects[obj].mesh.MESH_NUMFACES;nf++)
			{
			fprintf(fps,"\t\t\t*MESH_FACE %d: A: %d B: %d C: %d AB: %d BC: %d CA: %d \n",
				nf,
				scenes[scs].geomobjects[obj].mesh.mesh_face_list[nf].A,
				scenes[scs].geomobjects[obj].mesh.mesh_face_list[nf].B,
				scenes[scs].geomobjects[obj].mesh.mesh_face_list[nf].C,
				scenes[scs].geomobjects[obj].mesh.mesh_face_list[nf].AB,
				scenes[scs].geomobjects[obj].mesh.mesh_face_list[nf].BC,
				scenes[scs].geomobjects[obj].mesh.mesh_face_list[nf].CA);
			}
			fprintf(fps,"\t\t*MESH_TVERTLIST {\n ");
			for (int nv=0;nv<scenes[scs].geomobjects[obj].mesh.MESH_NUMTVERTEX;nv++)
			{
				fprintf(fps,"\t\t\t*MESH_TVERT %d %f %f %f \n",
					nv,
					scenes[scs].geomobjects[obj].mesh.mesh_tvertex_list[nv].x,
					scenes[scs].geomobjects[obj].mesh.mesh_tvertex_list[nv].y,
					scenes[scs].geomobjects[obj].mesh.mesh_tvertex_list[nv].z);
			}
			fprintf(fps,"\t\t}\n");
			fprintf(fps,"\t\t*MESH_TFACE_LIST {\n ");
			for (int nf=0;nf<scenes[scs].geomobjects[obj].mesh.MESH_NUMTVFACES;nf++)
			{
				fprintf(fps,"\t\t\t*MESH_TFACE %d: A: %d B: %d C: %d\n",
					nf,
					scenes[scs].geomobjects[obj].mesh.mesh_face_list[nf].A,
					scenes[scs].geomobjects[obj].mesh.mesh_face_list[nf].B,
					scenes[scs].geomobjects[obj].mesh.mesh_face_list[nf].C);
			}
			fprintf(fps,"\t\t}\n");
			fprintf(fps,"\t*MATERIAL_REF %d\n",scenes[scs].geomobjects[obj].mesh.MATERIAL_REF);
		}
	}
	fclose(fps);
*/
   return;
}

int models::LoadASEModel(int nr, char nazwa[32])
{
	char string[512];
	int n=0;
    char tmp[512];
	char tmp1[32];
	int itmp;
	int itmp1;
	sprintf((char*)string,"data/models/%s/%s.ASE",nazwa,nazwa);
	FILE *fp = fopen(string, "r");
	if(!fp)	{MessageBox(NULL, "Nie mozna otworzyc pliku", "Error", MB_OK); return 0;}
		scenes[nr].objects=0;
	    while(1)
		{
			strcpy(tmp,"");
			int result = fscanf(fp, "%s", &tmp);
			if((result == EOF) || (nr>=scenes_count)) break;

			if (!strcmp(tmp,"*MATERIAL_LIST"))
			{   itmp=0;
				fscanf(fp, "%s",&tmp);
				fscanf(fp, "%s",&tmp);	fscanf(fp, "%d",&scenes[nr].MATERIAL_COUNT);
				while(itmp<scenes[nr].MATERIAL_COUNT)
				{
					fscanf(fp, "%s",&tmp);
					if (!strcmp(tmp,"*MATERIAL_NAME"))
					{
						fscanf(fp, "%s",&tmp);
						fscanf(fp, "%d",&scenes[nr].material[itmp].our_index);
						itmp++;
					}					
				}
			}

			if (!strcmp(tmp,"*GEOMOBJECT"))
			{
				scenes[nr].objects++; 
			}

			if (!strcmp(tmp,"*NODE_NAME"))
			{
       		    fscanf(fp, "%s", scenes[nr].geomobjects[scenes[nr].objects-1].NODE_NAME);
			}

			if (!strcmp(tmp,"*MESH_NUMVERTEX"))
			{		
		        fscanf(fp, "%d", &scenes[nr].geomobjects[scenes[nr].objects-1].mesh.MESH_NUMVERTEX);
			}

			if (!strcmp(tmp,"*MESH_NUMFACES"))
			{
				fscanf(fp, "%d", &scenes[nr].geomobjects[scenes[nr].objects-1].mesh.MESH_NUMFACES);
			}

			if (!strcmp(tmp,"*MATERIAL_REF"))
			{
				fscanf(fp, "%d", &scenes[nr].geomobjects[scenes[nr].objects-1].mesh.MATERIAL_REF);
			}

			if (!strcmp(tmp,"*MESH_NUMTVERTEX"))
			{
				fscanf(fp, "%d", &scenes[nr].geomobjects[scenes[nr].objects-1].mesh.MESH_NUMTVERTEX);
			}

			if (!strcmp(tmp,"*MESH_NUMTVFACES"))
			{
				fscanf(fp, "%d", &scenes[nr].geomobjects[scenes[nr].objects-1].mesh.MESH_NUMTVFACES);
			}

			if (!strcmp(tmp,"*MESH_NORMALS"))
			{


			}

			if (!strcmp(tmp,"*MESH_VERTEX_LIST"))
			{
				fscanf(fp, "%s",&tmp);
				for (int i=0; i<scenes[nr].geomobjects[scenes[nr].objects-1].mesh.MESH_NUMVERTEX; i++)
				{
					fscanf(fp, "%s",&tmp);
                    fscanf(fp, "%d",&itmp);
					fscanf(fp, "%f", &scenes[nr].geomobjects[scenes[nr].objects-1].mesh.mesh_vertex_list[i].x);
					fscanf(fp, "%f", &scenes[nr].geomobjects[scenes[nr].objects-1].mesh.mesh_vertex_list[i].y);
					fscanf(fp, "%f", &scenes[nr].geomobjects[scenes[nr].objects-1].mesh.mesh_vertex_list[i].z);
				}
			}

			if (!strcmp(tmp,"*MESH_FACE_LIST"))
			{   fscanf(fp, "%s",&tmp);
				for (int i=0; i<scenes[nr].geomobjects[scenes[nr].objects-1].mesh.MESH_NUMFACES; i++)
				{   //*MESH_FACE    0:    A:    0 B:    2 C:    3 AB:    1 BC:    1 CA:    0	 *MESH_SMOOTHING 2 	*MESH_MTLID 1
					fscanf(fp, "%s",&tmp);
					fscanf(fp, "%s",&tmp);
					fscanf(fp, "%s",&tmp); fscanf(fp, "%d", &scenes[nr].geomobjects[scenes[nr].objects-1].mesh.mesh_face_list[i].A);
					fscanf(fp, "%s",&tmp); fscanf(fp, "%d", &scenes[nr].geomobjects[scenes[nr].objects-1].mesh.mesh_face_list[i].B);
					fscanf(fp, "%s",&tmp); fscanf(fp, "%d", &scenes[nr].geomobjects[scenes[nr].objects-1].mesh.mesh_face_list[i].C);
					fscanf(fp, "%s",&tmp); fscanf(fp, "%d", &scenes[nr].geomobjects[scenes[nr].objects-1].mesh.mesh_face_list[i].AB);
					fscanf(fp, "%s",&tmp); fscanf(fp, "%d", &scenes[nr].geomobjects[scenes[nr].objects-1].mesh.mesh_face_list[i].BC);
					fscanf(fp, "%s",&tmp); fscanf(fp, "%d", &scenes[nr].geomobjects[scenes[nr].objects-1].mesh.mesh_face_list[i].CA);
					fscanf(fp, "%s",&tmp);	
					fscanf(fp, "%s",&tmp);	
					fscanf(fp, "%s",&tmp);  
					fscanf(fp, "%s",&tmp); 	
				}
			}

			if (!strcmp(tmp,"*MESH_TVERTLIST"))
			{
				fscanf(fp, "%s",&tmp);
				for (int i=0; i<scenes[nr].geomobjects[scenes[nr].objects-1].mesh.MESH_NUMTVERTEX; i++)
				{
					fscanf(fp, "%s",&tmp);
                    fscanf(fp, "%d",&itmp);
					fscanf(fp, "%f", &scenes[nr].geomobjects[scenes[nr].objects-1].mesh.mesh_tvertex_list[i].x);
					fscanf(fp, "%f", &scenes[nr].geomobjects[scenes[nr].objects-1].mesh.mesh_tvertex_list[i].y);
					fscanf(fp, "%f", &scenes[nr].geomobjects[scenes[nr].objects-1].mesh.mesh_tvertex_list[i].z);
				}
			}

			if (!strcmp(tmp,"*MESH_TFACELIST"))
			{   fscanf(fp, "%s",&tmp);
				for (int i=0; i<scenes[nr].geomobjects[scenes[nr].objects-1].mesh.MESH_NUMTVFACES; i++)
				{   //*MESH_FACE    0:    A:    0 B:    2 C:    3 AB:    1 BC:    1 CA:    0	 *MESH_SMOOTHING 2 	*MESH_MTLID 1
					fscanf(fp, "%s",&tmp);
					fscanf(fp, "%s",&tmp);
					fscanf(fp, "%d", &scenes[nr].geomobjects[scenes[nr].objects-1].mesh.mesh_tface_list[i].A);
					fscanf(fp, "%d", &scenes[nr].geomobjects[scenes[nr].objects-1].mesh.mesh_tface_list[i].B);
					fscanf(fp, "%d", &scenes[nr].geomobjects[scenes[nr].objects-1].mesh.mesh_tface_list[i].C);
				}
			}

		}
	fclose(fp);
	scenes[nr].angle=0;
	scenes[nr].speed=0;
	scenes[nr].position.x=0;
	scenes[nr].position.y=0;
	scenes[nr].position.z=0;
	return 1;
}

void models::DrawASEModels(UINT g_Texture[100])
{
	int coordIndex;
	glDisable(GL_CULL_FACE);
	for (int scs=0;scs<scenes_count ;scs++)
	{
		glPushMatrix();
		int jk=0;
		if ((!scenes[scs].in_use) || (scs >= scenes_count))
			break;

		glTranslatef(getSceneMX(scs),getSceneMY(scs),getSceneMZ(scs));
		glRotatef(getSAngle(scs), 0.0, 1.0, 0.0);
		for(int kt_obj=0;kt_obj<scenes[scs].objects;kt_obj++)
		{
			//glRotatef(scenes[scs].geomobjects[kt_obj].angle, 0.0, 1.0, 0.0);
			//glBindTexture(GL_TEXTURE_2D, g_Texture[scenes[scs].geomobjects[kt_obj].mesh.MATERIAL_REF]);
			glBindTexture(GL_TEXTURE_2D, g_Texture[   scenes[scs].material[scenes[scs].geomobjects[kt_obj].mesh.MATERIAL_REF].our_index  ]);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);
			for (int kt_fcs=0; kt_fcs<scenes[scs].geomobjects[kt_obj].mesh.MESH_NUMFACES; kt_fcs++)
			{
				glBegin(GL_TRIANGLES);
				glNormal3f(scenes[scs].geomobjects[kt_obj].mesh.mesh_normal_list[kt_fcs].x,
					scenes[scs].geomobjects[kt_obj].mesh.mesh_normal_list[kt_fcs].y,
					scenes[scs].geomobjects[kt_obj].mesh.mesh_normal_list[kt_fcs].z);

				    coordIndex = scenes[scs].geomobjects[kt_obj].mesh.mesh_tface_list[kt_fcs].A;
				    glTexCoord2f(scenes[scs].geomobjects[kt_obj].mesh.mesh_tvertex_list[ coordIndex ].x,
					         scenes[scs].geomobjects[kt_obj].mesh.mesh_tvertex_list[ coordIndex ].y);
					glVertex3f(scenes[scs].geomobjects[kt_obj].mesh.mesh_vertex_list[scenes[scs].geomobjects[kt_obj].mesh.mesh_face_list[kt_fcs].A].x,
						scenes[scs].geomobjects[kt_obj].mesh.mesh_vertex_list[scenes[scs].geomobjects[kt_obj].mesh.mesh_face_list[kt_fcs].A].z,
						scenes[scs].geomobjects[kt_obj].mesh.mesh_vertex_list[scenes[scs].geomobjects[kt_obj].mesh.mesh_face_list[kt_fcs].A].y);

					coordIndex = scenes[scs].geomobjects[kt_obj].mesh.mesh_tface_list[kt_fcs].B;
					glTexCoord2f(scenes[scs].geomobjects[kt_obj].mesh.mesh_tvertex_list[ coordIndex ].x,
						         scenes[scs].geomobjects[kt_obj].mesh.mesh_tvertex_list[ coordIndex ].y);
					glVertex3f(scenes[scs].geomobjects[kt_obj].mesh.mesh_vertex_list[scenes[scs].geomobjects[kt_obj].mesh.mesh_face_list[kt_fcs].B].x,
						scenes[scs].geomobjects[kt_obj].mesh.mesh_vertex_list[scenes[scs].geomobjects[kt_obj].mesh.mesh_face_list[kt_fcs].B].z,
						scenes[scs].geomobjects[kt_obj].mesh.mesh_vertex_list[scenes[scs].geomobjects[kt_obj].mesh.mesh_face_list[kt_fcs].B].y);

					coordIndex = scenes[scs].geomobjects[kt_obj].mesh.mesh_tface_list[kt_fcs].C;
					glTexCoord2f(scenes[scs].geomobjects[kt_obj].mesh.mesh_tvertex_list[ coordIndex ].x,
					         scenes[scs].geomobjects[kt_obj].mesh.mesh_tvertex_list[ coordIndex ].y);
					glVertex3f(scenes[scs].geomobjects[kt_obj].mesh.mesh_vertex_list[scenes[scs].geomobjects[kt_obj].mesh.mesh_face_list[kt_fcs].C].x,
						scenes[scs].geomobjects[kt_obj].mesh.mesh_vertex_list[scenes[scs].geomobjects[kt_obj].mesh.mesh_face_list[kt_fcs].C].z,
						scenes[scs].geomobjects[kt_obj].mesh.mesh_vertex_list[scenes[scs].geomobjects[kt_obj].mesh.mesh_face_list[kt_fcs].C].y);
				glEnd();
			}
		}

		glLoadIdentity();
		glPopMatrix();
	}  
	glEnable(GL_CULL_FACE);								// Niewyswietlanie tylnej strony tekstur
	return;
}

void models::setModelVX(int scene, int obj, int vertex, float value)
{
    scenes[scene].geomobjects[obj].mesh.mesh_vertex_list[vertex].x=value;
}

void models::setModelVY(int scene, int obj, int vertex, float value)
{
    scenes[scene].geomobjects[obj].mesh.mesh_vertex_list[vertex].y=value;
}

void models::setModelVZ(int scene, int obj, int vertex, float value)
{
    scenes[scene].geomobjects[obj].mesh.mesh_vertex_list[vertex].z=value;
}

void models::setSceneMX(int scene, float value)
{
    scenes[scene].position.x=value;
}

void models::setSceneMY(int scene, float value)
{
    scenes[scene].position.y=value;
}

void models::setSceneMZ(int scene, float value)
{
    scenes[scene].position.z=value;
}

void models::setSSpeed(int scene, float value)
{
	scenes[scene].speed=value;
}

void models::setSAngle(int scene, float value)
{
	scenes[scene].angle=value;
}

float models::getSSpeed(int scene)
{
	return scenes[scene].speed;
}

float models::getSceneMX(int scene)
{
    return scenes[scene].position.x;
}

float models::getSceneMY(int scene)
{
    return scenes[scene].position.y;
}

float models::getSceneMZ(int scene)
{
    return scenes[scene].position.z;
}

float models::getSAngle(int scene)
{
	return scenes[scene].angle;
}

bool models::InUse(int scene)
{
	return scenes[scene].in_use;
}

int models::ScenesCount(void)
{
   return scenes_count;
}