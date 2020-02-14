#include "gameObj.h"
#include "glueToBack.h"

// glue b to back of a
void glueToBack(gameObj& a, gameObj &b)
{
	switch(a.lastMove) // place snake block behind tail
	{
		case game::UP:
			b.rect.x = a.rect.getRectL();
			b.rect.y = a.rect.getRectBottom();
		break;

		case game::DOWN:
			b.rect.x = a.rect.getRectL();
			b.rect.y = a.rect.getRectTop() - a.rect.h;
		break;

		case game::LEFT:
			b.rect.x = a.rect.getRectR();
			b.rect.y = a.rect.getRectTop();
		break;

		case game::RIGHT:
			b.rect.x = a.rect.getRectL() - a.rect.w;
			b.rect.y = a.rect.getRectTop();
		break;
	}
}
