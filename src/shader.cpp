#include "shader.h"
#include <cmath> // for some sin/cos/tan funcs

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//unsigned int ParticleUpdate::max_particles = 175000;

// Setting to a low value to keep FPS up
unsigned int ParticleUpdate::max_particles = 5000;

int mFont::init(Graphics *gfx)
{
#ifdef DIRECTX
	Shader::init(gfx, "media/hlsl/font.vsh", NULL, NULL);
#else
#ifdef __OBJC__
	if (Shader::init(gfx, "media/glsl/ver410/font.vs", NULL, "media/glsl/ver410/font.fs"))
	{
		program_handle = -1;
		return -1;
	}
#else
	if (Shader::init(gfx, "media/glsl/ver440/font.vs", NULL, "media/glsl/ver440/font.fs"))
	{
		program_handle = -1;
		return -1;
	}
#endif
	u_scale = glGetUniformLocation(program_handle, "u_scale");
	u_row = glGetUniformLocation(program_handle, "u_row");
	u_col = glGetUniformLocation(program_handle, "u_col");
	u_xpos = glGetUniformLocation(program_handle, "u_xpos");
	u_ypos = glGetUniformLocation(program_handle, "u_ypos");
	u_color = glGetUniformLocation(program_handle, "u_color");
	texture0 = glGetUniformLocation(program_handle, "texture0");
#endif
	return 0;
}

void mFont::prelink()
{
#ifndef DIRECTX
	glBindAttribLocation(program_handle, 0, "attr_position");
	glBindAttribLocation(program_handle, 1, "attr_TexCoord");
	glBindAttribLocation(program_handle, 4, "attr_color");
#endif
}

void mFont::Params(char c, float x, float y, float scale, vec3 &color)
{
	int col = c % 16;
	int row = 15 - c / 16;
	float offset = 16.0f / 256.0f;

#ifdef DIRECTX
	uniform->SetFloat(gfx->device, "u_scale", scale);
	uniform->SetFloat(gfx->device, "u_col", col * offset);
	uniform->SetFloat(gfx->device, "u_row", row * offset);
	uniform->SetFloat(gfx->device, "u_xpos", (2.0f * x));
	uniform->SetFloat(gfx->device, "u_ypos", (2.0f * y));
	uniform->SetFloatArray(gfx->device, "u_color", (float *)&color, 3);
#else
	glUniform1f(u_scale, scale);
	glUniform1f(u_col, col * offset);
	glUniform1f(u_row, row * offset);
	glUniform1f(u_xpos, (2.0f * x - 1.0f));
	glUniform1f(u_ypos, (2.0f * (1.0f - y) - 1.0f));
	glUniform3fv(u_color, 1, (float *)&color);
	glUniform1i(texture0, 0);
#endif
}

int Global::init(Graphics *gfx)
{
#ifdef DIRECTX
	Shader::init(gfx, "media/hlsl/basic.vsh", NULL, "media/hlsl/basic.psh");
#else
#ifdef __OBJC__
	if (Shader::init(gfx, "media/glsl/ver410/global.vs", NULL, "media/glsl/ver410/global.fs"))
	{
		program_handle = -1;
		return -1;
	}
#else
	if (Shader::init(gfx, "media/glsl/ver440/global.vs", NULL, "media/glsl/ver440/global.fs"))
	{
		program_handle = -1;
		return -1;
	}
#endif
	matrix = glGetUniformLocation(program_handle, "mvp");
	texture0 = glGetUniformLocation(program_handle, "texture0");
#endif
	return 0;
}

void Global::prelink()
{
#ifndef DIRECTX
	glBindAttribLocation(program_handle, 0, "attr_position");
	glBindAttribLocation(program_handle, 1, "attr_TexCoord");
	glBindAttribLocation(program_handle, 4, "attr_color");
#endif
}

void Global::Params(matrix4 &mvp, int tex0)
{
#ifdef DIRECTX
	uniform->SetMatrix(gfx->device, "mvp", (D3DXMATRIX *)mvp.m);
#else
	glUniformMatrix4fv(matrix, 1, GL_FALSE, mvp.m);
	glUniform1i(texture0, tex0);
#endif
}

int mLight2::init(Graphics *gfx)
{
	//"media/glsl/mlighting3.gs"
#ifdef DIRECTX
	Shader::init(gfx, "media/hlsl/mlighting3.vsh", NULL, "media/hlsl/mlighting3.psh");
#else
#ifdef __OBJC__
	if (Shader::init(gfx, "media/glsl/ver410/mlighting3.vs", "media/glsl/ver410/mlighting3.gs", "media/glsl/ver410/mlighting3.fs"))
	{
		program_handle = -1;
		return -1;
	}
#else
	if (Shader::init(gfx, "media/glsl/ver440/mlighting3.vs", "media/glsl/ver440/mlighting3.gs", "media/glsl/ver440/mlighting3.fs"))
	{
		program_handle = -1;
		return -1;
	}
#endif

	matrix = glGetUniformLocation(program_handle, "mvp");
	texture0 = glGetUniformLocation(program_handle, "texture0");
	texture1 = glGetUniformLocation(program_handle, "texture1");
	texture2 = glGetUniformLocation(program_handle, "texture2");
	texture3 = glGetUniformLocation(program_handle, "texture3");
	texture4 = glGetUniformLocation(program_handle, "texture4");
	texture5 = glGetUniformLocation(program_handle, "texture5");
	texture6 = glGetUniformLocation(program_handle, "texture6");
	texture7 = glGetUniformLocation(program_handle, "texture7");

	texture_lightmap = glGetUniformLocation(program_handle, "texture_lightmap");
	texture_normalmap = glGetUniformLocation(program_handle, "texture_normalmap");

	u_tcmod_scroll0 = glGetUniformLocation(program_handle, "u_tcmod_scroll0");
	u_tcmod_scroll1 = glGetUniformLocation(program_handle, "u_tcmod_scroll1");
	u_tcmod_scroll2 = glGetUniformLocation(program_handle, "u_tcmod_scroll2");
	u_tcmod_scroll3 = glGetUniformLocation(program_handle, "u_tcmod_scroll3");
	u_tcmod_scroll4 = glGetUniformLocation(program_handle, "u_tcmod_scroll4");
	u_tcmod_scroll5 = glGetUniformLocation(program_handle, "u_tcmod_scroll5");
	u_tcmod_scroll6 = glGetUniformLocation(program_handle, "u_tcmod_scroll6");
	u_tcmod_scroll7 = glGetUniformLocation(program_handle, "u_tcmod_scroll7");

	u_tcmod_scale0 = glGetUniformLocation(program_handle, "u_tcmod_scale0");
	u_tcmod_scale1 = glGetUniformLocation(program_handle, "u_tcmod_scale1");
	u_tcmod_scale2 = glGetUniformLocation(program_handle, "u_tcmod_scale2");
	u_tcmod_scale3 = glGetUniformLocation(program_handle, "u_tcmod_scale3");
	u_tcmod_scale4 = glGetUniformLocation(program_handle, "u_tcmod_scale4");
	u_tcmod_scale5 = glGetUniformLocation(program_handle, "u_tcmod_scale5");
	u_tcmod_scale6 = glGetUniformLocation(program_handle, "u_tcmod_scale6");
	u_tcmod_scale7 = glGetUniformLocation(program_handle, "u_tcmod_scale7");

	u_tcmod_sin0 = glGetUniformLocation(program_handle, "u_tcmod_sin0");
	u_tcmod_sin1 = glGetUniformLocation(program_handle, "u_tcmod_sin1");
	u_tcmod_sin2 = glGetUniformLocation(program_handle, "u_tcmod_sin2");
	u_tcmod_sin3 = glGetUniformLocation(program_handle, "u_tcmod_sin3");
	u_tcmod_sin4 = glGetUniformLocation(program_handle, "u_tcmod_sin4");
	u_tcmod_sin5 = glGetUniformLocation(program_handle, "u_tcmod_sin5");
	u_tcmod_sin6 = glGetUniformLocation(program_handle, "u_tcmod_sin6");
	u_tcmod_sin7 = glGetUniformLocation(program_handle, "u_tcmod_sin7");

	u_tcmod_cos0 = glGetUniformLocation(program_handle, "u_tcmod_cos0");
	u_tcmod_cos1 = glGetUniformLocation(program_handle, "u_tcmod_cos1");
	u_tcmod_cos2 = glGetUniformLocation(program_handle, "u_tcmod_cos2");
	u_tcmod_cos3 = glGetUniformLocation(program_handle, "u_tcmod_cos3");
	u_tcmod_cos4 = glGetUniformLocation(program_handle, "u_tcmod_cos4");
	u_tcmod_cos5 = glGetUniformLocation(program_handle, "u_tcmod_cos5");
	u_tcmod_cos6 = glGetUniformLocation(program_handle, "u_tcmod_cos6");
	u_tcmod_cos7 = glGetUniformLocation(program_handle, "u_tcmod_cos7");

	u_num_lights = glGetUniformLocation(program_handle, "u_num_lights");
	u_position = glGetUniformLocation(program_handle, "u_position");
	u_color = glGetUniformLocation(program_handle, "u_color");
#endif
	return 0;
}

void mLight2::prelink()
{
#ifndef DIRECTX
	glBindAttribLocation(program_handle, 0, "attr_position");
	glBindAttribLocation(program_handle, 1, "attr_TexCoord");
	glBindAttribLocation(program_handle, 2, "attr_LightCoord");
	glBindAttribLocation(program_handle, 3, "attr_normal");
	glBindAttribLocation(program_handle, 4, "attr_color");
	glBindAttribLocation(program_handle, 5, "attr_tangent");
#endif
}


void mLight2::Params(matrix4 &mvp, vector<Light *> &light_list, size_t num_lights, vec3 &offset)
{
	vec4 position[MAX_LIGHTS];
	vec4 color[MAX_LIGHTS];
	unsigned int i, j;

	for(i = 0, j = 0; i < num_lights && j < MAX_LIGHTS; i++, j++)
	{
		if (light_list[i]->active)
		{
			vec3 pos = light_list[i]->entity->position - offset;

			position[j] = vec4(pos.x, pos.y, pos.z, light_list[i]->attenuation);


			color[j] = vec4(light_list[i]->color.x,
							light_list[i]->color.y,
							light_list[i]->color.z,
					(float)(light_list[i]->intensity / 500.0f));
		}
	}

#ifdef DIRECTX
	uniform->SetMatrix(gfx->device, "mvp", (D3DXMATRIX *)mvp.m);
//	uniform->SetFloatArray(gfx->device, "u_position", (float *)position, num_lights);
//	uniform->SetFloatArray(gfx->device, "u_color", (float *)color, num_lights);
//	uniform->SetInt(gfx->device, "u_num_lights", num_lights);
#else
	glUniformMatrix4fv(matrix, 1, GL_FALSE, mvp.m);
	glUniform1i(texture0, 0);
	glUniform1i(texture1, 1);
	glUniform1i(texture2, 2);
	glUniform1i(texture3, 3);
//	glUniform1i(texture4, 4);
//	glUniform1i(texture5, 5);
//	glUniform1i(texture6, 6);
//	glUniform1i(texture7, 7);

// Going to treat normals and lightmaps like normal textures
	glUniform1i(texture_lightmap, 8);
	glUniform1i(texture_normalmap, 9);

	vec2 tcmod_scroll = vec2(0.0f, 0.0f);
	vec2 tcmod_scale = vec2(1.0f, 1.0f); // 2.0 makes it half the size
	float tcmod_sin = 0.0f;
	float tcmod_cos = 1.0f;

	glUniform2fv(u_tcmod_scroll0, 1, (float *)&tcmod_scroll);
	glUniform2fv(u_tcmod_scroll1, 1, (float *)&tcmod_scroll);
	glUniform2fv(u_tcmod_scroll2, 1, (float *)&tcmod_scroll);
	glUniform2fv(u_tcmod_scroll3, 1, (float *)&tcmod_scroll);
//	glUniform2fv(u_tcmod_scroll4, 1, (float *)&tcmod_scroll);
//	glUniform2fv(u_tcmod_scroll5, 1, (float *)&tcmod_scroll);
//	glUniform2fv(u_tcmod_scroll6, 1, (float *)&tcmod_scroll);
//	glUniform2fv(u_tcmod_scroll7, 1, (float *)&tcmod_scroll);

	glUniform2fv(u_tcmod_scale0, 1, (float *)&tcmod_scale);
	glUniform2fv(u_tcmod_scale1, 1, (float *)&tcmod_scale);
	glUniform2fv(u_tcmod_scale2, 1, (float *)&tcmod_scale);
	glUniform2fv(u_tcmod_scale3, 1, (float *)&tcmod_scale);
//	glUniform2fv(u_tcmod_scale4, 1, (float *)&tcmod_scale);
//	glUniform2fv(u_tcmod_scale5, 1, (float *)&tcmod_scale);
//	glUniform2fv(u_tcmod_scale6, 1, (float *)&tcmod_scale);
//	glUniform2fv(u_tcmod_scale7, 1, (float *)&tcmod_scale);

	glUniform1fv(u_tcmod_sin0, 1, &tcmod_sin);
	glUniform1fv(u_tcmod_sin1, 1, &tcmod_sin);
	glUniform1fv(u_tcmod_sin2, 1, &tcmod_sin);
	glUniform1fv(u_tcmod_sin3, 1, &tcmod_sin);
//	glUniform1fv(u_tcmod_sin4, 1, &tcmod_sin);
//	glUniform1fv(u_tcmod_sin5, 1, &tcmod_sin);
//	glUniform1fv(u_tcmod_sin6, 1, &tcmod_sin);
//	glUniform1fv(u_tcmod_sin7, 1, &tcmod_sin);

	glUniform1fv(u_tcmod_cos0, 1, &tcmod_cos);
	glUniform1fv(u_tcmod_cos1, 1, &tcmod_cos);
	glUniform1fv(u_tcmod_cos2, 1, &tcmod_cos);
	glUniform1fv(u_tcmod_cos3, 1, &tcmod_cos);
//	glUniform1fv(u_tcmod_cos4, 1, &tcmod_cos);
//	glUniform1fv(u_tcmod_cos5, 1, &tcmod_cos);
//	glUniform1fv(u_tcmod_cos6, 1, &tcmod_cos);
//	glUniform1fv(u_tcmod_cos7, 1, &tcmod_cos);
	
	glUniform1i(u_num_lights, j);
	glUniform4fv(u_position, j, (float *)&position);
	glUniform4fv(u_color, j, (float *)&color);
#endif
}

void mLight2::tcmod_scroll(vec2 &scroll, int index)
{
#ifndef DIRECTX
	switch (index)
	{
	case 0:
		glUniform2fv(u_tcmod_scroll0, 1, (float *)&scroll);
		break;
	case 1:
		glUniform2fv(u_tcmod_scroll1, 1, (float *)&scroll);
		break;
	case 2:
		glUniform2fv(u_tcmod_scroll2, 1, (float *)&scroll);
		break;
	case 3:
		glUniform2fv(u_tcmod_scroll3, 1, (float *)&scroll);
		break;
	case 4:
		glUniform2fv(u_tcmod_scroll4, 1, (float *)&scroll);
		break;
	case 5:
		glUniform2fv(u_tcmod_scroll5, 1, (float *)&scroll);
		break;
	case 6:
		glUniform2fv(u_tcmod_scroll6, 1, (float *)&scroll);
		break;
	case 7:
		glUniform2fv(u_tcmod_scroll7, 1, (float *)&scroll);
		break;
	}
#endif
}

void mLight2::tcmod_scale(vec2 &scale, int index)
{
#ifndef DIRECTX
	switch (index)
	{
	case 0:
		glUniform2fv(u_tcmod_scale0, 1, (float *)&scale);
		break;
	case 1:
		glUniform2fv(u_tcmod_scale1, 1, (float *)&scale);
		break;
	case 2:
		glUniform2fv(u_tcmod_scale2, 1, (float *)&scale);
		break;
	case 3:
		glUniform2fv(u_tcmod_scale3, 1, (float *)&scale);
		break;
	case 4:
		glUniform2fv(u_tcmod_scale4, 1, (float *)&scale);
		break;
	case 5:
		glUniform2fv(u_tcmod_scale5, 1, (float *)&scale);
		break;
	case 6:
		glUniform2fv(u_tcmod_scale6, 1, (float *)&scale);
		break;
	case 7:
		glUniform2fv(u_tcmod_scale7, 1, (float *)&scale);
		break;
	}
#endif
}

void mLight2::tcmod_rotate(float deg, int index)
{
#ifndef DIRECTX

	//convert to radians
	deg = (float)(MY_PI * deg / 180.0f);
	
	float sinval = sinf(deg);
	float cosval = cosf(deg);

	switch (index)
	{
	case 0:
		glUniform1fv(u_tcmod_sin0, 1, &sinval);
		glUniform1fv(u_tcmod_cos0, 1, &cosval);
		break;
	case 1:
		glUniform1fv(u_tcmod_sin1, 1, &sinval);
		glUniform1fv(u_tcmod_cos1, 1, &cosval);
		break;
	case 2:
		glUniform1fv(u_tcmod_sin2, 1, &sinval);
		glUniform1fv(u_tcmod_cos2, 1, &cosval);
		break;
	case 3:
		glUniform1fv(u_tcmod_sin3, 1, &sinval);
		glUniform1fv(u_tcmod_cos3, 1, &cosval);
		break;
	case 4:
		glUniform1fv(u_tcmod_sin4, 1, &sinval);
		glUniform1fv(u_tcmod_cos4, 1, &cosval);
		break;
	case 5:
		glUniform1fv(u_tcmod_sin5, 1, &sinval);
		glUniform1fv(u_tcmod_cos5, 1, &cosval);
		break;
	case 6:
		glUniform1fv(u_tcmod_sin6, 1, &sinval);
		glUniform1fv(u_tcmod_cos6, 1, &cosval);
		break;
	case 7:
		glUniform1fv(u_tcmod_sin7, 1, &sinval);
		glUniform1fv(u_tcmod_cos7, 1, &cosval);
		break;
	}
#endif
}

void mLight2::tcmod_stretch_sin(float amplitude, float phase, float freq, int tick_num, int index)
{
	vec2 value;

	value.x = (float)( amplitude * fsin(freq * tick_num + phase) );
	value.y = value.x;

	tcmod_scale(value, index);
}

void mLight2::tcmod_stretch_sawtooth(float amplitude, float phase, float freq, int tick_num, int index)
{
	vec2 value;

	//cot(x)=1/tan(x)
	value.x = (float) ( amplitude * ( -0.5 * atan(1.0 / tan(freq * tick_num / MY_PI + phase)) ) );
	value.y = value.x;

	tcmod_scale(value, index);
}

void mLight2::tcmod_stretch_inverse_sawtooth(float amplitude, float phase, float freq, int tick_num, int index)
{
	tcmod_stretch_sawtooth(amplitude, phase, freq, -tick_num, index);
}

void mLight2::tcmod_stretch_triangle(float amplitude, float phase, float freq, int tick_num, int index)
{
	vec2 value;
	float x = (float)(0.5 * tick_num * freq + phase);
	value.x = (float)( amplitude * (1.0 - 2.0 * abs32(sign(x) - x)) );
	value.y = value.x;

	tcmod_scale(value, index);
}

void mLight2::tcmod_stretch_square(float amplitude, float phase, float freq, int tick_num, int index)
{
	vec2 value;

	value.x = (float)( amplitude * (4.0 / ((int)(freq * tick_num + phase) % 2 * MY_PI)) );
	value.y = value.x;

	tcmod_scale(value, index);
}
#ifdef NOPE
int mLightDepth::init(Graphics *gfx)
{
	//"media/glsl/mlighting3.gs"
#ifdef DIRECTX
	Shader::init(gfx, "media/hlsl/mlighting3.vsh", NULL, "media/hlsl/mlighting3.psh");
#else
	if (Shader::init(gfx, "media/glsl/ver440/mlighting3_depthonly.vs", NULL, "media/glsl/ver440/mlighting3_depthonly.fs"))
	{
		program_handle = -1;
		return -1;
	}

	matrix = glGetUniformLocation(program_handle, "mvp");
#endif
	return 0;
}

void mLightDepth::prelink()
{
#ifndef DIRECTX
	glBindAttribLocation(program_handle, 0, "attr_position");
	glBindAttribLocation(program_handle, 1, "attr_TexCoord");
	glBindAttribLocation(program_handle, 2, "attr_LightCoord");
	glBindAttribLocation(program_handle, 3, "attr_normal");
	glBindAttribLocation(program_handle, 4, "attr_color");
	glBindAttribLocation(program_handle, 5, "attr_tangent");
#endif
}


void mLightDepth::Params(matrix4 &mvp)
{
#ifdef DIRECTX
//	uniform->SetMatrix(gfx->device, "mvp", (D3DXMATRIX *)mvp.m);
//	uniform->SetFloatArray(gfx->device, "u_position", (float *)position, num_lights);
//	uniform->SetFloatArray(gfx->device, "u_color", (float *)color, num_lights);
//	uniform->SetInt(gfx->device, "u_num_lights", num_lights);
#else
	glUniformMatrix4fv(matrix, 1, GL_FALSE, mvp.m);
#endif
}
#endif

int Post::init(Graphics *gfx)
{
#ifndef DIRECTX
#ifdef __OBJC__
    if (Shader::init(gfx, "media/glsl/ver410/post.vs", NULL, "media/glsl/ver410/post.fs"))
    {
        program_handle = -1;
        return -1;
    }
#else
    if (Shader::init(gfx, "media/glsl/ver440/post.vs", NULL, "media/glsl/ver440/post.fs"))
    {
        program_handle = -1;
        return -1;
    }
#endif
#else
	Shader::init(gfx, "media/hlsl/post.vsh", NULL, "media/hlsl/post.psh");
#endif

#ifndef DIRECTX
	texture0 = glGetUniformLocation(program_handle, "texture0");
	texture1 = glGetUniformLocation(program_handle, "texture1");
	tc_offset = glGetUniformLocation(program_handle, "tc_offset");

	glGenTextures(1, &image);
	glBindTexture(GL_TEXTURE_2D, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);   
	glGenTextures(1, &swap);
	glBindTexture(GL_TEXTURE_2D, swap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
#endif
	return 0;
}


void Post::prelink(void)
{
#ifndef DIRECTX
	glBindAttribLocation(program_handle, 0, "attr_position");
	glBindAttribLocation(program_handle, 1, "attr_TexCoord");
#endif
}

/*
	Could be done in fragment shader, but this should be faster
*/
void Post::resize(int width, int height)
{
	float xInc = 1.0f / width;
    float yInc = 1.0f / height;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            texCoordOffsets[((i * 3 + j) * 2)] = (-1.0f * xInc) + ((float)i * xInc);
            texCoordOffsets[((i * 3 + j) * 2) + 1] = (-1.0f * yInc) + ((float)j * yInc);
        }
    }
}

void Post::Params(int tex0, int tex1)
{
#ifdef DIRECTX
	uniform->SetFloatArray(gfx->device, "tc_offset", texCoordOffsets, 9);
#else
	glUniform1i(texture0, tex0);
	glUniform1i(texture1, tex1);
	glUniform2fv(tc_offset, 9, texCoordOffsets);
#endif
}

#ifdef NOPE
int mLight3::init(Graphics *gfx)
{
	//"media/glsl/mlighting3.gs"
#ifdef DIRECTX
	Shader::init(gfx, "media/hlsl/mlighting3.vsh", NULL, "media/hlsl/mlighting3.psh");
#else
#ifdef __OBJC__
	if (Shader::init(gfx, "media/glsl/ver410/mlight3.vs", "media/glsl/ver410/mlight3.gs", "media/glsl/ver410/mlight3.fs"))
	{
		program_handle = -1;
		return -1;
	}
#else
	if (Shader::init(gfx, "media/glsl/ver440/mlight3.vs", "media/glsl/ver440/mlight3.gs", "media/glsl/ver440/mlight3.fs"))
	{
		program_handle = -1;
		return -1;
	}
#endif

	matrix = glGetUniformLocation(program_handle, "mvp");
	texture0 = glGetUniformLocation(program_handle, "texture0");
	texture1 = glGetUniformLocation(program_handle, "texture1");
	texture2 = glGetUniformLocation(program_handle, "texture2");

	texture3 = glGetUniformLocation(program_handle, "shadowtex0");
	texture4 = glGetUniformLocation(program_handle, "shadowtex1");
	texture5 = glGetUniformLocation(program_handle, "shadowtex2");
	texture6 = glGetUniformLocation(program_handle, "shadowtex3");
	texture7 = glGetUniformLocation(program_handle, "shadowtex4");
	texture8 = glGetUniformLocation(program_handle, "shadowtex5");


	u_num_lights = glGetUniformLocation(program_handle, "u_num_lights");
	u_position = glGetUniformLocation(program_handle, "u_position");
	u_color = glGetUniformLocation(program_handle, "u_color");

#endif
	return 0;
}

void mLight3::prelink()
{
#ifndef DIRECTX
	glBindAttribLocation(program_handle, 0, "attr_position");
	glBindAttribLocation(program_handle, 1, "attr_TexCoord");
	glBindAttribLocation(program_handle, 2, "attr_LightCoord");
	glBindAttribLocation(program_handle, 3, "attr_normal");
	glBindAttribLocation(program_handle, 4, "attr_color");
	glBindAttribLocation(program_handle, 5, "attr_tangent");
#endif
}


void mLight3::Params(matrix4 &mvp, vector<Light *> &light_list, size_t num_lights)
{
	vec3 position[MAX_LIGHTS];
	vec4 color[MAX_LIGHTS];
	unsigned int i, j;

	for (i = 0, j = 0; i < num_lights && j < MAX_LIGHTS; i++, j++)
	{
		if (light_list[i]->entity->light->active)
		{
			position[j] = light_list[i]->entity->position;
			color[j] = vec4(light_list[i]->color.x, light_list[i]->color.y,
				light_list[i]->color.z, (float)(light_list[i]->intensity / 500.0f));
		}
	}

#ifdef DIRECTX
	uniform->SetMatrix(gfx->device, "mvp", (D3DXMATRIX *)mvp.m);
	uniform->SetFloatArray(gfx->device, "u_position", (float *)position, num_lights);
	uniform->SetFloatArray(gfx->device, "u_color", (float *)color, num_lights);
	uniform->SetInt(gfx->device, "u_num_lights", num_lights);
#else
	glUniformMatrix4fv(matrix, 1, GL_FALSE, mvp.m);
	glUniform1i(texture0, 0);
	glUniform1i(texture1, 1);
	glUniform1i(texture2, 2);
	glUniform1i(texture3, 3);
	glUniform1i(texture4, 4);
	glUniform1i(texture5, 5);
	glUniform1i(texture6, 6);
	glUniform1i(texture7, 7);
	glUniform1i(texture8, 8);

	glUniform1i(u_num_lights, j);
	glUniform3fv(u_position, j, (float *)&position);
	glUniform4fv(u_color, j, (float *)&color);
#endif
}

int ShadowMap::init(Graphics *gfx)
{
#ifndef DIRECTX

#ifdef __OBJC__
	if (Shader::init(gfx, "media/glsl/ver410/shadow.vs", NULL, "media/glsl/ver410/shadow.fs"))
	{
		program_handle = -1;
		return -1;
	}
#else
	if (Shader::init(gfx, "media/glsl/ver440/shadow.vs", NULL, "media/glsl/ver440/shadow.fs"))
	{
		program_handle = -1;
		return -1;
	}
#endif

	matrix = glGetUniformLocation(program_handle, "mvp");
	shadowmatrix = glGetUniformLocation(program_handle, "shadowmvp");
	shadowmap = glGetUniformLocation(program_handle, "ShadowMap");
	u_color = glGetUniformLocation(program_handle, "color");
#endif
	return 0;
}

void ShadowMap::prelink()
{
#ifndef DIRECTX
	glBindAttribLocation(program_handle, 0, "attr_position");
	glBindAttribLocation(program_handle, 1, "attr_TexCoord");
	glBindAttribLocation(program_handle, 2, "attr_LightCoord");
	glBindAttribLocation(program_handle, 3, "attr_normal");
	glBindAttribLocation(program_handle, 4, "attr_color");
	glBindAttribLocation(program_handle, 5, "attr_tangent");
#endif
}


void ShadowMap::Params(matrix4 &mvp, matrix4 &shadowmvp)
{
#ifdef DIRECTX
//	uniform->SetMatrix(gfx->device, "mvp", (D3DXMATRIX *)mvp.m);
//	uniform->SetFloatArray(gfx->device, "u_position", (float *)position, num_lights);
//	uniform->SetFloatArray(gfx->device, "u_color", (float *)color, num_lights);
//	uniform->SetInt(gfx->device, "u_num_lights", num_lights);
#else
	glUniformMatrix4fv(matrix, 1, GL_FALSE, mvp.m);
	glUniformMatrix4fv(shadowmatrix, 1, GL_FALSE, shadowmvp.m);
	glUniform1i(shadowmap, 4);
#endif
}
#endif


int ParticleUpdate::init(Graphics *gfx)
{
#ifndef DIRECTX
	if (Shader::init(gfx,
		"media/glsl/ver440/particle_update.vs",
		"media/glsl/ver440/particle_update.gs",
		NULL))
	{
		program_handle = -1;
		return -1;
	}

	u_emit_position = glGetUniformLocation(program_handle, "u_emit_position");
	u_emit_vel_min = glGetUniformLocation(program_handle, "u_emit_vel_min");
	u_emit_vel_range = glGetUniformLocation(program_handle, "u_emit_vel_range");
	u_emit_color = glGetUniformLocation(program_handle, "u_emit_color");
	u_emit_size = glGetUniformLocation(program_handle, "u_emit_size");
	u_emit_life_min = glGetUniformLocation(program_handle, "u_emit_life_min");
	u_emit_life_range = glGetUniformLocation(program_handle, "u_emit_life_range");
	u_emit_num = glGetUniformLocation(program_handle, "u_emit_num");

	u_gravity = glGetUniformLocation(program_handle, "u_gravity");
	u_delta_time = glGetUniformLocation(program_handle, "u_delta_time");
	u_seed = glGetUniformLocation(program_handle, "u_seed");

	glGenQueries(1, &query);



	vertex_t data[] = {
		{ vec3(1.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f), vec2(1.0f, 1.0f), vec3(1.0f, 0.0f, 1.0f), 1, vec4(1.0f, 1.0f, 1.0f, 1.0f) },
	};

	memset(particle, 0, sizeof(particle));
	memcpy(particle, data, sizeof(data));

	glGenBuffers(1, &ParticleBufferA);
	glBindBuffer(GL_ARRAY_BUFFER, ParticleBufferA);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_t) * ParticleUpdate::max_particles, 0, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertex_t) * 5, &particle[0]);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &ParticleBufferB);
	glBindBuffer(GL_ARRAY_BUFFER, ParticleBufferB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_t) * ParticleUpdate::max_particles, 0, GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
	return 0;
}


void ParticleUpdate::prelink(void)
{
#ifndef DIRECTX
	const GLchar* feedbackVaryings[] = {
		"vary_position",
		"vary_TexCoord",
		"vary_LightCoord",
		"vary_velocity",
		"vary_color",
		"vary_tangent"
	};
	glTransformFeedbackVaryings(program_handle, 6, feedbackVaryings, GL_INTERLEAVED_ATTRIBS);

	glBindAttribLocation(program_handle, 0, "attr_position");
	glBindAttribLocation(program_handle, 1, "attr_TexCoord");
	glBindAttribLocation(program_handle, 2, "attr_LightCoord");
	glBindAttribLocation(program_handle, 3, "attr_velocity");
	glBindAttribLocation(program_handle, 4, "attr_color");
	glBindAttribLocation(program_handle, 5, "attr_tangent");
#endif
}

void ParticleUpdate::Params(emitter_t &emit)
{
#ifndef DIRECTX
	glUniform3fv(u_emit_position, 1, (float *)&(emit.position));
	glUniform3fv(u_emit_vel_min, 1, (float *)&(emit.vel_min));
	glUniform3fv(u_emit_vel_range, 1, (float *)&(emit.vel_range));
	glUniform1i(u_emit_color, emit.color);
	glUniform1f(u_emit_size, emit.size);
	glUniform1f(u_emit_life_min, emit.life_min);
	glUniform1f(u_emit_life_range, emit.life_range);
	glUniform1i(u_emit_num, emit.num);
	glUniform3fv(u_gravity, 1, (float *)&(emit.gravity));

	glUniform1f(u_delta_time, emit.delta_time);
	glUniform3fv(u_seed, 1, (float *)&(emit.seed));
	//emitter = emit;
#endif
}

int ParticleUpdate::step(Graphics &gfx, emitter_t &emit)
{
	// Source buffer (input)
	gfx.SelectVertexBuffer(ParticleBufferA);

	// Dont rasterize
	glEnable(GL_RASTERIZER_DISCARD);

	// Target Buffer (output)
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, ParticleBufferB);

	glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, query);
	glBeginTransformFeedback(GL_POINTS);
	glDrawArrays(GL_POINTS, 0, emit.num);
	glEndTransformFeedback();
	glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);

	glDisable(GL_RASTERIZER_DISCARD);
	glFlush();

	// Swap buffers
	int temp = ParticleBufferA;
	ParticleBufferA = ParticleBufferB;
	ParticleBufferB = temp;

	// Get num primitives generated
	glGetQueryObjectuiv(query, GL_QUERY_RESULT, &num_particle);

	if (num_particle == 0)
	{
		printf("ParticleSystem Error: %u particles written!\n\n", num_particle);
	}

	//emitter.num = num_particle;
	emit.num = num_particle;

	return ParticleBufferA;
}

#define QUAD_PARTICLES

int ParticleRender::init(Graphics *gfx)
{
#ifndef DIRECTX

#ifdef QUAD_PARTICLES
	// quad particles
	if (Shader::init(gfx,
		"media/glsl/ver440/particle_render.vs",
		"media/glsl/ver440/particle_render.gs",
		"media/glsl/ver440/particle_render.fs"))
	{
		program_handle = -1;
		return -1;
	}
#else
	// point textured particles
	if (Shader::init(gfx,
		"media/glsl/ver440/particle_render2.vs",
		NULL,
		"media/glsl/ver440/particle_render2.fs"))
	{
		program_handle = -1;
		return -1;
	}
#endif

	u_mvp = glGetUniformLocation(program_handle, "u_mvp");
	u_quad1 = glGetUniformLocation(program_handle, "u_quad1");
	u_quad2 = glGetUniformLocation(program_handle, "u_quad2");
	u_texture0 = glGetUniformLocation(program_handle, "u_texture0");
	u_xshift = glGetUniformLocation(program_handle, "u_xshift");
	u_yshift = glGetUniformLocation(program_handle, "u_yshift");
#endif

	unsigned int *index = new unsigned int [ParticleUpdate::max_particles];
	for (unsigned int i = 0; i < ParticleUpdate::max_particles; i++)
	{
		index[i] = i;
	}
	ibo = gfx->CreateIndexBuffer(index, ParticleUpdate::max_particles);
	delete [] index;
	index = NULL;
	return 0;
}


void ParticleRender::prelink(void)
{
#ifndef DIRECTX
	glBindAttribLocation(program_handle, 0, "attr_position");
	glBindAttribLocation(program_handle, 1, "attr_TexCoord");
	glBindAttribLocation(program_handle, 2, "attr_LightCoord");
	glBindAttribLocation(program_handle, 3, "attr_velocity");
	glBindAttribLocation(program_handle, 4, "attr_color");
	glBindAttribLocation(program_handle, 5, "attr_tangent");
#endif
}

void ParticleRender::Params(matrix4 &mvp, vec3 &quad1, vec3 &quad2, float x, float y)
{
#ifndef DIRECTX
	glUniformMatrix4fv(u_mvp, 1, GL_FALSE, mvp.m);
	glUniform3fv(u_quad1, 1, (float *)&quad1);
	glUniform3fv(u_quad2, 1, (float *)&quad2);
	glUniform1f(u_xshift, x);
	glUniform1f(u_yshift, y);
	glUniform1i(u_texture0, 0);
#endif
}

void ParticleRender::render(Graphics &gfx, int start, int vbo, int num)
{
#ifndef DIRECTX
	gfx.Blend(true);
	gfx.Depth(false);
	gfx.BlendFunc(NULL, NULL);
	gfx.SelectVertexBuffer(vbo);
	gfx.SelectIndexBuffer(ibo);
	gfx.DrawArrayPoint(start, 0, num, num);
	gfx.Depth(true);
	gfx.Blend(false);
#endif
}
