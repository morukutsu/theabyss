// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#ifndef MK_MODELBONE
#define MK_MODELBONE

#include "Drawable.h"
#include "MathTools.h"
#include <list>
#include <vector>
#include <map>

namespace mk
{
	struct sModelAnim
    {
        //L'unité des positon est un multiple de la taille des frames
        int frmStart;
		int frmEnd;
        float fps;            //Vitesse de l'animation en frames
		std::string name;
    };

	struct sRotationRestriction
	{
		bool activated;
		quat4_t orient;
	};

	class MeshBone;
	class MeshBoneAnim;
	class Image;
	class ModelTri;
	class ModelBone : public Drawable
	{
		public:
			ModelBone();
			~ModelBone();
			void Draw();
			void Play(float dt); 

			void AssignMesh(MeshBone* mesh);
			void AssignAnim(MeshBoneAnim* anim);
			void LoadModelDescriptorFile(std::string filename);

			void Tint(unsigned char, unsigned char, unsigned char);
			void Alpha(float val);
			void Mirror(bool v, bool h);
			void SetRotCenter(int x, int y);
			void Translate(float x, float y);
			void MoveTo(float x, float y);
			void SetSize(float w, float h);
			void Scale(float w, float h);
			void Assign(Image* img);
			void Rotate(float Angle);
			void SetDepth(float z);

			int  AddAnimation(std::string name, int start, int end, float fps);
			void PlayAnim(int mode, int anim, bool trans = true);
			void PlayAnim(int mode, std::string anim, bool trans = true);
			void SetAnimationSpeed(int animation, float speed);
			void Pause();
			void Restart();
			void Stop();
			void SetAnimFrame(int frameNo);

			void Interpolate(float dt);
			void SavePositions();

		public:
			MeshBone* mMesh;
			MeshBoneAnim* mAnim;
			struct md5_joint_t *skeleton;// = NULL;
			struct anim_info_t animInfo;
			Image* image;                 //Pointeur vers l'image
			int isTexture;	

			int animated; // = 0;
			vec5_t *vertexArray;// = NULL;
			vec3_t *normalArray;// = NULL;
			unsigned int *vertexIndices;// = NULL;
			std::list<ModelTri> modelTris;

			int transition;
			float transTimer;
			bool isTransition;
			int mPlayingFrame;

			bool isStarted, isPaused, isLoop;        //Booléeins de depart et de pause pour les animations
			int animNbr, currentAnim, defaultAnim;   //Nombre d'animations dans le sprite
			sModelAnim animations[16];               //Tableau d'animations du sprite
			
			std::map<std::string, int> animationsLookup;
			
			std::vector<sRotationRestriction> mRotRestrictions;

			vec3_t bboxMin, bboxMax;

		public:
			void InterpolateSkeletons (const struct md5_joint_t *skelA, const struct md5_joint_t *skelB, int num_joints, float interp, struct md5_joint_t *out);
			void PrepareMesh (const struct md5_mesh_t *mesh, const struct md5_joint_t *skeleton);
			void Animate (const struct md5_anim_t *anim, struct anim_info_t *animInfo, double dt);
			void DrawSkeleton (const struct md5_joint_t *skeleton, int num_joints);
			void SetupVertexIndices(const struct md5_mesh_t* mesh);
	};

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
};

#endif