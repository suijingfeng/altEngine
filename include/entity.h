#include "include.h"

#ifndef ENTITY_H
#define ENTITY_H

typedef enum
{
	ENT_UNKNOWN,
	ENT_ITEM_ARMOR_COMBAT,
	ENT_ITEM_ARMOR_BODY,
	ENT_ITEM_ARMOR_SHARD,
	ENT_ITEM_HEALTH,
	ENT_ITEM_HEALTH_LARGE,
	ENT_ITEM_HEALTH_SMALL,
	ENT_ITEM_HEALTH_MEGA,
	ENT_ITEM_QUAD,
	ENT_ITEM_ENVIRO,
	ENT_ITEM_FLIGHT,
	ENT_ITEM_HASTE,
	ENT_ITEM_INVIS,
	ENT_ITEM_REGEN,
	ENT_ITEM_BOT_ROAM,
	ENT_TEAM_CTF_BLUEFLAG,
	ENT_TEAM_CTF_REDFLAG,
	ENT_AMMO_BULLETS,
	ENT_AMMO_ROCKETS,
	ENT_AMMO_SLUGS,
	ENT_AMMO_SHELLS,
	ENT_AMMO_CELLS,
	ENT_AMMO_LIGHTNING,
	ENT_AMMO_BFG,
	ENT_AMMO_GRENADES,
	ENT_AMMO_PLASMA,
	ENT_WEAPON_ROCKETLAUNCHER,
	ENT_WEAPON_LIGHTNING,
	ENT_WEAPON_BFG,
	ENT_WEAPON_SHOTGUN,
	ENT_WEAPON_MACHINEGUN,
	ENT_WEAPON_RAILGUN,
	ENT_WEAPON_PLASMA,
	ENT_WEAPON_GRENADELAUNCHER,
	ENT_HOLDABLE_MEDKIT,
	ENT_HOLDABLE_TELEPORTER,
	ENT_INFO_PLAYER_INTERMISSION,
	ENT_INFO_PLAYER_START,
	ENT_INFO_PLAYER_DEATHMATCH,
	ENT_INFO_SPECTATOR_START,
	ENT_INFO_FIRSTPLACE,
	ENT_INFO_SECONDPLACE,
	ENT_INFO_THIRDPLACE,
	ENT_INFO_NOTNULL,
	ENT_INFO_NULL,
	ENT_INFO_CAMP,
	ENT_FUNC_BOBBING,
	ENT_FUNC_BUTTON,
	ENT_FUNC_DOOR,
	ENT_FUNC_PLAT,
	ENT_FUNC_PENDULUM,
	ENT_FUNC_ROTATING,
	ENT_FUNC_TRAIN,
	ENT_FUNC_STATIC,
	ENT_FUNC_TIMER,
	ENT_TARGET_TELEPORTER,
	ENT_TARGET_POSITION,
	ENT_TARGET_GIVE,
	ENT_TARGET_RELAY,
	ENT_TARGET_DELAY,
	ENT_TARGET_SPEAKER,
	ENT_TARGET_REMOVE_POWERUPS,
	ENT_TARGET_LOCATION,
	ENT_TRIGGER_HURT,
	ENT_TRIGGER_TELEPORT,
	ENT_TRIGGER_PUSH,
	ENT_TRIGGER_ALWAYS,
	ENT_TRIGGER_MULTIPLE,
	ENT_MISC_MODEL,
	ENT_MISC_PORTAL_SURFACE,
	ENT_MISC_PORTAL_CAMERA,
	ENT_MISC_TELEPORTER_DEST,
	ENT_LIGHT,
	ENT_WORLDSPAWN,
	ENT_SHOOTER_GRENADE,
	ENT_NAVPOINT
}entity_type;

class Entity
{
public:
	Entity();
	~Entity();

	vec3			position;
	bool			visible;
	bool			frustum_visible;
	bool			bsp_visible;
	int				bsp_leaf;
	bool			nodraw;
	bool			network;
	RigidBody		*rigid;
	Model			*model;
	Light			*light;
	Speaker			*speaker;
	Trigger			*trigger;
	Player			*player;

	//Really bsp related items
	int				ent_type; // will replace string type eventually
	nettype_t		nettype;
	int				angle;
	int				model_ref;
	float			height; // for movers will make own class later

	bool particle_on;
	int num_particle;

	char			type[64];
	char			target[64];
	char			target_name[64];
};
#endif
