#include "include.h"

#ifndef ENGINE_H
#define ENGINE_H

class Engine
{
public:
	void init(void *param1, void *param2);
	void load(char *level);
	void unload();
	void destroy();
	void quit();

	void render();
	void render_scene(bool lights);
	void render_entities();
	void render_shadows();


	void render_shadowmaps();
	void post_process(int num_passes);
	void resize(int width, int height);
	void debug_messages();

	void step();
	void dynamics();
	void spatial_testing();
	void activate_light(float distance, Light *light);
	void check_triggers();
	void update_audio();

	bool mousepos(int x, int y, int deltax, int deltay);
	void keypress(char *key, bool pressed);
	void keystroke(char key);

	void init_camera();
	void load_entities();
	void load_sounds();
	void load_models();
	void load_model(Entity &ent);
	void create_sources();

	void console(char *cmd);
	void handle_console(char key);
	void handle_game(char key);

	bool collision_detect(RigidBody &body);
	bool map_collision(RigidBody &body);
	bool body_collision(RigidBody &body);

	//temp section
	void bind(int port);
	void connect(char *server);
	void chat(char *msg);
	int GetKeyState(button_t &keyboard);
	button_t GetKeyState(int keystate);
	void server_step();
	void client_step();
	void send_entities();

	void generate_buffers();
	void destroy_buffers();

private:
	matrix4				transformation;
	matrix4				projection;
	matrix4				identity;

	Graphics			gfx;
	Sound				audio;
	Bsp					map;
	button_t			keyboard;
	Menu				menu;
	Frame				camera;

	int xres, yres;

	vector<Entity *>	entity_list;
	vector<Light *>		light_list;
	vector<wave_t>		snd_wave;

	// temp section
	int				no_tex;
	char			key_buffer[1024];
	int				sequence;
	Net				net;
	reliablemsg_t	reliable;
	int				spawn;
	int				reload;

	// md5 stuff
	int				frame_step;
	MD5Model		zcc;

	//server
	bool	server;
	vector <client_t *> client_list;
	
	//client
	bool	client;
	int		last_server_sequence;

	Post	post;
	mLight2	mlight2;
	Global	global;
	ShadowMap shadowmap;
	void	*param1;
	void	*param2;
};

#endif

