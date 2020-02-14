#include "gameObj.h"
#include "glueToBack.h"

// glue b to back of a
void glueToBack(gameObj& a, gameObj &b)
{
	switch(a.lastMove) // place snake block behind tail
	{
		case game::UP:
			b.rect.x = a.rect.getL();
			b.rect.y = a.rect.getBottom();
		break;

		case game::DOWN:
			b.rect.x = a.rect.getL();
			b.rect.y = a.rect.getTop() - a.rect.h;
		break;

		case game::LEFT:
			b.rect.x = a.rect.getR();
			b.rect.y = a.rect.getTop();
		break;

		case game::RIGHT:
			b.rect.x = a.rect.getL() - a.rect.w;
			b.rect.y = a.rect.getTop();
		break;
	}
}
