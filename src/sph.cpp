#include "sph.h"
#include <math.h>

#define SCALE	0.04f
#define EPSILON 0.000001f
#define PMASS	0.010543f //kg 0.020543
#define I_STIFF (2.0f / 1000.0f)
#define E_STIFF 10000
#define E_DAMP	256
#define REST_DENS	1000 //kg.m-3

#define ACCEL_LIMIT	200 //m.s-2
#define VISC	0.05f
#define RAD		0.004f //m

#define DT		0.004f

#define kH		0.3f
#define kH2		(kH * kH)

#define POLY6_KERN		(315.0f / (64.0f * MY_PI * (float)pow(kH, 9)))
#define GRAD_POLY6_KERN 945.0f / (32.0f * MY_PI * (float)pow(h, 9));
#define LAP_POLY6_KERN  945.0f / (32.0f * MY_PI * (float)pow(h, 9));
#define SPIKY_KERN		(-45.0f / (MY_PI * (float)pow(kH, 6)))
#define VISCOSITY_KERN	(45.0f / (MY_PI * (float)pow(kH, 6)))

#define MAX_NEIGHBOR 32

Sph::Sph()
{
	num_particle = 3000;
	last_calculated = 0;
	last_rendered = 0;

	max_bound = vec3(500.0f, 1000.0f, -500.0f);
	min_bound = vec3(-500.0f, -500.0f, 0.0f);
	part = new particle_t[num_particle];

	grid_width = 4;
	grid_height = 4;
	grid_depth = 4;

	init();
	update_grid();
	update_neighbors();
}

void Sph::init()
{
	for (int i = 0; i < num_particle; i++)
	{
		memset(&part[i], 0, sizeof(particle_t));
		part[i].pos.x = rand_float(min_bound.x, max_bound.x);
		part[i].pos.y = rand_float(min_bound.y, max_bound.y);
		part[i].pos.z = rand_float(min_bound.z, max_bound.z);
	}
}

// place all particles in bounds into grid
void Sph::update_grid()
{
	for (int i = 0; i < grid_width; i++)
	{
		for (int j = 0; j < grid_height; j++)
		{
			for (int k = 0; k < grid_depth; k++)
			{
				grid[i][j][k].num_part = 0;
			}
		}
	}


	for (int i = 0; i < num_particle; i++)
	{
		float xf = part[i].pos.x / (max_bound.x - min_bound.x) + 0.5f;
		int x = xf * grid_width;

		float yf = part[i].pos.y / (max_bound.y - min_bound.y) + 0.5f;
		int y = yf * grid_height;

		float zf = part[i].pos.z / (max_bound.z - min_bound.z) + 0.5f;
		int z = zf * grid_width;


		int num = grid[x][y][z].num_part;
		if (num < MAX_GRID_PARTICLE)
		{
			grid[x][y][z].part[num] = &part[i];
			grid[x][y][z].num_part++;
		}
		else
		{
			printf("Filled grid %d %d %d with max particles\r\n", x, y, z);
			part[i].pos.x = rand_float(min_bound.x, max_bound.x);
			part[i].pos.y = rand_float(min_bound.y, max_bound.y);
			part[i].pos.z = rand_float(min_bound.z, max_bound.z);
			i--;
		}
	}
}

// check for neighbor particles
void Sph::update_neighbors()
{
	float dr2 = 0;
	float lh2 = 4 * kH2;
	int max_neighbor = 0;


	// Initialize to zero
	for (int i = 0; i < num_particle; i++)
	{
		part[i].nbCount = 0;
		for (int j = 0; j < MAX_NEIGHBOR; j++)
		{
			part[i].nbList[j] = 0;
		}
	}

	// Update neighbors on a grid by grid basis (ignoring particles outside of same grid)
	for (int x = 0; x < grid_width; x++)
	{
		for (int y = 0; y < grid_height; y++)
		{
			for (int z = 0; z < grid_depth; z++)
			{
				for (int i = 0; i < grid[x][y][z].num_part; i++)
				{
					for (int j = 0; j < grid[x][y][z].num_part; j++)
					{
						if (i >= j)
							continue;

						if (grid[x][y][z].part[i]->nbCount >= MAX_NEIGHBOR)
						{
							break;
						}

						if (part[i].nbCount >= MAX_NEIGHBOR)
						{
							break;
						}

						if (part[i].nbCount > max_neighbor)
						{
							max_neighbor = part[i].nbCount;
							printf("max neighbor: %d\r\n", max_neighbor);
						}
						//calculate distance
						dr2 = 0;

						vec3 dist = (part[i].pos - part[j].pos);
						dr2 += dist * dist * SCALE * SCALE;

						//list neighborhood particle
						if (dr2 < lh2)
						{
							part[i].nbList[part[i].nbCount] = j;
							part[i].nbCount++;
							if (part[j].nbCount < MAX_NEIGHBOR)
							{
								part[j].nbList[part[j].nbCount] = i;
								part[j].nbCount++;
							}
						}
					}
				}
			}
		}
	}


}

void Sph::step(int frame)
{
	if (last_calculated == last_rendered)
	{
		update_grid();
		update_neighbors();
		calc_density_pressure();
		calc_force();
		calc_pos();
		last_calculated = frame;
	}
}





// DENSITY 
//
// Calculate the density by basically making a weighted sum
// of the distances of neighboring particles within the radius of support (r)
void Sph::calc_density_pressure()
{
	float r2, sum, dist;
	int num = 0;

	for (int i = 0; i < num_particle; i++)
	{
		sum = 0;
		for (int j = 0; j < part[i].nbCount; j++)
		{
			if (i <= j)
				continue;

			num = part[i].nbList[j];
			r2 = norm2(part[i].pos, part[j].pos) * SCALE * SCALE;

			if (kH2 < r2)
			{
				dist = kH2 - r2;
				// density
				sum += dist * dist * dist;
			}
		}

		sum += kH2 * kH2 * kH2;

		part[i].dens = PMASS * POLY6_KERN * sum;

		// PRESSURE
		//
		// Make the simple pressure calculation from the equation of state.
		part[i].pres = I_STIFF * (part[i].dens - REST_DENS);
	}

}

//calculate acceleration and color field
void Sph::calc_force()
{
	float pterm, vterm, r, color, temp, temp2;
	vec3 pacc;
	vec3 vacc;
	int num = 0;

	//debug variable
	float min = 0;
	float max = 0;

	// PRESSURE FORCE
	//
	// We will force particles in or out from their neighbors
	// based on their difference from the rest density.
	for (int i = 0; i < num_particle; i++)
	{
		pacc = vec3(0.0f, 0.0f, 0.0f);
		vacc = vec3(0.0f, 0.0f, 0.0f);
		color = 1.0f;

		for (int j = 0; j < part[i].nbCount; j++)
		{
			num = part[i].nbList[j];

			if (i != num)
			{
				r = newtonSqrt(norm2(part[i].pos, part[num].pos)) * SCALE;

				if (r < kH)
				{
					// Compute force due to pressure and viscosity
					temp = kH - r;
					pterm = temp * temp * (part[i].pres + part[num].pres) / (r * part[num].dens);
					vterm = temp / part[num].dens;
					temp2 = kH2 - r * r;
					color += 1.0f / part[num].dens * temp2 * temp2 * temp2;

					pacc += (part[i].pos - part[num].pos) * pterm;
					vacc += (part[num].vel - part[i].vel) * vterm;
				}
			}
		}

		part[i].color = PMASS * POLY6_KERN * color;
		part[i].acc = (pacc  * -0.5 * SPIKY_KERN
			+ vacc * VISCOSITY_KERN * VISC) * PMASS / part[i].dens;

		//debug
		if (i == 0)
		{
			min = max = part[i].color;
		}
		else
		{
			temp = part[i].color;
			temp = clamp(temp, min, max);
		}
	}

}

//calculate position from forces and accelerations
void Sph::calc_pos()
{
	float temp;

	for (int i = 0; i < num_particle; i++)
	{
		vec3 zero(0.0f, 0.0f, 0.0f);
		//acceleration limit
		temp = norm2(part[i].acc, zero);
		if (ACCEL_LIMIT * ACCEL_LIMIT < temp)
		{
			part[i].acc *= ACCEL_LIMIT / sqrt(temp);
		}

		//lower boundary condition
		temp = 2 * RAD - (part[i].pos.y - min_bound.y) * SCALE;
		if (temp > EPSILON)
		{
			part[i].acc.y += E_STIFF * temp - E_DAMP * part[i].vel.y;
		}

		//upper boundary condition
		temp = 2 * RAD - (max_bound.y - part[i].pos.y) * SCALE;
		if (temp > EPSILON)
		{
			part[i].acc.y += -E_STIFF * temp - E_DAMP * part[i].vel.y;
		}

		part[i].acc.y += -9.8f;

		part[i].vel += part[i].acc * DT;
		part[i].pos += part[i].vel * DT / SCALE;
	}
}


inline float Sph::norm2(vec3 &a, vec3 &b)
{
	//	return (a - b) * (a - b);
	return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z);
}


// Sum up densities
double Sph::update_sum(int *x, vec3 &xp)
{
	double sum = 0;
	double dr2, temp;
	int num;

	for (int x = 0; x < grid_width; x++)
	{
		for (int y = 0; y < grid_height; y++)
		{
			for (int z = 0; z < grid_depth; z++)
			{
				dr2 = 0;
				num = grid[x][y][z].num_part;

				dr2 += (xp / SCALE - grid[x][y][z].part[num]->pos) * (xp / SCALE - grid[x][y][z].part[num]->pos);
				dr2 *= SCALE * SCALE;

				if (dr2 < kH2)
				{
					temp = kH2 - dr2;
					sum += temp * temp * temp;
				}
			}
		}
	}


	return sum * 100000;
}

void Sph::render()
{
	last_rendered = last_calculated;
	// Draw Fluid Particles
	glPointSize(10.0f);
	for (int i = 0; i < num_particle; ++i)
	{
		float c = 0.1f * part[i].pres;
		float x = 20 * fabs(part[i].nbCount);
		float y = 20 * fabs(part[i].dens);

		if (i % 4 == 0)
			glColor3f(1.0f, 0.0f, 0.0f);
		else if (i % 4 == 1)
			glColor3f(1.0f, 1.0f, 0.0f);
		else if (i % 4 == 2)
			glColor3f(1.0f, 1.0f, 1.0f);
		else if (i % 4 == 3)
			glColor3f(0.0f, 0.0f, 1.0f);


		glBegin(GL_POINTS);
		glVertex3f(part[i].pos.x, part[i].pos.y, part[i].pos.z);
		glEnd();
	}
}