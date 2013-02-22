// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#ifndef MK_MESHBONE
#define MK_MESHBONE

#include "MathTools.h"
#include "Ressource.h"
#include "lowDisplay.h"

namespace mk
{
	enum { MESH_OK, MESH_ERROR };

	class MeshBone : public Ressource
	{
		public:
			MeshBone();
			~MeshBone();
			void Free();
			
			int LoadFromFile(const char* filename);		// Loads a md5 model from file (with filename)
			int LoadFromFile(FILE* fp, long size);		// The file pointer must be positionned on the mesh file
			int LoadFromMemory(char* mem, int size);	// Load a png from memory
			
			void computeWeightNormals();

		public:
			//variable definitions
			struct md5_model_t md5file;

			/* vertex array related stuff */
			int max_verts;// = 0;
			int max_tris;// = 0;
	
		private:
			int ReadMD5Model (const char *filename, struct md5_model_t *mdl);
			void FreeModel (struct md5_model_t *mdl);
	};
};


#endif