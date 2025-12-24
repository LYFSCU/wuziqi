#include "Chess.h"
#include<math.h>


void const putimagePNG(int x, int y, IMAGE* picture) //x为载入图片的X坐标，y为Y坐标
{
	// 变量初始化
	DWORD* dst = GetImageBuffer();    // GetImageBuffer()函数，用于获取绘图设备的显存指针，EASYX自带
	DWORD* draw = GetImageBuffer();
	DWORD* src = GetImageBuffer(picture); //获取picture的显存指针
	int picture_width = picture->getwidth(); //获取picture的宽度，EASYX自带
	int picture_height = picture->getheight(); //获取picture的高度，EASYX自带
	int graphWidth = getwidth();       //获取绘图区的宽度，EASYX自带
	int graphHeight = getheight();     //获取绘图区的高度，EASYX自带
	int dstX = 0;    //在显存里像素的角标

	// 实现透明贴图 公式： Cp=αp*FP+(1-αp)*BP ， 贝叶斯定理来进行点颜色的概率计算
	for (int iy = 0; iy < picture_height; iy++)
	{
		for (int ix = 0; ix < picture_width; ix++)
		{
			int srcX = ix + iy * picture_width; //在显存里像素的角标
			int sa = ((src[srcX] & 0xff000000) >> 24); //0xAArrggbb;AA是透明度
			int sr = ((src[srcX] & 0xff0000) >> 16); //获取RGB里的R
			int sg = ((src[srcX] & 0xff00) >> 8);   //G
			int sb = src[srcX] & 0xff;              //B
			if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
			{
				dstX = (ix + x) + (iy + y) * graphWidth; //在显存里像素的角标
				int dr = ((dst[dstX] & 0xff0000) >> 16);
				int dg = ((dst[dstX] & 0xff00) >> 8);
				int db = dst[dstX] & 0xff;
				draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  //公式： Cp=αp*FP+(1-αp)*BP  ； αp=sa/255 , FP=sr , BP=dr
					| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)         //αp=sa/255 , FP=sg , BP=dg
					| (sb * sa / 255 + db * (255 - sa) / 255);              //αp=sa/255 , FP=sb , BP=db
			}
		}
	}
}

void Chess::updateGameMap(ChessPos* pos)
{
	lastPos = *pos;
	chessMap[pos->row][pos->col] = playerFlag ? CHESS_BLACK : CHESS_WHITE;
	playerFlag = !playerFlag;//双方交换
}


struct vector2 {
	float x, y;
};

bool Chess::undo()
{
	if (undoStack.empty()) return false;
	ChessMove lastMove = undoStack.top();
	undoStack.pop();
	chessMap[lastMove.pos.row][lastMove.pos.col] = 0;
	return true;
}

bool Chess::performUndo()
{
	bool success = false;
	if (!playerFlag) return success;
	if (undo()) {//撤回白棋
		if (undo()) {//撤回黑棋
			success = true;
		}
	}
	return success;
}


void Chess::init()
{
	//创建我们的游戏窗口
	initgraph(895, 900, EX_SHOWCONSOLE);//895+205
	//显示我们的棋盘图片
	//setbkcolor();
	loadimage(0, "res/棋盘3.png");
	buttons.clear();
	//悔棋按钮
	buttons.emplace_back(960, 400, 70, 50, "悔    棋", [this]() {
		performUndo();
	});
	buttons.emplace_back(950, 800, 90, 50, "退出游戏", [this]() {
		exit(0);
	});
	
	//播放开始提示音
	mciSendString("play res/start.wav", 0, 0, 0);

	loadImg();//加载所有的图片


	//棋盘清零
	for (int i = 0; i < gradeSize; i++) {
		for (int j = 0; j < gradeSize; j++) {
			chessMap[i][j] = 0;
		}
		playerFlag = true;
	}
}

//函数Verify,两顶点距离小于偏移量offset返回true
bool const Verify(const vector2& vertice1,const vector2& vertice2,int offset)  {
	double len = sqrt((vertice1.x - vertice2.x) * (vertice1.x - vertice2.x) + (vertice1.y - vertice2.y) * (vertice1.y - vertice2.y));
	if (len < offset) {
		return true;
	}
	return false;
}

bool Chess::clickBoard(float x, float y, ChessPos* pos)
{
	vector2 clickpos{ x,y };
	int row = (y - margin_y) / chessSize;
	int col = (x - margin_x) / chessSize;

	int leftTopPosX = margin_x + chessSize * col;
	int leftTopPosY = margin_y + chessSize * row;
	int rightTopPosX = leftTopPosX + chessSize;
	int rightTopPosY = leftTopPosY ;
	int leftBottomPosX = leftTopPosX;
	int leftBottomPosY = leftTopPosY - chessSize;
	int rightBottomPosX = rightTopPosX;
	int rightBottomPosY = rightTopPosY - chessSize;;

	int offset = chessSize * 0.3;

	bool ret = false;

	do {
		//左上角判断
		vector2 vertice0{leftTopPosX,leftTopPosY};
		if (Verify(clickpos,vertice0,offset)) {
			pos->row = row;
			pos->col = col;
			if (chessMap[row][col] == 0) {
				ret = true;
			}
			break;
		}
		//右上角判断
		vertice0.x =rightTopPosX;
		vertice0.y =rightTopPosY;
		if (Verify(clickpos, vertice0, offset)) {
			pos->row = row;
			pos->col = col + 1;
			if (chessMap[row][col + 1] == 0) {
				ret = true;
			}
			break;
		}

		//左下角判断
		vertice0.x = leftBottomPosX;
		vertice0.y = leftBottomPosY;
		if (Verify(clickpos, vertice0, offset)) {
			pos->row = row + 1;
			pos->col = col;
			if (chessMap[row + 1][col] == 0) {
				ret = true;
			}
			break;
		}

		//右下角判断
		vertice0.x = rightBottomPosX;
		vertice0.y = rightBottomPosY;
		if (Verify(clickpos, vertice0, offset)) {
			pos->row = row + 1;
			pos->col = col + 1;
			if (chessMap[row + 1][col + 1] == 0) {
				ret = true;
			}
			break;
		}
	} while (0);
	return ret;
}

bool Chess::chessDown(ChessPos* pos, int kind)
{
	
	int x2 = margin_x + chessSize * lastPos.col - 0.5 * chessSize;//
	int y2 = margin_y + chessSize * lastPos.row - 0.5 * chessSize;
	int x = margin_x + chessSize * pos->col - 0.5 * chessSize;//棋子（锚点？）在左上角
	int y = margin_y + chessSize * pos->row - 0.5 * chessSize;
	

	if (kind == CHESS_WHITE) {
		if (steps) putimagePNG(x2, y2, &chessBlackImg);
		putimagePNG(x, y, &chessWhiteImg2);
	}
	else {
		if (steps) putimagePNG(x2, y2, &chessWhiteImg);
		steps++;
		putimagePNG(x, y, &chessBlackImg2);
	}
	undoStack.push(ChessMove(*pos, kind));
	mciSendString("play res/down7.wav", 0, 0, 0);
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

bool Chess::loadImg()
{
	//加载黑棋白棋棋子的图片
	loadimage(&chessBlackImg, "res/black.png", chessSize, chessSize, true);
	loadimage(&chessBlackImg2, "res/black2.png", chessSize, chessSize, true);
	loadimage(&chessWhiteImg, "res/white.png", chessSize, chessSize, true);
	loadimage(&chessWhiteImg2, "res/white2.png", chessSize, chessSize, true);

	return true;
}


