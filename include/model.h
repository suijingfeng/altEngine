#include "include.h"

#ifndef MODEL_H
#define MODEL_H

class Model
{
public:
	Model(Entity *entity);
	Model();
	virtual ~Model();
	static void CreateObjects(Graphics &gfx);
	void calc_frustum(matrix4 &clip);
	void load(Graphics &gfx, char *file);
	void create_box(Graphics &gfx, vec3 *aabb);
	virtual float *get_matrix(float *matrix);
	void render(Graphics &gfx);
	void render_box(Graphics &gfx);
	void clone(Model &model);
	void getForwardVector(vec3 &forward);
	virtual void get_frame(Frame &frame);
	void make_aabb();

	Entity			*entity;

	int				num_vertex;
	int				num_index;
	vertex_t		*model_vertex_array;
	unsigned int	*model_index_array;
	char			*model_file;
	char			*index_file;
	int				model_vertex;
	int				model_index;
	int				model_tex;
	int				normal_tex;

	//quaternion		orientation;
	matrix3			morientation;
	vec3			aabb[8];
	int				box_vertex;
	int				box_index;
	vec3			center;
	vec4			frustum[6];

	static			int quad_vertex;
	static			int quad_index;

	char name[128];



	struct flag
	{
		unsigned char blend : 1,
			cull_none : 1,
			rail_trail : 1,
			lightning_trail : 1,
			pad : 4;
	} flags;

};

#endif
