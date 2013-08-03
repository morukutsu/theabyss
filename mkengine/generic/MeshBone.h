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

	//Pour le tri des vertices des modèles
	class ModelTri
	{
	public:
		ModelTri(int indice, float depth)
		{
			i = indice;
			z = depth;
		}

		bool operator<(const ModelTri& a)
		{
			return z > a.z;
		}

	public:
		int i;
		float z;
	};

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
			void SetupVertexIndices();
			void ProcessModel();

		public:
			//variable definitions
			struct md5_model_t md5file;

			/* vertex array related stuff */
			int max_verts;// = 0;
			int max_tris;// = 0;
			
			unsigned int *vertexIndices;// = NULL;
			vec5_t *vertexArray;// = NULL;
			std::list<ModelTri> modelTris;

			mk::BoundingBox bounds;

		private:
			int ReadMD5Model (const char *filename, struct md5_model_t *mdl);
			void FreeModel (struct md5_model_t *mdl);
	};
};


#endif