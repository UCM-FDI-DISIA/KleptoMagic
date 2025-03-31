#include "DestroyOnBorder.h"
DestroyOnBorder::DestroyOnBorder()
{
}

DestroyOnBorder::~DestroyOnBorder()
{
}

void DestroyOnBorder::initComponent()
{
	_tr = game().getMngr()->getComponent<Transform>(this->_ent);
}
void DestroyOnBorder::update()
{

	auto& pos = _tr->getPos();

	auto height_ = _tr->getHeight();
	auto side_ = _tr->getWidth();

	if(pos.getX()<-side_||pos.getX()>sdlutils().width()+side_||pos.getY()<-height_||pos.getY()>sdlutils().height()+height_)
	{
		std::cout << "se elimino";
		game().getMngr()->setAlive(this->_ent, false);
	}
}
