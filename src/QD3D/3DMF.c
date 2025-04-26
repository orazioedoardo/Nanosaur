/****************************/
/*   	3DMF.C 		   	    */
/* (c)1996 Pangea Software  */
/* By Brian Greenstone      */
/****************************/


/****************************/
/*    EXTERNALS             */
/****************************/

#include "game.h"


/****************************/
/*    PROTOTYPES            */
/****************************/


/****************************/
/*    CONSTANTS             */
/****************************/


/*********************/
/*    VARIABLES      */
/*********************/

TQ3MetaFile*				gObjectGroupFile[MAX_3DMF_GROUPS];
GLuint*						gObjectGroupTextures[MAX_3DMF_GROUPS];
TQ3TriMeshFlatGroup			gObjectGroupList[MAX_3DMF_GROUPS][MAX_OBJECTS_IN_GROUP];
float			gObjectGroupRadiusList[MAX_3DMF_GROUPS][MAX_OBJECTS_IN_GROUP];
TQ3BoundingBox 	gObjectGroupBBoxList[MAX_3DMF_GROUPS][MAX_OBJECTS_IN_GROUP];
short			gNumObjectsInGroupList[MAX_3DMF_GROUPS] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


/******************* INIT 3DMF MANAGER *************************/

void Init3DMFManager(void)
{
short	i;

	for (i=0; i < MAX_3DMF_GROUPS; i++)
	{
		gObjectGroupFile[i] = nil;
		gObjectGroupTextures[i] = nil;
		gNumObjectsInGroupList[i] = 0;
	}
}

/******************** LOAD GROUPED 3DMF ***********************/
//
// Loads and registers a grouped 3DMF file by reading each element out of the file into
// a master object list.
//
// INPUT: fileName = name of file to load (nil = select one)
//		  groupNum = group # to assign this file.


void LoadGrouped3DMF(FSSpec *spec, Byte groupNum)
{
	GAME_ASSERT(groupNum < MAX_3DMF_GROUPS);

			/* DISPOSE OF ANY OLD OBJECTS */

	Free3DMFGroup(groupNum);

			/* LOAD NEW GEOMETRY */

	TQ3MetaFile* the3DMFFile = Q3MetaFile_Load3DMF(spec);
	GAME_ASSERT(the3DMFFile);

	gObjectGroupFile[groupNum] = the3DMFFile;

			/* UPLOAD TEXTURES TO GPU */

	gObjectGroupTextures[groupNum] = (GLuint *) NewPtrClear(the3DMFFile->numTextures * sizeof(GLuint));

	Render_Load3DMFTextures(the3DMFFile, gObjectGroupTextures[groupNum]);

			/* BUILD OBJECT LIST */

	int nObjects = the3DMFFile->numTopLevelGroups;
	GAME_ASSERT(nObjects > 0);
	GAME_ASSERT(nObjects <= MAX_OBJECTS_IN_GROUP);

	for (int i = 0; i < nObjects; i++)
	{
		TQ3TriMeshFlatGroup meshList = the3DMFFile->topLevelGroups[i];
		gObjectGroupList[groupNum][i] = meshList;
		GAME_ASSERT(0 != meshList.numMeshes);
		GAME_ASSERT(nil != meshList.meshes);

		gObjectGroupRadiusList[groupNum][i] = QD3D_CalcObjectRadius(meshList.numMeshes, meshList.meshes);	// save radius of it
		QD3D_CalcObjectBoundingBox(meshList.numMeshes, meshList.meshes, &gObjectGroupBBoxList[groupNum][i]); // save bbox
	}

	gNumObjectsInGroupList[groupNum] = nObjects;					// set # objects.
}


/******************** DELETE 3DMF GROUP **************************/

void Free3DMFGroup(Byte groupNum)
{
			/* DISPOSE TEXTURES */

	if (gObjectGroupTextures[groupNum] != nil)
	{
		GAME_ASSERT(gObjectGroupFile[groupNum] != nil);
		glDeleteTextures(gObjectGroupFile[groupNum]->numTextures, gObjectGroupTextures[groupNum]);
		DisposePtr((Ptr) gObjectGroupTextures[groupNum]);
		gObjectGroupTextures[groupNum] = nil;
	}

			/* DISPOSE METAFILE */

	if (gObjectGroupFile[groupNum] != nil)
	{
		Q3MetaFile_Dispose(gObjectGroupFile[groupNum]);
		gObjectGroupFile[groupNum] = nil;
	}

	SDL_memset(gObjectGroupList[groupNum], 0, sizeof(gObjectGroupList[groupNum]));

	gNumObjectsInGroupList[groupNum] = 0;
}


/******************* DELETE ALL 3DMF GROUPS ************************/

void DeleteAll3DMFGroups(void)
{
	for (int i = 0; i < MAX_3DMF_GROUPS; i++)
		Free3DMFGroup(i);
}
