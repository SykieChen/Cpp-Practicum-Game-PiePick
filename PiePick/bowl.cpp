#include "pie.h"

bowl::bowl(int x, int y, IMAGE* ilmario, IMAGE* ilmariox, IMAGE* irmario, IMAGE* irmariox, IMAGE* main_bg) {
	this->x = x;
	this->y = y;
	this->ilmario = ilmario;
	this->ilmariox = ilmariox;
	this->irmario = irmario;
	this->irmariox = irmariox;
	this->main_bg = main_bg;
	this->w = irmario->getwidth();
	this->h = irmario->getheight();
	show();
}
int bowl::getX() {
	return x;
}
void bowl::moveL() {
	x -= 3;
	isFacingL = true;
	show();
}
void bowl::moveR() {
	x += 3;
	isFacingL = false;
	show();
}
void bowl::show() {
	if (isFacingL)
		transimg(x, y, ilmario, ilmariox);
	else
		transimg(x, y, irmario, irmariox);
}
int bowl::getW() {
	return w;
}
int bowl::getH() {
	return h;
}
void bowl::hide() {
	repaintBlock(x, y, x + w, y + h, main_bg);
}
void bowl::reset(int x) {
	this->x = x;
	this->isFacingL = false;
	show();
}
bool bowl::ifCaught(ballNode* ball0) {
	if (ball0->item.isVisible &&
		ball0->item.getX() <= x + w &&
		ball0->item.getX() + ball0->item.getW() >= x) {

		ball0->item.isVisible = false;
		return true;
	}
	return false;
}