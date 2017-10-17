#version 440 core

// Per vertex interpolated program input
in vec4 vary_position;
flat in int vary_color;
in vec2 vary_TexCoord;

// Final fragment color output
out vec4 Fragment;

// constant program input
layout(binding=0) uniform sampler2D texture0;
layout(binding=1) uniform sampler2D texture1;

uniform vec2 tc_offset[9];

uniform int u_type;

uniform int u_dir;
uniform float u_scale;
uniform float u_amount;
uniform float u_strength;

float GaussianFunction(float x, float dev)
{
  return ((1.0/sqrt(2.0*3.142857*dev))*exp(-(x*x)/(2.0*dev)));
} 

void main(void)
{
	vec4 texsample[9];

	for (int i = 0; i < 9; i++)
	{
		texsample[i] = texture2D(texture0, vary_TexCoord + tc_offset[i]);
	}


	if (u_type == 0)
	{
		// laplacian edge detect
		Fragment =  (texsample[4] * 8.0) - 
			(texsample[0] + texsample[1] + texsample[2] + 
			texsample[3] + texsample[5] + 
			texsample[6] + texsample[7] + texsample[8]);
	}
	else if (u_type == 1)
	{
		// gaussian blur
		Fragment = (1.0 * texsample[0] + 2.0 * texsample[1] + 1.0 * texsample[2] + 
			2.0 * texsample[3] + 4.0 * texsample[4] + 2.0 * texsample[5] + 
			1.0 * texsample[6] + 2.0 * texsample[7] + 1.0 * texsample[8]) / 16.0;
	}
	else if (u_type == 2)
	{
		// embosss
		Fragment = (-2.0 * texsample[0] + -1.0 * texsample[1] +
			-1.0 * texsample[3] + 1.0 * texsample[4] + 1.0 * texsample[5] + 
						1.0 * texsample[7] + 2.0 * texsample[8]) / 16.0;
	}
	else if (u_type == 3)
	{
		vec4	color = vec4(0.0,0.0,0.0,0.0);
		vec4	temp =  vec4(0.0,0.0,0.0,0.0);
		float	strength = 1.0 - u_strength;
		float	half1 = float(u_amount) * 0.5;
		float	texel = (1.0/1024.0);
		float	dev = u_amount * 0.5 * 0.5;
		int	count = int(u_amount);
	
		dev *= dev;
		if (u_dir == 0) 
		{
			for (int i = 0; i < count; i++)
			{
				float offset = float(i) - half1;
				temp = texture2D(texture0, vary_TexCoord + vec2(offset * texel * u_scale, 0.0)) * GaussianFunction(offset * u_strength, dev);
				color += temp;
			}
		} 
		else 
		{
			for (int i = 0; i < count; i++)
			{
				float offset = float(i) - half1;
				temp = texture2D(texture0, vary_TexCoord + vec2(0.0, offset * texel * u_scale)) * GaussianFunction(offset * u_strength, dev);
				color += temp;
			}
		}
	  
		Fragment = clamp(color, 0.0, 1.0);
		Fragment.w = 1.0;
	}
}



/*


// box blur
//   1 2 1
//   2 1 2   / 13
//   1 2 1

// edge detection
//    1  2  1
//    0  0  0   / 1
//   -1 -2 -1

// Sharpening
//     0  -2  0
//    -2  11 -2   / 3
//     0  -2  0

// Depth of field
//	H = (L * L) / (F * D)  - hyperfocal distance (dist to focused object)
//	NF = H * D / (H+(D-L)) - Near focus limit
//	FF = H * D / (H-(D-L)) - Far focus limit
//	L = lens focal length, 35mm 105mm... dist from center of lens to receiver
//	f = lens aperature (fstop)
//	d = diameter of least circle of confusion (sharpness)
//	D = lens focal distance
// Render blurry image, copy good pixels and blurred pixels based on Z distance

//heat shimmer, random offsets in a distortion texture, particle system to emits distortion from source


// exposure / hdr / glare effect
// exposure level = average intensity of image, normalize to 0.5 over time
// color = color * alpha


// star bursts
// directional star blur

// ghosts
// flip scale a bunch of times, mask out edges with circle blur

//directional lighting
// point lighting
// spot light, obstructed point light
// area lights (point light with area)

//spot light equations
// I = (cos alpha - cos thi) / (cos theta - cos thi)
// alpha = current lighting angle within cone
// theta = inner cone angle
// thi = outer cone angle

//bdrfs - better than phong shading


//height map to normal map

//dh/du = slope in U direction
//dh/dv = slope in V direction
//Len = sqrt(dh/du + dh/dv + 1)

//bumpmap.x = (dh/hu) / len
//bumpmap.y = (dh/dv) / len
//bumpmap.z = 1 / len

//tangent = vector along U
//binormal = vector along V

*/