#pragma once

#include "Man.h"
#include "AI.h"
#include "Chess.h"

#include<iostream>
#include <vector>
#include <string>

class ChessGameMode
{
private:
	Man* man;
	AI* ai;
	Chess* chess;
	int count;//当前棋盘所有棋子总数，用来判和局
	int gradeSize;//棋盘长度
	int currentDifficulty;//难度
	std::vector<Button> buttons ;//菜单控件
	bool playerFlag;//true黑棋

private:
	bool checkWin();//检查棋局胜负，若胜负已分，分会true否则false
	bool checkOver();//检查棋局是否结束
	void start();//开始游戏
	void play();//对局开始


public:
	ChessGameMode() {};
	ChessGameMode(Man* man, AI* ai, Chess* chess)
		:man(man), ai(ai), chess(chess) ,count(0),gradeSize(chess->getGradeSize()),playerFlag(chess->playerFlag)
	{};
	void StartMenu();//开始菜单
	void endGameMenu();//游戏结束菜单
	void handleInput();//处理控件的鼠标点击事件
	void render();//绘制
	void updateDifficultyText();//更新难度文本
};

