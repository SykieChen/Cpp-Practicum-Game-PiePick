#include <iostream>
#include <conio.h>
#include <graphics.h>
#include <time.h>


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
class textBox {
private:
	int value;
	wchar_t noti[30];
	IMAGE bgImgBak;
	int x1, x2, y1, y2;
public:
	void setNoti(LPCTSTR txt) {
		wcscpy_s(noti, txt);
	}
	void setValue(int val) {
		value = val;
	}
	/*LPCTSTR getText() {
		return text;
	}*/
	void setPos(int x1, int y1, int x2, int y2) {
		this->x1 = x1;
		this->x2 = x2;
		this->y1 = y1;
		this->y2 = y2;
	}
	void bgBak() {
		//back up the background
		getimage(&bgImgBak, x1, y1, x2-x1, y2-y1);
	}
	void show() {
		bgBak();
		setbkmode(TRANSPARENT);
		RECT r = {x1,y1,x2,y2 };
		wchar_t sTxt[30];
		swprintf_s(sTxt, L"%s: %d", noti,value);
		drawtext(sTxt, &r, DT_LEFT | DT_TOP);
	}
	void refresh() {
		putimage(x1, y1, &bgImgBak);
		show();
	}
};

class mainForm {
private:
	wchar_t username[20];
	IMAGE background;
	int highScore = 0;
	int score = 0;
	int time = 200;
	int life = 3;
public:
	textBox highScoreBox;
	textBox scoreBox;
	textBox timeBox;
	textBox lifeBox;
	void setHighScore(int hs) {
		highScore = hs;
		highScoreBox.setValue(hs);
	}
	void setScore(int s) {
		score = s;
		scoreBox.setValue(s);
	}
	void setLife(int l) {
		life = l;
		lifeBox.setValue(l);
	}
	void setTime(int time) {
		this->time = time;
		timeBox.setValue(time);
	}
	int getTime() {
		return time;
	}

	//text boxes to be rewritten as a class
	void loadBackground(LPCTSTR bgPath) {
		loadimage(&background, bgPath);
	}
	void refreshBackground() {
		putimage(0, 0, &background);
	}
	void setUsername(LPCTSTR name) {
		wcscpy_s(username, name);
	}
	void showUsername() {
		setbkmode(TRANSPARENT);
		RECT rName = { 5,10,800,40 };
		wchar_t sName[30];
		swprintf_s(sName, L"Player: %s", username);
		drawtext(sName, &rName, DT_LEFT | DT_TOP);
	}

	void init() {
		initgraph(800, 600);
		//paint bg
		refreshBackground();
		showUsername();
		highScoreBox.setNoti(L"High Score");
		highScoreBox.setValue(highScore);
		highScoreBox.setPos(500, 10, 650, 40);
		highScoreBox.show();
		scoreBox.setNoti(L"Score");
		scoreBox.setValue(score);
		scoreBox.setPos(650, 10, 800, 40);
		scoreBox.show();
		timeBox.setNoti(L"Time");
		timeBox.setValue(time);
		timeBox.setPos(350, 10, 500, 40);
		timeBox.show();
		lifeBox.setNoti(L"Life");
		lifeBox.setValue(life);
		lifeBox.setPos(200, 10, 350, 40);
		lifeBox.show();
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
	IMAGE backupBg; //bg's backup
	int x;
	int y;
	int height;
	int width;
	int bkX;
	int bkY;
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
		//back up the background
		getimage(&backupBg, x, y, width, height);
		transimg(x, y, &background, &backgroundX);
		bkX = x;
		bkY = y;
	}
	void moveL(int step) {
		if (x - step >= 0) {
			x -= step;
			refresh();
		}
	}
	void moveR(int step) {
		if (x + step <= 800 - width) {
			x += step;
			refresh();
		}
	}
	void refresh() {
		BeginBatchDraw();
		putimage(bkX, bkY, &backupBg);
		show();
		EndBatchDraw();
	}
	int getX() {
		return x;
	}
};

class ball {
private:
	bool flgIsPie = true;
	bool flgIsVisible = false;
	IMAGE pie;
	IMAGE pieX;
	IMAGE bomb;
	IMAGE bombX;
	IMAGE unknown;
	IMAGE unknownX;
	IMAGE backupBg; //bg's backup
	int x, y, width, height;
	int bkX, bkY;
public:
	ball() {
		loadimage(&pie, L"img\\pie.bmp");
		loadimage(&pieX, L"img\\piex.bmp");
		loadimage(&bomb, L"img\\bomb.bmp");
		loadimage(&bombX, L"img\\bombx.bmp");
		loadimage(&unknown, L"img\\unknown.bmp");
		loadimage(&unknownX, L"img\\unknownx.bmp");
		height = unknown.getheight();
		width = unknown.getwidth();
		y = 40;
	}
	void setX(int x) {
		this->x = x;
	}
	void setAsPie(bool ifPie) {
		flgIsPie = ifPie;
	}
	void show() {
		getimage(&backupBg, x, y, width, height);
		if (flgIsVisible) {
			if (flgIsPie)
				transimg(x, y, &pie, &pieX);
			else
				transimg(x, y, &bomb, &bombX);
		}
		else {
			transimg(x, y, &unknown, &unknownX);
		}
		bkX = x;
		bkY = y;
	}
	void refresh() {
		BeginBatchDraw();
		putimage(bkX, bkY, &backupBg);
		show();
		EndBatchDraw();
	}
	void uncover() {
		flgIsVisible = true;
	}
	void cover() {
		flgIsVisible = false;
	}
	int getX() {
		return x;
	}
	void moveDown(int step) {
		y += step;
		refresh();
	}
	bool isPie() {
		return flgIsPie;
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
	}

	// End of demo anim

	*/

	//Get username
	wchar_t usrName[20] = L"";
	while (!InputBox(usrName, 20, L"Please input your name:", L"Input name") || !wcscmp(usrName, L""));
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
	//load bow
	bow mario;
	mario.loadBg(L"img\\mario.bmp", L"img\\mariox.bmp");
	mario.setPos(400, 465);
	mario.show();



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
	bool flgPlay = false;
	bool flgKeyL = false;
	bool flgKeyR = false;
	clock_t timeKey = clock();
	clock_t timeGame = 0;
	MOUSEMSG mouseMsg = GetMouseMsg();
	while (!flgEsc) {
		// check mouse
		if (MouseHit()) {
			mouseMsg = GetMouseMsg();
			if (mouseMsg.uMsg == WM_LBUTTONDOWN) {
				if (btExit.chkRange(mouseMsg.x, mouseMsg.y))
					flgEsc = true;
				else if (btPlay.chkRange(mouseMsg.x, mouseMsg.y)) {
					flgPlay = true;
					timeGame = 0;
					mForm.setTime(200);
					mForm.timeBox.refresh();
					mForm.setLife(3);
					mForm.lifeBox.refresh();
					mForm.setScore(0);
					mForm.scoreBox.refresh();
				}
				else if (btStop.chkRange(mouseMsg.x, mouseMsg.y)) {
					flgPlay = false;
					mario.setPos(400, 465);
					mario.refresh();
				}
			}
		}
		if (flgPlay) { //if in game
			//check keyboard
			if (_kbhit()) {
				//slow down the response
				if (((clock() - timeKey)) > 1) {
					timeKey = clock();
					if (GetAsyncKeyState(VK_LEFT) & 0x8000)
						mario.moveL(2);
					else if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
						mario.moveR(2);
				}
			}
			//refresh time
			if (timeGame==0) timeGame = clock(); //first second
			if (clock() - timeGame >= 1000) {
				timeGame = clock();
				mForm.setTime(mForm.getTime() - 1);
				mForm.timeBox.refresh();
				if (mForm.getTime() == 0) {
					//reset game
					flgPlay = false;
					mario.setPos(400, 465);
					mario.refresh();
				}
			}
		}

	}


	//_getch();
	closegraph();
	return 0;
}