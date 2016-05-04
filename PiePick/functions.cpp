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
				if (frm->btPause.chkRange(mouseMsg.x, mouseMsg.y)) {
					frm->btPlay.show();
					return;
				}
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

wchar_t* char2wchar(LPSTR ichar) {
	size_t len = strlen(ichar) + 10;
	size_t converted = 0;
	wchar_t *WStr;
	WStr = (wchar_t*)malloc(len * sizeof(wchar_t));
	mbstowcs_s(&converted, WStr, len, ichar, _TRUNCATE);
	return WStr;
}

int settings(int difficulty) {
	formSettings frSett(200, 150, 400, 300, difficulty);
	return frSett.waitOrder();
}

void saveGame(formMain* fMain, bowl* mario, ballList* box) {
	fstream oFile;
	wchar_t fileName[50];
	//get file name
	while (!InputBox(fileName, 50, L"Pie Pick - Sykie Chen\nwww.devchen.com\n\nPlease input file name to save:", L"Pie Pick - Sykie Chen", L"savedgame", 0, 0, true) || !wcscmp(fileName, L""));
	char* fName = wchar2char(fileName);
	char* uName = wchar2char(fMain->usrName);
	strcat_s(fName, wcslen(fileName) + 10, ".sav");
	oFile.open(fName, ios::out);
	if (!oFile.is_open()) {
		showmsg(L"File open failed.\n\nCheck your file name and retry.");
		delete fName;
		delete uName;
		return;
	}
	oFile << uName << '\n'
		<< fMain->difficulty << "\n"
		<< fMain->getTime() << '\n'
		<< fMain->getScore() << '\n'
		<< fMain->getLife() << '\n'
		<< mario->getX() << '\n';
	ballNode* p = box->head->next;
	while (p != NULL) {
		oFile << "Node\n"
			<< p->item.getX() << '\n'
			<< p->item.getY() << '\n'
			<< p->item.isPie << '\n'
			<< p->item.isVisible << '\n';
		p = p->next;
	}
	wchar_t txt[50];
	swprintf_s(txt, L"Game successfully saved to %s.sav", fileName);
	showmsg(txt);
	oFile.close();
	delete fName;
	delete uName;
}

void loadGame(formMain* fMain, bowl* mario, ballList* box, IMAGE* bg) {
	fstream oFile;
	wchar_t fileName[50];
	//get file name
	bool ret = InputBox(fileName, 50, L"Pie Pick - Sykie Chen\nwww.devchen.com\n\nWARNING: Loading a game will overwrite your current game process!\nMake sure you saved your game!\nClick cancel to go back and save your game, or input file name to be loaded:",
		L"Pie Pick - Sykie Chen", L"savedgame", 0, 0, false);
	//if canceled, return
	if (!ret || wcscmp(fileName, L"") == 0) return;
	repaintBlock(0, 0, 800, 530, bg);
	char* fName = wchar2char(fileName);
	strcat_s(fName, wcslen(fileName) + 10, ".sav");
	char uName[30];
	int time, score, life, marioX, ballX, ballY, ballIsPie, ballIsVisible;
	int difficulty;
	oFile.open(fName, ios::in);
	//check if opened
	if (!oFile.is_open()) {
		showmsg(L"File open failed.\n\nCheck your file name and retry.");
		delete fName;
		return;
	}
	wchar_t* wUName;
	oFile >> uName >> difficulty >> time >> score >> life >> marioX;
	wUName = char2wchar(uName);
	fMain->setUsrName(wUName);
	fMain->difficulty = difficulty;
	box->setDifficulty(difficulty);
	fMain->setTime(time);
	fMain->setLife(life);
	fMain->setScore(score);
	mario->reset(marioX);
	box->clear();
	char nodeMark[30];
	oFile >> nodeMark;
	wchar_t* wNodeMark = char2wchar(nodeMark);
	while (wcscmp(wNodeMark, L"Node") == 0) {
		//input balls
		oFile >> ballX >> ballY >> ballIsPie >> ballIsVisible;
		box->addNode(ballX, ballIsPie);
		box->tail->item.isVisible = ballIsVisible;
		box->tail->item.setY(ballY);

		//read next
		oFile >> nodeMark;
		delete wNodeMark;
		wNodeMark = char2wchar(nodeMark);
	}
	delete fName;
	delete wUName;
	delete wNodeMark;
	showmsg(L"Game loaded successfully!\n\nGame will suto start when this disappear.");
}