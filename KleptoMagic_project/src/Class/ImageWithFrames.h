#pragma once
#include "../ecs/Component.h"

class Transform;
class Texture;

class ImageWithFrames : public ecs::Component {
private:
	Transform* _tr;
	Texture* _tex;
public:

	__CMPID_DECL__(ecs::cmp::IMAGEWITHFRAMES)

	// Parámetros de animación
	int totalFrames_;      // Total de frames en la spritesheet
	int frameWidth_;       // Ancho de cada frame
	int frameHeight_;      // Alto de cada frame
	int currentFrame_;     // Frame actual (0 a totalFrames-1)
	float frameTime_;      // Tiempo entre frames (en ms)
	float currentTime_;    // Tiempo acumulado
	int cols_;
	int rows_;


	ImageWithFrames();
	ImageWithFrames(Texture* tex, int cols, int rows);
	virtual ~ImageWithFrames();

	void setTexture(Texture* tex) {
		_tex = tex;
	}

	void initComponent() override;
	void render() override;
	void update() override;


};
