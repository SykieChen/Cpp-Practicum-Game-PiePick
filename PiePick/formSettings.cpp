#include "pie.h"

formSettings::formSettings(int x, int y, int w, int h, int difficulty) :
	lbTitle(w - 100, h / 7, x + 140, y, NULL),
	lbEasy(w - 100, h / 7, x + 170, y + (h / 7), NULL),
	lbNormal(w - 100, h / 7, x + 170, y + (h / 7) * 3, NULL),
	lbHard(w - 100, h / 7, x + 170, y + (h / 7) * 5, NULL) {

	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	getimage(&bgbk, x, y, w + 10, h + 10);
	show(difficulty);
}

void formSettings::show(int difficulty) {
	setfillcolor(BLACK);
	solidrectangle(x, y, x + w, y + h);
	lbTitle.setText(L"--Difficulty Setting--");
	if (difficulty == 1) lbEasy.setText(L"[ * ] Easy");
	else lbEasy.setText(L"[   ] Easy");
	if (difficulty == 2) lbNormal.setText(L"[ * ] Normal");
	else lbNormal.setText(L"[   ] Normal");
	if (difficulty == 3) lbHard.setText(L"[ * ] Hard");
	else lbHard.setText(L"[   ] Hard");
}

formSettings::~formSettings() {
	putimage(x, y, &bgbk);
}

int formSettings::waitOrder() {
	MOUSEMSG mouseMsg = GetMouseMsg();
	bool clicked = false;
	int difficulty;
	while (!clicked) {
		if (MouseHit()) {
			mouseMsg = GetMouseMsg();
			if (mouseMsg.uMsg == WM_LBUTTONDOWN) {
				//clicked
				if (lbEasy.chkRange(mouseMsg.x, mouseMsg.y)) {
					show(1);
					difficulty = 1;
					clicked = true;
				}
				else if (lbNormal.chkRange(mouseMsg.x, mouseMsg.y)) {
					show(2);
					difficulty = 2;
					clicked = true;
				}
				else if (lbHard.chkRange(mouseMsg.x, mouseMsg.y)) {
					show(3);
					difficulty = 3;
					clicked = true;
				}
			}
		}
		Sleep(1);
	}
	//perform a delay to see settings clearly before disappears
	Sleep(1500);
	return difficulty;
}