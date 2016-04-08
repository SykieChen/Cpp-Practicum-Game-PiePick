#include <iostream>
#include <conio.h>
#include <graphics.h>
#include <string.h>


using namespace std;

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

class mainForm {
private:
	wchar_t username[20];
	IMAGE background;
	IMAGE scoreBackground; //score display's backup
	int highScore = 0;
	int score = 0;
	int time = 200;
public:
	void loadBackground(LPCTSTR bgPath) {
		loadimage(&background, bgPath);
	}
	void refreshBackground() {
		putimage(0, 0, &background);
	}
	void setUsername(LPCTSTR name) {
		wcscpy_s(username,name);
	}
	void showUsername() {
		setbkmode(TRANSPARENT);
		RECT rName = { 5,10,800,40 };
		wchar_t sName[30];
		swprintf_s(sName, L"Player: %s", username);
		drawtext(sName, &rName, DT_LEFT | DT_TOP);
	}
	void showHighScore() {
		setbkmode(TRANSPARENT);
		RECT rHS = { 400,10,800,40 };
		wchar_t sHS[30];
		swprintf_s(sHS,L"High score: %d",highScore);
		drawtext(sHS, &rHS, DT_LEFT | DT_TOP);
	}
	void backupScore() {
		//back up the background
		getimage(&scoreBackground, 600, 10, 800 - 600, 40 - 10);
	}
	void showScore() {
		setbkmode(TRANSPARENT);
		RECT rScore = { 600,10,800,40 };
		wchar_t sScore[30];
		swprintf_s(sScore, L"Score: %d", score);
		drawtext(sScore, &rScore, DT_LEFT | DT_TOP);
	}
	void refreshScore() {
		//repaint background
		putimage(600, 10, &scoreBackground);
		showScore();
	}
	void init() {
		initgraph(800, 600);
		//paint bg
		refreshBackground();
		showUsername();
		showHighScore();
		backupScore();
		showScore();

	}
};

class button {
private:
	int x;
	int y;
	int height;
	int width;
	IMAGE background;
	IMAGE backgroundX;
public:
	void loadBg(LPCTSTR bgPath, LPCTSTR bgXPath) {
		loadimage(&background, bgPath);
		height = background.getheight();
		width = background.getwidth();
		loadimage(&backgroundX, bgXPath);
	}
	int getHeight() {
		return height;
	}
	int getWidth() {
		return width;
	}
	void setPos(int x, int y) {
		this->x = x;
		this->y = y;
	}
	void show() {
		transimg(x, y, &background, &backgroundX);
	}
	bool chkRange(int x, int y) {
		if (x >= this->x && x <= this->x + width && y >= this->y && y <= this->y + height)
			return true;
		return false;
	}
};

class bow {
	//to be rewritted as a son of pic class
private:
	IMAGE background;
	IMAGE backgroundX;
	int x;
	int y;
	int height;
	int width;
public:
	void loadBg(LPCTSTR bgPath, LPCTSTR bgXPath) {
		loadimage(&background, bgPath);
		height = background.getheight();
		width = background.getwidth();
		loadimage(&backgroundX, bgXPath);
	}
	int getHeight() {
		return height;
	}
	int getWidth() {
		return width;
	}
};

int main() {
	/*initgraph(800, 600);
	// Load background
	IMAGE background;
	loadimage(&background, L"img\\background.jpg");
	putimage(0, 0, &background);
	// Load mario
	IMAGE mariox, mario;
	loadimage(&mariox, L"img\\mariox.bmp");
	loadimage(&mario, L"img\\mario.bmp");

	int marioPosX = 0;
	int marioPosY = 465;
	transimg(marioPosX, marioPosY, &mario, &mariox);
	// Demo anim
	
	for (; marioPosX < 795; marioPosX += 5) {
		putimage(0, 0, &background);
		transimg(marioPosX, marioPosY, &mario, &mariox);
		Sleep(15);
	}
	for (; marioPosX > 400; marioPosX -= 5) {
		putimage(0, 0, &background);
		transimg(marioPosX, marioPosY, &mario, &mariox);
		Sleep(10);
	}*/

	// End of demo anim

	
	//Get username
	wchar_t usrName[20]=L"";
	while (!InputBox(usrName, 20, L"Please input your name:", L"Input name")||!wcscmp(usrName,L""));
	mainForm mForm;
	mForm.loadBackground(L"img\\background.jpg");
	mForm.setUsername(usrName);
	mForm.init();
	//load buttons
	button btPlay;
	button btStop;
	button btExit;
	btPlay.loadBg(L"img\\bt_play.bmp", L"img\\btx.bmp");
	btStop.loadBg(L"img\\bt_stop.bmp", L"img\\btx.bmp");
	btExit.loadBg(L"img\\bt_exit.bmp", L"img\\btx.bmp");
	btPlay.setPos(35, 540);
	btStop.setPos(350, 540);
	btExit.setPos(650, 540);
	btPlay.show();
	btStop.show();
	btExit.show();




	/*char key;
	bool flgEsc=false;
	while (!flgEsc) {
		key = _getch();
		if (key == 'q') flgEsc = true;
		else if (key == 'a') {
			// move left
			if (marioPosX > 5) marioPosX -= 5;
			//repaint bg
			putimage(0, 0, &background);
			transimg(marioPosX, marioPosY, &mario, &mariox);
		}
		else if (key == 'd') {
			// move left
			if (marioPosX <795) marioPosX += 5;
			//repaint bg
			putimage(0, 0, &background);
			transimg(marioPosX, marioPosY, &mario, &mariox);
		}
	}

	*/

	//main loop
	bool flgEsc = false;
	MOUSEMSG mouseMsg = GetMouseMsg();
	while (!flgEsc) {
		// check mouse
		if (MouseHit()) {
			mouseMsg = GetMouseMsg();
			if (mouseMsg.uMsg == WM_LBUTTONDOWN) {
				if (btExit.chkRange(mouseMsg.x, mouseMsg.y)) {
					flgEsc = true;
				}
			}
		}

	}


	//_getch();
	closegraph();
	return 0;
}