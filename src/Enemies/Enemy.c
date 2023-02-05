/****************************/
/*   	ENEMY.C  			*/
/* (c)1997 Pangea Software  */
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

short		gNumEnemies;
signed char	gNumEnemyOfKind[NUM_ENEMY_KINDS];


/*********************  INIT ENEMY MANAGER **********************/

void InitEnemyManager(void)
{
short	i;

	gNumEnemies = 0;

	for (i=0; i < NUM_ENEMY_KINDS; i++)
		gNumEnemyOfKind[i] = 0;
}


/********************** DELETE ENEMY **************************/

void DeleteEnemy(ObjNode *theEnemy)
{
	gNumEnemyOfKind[theEnemy->Kind]--;					// dec kind count
	if (gNumEnemyOfKind[theEnemy->Kind] < 0)
		gNumEnemyOfKind[theEnemy->Kind] = 0;

	gNumEnemies--;										// dec global count

	DeleteObject(theEnemy);								// nuke the obj
}


/******************** KILL ENEMY *************************/

static void KillEnemy(ObjNode *theEnemy)
{
static const int enemyPoints[]=
{
	1250,			// ENEMY_KIND_REX
	2500,			// ENEMY_KIND_PTERA
	1000,			// ENEMY_KIND_STEGO
	1500,			// ENEMY_KIND_TRICER
	725,			// ENEMY_KIND_SPITTER
	500,
	500,
	500,
	500
};
	
	AddToScore(enemyPoints[theEnemy->Kind]);	// get points
	
	QD3D_ExplodeGeometry(theEnemy, 570.0f, 0, EXPLODEGEOMETRY_DENOMINATOR, .4);
	
	DeleteEnemy(theEnemy);
	PlayEffect(EFFECT_ENEMYDIE);
}




/****************** DO ENEMY COLLISION DETECT ***************************/
//
// For use by non-skeleton enemies.
//
// OUTPUT: true = was deleted
//

Boolean DoEnemyCollisionDetect(ObjNode *theEnemy, uint32_t ctype)
{
ObjNode	*hitObj;


			/* AUTOMATICALLY HANDLE THE BORING STUFF */
			
	HandleCollisions(theEnemy, ctype);


			/******************************/
			/* SCAN FOR INTERESTING STUFF */
			/******************************/
			

	for (int i = 0; i < gNumCollisions; i++)
	{
		if (gCollisionList[i].type == COLLISION_TYPE_OBJ)
		{
			hitObj = gCollisionList[i].objectPtr;						// get ObjNode of this collision
			ctype = hitObj->CType;
			
			if (ctype & CTYPE_HURTENEMY)
			{
				if (EnemyGotHurt(theEnemy,hitObj,hitObj->Damage))		// handle hit (returns true if was deleted)
					return(true);
			}
				/* SEE IF HIT ME - HURT ME */
			else
			if (ctype & CTYPE_PLAYER)
			{
				PlayerGotHurt(hitObj,theEnemy->Damage, true, false);
			}
		}
	}
	
	return(false);
}



/******************* ENEMY GOT HURT *************************/
//
// INPUT:	theEnemy = node of theEnemy which was hit.
//			theHurter = node of hurter which did it.
//
// OUTPUT: true = was deleted
//

Boolean EnemyGotHurt(ObjNode *theEnemy, ObjNode *theHurter, float damage)
{
	(void) theHurter;


			/* LOSE HEALTH */
			
	theEnemy->Health -= damage;
	
	
			/* HANDLE DEATH OF ENEMY */
			
	if (theEnemy->Health <= 0)
	{
		KillEnemy(theEnemy);
		return(true);
	}
	
	
	
	return(false);
}



/*********************** UPDATE ENEMY ******************************/

void UpdateEnemy(ObjNode *theNode)
{

	UpdateObject(theNode);
}


/********************* FIND CLOSEST ENEMY *****************************/
//
// OUTPUT: nil if no enemies
//

ObjNode *FindClosestEnemy(TQ3Point3D *pt, float *dist)
{
ObjNode		*thisNodePtr,*best = nil;
float	d,minDist = 100000;

			
	thisNodePtr = gFirstNodePtr;
	
	do
	{
		if (thisNodePtr->Slot >= SLOT_OF_DUMB)					// see if reach end of usable list
			break;
	
		if (thisNodePtr->CType & CTYPE_ENEMY)
		{
			d = CalcQuickDistance(pt->x,pt->z,thisNodePtr->Coord.x, thisNodePtr->Coord.z);
			if (d < minDist)
			{
				minDist = d;
				best = thisNodePtr;
			}
		}	
		thisNodePtr = (ObjNode *)thisNodePtr->NextNode;		// next node
	}
	while (thisNodePtr != nil);

	if (dist)
		*dist = minDist;
	return(best);
}


/******************* MAKE ENEMY SKELETON *********************/
//
// This routine creates a non-character skeleton which is an enemy.
//
// INPUT:	itemPtr->parm[0] = skeleton type 0..n
//
// OUTPUT:	ObjNode or nil if err.
//

ObjNode *MakeEnemySkeleton(Byte skeletonType, float x, float z)
{
ObjNode	*newObj;
	
			/****************************/
			/* MAKE NEW SKELETON OBJECT */
			/****************************/

	gNewObjectDefinition.type 	= skeletonType;
	gNewObjectDefinition.animNum = 0;							// assume default anim is #0
	gNewObjectDefinition.coord.x = x;
	gNewObjectDefinition.coord.y = GetTerrainHeightAtCoord_Quick(x,z);
	gNewObjectDefinition.coord.z = z;
	gNewObjectDefinition.flags = 0;
	gNewObjectDefinition.slot = PLAYER_SLOT+10;
	gNewObjectDefinition.moveCall = nil;
	gNewObjectDefinition.rot = 0;
	gNewObjectDefinition.scale = 1.0;

	newObj = MakeNewSkeletonObject(&gNewObjectDefinition);
	if (newObj == nil)
		DoFatalAlert("MakeEnemySkeleton: MakeNewSkeletonObject failed!");
	
	
				/* SET DEFAULT COLLISION INFO */
				
	newObj->CType = CTYPE_ENEMY | CTYPE_HURTIFTOUCH;
	newObj->CBits = CBITS_ALLSOLID;
	
//	SIDE_BITS_LEFT | SIDE_BITS_RIGHT | SIDE_BITS_FRONT |	// not solid on bottom or top
//					 SIDE_BITS_BACK;
	return(newObj);
}


/********************* MOVE ENEMY **********************/
//
// INPUT: flightHeight = dist from origin to desired foot position
//
// OUTPUT: true = one ground
//

Boolean MoveEnemy(ObjNode *theNode, float flightHeight)
{
float	y;

	gCoord.y += gDelta.y*gFramesPerSecondFrac;					// move
	gCoord.x += gDelta.x*gFramesPerSecondFrac;
	gCoord.z += gDelta.z*gFramesPerSecondFrac;


			/* SEE IF ON GROUND */
			
	y = GetTerrainHeightAtCoord_Planar(gCoord.x, gCoord.z);		// get center Y
	if ((gCoord.y + flightHeight) < y)
	{
		gCoord.y = y - flightHeight;
		gDelta.y = 0;
		theNode->StatusBits |= STATUS_BIT_ONGROUND;
		return(true);
	}
	theNode->StatusBits &= ~STATUS_BIT_ONGROUND;
	return(false);
}














