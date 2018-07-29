#include "include.h"

#ifndef HLBSP_H
#define HLBSP_H

class HLBsp
{
public:
	HLBsp();
	int load(Graphics &gfx, char *file);
	void render(Graphics &gfx, vec3 &pos);
	void temp_render(Graphics &gfx);


	bool loaded;
	char map_name[128];

private:
	void render_leaf (Graphics &gfx, int leaf);
	void build_face(int face);
	void render_face(Graphics &gfx, int face);

	int find_leaf(vec3 pos, int node);
	void bsp_render_node(Graphics &gfx, int node_index, int leaf, vec3 pos);
	bool point_AABB(vec3 pos, short min[3], short max[3]);
	void change_axis();
	void load_lightmap(Graphics &gfx);
	void load_textures(Graphics &gfx);

	vector <unsigned int> index;
	hl_bsp_t data;

	int ic;
	
	int map_vertex_vbo;
	int map_index_vbo;
	int *lightmap_object;
	int *tex_object;
	vertex_t *map_vertex;
	int *texdata_to_obj;
	bool initialized;
};

#endif