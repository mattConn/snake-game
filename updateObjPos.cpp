#include "gameObj.h"
#include "updateObjPos.h"

void updateObjPos(gameObj& obj)
{
	switch(obj.lastMove)
	{
		case game::UP:
			if(obj.getRectTop() > 0)
				obj.rect.y += (-obj.velocity * obj.velocityMod);
		break;

		case game::DOWN:
			if(obj.getRectBottom() < game::SCREEN_HEIGHT)
				obj.rect.y += (obj.velocity * obj.velocityMod);
		break;

		case game::LEFT:
			if(obj.getRectL() > 0)
				obj.rect.x += (-obj.velocity * obj.velocityMod);
		break;

		case game::RIGHT:
			if(obj.getRectR() < game::SCREEN_WIDTH)
				obj.rect.x += (obj.velocity * obj.velocityMod);
		break;
	}
}
