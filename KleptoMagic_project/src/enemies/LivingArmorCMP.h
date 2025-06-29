#include "../ecs/Component.h"
#include "../ecs/ecs.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../ecs/ecs_defs.h"
#include "../ecs/Transform.h"
#include "../sdlutils/SDLUtils.h"
#include <chrono>
#include <random>

namespace ecs
{
    class ArmorVectorComponent : public Component {
    private:
        void createStart();

        Transform* _armorTransform;
        Transform* _playerTransform;

    public:
        __CMPID_DECL__(ecs::cmp::ARMORVECCMP);
        Vector2D direction;
        Vector2D lastDirection;  // NUEVA VARIABLE
        Vector2D _dest;
        bool isCharging = false;
        std::chrono::steady_clock::time_point lastStoppedTime;

        void initComponent() override {
            _armorTransform = _ent->getMngr()->getComponent<Transform>(_ent);
            _playerTransform = _ent->getMngr()->getComponent<Transform>(
                _ent->getMngr()->getHandler(ecs::hdlr::PLAYER));
            SelectDirection();  // Selecciona primera dirección
        }

        void SelectDirection(Vector2D avoidDir = Vector2D(0, 0)) {
            std::vector<Vector2D> possibleDirections = {
                Vector2D(0, -1),  // Arriba
                Vector2D(0, 1),   // Abajo
                Vector2D(1, 0),   // Derecha
                Vector2D(-1, 0)   // Izquierda
            };

            if (avoidDir != Vector2D(0, 0)) {
                possibleDirections.erase(
                    std::remove_if(possibleDirections.begin(), possibleDirections.end(),
                        [&](const Vector2D& v) {
                            return (avoidDir.getX() != 0 && v.getX() == avoidDir.getX()) ||
                                (avoidDir.getY() != 0 && v.getY() == avoidDir.getY());
                        }),
                    possibleDirections.end()
                );
            }

            if (!possibleDirections.empty()) {
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<int> dist(0, possibleDirections.size() - 1);
                lastDirection = direction; // GUARDAMOS DIRECCIÓN ANTERIOR
                direction = possibleDirections[dist(gen)];
            }
            else {
                direction = avoidDir * -1;
            }

            isCharging = true;
            std::cout << "[NEW DIRECTION] Selected: (" << direction.getX() << "," << direction.getY() << ")\n";
        }
    };

    class ArmorStatComponent : public Component {
    public:
        __CMPID_DECL__(ecs::cmp::ARMORSTATCMP);
        float normalSpeed = 0.1f;
        float chargeSpeed = 5.0f;
        float damage = 1;
        float chargeCooldown = 3000;
        void initComponent() override {}
        void update() override {}
    };

    class ArmorAttackComponent : public Component
    {
        Transform* _armorTransform;
        Transform* _player;
        bool hitPlayer = false;
        Manager* _mngr;
    public:
        __CMPID_DECL__(ecs::cmp::ARMORATKCMP);
        std::chrono::steady_clock::time_point lastChargeTime = std::chrono::steady_clock::now();

        void initComponent() override {
            _mngr = _ent->getMngr();
            _armorTransform = _mngr->getComponent<Transform>(_ent);
            _player = _mngr->getComponent<Transform>(_mngr->getHandler(ecs::hdlr::PLAYER));
        }

        bool CheckPlayerCollision() {
            Vector2D armorPos = _armorTransform->getPos();
            Vector2D playerPos = _player->getPos();

            float armorLeft = armorPos.getX();
            float armorRight = armorPos.getX() + _armorTransform->getWidth();
            float armorTop = armorPos.getY();
            float armorBottom = armorPos.getY() + _armorTransform->getHeight();

            float playerLeft = playerPos.getX();
            float playerRight = playerPos.getX() + _player->getWidth();
            float playerTop = playerPos.getY();
            float playerBottom = playerPos.getY() + _player->getHeight();

            return !(armorRight < playerLeft ||
                armorLeft > playerRight ||
                armorBottom < playerTop ||
                armorTop > playerBottom);
        }

        void CheckPlayerHit() {
            if (CheckPlayerCollision()) {
                auto now = std::chrono::steady_clock::now();
                auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastHitTime).count();

                if (!hitPlayer && elapsedTime > 500) { // Small cooldown to prevent multiple hits
                    hitPlayer = true;
                    lastHitTime = now;

                    // Damage player
                    auto player = _mngr->getHandler(ecs::hdlr::PLAYER);
                    if (_mngr->isAlive(player)) {
                        auto* playerStats = _mngr->getComponent<EntityStat>(player);
                        if (playerStats != nullptr) {
                            auto stat = _mngr->getComponent<ArmorStatComponent>(_ent);
                            playerStats->ChangeStat(-stat->damage, EntityStat::Stat::HealthCurrent);
                        }
                    }
                }
            }
        }

        void update() override
        {
            auto stat = _mngr->getComponent<ArmorStatComponent>(_ent);
            auto vector = _mngr->getComponent<ArmorVectorComponent>(_ent);

            // Solo verificar golpes al jugador cuando está cargando
            if (vector->isCharging) {
                CheckPlayerHit();
            }
        }

    private:
        std::chrono::steady_clock::time_point lastHitTime = std::chrono::steady_clock::now();
    };

    class ArmorMovementComponent : public Component
    {
        Transform* _armorTransform;
        Transform* _player;
        TileCollisionChecker* _tileChecker;
        Manager* _mngr;
        Vector2D lastPosition;  // Guarda la última posición del Armor

    public:
        __CMPID_DECL__(ecs::cmp::ARMORMOVCMP);

        void initComponent() override {
            _mngr = _ent->getMngr();
            _armorTransform = _mngr->getComponent<Transform>(_ent);  // Obtener el componente Transform del Armor
            _player = _mngr->getComponent<Transform>(_mngr->getHandler(ecs::hdlr::PLAYER));  // Obtener el Transform del jugador
            _tileChecker = _mngr->getComponent<TileCollisionChecker>(_ent);  // Intentamos obtener TileCollisionChecker desde la misma entidad
            lastPosition = _armorTransform->getPos();  // Guardar la posición inicial
        }

        void update() override {
            auto vector = _mngr->getComponent<ArmorVectorComponent>(_ent);
            auto stat = _mngr->getComponent<ArmorStatComponent>(_ent);

            if (vector->isCharging) {
                Vector2D dir = vector->direction;
                _armorTransform->getVel() = dir * stat->chargeSpeed;

                // Guardar la posición actual para la siguiente comparación
                Vector2D currentPosition = _armorTransform->getPos();

                if (_tileChecker) {
                    bool collisionX = (dir.getX() != 0 && !_tileChecker->getCanMoveX());
                    bool collisionY = (dir.getY() != 0 && !_tileChecker->getCanMoveY());

                    if (collisionX || collisionY) {
                        _armorTransform->getVel() = Vector2D(0, 0);
                        vector->isCharging = false;
                        vector->lastStoppedTime = std::chrono::steady_clock::now();

                        std::cout << "[WALL HIT] Chocó con la pared. Esperando cooldown...\n";
                    }
                }

                // Verificar si el armor no ha avanzado (se ha quedado estancado)
                if (currentPosition == lastPosition) {
                    auto now = std::chrono::steady_clock::now();
                    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - vector->lastStoppedTime).count();

                    if (elapsed >= stat->chargeCooldown) {
                        Vector2D avoidDir = vector->direction; // dirección que causó la colisión
                        vector->SelectDirection(avoidDir);     // no la reutilizamos como base, solo evitamos
                        vector->isCharging = true;

                        std::cout << "[RECHARGE] Seleccionando nueva dirección tras esperar " << stat->chargeCooldown << " ms\n";
                    }
                }

                // Actualizar la última posición
                lastPosition = currentPosition;
            }
        }
    };

    class ArmorAnimComponent : public AnimatorComponent {
        friend AnimatorComponent;
    public:
        __CMPID_DECL__(ecs::cmp::ARMORANIMCMP);

        ArmorAnimComponent() : lastDirectionChangeTime(std::chrono::steady_clock::now()) {}

        void update() override {
            Vector2D velocity = _tr->getVel();

            // Solo cambiar animación si está moviéndose
            if (velocity.magnitude() > 0.01f) {
                char newDir;

                // Movimiento vertical tiene prioridad si es más fuerte
                if (std::abs(velocity.getY()) >= std::abs(velocity.getX())) {
                    if (velocity.getY() > 0)
                        newDir = 'D'; // Sur
                    else
                        newDir = 'U'; // Norte
                }
                else {
                    if (velocity.getX() > 0)
                        newDir = 'R'; // Este
                    else
                        newDir = 'L'; // Oeste
                }

                // Solo cambiar de dirección si es necesario y ha pasado suficiente tiempo
                auto now = std::chrono::steady_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastDirectionChangeTime).count();

                if (newDir != currentDir && elapsed >= directionChangeCooldown) {
                    changeDirection(newDir);
                    lastDirectionChangeTime = now; // Actualizamos el tiempo del último cambio de dirección
                }
            }
        }

        void changeDirection(char dir) {
            // Si la dirección no ha cambiado, no actualizamos nada
            if (dir == currentDir)
                return;

            currentDir = dir;

            switch (dir) {
            case 'D': offset = 0; break; // Sur
            case 'U': offset = 1; break; // Norte
            case 'R': offset = 2; break; // Este
            case 'L': offset = 3; break; // Oeste
            default: offset = 0; break;
            }

            // Aseguramos que el número de frames es 4 por dirección
            _img->setStartingFrame(startFrame + offset * 4);  // 4 frames por fila
            _img->setNumFrames(4);

            std::cout << "[ANIM CHANGE] Nueva dirección: " << dir << "\n"; // Para depuración
        }

        char facingDirection() const {
            return currentDir;
        }

    private:
        int offset = 0;
        char currentDir = 'D'; // Dirección inicial: sur
        std::chrono::steady_clock::time_point lastDirectionChangeTime;  // Tiempo del último cambio
        const int directionChangeCooldown = 100;  // Tiempo en milisegundos que debe pasar para cambiar de dirección

        void createStart();
    };
}