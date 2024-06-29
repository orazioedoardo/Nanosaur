/****************************/
/*         MY GLOBALS       */
/* (c)1997 Pangea Software  */
/* By Brian Greenstone      */
/****************************/

#ifndef MYGLOBALS_H
#define MYGLOBALS_H



#include <math.h>
#include <QD3D.h>
#include <QD3DMath.h>



#define	GRAVITY_CONSTANT	1400.0f


		/* 2D ARRAY MACROS */

#define Alloc_2d_array(type, array, n, m)					\
{															\
	array = (type **) AllocPtr((long)n * sizeof(type *));	\
	array[0] = (type *) AllocPtr((long)n * (long)m * sizeof(type));\
	for (int pos = 1; pos < n; pos++)						\
		array[pos] = array[pos-1] + m;						\
}


#define Free_2d_array(array)			\
		DisposePtr((Ptr)array[0]);		\
		DisposePtr((Ptr)array);			\
		array = nil;


#define	PI					kQ3Pi	//3.141592654
#define PI2					(2.0f*PI)


#define	CHAR_RETURN			0x0d	/* ASCII code for Return key */
#define CHAR_UP				0x1e
#define CHAR_DOWN			0x1f
#define	CHAR_LEFT			0x1c
#define	CHAR_RIGHT			0x1d
#define	CHAR_DELETE			0x08


							// COLLISION SIDE INFO
							//=================================
							
#define	SIDE_BITS_TOP		(1)							// %000001	(r/l/b/t)
#define	SIDE_BITS_BOTTOM	(1<<1)						// %000010
#define	SIDE_BITS_LEFT		(1<<2)						// %000100 
#define	SIDE_BITS_RIGHT		(1<<3)						// %001000
#define	SIDE_BITS_FRONT		(1<<4)						// %010000 
#define	SIDE_BITS_BACK		(1<<5)						// %100000
#define	ALL_SOLID_SIDES		(SIDE_BITS_TOP|SIDE_BITS_BOTTOM|SIDE_BITS_LEFT|SIDE_BITS_RIGHT|\
							SIDE_BITS_FRONT|SIDE_BITS_BACK)


							// CBITS (32 BIT VALUES)
							//==================================

enum
{
	CBITS_TOP 			= SIDE_BITS_TOP,
	CBITS_BOTTOM 		= SIDE_BITS_BOTTOM,
	CBITS_LEFT 			= SIDE_BITS_LEFT,
	CBITS_RIGHT 		= SIDE_BITS_RIGHT,
	CBITS_FRONT 		= SIDE_BITS_FRONT,
	CBITS_BACK 			= SIDE_BITS_BACK,
	CBITS_ALLSOLID		= ALL_SOLID_SIDES,
	CBITS_NOTTOP		= SIDE_BITS_LEFT|SIDE_BITS_RIGHT|SIDE_BITS_FRONT|SIDE_BITS_BACK,
	CBITS_TOUCHABLE		= (1<<6)
};


							// CTYPES (32 BIT VALUES)
							//==================================

enum
{
	CTYPE_PLAYER	=	1,			// Me
	CTYPE_ENEMY		=	(1<<1),		// Enemy
	CTYPE_MYBULLET	=	(1<<2),		// Player's bullet
	CTYPE_BONUS		=	(1<<3),		// Bonus item
	CTYPE_TRIGGER	=	(1<<5),		// Trigger
	CTYPE_SKELETON	=	(1<<6),		// Skeleton
	CTYPE_MISC		=	(1<<7),		// Misc
	CTYPE_BLOCKSHADOW =	(1<<8),		// Shadows go over it
	CTYPE_HURTIFTOUCH=	(1<<9),		// Hurt if touched
	CTYPE_PORTAL	=	(1<<10),	// time portal
	CTYPE_BGROUND2 	=	(1<<11),	// Collide against Terrain BGround 2 path tiles
	CTYPE_PICKUP	= 	(1<<12),	// Pickup
	CTYPE_CRYSTAL	= 	(1<<13),	// Crystal
	CTYPE_HURTME	= 	(1<<14),	// Hurt Me
	CTYPE_HURTENEMY	= 	(1<<15),	// Hurt Enemy
	CTYPE_BGROUND 	=	(1<<16),	// Terrain BGround path tiles
	CTYPE_PLAYERTRIGGERONLY	=	(1<<17)		// combined with _TRIGGER, this trigger is only triggerable by player
};



							// OBJNODE STATUS BITS
							//==================================

enum
{
	STATUS_BIT_ONGROUND		=	1,			// OnGround
	STATUS_BIT_ISCARRYING	=	(1<<1),		// Is Carrying Something
	STATUS_BIT_DONTCULL		=	(1<<2),		// set if don't want to perform custom culling on this object
	STATUS_BIT_NOCOLLISION  = 	(1<<3),		// set if want collision code to skip testing against this object
	STATUS_BIT_NOMOVE  		= 	(1<<4),		// dont call object's move function
	STATUS_BIT_ANIM  		= 	(1<<5),		// set if can animate
	STATUS_BIT_HIDDEN		=	(1<<6),		// dont draw object
	STATUS_BIT_REFLECTIONMAP = 	(1<<7),		// use reflection mapping
	STATUS_BIT_ROTZYX		 = 	(1<<8),		// set if want to calc rot matrix by Z->Y->X
	STATUS_BIT_ROTXZY		 = 	(1<<9),		// set if want to calc rot matrix by X->Z->Y
	STATUS_BIT_ISCULLED		 = 	(1<<10),	// set if culling function deemed it culled
	STATUS_BIT_HIGHFILTER	 =  (1<<11),	// set if want to use high texture filtering
	STATUS_BIT_HIGHFILTER2	 =  (1<<12),	// when combined with STATUS_BIT_HIGHFILTER causes best filtering possible
	STATUS_BIT_NULLSHADER	 =  (1<<13),	// used when want to render object will NULL shading (no lighting)
	STATUS_BIT_ALWAYSCULL	 =  (1<<14),	// to force a cull-check
	STATUS_BIT_BLEND_INTERPOLATE =  (1<<15), // to use interpolation blending
	STATUS_BIT_NOTRICACHE 	 =  (1<<16), 	// set if want to disable triangle caching when drawing this xparent obj
	STATUS_BIT_KEEPBACKFACES =	(1<<17),	// set if want to render both front and back faces
	STATUS_BIT_NOZWRITE		=	(1<<18),	// set when want to turn off z buffer writes
};


#include "structs.h"

#endif


