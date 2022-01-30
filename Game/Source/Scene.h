#ifndef __SCENE_H__
#define __SCENE_H__


#include "App.h"
#include "GameScene.h"
#include "MenuScene.h"
#include "GuiControl.h"

struct SDL_Texture;

enum background {
	Blue = 0,
	Brown,
	Gray,
	Green,
	Pink,
	Purple,
	Yellow
};

enum class CScene
{
	NONE = 0, 
	GAMESCENE,
	MENUSCENE,
	GAMESCENELOAD
};

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
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start() ;

	// Called before all Updates
	bool PreUpdate() ;

	// Called each loop iteration
	bool Update(float dt) ;

	// Called before all Updates
	bool PostUpdate() ;

	// Called before quitting
	bool CleanUp();

	bool SaveSettings(pugi::xml_node& config);

	void OnCollision(Collider* c1, Collider* c2) override;

	void WillCollision(Collider* c1, Collider* c2) override;

	bool LoadState(pugi::xml_node&) override;

	bool SaveState(pugi::xml_node&) const override;

	bool OnGuiMouseClickEvent(GuiControl* control);

	void changeScene(CScene scene = CScene::NONE);
	
private:
	void bgSelector();

	void drawBackground();

public:
	GameScene* gameScene = nullptr;
	MenuScene* menuScene = nullptr;

	CScene cScene = CScene::MENUSCENE;
	bool changeScenePetition = false;
	CScene nextScene;

	bool exitPetition = false;

	bool debugTiles = false;

	int highScoreI = 0;

private:
	SDL_Texture* img;

	List<Level*> levelList;

	SDL_Texture* bgTex = nullptr;
	SString bg;

	int bgPivX = 0;
	int bgPivY = -128;
};

#endif // __SCENE_H__