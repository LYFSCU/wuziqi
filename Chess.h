#pragma once
#include<graphics.h>//easyx图形库的头文件
#include<vector>
#include "Button.h"
#include <stack>



//落子位置
struct ChessPos {
	int row;
	int col;
	ChessPos(int row,int col):row(row),col(col){}
	ChessPos() 
	:row(0),col(0){};
};

//存储当前棋子的位置，是什么棋
struct ChessMove {
	ChessPos pos;
	int playerflag;
	ChessMove(ChessPos pos, int p)
		:pos(pos), playerflag(p) {}
};

//
typedef enum {
	CHESS_WHITE = -1, //白棋
	CHESS_BLACK = 1 //黑棋
};

class Chess
{
private:
	IMAGE chessBlackImg;//黑棋
	IMAGE chessBlackImg2;//刚下的黑棋
	IMAGE chessWhiteImg;//白棋
	IMAGE chessWhiteImg2;//刚下的白棋

	int gradeSize;//棋盘大小（13线，15，17，19）
	int margin_x;//棋盘左侧边界
	int margin_y;//棋盘顶部边界
	float chessSize;//棋子大小（方格大小）
	
	std::stack<ChessMove> undoStack;

public:
	//存储当前棋子分布数据 
    //chessMap[3][5]表示棋盘的第三行第五列的落子情况 0：空白 1：黑子 -1：白子
	std::vector<std::vector<int>> chessMap;
	int steps;//记录步数

	//表示现在该谁下棋（落子）
	bool playerFlag;//true：黑子走 false：白子走
	ChessPos lastPos;//记录上一个棋子的位置

private:
	void updateGameMap(ChessPos* pos);//更新棋盘

	std::vector<Button> buttons;//游戏中控件

	//悔棋功能
	bool undo();

	bool performUndo();

public:
	Chess(int gradeSize, int marginX, int marginY, float chessSize)
		:gradeSize(gradeSize), margin_x(marginX), margin_y(marginY), chessSize(chessSize),steps(0)
	{
		playerFlag = CHESS_BLACK;
		for (int i = 0; i < gradeSize; i++) {
			std::vector<int> row;
			for (int j = 0; j < gradeSize; j++) {
				row.push_back(0);
			}
			chessMap.push_back(row);
		};
	}
	~Chess() {};



	//棋盘初始化：加载棋盘图片资源，初始化棋盘的相关数据
	void init();
	
	//判断在指定坐标（x，y）位置，是否有效点击
	//有效，则将位置保存在pos中
	bool clickBoard(float x, float y, ChessPos* pos);
	
	//在棋盘pos位置落子
	bool chessDown(ChessPos *pos, int kind);

	//获取棋盘大小（13线，15线，19线）
	int getGradeSize();

	//获取指定位置是黑棋，白棋，还是空白
	int getChessData(ChessPos* pos);
	int getChessData(int row,int col);

	bool loadImg();
};

