#pragma once
#include "Entity.h"

enum class InteractablesId
{
    NONE= 0,
    CHECKPOINT,
    TROPHY,
    SPIKETRAP_LEFT,
    SPIKETRAP_RIGHT,
    SPIKETRAP_TOP,
    SPIKETRAP_BOTTOM
};

class Interactable :
    public Entity
{
public:
    Interactable();

    virtual ~Interactable();


    virtual bool Start() override;
    virtual bool CleanUp() override;

    virtual bool PreUpdate() override;
    virtual bool Update(float dt) override;
    virtual bool PostUpdate() override;

    virtual void OnCollision(Collider* col) override;

    virtual void WillCollision() override;

    virtual void stateMachine();

    virtual void Save(pugi::xml_node& data) const override;

protected:

    

public:

    InteractablesId id = InteractablesId::NONE;

    bool collided = false;

    bool stateChanged = true;

public:

    void setStateChanged(bool value)
    {
        stateChanged = value;
    }
};

