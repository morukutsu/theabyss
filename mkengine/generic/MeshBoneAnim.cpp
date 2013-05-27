// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#include "MeshBoneAnim.h"
#include "lowSystem.h"

namespace mk
{
	MeshBoneAnim::MeshBoneAnim()
	{
		mLoaded = false;
	}

	MeshBoneAnim::~MeshBoneAnim()
	{
		FreeAnim(&md5anim);
	}

	void MeshBoneAnim::Free()
	{
		
	}
			
	int MeshBoneAnim::LoadFromFile(const char* filename)
	{
		if(!ReadMD5Anim (filename, &md5anim))
		{
			FreeAnim (&md5anim);
			mLoaded = true;
			return MESHA_ERROR;
		}
		else
		{
			return MESHA_OK;
		}
	}

	int MeshBoneAnim::LoadFromFile(FILE* fptr, long size)
	{
		FILE *fp = fptr;
		char buff[512];
		struct joint_info_t *jointInfos = NULL;
		struct baseframe_joint_t *baseFrame = NULL;
		float *animFrameData = NULL;
		int version;
		int numAnimatedComponents;
		int frame_index;
		int i;
		int totalRead = size;
		int read;
		struct md5_anim_t *anim = &md5anim;
		int frameCount = 0;
		bool quit = false;

		while(totalRead > 0 && !quit)
		{
			/* Read whole line */
			read = lowFileGets(buff, 512, fp);
			totalRead -= read;

			if(sscanf (buff, " MD5Version %d", &version) == 1)
			{
				if(version != 10)
				{
					/* Bad version */
					fprintf (stderr, "Error: bad animation version\n");
					return MESHA_ERROR;
				}
			}
			else if(sscanf (buff, " numFrames %d", &anim->num_frames) == 1)
			{
				/* Allocate memory for skeleton frames and bounding boxes */
				if(anim->num_frames > 0)
				{
					anim->skelFrames = (struct md5_joint_t **)
					malloc (sizeof (struct md5_joint_t*) * anim->num_frames);
					anim->bboxes = (struct md5_bbox_t *)
					malloc (sizeof (struct md5_bbox_t) * anim->num_frames);
				}
			}
			else if(sscanf (buff, " numJoints %d", &anim->num_joints) == 1)
			{
				if(anim->num_joints > 0)
				{
					for (i = 0; i < anim->num_frames; ++i)
					{
						/* Allocate memory for joints of each frame */
						anim->skelFrames[i] = (struct md5_joint_t *)
						malloc (sizeof (struct md5_joint_t) * anim->num_joints);
					}

					/* Allocate temporary memory for building skeleton frames */
					jointInfos = (struct joint_info_t *)
					malloc (sizeof (struct joint_info_t) * anim->num_joints);

					baseFrame = (struct baseframe_joint_t *)
					malloc (sizeof (struct baseframe_joint_t) * anim->num_joints);
				}
			}
			else if (sscanf (buff, " frameRate %d", &anim->frameRate) == 1)
			{			
				//printf ("md5anim: animation's frame rate is %d\n", anim->frameRate);
			}
			else if(sscanf (buff, " numAnimatedComponents %d", &numAnimatedComponents) == 1)
			{
				if(numAnimatedComponents > 0)
				{
					/* Allocate memory for animation frame data */
					animFrameData = (float *)malloc (sizeof (float) * numAnimatedComponents);
				}
			}
			else if (strncmp (buff, "hierarchy {", 11) == 0)
			{
				for (i = 0; i < anim->num_joints; ++i)
				{
					/* Read whole line */
					read = lowFileGets(buff, 512, fp);
					totalRead -= read;

					/* Read joint info */
					sscanf (buff, " %s %d %d %d", jointInfos[i].name, &jointInfos[i].parent,
						&jointInfos[i].flags, &jointInfos[i].startIndex);
				}
			}
			else if (strncmp (buff, "bounds {", 8) == 0)
			{
				for (i = 0; i < anim->num_frames; ++i)
				{
					/* Read whole line */
					read = lowFileGets(buff, 512, fp);
					totalRead -= read;

					/* Read bounding box */
					sscanf (buff, " ( %f %f %f ) ( %f %f %f )",
						&anim->bboxes[i].min[0], &anim->bboxes[i].min[1],
						&anim->bboxes[i].min[2], &anim->bboxes[i].max[0],
						&anim->bboxes[i].max[1], &anim->bboxes[i].max[2]);
				}
			}
			else if (strncmp (buff, "baseframe {", 10) == 0)
			{
				for (i = 0; i < anim->num_joints; ++i)
				{
					/* Read whole line */
					read = lowFileGets(buff, 512, fp);
					totalRead -= read;

					/* Read base frame joint */
					if (sscanf (buff, " ( %f %f %f ) ( %f %f %f )",
						&baseFrame[i].pos[0], &baseFrame[i].pos[1],
						&baseFrame[i].pos[2], &baseFrame[i].orient[0],
						&baseFrame[i].orient[1], &baseFrame[i].orient[2]) == 6)
					{
						baseFrame[i].pos[2] = 0;
						baseFrame[i].orient[2] = 0;

						/* Compute the w component */
						Quat_computeW (baseFrame[i].orient);
					}
				}
			}
			else if (sscanf (buff, " frame %d", &frame_index) == 1)
			{
				/* Read frame data */
				for (i = 0; i < numAnimatedComponents; ++i)
					fscanf (fp, "%f", &animFrameData[i]);

				/* Build frame skeleton from the collected data */
				BuildFrameSkeleton (jointInfos, baseFrame, animFrameData, anim->skelFrames[frame_index], anim->num_joints);
				
				frameCount++;
				if(frameCount == anim->num_frames)
					quit = true;
			}
		}

		/* Free temporary data allocated */
		if (animFrameData)
			free (animFrameData);

		if (baseFrame)
			free (baseFrame);

		if (jointInfos)
			free (jointInfos);

		mLoaded = true;

		return MESHA_OK;
	}

	int MeshBoneAnim::LoadFromMemory(char* mem, int size)
	{
		return 0;
	}

	int MeshBoneAnim::ReadMD5Anim (const char *filename, struct md5_anim_t *anim)
	{
		FILE *fp = NULL;
		char buff[512];
		struct joint_info_t *jointInfos = NULL;
		struct baseframe_joint_t *baseFrame = NULL;
		float *animFrameData = NULL;
		int version;
		int numAnimatedComponents;
		int frame_index;
		int i;

		fp = fopen (filename, "rb");
		if(!fp)
		{
			fprintf (stderr, "error: couldn't open \"%s\"!\n", filename);
			return 0;
		}

		while(!feof (fp))
		{
			/* Read whole line */
			fgets(buff, sizeof (buff), fp);

			if(sscanf (buff, " MD5Version %d", &version) == 1)
			{
				if(version != 10)
				{
					/* Bad version */
					fprintf (stderr, "Error: bad animation version\n");
					fclose (fp);
					return 0;
				}
			}
			else if(sscanf (buff, " numFrames %d", &anim->num_frames) == 1)
			{
				/* Allocate memory for skeleton frames and bounding boxes */
				if(anim->num_frames > 0)
				{
					anim->skelFrames = (struct md5_joint_t **)
					malloc (sizeof (struct md5_joint_t*) * anim->num_frames);
					anim->bboxes = (struct md5_bbox_t *)
					malloc (sizeof (struct md5_bbox_t) * anim->num_frames);
				}
			}
			else if(sscanf (buff, " numJoints %d", &anim->num_joints) == 1)
			{
				if(anim->num_joints > 0)
				{
					for (i = 0; i < anim->num_frames; ++i)
					{
						/* Allocate memory for joints of each frame */
						anim->skelFrames[i] = (struct md5_joint_t *)
						malloc (sizeof (struct md5_joint_t) * anim->num_joints);
					}

					/* Allocate temporary memory for building skeleton frames */
					jointInfos = (struct joint_info_t *)
					malloc (sizeof (struct joint_info_t) * anim->num_joints);

					baseFrame = (struct baseframe_joint_t *)
					malloc (sizeof (struct baseframe_joint_t) * anim->num_joints);
				}
			}
			else if (sscanf (buff, " frameRate %d", &anim->frameRate) == 1)
			{			
				//printf ("md5anim: animation's frame rate is %d\n", anim->frameRate);
			}
			else if(sscanf (buff, " numAnimatedComponents %d", &numAnimatedComponents) == 1)
			{
				if(numAnimatedComponents > 0)
				{
					/* Allocate memory for animation frame data */
					animFrameData = (float *)malloc (sizeof (float) * numAnimatedComponents);
				}
			}
			else if (strncmp (buff, "hierarchy {", 11) == 0)
			{
				for (i = 0; i < anim->num_joints; ++i)
				{
					/* Read whole line */
					fgets (buff, sizeof (buff), fp);

					/* Read joint info */
					sscanf (buff, " %s %d %d %d", jointInfos[i].name, &jointInfos[i].parent,
						&jointInfos[i].flags, &jointInfos[i].startIndex);
				}
			}
			else if (strncmp (buff, "bounds {", 8) == 0)
			{
				for (i = 0; i < anim->num_frames; ++i)
				{
					/* Read whole line */
					fgets (buff, sizeof (buff), fp);

					/* Read bounding box */
					sscanf (buff, " ( %f %f %f ) ( %f %f %f )",
						&anim->bboxes[i].min[0], &anim->bboxes[i].min[1],
						&anim->bboxes[i].min[2], &anim->bboxes[i].max[0],
						&anim->bboxes[i].max[1], &anim->bboxes[i].max[2]);
				}
			}
			else if (strncmp (buff, "baseframe {", 10) == 0)
			{
				for (i = 0; i < anim->num_joints; ++i)
				{
					/* Read whole line */
					fgets (buff, sizeof (buff), fp);

					/* Read base frame joint */
					if (sscanf (buff, " ( %f %f %f ) ( %f %f %f )",
						&baseFrame[i].pos[0], &baseFrame[i].pos[1],
						&baseFrame[i].pos[2], &baseFrame[i].orient[0],
						&baseFrame[i].orient[1], &baseFrame[i].orient[2]) == 6)
					{
						/* Compute the w component */
						Quat_computeW (baseFrame[i].orient);
					}
				}
			}
			else if (sscanf (buff, " frame %d", &frame_index) == 1)
			{
				/* Read frame data */
				for (i = 0; i < numAnimatedComponents; ++i)
					fscanf (fp, "%f", &animFrameData[i]);

				/* Build frame skeleton from the collected data */
				BuildFrameSkeleton (jointInfos, baseFrame, animFrameData, anim->skelFrames[frame_index], anim->num_joints);
			}
		}

		fclose (fp);

		/* Free temporary data allocated */
		if (animFrameData)
			free (animFrameData);

		if (baseFrame)
			free (baseFrame);

		if (jointInfos)
			free (jointInfos);

		return 1;
	}

	void MeshBoneAnim::FreeAnim (struct md5_anim_t *anim)
	{
		int i;
		if(anim->skelFrames)
		{
			for (i = 0; i < anim->num_frames; ++i)
			{
				if(anim->skelFrames[i])
				{
					free(anim->skelFrames[i]);
					anim->skelFrames[i] = NULL;
				}
			}

			free (anim->skelFrames);
			anim->skelFrames = NULL;
		}

		if(anim->bboxes)
		{
			free (anim->bboxes);
			anim->bboxes = NULL;
		}
	}

	void MeshBoneAnim::BuildFrameSkeleton (const struct joint_info_t *jointInfos, const baseframe_joint_t *baseFrame, const float *animFrameData, struct md5_joint_t *skelFrame, int num_joints)
	{
		int i;

		for(i = 0; i < num_joints; ++i)
		{
			const struct baseframe_joint_t *baseJoint = &baseFrame[i];
			vec3_t animatedPos;
			quat4_t animatedOrient;
			int j = 0;

			memcpy(animatedPos, baseJoint->pos, sizeof (vec3_t));
			memcpy(animatedOrient, baseJoint->orient, sizeof (quat4_t));

			if(jointInfos[i].flags & 1) /* Tx */
			{
				animatedPos[0] = animFrameData[jointInfos[i].startIndex + j];
				++j;
			}

			if(jointInfos[i].flags & 2) /* Ty */
			{
				animatedPos[1] = animFrameData[jointInfos[i].startIndex + j];
				++j;
			}

			if(jointInfos[i].flags & 4) /* Tz */
			{
				animatedPos[2] = animFrameData[jointInfos[i].startIndex + j];
				++j;
			}

			if(jointInfos[i].flags & 8) /* Qx */
			{
				animatedOrient[0] = animFrameData[jointInfos[i].startIndex + j];
				++j;
			}

			if(jointInfos[i].flags & 16) /* Qy */
			{
				animatedOrient[1] = animFrameData[jointInfos[i].startIndex + j];
				++j;
			}

			if(jointInfos[i].flags & 32) /* Qz */
			{
				animatedOrient[2] = animFrameData[jointInfos[i].startIndex + j];
				++j;
			}

			/* Compute orient quaternion's w value */
			Quat_computeW (animatedOrient);

			/* NOTE: we assume that this joint's parent has
			 already been calculated, i.e. joint's ID should
			never be smaller than its parent ID. */
			struct md5_joint_t *thisJoint = &skelFrame[i];

			int parent = jointInfos[i].parent;
			thisJoint->parent = parent;
			strcpy (thisJoint->name, jointInfos[i].name);

			/* Has parent? */
			if (thisJoint->parent < 0)
			{
				memcpy (thisJoint->pos, animatedPos, sizeof (vec3_t));
				memcpy (thisJoint->orient, animatedOrient, sizeof (quat4_t));
			}
			else
			{
				struct md5_joint_t *parentJoint = &skelFrame[parent];
				vec3_t rpos; /* Rotated position */

				/* Add positions */
				Quat_rotatePoint (parentJoint->orient, animatedPos, rpos);
				thisJoint->pos[0] = rpos[0] + parentJoint->pos[0];
				thisJoint->pos[1] = rpos[1] + parentJoint->pos[1];
				thisJoint->pos[2] = rpos[2] + parentJoint->pos[2];

				/* Concatenate rotations */
				Quat_multQuat (parentJoint->orient, animatedOrient, thisJoint->orient);
				Quat_normalize (thisJoint->orient);
			}
		}
	}
};