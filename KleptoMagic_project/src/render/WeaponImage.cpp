#include "WeaponImage.h"
#include "Camera.h"

WeaponImage::WeaponImage() :
    _tex(nullptr), _tr(nullptr), _radius(40.0f), _angleDeg(0), _flip(false), _texRow(0) {}

WeaponImage::WeaponImage(Texture* tex, float radius, int texRow) :
    _tex(tex), _tr(nullptr), _radius(radius), _angleDeg(0), _flip(false), _texRow(texRow) {}

void WeaponImage::initComponent() {
    _mngr = _ent->getMngr();
    _tr = _mngr->getComponent<Transform>(_mngr->getHandler(ecs::hdlr::PLAYER));
    assert(_tr != nullptr);
}

void WeaponImage::update() {
    Vector2D playerCenter = {
        _tr->getPos().getX() + _tr->getWidth() / 2,
        _tr->getPos().getY() + _tr->getHeight() / 2
    };

    Vector2D aim = input().getAimVector(playerCenter - camOffset);

    if (aim.magnitude() > 0) {
        aim = aim.normalize();
        _weaponPos = playerCenter + aim * _radius;

        _flip = (aim.getX() < 0); 

        _angleDeg = aim.angle(Vector2D(1, 0));
    }
    else {
        _weaponPos = Vector2D(0, 0); 
    }
}

void WeaponImage::render() {
    assert(_tex != nullptr);

    if (_weaponPos.getX() == 0 && _weaponPos.getY() == 0) {
        return; 
    }
    float w = 32.0f; // Assuming the width of the weapon image is 32 pixels
    float h = 32.0f; // Assuming the height of the weapon image is 32 pixels
    Vector2D pos = _weaponPos - Vector2D(w / 2, h / 2) - camOffset;

    SDL_Rect dest = build_sdlrect(pos, w, h);

    
    _tex->render({0, _texRow * 32, 32, 32},dest, -_angleDeg, nullptr, _flip ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE);
}
