#include "pie.h"

void transimg(int dstX, int dstY, IMAGE *sImg, IMAGE *xImg) {
	// Trick to display transparent images
	IMAGE *workImg = GetWorkingImage();
	IMAGE *hImg = new IMAGE; // make a copy of the origional image
	Resize(hImg, sImg->getwidth(), sImg->getheight());
	SetWorkingImage(hImg);
	putimage(0, 0, sImg);
	putimage(0, 0, xImg, SRCINVERT);
	SetWorkingImage(workImg);
	putimage(dstX, dstY, xImg, SRCAND);
	putimage(dstX, dstY, hImg, SRCPAINT);
	delete hImg;
}

void repaintBlock(int x1, int y1, int x2, int y2, IMAGE* bg) {
	putimage(x1, y1, x2 - x1, y2 - y1, bg, x1, y1);
}

void pause(formMain* frm) {
	MOUSEMSG mouseMsg = GetMouseMsg();
	while (1) {
		if (MouseHit()) {
			mouseMsg = GetMouseMsg();
			if (mouseMsg.uMsg == WM_LBUTTONDOWN) {
				//clicked
				if (frm->btPause.chkRange(mouseMsg.x, mouseMsg.y))
					return;
			}
		}
		Sleep(1);
	}
}

void showmsg(LPCTSTR text) {
	IMAGE bgbk;
	getimage(&bgbk, 0, 0, 800, 600);
	setfillcolor(BLACK);
	solidrectangle(200, 150, 600, 450);
	RECT a = { 200,250,600,400 };
	drawtext(text, &a, DT_CENTER);
	a = { 200,400,600,450 };
	drawtext(L"This window will close in 3 seconds...", &a, DT_CENTER);
	Sleep(3000);
	putimage(0, 0, &bgbk);
}

char* wchar2char(LPCTSTR wchar) {
	char* CStr;
	size_t len = wcslen(wchar) + 10;
	size_t converted = 0;
	CStr = (char*)malloc(len * sizeof(char));
	wcstombs_s(&converted, CStr, len, wchar, _TRUNCATE);
	return CStr;
}