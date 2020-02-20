#include "gameObj.h"
#include "updateObjPos.h"

void updateObjPos(gameObj& obj)
{
	switch(obj.move)
	{
		case game::UP:
			if(obj.rect.getTop() > 0)
				obj.rect.y += (-obj.velocity * obj.velocityMod);
		break;

		case game::DOWN:
			if(obj.rect.getBottom() < game::SCREEN_HEIGHT)
				obj.rect.y += (obj.velocity * obj.velocityMod);
		break;

		case game::LEFT:
			if(obj.rect.getL() > 0)
				obj.rect.x += (-obj.velocity * obj.velocityMod);
		break;

		case game::RIGHT:
			if(obj.rect.getR() < game::SCREEN_WIDTH)
				obj.rect.x += (obj.velocity * obj.velocityMod);
		break;
	}
}
