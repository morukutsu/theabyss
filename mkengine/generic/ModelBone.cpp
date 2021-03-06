// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#include "ModelBone.h"
#include "MeshBone.h"
#include "MeshBoneAnim.h"
#include "Image.h"
#include "AsciiFile.h"
#include "RessourceManager.h"
#include <iostream>
#include "xml\tinyxml.h"
#include <cfloat>
#include "mkengine.h"

namespace mk
{
	ModelBone::ModelBone() : Drawable()
	{
		animated = 0;
		vertexArray = NULL;
		normalArray = NULL;
		skeleton = NULL;
		priority = 0;
		centerX = centerY = 0;
        posX = posY = 0;
        scaleX = scaleY = 1.0f;
        angle = 0.0f;
        mirrorX = mirrorY = false;
        alpha = 1.0f;
		r = g = b = 255;

		animNbr = 0;
        defaultAnim = 0;
		currentAnim = -1;
		isStarted = isPaused = isLoop = false;

		isTransition = false;
		transition = 0;
		transTimer = 0.0;
		mPlayingFrame = 0;

		mMesh = NULL;
		mAnim = NULL;
		image = NULL; 

		currentTime = oldTime = 0.0f;
	}

	ModelBone::~ModelBone()
	{
		if(vertexArray)
		{
			free (vertexArray);
			vertexArray = NULL;
		}

		/*if(normalArray)
		{
			free (normalArray);
			normalArray = NULL;
		}*/

		if(skeleton)
		{
			free (skeleton);
			skeleton = NULL;
		}

		// Liberation de la m�moire
		//modelTris.swap(modelTris);
		//modelTris.clear();

		mRotRestrictions.swap(mRotRestrictions);
		mRotRestrictions.clear();
	}

	void ModelBone::Draw()
	{	
		lowDisplayModelBone(this);
		//DrawSkeleton(skeleton, mMesh->md5file.num_joints);
	}

	void ModelBone::Play(float dt)
	{
		if(isStarted && !isPaused)
		{
			Animate(&mAnim->md5anim, &animInfo, (double)dt * (double)animations[currentAnim].fps);
			//std::cout << (double)animations[currentAnim].fps << std::endl;

			/*float deltaFrame = dt * animations[currentAnim].fps;

			InterpolateSkeletons(mAnim->md5anim.skelFrames[mPlayingFrame],
				mAnim->md5anim.skelFrames[animInfo.next_frame],
				mAnim->md5anim.num_joints,
				(animInfo.last_time + deltaFrame) * mAnim->md5anim.frameRate * animations[currentAnim].fps,
				skeleton);*/
		}

		int nf = 0;
		if(!isTransition)
		{
			nf = animInfo.curr_frame;
		}
		else
		{
			nf = transition;
		}

		mPlayingFrame = nf;
	}

	void ModelBone::AssignMesh(MeshBone* mesh)
	{
		mMesh = mesh;
		vertexArray = (vec5_t *)malloc (sizeof (vec5_t) * mesh->max_verts);
		//vertexIndices = (unsigned int *)malloc (sizeof (unsigned int) * mesh->max_tris * 3);
		//normalArray = (vec3_t *)malloc (sizeof (vec3_t) * mesh->max_verts);
		//mMesh->computeWeightNormals();

		// Bounding box
		bounds = mMesh->bounds;
	}

	void ModelBone::AssignAnim(MeshBoneAnim* anim)
	{
		mAnim = anim;
		animated = 1;
		animInfo.curr_frame = 0;
		animInfo.next_frame = 1;

		animInfo.last_time = 0;
		animInfo.max_time = 1.0 / mAnim->md5anim.frameRate;

		skeleton = (struct md5_joint_t *)
			malloc (sizeof (struct md5_joint_t) * mAnim->md5anim.num_joints);

		mRotRestrictions.resize(mAnim->md5anim.num_joints);
		for(int k = 0; k < mRotRestrictions.size(); k++)
		{
			mRotRestrictions[k].activated = false;
		}
	}

	//Private
	void ModelBone::InterpolateSkeletons (const struct md5_joint_t *skelA, const struct md5_joint_t *skelB, int num_joints, double interp, struct md5_joint_t *out)
	{
		int i;

		for (i = 0; i < num_joints; ++i)
		{
			/* Copy parent index */
			out[i].parent = skelA[i].parent;

			/* Linear interpolation for position */
			out[i].pos[0] = skelA[i].pos[0] + interp * (skelB[i].pos[0] - skelA[i].pos[0]);
			out[i].pos[1] = skelA[i].pos[1] + interp * (skelB[i].pos[1] - skelA[i].pos[1]);
			out[i].pos[2] = skelA[i].pos[2] + interp * (skelB[i].pos[2] - skelA[i].pos[2]);

			/* Spherical linear interpolation for orientation */
			if(mRotRestrictions[i].activated)
			{
				out[i].orient[W] = mRotRestrictions[i].orient[W];
				out[i].orient[X] = mRotRestrictions[i].orient[X];
				out[i].orient[Y] = mRotRestrictions[i].orient[Y];
				out[i].orient[Z] = mRotRestrictions[i].orient[Z];
			}
			else
				Quat_slerp (skelA[i].orient, skelB[i].orient, interp, out[i].orient);
		}
	}

	

	void ModelBone::PrepareMesh (const struct md5_mesh_t *mesh, const struct md5_joint_t *skeleton, vec5_t *vtxArray)
	{
		int i, j, k;

		/* Setup vertices */
		for (i = 0; i < mesh->num_verts; ++i)
		{
			vec3_t finalVertex = { 0.0f, 0.0f, 0.0f };
			//vec3_t wn = { 0.0f, 0.0f, 0.0f };
			//vec3_t newnorm = { 0.0f, 0.0f, 0.0f };

			/* Calculate final vertex to draw with weights */
			for (j = 0; j < mesh->vertices[i].count; ++j)
			{
				const struct md5_weight_t *weight = &mesh->weights[mesh->vertices[i].start + j];
				const struct md5_joint_t *joint = &skeleton[weight->joint];

				/* Calculate transformed vertex for this weight */
				vec3_t wv;
				Quat_rotatePoint (joint->orient, weight->pos, wv);
				
				// Calculate transformed normal for this weight
				/*wn[0] = weight->norm[0];
				wn[1] = weight->norm[1];
				wn[2] = weight->norm[2];
				
				Quat_rotatePoint(joint->orient, wn, newnorm);

				newnorm[0] += newnorm[0] * weight->bias;
				newnorm[1] += newnorm[1] * weight->bias;
				newnorm[2] += newnorm[2] * weight->bias;*/

				/* The sum of all weight->bias should be 1.0 */
				finalVertex[0] += (joint->pos[0] + wv[0]) * weight->bias;
				finalVertex[1] += (joint->pos[1] + wv[1]) * weight->bias;
			}
			
			/*float norme = sqrt(newnorm[0]*newnorm[0] + newnorm[1]*newnorm[1] + newnorm[2]*newnorm[2]);
			newnorm[0] = newnorm[0]/norme;
			newnorm[1] = newnorm[1]/norme;
			newnorm[2] = newnorm[2]/norme;*/

			/*Vec_normalize(newnorm);

			normalArray[i][0] = newnorm[0];
			normalArray[i][1] = newnorm[1];
			normalArray[i][2] = newnorm[2];*/

			vtxArray[i][0] = finalVertex[0];
			vtxArray[i][1] = finalVertex[1];
			vtxArray[i][2] = 0;
			vtxArray[i][3] = mesh->vertices[i].st[0];
			vtxArray[i][4] = mesh->vertices[i].st[1];
		}
	}

	void ModelBone::PrepareMeshFirst (const struct md5_mesh_t *mesh, const struct md5_joint_t *skeleton, vec5_t *vtxArray)
	{
		int i, j, k;

		/* Setup vertices */
		for (i = 0; i < mesh->num_verts; ++i)
		{
			vec3_t finalVertex = { 0.0f, 0.0f, 0.0f };
			//vec3_t wn = { 0.0f, 0.0f, 0.0f };
			//vec3_t newnorm = { 0.0f, 0.0f, 0.0f };

			/* Calculate final vertex to draw with weights */
			for (j = 0; j < mesh->vertices[i].count; ++j)
			{
				const struct md5_weight_t *weight = &mesh->weights[mesh->vertices[i].start + j];
				const struct md5_joint_t *joint = &skeleton[weight->joint];

				/* Calculate transformed vertex for this weight */
				vec3_t wv;
				Quat_rotatePoint (joint->orient, weight->pos, wv);
				
				// Calculate transformed normal for this weight
				/*wn[0] = weight->norm[0];
				wn[1] = weight->norm[1];
				wn[2] = weight->norm[2];
				
				Quat_rotatePoint(joint->orient, wn, newnorm);

				newnorm[0] += newnorm[0] * weight->bias;
				newnorm[1] += newnorm[1] * weight->bias;
				newnorm[2] += newnorm[2] * weight->bias;*/

				/* The sum of all weight->bias should be 1.0 */
				finalVertex[0] += (joint->pos[0] + wv[0]) * weight->bias;
				finalVertex[1] += (joint->pos[1] + wv[1]) * weight->bias;
				finalVertex[2] += (joint->pos[2] + wv[2]) * weight->bias;
			}
			
			/*float norme = sqrt(newnorm[0]*newnorm[0] + newnorm[1]*newnorm[1] + newnorm[2]*newnorm[2]);
			newnorm[0] = newnorm[0]/norme;
			newnorm[1] = newnorm[1]/norme;
			newnorm[2] = newnorm[2]/norme;*/

			/*Vec_normalize(newnorm);

			normalArray[i][0] = newnorm[0];
			normalArray[i][1] = newnorm[1];
			normalArray[i][2] = newnorm[2];*/

			vtxArray[i][0] = finalVertex[0];
			vtxArray[i][1] = finalVertex[1];
			vtxArray[i][2] = finalVertex[2];
			//vtxArray[i][2] = 0;
			vtxArray[i][3] = mesh->vertices[i].st[0];
			vtxArray[i][4] = mesh->vertices[i].st[1];
		}
	}

	void ModelBone::Animate (const struct md5_anim_t *anim, struct anim_info_t *animInfo_s, double dt)
	{
		int maxFrames = animations[currentAnim].frmEnd;

		//On update le counter des transition
		if(isTransition)
		{
			transTimer += dt;

			if(transTimer >= animInfo_s->max_time)
			{
				transTimer = 0.0;
				isTransition = false;
			}
		}

		animInfo_s->last_time += dt;

		/* move to next frame */
		if (animInfo_s->last_time >= animInfo_s->max_time)
		{
			animInfo_s->curr_frame++;
			animInfo_s->next_frame++;
			animInfo_s->last_time = 0.0;

			if (animInfo_s->curr_frame > maxFrames)
				animInfo_s->curr_frame = animations[currentAnim].frmStart;

			if (animInfo_s->next_frame > maxFrames)
				animInfo_s->next_frame = animations[currentAnim].frmStart;
		}
	}

	void ModelBone::SetAnimFrame(int frameNo)
	{
		const struct md5_anim_t *anim = &mAnim->md5anim;
		int maxFrames = animations[currentAnim].frmEnd;

		animInfo.curr_frame = frameNo;
		animInfo.next_frame = frameNo + 1;
		animInfo.last_time = 0.0;

		if (animInfo.curr_frame > maxFrames)
			animInfo.curr_frame = animations[currentAnim].frmStart;

		if (animInfo.next_frame > maxFrames)
			animInfo.next_frame = animations[currentAnim].frmStart;
	}

	int ModelBone::GetAnimFrame()
	{
		return animInfo.curr_frame;
	}

	void ModelBone::DrawSkeleton (const struct md5_joint_t *skeleton, int num_joints)
	{
		glPushMatrix();
		glTranslatef(posX, posY, posZ);

		
		if(mirrorX)
			glRotatef(180.0f, 0.0, 1.0, 0.0); //temporaire, gestion du mirroring
		else
			glRotatef(0.0f, 0.0, 1.0, 0.0); //temporaire, gestion du mirroring
	
		if(mirrorY)
			glRotatef(180.0f, 1.0, 0.0, 0.0); //temporaire, gestion du mirroring
		else
			glRotatef(0.0f, 1.0, 0.0, 0.0); //temporaire, gestion du mirroring


		glRotatef(angle, 0.0, 0.0, 1.0);

		glScalef(scaleX, scaleY, 1.0f);

		glDisable (GL_TEXTURE_2D);
		glDisable (GL_DEPTH_TEST);
		glEnable (GL_BLEND);

		int i;

		// Draw each joint
		glPointSize (5.0f);
		glColor3f (1.0f, 0.0f, 0.0f);
		glBegin (GL_POINTS);
		for (i = 0; i < num_joints; ++i)
		{
			glVertex3f (skeleton[i].pos[0], skeleton[i].pos[1], 0);
		}
		glEnd ();
		glPointSize (1.0f);

		// Draw each bone
		glColor3f (0.0f, 1.0f, 0.0f);
		glBegin (GL_LINES);
		for (i = 0; i < num_joints; ++i)
		{
			if (skeleton[i].parent != -1)
			{
				glVertex3f (skeleton[skeleton[i].parent].pos[0], skeleton[skeleton[i].parent].pos[1], 0);
				glVertex3f (skeleton[i].pos[0], skeleton[i].pos[1], 0);
			}
		}
		glEnd();

		glColor3f(1.0f,1.0f,1.0f);

		glDisable (GL_BLEND);
		glEnable (GL_DEPTH_TEST);

		glPopMatrix();

		//mAutoDepth += DEPTH_AUTOINCREMENT;
	}

    void ModelBone::Tint(unsigned char cr, unsigned char cg, unsigned char cb)
    {
        r = cr;
		g = cg;
		b = cb;
    }

	void ModelBone::Assign(Image* img)
    {
		image = img;
        isTexture = true;
    }

	void ModelBone::Scale(float w, float h)
    {
		scaleX = w;
        scaleY = h;
    }

	void ModelBone::SetSize(float w, float h)
    {
		scaleX = w/image->getImageWidth();
        scaleY = h/image->getImageHeight();
    }

    void ModelBone::Rotate(float Angle)
    {
        angle = Angle;
    }

    void ModelBone::MoveTo(float x, float y)
    {
        posX = x;
        posY = y;
    }

    void ModelBone::Translate(float x, float y)
    {
        posX += x;
        posY += y;
    }
        
	void ModelBone::SetRotCenter(int x, int y)
    {
		centerX = x;
        centerY = y;
    }
        
	void ModelBone::Mirror(bool v, bool h)
    {
		mirrorX = v;
        mirrorY = h;

		bounds = mMesh->bounds;

		if(mirrorX)
		{
			// Rotation de la bounding box
			float sav = bounds.x2;
			bounds.x2 = -bounds.x1;
			bounds.x1 = -sav;
		}

		if(!mirrorY)
		{
			// Rotation de la bounding box
			float sav = bounds.y2;
			bounds.y2 = -bounds.y1;
			bounds.y1 = -sav;
		}
    }

    void ModelBone::Alpha(float val)
    {
		alpha = val;
    }

	void ModelBone::SetDepth(float z)
	{
		posZ = z;
	}

	int ModelBone::AddAnimation(std::string name, int start, int end, float fps)
    {
        //D�claration d'une structure anim temporaire
        sModelAnim tmpAnim = {start, end, fps, name};
        //Ajout au vector
        animations[animNbr] = tmpAnim;
        animNbr++;
        return animNbr-1;
    }

    void ModelBone::PlayAnim(int mode, int anim, bool trans)
    {
        //Reset de l'ancienne animation
        switch (mode)
        {
        case ANIM_LOOP:
            //Reset de l'ancienne animation
            if (currentAnim != anim)
            {	
				transition = animInfo.curr_frame;
				isTransition = trans;
				transTimer = 0.0;

				animInfo.last_time = 0.0;
				animInfo.curr_frame = animations[anim].frmStart;

				animInfo.next_frame = animInfo.curr_frame + 1;	
				if (animInfo.next_frame > animations[anim].frmEnd)
					animInfo.next_frame = animations[anim].frmStart;

                //D�marrage d'une animation
                isStarted = true;
                isLoop = true;
                //Settings
                currentAnim = anim;
            }
            break;

        case ANIM_ONCE:
            if (currentAnim != anim)
            {
				animInfo.last_time = 0.0;
				animInfo.curr_frame = animations[anim].frmStart;
				animInfo.next_frame = animInfo.curr_frame + 1;

				if (animInfo.next_frame > animations[anim].frmEnd)
					animInfo.next_frame = animations[anim].frmStart;

                //D�marrage d'une animation
                isStarted = true;
                isLoop = false;
                //Settings
                currentAnim = anim;
            }
            break;
        }
    }

	void ModelBone::PlayAnim(int mode, std::string anim, bool trans)
	{
		int a = animationsLookup[anim];
		PlayAnim(mode, a, trans);
	}

    void ModelBone::SetAnimationSpeed(int animation, float speed)
    {
        animations[animation].fps = speed;
    }

    void ModelBone::Pause()
    {
        isPaused = true;
    }

    void ModelBone::Restart()
    {
        isPaused = false;
    }

    void ModelBone::Stop()
    {
        animInfo.last_time = 0.0;
		animInfo.curr_frame--;
		animInfo.next_frame = 0;
        isStarted = false;
    }

	int ModelBone::GetBoneID(std::string name)
	{
		int id = -1;

		for(int i = 0; i < mMesh->md5file.num_joints; i++) 
		{
			if(strcmp(name.c_str(), mMesh->md5file.baseSkel[i].name) == 0)
			{
				id = i;
				break;
			}
		}

		return id;
	}

	md5_joint_t* ModelBone::GetBone(int id)
	{
		return &skeleton[id];
	}

	void ModelBone::Interpolate(float dt)
	{
		UpdateTime();

		double deltaFrame = 0 /*(1.0/30.0) * (double)animations[currentAnim].fps * (double)dt*/;
		
		Play(currentTime - oldTime);

		InterpolateSkeletons(mAnim->md5anim.skelFrames[mPlayingFrame],
			mAnim->md5anim.skelFrames[animInfo.next_frame],
			mAnim->md5anim.num_joints,
			(animInfo.last_time + deltaFrame) * mAnim->md5anim.frameRate,
			skeleton);

		posX = Lerp(dt, 0.0f, 1.0f, prevPos.posX, curPos.posX);
		posY = Lerp(dt, 0.0f, 1.0f, prevPos.posY, curPos.posY);
		posZ = Lerp(dt, 0.0f, 1.0f, prevPos.posZ, curPos.posZ);
		alpha = Lerp(dt, 0.0f, 1.0f, prevPos.alpha, curPos.alpha);
		scaleX = Lerp(dt, 0.0f, 1.0f, prevPos.scaleX, curPos.scaleX);
		scaleY = Lerp(dt, 0.0f, 1.0f, prevPos.scaleY, curPos.scaleY);

		if(prevPos.angle != curPos.angle)
			angle = RadiansToDegrees(Slerp2D(dt, 0.0f, 1.0f, DegreesToRadians(prevPos.angle), DegreesToRadians(curPos.angle)));
	}

	void ModelBone::UpdateTime()
	{
		oldTime = currentTime;
		currentTime = lowGetTime();
	}

	void ModelBone::SavePositions()
	{
		prevPos = curPos;
		curPos.posX = posX;
		curPos.posY = posY;
		curPos.posZ = posZ;
		curPos.alpha = alpha;
		curPos.angle = angle;
		curPos.scaleX = scaleX;
		curPos.scaleY = scaleY;
	}

	void ModelBone::LoadModelDescriptorFile(std::string filename)
	{
		mk::AsciiFile * f = (mk::AsciiFile*)mk::RessourceManager::getInstance()->LoadRessource(filename);

		TiXmlDocument doc;

		doc.Parse(f->getString() );

		TiXmlElement * root = doc.FirstChildElement("Model");
		TiXmlElement * elem;

		// Clearing our data structures
		/*animNbr = 0;
		currentAnim = 0;
		animationsLookup.clear();*/


		// Reading from XML file
		std::string mesh, anim, tex;

		elem = root->FirstChildElement("Mesh");
		mesh = elem->GetText();

		elem = root->FirstChildElement("Anim");
		anim = elem->GetText();

		elem = root->FirstChildElement("Texture");
		tex = elem->GetText();

		// Loading animations
		elem = root->FirstChildElement("Animations")->FirstChildElement();
		while(elem) 
		{
			std::string name = elem->Attribute("name");
			int start, end;
			float speed;

			elem->QueryIntAttribute("start", &start);
			elem->QueryIntAttribute("end", &end);
			elem->QueryFloatAttribute("speed", &speed);

			AddAnimation(name, start, end, speed);

			animationsLookup[name] = animNbr-1;

			elem = elem->NextSiblingElement();
		}

		AssignMesh((mk::MeshBone*)mk::RessourceManager::getInstance()->LoadRessource(mesh));
		AssignAnim((mk::MeshBoneAnim*)mk::RessourceManager::getInstance()->LoadRessource(anim));
		Assign((mk::Image*)mk::RessourceManager::getInstance()->LoadRessource(tex));
	}
};