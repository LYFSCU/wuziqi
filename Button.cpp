#include "Button.h"

bool Button::isHover() const
{
	MOUSEMSG msg = GetMouseMsg();
	return(msg.x >= x && msg.x <= x + width &&
		msg.y >= y && msg.y <= y + height);
}

void Button::draw() const
{
	setfillcolor(isHover() ? 0xDDDDDD : 0xFFFFFF);//悬停时颜色变化
	solidrectangle(x, y, x + width, y + height);//?

	setbkmode(TRANSPARENT);
	settextcolor(BLACK);//文本颜色
	outtextxy(x + 10, y + (height - textheight(text)) / 2, text);//?
}
