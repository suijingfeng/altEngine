#include "include.h"

#ifndef MD5_H
#define MD5_H

typedef struct anim_list_s
{
	char name[128];
	struct md5_anim_t *anim;
	struct anim_list_s *next;
} anim_list_t;

class MD5
{
public:
	MD5()
	{
		model = new md5_model_t;
	}

	~MD5()
	{
		delete [] model->joint;
		for(int i = 0; i < model->num_mesh; i++)
		{
			delete [] model->mesh[i].triangle;
			delete [] model->mesh[i].vertex;
			delete [] model->mesh[i].weight;
		}
		delete [] model->mesh;
		delete model;
	}

	int load_md5(char *file);
	int load_md5_animation(char *file, anim_list_t *plist);

	int parse_joint(char *data, md5_joint_t *joint, int num_joint);
	int parse_mesh(char *data, md5_mesh_t *mesh);
	int parse_hierarchy(char *data, int num_joint, md5_hierarchy_t *hierarchy);
	int parse_bounds(char *data, int num_bound, md5_aabb_t *aabb);
	int parse_base(char *data, int num_base, md5_base_t *base);
	int parse_frame(char *data, int num_frame, int num_ani, float *frame);


	void InterpolateSkeletons(const md5_joint_t *skelA, const md5_joint_t *skelB, int num_joints, float interp, md5_joint_t *out);
	void PrepareMesh(int mesh_index, md5_joint_t *skeleton, int &num_index, int *index_array, vertex_t *vertex_array, int &num_vertex);

	void generate_animation(md5_joint_t **&frame, md5_anim_t *anim);
	void destroy_animation(md5_joint_t **&frame, md5_anim_t *anim);
	void generate_tangent(int *index_array, int num_index, vertex_t *vertex_array, int num_vertex);
	void calc_tangent(vertex_t &a, vertex_t &b, vertex_t &c);
	void build_frame(md5_joint_t *joint, float *frame, md5_anim_t *anim);

	vertex_t	vertex_array[16][2048];
	int			index_array[16][2048];
	int			num_index[16];
	int			num_vertex[16];

	md5_model_t *model;
private:
};

#endif