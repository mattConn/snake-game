#include "gameObj.h"
#include "updateObjPos.h"

void updateObjPos(gameObj& obj)
{
	switch(obj.lastMove)
	{
		case game::UP:
			if(obj.rect.getRectTop() > 0)
				obj.rect.y += (-obj.velocity * obj.velocityMod);
		break;

		case game::DOWN:
			if(obj.rect.getRectBottom() < game::SCREEN_HEIGHT)
				obj.rect.y += (obj.velocity * obj.velocityMod);
		break;

		case game::LEFT:
			if(obj.rect.getRectL() > 0)
				obj.rect.x += (-obj.velocity * obj.velocityMod);
		break;

		case game::RIGHT:
			if(obj.rect.getRectR() < game::SCREEN_WIDTH)
				obj.rect.x += (obj.velocity * obj.velocityMod);
		break;
	}
}
