#include <iostream>
#include <conio.h>
#include <graphics.h>

using namespace std;

void transimg(int dstX, int dstY, IMAGE *sImg, IMAGE *xImg, IMAGE *bgImg = NULL) {
	// Trick to display transparent images
	IMAGE *hImg = new IMAGE; // make a copy of the origional image
	*hImg = *sImg;
	SetWorkingImage(hImg);
	putimage(0, 0, xImg, SRCINVERT);
	SetWorkingImage(bgImg);
	putimage(dstX, dstY, xImg, SRCAND);
	putimage(dstX, dstY, hImg, SRCPAINT);
	delete hImg;
}


int main() {
	initgraph(800, 600);
	// Load background
	IMAGE background;
	loadimage(&background, _T("img\\background.jpg"));
	putimage(0, 0, &background);
	// Load mario
	IMAGE mariox, mario;
	loadimage(&mariox, _T("img\\mariox.bmp"));
	loadimage(&mario, _T("img\\mario.bmp"));

	transimg(400, 465, &mario, &mariox);

	char key;
	bool flgEsc=false;
	while (!flgEsc) {
		key = _getch();
		if 
	}


	_getch();
	closegraph();
	return 0;
}