#include "gameObj.h"
#include "glueToBack.h"

// glue b to back of a
void glueToBack(gameObj& a, gameObj &b)
{
	switch(a.lastMove) // place snake block behind tail
	{
		case game::UP:
			b.rect.x = a.getRectL();
			b.rect.y = a.getRectBottom();
		break;

		case game::DOWN:
			b.rect.x = a.getRectL();
			b.rect.y = a.getRectTop() - a.rect.h;
		break;

		case game::LEFT:
			b.rect.x = a.getRectR();
			b.rect.y = a.getRectTop();
		break;

		case game::RIGHT:
			b.rect.x = a.getRectL() - a.rect.w;
			b.rect.y = a.getRectTop();
		break;
	}
}
