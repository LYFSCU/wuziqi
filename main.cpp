#include"ChessGameMode.h"
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

int main(void) {
    // 在main函数开始处添加：
	ChessGameMode game;

    game.StartMenu();
    BeginBatchDraw(); // 启用双缓冲

    while (true) {
        game.render();
        game.handleInput();
    }
    EndBatchDraw();
    return 0;
}