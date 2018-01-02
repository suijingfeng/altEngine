#include "include.h"

HLBsp::HLBsp()
{
	loaded = false;
}

int HLBsp::load(Graphics &gfx, char *map)
{
	int size;
	dheader_t *tBsp = (dheader_t *)get_file(map, &size);
	char *pBsp = (char *)tBsp;

	if (pBsp == NULL)
	{
		debugf("Failed to open %s", map);
		return -1;
	}

	if (tBsp->magic != HL_BSP_HEADER)
	{
		debugf("Not a valid source engine bsp\n");
		return -1;
	}

	// Version should be 20 assuming counter strike maps
	if (tBsp->version < 17 || tBsp->version >= 29)
	{
		debugf("Unexpected BSP Version %X\n", tBsp->version);
		return -1;
	}


	data.header = (dheader_t *)tBsp;
	data.Entity = (char *)&pBsp[tBsp->lumps[LMP_ENTITIES].offset];
	data.Plane = (dplane_t *)&pBsp[tBsp->lumps[LMP_PLANES].offset];
	data.TexData = (dtexdata_t *)&pBsp[tBsp->lumps[LMP_TEXDATA].offset];
	data.Vert = (vec3 *)&pBsp[tBsp->lumps[LMP_VERTICES].offset];
	data.Vis = (dvis_t *)&pBsp[tBsp->lumps[LMP_VISIBILITY].offset];
	data.Node = (dnode_t *)&pBsp[tBsp->lumps[LMP_NODES].offset];
	data.TexInfo = (dtexinfo_t *)&pBsp[tBsp->lumps[LMP_TEXINFO].offset];
	data.Face = (dface_t *)&pBsp[tBsp->lumps[LMP_FACES].offset];
	data.Lightmap = (ColorRGBExp32 *)&pBsp[tBsp->lumps[LMP_LIGHTING].offset];
	// lump 9 missing LMP_OCCLUSION
	data.Leaf = (dleaf_t *)&pBsp[tBsp->lumps[LMP_LEAFS].offset];
	//lump 11 missing LMP_FACEIDS
	data.Edge = (dedge_t *)&pBsp[tBsp->lumps[LMP_EDGES].offset];
	data.SurfEdge = (int *)&pBsp[tBsp->lumps[LMP_SURFEDGES].offset];
	data.Model = (dmodel_t *)&pBsp[tBsp->lumps[LMP_MODELS].offset];
	//lump 15 missing LMP_WORLDLIGHTS
	data.LeafFace = (unsigned short int *)&pBsp[tBsp->lumps[LMP_LEAFFACES].offset];
	data.LeafBrush = (unsigned short int *)&pBsp[tBsp->lumps[LMP_LEAFBRUSHES].offset];
	data.Brush = (dbrush_t *)&pBsp[tBsp->lumps[LMP_BRUSHES].offset];
	data.BrushSide = (dbrushside_t *)&pBsp[tBsp->lumps[LMP_BRUSHSIDES].offset];
	// huge gap
	//LMP_VERTNORMALS Face plane normals 
	//LMP_VERTNORMALINDICES Face plane normal index array 
	//LUMP_PAKFILE Embedded uncompressed Zip-format file 

	data.Game = (dgamelump_t *)&pBsp[tBsp->lumps[LMP_GAME_LUMP].offset];
	data.StringTable = (int *)&pBsp[tBsp->lumps[LMP_TEXDATA_STRING_TABLE].offset];
	data.StringData = (char *)&pBsp[tBsp->lumps[LMP_TEXDATA_STRING_DATA].offset];


	data.num_entity = tBsp->lumps[LMP_ENTITIES].length;;
	data.num_planes = tBsp->lumps[LMP_PLANES].length / sizeof(dplane_t);
	data.num_texdata = tBsp->lumps[LMP_TEXDATA].length / sizeof(dtexdata_t);
	data.num_verts = tBsp->lumps[LMP_VERTICES].length / sizeof(vec3);
	data.num_vis = tBsp->lumps[LMP_VERTICES].length / sizeof(dvis_t);
	data.num_nodes = tBsp->lumps[LMP_NODES].length / sizeof(dnode_t);
	data.num_texinfo = tBsp->lumps[LMP_TEXINFO].length / sizeof(dtexinfo_t);
	data.num_faces = tBsp->lumps[LMP_FACES].length / sizeof(dface_t);
	data.num_lightmap = tBsp->lumps[LMP_MODELS].length / sizeof(ColorRGBExp32);
	data.num_leafs = tBsp->lumps[LMP_LEAFS].offset / sizeof(dleaf_t);
	data.num_edges = tBsp->lumps[LMP_EDGES].length / sizeof(dedge_t);
	data.num_surfedges = tBsp->lumps[LMP_SURFEDGES].length / sizeof(int);
	data.num_model = tBsp->lumps[LMP_MODELS].length / sizeof(dmodel_t);
	data.num_LeafFaces = tBsp->lumps[LMP_LEAFFACES].length / sizeof(unsigned short int);
	data.num_LeafBrush = tBsp->lumps[LMP_LEAFBRUSHES].length / sizeof(unsigned short int);
	data.num_Brush = tBsp->lumps[LMP_BRUSHES].length / sizeof(dbrush_t);
	data.num_BrushSide = tBsp->lumps[LMP_BRUSHSIDES].length / sizeof(dbrushside_t);
	data.num_game = tBsp->lumps[LMP_GAME_LUMP].length / sizeof(dgamelump_t);
	data.num_StringTable = tBsp->lumps[LMP_TEXDATA_STRING_TABLE].length / sizeof(int);
	data.num_StringData = tBsp->lumps[LMP_TEXDATA_STRING_DATA].length;


	map_vertex = new vertex_t[data.num_verts];
	lightmap_object = new int[8192];
	tex_object = new int[8192];

	// write entity string to file
	char name[80];
	sprintf(name, "%s", map);
	strcat(name, ".ent");
	write_file(name, data.Entity, data.num_entity);
	name[strlen(name) - 4] = '\0';
	char *pname = strstr(name, "maps/");

	if (pname)
	{
		sprintf(map_name, "%s", pname + 5);
	}

	change_axis();

	load_lightmap(gfx);
	load_textures(gfx);


	// generate index array, isnt using PVS
	render(vec3());


	for (int i = 0; i < data.num_verts; i++)
	{
		map_vertex[i].position = data.Vert[i];// +vec3(-11584, -5088, 2050); //offset for dust2
											  //		map_vertex[i].texCoord0 = vec2(data.Vert[i].x, data.Vert[i].y); //just to have something
											  //		map_vertex[i].texCoord1 = vec2(data.Vert[i].x, data.Vert[i].y); //just to have something
		map_vertex[i].tangent = vec4();
	}

	// generate normals
	for (int i = 0; i < index.size();)
	{
		if (i == 0)
		{
			// Triangle fan, first set will have 3 points

			vec3 a = data.Vert[index[i + 1]] - data.Vert[index[i + 0]];
			vec3 b = data.Vert[index[i + 2]] - data.Vert[index[i + 0]];
			vec3 normal = vec3::crossproduct(a, b);

			map_vertex[index[i + 0]].normal = normal;
			map_vertex[index[i + 1]].normal = normal;
			map_vertex[index[i + 2]].normal = normal;

			i += 3;
		}
		else
		{
			// rest will be one new point and two previous
			vec3 a = data.Vert[index[i - 1]] - data.Vert[index[i - 2]];
			vec3 b = data.Vert[index[i + 0]] - data.Vert[index[i - 2]];
			vec3 normal = vec3::crossproduct(a, b);

			map_vertex[index[i + 0]].normal = normal;
			i++;
		}
	}

	map_vertex_vbo = gfx.CreateVertexBuffer(map_vertex, data.num_verts);
	map_index_vbo = gfx.CreateIndexBuffer(index.data(), index.size());

	loaded = true;

	return 0;
}

void HLBsp::temp_render(Graphics &gfx)
{
	gfx.SelectVertexBuffer(map_vertex_vbo);
	gfx.SelectIndexBuffer(map_index_vbo);
	gfx.SelectTexture(0, tex_object[0]); // need face index
	gfx.SelectTexture(1, lightmap_object[0]);
	gfx.DrawArrayTri(0, 0, index.size(), data.num_verts);

}

void HLBsp::render(vec3 &pos)
{
	int curLeaf = find_leaf(pos, 0);
	bsp_render_node(0, curLeaf, pos);
}

void HLBsp::bsp_render_node(int node_index, int leaf, vec3 pos)
{
	if (node_index < 0)
	{
		if (node_index == -1)
			return;

		render_leaf(~node_index);
		return;
	}

	float distance;

	if (data.Plane[data.Node[node_index].planenum].type == 0)
		distance = pos.x - data.Plane[data.Node[node_index].planenum].distance;
	else if (data.Plane[data.Node[node_index].planenum].type == 1)
		distance = pos.y - data.Plane[data.Node[node_index].planenum].distance;
	else if (data.Plane[data.Node[node_index].planenum].type == 2)
		distance = pos.z - data.Plane[data.Node[node_index].planenum].distance;
	else
		distance = data.Plane[data.Node[node_index].planenum].normal * pos - data.Plane[data.Node[node_index].planenum].distance;

	if (distance > 0)
	{
		bsp_render_node(data.Node[node_index].children[1], leaf, pos);
		bsp_render_node(data.Node[node_index].children[0], leaf, pos);
	}

	else
	{
		bsp_render_node(data.Node[node_index].children[0], leaf, pos);
		bsp_render_node(data.Node[node_index].children[1], leaf, pos);
	}

}

void HLBsp::render_leaf(int leaf)
{
	for (int i = 0; i < data.Leaf[leaf].numleaffaces; i++)
	{
		render_face(data.LeafFace[data.Leaf[leaf].firstleafface + i]);
	}
}

void HLBsp::render_face(int face)
{
	int shared;
	int edge0;
	int edge1;
	int edge2;
	int edge3;

	// usual case is four edges making a quad
	for (int i = 0; i < data.Face[face].numedges; i++)
	{
		int edge_index = data.SurfEdge[data.Face[face].firstedge + i];
		dedge_t edge = data.Edge[abs32(edge_index)];
		bool reverse = (edge_index >= 0);

		if (i == 0)
		{
			edge0 = edge.v[reverse ? 0 : 1];
			shared = edge.v[reverse ? 1 : 0];
		}
		else
		{
			shared = edge.v[reverse ? 0 : 1];
			if (shared == edge0)
				continue;
			else
				edge1 = shared;

			shared = edge.v[reverse ? 1 : 0];
			if (shared == edge0)
				continue;
			else
				edge2 = shared;

			index.push_back(edge0);
			index.push_back(edge1);
			index.push_back(edge2);
		}
	}
}

int HLBsp::find_leaf(vec3 pos, int node)
{
	for (int x = 0; x < 2; x++)
	{
		if (data.Node[node].children[x] >= 0)
		{
			if (point_AABB(pos, data.Node[data.Node[node].children[x]].mins, data.Node[data.Node[node].children[x]].maxs))
				return find_leaf(pos, data.Node[node].children[x]);
		}

		else if (~data.Node[node].children[x] != 0)
		{
			if (point_AABB(pos, data.Leaf[~(data.Node[node].children[x])].mins, data.Leaf[~(data.Node[node].children[x])].maxs))
				return ~data.Node[node].children[x];
		}
	}

	return -1;
}

bool HLBsp::point_AABB(vec3 pos, short min[3], short max[3])
{
	if ((min[0] <= pos.x && pos.x <= max[0] && min[1] <= pos.y && pos.y <= max[1] && min[2] <= pos.z && pos.z <= max[2]) ||
		(min[0] >= pos.x && pos.x >= max[0] && min[1] >= pos.y && pos.y >= max[1] && min[2] >= pos.z && pos.z >= max[2]))
	{
		return true;
	}
	else
	{
		return false;
	}
}


/*
Converts axis from quake3 to opengl format
*/
void HLBsp::change_axis()
{
	for (unsigned int i = 0; i < data.num_verts; i++)
	{
		vec3 *vert = &data.Vert[i];
		float temp;

		temp = vert->y;
		vert->y = vert->z;
		vert->z = -temp;
	}


	//	data.Plane[plane_index].normal
	for (unsigned int i = 0; i < data.num_planes; i++)
	{
		dplane_t *plane = &data.Plane[i];
		float	temp;

		temp = plane->normal.y;
		plane->normal.y = plane->normal.z;
		plane->normal.z = -temp;

		//		plane->d *= (1.0f / UNITS_TO_METERS);
	}

	for (unsigned int i = 0; i < data.num_leafs; i++)
	{
		int temp = data.Leaf[i].mins[1];
		data.Leaf[i].mins[1] = data.Leaf[i].mins[2];
		data.Leaf[i].mins[2] = -temp;

		temp = data.Leaf[i].maxs[1];
		data.Leaf[i].maxs[1] = data.Leaf[i].maxs[2];
		data.Leaf[i].maxs[2] = -temp;
	}

	for (unsigned int i = 0; i < data.num_nodes; i++)
	{
		int temp = data.Node[i].mins[1];
		data.Node[i].mins[1] = data.Node[i].mins[2];
		data.Node[i].mins[2] = -temp;

		temp = data.Node[i].maxs[1];
		data.Node[i].maxs[1] = data.Node[i].maxs[2];
		data.Node[i].maxs[2] = -temp;
	}
}



void HLBsp::load_textures(Graphics &gfx)
{
	int loaded = 0;
	int attempted = 0;
	vector<char *> texture_list;


	for (int i = 0; i < data.num_faces; i++)
	{
		bool duplicate = false;
		char filename[128];
		dtexinfo_t *texinfo = &data.TexInfo[data.Face[i].texinfo];
		dtexdata_t *texdata = &data.TexData[texinfo->texdata];
		int string_offset = data.StringTable[texdata->nameStringTableID];
		char *texture = &data.StringData[string_offset];

		sprintf(filename, "media/materials/%s.tga", texture);

		for (int i = 0; i < texture_list.size(); i++)
		{
			if (strcmp(filename, texture_list[i]) == 0)
			{
				duplicate = true;
				break;
			}
		}

		if (duplicate)
		{
			continue;
		}

		tex_object[i] = load_texture(gfx, filename, false, false, 0);
		if (tex_object[i] != 0)
		{
			printf("loaded %s", filename);
			loaded++;
			attempted++;
		}
		else
		{
			printf("failed to load %s", filename);
			attempted++;
		}
		char *file = new char[80];
		sprintf(file, "%s", filename);
		texture_list.push_back(file);



		//texinfo->textureVecs[0] -- need to set texcoords too
	}
	printf("loaded %d of %d textures\n", loaded, attempted);

	for (int i = 0; i < texture_list.size(); i++)
	{
		delete[] texture_list[i];
	}

	// Could just loop through string table to avoid duplicates, but this gives height/width info

	// So a GCF file is packet and a VTF texture is inside along with text data vmt data
	// Could use devil 
}

void HLBsp::load_lightmap(Graphics &gfx)
{
	for (int i = 0; i < data.num_faces; i++)
	{
		vec2 min(8192, 8192);
		vec2 max(-8192, -8192);

		if (data.Face[i].styles[0] != 0 || (signed)data.Face[i].lightofs == -1)
		{
			continue;
		}

		dtexinfo_t *info = &data.TexInfo[data.Face[i].texinfo];

		int w = data.Face[i].LightmapTextureSizeInLuxels[0] + 1;
		int h = data.Face[i].LightmapTextureSizeInLuxels[1] + 1;



		// calculate texture coordinates: https://www.gamedev.net/forums/topic/538713-bspv38-quake-2-bsp-loader-lightmap-problem/
#if 1
		//go through every vertex of the face, and calculate their UV co-ordinates 
		for (int j = 0; j < data.Face[i].numedges; j++)
		{
			vec3 v;
			int edge_index = data.SurfEdge[data.Face[i].firstedge + j];

			if (edge_index >= 0)
				v = data.Vert[data.Edge[edge_index].v[0]];
			else
				v = data.Vert[data.Edge[-edge_index].v[1]];

			float x = info->lightmapVecs[0][0] * v.x + info->lightmapVecs[0][1] * v.y + info->lightmapVecs[0][2] * v.z + info->lightmapVecs[0][3];
			float y = info->lightmapVecs[1][0] * v.x + info->lightmapVecs[1][1] * v.y + info->lightmapVecs[1][2] * v.z + info->lightmapVecs[1][3];

			if (x < min.x)
				min.x = x;
			if (x > max.x)
				max.x = x;

			if (y < min.y)
				min.y = y;
			if (y > max.y)
				max.y = y;
		}

		int width = (int)(ceil(max.x / 16.0f) - floor(min.x / 16.0f)) + 1;
		int height = (int)(ceil(max.y / 16.0f) - floor(min.y / 16.0f)) + 1;
		lightmap_object[i] = gfx.LoadTexture(width, height, GL_RGBA, GL_RGBA, (void *)&data.Lightmap[data.Face[i].lightofs / sizeof(ColorRGBExp32)], false, 0);


		//generate a texture coordinate that's in the range 0.0 to 1.0
		vec2 mid_poly((min.x + max.x) * 0.5f,
			(min.y + max.y) * 0.5f);

		vec2 mid_tex(width * 0.5f,
			height * 0.5f);

		for (int j = 0; j < data.Face[i].numedges; ++j)
		{
			vec3 v;
			int edge_index = data.SurfEdge[data.Face[i].firstedge + j];

			if (edge_index >= 0)
				v = data.Vert[data.Edge[edge_index].v[0]];
			else
				v = data.Vert[data.Edge[-edge_index].v[1]];

			float x = info->lightmapVecs[0][0] * v.x +
				info->lightmapVecs[0][1] * v.y +
				info->lightmapVecs[0][2] * v.z +
				info->lightmapVecs[0][3];

			float y = info->lightmapVecs[1][0] * v.x +
				info->lightmapVecs[1][1] * v.y +
				info->lightmapVecs[1][2] * v.z +
				info->lightmapVecs[1][3];

			vec2 lightmap(mid_tex.x + (x - mid_poly.x) / 16.0f,
				mid_tex.y + (y - mid_poly.y) / 16.0f);

			vec2 coord(lightmap.x / (float)width, lightmap.y / (float)height);

			if (edge_index >= 0)
			{
				map_vertex[data.Edge[edge_index].v[0]].texCoord0 = coord;
				map_vertex[data.Edge[edge_index].v[0]].texCoord1 = coord;
			}
			else
			{
				map_vertex[data.Edge[-edge_index].v[1]].texCoord0 = coord;
				map_vertex[data.Edge[-edge_index].v[1]].texCoord1 = coord;
			}
		}
#endif

	}
}