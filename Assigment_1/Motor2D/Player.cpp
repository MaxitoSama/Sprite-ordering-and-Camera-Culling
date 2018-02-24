#include "Player.h"
#include "j1Window.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Audio.h"
#include "j1Textures.h"
#include "j1Colliders.h"
#include "p2Point.h"
#include "j1Render.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Pathfinding.h"


Player::Player(int x, int y, ENTITY_TYPES type) : Entity(x, y,type)
{
	LOG("Loading Player Sheet");

	//IDLE RIGHT----------------------------------------------------
	{
		idle_right.PushBack({ 64,111,176,331 });
		idle_right.PushBack({ 310,111,176,331 });
		idle_right.PushBack({ 575,111,176,331 });
		idle_right.PushBack({ 845,111,176,331 });
		idle_right.PushBack({ 1138,111,176,331 });
		idle_right.PushBack({ 1450,111,176,331 });
		idle_right.PushBack({ 1735,111,176,331 });
		idle_right.PushBack({ 2019,111,176,331 });
		idle_right.PushBack({ 2284,111,176,331 });
		idle_right.PushBack({ 2559,111,176,331 });

		idle_right.loop = true;
		idle_right.speed = 30.0f;
	}

	//IDLE LEFT----------------------------------------------------
	{
		idle_left.PushBack({ 64,1159,176,329 });
		idle_left.PushBack({ 311,1159,176,329 });
		idle_left.PushBack({ 577,1159,176,329 });
		idle_left.PushBack({ 848,1159,176,329 });
		idle_left.PushBack({ 1142,1159,176,329 });
		idle_left.PushBack({ 1455,1159,176,329 });
		idle_left.PushBack({ 1739,1159,176,329 });
		idle_left.PushBack({ 2022,1159,176,329 });
		idle_left.PushBack({ 2286,1159,176,329 });
		idle_left.PushBack({ 2560,1159,176,329 });

		idle_left.loop = true;
		idle_left.speed = 30.0f;
	}


	// MOVE RICHT----------------------------------------------------
	{
		right.PushBack({ 64,472,263,332 });
		right.PushBack({ 327,472,263,332 });
		right.PushBack({ 590,472,263,332 });
		right.PushBack({ 853,472,263,332 });
		right.PushBack({ 1137,472,263,332 });
		right.PushBack({ 1449,472,263,332 });
		right.PushBack({ 1734,472,263,332 });
		right.PushBack({ 2019,472,263,332 });
		right.PushBack({ 2286,472,263,332 });
		right.PushBack({ 2560,472,263,332 });

		right.loop = true;
		right.speed = 30.0f;
	}

	// MOVE LEFT----------------------------------------------------
	{
		left.PushBack({ 2611,813,283,341 });
		left.PushBack({ 2328,813,283,341 });
		left.PushBack({ 2045,813,283,341 });
		left.PushBack({ 1762,813,283,341 });
		left.PushBack({ 1479,813,283,341 });
		left.PushBack({ 1196,813,283,341 });
		left.PushBack({ 913,813,283,341 });
		left.PushBack({ 630,813,283,341 });
		left.PushBack({ 347,813,283,341 });
		left.PushBack({ 64,813,283,341 });

		left.loop = true;
		left.speed = 30.0f;
	}

	//Init Screen vars----------------------------------------------------
	win_width = App->win->screen_surface->w;
	win_height = App->win->screen_surface->h;
	win_scale = App->win->GetScale();
	score = 0;
	coins = 0;
	lives = 3;

	animation = &idle_right;
	scale = 0.5f;
	initial_pos.x = original_pos.x;
	Curr_map = 1;
}

bool Player::Awake(pugi::xml_node& entity_config)
{
	LOG("Init Player config");
	pugi::xml_node player = entity_config.child("player");

	//Init position var----------------------------------------------------
	original_pos.x = player.child("position").attribute("pos_x").as_float(10);
	original_pos.y = player.child("position").attribute("pos_y").as_float(10);

	//Init Velocity var----------------------------------------------------
	gravity = player.child("gravity").attribute("value").as_uint(10);
	acceleration = player.child("acceleration").attribute("value").as_uint(0);
	accel_counter = player.child("accel_counter").attribute("value").as_uint(0);
	velocity = player.child("velocity").attribute("value").as_float(1);
	slide_counter = player.child("slide_counter").attribute("value").as_uint(0);

	//Jump vars----------------------------------------------------
	jump_height = player.child("jump").attribute("jump_height").as_uint(300);
	jump_vel = player.child("jump").attribute("jump_vel").as_uint(10);
	fall = player.child("jump_bool").attribute("value").as_bool(false);
	Jump = player.child("fall_bool").attribute("value").as_bool(false);

	bool ret = true;

	return ret;
}


Player::~Player()
{
	LOG("Unloading player Sheet");
	App->tex->UnLoad(godmode);

	LOG("Destroying Player Collider");
	if (collider_feet != nullptr)
		collider_feet->to_delete = true;
	if (collider_player != nullptr)
		collider_player->to_delete = true;
}

bool Player::Start()
{
	LOG("Loading Player Sheet");

	godmode = App->tex->Load("assets/character/god_mode.png");

	LOG("Loading Player Collider");
	collider = App->colliders->AddCollider({ (int)original_pos.x, (int)original_pos.y, 200 / 4, 332 / 2 }, COLLIDER_PLAYER, (j1Module*)App->entities);
	collider_feet = App->colliders->AddCollider({ ((int)original_pos.x + (263 / 4) - 54), (int)original_pos.y + (310 / 2) - 1, 64, 10 }, COLLIDER_FEET, (j1Module*)App->entities);

	//Init Screen vars----------------------------------------------------
	win_width = App->win->screen_surface->w;
	win_height = App->win->screen_surface->h;
	win_scale = App->win->GetScale();

	//GOD = false;
	
	return true;
}

bool Player::Update(float dt)
{
	SDL_Event e;

	//MOVE_LEFT----------------------------------------------------
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE)
		{
			if (original_pos.x >= 0)
			{
				speed.x = -(velocity)*dt;
				original_pos.x += speed.x;
			}

			if (animation != &left && !Jump)
			{
				left.Reset();
				animation = &left;
				player_last_direction = LEFT;
			}
		}
		else
		{
			animation = &idle_left;
		}

	}

	//MOVE_RIGHT---------------------------------------------------
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE)
		{
			if (original_pos.x < 25600)
			{
				speed.x = (velocity)*dt;
				original_pos.x += speed.x;
			}

			if (animation != &right && !Jump)
			{
				right.Reset();
				animation = &right;
				player_last_direction = RIGHT;
			}
		}
		else
		{
			animation = &idle_right;
		}

	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		if (original_pos.x >= 0)
		{
			speed.y = velocity*dt;
			original_pos.y += speed.y;
		}
		if (animation != &right)
		{
			right.Reset();
			animation = &right;
			player_last_direction = RIGHT;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		if (original_pos.x >= 0)
		{
			speed.y = -(velocity)*dt;
			original_pos.y += speed.y;
		}
		if (animation != &right)
		{
			right.Reset();
			animation = &right;
			player_last_direction = RIGHT;
		}
	}

	//IDLE ANIMATIONS------------------------------------------
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_IDLE
		&& App->input->GetKey(SDL_SCANCODE_UP) == KEY_IDLE
		&& App->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE
		&& App->input->GetKey(SDL_SCANCODE_W) == KEY_IDLE
		&& App->input->GetKey(SDL_SCANCODE_S) == KEY_IDLE
		&& App->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE
		&& App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_IDLE
		&& !dead)
	{
		switch (player_last_direction)
		{
		case Player::UP:
			animation = &idle_right;
			break;
		case Player::DOWN:
			animation = &idle_right;
			break;
		case Player::LEFT:
			animation = &idle_left;
			break;
		case Player::RIGHT:
			animation = &idle_right;
			break;
		default:
			break;
		}
	}

	//Player Colliders Position--------------------------------
	position = original_pos;

	return true;
}


bool Player::Load(pugi::xml_node& data)
{
	int map = data.child("player").attribute("Map").as_int();
	int x = data.child("player").attribute("x").as_int();
	int y = data.child("player").attribute("y").as_int();

	score = data.child("player").attribute("score").as_uint();
	lives = data.child("player").attribute("lives").as_uint();
	coins = data.child("player").attribute("coins").as_uint();

	return true;
}

bool Player::Save(pugi::xml_node& data) const
{
	pugi::xml_node playernode = data.append_child("player");

	playernode.append_attribute("x") = original_pos.x;
	playernode.append_attribute("y") = original_pos.y;
	playernode.append_attribute("Map") = Curr_map;
	playernode.append_attribute("score") = score;
	playernode.append_attribute("lives") = lives;
	playernode.append_attribute("coins") = coins;

	return true;
}