//=============================================================================
// This file is part of the altEngine distribution
// (https://github.com/akw0088/altEngine/)
// Copyright (c) 2018 Alexander Wright All Rights Reserved.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON - INFRINGEMENT.IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//=============================================================================

#include "include.h"

#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#define MAX_VELOCITY 800.0f


typedef struct
{
	bool escort;
	float speed;
	int wait;
	float path_min_dist;
} train_t;

class RigidBody : public Model
{
public:
	RigidBody(Entity *entity);
	void recalc();
	void integrate(float time);
	bool collision_detect(vec3 &v);
	bool collision_detect(plane_t &p);
	bool collision_detect_simple(RigidBody &body);
	bool collision_detect(RigidBody &body);
	bool collision_distance(RigidBody &body);
	void impulse(plane_t &plane, vec3 &vertex);
	void impulse(RigidBody &rigid, vec3 &point);
	void impulse(RigidBody &rigid, vec3 &point, plane_t &plane);
	void frame2ent(Frame *camera_frame, input_t &input);
	void frame2ent_yaw(Frame *camera_frame, input_t &input);
	void save_config(cfg_t &config);
	void load_config(cfg_t &config);
	void get_frame(Frame &frame);

	//AI
	void seek(const vec3 &position);
	void flee(const vec3 &position);
	void arrive(const vec3 &position);
	void pursue();
	void evade();
	void wander(float radius, float distance, float jitter);
	void set_target(Entity &target);


	bool move(input_t &input, float speed_scale);
	bool ground_move(input_t &input, float speed_scale);
	bool air_move(input_t &input, float speed_scale);
	bool water_move(input_t &input, float speed_scale);
	bool flight_move(input_t &input, float speed_scale);

	void move_forward(float speed_scale);
	void move_backward(float speed_scale);
	void move_left(float speed_scale);
	void move_right(float speed_scale);
	void move_up(float speed_scale); // jump
	void move_down(float speed_scale);
	void lookat(vec3 &target);
	void lookat_yaw(vec3 &target);
	void pid_follow_path(vec3 *path_list, int num_path, float max_velocity, float distance, int wait);
	int train_follow_path(vec3 *target, float max_velocity, float distance, int wait);

	float get_volume();
	float get_height();
	float *get_matrix(float *matrix);


	unsigned int impact_index;


	float			impact_velocity;
	Entity			*target;
	vec3			sphere_target;
	int			jump_timer;
	unsigned int	bounce;


	struct flag
	{
		unsigned short int on_ground : 1,
			hard_impact : 1,
			sleep : 1,
			gravity : 1,
			noclip : 1,
			flight : 1,
			water : 1,
			last_water : 1,
			lava : 1,
			slime : 1,
			rotational_friction_flag : 1,
			translational_friction_flag : 1,
			ground_friction_flag : 1,
			pursue_flag : 1,
			step_flag : 1,
			pad : 1;
	} flags;


	float			restitution;
	int				y_offset;
	int				step_type;
	int				bsp_trigger_volume;
	int				bsp_model_platform;
	float			rotational_friction;
	float			translational_friction;
	float			water_depth;
	//Physical
	float			mass;
	matrix3			inverse_tensor;
	matrix3			world_tensor;
	vec3			velocity;
	vec3			angular_velocity;
	vec3			net_force;
	vec3			net_torque;
	vec3			old_position;
	matrix3			old_orientation;

	vec3 wishdir;


	train_t train;

	func_path_t path;
	pid_state_t pid;
};

#endif
