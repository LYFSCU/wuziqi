#pragma once
#include "Chess.h"

class Man
{
public:
	Man(Chess* chess)
		:chess(chess) {};
	void go();
private:
	Chess* chess;
};

