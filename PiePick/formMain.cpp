#include "pie.h"

formMain::formMain(LPCTSTR usrName, int width, int height, IMAGE* bg,
	IMAGE* bt_play, IMAGE* bt_pause, IMAGE* bt_stop, IMAGE* bt_exit,
	IMAGE* bt_hs, IMAGE* bt_save, IMAGE* bt_set, IMAGE* bt_lg, IMAGE* bt_sg, IMAGE* btx) :
	lbUsrName(170, 20, 15, 5, bg),
	lbTime(120, 20, 188, 5, bg),
	lbScore(120, 20, 341, 5, bg),
	lbLife(120, 20, 494, 5, bg),
	lbHighScore(120, 20, 647, 5, bg),
	btPlay(btx->getwidth(), btx->getheight(), 49, 534, bt_play, btx),
	btPause(btx->getwidth(), btx->getheight(), 49, 534, bt_pause, btx),
	btStop(btx->getwidth(), btx->getheight(), 132, 567, bt_stop, btx),
	btExit(btx->getwidth(), btx->getheight(), 216, 534, bt_exit, btx),
	btHs(btx->getwidth(), btx->getheight(), 383, 534, bt_hs, btx),
	btSave(btx->getwidth(), btx->getheight(), 466, 567, bt_save, btx),
	btSet(btx->getwidth(), btx->getheight(), 299, 567, bt_set, btx),
	btSg(btx->getwidth(), btx->getheight(), 633, 567, bt_sg, btx),
	btLg(btx->getwidth(), btx->getheight(), 550, 534, bt_lg, btx)
{

	initgraph(800, 600);
	//paint bg
	putimage(0, 0, bg);
	//init boxes
	wchar_t txt[40];
	swprintf_s(txt, L"Player: %s", usrName);
	lbUsrName.setText(txt);
	swprintf_s(txt, L"Time: %d", time);
	lbTime.setText(txt);
	swprintf_s(txt, L"Score: %d", score);
	lbScore.setText(txt);
	swprintf_s(txt, L"Life: %d", life);
	lbLife.setText(txt);
	swprintf_s(txt, L"High Score: N/A");
	lbHighScore.setText(txt);

	//buttons
	btPlay.show();
	//btPause.show();
	btStop.show();
	btExit.show();
	btHs.show();
	btSave.show();
	btSet.show();
	btSg.show();
	btLg.show();
}

formMain::~formMain() {
	closegraph();
}

int formMain::getTime() {
	return time;
}
void formMain::setTime(int time) {
	this->time = time;
	wchar_t txt[40];
	swprintf_s(txt, L"Time: %d", time);
	this->lbTime.hide();
	this->lbTime.setText(txt);
}
int formMain::getScore() {
	return score;
}
void formMain::setScore(int score) {
	this->score = score;
	wchar_t txt[40];
	swprintf_s(txt, L"Score: %d", score);
	this->lbScore.hide();
	this->lbScore.setText(txt);
}
int formMain::getHighScore() {
	return highScore;
}
void formMain::setHighScore(int hs) {
	this->highScore = hs;
	wchar_t txt[40];
	swprintf_s(txt, L"High Score: %d", highScore);
	this->lbHighScore.hide();
	this->lbHighScore.setText(txt);
}

int formMain::getLife() {
	return life;
}
void formMain::setLife(int life) {
	this->life = life;
	wchar_t txt[40];
	swprintf_s(txt, L"Life: %d", life);
	this->lbLife.hide();
	this->lbLife.setText(txt);
}

void formMain::refreshLabels() {
	lbHighScore.show();
	lbLife.show();
	lbUsrName.show();
	lbTime.show();
	lbScore.show();
}