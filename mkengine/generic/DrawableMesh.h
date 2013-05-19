// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#ifndef MK_DRAWABLE_MESH
#define MK_DRAWABLE_MESH

#include "mkengine.h"

namespace mk
{
	class DrawableMesh : public Drawable
	{
		public:
			DrawableMesh();
			~DrawableMesh();

			void Draw();
			void MoveTo(float x, float y, float z);

			void AddVertex(mk::Vertex v);
			void SetVertexMode(int mode) { vertexMode = mode; };

			void SavePositions();
			void Interpolate(float dt);

		private:
			std::vector<Vertex> verts;
			int vertexMode;
	};
};

#endif