// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#include "MeshBone.h"
#include "lowSystem.h"

namespace mk
{
	MeshBone::MeshBone()
	{
		//variable initialisations
		//animated = 0;
		//skeleton = NULL;

		/* vertex array related stuff */
		max_verts = 0;
		max_tris = 0;

		/*vertexArray = NULL;
		vertexIndices = NULL;
		normalArray = NULL;*/

		vertexArray = NULL;
		vertexIndices = NULL;

		mLoaded = false;
	}

	MeshBone::~MeshBone()
	{
		FreeModel(&md5file);

		if(vertexIndices)
		{
			free (vertexIndices);
			vertexIndices = NULL;
		}

		if(vertexArray)
		{
			free (vertexArray);
			vertexArray = NULL;
		}
	}


	void MeshBone::Free()
	{

	}

	int MeshBone::LoadFromFile(const char* filename)
	{
		//On charge le modèle
		if(!ReadMD5Model(filename, &md5file) )
			return MESH_ERROR;

		//std::cout << "Notif : Modele charge " << filename << std::endl;
		//AllocVertexArrays();

		SetupVertexIndices();
		ProcessModel();

		mLoaded = true;

		return MESH_OK;
	}

	int MeshBone::LoadFromFile(FILE* fptr, long size)
	{
		FILE *fp = fptr;
		char buff[512];
		int version;
		int curr_mesh = 0;
		int i;
		int read = 0;
		int totalRead = size;
		struct md5_model_t *mdl = &md5file;

		while (totalRead > 0)
		{
			/* Read whole line */
			read = lowFileGets (buff, 512, fp);
			totalRead -= read;

			if (sscanf (buff, " MD5Version %d", &version) == 1)
			{
				if (version != 10)
				{
					/* Bad version */
					fprintf (stdout, "Error: bad model version\n");
					return MESH_ERROR;
				}
			}
			else if (sscanf (buff, " numJoints %d", &mdl->num_joints) == 1)
			{
				if (mdl->num_joints > 0)
				{
					/* Allocate memory for base skeleton joints */
					mdl->baseSkel = (struct md5_joint_t *)
					calloc (mdl->num_joints, sizeof (struct md5_joint_t));
				}
			}
			else if (sscanf (buff, " numMeshes %d", &mdl->num_meshes) == 1)
			{
				if (mdl->num_meshes > 0)
				{
					/* Allocate memory for meshes */
					mdl->meshes = (struct md5_mesh_t *)
					calloc (mdl->num_meshes, sizeof (struct md5_mesh_t));
				}
			}
			else if (strncmp (buff, "joints {", 8) == 0)
			{
				/* Read each joint */
				for (i = 0; i < mdl->num_joints; ++i)
				{
					struct md5_joint_t *joint = &mdl->baseSkel[i];

					/* Read whole line */
					read = lowFileGets (buff, 512, fp);
					totalRead -= read;

					if (sscanf (buff, "%s %d ( %f %f %f ) ( %f %f %f )",
					  joint->name, &joint->parent, &joint->pos[0],
					  &joint->pos[1], &joint->pos[2], &joint->orient[0],
					  &joint->orient[1], &joint->orient[2]) == 8)
					{
						//joint->pos[2] = 0;
						//joint->orient[2] = 0;
						/* Compute the w component */
						Quat_computeW (joint->orient);
					}
				}
			}
			else if (strncmp (buff, "mesh {", 6) == 0)
			{
				struct md5_mesh_t *mesh = &mdl->meshes[curr_mesh];
				int vert_index = 0;
				int tri_index = 0;
				int weight_index = 0;
				float fdata[4];
				int idata[3];

				while ((buff[0] != '}') && totalRead > 0)
				{
					/* Read whole line */
					read = lowFileGets (buff, 512, fp);
					totalRead -= read;

					if (strstr (buff, "shader "))
					{
						int quote = 0, j = 0;

						/* Copy the shader name whithout the quote marks */
						for (i = 0; i < sizeof (buff) && (quote < 2); ++i)
						{
							if (buff[i] == '\"')
							quote++;

							if ((quote == 1) && (buff[i] != '\"'))
							{
								mesh->shader[j] = buff[i];
								j++;
							}
						}
					}
					else if (sscanf (buff, " numverts %d", &mesh->num_verts) == 1)
					{
						if (mesh->num_verts > 0)
						{
							/* Allocate memory for vertices */
							mesh->vertices = (struct md5_vertex_t *)
							malloc (sizeof (struct md5_vertex_t) * mesh->num_verts);
						}

						if (mesh->num_verts > max_verts)
							max_verts = mesh->num_verts;
					}
					else if (sscanf (buff, " numtris %d", &mesh->num_tris) == 1)
					{
						if (mesh->num_tris > 0)
						{
							/* Allocate memory for triangles */
							mesh->triangles = (struct md5_triangle_t *)
							malloc (sizeof (struct md5_triangle_t) * mesh->num_tris);
						}

						if (mesh->num_tris > max_tris)
							max_tris = mesh->num_tris;
					}
					else if (sscanf (buff, " numweights %d", &mesh->num_weights) == 1)
					{
						if (mesh->num_weights > 0)
						{
							/* Allocate memory for vertex weights */
							mesh->weights = (struct md5_weight_t *)
							malloc (sizeof (struct md5_weight_t) * mesh->num_weights);
						}
					}
					else if (sscanf (buff, " vert %d ( %f %f ) %d %d", &vert_index,
						&fdata[0], &fdata[1], &idata[0], &idata[1]) == 5)
					{
						/* Copy vertex data */
						mesh->vertices[vert_index].st[0] = fdata[0];
						mesh->vertices[vert_index].st[1] = fdata[1];
						mesh->vertices[vert_index].start = idata[0];
						mesh->vertices[vert_index].count = idata[1];
					}
					else if (sscanf (buff, " tri %d %d %d %d", &tri_index,
						&idata[0], &idata[1], &idata[2]) == 4)
					{
						/* Copy triangle data */
						mesh->triangles[tri_index ].index[0] = idata[0];
						mesh->triangles[tri_index ].index[1] = idata[1];
						mesh->triangles[tri_index ].index[2] = idata[2];
					}
					else if (sscanf (buff, " weight %d %d %f ( %f %f %f )",
						&weight_index, &idata[0], &fdata[3],
						&fdata[0], &fdata[1], &fdata[2]) == 6)
					{
						/* Copy vertex data */
						mesh->weights[weight_index].joint  = idata[0];
						mesh->weights[weight_index].bias   = fdata[3];
						mesh->weights[weight_index].pos[0] = fdata[0];
						mesh->weights[weight_index].pos[1] = fdata[1];
						mesh->weights[weight_index].pos[2] = fdata[2];

						//mesh->weights[weight_index].pos[2] = 0;
					}
				}

				curr_mesh++;
			}
		}

		SetupVertexIndices();
		ProcessModel();
		mLoaded = true;

		return MESH_OK;
	}

	int MeshBone::LoadFromMemory(char* mem, int size)
	{
		return 0;
	}

	int MeshBone::ReadMD5Model (const char *filename, struct md5_model_t *mdl)
	{
		FILE *fp;
		char buff[512];
		int version;
		int curr_mesh = 0;
		int i;

		fp = fopen (filename, "rb");
		if (!fp)
		{
			fprintf (stderr, "Error: couldn't open \"%s\"!\n", filename);
			return 0;
		}

		while (!feof (fp))
		{
			/* Read whole line */
			fgets (buff, sizeof (buff), fp);

			if (sscanf (buff, " MD5Version %d", &version) == 1)
			{
				if (version != 10)
				{
					/* Bad version */
					fprintf (stderr, "Error: bad model version\n");
					fclose (fp);
					return 0;
				}
			}
			else if (sscanf (buff, " numJoints %d", &mdl->num_joints) == 1)
			{
				if (mdl->num_joints > 0)
				{
					/* Allocate memory for base skeleton joints */
					mdl->baseSkel = (struct md5_joint_t *)
					calloc (mdl->num_joints, sizeof (struct md5_joint_t));
				}
			}
			else if (sscanf (buff, " numMeshes %d", &mdl->num_meshes) == 1)
			{
				if (mdl->num_meshes > 0)
				{
					/* Allocate memory for meshes */
					mdl->meshes = (struct md5_mesh_t *)
					calloc (mdl->num_meshes, sizeof (struct md5_mesh_t));
				}
			}
			else if (strncmp (buff, "joints {", 8) == 0)
			{
				/* Read each joint */
				for (i = 0; i < mdl->num_joints; ++i)
				{
					struct md5_joint_t *joint = &mdl->baseSkel[i];

					/* Read whole line */
					fgets (buff, sizeof (buff), fp);
					
					if (sscanf (buff, "\"%s\" %d ( %f %f %f ) ( %f %f %f )",
					  joint->name, &joint->parent, &joint->pos[0],
					  &joint->pos[1], &joint->pos[2], &joint->orient[0],
					  &joint->orient[1], &joint->orient[2]) == 8)
					{
						printf("\"%s\" %d ( %f %f %f ) ( %f %f %f )\n",
					  joint->name, joint->parent, joint->pos[0],
					  joint->pos[1], joint->pos[2], joint->orient[0],
					  joint->orient[1], joint->orient[2]);
						/* Compute the w component */
						Quat_computeW (joint->orient);
					}
				}
			}
			else if (strncmp (buff, "mesh {", 6) == 0)
			{
				struct md5_mesh_t *mesh = &mdl->meshes[curr_mesh];
				int vert_index = 0;
				int tri_index = 0;
				int weight_index = 0;
				float fdata[4];
				int idata[3];

				while ((buff[0] != '}') && !feof (fp))
				{
					/* Read whole line */
					fgets (buff, sizeof (buff), fp);

					if (strstr (buff, "shader "))
					{
						int quote = 0, j = 0;

						/* Copy the shader name whithout the quote marks */
						for (i = 0; i < sizeof (buff) && (quote < 2); ++i)
						{
							if (buff[i] == '\"')
							quote++;

							if ((quote == 1) && (buff[i] != '\"'))
							{
								mesh->shader[j] = buff[i];
								j++;
							}
						}
					}
					else if (sscanf (buff, " numverts %d", &mesh->num_verts) == 1)
					{
						if (mesh->num_verts > 0)
						{
							/* Allocate memory for vertices */
							mesh->vertices = (struct md5_vertex_t *)
							malloc (sizeof (struct md5_vertex_t) * mesh->num_verts);
						}

						if (mesh->num_verts > max_verts)
							max_verts = mesh->num_verts;
					}
					else if (sscanf (buff, " numtris %d", &mesh->num_tris) == 1)
					{
						if (mesh->num_tris > 0)
						{
							/* Allocate memory for triangles */
							mesh->triangles = (struct md5_triangle_t *)
							malloc (sizeof (struct md5_triangle_t) * mesh->num_tris);
						}

						if (mesh->num_tris > max_tris)
							max_tris = mesh->num_tris;
					}
					else if (sscanf (buff, " numweights %d", &mesh->num_weights) == 1)
					{
						if (mesh->num_weights > 0)
						{
							/* Allocate memory for vertex weights */
							mesh->weights = (struct md5_weight_t *)
							malloc (sizeof (struct md5_weight_t) * mesh->num_weights);
						}
					}
					else if (sscanf (buff, " vert %d ( %f %f ) %d %d", &vert_index,
						&fdata[0], &fdata[1], &idata[0], &idata[1]) == 5)
					{
						/* Copy vertex data */
						mesh->vertices[vert_index].st[0] = fdata[0];
						mesh->vertices[vert_index].st[1] = fdata[1];
						mesh->vertices[vert_index].start = idata[0];
						mesh->vertices[vert_index].count = idata[1];
					}
					else if (sscanf (buff, " tri %d %d %d %d", &tri_index,
						&idata[0], &idata[1], &idata[2]) == 4)
					{
						/* Copy triangle data */
						mesh->triangles[tri_index ].index[0] = idata[0];
						mesh->triangles[tri_index ].index[1] = idata[1];
						mesh->triangles[tri_index ].index[2] = idata[2];
					}
					else if (sscanf (buff, " weight %d %d %f ( %f %f %f )",
						&weight_index, &idata[0], &fdata[3],
						&fdata[0], &fdata[1], &fdata[2]) == 6)
					{
						/* Copy vertex data */
						mesh->weights[weight_index].joint  = idata[0];
						mesh->weights[weight_index].bias   = fdata[3];
						mesh->weights[weight_index].pos[0] = fdata[0];
						mesh->weights[weight_index].pos[1] = fdata[1];
						mesh->weights[weight_index].pos[2] = fdata[2];
					}
				}

				curr_mesh++;
			}
		}

		fclose (fp);

		return 1;
	}


	void MeshBone::FreeModel (struct md5_model_t *mdl)
	{
		int i;

		if(mdl->baseSkel)
		{
			free (mdl->baseSkel);
			mdl->baseSkel = NULL;
		}

		if(mdl->meshes)
		{
			/* Free mesh data */
			for (i = 0; i < mdl->num_meshes; ++i)
			{
				if(mdl->meshes[i].vertices)
				{
					free (mdl->meshes[i].vertices);
					mdl->meshes[i].vertices = NULL;
				}

				if(mdl->meshes[i].triangles)
				{
					free (mdl->meshes[i].triangles);
					mdl->meshes[i].triangles = NULL;
				}

				if(mdl->meshes[i].weights)
				{
					free (mdl->meshes[i].weights);
					mdl->meshes[i].weights = NULL;
				}
			}

			free (mdl->meshes);
			mdl->meshes = NULL;
		}
	}

	void MeshBone::SetupVertexIndices()
	{
		vertexIndices = (unsigned int *)malloc (sizeof (unsigned int) * max_tris * 3);

		/* Setup vertex indices */
		int i, j, k;
		for (k = 0, i = 0; i < md5file.meshes[0].num_tris; ++i)
		{
			for (j = 0; j < 3; ++j, ++k)
				vertexIndices[k] = md5file.meshes[0].triangles[i].index[j];
		}
	}

	void MeshBone::ProcessModel()
	{
		vertexArray = (vec5_t *)malloc (sizeof (vec5_t) * max_verts);

		// Bounding box
		bounds.x1 = bounds.y1 = FLT_MAX;
		bounds.x2 = bounds.y2 = FLT_MIN;

		//Tri des triangles selon Z
		//SetupVertexIndices(&mMesh->md5file.meshes[0]);
		mk::ModelBone::PrepareMesh (&md5file.meshes[0], md5file.baseSkel, vertexArray);
		for(int k = 0; k < md5file.meshes[0].num_tris; k++)
		{
			float depth = 0;
			float* vtx_ = vertexArray[md5file.meshes[0].triangles[k].index[0]];
			depth = -vtx_[2];
			for(int j = 1; j < 3; j++)
			{
				float* vtx = vertexArray[md5file.meshes[0].triangles[k].index[j]];
				if(-vtx_[2] < depth)
					depth = -vtx_[2];

				// Calculate bbox
				if(vtx[0] < bounds.x1)
					bounds.x1 = vtx[0];
				if(vtx[1] < bounds.y1)
					bounds.y1 = vtx[1];
				if(vtx[0] > bounds.x2)
					bounds.x2 = vtx[0];
				if(vtx[1] > bounds.y2)
					bounds.y2 = vtx[1];
			}
			//ModelTri mt(k, depth);
			//modelTris.push_back(mt);
		}
		//modelTris.sort();

		// Rotation de la bounding box
		float sav = bounds.y2;
		bounds.y2 = -bounds.y1;
		bounds.y1 = -sav;
	}

	void MeshBone::computeWeightNormals()
	{
		/*vec3_t* bindposnorms = (vec3_t *)malloc(sizeof (vec3_t) * max_verts);
		for(int k = 0; k < max_verts; k++)
		{
			bindposnorms[k][0] = 0.0f;
			bindposnorms[k][1] = 0.0f;
			bindposnorms[k][2] = 0.0f;
		}

		for(int i = 0; i < md5file.num_meshes; ++i)
		{
			md5_mesh_t* mesh = &md5file.meshes[i];

			// Setup vertices 
			for (int g = 0; g < mesh->num_verts; ++g)
			{
				vec3_t finalVertex = { 0.0f, 0.0f, 0.0f };

				// Calculate final vertex to draw with weights
				for (int j = 0; j < mesh->vertices[g].count; ++j)
				{
					const struct md5_weight_t *weight = &mesh->weights[mesh->vertices[g].start + j];
					const struct md5_joint_t *joint = &md5file.baseSkel[weight->joint];

					// Calculate transformed vertex for this weight
					vec3_t wv;
					Quat_rotatePoint (joint->orient, weight->pos, wv);

					// The sum of all weight->bias should be 1.0
					finalVertex[0] += (joint->pos[0] + wv[0]) * weight->bias;
					finalVertex[1] += (joint->pos[1] + wv[1]) * weight->bias;
					finalVertex[2] += (joint->pos[2] + wv[2]) * weight->bias;
				}

				vertexArray[g][0] = finalVertex[0];
				vertexArray[g][1] = finalVertex[1];
				vertexArray[g][2] = finalVertex[2];
			}

			//Calcul des normales
			for (int g = 0; g < mesh->num_tris; ++g)
			{
				md5_triangle_t* triangle = &mesh->triangles[g];
				
				vec3_t norm;

				vec3_t tri[3];
				tri[0][0] = vertexArray[triangle->index[0]][0], tri[0][1] = vertexArray[triangle->index[0]][1], tri[0][2] = vertexArray[triangle->index[0]][2];
				tri[1][0] = vertexArray[triangle->index[1]][0], tri[1][1] = vertexArray[triangle->index[1]][1], tri[1][2] = vertexArray[triangle->index[1]][2];
				tri[2][0] = vertexArray[triangle->index[2]][0], tri[2][1] = vertexArray[triangle->index[2]][1], tri[2][2] = vertexArray[triangle->index[2]][2];
				
				vec2_t p1, p2;
				p1[0] = tri[2][0] - tri[0][0];
				p1[1] = tri[2][1] - tri[0][1];
				p1[2] = tri[2][2] - tri[0][2];

				p2[0] = tri[1][0] - tri[0][0]; 
				p2[1] = tri[1][1] - tri[0][1];
				p2[2] = tri[1][2] - tri[0][2];

				norm[0] = ((p1[1] * p2[2]) - (p1[2] + p2[1]));
				norm[1] = ((p1[2] * p2[0]) - (p1[0] + p2[2]));
				norm[2] = ((p1[0] * p2[1]) - (p1[1] + p2[0]));

				Vec_normalize(norm);

				bindposnorms[triangle->index[0]][0] += norm[0], bindposnorms[triangle->index[0]][1] += norm[1], bindposnorms[triangle->index[0]][2] += norm[2];
				bindposnorms[triangle->index[1]][0] += norm[0], bindposnorms[triangle->index[1]][1] += norm[1], bindposnorms[triangle->index[1]][2] += norm[2];
				bindposnorms[triangle->index[2]][0] += norm[0], bindposnorms[triangle->index[2]][1] += norm[1], bindposnorms[triangle->index[2]][2] += norm[2];	
			}

			for(int k = 0; k < mesh->num_verts; k++)
			{
				Vec_normalize(bindposnorms[k]);
			}

			//Calcul des weight normals
			for(int g = 0; g < mesh->num_weights; g++)
			{
				mesh->weights[g].norm[0] = 0.0f;
				mesh->weights[g].norm[1] = 0.0f;
				mesh->weights[g].norm[2] = 0.0f;
			}
			
			for(int g = 0; g < mesh->num_verts; g++)
			{
				for(int k = 0; k < mesh->vertices[g].count; k++)
				{
					md5_weight_t* pWeight = &mesh->weights[mesh->vertices[g].start + k];
					md5_joint_t* pJoint = &md5file.baseSkel[pWeight->joint];

					vec3_t norm;
					norm[0] = bindposnorms[g][0], norm[1] = bindposnorms[g][1], norm[2] = bindposnorms[g][2];

					quat4_t invRot;
					Quat_inverse(pJoint->orient, invRot);
					
					vec3_t newnorm;
					Quat_rotatePoint(invRot, norm, newnorm);

					pWeight->norm[0] += newnorm[0];
					pWeight->norm[1] += newnorm[1];
					pWeight->norm[2] += newnorm[2];
				}
			}

			for(int k = 0; k < mesh->num_weights; k++)
			{
				Vec_normalize(mesh->weights[k].norm);
			}
		}
		free(bindposnorms);*/
	}
};