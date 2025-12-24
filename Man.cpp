#include "Man.h"

void Man::go()
{
	MOUSEMSG msg;
	ChessPos pos;

	while (1) {
		
		//获取鼠标点击消息
		if (MouseHit()) {//鼠标事件
			MOUSEMSG msg = GetMouseMsg();
			if (msg.uMsg == WM_LBUTTONDOWN) {
				if (msg.x <= 900 && msg.y <= 895) {
					//通过chess对象，来调用判断落子是否有效，以及落子功能
					if (msg.uMsg == WM_LBUTTONDOWN && chess->clickBoard(msg.x, msg.y, &pos)) {
						break;
					}
				}
			}
		}
	}
	printf("%d,%d\n", pos.row, pos.col);

	
	
	//落子
	chess->chessDown(&pos, CHESS_BLACK);

}

