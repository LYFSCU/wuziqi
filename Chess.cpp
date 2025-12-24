#include "Chess.h"
#include <math.h>

void Chess::updateGameMap(ChessPos* pos)
{
	lastPos = *pos;
	chessMap[pos->row][pos->col] = playerFlag ? CHESS_BLACK : CHESS_WHITE;
	playerFlag = !playerFlag;//双方交换
}

struct vector2 {
	float x, y;
};

// 辅助函数：绘制棋子
// isLastMove 为 true 时，在棋子中心画一个小标记
void Chess::drawPiece(int row, int col, int color, bool isLastMove)
{
	int x = margin_x + col * chessSize;
	int y = margin_y + row * chessSize;
	int r = chessSize / 2 * 0.9; // 棋子半径

	if (color == CHESS_BLACK) {
		setfillcolor(BLACK);
		setlinecolor(BLACK);
	}
	else {
		setfillcolor(WHITE);
		setlinecolor(LIGHTGRAY); // 白棋边框稍微深一点以便区分
	}

	fillcircle(x, y, r);

	// 如果是最后一步，画个标记(红色小方块或十字)
	if (isLastMove) {
		setlinecolor(RED);
		setfillcolor(RED);
		int markSize = 3;
		solidrectangle(x - markSize, y - markSize, x + markSize, y + markSize);
	}
}

bool Chess::undo()
{
	if (undoStack.empty()) return false;
	ChessMove lastMove = undoStack.top();
	undoStack.pop();
	chessMap[lastMove.pos.row][lastMove.pos.col] = 0;

	// 悔棋后需要重绘该位置为背景（擦除棋子）
	// 因为是直接在屏幕绘制，简单的做法是重新绘制棋盘背景的一小块和网格，或者简单地重绘整个棋盘
	// 为了简单起见，这里我们只重绘当前点。但考虑到网格线，最好的方式可能是重绘整个棋盘
	// 但这会导致闪烁。这里我们采用简易擦除法：画背景色矩形，补画十字线。

	int x = margin_x + lastMove.pos.col * chessSize;
	int y = margin_y + lastMove.pos.row * chessSize;

	// 1. 填补背景
	setfillcolor(RGB(220, 180, 100));
	solidrectangle(x - chessSize / 2, y - chessSize / 2, x + chessSize / 2, y + chessSize / 2);

	// 2. 补画十字线
	setlinecolor(BLACK);
	// 横线
	if (lastMove.pos.col > 0) line(x - chessSize / 2, y, x, y);
	if (lastMove.pos.col < gradeSize - 1) line(x, y, x + chessSize / 2, y);
	// 竖线
	if (lastMove.pos.row > 0) line(x, y - chessSize / 2, x, y);
	if (lastMove.pos.row < gradeSize - 1) line(x, y, x, y + chessSize / 2);

	// 3. 恢复上一步的标记（如果有的话）
	if (!undoStack.empty()) {
		ChessMove prev = undoStack.top();
		drawPiece(prev.pos.row, prev.pos.col, prev.playerflag == 1 ? CHESS_BLACK : CHESS_WHITE, true);
	}
	// 注意：这里steps需要递减，lastPos需要更新回前一步，这在performUndo逻辑中可能需要完善，
	// 原代码中performUndo只是调用undo两次，并没有显式处理steps和lastPos的回退。
	// 原逻辑可能有些依赖重绘，或者在悔棋后重新落子时覆盖。
	// 鉴于不大幅修改逻辑，我们只处理视觉擦除。

	return true;
}

bool Chess::performUndo()
{
	bool success = false;
	if (!playerFlag) return success; // 只有轮到黑棋（玩家）下的时候才能悔棋（悔两步）

	// 悔白棋
	if (undo()) {
		steps--;
		// 悔黑棋
		if (undo()) {
			steps--;
			success = true;
			// 恢复 lastPos
			if (!undoStack.empty()) {
				lastPos = undoStack.top().pos;
			}
			else {
				// 栈空了，说明回到了起点
				steps = 0;
			}
		}
	}
	return success;
}


void Chess::init()
{
	// 创建游戏窗口
	initgraph(895, 900, EX_SHOWCONSOLE);

	// 设置背景色（木纹色）
	setbkcolor(RGB(220, 180, 100));
	cleardevice();

	// 绘制棋盘网格
	setlinecolor(BLACK);
	setlinestyle(PS_SOLID, 1);

	for (int i = 0; i < gradeSize; i++) {
		int x = margin_x + i * chessSize;
		int y = margin_y + i * chessSize;
		// 竖线
		line(x, margin_y, x, margin_y + (gradeSize - 1) * chessSize);
		// 横线
		line(margin_x, y, margin_x + (gradeSize - 1) * chessSize, y);
	}

	// 绘制天元和星位 (以19路为例，星位在4, 10, 16)
	// 简单适配一下，只画中心天元
	int center = gradeSize / 2;
	setfillcolor(BLACK);
	fillcircle(margin_x + center * chessSize, margin_y + center * chessSize, 3);
	if (gradeSize == 19) {
		// 四角星位
		int stars[] = { 3, 9, 15 };
		for (int r : stars) {
			for (int c : stars) {
				fillcircle(margin_x + c * chessSize, margin_y + r * chessSize, 3);
			}
		}
	}

	buttons.clear();
	// 悔棋按钮
	buttons.emplace_back(960, 400, 70, 50, "悔    棋", [this]() {
		performUndo();
		});
	buttons.emplace_back(950, 800, 90, 50, "退出游戏", [this]() {
		exit(0);
		});

	// 播放开始提示音 (已注释)
	// mciSendString("play res/start.wav", 0, 0, 0);

	// 棋盘清零
	for (int i = 0; i < gradeSize; i++) {
		for (int j = 0; j < gradeSize; j++) {
			chessMap[i][j] = 0;
		}
		playerFlag = true;
	}
}

// 函数Verify,两顶点距离小于偏移量offset返回true
bool const Verify(const vector2& vertice1, const vector2& vertice2, int offset) {
	double len = sqrt((vertice1.x - vertice2.x) * (vertice1.x - vertice2.x) + (vertice1.y - vertice2.y) * (vertice1.y - vertice2.y));
	if (len < offset) {
		return true;
	}
	return false;
}

bool Chess::clickBoard(float x, float y, ChessPos* pos)
{
	// (保持原有逻辑不变)
	vector2 clickpos{ x,y };
	int row = (y - margin_y) / chessSize;
	int col = (x - margin_x) / chessSize;

	int leftTopPosX = margin_x + chessSize * col;
	int leftTopPosY = margin_y + chessSize * row;
	int rightTopPosX = leftTopPosX + chessSize;
	int rightTopPosY = leftTopPosY;
	int leftBottomPosX = leftTopPosX;
	int leftBottomPosY = leftTopPosY - chessSize;
	int rightBottomPosX = rightTopPosX;
	int rightBottomPosY = rightTopPosY - chessSize;;

	int offset = chessSize * 0.4; //稍微放宽一点点击范围

	bool ret = false;

	do {
		//左上角判断
		vector2 vertice0{ (float)leftTopPosX, (float)leftTopPosY };
		if (Verify(clickpos, vertice0, offset)) {
			pos->row = row;
			pos->col = col;
			if (chessMap[row][col] == 0) ret = true;
			break;
		}
		//右上角判断
		vertice0.x = rightTopPosX;
		vertice0.y = rightTopPosY;
		if (Verify(clickpos, vertice0, offset)) {
			pos->row = row;
			pos->col = col + 1;
			if (chessMap[row][col + 1] == 0) ret = true;
			break;
		}
		//左下角判断
		vertice0.x = leftBottomPosX;
		vertice0.y = leftBottomPosY;
		if (Verify(clickpos, vertice0, offset)) {
			pos->row = row + 1;
			pos->col = col;
			if (chessMap[row + 1][col] == 0) ret = true;
			break;
		}
		//右下角判断
		vertice0.x = rightBottomPosX;
		vertice0.y = rightBottomPosY;
		if (Verify(clickpos, vertice0, offset)) {
			pos->row = row + 1;
			pos->col = col + 1;
			if (chessMap[row + 1][col + 1] == 0) ret = true;
			break;
		}
	} while (0);
	return ret;
}

bool Chess::chessDown(ChessPos* pos, int kind)
{
	// 绘制上一步的棋子（去除标记）
	if (steps > 0) {
		// 上一步是对方下的，颜色与当前 kind 相反
		int lastColor = (kind == CHESS_WHITE) ? CHESS_BLACK : CHESS_WHITE;
		drawPiece(lastPos.row, lastPos.col, lastColor, false);
	}

	// 绘制当前步的棋子（带标记）
	drawPiece(pos->row, pos->col, kind, true);
	steps++;

	undoStack.push(ChessMove(*pos, kind));
	// mciSendString("play res/down7.wav", 0, 0, 0); // 注释音效
	updateGameMap(pos);
	return true;
}

int Chess::getGradeSize()
{
	return gradeSize;
}

int Chess::getChessData(ChessPos* pos)
{
	return chessMap[pos->row][pos->col];
}

int Chess::getChessData(int row, int col)
{
	return chessMap[row][col];
}