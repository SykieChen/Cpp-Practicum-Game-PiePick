#include "pie.h"

int main() {
	srand((unsigned)time(NULL));
	//load resources
	IMAGE bg, lmario, lmariox, rmario, rmariox;
	IMAGE unknown, unknownx, pie, piex, bomb, bombx;
	IMAGE bt_play, bt_pause, bt_save, bt_hs, bt_exit, bt_stop, bt_set, bt_lg, bt_sg, btx;
	loadimage(&bg, L"img\\bg.jpg");
	loadimage(&lmario, L"img\\lmario.bmp");
	loadimage(&lmariox, L"img\\lmariox.bmp");
	loadimage(&rmario, L"img\\rmario.bmp");
	loadimage(&rmariox, L"img\\rmariox.bmp");
	loadimage(&unknown, L"img\\unknown.bmp");
	loadimage(&unknownx, L"img\\unknownx.bmp");
	loadimage(&pie, L"img\\pie.bmp");
	loadimage(&piex, L"img\\piex.bmp");
	loadimage(&bomb, L"img\\bomb.bmp");
	loadimage(&bombx, L"img\\bombx.bmp");
	loadimage(&bt_play, L"img\\bt_play.bmp");
	loadimage(&bt_pause, L"img\\bt_pause.bmp");
	loadimage(&bt_save, L"img\\bt_save.bmp");
	loadimage(&bt_hs, L"img\\bt_hs.bmp");
	loadimage(&bt_exit, L"img\\bt_exit.bmp");
	loadimage(&bt_stop, L"img\\bt_stop.bmp");
	loadimage(&bt_set, L"img\\bt_set.bmp");
	loadimage(&bt_lg, L"img\\bt_lg.bmp");
	loadimage(&bt_sg, L"img\\bt_sg.bmp");
	loadimage(&btx, L"img\\btx.bmp");

	//Get username
	wchar_t usrName[20];
	while (!InputBox(usrName, 20, L"Pie Pick - Sykie Chen\nwww.devchen.com\n\nPlease input your name:", L"Pie Pick - Sykie Chen", L"1P", 0, 0, true) || !wcscmp(usrName, L""));

	//init
	formMain frMain(usrName, 800, 600,
		&bg, &bt_play, &bt_pause, &bt_stop, &bt_exit, &bt_hs, &bt_save, &bt_set, &bt_lg, &bt_sg, &btx);
	bowl mario(400, 466, &lmario, &lmariox, &rmario, &rmariox, &bg);
	ballList boxes(frMain.difficulty, &unknown, &unknownx, &pie, &piex, &bomb, &bombx, &bg);

	//flags
	int scoreOfThisGameBackUpForSaving = 0;
	bool isExiting = false;
	bool isPlaying = false;
	bool gameOver = false;
	clock_t kbDelay = clock();
	clock_t gameTime = 0;
	clock_t animDelay = clock();
	clock_t ballBreak = 0;
	MOUSEMSG mouseMsg = GetMouseMsg();
	fstream oFile;

	//main loop
	while (!isExiting) {
		//check mouse
		if (MouseHit()) {
			mouseMsg = GetMouseMsg();
			if (mouseMsg.uMsg == WM_LBUTTONDOWN) {
				//clicked
				if (frMain.btExit.chkRange(mouseMsg.x, mouseMsg.y)) {
					//clicked exit
					isExiting = true;
				}
				else if (frMain.btPlay.chkRange(mouseMsg.x, mouseMsg.y)) {
					if (!isPlaying) {
						//clicked play
						frMain.btPause.show();
						isPlaying = true;
						frMain.setLife(3);
						frMain.setScore(0);
						frMain.setTime(200);
					}
					else {
						//clicked pause
						frMain.btPlay.show();
						pause(&frMain);
						animDelay = clock();
						ballBreak = 1;
					}
				}
				else if (frMain.btStop.chkRange(mouseMsg.x, mouseMsg.y)) {
					//clicked stop
					if (isPlaying) gameOver = true;
				}
				else if (frMain.btHs.chkRange(mouseMsg.x, mouseMsg.y)) {
					//high score
					char* filename = wchar2char(usrName);
					//switch wchar to char

					//use current username as filename
					strcat_s(filename, wcslen(usrName) + 10, ".txt");

					//load score from file
					wchar_t txt[400];
					oFile.open(filename, ios::in);
					if (oFile.is_open()) {
						//open succeeded
						int savedScore = 0;
						bool isHSUpdated = false;
						while (!oFile.eof()) {
							oFile >> savedScore;
							if (savedScore > frMain.getHighScore()) {
								frMain.setHighScore(savedScore);
								isHSUpdated = true;
							}
						}
						oFile.close();
						if (isHSUpdated) {
							swprintf_s(txt, L"Successfully loaded score from file %s.txt\n\nLoaded highest score %d", usrName, frMain.getHighScore());
						}
						else {
							swprintf_s(txt, L"Successfully loaded file %s.txt\n\nBut no score is higher than current High Score.", usrName);
						}
					}
					else {
						//no saved score
						swprintf_s(txt, L"Failed to load %s.txt\n\nYou did not save any score.", usrName);
					}
					showmsg(txt);
					delete filename;
				}
				else if (frMain.btSave.chkRange(mouseMsg.x, mouseMsg.y)) {
					//save game score
					if (!isPlaying) {
						//write to file

						//switch wchar to char
						char* filename = wchar2char(usrName);
						//use current username as filename
						strcat_s(filename, wcslen(usrName) + 10, ".txt");
						oFile.open(filename, ios::app);
						oFile << scoreOfThisGameBackUpForSaving << '\n';
						oFile.close();
						wchar_t txt[400];
						swprintf_s(txt, L"Score saved to %s.txt", usrName);
						showmsg(txt);
						delete filename;
					}
					else {
						showmsg(L"You can not save score while the game is on going!\n\nTry saving when game ends.");
					}
				}
				else if (frMain.btSet.chkRange(mouseMsg.x, mouseMsg.y)) {
					//settings
					if (isPlaying) {
						showmsg(L"You can not change settings while the game is on going!\n\nTry saving when game ends.");
					}
					else {
						frMain.difficulty = settings(frMain.difficulty);
						boxes.setDifficulty(frMain.difficulty); //set boxes' time
					}
				}
				else if (frMain.btLg.chkRange(mouseMsg.x, mouseMsg.y)) {
					//load game
					loadGame(&frMain, &mario, &boxes, &bg);
					frMain.refreshLabels();
					wcscpy_s(usrName, frMain.usrName);
					isPlaying = true;
					frMain.btPause.show();
					animDelay = clock();
					ballBreak = 1;
				}
				else if (frMain.btSg.chkRange(mouseMsg.x, mouseMsg.y)) {
					//save game
					if (!isPlaying) {
						showmsg(L"Game can only be saved while a game is on going.\n\nStart a game first.");
					}
					else {
						saveGame(&frMain, &mario, &boxes);
					}
				}
			}
		}
		//game over?
		if (gameOver) {
			scoreOfThisGameBackUpForSaving = frMain.getScore();
			//update highscore
			if (frMain.getHighScore() < frMain.getScore()) {
				frMain.setHighScore(frMain.getScore());
			}
			//reset game
			boxes.clear();
			repaintBlock(0, 0, 800, 530, &bg);
			mario.reset(400);
			frMain.setLife(3);
			frMain.setTime(200);
			frMain.setScore(0);
			frMain.refreshLabels();
			frMain.btPlay.show();
			isPlaying = false;
			gameOver = false;
			ballBreak = 0;
		}
		else {
			if (isPlaying) {
				//in game
				//check keyboard
				if (_kbhit()) {
					//slow down the loop
					if (clock() - kbDelay > 6) {
						kbDelay = clock();
						if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
							//move left
							BeginBatchDraw();
							mario.hide();
							boxes.show();
							frMain.refreshLabels();
							mario.moveL();
							EndBatchDraw();
						}
						else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
							//move right
							BeginBatchDraw();
							mario.hide();
							boxes.show();
							frMain.refreshLabels();
							mario.moveR();
							EndBatchDraw();
						}

					}
				}

				//refresh time
				if (clock() - gameTime >= 1000) {
					gameTime = clock();
					//frMain.refreshLabels();
					frMain.setTime(frMain.getTime() - 1);
					if (frMain.getTime() == 0) {
						gameOver = true;
						showmsg(L"Game Over\n\nTime up.");
					}
				}

				//auto moves
				if (clock() - animDelay >= 30) {
					BeginBatchDraw();
					animDelay = clock();
					//release new boxes
					if ((clock() - ballBreak) >= (500 + (4 - frMain.difficulty) * 1000) || ballBreak == 0) {
						ballBreak = clock() - (rand() % 1000);
						//easy:2.5-3.5s  normal:1.5-2.5s  hard:0.5-1.5s
						boxes.addNode(rand() % (800 - boxes.head->item.getW()), rand() % 3);
						//random x pos, 66% possibility to be a pie
					}
					//hide boxes
					boxes.hide();


					//move boxes
					boxes.move(frMain.difficulty);//the speed of ball falls

					//hit mario's head
					ballNode* p = boxes.whoIsCaught(&mario);
					if (p != NULL) {
						//someone hit his head
						if (p->item.isPie) {
							//a pie hit
							frMain.setScore(frMain.getScore() + 10);
						}
						else {
							//a bomb hit
							frMain.setScore(frMain.getScore() - 100);
							frMain.setLife(frMain.getLife() - 1);
							if (frMain.getLife() == 0) {
								//dead
								EndBatchDraw();
								gameOver = true;
								showmsg(L"Game Over\n\nLife zero.");
							}
						}
					}

					//hit bottom
					if (boxes.head->next->item.getY() >= 474) {
						if (boxes.head->next->item.isVisible &&
							boxes.head->next->item.isPie) {
							//drpooed pie -10
							frMain.setScore(frMain.getScore() - 10);
						}
						boxes.delNode();
					}

					//repaint things
					frMain.refreshLabels();
					mario.show();



					EndBatchDraw();
				}

			}
		}


	}
	return 0;
}