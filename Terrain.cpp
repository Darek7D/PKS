#include "main.h"

// This tells us if we want detail texturing turned on or off
extern bool g_bDetail;

// This holds the current tile scale value for our detail texture
extern int g_DetailScale;

int Height(BYTE *pHeightMap, int X, int Y)
{
	// Make sure we don't go past our array size
	//int x = X % MAP_SIZE;					// Error check our x value
	//int y = Y % MAP_SIZE;					// Error check our y value

	//X=floor(X/SCALEXY);
	//Y=floor(Y/SCALEXY);

	if(!pHeightMap) return 0;				// Make sure our data is valid

	// Use the equation: index = (x + (y * arrayWidth) ) to find the current height
   if ((X<0) || (Y<0) || (X>MAP_SIZE) || (Y>MAP_SIZE)) return 0;
	return pHeightMap[X + (Y * MAP_SIZE)]*HEIGHT_RATIO;	// Index into our height array and return the height
}

float Heightf(BYTE *pHeightMap, float X, float Y)
{
	// Make sure we don't go past our array size
	//int x = X % MAP_SIZE;					// Error check our x value
	//int y = Y % MAP_SIZE;					// Error check our y value

	//char str[512]; sprintf(str,"X=%f Y=%f",X,Y);
	//MessageBox(NULL, str, "Error", MB_OK);


	if(!pHeightMap) return 0;				// Make sure our data is valid

	int x1,y1,x0,y0,h0,h1;
	float posx=X/SCALEXY;
	float posy=Y/SCALEXY;

	x0=floor(posx);
	y0=floor(posy);
	x1=ceil(posx);
	y1=ceil(posy);

    if ((x0<0) || (y0<0) || (x1>MAP_SIZE) || (y1>MAP_SIZE)) return 0;

	h0= pHeightMap[(int)x0 + ((int)y0 * MAP_SIZE)];
	h1= pHeightMap[x1 + (y1 * MAP_SIZE)];

	float h,l,lm;

	h=h1-h0;
	l=x1-x0;
	if (h0>h1)
	   lm=posx-x0;
	else
       lm=x1-posx;

	float outpa = (h*lm)/l;
	h=h1-h0;
	l=y1-y0;
	if (h0<h1)
    	lm=posy-y0;
	else
		lm=y1-posy;

	float outpb = (h*lm)/l;

	//char str[512]; sprintf(str,"h0=%d h1=%d lm=%f l=%f x0=%f x1=%f posx=%f X=%f outpa=%f",h0,h1,lm,l,x0,x1,posx,X, (h0+outpa));
	//MessageBox(NULL, str, "Error", MB_OK);

	return (h0+((outpa+outpb)/2))*SCALEXY;
}


void SetTextureCoord(float x, float z)
{
	// Find the (u, v) coordinate for the current vertex
	float u =  (float)x / (float)MAP_SIZE;
	float v = -(float)z / (float)MAP_SIZE;
	
	// Give OpenGL the current terrain texture coordinate for our height map
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB, u, v);

	// Give OpenGL the current detail texture coordinate for our height map
	glMultiTexCoord2fARB(GL_TEXTURE1_ARB, u, v);

}

void RenderHeightMap(BYTE pHeightMap[])
{
	int X = 0, Y = 0;						// Create some variables to walk the array with.
	int x, y, z;							// Create some variables for readability
	bool bSwitchSides = false;

	// Make sure our height data is valid
	if(!pHeightMap) return;		
	
	// Activate the first texture ID and bind the tree background to it
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, g_Texture[0]);

	// If we want detail texturing on, let's render the second texture
	if(g_bDetail)
	{
		// Activate the second texture ID and bind the fog texture to it
		glActiveTextureARB(GL_TEXTURE1_ARB);
		glEnable(GL_TEXTURE_2D);
		
		// Here we turn on the COMBINE properties and increase our RGB
		// gamma for the detail texture.  2 seems to work just right.
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
		glTexEnvi(GL_TEXTURE_ENV, GL_RGB_SCALE_ARB, 2);
		
		// Bind the detail texture
		glBindTexture(GL_TEXTURE_2D, g_Texture[1]);
	
		// Now we want to enter the texture matrix.  This will allow us
		// to change the tiling of the detail texture.
		glMatrixMode(GL_TEXTURE);

			// Reset the current matrix and apply our chosen scale value
			glLoadIdentity();
			glScalef((float)g_DetailScale, (float)g_DetailScale, 1);
			// Leave the texture matrix and set us back in the model view matrix
		glMatrixMode(GL_MODELVIEW);
	}

   // glScalef(SCALEXY, SCALEXY, SCALEXY);
	// We want to render triangle strips
	glBegin( GL_TRIANGLE_STRIP );			

	// Go through all of the rows of the height map
	for ( X = 0; X <= MAP_SIZE; X += STEP_SIZE )
	{
		// Check if we need to render the opposite way for this column
		if(bSwitchSides)
		{	
			// Render a column of the terrain, for this current X.
			// We start at MAP_SIZE and render down to 0.
			for ( Y = MAP_SIZE; Y >= 0; Y -= STEP_SIZE )
			{
				// Get the (X, Y, Z) value for the bottom left vertex		
				x = X;							
				y = Height(pHeightMap, X, Y );	
				z = Y;							

				// Set the current texture coordinate and render the vertex
				SetTextureCoord( (float)x, (float)z );
				glVertex3i(x, y, z);		

				// Get the (X, Y, Z) value for the bottom right vertex		
				x = X + STEP_SIZE; 
				y = Height(pHeightMap, X + STEP_SIZE, Y ); 
				z = Y;

				// Set the current texture coordinate and render the vertex
				SetTextureCoord( (float)x, (float)z );
				glVertex3i(x, y, z);			
			}
		}
		else
		{	
			// Render a column of the terrain, for this current X.
			// We start at 0 and render down up to MAP_SIZE.
			for ( Y = 0; Y <= MAP_SIZE; Y += STEP_SIZE )
			{
				// Get the (X, Y, Z) value for the bottom right vertex		
				x = X + STEP_SIZE; 
				y = Height(pHeightMap, X + STEP_SIZE, Y ); 
				z = Y;

				// Set the current texture coordinate and render the vertex
				SetTextureCoord( (float)x, (float)z );
				glVertex3i(x, y, z);

				// Get the (X, Y, Z) value for the bottom left vertex		
				x = X;							
				y = Height(pHeightMap, X, Y );	
				z = Y;							

				// Set the current texture coordinate and render the vertex
				SetTextureCoord( (float)x, (float)z );
				glVertex3i(x, y, z);		
			}
		}

		// Switch the direction the column renders to allow the fluid tri strips
		bSwitchSides = !bSwitchSides;
	}

	// Stop rendering triangle strips
	glEnd();

    // Now let's clean up our multitexturing so it doesn't effect anything else

	// Turn the second multitexture pass off
	glActiveTextureARB(GL_TEXTURE1_ARB);
    glDisable(GL_TEXTURE_2D);

	// Turn the first multitexture pass off
	glActiveTextureARB(GL_TEXTURE0_ARB);		
    glDisable(GL_TEXTURE_2D);
}

void LoadRawFile(LPSTR strName, int nSize, BYTE *pHeightMap)
{
	FILE *pFile = NULL;

	// Let's open the file in Read/Binary mode.
	pFile = fopen( strName, "rb" );

	// Check to see if we found the file and could open it
	if ( pFile == NULL )	
	{
		// Display our error message and stop the function
		MessageBox(NULL, "Can't find the height map!", "Error", MB_OK);
		return;
	}

	// Here we load the .raw file into our pHeightMap data array.
	// We are only reading in '1', and the size is the (width * height)
	fread( pHeightMap, 1, nSize, pFile );

	// After we read the data, it's a good idea to check if everything read fine.
	int result = ferror( pFile );

	// Check if we received an error.
	if (result)
	{
		MessageBox(NULL, "Can't get data!", "Error", MB_OK);
	}

	// Close the file.
	fclose(pFile);
}

