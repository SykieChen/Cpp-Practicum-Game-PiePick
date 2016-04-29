#include <graphics.h>
#include <iostream>
#include <conio.h>
#include <time.h>
#include <fstream>

using namespace std;

void transimg(int dstX, int dstY, IMAGE *sImg, IMAGE *xImg);
void repaintBlock(int x1, int y1, int x2, int y2, IMAGE*);
class button
{
private:
	int width;
	int height;
	int x;
	int y;
	IMAGE* bg;
	IMAGE* bgx;
public:
	button(int w, int h, int x, int y, IMAGE* bg, IMAGE* bgx);
	void show();
	bool chkRange(int mouseX, int mouseY);
};

class label
{
private:
	int x;
	int y;
	int width;
	int height;
	//int value;
	wchar_t text[30];
	IMAGE* main_bg;

public:
	label(int w, int h, int x, int y, IMAGE* main_bg);
	void show();
	void hide();
	void setText(LPCTSTR);

};

class formMain
{
private:
	int width;
	int height;
	IMAGE* bg;
	label lbUsrName, lbTime, lbScore, lbHighScore, lbLife;
	int time = 60, score = 0, life = 3;
	int highScore = 0;
public:
	button btPlay, btPause, btStop, btExit, btHs, btSave;
	formMain(LPCTSTR usrName, int width, int height, IMAGE* bg,
		IMAGE* bt_play, IMAGE* bt_pause, IMAGE* bt_stop, IMAGE* bt_exit, IMAGE* bt_hs, IMAGE* bt_save, IMAGE* btx);
	int getTime();
	void setTime(int);
	int getScore();
	void setScore(int);
	int getHighScore();
	void setHighScore(int);
	int getLife();
	void setLife(int);
	void refreshLabels();
	~formMain();
};

class ball {
private:
	int x, y;
	int w, h;
	IMAGE* iunknown, *iunknownx;
	IMAGE* ipie, *ipiex;
	IMAGE* ibomb, *ibombx;
	IMAGE* main_bg;
	bool isCovered = true;
	clock_t lastShown;
public:
	bool isPie;
	bool isVisible = true;
	ball(bool isPie, int x,
		IMAGE* iunknown, IMAGE* iunknownx,
		IMAGE* ipie, IMAGE* ipiex,
		IMAGE* ibomb, IMAGE* ibombx,
		IMAGE* main_bg);
	int getX();
	int getY();
	int getW();
	int getH();
	void move(int speed);
	void show();
	void hide();
};

class ballNode {
public:
	ball item;
	ballNode* next;
	ballNode(int x, bool isPie,
		IMAGE* iunknown, IMAGE* iunknownx,
		IMAGE* ipie, IMAGE* ipiex,
		IMAGE* ibomb, IMAGE* ibombx,
		IMAGE* main_bg);
};


class bowl {
private:
	int x, y;
	int w, h;
	bool isFacingL = false;
	IMAGE* ilmario, *ilmariox;
	IMAGE* irmario, *irmariox;
	IMAGE* main_bg;
public:
	bowl(int x, int y, IMAGE* ilmario, IMAGE* ilmariox, IMAGE* irmario, IMAGE* irmariox, IMAGE* main_bg);
	int getX();
	void moveL();
	void moveR();
	void show();
	int getW();
	int getH();
	void hide();
	void reset(int x);
	bool ifCaught(ballNode* ball0);
};

class ballList {
private:
	IMAGE* iunknown, *iunknownx;
	IMAGE* ipie, *ipiex;
	IMAGE* ibomb, *ibombx;
	IMAGE* main_bg;
public:
	ballNode* head;
	ballNode* tail;
	ballList(IMAGE* iunknown, IMAGE* iunknownx,
		IMAGE* ipie, IMAGE* ipiex,
		IMAGE* ibomb, IMAGE* ibombx,
		IMAGE* main_bg);
	void addNode(int x, bool isPie);
	void delNode();
	void show();
	void hide();
	void move(int speed);
	void clear();
	ballNode* whoIsCaught(bowl* bowl0);
	~ballList();
};


void pause(formMain* frm);
void showmsg(LPCTSTR text);