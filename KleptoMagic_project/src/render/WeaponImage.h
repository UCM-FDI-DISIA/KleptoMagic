#pragma once
#include "../ecs/Component.h"
#include "../sdlutils/SDLUtils.h"
#include "../ecs/Transform.h"
#include "../ecs/Manager.h"
#include "../sdlutils/NewInputHandler.h"

class WeaponImage : public ecs::Component {
    public:
        __CMPID_DECL__(ecs::cmp::WEAPONIMAGE)
    
        WeaponImage();
        WeaponImage(Texture* tex, float radius, int textRow);
    
        virtual ~WeaponImage() {}
    
        void initComponent() override;
        void update() override;
        void render() override;
    
        void setRadius(float r) { _radius = r; }
        void setTexture(Texture* tex) { _tex = tex; }
    
    private:
        Texture* _tex;
        Transform* _tr;
        ecs::Manager* _mngr;
        float _radius;
        float _angleDeg;
        bool _flip;
        int _texRow;
        Vector2D _weaponPos;
    };
    