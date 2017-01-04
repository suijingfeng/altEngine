#include "include.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <algorithm>


Bsp::Bsp()
{
	loaded = false;
	textures_loaded = false;
	memset(map_name, 0, 80);
	sky_face = -1;
}


bool Bsp::load(char *map, char **pk3list, int num_pk3)
{
//	tBsp = (bsp_t *)get_file(map);
	for (int i = 0; i < num_pk3; i++)
	{
		get_zipfile(pk3list[i], map, (unsigned char **)&tBsp, NULL);
		if (tBsp != NULL)
			break;
	}

	if (tBsp == NULL)
	{
		printf("Unable to open map %s\n", map);
		return false;
	}

	byte *pBsp = (byte *)tBsp;

	// init data
	data.Vert = (bspvertex_t *)	&pBsp[tBsp->directory[VertexArray].offset];
	data.Ent = (byte *)			&pBsp[tBsp->directory[Entities].offset];
	data.Material = (material_t *)&pBsp[tBsp->directory[Materials].offset];
	data.Plane = (plane_t *)	&pBsp[tBsp->directory[Planes].offset];
	data.Node = (node_t *)		&pBsp[tBsp->directory[Nodes].offset];
	data.Leaf = (leaf_t *)		&pBsp[tBsp->directory[Leafs].offset];
	data.LeafFace = (int *)		&pBsp[tBsp->directory[LeafFaces].offset];
	data.LeafBrush = (int *)	&pBsp[tBsp->directory[LeafBrushes].offset];
	data.Model = (model_t *)&pBsp[tBsp->directory[Models].offset];
	data.Brushes = (brush_t *)	&pBsp[tBsp->directory[Brushes].offset];
	data.BrushSides = (brushSide_t *)	&pBsp[tBsp->directory[BrushSides].offset];
	data.Face = (face_t *)		&pBsp[tBsp->directory[Faces].offset];
	data.VisData = (visData_t *)&pBsp[tBsp->directory[VisData].offset];
	data.IndexArray = (int *)		&pBsp[tBsp->directory[IndexArray].offset];
	data.LightMaps = (lightmap_t *)	&pBsp[tBsp->directory[LightMaps].offset];
	data.Fog = (fog_t *) &pBsp[tBsp->directory[Fog].offset];

	data.num_verts = tBsp->directory[VertexArray].length / sizeof(bspvertex_t);
	data.num_ents = tBsp->directory[Entities].length;
	data.num_materials = tBsp->directory[Materials].length / sizeof(material_t);
	data.num_planes = tBsp->directory[Planes].length / sizeof(plane_t);
	data.num_nodes = tBsp->directory[Nodes].length / sizeof(node_t);
	data.num_leafs = tBsp->directory[Leafs].length / sizeof(leaf_t);
	data.num_LeafFaces = tBsp->directory[LeafFaces].length / sizeof(int);
	data.num_LeafBrushes = tBsp->directory[LeafBrushes].length / sizeof(int);
	data.num_LeafBrushes = tBsp->directory[LeafBrushes].length / sizeof(int);
	data.num_model = tBsp->directory[Models].length / sizeof(model_t);
	data.num_brushes = tBsp->directory[Brushes].length / sizeof(brush_t);
	data.num_BrushSides = tBsp->directory[BrushSides].length / sizeof(brushSide_t);
	data.num_faces = tBsp->directory[Faces].length / sizeof(face_t);
	data.num_vis = tBsp->directory[VisData].length / sizeof(visData_t);
	data.num_index = tBsp->directory[IndexArray].length / sizeof(int);
	data.num_lightmaps = tBsp->directory[LightMaps].length / sizeof(lightmap_t);
	data.num_fog = tBsp->directory[Fog].length / sizeof(fog_t);

	change_axis();

	face_to_patch = new int[data.num_faces];
	for (unsigned int i = 0; i < data.num_faces; i++)
	{
		face_to_patch[i] = -1;
	}


	tangent = new vec4 [data.num_verts];
	memset(tangent, 0, sizeof(vec4) * data.num_verts);
//	CalculateTangentArray(data.Vert, data.num_verts, data.IndexArray, data.num_index, tangent);

	tex_object = new texture_t [data.num_materials];
	lightmap_object = new int [data.num_lightmaps];
	normal_object = new int [data.num_materials];

	for (unsigned int i = 0; i < data.num_materials; i++)
	{
		for (int j = 0; j < MAX_TEXTURES; j++)
		{
			tex_object[i].texObj[j] = (unsigned int)0; // zero deselects a texture
			tex_object[i].texObjAnim[j] = 0;
		}
		tex_object[i].num_anim = 0;
		tex_object[i].num_tex = 0;
		tex_object[i].index = (unsigned int)-1;
//		tex_object[i].stage = (unsigned int)-1;
	}
	for(unsigned int i = 0; i < data.num_lightmaps; i++)
		lightmap_object[i] = (unsigned int)-1;
	for(unsigned int i = 0; i < data.num_materials; i++)
		normal_object[i] = (unsigned int)-1;

	loaded = true;
	memcpy(map_name, map, strlen(map) + 1);
	return true;
}

void get_control_points(bspvertex_t *cp, const bspvertex_t *data, int set, int width, int height)
{
	int x = 0;
	int y = 0;

	int x_shift = (1 + (width - 3) / 2);
	int y_shift = (1 + (height - 3) / 2);


	for (y = 0; y < y_shift; y++)
	{
		for (x = 0; x < x_shift; x += 1)
		{
			const bspvertex_t *box = &data[x + y * width];

			cp[0] = box[x + 0 + (y + 0) * width];
			cp[1] = box[x + 1 + (y + 0) * width];
			cp[2] = box[x + 2 + (y + 0) * width];
			cp[3] = box[x + 0 + (y + 1) * width];
			cp[4] = box[x + 1 + (y + 1) * width];
			cp[5] = box[x + 2 + (y + 1) * width];
			cp[6] = box[x + 0 + (y + 2) * width];
			cp[7] = box[x + 1 + (y + 2) * width];
			cp[8] = box[x + 2 + (y + 2) * width];
			set--;

			if (set <= 0)
				return;
		}
		x = 0;
	}
}

/*
	Loops through map data to find bezeir patches to tessellate into verticies
	And now also creates vbos for map and meshes
*/
void Bsp::generate_meshes(Graphics &gfx)
{
	int mesh_index = 0;

	num_meshes = 0;
	mesh_level = 8;

	// Find number  of 3x3 patches
	printf("quadratic_bezier_surface dimensions for %s: ", map_name);
	for (unsigned int i = 0; i < data.num_faces; i++)
	{
		face_t *face = &data.Face[i];

		if (face->type == 2)
		{
			int num_patch = (1 + (face->patchWidth - 3) / 2) * (1 + (face->patchHeight - 3) / 2);
			num_meshes += num_patch; // number of 3x3 meshes
			printf("%dx%d, ", face->patchWidth, face->patchHeight);
		}
	}
	printf("\n");

	patchdata = new patch_t[num_meshes];


	// Generate buffer objects for 3x3 patches given NxN control points
	for (unsigned int i = 0; i < data.num_faces; i++)
	{
		face_t *face = &data.Face[i];
		bspvertex_t controlpoint[9];

		if (face->type == 2)
		{
			int num_patch = (1 + (face->patchWidth - 3) / 2) * (1 + (face->patchHeight - 3) / 2);

			for (int j = 0; j < num_patch; j++)
			{
				get_control_points(controlpoint, &data.Vert[face->vertex], j + 1, face->patchWidth, face->patchHeight);

				patchdata[mesh_index].num_mesh = num_patch;
				patchdata[mesh_index].facevert = face->vertex;

				//tessellate_quadratic_bezier_surface(control, patchdata[mesh_index].vertex_array, patchdata[mesh_index].index_array, patchdata[mesh_index].num_verts, patchdata[mesh_index].num_indexes, mesh_level);
				tessellate(mesh_level, controlpoint, &patchdata[mesh_index].vertex_array, patchdata[mesh_index].num_verts, &patchdata[mesh_index].index_array, patchdata[mesh_index].num_indexes, data.Vert[face->vertex].texCoord0);
				patchdata[mesh_index].vbo = gfx.CreateVertexBuffer(patchdata[mesh_index].vertex_array, patchdata[mesh_index].num_verts);
				patchdata[mesh_index].ibo = gfx.CreateIndexBuffer(patchdata[mesh_index].index_array, patchdata[mesh_index].num_indexes);
				delete[] patchdata[mesh_index].vertex_array;
				delete[] patchdata[mesh_index].index_array;
				mesh_index++;
			}
		}
	}

	vertex = new vertex_t [data.num_verts];
	CreateTangentArray(vertex, data.Vert, data.num_verts, tangent);

	map_vertex_vbo = gfx.CreateVertexBuffer(vertex, data.num_verts);
	map_index_vbo = gfx.CreateIndexBuffer(data.IndexArray, data.num_index);
}

/*
	We need to stick tangent vector into old bsp datatype
*/
void Bsp::CreateTangentArray(vertex_t *vertex_out, bspvertex_t *bsp_vertex, int num_vert, vec4 *tangent_in)
{
	for(int i = 0; i < num_vert; i++)
	{

		vertex_out[i].position = bsp_vertex[i].position;
		vertex_out[i].texCoord0 = bsp_vertex[i].texCoord0;
		vertex_out[i].texCoord1 = bsp_vertex[i].texCoord1;
		vertex_out[i].normal = bsp_vertex[i].normal;
		vertex_out[i].color = bsp_vertex[i].color;
		vertex_out[i].tangent = tangent_in[i];
	}
}

/*
	Converts axis from quake3 to opengl format
*/
void Bsp::change_axis()
{
	for(unsigned int i = 0; i < data.num_verts; i++)
	{
		bspvertex_t *vert = &data.Vert[i];
		float temp;

		temp = vert->position.y;
		vert->position.y = vert->position.z;
		vert->position.z =  -temp;

		temp = vert->normal.y;
		vert->normal.y =  vert->normal.z;
		vert->normal.z =  -temp;
		vert->texCoord0.y = -vert->texCoord0.y;
		vert->texCoord1.y = -vert->texCoord1.y;


//		vert->vPosition *= (1.0f / UNITS_TO_METERS);
	}


//	data.Plane[plane_index].normal
	for(unsigned int i = 0; i < data.num_planes; i++)
	{
		plane_t *plane = &data.Plane[i];
		float	temp;

		temp = plane->normal.y;
		plane->normal.y = plane->normal.z;
		plane->normal.z =  -temp;

//		plane->d *= (1.0f / UNITS_TO_METERS);
	}

	for(unsigned int i = 0; i < data.num_leafs; i++)
	{
		int temp = data.Leaf[i].min[1];
		data.Leaf[i].min[1] = data.Leaf[i].min[2];
		data.Leaf[i].min[2] = -temp;

		temp = data.Leaf[i].max[1];
		data.Leaf[i].max[1] = data.Leaf[i].max[2];
		data.Leaf[i].max[2] = -temp;
	}

	for(unsigned int i = 0; i < data.num_nodes; i++)
	{
		int temp = data.Node[i].min[1];
		data.Node[i].min[1] = data.Node[i].min[2];
		data.Node[i].min[2] = -temp;

		temp = data.Node[i].max[1];
		data.Node[i].max[1] = data.Node[i].max[2];
		data.Node[i].max[2] = -temp;
	}

	for (unsigned int i = 0; i < data.num_model; i++)
	{
		float temp = data.Model[i].min[1];
		data.Model[i].min[1] = data.Model[i].min[2];
		data.Model[i].min[2] = -temp;

		temp = data.Model[i].max[1];
		data.Model[i].max[1] = data.Model[i].max[2];
		data.Model[i].max[2] = -temp;
	}
}

/*
	Returns entity string for parsing
*/
const char *Bsp::get_entities()
{
	return (const char *)data.Ent;
}

/*
	Should cleanly unload a map
*/
void Bsp::unload(Graphics &gfx)
{
	//int mesh_index = 0;

	loaded = false;
	textures_loaded = false;

	anim_list.clear();

	delete[] face_to_patch;

	if (vertex != NULL)
	{
		delete [] vertex;
		vertex = NULL;
	}


	delete[] patchdata;


	gfx.DeleteIndexBuffer(map_index_vbo);
	map_index_vbo = 0;
	gfx.DeleteVertexBuffer(map_vertex_vbo);
	map_vertex_vbo = 0;
	mesh_level = 0;
	num_meshes = 0;

	//Todo, try to find a way to keep loaded textures between map loads
	for(unsigned int i = 0; i < data.num_materials; i++)
	{
		for(int j = 0; j < tex_object->num_tex; j++)
		{
			if ( tex_object[i].texObj[j] )
			{
				gfx.DeleteTexture(tex_object[i].texObj[j]);
				tex_object[i].texObj[j] = 0;
			}
		}
		tex_object[i].index = -1;
		

		if (normal_object[i] != -1)
		{
			gfx.DeleteTexture(normal_object[i]);
			normal_object[i] = -1;
		}
	}
	delete [] tex_object;
	delete [] normal_object;
	tex_object = NULL;
	normal_object = NULL;

	for(unsigned int i = 0; i < data.num_lightmaps; i++)
	{
		if (lightmap_object[i] != -1)
		{
			gfx.DeleteTexture(lightmap_object[i]);
			lightmap_object[i] = -1;
		}
	}
	delete [] lightmap_object;
	lightmap_object = NULL;

	// raw lump file structure
	delete [] tBsp;
	tBsp = NULL;

	// high level data pointers into lump
	memset((void *)&data, 0, sizeof(bspData_t));

	delete [] tangent;

	loaded = false;
}

/*
	Given a position return the bsp leaf node containing that point
*/
inline int Bsp::find_leaf(const vec3 &position)
{
	float	distance;
	int		i = 0;

	while (i >= 0)
	{
		node_t *node = &data.Node[i];
		plane_t *plane = &data.Plane[node->plane];

		distance = (plane->normal * position) - plane->d;

		if (distance >= 0)	
			i = data.Node[i].front;
		else
			i = data.Node[i].back;
	}
	return -(i + 1);
}

void Bsp::sort_leaf(vector<int> *leaf_list, int node_index, const vec3 &position)
{
	if (node_index < 0)
	{
		leaf_list->push_back( -(node_index + 1) );
		return;
	}

	node_t *node = &data.Node[node_index];
	plane_t *plane = &data.Plane[node->plane];

	float	distance = (plane->normal * position) - plane->d;
	if (distance >= 0)
	{
		sort_leaf(leaf_list, data.Node[node_index].front, position);
		sort_leaf(leaf_list, data.Node[node_index].back, position);
	}
	else
	{
		sort_leaf(leaf_list, data.Node[node_index].back, position);
		sort_leaf(leaf_list, data.Node[node_index].front, position);
	}
}

bool Bsp::collision_detect(vec3 &point, plane_t *plane, float *depth, bool &water, float &water_depth, vector<surface_t *> &surface_list, bool debug, vec3 &clip, vec3 &velocity)
{
	int leaf_index = find_leaf(point);
	leaf_t *leaf = &data.Leaf[leaf_index];

	water = false;
	water_depth = 2048.0f;

	if (leaf_index == -1)
	{
		// Outside of map
		return true;
	}



	// A leaf is a convex volume of open space divided from the other leafs by brushes and bsp planes

	// A brush is usually a solid block usually defined by 6 planes, but can have more / less planes
	// (can be any shape really, but think floors walls etc, must be convex)
	// Can be non solid if majority of planes are non solid (eg: water/fog are nonsolid brushes
	// Each face of the brush has a texture, non visible faces (common/caulk etc) are removed for rendering

	for (int i = 0; i < leaf->num_brushes; i++)
	{
		int *index = &data.LeafBrush[leaf->leaf_brush + i];
		brush_t	*brush = &data.Brushes[*index];
		int brush_index = brush->first_side;
		int num_sides = brush->num_sides;
		int count = 0;


		// Let water pass through for water flag
		if ((data.Material[brush->material].contents & CONTENTS_WATER) == 0)
		{
			// Ignore non solid brushes
			if ((data.Material[brush->material].contents & CONTENTS_SOLID) == 0)
				continue;
		}

		for( int j = 0; j < num_sides; j++)
		{
			brushSide_t *brushSide = &data.BrushSides[brush_index + j];
			int plane_index = brushSide->plane;

			float d = point * data.Plane[plane_index].normal - data.Plane[plane_index].d;

			// outside of brush plane
			if (d > 0.0f)
				continue;

			// Inside a brush
			if (data.Material[brush->material].contents & CONTENTS_WATER)
			{
				// Set underwater flag + depth
				water = true;
				water_depth = -d;
//				printf("underwater depth = %f\n", d);
				continue;
			}

			// Ignore individual non solid surfaces (had to move lower to allow water)
			if (data.Material[brush->material].surface & SURF_NONSOLID)
				continue;

			plane->normal = data.Plane[plane_index].normal;
			plane->d = data.Plane[plane_index].d;
			*depth = d;
			count++;


			if (strcmp(data.Material[brush->material].name, "textures/common/weapclip") != 0)
			{
				//one is enough for almost everything except for some weird brushes
				if (debug)
				{
					printf("Inside brush %d with texture %s and contents 0x%X surf 0x%X\nDepth is %3.3f count is %d\n", i,
						data.Material[brush->material].name,
						data.Material[brush->material].contents,
						data.Material[brush->material].surface,
						*depth, count);
					printf("Normal %f %f %f\nVelocity %f %f %f\nClip %f %f %f\n", plane->normal.x, plane->normal.y, plane->normal.z,
						velocity.x, velocity.y, velocity.z, clip.x, clip.y, clip.z);

				}


//				clip = (plane->normal.normalize() * -(velocity * plane->normal.normalize())) * 0.01;
				return true;
			}
		}

		if (count == num_sides)
		{
			if (debug)
			{
				printf("Inside brush %d with texture %s and contents 0x%X surf 0x%X\nDepth is %3.3f count is %d\n", i,
					data.Material[brush->material].name,
					data.Material[brush->material].contents,
					data.Material[brush->material].surface,
					*depth, count);
				/*
				for (unsigned int k = 0; k < surface_list.size(); k++)
				{
					if (strcmp(data.Material[brush->material].name, surface_list[k]->name))
					{
						printf("surfaceparm_noimpact %d\n", (int)surface_list[k]->surfaceparm_noimpact);
						break;
					}
				}
				*/
			}
			return true;
		}
	}

	return false;
}




void Bsp::render_sky(Graphics &gfx, mLight2 &mlight2, int tick_num, vector<surface_t *> surface_list)
{
	float time = (float)tick_num / TICK_RATE;

	if (sky_face == -1)
		return;

	gfx.SelectVertexBuffer(Model::skybox_vertex);
	gfx.SelectIndexBuffer(Model::skybox_index);


	if (textures_loaded)
	{
		static vec2 scroll(0.0f, 0.0f);
		for (int i = 0; i < MAX_TEXTURES; i++)
		{
			int surface_index = tex_object[data.Face[sky_face].material].index;

			if (surface_list[surface_index]->stage[i].tcmod_rotate)
			{
				mlight2.tcmod_rotate(surface_list[surface_index]->stage[i].tcmod_rotate_value * time, i);
			}
			if (surface_list[surface_index]->stage[i].tcmod_scroll)
			{
				scroll.x += surface_list[surface_index]->stage[i].tcmod_scroll_value.x * time * 0.01f;
				scroll.y += surface_list[surface_index]->stage[i].tcmod_scroll_value.y * time * 0.01f;
				mlight2.tcmod_scroll(scroll, i);
			}
			if (surface_list[surface_index]->stage[i].tcmod_scale)
			{
				mlight2.tcmod_scale(surface_list[surface_index]->stage[i].tcmod_scale_value, i);
			}
			
			gfx.SelectTexture(i, tex_object[data.Face[sky_face].material].texObj[i]);
		}
	}

	gfx.DrawArrayTri(0, 0, 36, 36);
}


inline void Bsp::render_face(face_t *face, Graphics &gfx)
{
	gfx.SelectVertexBuffer(map_vertex_vbo);
	gfx.SelectIndexBuffer(map_index_vbo);

	if (textures_loaded)
	{
		for (int i = 0; i < MAX_TEXTURES; i++)
		{
			gfx.SelectTexture(i, tex_object[face->material].texObj[i]);
		}
	}
#ifdef LIGHTMAP
	// surfaces that arent lit with lightmaps eg: skies
	if (face->lightmap != -1)
		gfx.SelectTexture(8, lightmap_object[face->lightmap]);
#endif
#ifdef NORMALMAP
	gfx.SelectTexture(9, normal_object[face->material]);
#endif
	gfx.DrawArrayTri(face->index, face->vertex, face->num_index, face->num_verts);
}

inline void Bsp::render_patch(face_t *face, Graphics &gfx)
{
	int mesh_index = -1;
	int index_per_row = 2 * (mesh_level + 1);

	// Find pre-generated vertex data for patch O(n)

	mesh_index = face_to_patch[face->vertex];
	if (mesh_index == -1)
	{
		for (int i = 0; i < num_meshes; i++)
		{
			if (patchdata[i].facevert == face->vertex)
			{
				mesh_index = i;
				face_to_patch[face->vertex] = i;
				break;
			}
		}
	}


	// will be same texture for all 3x3 patches making up this mesh
	if (textures_loaded)
	{
		for (int j = 0; j < MAX_TEXTURES; j++)
			gfx.SelectTexture(j, tex_object[face->material].texObj[j]);
	}


	for (int i = 0; i < patchdata[mesh_index].num_mesh; i++)
	{
		gfx.SelectVertexBuffer(patchdata[mesh_index + i].vbo);
		gfx.SelectIndexBuffer(patchdata[mesh_index + i].ibo);

#ifdef LIGHTMAP
		gfx.SelectTexture(8, lightmap_object[face->lightmap]);
#endif
#ifdef NORMALMAP
		gfx.SelectTexture(9, normal_object[face->material]);
#endif

		// Rendered row by row because tessellate leaves a degenerate triangles at row ends
		for( int row = 0; row < mesh_level; row++)
		{
			gfx.DrawArrayTriStrip(row * index_per_row, 0,
				index_per_row, patchdata[mesh_index + i].num_verts);
		}
		
		/*
		tessellate_quadratic_bezier_surface could do 3x3 rendering in one pass
		Then we could probably put all the 3x3 patches for a mesh into a single ibo/vbo
		*/
		//gfx.DrawArrayTri(0, 0,
		//patchdata[mesh_index + i].num_indexes,
		//patchdata[mesh_index + i].num_verts);
		
	}
}

inline void Bsp::render_billboard(face_t *face, Graphics &gfx)
{
	if (textures_loaded)
	{
		for (int i = 0; i < MAX_TEXTURES; i++)
		{
			gfx.SelectTexture(i, tex_object[face->material].texObj[i]);
		}
	}
#ifdef NORMALMAP
	gfx.SelectTexture(9, normal_object[face->material]);
#endif
	gfx.SelectIndexBuffer(Model::quad_index);
	gfx.SelectVertexBuffer(Model::quad_vertex);
	gfx.DrawArrayTri(0, 0, 6, 4);
}


bool compare(const faceinfo_t &a, const faceinfo_t &b)
{
	return a.name < b.name;
}

void Bsp::render(vec3 &position, matrix4 &mvp, Graphics &gfx, vector<surface_t *> &surface_list, mLight2 &mlight2, int tick_num)
{
	int frameIndex = find_leaf(position);
	static int lastIndex = -2;
	vec2 zero(0.0f, 0.0f);
	vec2 one(1.0f, 1.0f);
	float time = ((float)tick_num / TICK_RATE);

	leaf_t *frameLeaf = &data.Leaf[frameIndex];

	if (frameIndex != lastIndex || tick_num < TICK_RATE)
	{
		lastIndex = frameIndex;
		blend_list.clear();
		face_list.clear();

		// walk bsp tree, sort leafs front to back
//		vector<int> leaf_list;
//		sort_leaf(&leaf_list, 0, position);

		// loop through all leaves, checking if leaf visible from current leaf
		for (unsigned int i = 0; i < data.num_leafs; i++)
		{
			leaf_t *leaf = &data.Leaf[i];

			if (cluster_visible(frameLeaf->cluster, leaf->cluster) == false)
				continue;

			

			/*
			vec3 min((float)leaf->min[0], (float)leaf->min[1], (float)leaf->min[2]);
			vec3 max((float)leaf->max[0], (float)leaf->max[1], (float)leaf->max[2]);
			if (aabb_visible(min, max, mvp) == false)
				continue;
			*/
			

			// generate face lists
			for (int j = 0; j < leaf->num_faces; j++)
			{
				int face_index = data.LeafFace[leaf->leaf_face + j];
				face_t *face = &data.Face[face_index];

//				int stage = tex_object[face->material].stage;

				if (tex_object[face->material].index != -1)
				{
					// Texture with a shader
					surface_t *surface = surface_list[tex_object[face->material].index];
					bool blend = false;
					faceinfo_t render;

					//dont memset, forces restart of scrolling shaders between pvs updates
//					memset(&render, 0, sizeof(faceinfo_t));
					render.face = face_index;
					render.shader = true;
					render.name = face->material;

					if (surface->surfaceparm_sky)
					{
						render.sky = true;
						sky_face = face_index;
					}

					for (unsigned int k = 0; k < surface->num_stage; k++)
					{
						render.tcmod_rotate[k] = surface->stage[k].tcmod_rotate;
						render.deg[k] = surface->stage[k].tcmod_rotate_value;
						render.tcmod_scroll[k] = surface->stage[k].tcmod_scroll;
						render.scroll[k] = surface->stage[k].tcmod_scroll_value;
						render.tcmod_scale[k] = surface->stage[k].tcmod_scale;
						render.scale[k] = surface->stage[k].tcmod_scale_value;
						render.tcmod_stretch_sin[k] = surface->stage[k].tcmod_stretch_sin;
						render.tcmod_stretch_square[k] = surface->stage[k].tcmod_stretch_square;
						render.tcmod_stretch_triangle[k] = surface->stage[k].tcmod_stretch_triangle;
						render.tcmod_stretch_sawtooth[k] = surface->stage[k].tcmod_stretch_sawtooth;
						render.tcmod_stretch_inverse_sawtooth[k] = surface->stage[k].tcmod_stretch_inverse_sawtooth;
						render.stretch_value[k] = surface->stage[k].tcmod_stretch_value;


						if (surface->stage[k].blendfunc_blend ||
							surface->stage[k].blendfunc_add ||
							surface->stage[k].blendfunc_filter ||
							surface->stage[k].blend_one_one ||
							surface->stage[k].blend_one_zero //||
							/*surface->surfaceparm_fog*/)
						{
							blend = true;
						}

						
						/*
						if (surface->cull_none || surface->cull_disable || surface->cull_twosided)
						{
							render.cull_none = true;
						}
						else
						{
							render.cull_none = false;
						}
						*/
					}


					if (blend)
						blend_list.push_back(render);
					else
						face_list.push_back(render);
				}
				else
				{
					// Texture without a shader
					faceinfo_t render;

//					memset(&render, 0, sizeof(faceinfo_t));
					render.face = face_index;
					render.shader = false;
					
					// need a way to tell if a face should be blended
					if (tex_object[face->material].texObj[0] < 0)
						blend_list.push_back(render);
					else
						face_list.push_back(render);
				}
			}
		}
//		leaf_list.clear();
	}

//	std::sort(face_list.begin(), face_list.end(), compare);
//	std::sort(blend_list.begin(), blend_list.end(), compare);


	for (unsigned int i = 0; i < face_list.size(); i++)
	{
		face_t *face = &data.Face[face_list[i].face];

		if (face_list[i].shader)
		{
			for (int j = 0; j < MAX_TEXTURES; j++)
			{
				if (face_list[i].tcmod_rotate[j])
				{
					mlight2.tcmod_rotate(face_list[i].deg[j] * time, j);
				}
				if (face_list[i].tcmod_scroll[j])
				{
					face_list[i].scroll_value[j].x += face_list[i].scroll[j].x * time * 0.01f;
					face_list[i].scroll_value[j].y += face_list[i].scroll[j].y * time * 0.01f;
					mlight2.tcmod_scroll(face_list[i].scroll_value[j], j);
				}
				if (face_list[i].tcmod_scale[j])
				{
					mlight2.tcmod_scale(face_list[i].scale[j], j);
				}
				if (face_list[i].tcmod_stretch_sin[j])
				{
					mlight2.tcmod_stretch_sin(face_list[i].stretch_value[j].x,
						face_list[i].stretch_value[j].y,
						face_list[i].stretch_value[j].z,
						tick_num, j);
				}
				if (face_list[i].tcmod_stretch_square[j])
				{
					mlight2.tcmod_stretch_square(face_list[i].stretch_value[j].x,
						face_list[i].stretch_value[j].y,
						face_list[i].stretch_value[j].z,
						tick_num, j);
				}
				if (face_list[i].tcmod_stretch_triangle[j])
				{
					mlight2.tcmod_stretch_square(face_list[i].stretch_value[j].x,
						face_list[i].stretch_value[j].y,
						face_list[i].stretch_value[j].z,
						tick_num, j);
				}
				if (face_list[i].tcmod_stretch_sawtooth[j])
				{
					mlight2.tcmod_stretch_square(face_list[i].stretch_value[j].x,
						face_list[i].stretch_value[j].y,
						face_list[i].stretch_value[j].z,
						tick_num, j);
				}
				if (face_list[i].tcmod_stretch_inverse_sawtooth[j])
				{
					mlight2.tcmod_stretch_square(face_list[i].stretch_value[j].x,
						face_list[i].stretch_value[j].y,
						face_list[i].stretch_value[j].z,
						tick_num, j);
				}
			}
		}

		if (face_list[i].sky)
		{
			for (int j = 0; j < MAX_TEXTURES; j++)
			{
				//technically shouldnt need to reset
				mlight2.tcmod_rotate(0, j);
				mlight2.tcmod_scroll(zero, j);
				mlight2.tcmod_scale(one, j);
			}
			//skip sky texture, already drawn
			continue;
		}

		if (face_list[i].cull_none)
			gfx.CullFace(2);
		else
			gfx.CullFace(3);

		if (face->type == 1 || face->type == 3)
		{
			render_face(face, gfx);
		}
		else if (face->type == 2)
		{
			render_patch(face, gfx);
		}
		else// (face->type == 4)
		{
			render_billboard(face, gfx);
		}

		if (face_list[i].shader)
		{
			for (int j = 0; j < MAX_TEXTURES; j++)
			{
				//technically shouldnt need to reset
				mlight2.tcmod_rotate(0, j);
				mlight2.tcmod_scroll(zero, j);
				mlight2.tcmod_scale(one, j);
			}
		}
	}

	if (blend_list.size())
	{
		//	gfx.Depth(false);
		gfx.Blend(true);
	}
	// leaves sorted front to back, render blends back to front
	for (int i = blend_list.size() - 1; i >= 0; i--)
	{
		face_t *face = &data.Face[blend_list[i].face];


		if (blend_list[i].shader)
		{
			for (int j = 0; j < MAX_TEXTURES; j++)
			{
				if (blend_list[i].tcmod_rotate[j])
				{
					mlight2.tcmod_rotate(blend_list[i].deg[j] * time, j);
				}
				if (blend_list[i].tcmod_scroll[j])
				{
					// Note scrolling restarts when index changes due to pvs update
					blend_list[i].scroll_value[j].x += blend_list[i].scroll[j].x * time;
					blend_list[i].scroll_value[j].y += blend_list[i].scroll[j].y * time;
					mlight2.tcmod_scroll(blend_list[i].scroll_value[j], j);
				}
				if (blend_list[i].tcmod_scale[j])
				{
					mlight2.tcmod_scale(blend_list[i].scale[j], j);
				}
				if (blend_list[i].tcmod_stretch_sin[j])
				{
					mlight2.tcmod_stretch_sin(	blend_list[i].stretch_value[j].x,
												blend_list[i].stretch_value[j].y,
												blend_list[i].stretch_value[j].z,
												tick_num, j);
				}
				if (blend_list[i].tcmod_stretch_square[j])
				{
					mlight2.tcmod_stretch_square(blend_list[i].stretch_value[j].x,
						blend_list[i].stretch_value[j].y,
						blend_list[i].stretch_value[j].z,
						tick_num, j);
				}
				if (blend_list[i].tcmod_stretch_triangle[j])
				{
					mlight2.tcmod_stretch_square(blend_list[i].stretch_value[j].x,
						blend_list[i].stretch_value[j].y,
						blend_list[i].stretch_value[j].z,
						tick_num, j);
				}
				if (blend_list[i].tcmod_stretch_sawtooth[j])
				{
					mlight2.tcmod_stretch_square(blend_list[i].stretch_value[j].x,
						blend_list[i].stretch_value[j].y,
						blend_list[i].stretch_value[j].z,
						tick_num, j);
				}
				if (blend_list[i].tcmod_stretch_inverse_sawtooth[j])
				{
					mlight2.tcmod_stretch_square(blend_list[i].stretch_value[j].x,
						blend_list[i].stretch_value[j].y,
						blend_list[i].stretch_value[j].z,
						tick_num, j);
				}
			}
		}

		/*
		if (blend_list[i].sky)
		{
			for (int j = 0; j < MAX_TEXTURES; j++)
			{
				//technically shouldnt need to reset
				mlight2.tcmod_rotate(0, j);
				mlight2.tcmod_scroll(zero, j);
				mlight2.tcmod_scale(one, j);
			}
			continue;
		}
		*/

		if (face->type == 1 || face->type == 3)
		{
			render_face(face, gfx);
		}
		else if (face->type == 2)
		{
			render_patch(face, gfx);
		}
		else// (face->type == 4)
		{
			render_billboard(face, gfx);
		}

		if (blend_list[i].shader)
		{
			for (int j = 0; j < MAX_TEXTURES; j++)
			{
				//technically shouldnt need to reset
				mlight2.tcmod_rotate(0, j);
				mlight2.tcmod_scroll(zero, j);
				mlight2.tcmod_scale(one, j);
			}
		}
	}
	if (blend_list.size())
	{
		//	gfx.Depth(true);
		gfx.Blend(false);
	}

//	draw_box(frameLeaf->mins, frameLeaf->maxs);
}


/*
	This fixes origin for func_door's, func_bobbing, trigger's etc
*/
vec3 Bsp::model_origin(unsigned int index)
{
	if (index >= data.num_model)
	{
		debugf("Invalid model index\n");
		return vec3();
	}
	model_t *model = &data.Model[index];

	vec3 min((float)model->min[0], (float)model->min[1], (float)model->min[2]);
	vec3 max((float)model->max[0], (float)model->max[1], (float)model->max[2]);
	vec3 origin(
		(max.x - min.x) * 0.5f + min.x,
		(max.y - min.y) * 0.5f + min.y,
		(max.z - min.z) * 0.5f + min.z);

	return origin;
}

void Bsp::render_model(unsigned int index, Graphics &gfx)
{
	if (index >= data.num_model)
	{
		debugf("Invalid model index\n");
		return;
	}
	model_t *model = &data.Model[index];

	vec3 min((float)model->min[0], (float)model->min[1], (float)model->min[2]);
	vec3 max((float)model->max[0], (float)model->max[1], (float)model->max[2]);

	for (int i = 0; i < model->num_faces; i++)
	{
		int face_index = model->face_index + i;
		face_t *face = &data.Face[face_index];

		if (face->type == 1 || face->type == 3)
		{
			render_face(face, gfx);
		}
		else if (face->type == 2)
		{
			render_patch(face, gfx);
		}
		else// (face->type == 4)
		{
			render_billboard(face, gfx);
		}
	}
}

/*
	Determines set of visible leafs from current leaf
*/
inline int Bsp::cluster_visible(int vis_cluster, int test_cluster)
{
	int byte_offset, bit_offset;
	char test_byte;

	// if no vis data or if camera leaf invalid (outside map) draw everything
	if (vis_cluster < 0 || data.num_vis == 0)
		return 1;

	// bit of pVecs we need to return
	bit_offset = vis_cluster + test_cluster * data.VisData->vector_size * 8; 
	byte_offset = bit_offset / 8;
	test_byte = 1 << (bit_offset - byte_offset * 8);
	return 	(&data.VisData->pVecs)[byte_offset] & test_byte;
}

void Bsp::load_from_file(char *filename, texture_t &texObj, Graphics &gfx, char **pk3_list, int num_pk3)
{
	char	texture_name[LINE_SIZE] = { 0 };
	int		tex_object = 0;

	//printf("Attempting to load %s, trying .tga\n", filename);
	snprintf(texture_name, LINE_SIZE, "media/%s.tga", filename);
	tex_object = load_texture_pk3(gfx, texture_name, pk3_list, num_pk3, false);

	if (tex_object == 0)
	{
		//printf("Attempting to load %s, trying .jpg\n", filename);
		snprintf(texture_name, LINE_SIZE, "media/%s.jpg", filename);
		tex_object = load_texture_pk3(gfx, texture_name, pk3_list, num_pk3, false);
	}
	if (tex_object != 0)
	{
		//printf("loaded %s into unit %d\n", filename, texObj.num_tex);
		texObj.texObj[texObj.num_tex] = tex_object;
	}

}

void Bsp::load_from_shader(char *name, vector<surface_t *> &surface_list, texture_t *texObj, Graphics &gfx, char **pk3_list, int num_pk3)
{
	char			texture_name[LINE_SIZE + 1];
	int				tex_object = 0;
	unsigned int	j = 0;

	memset(texture_name, 0, sizeof(texture_name));

	//printf("Attempting to load %s, trying surface_list\n", name);
	for (j = 0; j < surface_list.size(); j++)
	{
		if (strcmp(name, surface_list[j]->name) == 0)
		{
			//printf("Found shader [%s], trying stages\n", surface_list[j]->name);
			texObj->index = j;
			break;
		}
	}


	if (j == surface_list.size())
	{
		return;
	}

	for (unsigned int k = 0; k < surface_list[j]->num_stage && k < 4; k++)
	{
		//printf("Raw stage %d is [%s]\n", j, surface_list[i]->stage.stage[j]);
		if (surface_list[j]->stage[k].map /*&& surface_list[j]->stage[k].tcgen_env == false*/)
		{
			snprintf(texture_name, LINE_SIZE, "media/%s", surface_list[j]->stage[k].map_tex);

			//printf("Trying texture [%s]\n", texture_name);
			tex_object = load_texture_pk3(gfx, texture_name, pk3_list, num_pk3, false);
		}
		else if (surface_list[j]->stage[k].clampmap)
		{
			snprintf(texture_name, LINE_SIZE, "media/%s", surface_list[j]->stage[k].clampmap_tex);
			tex_object = load_texture_pk3(gfx, texture_name, pk3_list, num_pk3, true);
		}
		else if (surface_list[j]->stage[k].anim_map)
		{
			char *tex = strtok(surface_list[j]->stage[k].anim_map_tex, " ");
			anim_list.push_back(texObj);
			int n = 0;

			int freq = atoi(tex);
			texObj->freq = freq;

//			printf("animmap freq %d\n", freq);
			tex = strtok(NULL, " ");
			while (tex != NULL)
			{
//				printf("animmap tex %s\n", tex);
				snprintf(texture_name, LINE_SIZE, "media/%s", tex);
				texObj->texObjAnim[n++] = load_texture_pk3(gfx, texture_name, pk3_list, num_pk3, false);
				tex = strtok(NULL, " ");
			}

			texObj->texObj[k] = texObj->texObjAnim[0];
			texObj->anim_unit = k;
			texObj->num_anim = n;
			continue;
		}


		if (tex_object != 0 && tex_object != -1 && surface_list[j]->stage[k].anim_map == false)
		{
			//printf("Loaded texture stage %d into unit %d for shader with texture %s\n", k, texObj->num_tex, texture_name);
			//texObj->stage = k;
			texObj->texObj[k] = tex_object;
			continue;
		}

		if (strlen(texture_name) > 4)
		{
			texture_name[strlen(texture_name) - 4] = '\0';
			strcat(texture_name, ".jpg");
			//printf("Trying jpeg texture [%s]\n", texture_name);
		}
		else
		{
			continue;
		}

		tex_object = load_texture_pk3(gfx, texture_name, pk3_list, num_pk3, false);
		if (tex_object != 0)
		{
			//printf("Loaded texture stage %d for shader with texture %s\n", k, texture_name);
			//texObj->stage = k;
			texObj->texObj[k] = tex_object;
			continue;
		}
	}

	return;
}



void Bsp::load_textures(Graphics &gfx, vector<surface_t *> &surface_list, char **pk3_list, int num_pk3)
{
	textures_loaded = true;
#ifdef LIGHTMAP
	for (unsigned int i = 0; i < data.num_lightmaps; i++)
	{
#ifndef DIRECTX
		lightmap_object[i] = gfx.LoadTexture(128, 128, 3, GL_RGB, (void *)data.LightMaps[i].image, false);
#else
		byte *pBits = tga_24to32(128, 128, (byte *)data.LightMaps[i].image);
		lightmap_object[i] = gfx.LoadTexture(128, 128, 4, 4, (void *)data.LightMaps[i].image, false);
		delete [] pBits;
#endif
	}
#endif

	for (unsigned int i = 0; i < data.num_materials; i++)
	{
		material_t	*material = &data.Material[i];
		
		strcpy(tex_object[i].name, data.Material[i].name);
		load_from_shader(material->name, surface_list, &tex_object[i], gfx, pk3_list, num_pk3);
		if (tex_object[i].texObj[tex_object[i].num_tex] == 0)
		{
			load_from_file(material->name, tex_object[i], gfx, pk3_list, num_pk3);
		}
		
		if (tex_object[i].texObj[tex_object[i].num_tex] == 0)
		{
			//printf("******* Failed to find texture for shader %s texunit %d\n", material->name, tex_object[i].num_tex);
//			tex_object[i].texObj[tex_object[i].num_tex] = 1; // no_tex image
//			tex_object->num_tex++;
			continue;
		}
		tex_object[i].num_tex++;

#ifdef NORMALMAP
		snprintf(texture_name, LINE_SIZE, "media/%s_normal.tga", material->name);
		normal_object[i] = load_texture(gfx, texture_name, false);
#endif
	}
}

/*
	cylindrical patches are a 3x9 set of control points
	U patches are a 3x5 set of control points
	This function assumes it's given 3x3 set of control points
	hacky fix for cylindrical patches and U patches in calling function
*/
void Bsp::tessellate(int level, bspvertex_t control[], vertex_t **vertex_array, int &num_verts, int **index_array, int &num_indexes, vec2 &texcoord)
{
	vec3 a, b;
	int i, j;

	num_verts = level + 1;

	*vertex_array = new vertex_t[num_verts * num_verts];

	// calculate first set of verts
	for (i = 0; i <= level; i++)
	{
		float a2 = (float) i / level;
		float b2 = 1.0f - a2;
		(*vertex_array)[i].position =
			control[0].position * (b2 * b2) +
			control[3].position * (2 * b2 * a2) +
			control[6].position * (a2 * a2);
	}

	// calculate rest of verts
	for ( i = 1; i <= level; i++)
	{
		float a2 = (float)i / level;
		float b2 = 1.0f - a2;

		vertex_t temp[3];

		temp[0].position = 
			control[0].position * (b2 * b2) + 
			control[1].position * (2 * b2 * a2) + 
			control[2].position * (a2 * a2);
		temp[1].position = 
			control[3].position * (b2 * b2) + 
			control[4].position * (2 * b2 * a2) + 
			control[5].position * (a2 * a2);
		temp[2].position = 
			control[6].position * (b2 * b2) + 
			control[7].position * (2 * b2 * a2) + 
			control[8].position * (a2 * a2);

		for(j = 0; j <= level; j++)
		{
			float a3 = (float) j / level;
			float b3 = 1.0f - a3;

			(*vertex_array)[i * num_verts + j].position =
				temp[0].position * (b3 * b3) +
				temp[1].position * (2 * b3 * a3) +
				temp[2].position * (a3 * a3);
		}
	}
	
	//Create index array
	num_indexes = level * num_verts * 2;
	*index_array = new int[num_indexes];
	for (i = 0; i < level; i++)
	{
		for(j = 0; j <= level; j++)
		{
			(*index_array)[(i * num_verts + j) * 2 + 1] = i * num_verts + j;
			(*index_array)[(i * num_verts + j) * 2] = (i + 1) * num_verts + j;
		}
	}

	// Generate normals and tangent
	for(i = 0; i <= level; i++)
	{
		for(j = 0; j <= level; j++)
		{
			if (j != level)
				a = (*vertex_array)[i * num_verts + j].position - (*vertex_array)[i * num_verts + (j + 1)].position;
			else
				a = (*vertex_array)[i * num_verts + j].position - (*vertex_array)[i * num_verts + (j - 1)].position;
				
			if ( i != level)
				b = (*vertex_array)[i * num_verts + j].position - (*vertex_array)[(i + 1) * num_verts + j].position;
			else
				b = (*vertex_array)[i * num_verts + j].position - (*vertex_array)[(i - 1) * num_verts + j].position;

			(*vertex_array)[i * num_verts + j].color = -1;
			(*vertex_array)[i * num_verts + j].texCoord0 = vec2((float)(i % 2), (float)(j % 2));
			(*vertex_array)[i * num_verts + j].texCoord0.x = i * (1.0f / level) + texcoord.x;
			(*vertex_array)[i * num_verts + j].texCoord0.y = j * (-1.0f / level) + texcoord.y;
			(*vertex_array)[i * num_verts + j].tangent.x = a.x;
			(*vertex_array)[i * num_verts + j].tangent.y = a.y;
			(*vertex_array)[i * num_verts + j].tangent.z = a.z;
			(*vertex_array)[i * num_verts + j].tangent.w = 0.0f;

			if (j == level || i == level)
			{
				(*vertex_array)[i * num_verts + j].normal = vec3::crossproduct(a,b).normalize();
			}
			else
			{
				(*vertex_array)[i * num_verts + j].normal = vec3::crossproduct(b,a).normalize();
				(*vertex_array)[i * num_verts + j].tangent.x = b.x;
				(*vertex_array)[i * num_verts + j].tangent.y = b.y;
				(*vertex_array)[i * num_verts + j].tangent.z = b.z;
				(*vertex_array)[i * num_verts + j].tangent.w = 0.0f;
			}

			if (j == level && i == level)
			{
				(*vertex_array)[i * num_verts + j].normal = vec3::crossproduct(b,a).normalize();
				(*vertex_array)[i * num_verts + j].tangent.x = b.x;
				(*vertex_array)[i * num_verts + j].tangent.y = b.y;
				(*vertex_array)[i * num_verts + j].tangent.z = b.z;
				(*vertex_array)[i * num_verts + j].tangent.w = 0.0f;
			}
		}
	}

	// correct numVerts size
	num_verts = num_verts * num_verts;
}

bool Bsp::vis_test(vec3 &x, vec3 &y, int &leaf_a, int &leaf_b)
{
		int a = find_leaf(x);
		int b = find_leaf(y);

		leaf_a = a;
		leaf_b = b;


		if (a == b)
		{
			return true;
		}

		leaf_t *a_leaf = &data.Leaf[a];
		leaf_t *b_leaf = &data.Leaf[b];

		if ( !cluster_visible(a_leaf->cluster, b_leaf->cluster) )
			return false;
		else
			return true;
}

bool Bsp::leaf_test(vec3 &x, vec3 &y)
{
		int a = find_leaf(x);
		int b = find_leaf(y);

		if (a == b)
			return true;
		else
			return false;
}

/*
	Can be done in a geometry shader
*/
void Bsp::CalculateTangentArray(bspvertex_t *vertex, int num_vert, int *index, int num_index, vec4 *tangent)
{
	vec3 *temp_tan = new vec3 [num_vert];
	vec3 *temp_btan = new vec3 [num_vert];

	memset(temp_tan, 0, num_vert * sizeof(vec3));
	memset(temp_btan, 0, num_vert * sizeof(vec3));
	for (int i = 0; i < num_index - 2;)
	{
		int ai = index[i];
		int bi = index[i + 1];
		int ci = index[i + 2];
        
		const vec3 p0 = vertex[ai].position;
		const vec3 p1 = vertex[bi].position;
		const vec3 p2 = vertex[ci].position;

		const vec2 tex0 = vertex[ai].texCoord0;
		const vec2 tex1 = vertex[bi].texCoord0;
		const vec2 tex2 = vertex[ci].texCoord0;
		
		
		vec3 p, q, n;
		vec3 t, b;
		float s1, s2, t1, t2;
		float denom;


		// triangle span vectors
		p = p1 - p0;
		q = p2 - p0;

		//cross product for normal
		n = vec3::crossproduct(p,q);

		// texture coordinate vectors
		s1 = tex1.x - tex0.x;
		t1 = tex1.y - tex0.y;

		s2 = tex2.x - tex0.x;
		t2 = tex2.y - tex0.y;

		// tangent and bitangent
		denom = (s1 * t2 - s2 * t1);
		if (denom != 0)
		{
			t = (p * t2 - q * t1) / denom;
			b = (q * s1 - p * s2) / denom;
		}
		else
		{
			t.x = 1.0f;
			t.y = 0.0f;
			t.z = 0.0f;
			b.x = 0.0f;
			b.y = 0.0f;
			b.z = 1.0f;
		}
		temp_tan[i]    = t;
		temp_tan[i+1]  = t;
		temp_tan[i+2]  = t;
		temp_btan[i]   = b;
		temp_btan[i+1] = b;
		temp_btan[i+2] = b;
	}

	for (int i = 0; i < num_vert; i++)
	{
		tangent[i].x = temp_tan[i].x;
		tangent[i].y = temp_tan[i].y;
		tangent[i].z = temp_tan[i].z;
		tangent[i].w = 1.0f;
	}

	delete[] temp_tan;
	delete[] temp_btan;
}
/*
void Bsp::CalculateTangentArray(bspvertex_t *vertex, int num_vert, int *index, int num_index, vec4 *tangent)
{
	vec3 *temp_tan = new vec3 [num_vert];
	vec3 *temp_btan = new vec3 [num_vert];

	memset(temp_tan, 0, num_vert * sizeof(vec3));
	memset(temp_btan, 0, num_vert * sizeof(vec3));
	for (int i = 0; i < num_index - 2;)
	{
		int a = index[i];
		int b = index[i + 1];
		int c = index[i + 2];
        
		const vec3 p0 = vertex[a].position;
		const vec3 p1 = vertex[b].position;
		const vec3 p2 = vertex[c].position;

		const vec2 tex0 = vertex[a].texCoord0;
		const vec2 tex1 = vertex[b].texCoord0;
		const vec2 tex2 = vertex[c].texCoord0;

		vec3 q1 = p1 - p0;
		vec3 q2 = p2 - p0;

		float s1 = tex1.x - tex0.x;
		float t1 = tex1.y - tex0.y;
		float s2 = tex2.x - tex0.x;
		float t2 = tex2.y - tex0.y;
        
		float denom = s1 * t2 - s2 * t1;

		// singular triangle
		if (denom == 0)
		{
			vec3 f(1.0f, 0.0f, 0.0f);
			vec3 r = vec3::crossproduct(f, vertex[a].normal);
			f = vec3::crossproduct(r, vertex[a].normal);

			temp_tan[a] += f;
			temp_tan[b] += f;
			temp_tan[c] += f;
        
			temp_btan[a] += r;
			temp_btan[b] += r;
			temp_btan[c] += r;
			i += 3;
			continue;
		}

		float r = 1.0f / denom;

		vec3 sdir((t2 * q1.x - t1 * q2.x) * r, (t2 * q1.y - t1 * q2.y) * r, (t2 * q1.z - t1 * q2.z) * r);
		vec3 tdir((s1 * q2.x - s2 * q1.x) * r, (s1 * q2.y - s2 * q1.y) * r, (s1 * q2.z - s2 * q1.z) * r);
		sdir.normalize();
		tdir.normalize();
        
		// adding so vectors are averaged for shared verticies
		temp_tan[a] += sdir;
		temp_tan[b] += sdir;
		temp_tan[c] += sdir;
        
		temp_btan[a] += tdir;
		temp_btan[b] += tdir;
		temp_btan[c] += tdir;
        
		i += 3;
//		vec3 n = vec3::crossproduct(q2,q1).normalize();
//		printf("n = %f %f %f   = %f %f %f\n", n.x, n.y, n.z, vertex[a].normal.x, vertex[a].normal.y, vertex[a].normal.z);
//		printf("t = %f %f %f b = %f %f %f\n", sdir.x, sdir.y, sdir.z, tdir.x, tdir.y, tdir.z);
	}
    
	for (int i = 0; i < num_vert; i++)
	{
		vec3 n = vertex[i].normal;
		vec3 t = temp_tan[i];
		vec3 b = temp_btan[i];
		vec3 vtan;
		vec3 vbtan;
        
		// Gram-Schmidt orthogonalize
		vtan = (t - n * (n * t)).normalize();
		vbtan = (b - n * (n * b)) - vtan * (vtan * b);
        
		// Calculate handedness
		tangent[i].x = vtan.x;
		tangent[i].y = vtan.y;
		tangent[i].z = vtan.z;
		tangent[i].w = (vec3::crossproduct(n, t) * temp_btan[i] < 0.0f) ? -1.0f : 1.0f;
	}

	delete[] temp_tan;
	delete[] temp_btan;
}
*/

/*
Loop through all the model's triangles
If triangle faces the light source (dot product > 0)
Insert the three edges (pair of vertices), into an edge stack
Check for previous occurrence of each edges or it's reverse in the stack
If an edge or its reverse is found in the stack, remove both edges
Start with new triangle
*/
void Bsp::find_edges(vec3 &position, Edge &edge_list)
{
	int leaf_index = find_leaf(position);

	leaf_t *light_Leaf = &data.Leaf[leaf_index];

	// loop through all leaves, checking if leaf visible from current leaf
	for (unsigned int i = 0; i < data.num_leafs; i++)
	{
		leaf_t *leaf = &data.Leaf[i];

		if (!cluster_visible(light_Leaf->cluster, leaf->cluster))
			continue;

		for (int j = 0; j < leaf->num_faces; j++)
		{
			vector<vec3> vertex_list;
			int face_index = data.LeafFace[leaf->leaf_face + j];
			face_t *face = &data.Face[face_index];

			for (int k = 0; k < face->num_index; k++)
			{
				int index = data.IndexArray[face->index + k];
				vec3 x = data.Vert[face->vertex + index].position;
				vertex_list.push_back(x);
			}

			for(unsigned int k = 0; k < vertex_list.size(); k += 3)
			{
				vec3 x = vertex_list[k];
				vec3 y = vertex_list[k + 1];
				vec3 z = vertex_list[k + 2];

				
				vec3 a = y - x;
				vec3 b = z - x;
				vec3 normal = vec3::crossproduct(a, b);

				vec3 lightdir1 = x - position;
				vec3 lightdir2 = y - position;
				vec3 lightdir3 = z - position;
				vec3 lightdir;

				if (lightdir1.magnitude() < lightdir2.magnitude() && lightdir1.magnitude() < lightdir3.magnitude())
					lightdir = lightdir1;
				else if (lightdir2.magnitude() < lightdir1.magnitude() && lightdir2.magnitude() < lightdir3.magnitude())
					lightdir = lightdir2;
				else
					lightdir = lightdir3;

				normal.normalize();
//				if (lightdir.magnitude() > 400.0f)
//					continue;


				if (lightdir * normal > 0)
				{
					vec3 triple[3][2];

					if (x.x < y.x)
					{
						triple[0][0] = x;
						triple[0][1] = y;
					}
					else
					{
						triple[0][1] = x;
						triple[0][0] = y;
					}

					if (x.x < z.x)
					{
						triple[1][0] = x;
						triple[1][1] = z;
					}
					else
					{
						triple[1][1] = x;
						triple[1][0] = z;
					}

					if (y.x < z.x)
					{
						triple[2][0] = y;
						triple[2][1] = z;
					}
					else
					{
						triple[2][1] = y;
						triple[2][0] = z;
					}
					edge_list.insert(&triple[0][0]);
					edge_list.insert(&triple[1][0]);
					edge_list.insert(&triple[2][0]);
				}
			}
		}
	}
}


void Bsp::hitscan(vec3 &origin, vec3 &dir, float &distance)
{
	// Really need to test brush planes not leaf aabbs
	for (unsigned int i = 0; i < data.num_leafs; i++)
	{
		vec3 min((float)data.Leaf[i].min[0], (float)data.Leaf[i].min[1], (float)data.Leaf[i].min[2]);
		vec3 max((float)data.Leaf[i].max[0], (float)data.Leaf[i].max[1], (float)data.Leaf[i].max[2]);

		RayBoxSlab(origin, dir, min, max, distance);
	}
	
}
