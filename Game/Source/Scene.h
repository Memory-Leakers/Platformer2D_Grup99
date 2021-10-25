#ifndef __SCENE_H__
#define __SCENE_H__


#include "App.h"
#include "GameScene.h"

struct SDL_Texture;

struct Level
{
	Level(int levelNum, SString file)
	{
		this->levelNum = levelNum;
		this->file = file;
	}
	~Level()
	{

	}

	int levelNum; //Number of the level
	SString file;
	int camX = 0, camY = 0;
};


class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start() ;

	// Called before all Updates
	bool PreUpdate() ;

	// Called each loop iteration
	bool Update(float dt) ;

	// Called before all Updates
	bool PostUpdate() ;

	// Called before quitting
	bool CleanUp() ;

public:
	GameScene* gameScene = nullptr;

private:
	SDL_Texture* img;

	List<Level*> levelList;

};

#endif // __SCENE_H__