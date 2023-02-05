/****************************/
/*   	SKELETON2.C    	    */
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





/******************** UPDATE JOINT TRANSFORMS ****************************/
//
// Updates ALL of the transforms in a joint's transform group based on the theNode->Skeleton->JointCurrentPosition 
//
// INPUT:	jointNum = joint # to rotate
//

void UpdateJointTransforms(SkeletonObjDataType *skeleton, int jointNum)
{
TQ3Matrix4x4			matrix1;
static TQ3Matrix4x4		matrix2 = {{ {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,1} }};
TQ3Matrix4x4			*destMatPtr;
JointKeyframeType		*kfPtr;

	destMatPtr = &skeleton->jointTransformMatrix[jointNum];					// get ptr to joint's xform matrix

	kfPtr = &skeleton->JointCurrentPosition[jointNum];													// get ptr to keyframe

	if ((kfPtr->scale.x != 1.0f) || (kfPtr->scale.y != 1.0f) || (kfPtr->scale.z != 1.0f))				// SEE IF CAN IGNORE SCALE
	{
						/* ROTATE IT */
				
		Q3Matrix4x4_SetRotate_XYZ(&matrix1, kfPtr->rotation.x, kfPtr->rotation.y, kfPtr->rotation.z);	// set matrix for x/y/z rot


					/* SCALE & TRANSLATE */
	
		matrix2.value[0][0] = kfPtr->scale.x;									
												matrix2.value[1][1] = kfPtr->scale.y;											
																						matrix2.value[2][2] = kfPtr->scale.z;
		matrix2.value[3][0] = kfPtr->coord.x;	matrix2.value[3][1] = kfPtr->coord.y;	matrix2.value[3][2] = kfPtr->coord.z;
		
		Q3Matrix4x4_Multiply(&matrix1,&matrix2,destMatPtr);		
	}
	else
	{
						/* ROTATE IT */
				
		Q3Matrix4x4_SetRotate_XYZ(destMatPtr, kfPtr->rotation.x, kfPtr->rotation.y, kfPtr->rotation.z);	// set matrix for x/y/z rot
	
						/* NOW TRANSLATE IT */
	
		destMatPtr->value[3][0] =  kfPtr->coord.x;
		destMatPtr->value[3][1] =  kfPtr->coord.y;
		destMatPtr->value[3][2] =  kfPtr->coord.z;
	}
													
}


/*************** FIND COORD OF JOINT *****************/
//
// Returns the 3-space coord of the given joint.
//

void FindCoordOfJoint(ObjNode *theNode, int jointNum, TQ3Point3D *outPoint)
{
TQ3Matrix4x4	matrix;
static TQ3Point3D		point3D = {0,0,0};				// use joint's origin @ 0,0,0

	FindJointFullMatrix(theNode,jointNum,&matrix);		// calc matrix
	Q3Point3D_Transform(&point3D, &matrix, outPoint);	// apply matrix to origin @ 0,0,0 to get new 3-space coords
}


/*************** FIND COORD ON JOINT *****************/
//
// Returns the 3-space coord of a point on the given joint.
//
// Essentially the same as FindCoordOfJoint except that rather than
// using 0,0,0 as the applied point, it takes the input point which
// is an offset from the origin of the joint.  Thus, the returned point
// is "on" the joint rather than the exact coord of the hinge.
//

void FindCoordOnJoint(ObjNode *theNode, int jointNum, const TQ3Point3D *inPoint, TQ3Point3D *outPoint)
{
TQ3Matrix4x4	matrix;

	FindJointFullMatrix(theNode,jointNum,&matrix);		// calc matrix
	Q3Point3D_Transform(inPoint, &matrix, outPoint);	// apply matrix to origin @ 0,0,0 to get new 3-space coords
}



/************* FIND JOINT FULL MATRIX ****************/
//
// Returns an accumulated matrix for a joint's coordinates.
//

void FindJointFullMatrix(ObjNode *theNode, int jointNum, TQ3Matrix4x4 *outMatrix)
{
const SkeletonDefType *skeletonDefPtr;
SkeletonObjDataType	*skeletonPtr;
BoneDefinitionType	*bonePtr;

			/* ACCUMULATE A MATRIX DOWN THE CHAIN */
			
	*outMatrix = theNode->Skeleton->jointTransformMatrix[jointNum];		// init matrix

	skeletonPtr =  theNode->Skeleton;									// point to skeleton
	skeletonDefPtr = skeletonPtr->skeletonDefinition;					// point to skeleton defintion

	GAME_ASSERT(jointNum >= 0 && jointNum < skeletonDefPtr->NumBones);	// check for illegal joints

	bonePtr = skeletonDefPtr->Bones;									// point to bones list

	while(bonePtr[jointNum].parentBone != NO_PREVIOUS_JOINT)
	{
		jointNum = bonePtr[jointNum].parentBone;
		
  		Q3Matrix4x4_Multiply(outMatrix,&skeletonPtr->jointTransformMatrix[jointNum],outMatrix);				
	}
	
			/* ALSO FACTOR IN THE BASE MATRIX */

	Q3Matrix4x4_Multiply(outMatrix,&theNode->BaseTransformMatrix,outMatrix);
}

