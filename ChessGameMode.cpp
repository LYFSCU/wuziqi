#include "ChessGameMode.h"
#include<graphics.h>

bool ChessGameMode::checkWin()
{
	// 横竖斜四种大情况，每种情况都根据当前落子往后遍历5个棋子，有一种符合就算赢
// 水平方向
	int row = chess->lastPos.row;
	int col = chess->lastPos.col;

	for (int i = 0; i < 5; i++)
	{
		// 往左5个，往右匹配4个子，20种情况
		if (col - i >= 0 &&
			col - i + 4 < gradeSize &&
			chess->chessMap[row][col - i] == chess->chessMap[row][col - i + 1] &&
			chess->chessMap[row][col - i] == chess->chessMap[row][col - i + 2] &&
			chess->chessMap[row][col - i] == chess->chessMap[row][col - i + 3] &&
			chess->chessMap[row][col - i] == chess->chessMap[row][col - i + 4])
			return true;
	}

	// 竖直方向(上下延伸4个)
	for (int i = 0; i < 5; i++)
	{
		if (row - i >= 0 &&
			row - i + 4 < gradeSize &&
			chess->chessMap[row - i][col] == chess->chessMap[row - i + 1][col] &&
			chess->chessMap[row - i][col] == chess->chessMap[row - i + 2][col] &&
			chess->chessMap[row - i][col] == chess->chessMap[row - i + 3][col] &&
			chess->chessMap[row - i][col] == chess->chessMap[row - i + 4][col])
			return true;
	}

	// “/"方向
	for (int i = 0; i < 5; i++)
	{
		if (row + i < gradeSize &&
			row + i - 4 >= 0 &&
			col - i >= 0 &&
			col - i + 4 < gradeSize &&
			// 第[row+i]行，第[col-i]的棋子，与右上方连续4个棋子都相同
			chess->chessMap[row + i][col - i] == chess->chessMap[row + i - 1][col - i + 1] &&
			chess->chessMap[row + i][col - i] == chess->chessMap[row + i - 2][col - i + 2] &&
			chess->chessMap[row + i][col - i] == chess->chessMap[row + i - 3][col - i + 3] &&
			chess->chessMap[row + i][col - i] == chess->chessMap[row + i - 4][col - i + 4])
			return true;
	}

	// “\“ 方向
	for (int i = 0; i < 5; i++)
	{
		// 第[row+i]行，第[col-i]的棋子，与右下方连续4个棋子都相同
		if (row - i >= 0 &&
			row - i + 4 < gradeSize &&
			col - i >= 0 &&
			col - i + 4 < gradeSize &&
			chess->chessMap[row - i][col - i] == chess->chessMap[row - i + 1][col - i + 1] &&
			chess->chessMap[row - i][col - i] == chess->chessMap[row - i + 2][col - i + 2] &&
			chess->chessMap[row - i][col - i] == chess->chessMap[row - i + 3][col - i + 3] &&
			chess->chessMap[row - i][col - i] == chess->chessMap[row - i + 4][col - i + 4])
			return true;
	}

	return false;
}//

bool ChessGameMode::checkOver()
{
	//判断和局
	int chessCount = gradeSize * gradeSize;
	for (int i = 0; i < gradeSize; i++) {
		for (int j = 0; j < gradeSize; j++) {
			if (chess->chessMap[i][j]) {
				count++;
			}
		}
	}
	if (count == chessCount) {
		//和局
		chess->steps = 0;
		std::cout << "和局" << std::endl;
		endGameMenu();
	}
	if (checkWin()) {
		chess->steps = 0;
		Sleep(1500);
		endGameMenu();
	}
	return false;
}


void ChessGameMode::play()
{
	chess->init();

	while (1) {
		//先棋手走棋
		man->go();
		if (checkOver()) {
			chess->init();
			continue;
		}

		//由ai走
		ai->go();
		if (checkOver()) {
			chess->init();
			continue;
		}
	}
}

void ChessGameMode::start()
{
	//Chess chess;
	Chess chess(19, 53, 51 , 43);
	Man man(&chess);
	AI ai(&chess);
	ChessGameMode game(&man, &ai, &chess);

	game.play();
}

void ChessGameMode::updateDifficultyText()
{
	const std::string levels[3] = { "简单" ,"普通","困难" };
	buttons[1].text = ("难度：" + std::string(levels[currentDifficulty])).c_str();
}

void ChessGameMode::StartMenu()
{
	
	initgraph(600, 800, EX_SHOWCONSOLE);
	loadimage(0, "res/R-C.jpg");
	buttons.clear();
	//开始游戏按钮
	buttons.emplace_back(300, 400, 200, 50, "开始游戏", [this]() {
		closegraph();
		start();
	});

	//难度选择
	buttons.emplace_back(300, 500, 200, 50, "难度：简单", [this]() {
		currentDifficulty = (currentDifficulty + 1) % 3;
		updateDifficultyText();
	});

	//退出按钮
	buttons.emplace_back(300, 600, 200, 50, "退出游戏", []() {
		closegraph();
		exit(0);
		});
}

void ChessGameMode::endGameMenu()
{
	closegraph();
	initgraph(897, 622, EX_SHOWCONSOLE);
	if (chess->playerFlag) {//已经结束，该黑棋走，说明白棋赢
		mciSendString("play res/失败.mp3", 0, 0, 0);
		loadimage(0, "res/失败.jpg");
	}
	else {//黑棋赢
		mciSendString("play res/不错.mp3", 0, 0, 0);
		loadimage(0, "res/胜利.jpg");
	}
	buttons.clear();
	buttons.emplace_back(355, 300, 200, 50, "再次游玩", [this]() {
		start();
		});
	buttons.emplace_back(355, 400, 200, 50, "返回菜单", [this]() {
		StartMenu();
		});
	buttons.emplace_back(355, 500, 200, 50, "退出游戏", []() {
		closegraph();
		exit(0);
		});
	while (true) {
		render();
		handleInput();
	}
}

void ChessGameMode::handleInput()
{
	while (true) {
		if (MouseHit()) {//鼠标事件
			MOUSEMSG msg = GetMouseMsg();
			if (msg.uMsg == WM_LBUTTONDOWN) {
				for (auto& btn : buttons) {
					if (btn.isHover()) {
						btn.onClick();
						return;
					}
				}
			}
		}
		Sleep(10);//降低CPU占用
	}
}

void ChessGameMode::render()
{
	//绘制所有按钮
	for (const auto& btn : buttons) {
		btn.draw();
	}
	FlushBatchDraw();//双缓冲绘制
}