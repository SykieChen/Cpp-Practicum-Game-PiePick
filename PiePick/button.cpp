#include "pie.h"

button::button(int w, int h, int x, int y, IMAGE* bg, IMAGE* bgx) {
	this->width = w;
	this->height = h;
	this->x = x;
	this->y = y;
	this->bg = bg;
	this->bgx = bgx;
	show();
}

void button::show() {
	transimg(x, y, bg, bgx);
}

bool button::chkRange(int mouseX, int mouseY) {
	if (mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height)
		return true;
	else return false;
}
