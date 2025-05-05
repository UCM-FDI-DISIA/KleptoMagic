#include "WeaponImage.h"

WeaponImage::WeaponImage() :
    _tex(nullptr), _tr(nullptr), _radius(40.0f), _angleDeg(0), _flip(false) {}

WeaponImage::WeaponImage(Texture* tex, float radius) :
    _tex(tex), _tr(nullptr), _radius(radius), _angleDeg(0), _flip(false) {}

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

    Vector2D aim = input().getAimVector(playerCenter);

    if (aim.magnitude() > 0) {
        aim = aim.normalize();
        _weaponPos = playerCenter + aim * _radius;

        _flip = (aim.getX() < 0); 

        _angleDeg = aim.angle(Vector2D(1, 0));
        if (_flip) {
            _angleDeg += 45.0f; 
        }
        else {
            _angleDeg -= 45.0f; 
        }
    }
}

void WeaponImage::render() {
    assert(_tex != nullptr);

    float w = static_cast<float>(_tex->width());
    float h = static_cast<float>(_tex->height());
    Vector2D pos = _weaponPos - Vector2D(w / 2, h / 2);

    SDL_Rect dest = build_sdlrect(pos, w, h);

    
    _tex->render({0, 0, _tex->getWidth(), _tex->getHeight()},dest, -_angleDeg, nullptr, _flip ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE);
}
