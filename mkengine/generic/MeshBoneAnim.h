// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#ifndef MK_MESHBONEANIM
#define MK_MESHBONEANIM

#include "MathTools.h"
#include "Ressource.h"
#include "lowDisplay.h"

namespace mk
{
	enum { MESHA_OK, MESHA_ERROR };

	class MeshBoneAnim : public Ressource
	{
		public:
			MeshBoneAnim();
			~MeshBoneAnim();
			void Free();
			
			int LoadFromFile(const char* filename);		// Loads a md5 model from file (with filename)
			int LoadFromFile(FILE* fp, long size);		// The file pointer must be positionned on the mesh file
			int LoadFromMemory(char* mem, int size);	// Load a png from memory

		public:
			//variable definitions
			struct md5_anim_t md5anim;

		private:
			int ReadMD5Anim (const char *filename, struct md5_anim_t *anim);
			void FreeAnim (struct md5_anim_t *anim);
			void BuildFrameSkeleton (const struct joint_info_t *jointInfos, const baseframe_joint_t *baseFrame, const float *animFrameData, struct md5_joint_t *skelFrame, int num_joints);
	};
};

#endif