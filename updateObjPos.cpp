#include "gameObj.h"
#include "updateObjPos.h"

void updateObjPos(gameObj& obj)
{
	switch(obj.lastMove)
	{
		case global::UP:
			if(obj.getRectTop() > 0)
				obj.rect.y += (-obj.velocity * obj.velocityMod);
		break;

		case global::DOWN:
			if(obj.getRectBottom() < global::SCREEN_HEIGHT)
				obj.rect.y += (obj.velocity * obj.velocityMod);
		break;

		case global::LEFT:
			if(obj.getRectL() > 0)
				obj.rect.x += (-obj.velocity * obj.velocityMod);
		break;

		case global::RIGHT:
			if(obj.getRectR() < global::SCREEN_WIDTH)
				obj.rect.x += (obj.velocity * obj.velocityMod);
		break;
	}
}
