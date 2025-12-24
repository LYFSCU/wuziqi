#pragma once
#include "Chess.h"

class AI
{
private:
	Chess* chess;
	std::vector<std::vector<int>> scoreMap;//vector二维数组
	ChessPos think();
	int size;

private:
	void calculateScore();//计算每个位置积分

public:
	//AI根据棋盘初始化
	AI(Chess* chess)
		:chess(chess) ,size(chess->getGradeSize())
	{
		std::vector<int> row;
		row.reserve(size);
		scoreMap.reserve(size);
		for (int i = 0; i < size; i++) {
			row.emplace_back(0);
		}
		for (int i = 0; i < size; i++) {
			scoreMap.emplace_back(row);
		}
	};
	void go();
};

