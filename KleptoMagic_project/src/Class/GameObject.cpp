#include "GameObject.h"

GameObject::GameObject(GameState* state/*, Texture* tex = nullptr */) {
    this->state = state;
    //this->texture = tex;
}

GameObject::~GameObject()
{
}

void GameObject::render() const
{
}

void GameObject::update()
{
}
