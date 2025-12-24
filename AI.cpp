#include "AI.h"
#include <chrono>
#include <iostream>
#include<random>

void AI::calculateScore() {
	int personNum;//棋手方（黑棋）有多少连续的棋子
	int aiNum;//ai（白棋）有多少连续的棋子
	int emptyNum;//该方向上空白位个数

	//评分向量数组清零
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			scoreMap[i][j] = 0;
		}
	}

	for (int row = 0; row < size; row++) {
		for (int col = 0; col < size; col++) {
			//对每个点进行计算
			
			//默认0无子，有子跳过
			if (chess->getChessData(row, col))
				continue;
			for (int y = -1; y < 1; y++) {
				for (int x = -1; x < 2; x++) {

					//对每个点计算
					if (x == 0 && y == 0) continue;
					if (x != 1 && y == 0) continue;

					personNum = 0;
					aiNum = 0;
					emptyNum = 0;

					//假设黑棋在该位置落子，构成什么棋形
					for (int i = 1; i < 5; i++) {//找4个就够了（还有自己）
						int curRow = row + i * y;//y为正往下走
						int curCol = col + i * x;

						if (curRow > -1 && curRow<size &&
							curCol>-1 && curCol < size &&
							chess->getChessData(curRow, curCol) == 1) {
							personNum++;
						}
						else if (curRow > -1 && curRow<size &&
							curCol>-1 && curCol < size &&
							chess->getChessData(curRow, curCol) == 0) {
							emptyNum++;
							break;
						}
						else {
							break;
						}
					}

					//相反方向
					for (int i = 1; i < 5; i++) {//找4个就够了（还有自己）
						int curRow = row - i * y;//y为正往上走
						int curCol = col - i * x;

						if (curRow > -1 && curRow<size &&
							curCol>-1 && curCol < size &&
							chess->getChessData(curRow, curCol) == 1) {
							personNum++;
						}
						else if (curRow > -1 && curRow<size &&
							curCol>-1 && curCol < size &&
							chess->getChessData(curRow, curCol) == 0) {
							emptyNum++;
							break;
						}
						else {
							break;
						}
					}

					if (personNum == 1) {//连2
						scoreMap[row][col] += 10;
					}
					else if (personNum == 2) {
						if (emptyNum == 1) {//死3
							scoreMap[row][col] += 30;
						}
						else if (emptyNum == 2) {//活3
							scoreMap[row][col] += 40;
						}
					}
					else if (personNum == 3) {
						if (emptyNum == 1) {//死4
							scoreMap[row][col] += 60;
						}
						else if (emptyNum == 2) {//活4
							scoreMap[row][col] += 2000;
						}
					}
					else if (personNum == 4) {//5
						scoreMap[row][col] += 300000;
					}

					emptyNum = 0;
					//假设白棋在该位置落子，构成什么棋形
					for (int i = 1; i < 5; i++) {//找4个就够了（还有自己）
						int curRow = row + i * y;//y为正往下走
						int curCol = col + i * x;

						if (curRow > -1 && curRow<size &&
							curCol>-1 && curCol < size &&
							chess->getChessData(curRow, curCol) == -1) {
							aiNum++;
						}
						else if (curRow > -1 && curRow<size &&
							curCol>-1 && curCol < size &&
							chess->getChessData(curRow, curCol) == 0) {
							emptyNum++;
							break;
						}
						else {
							break;
						}
					}
					//相反方向
					for (int i = 1; i < 5; i++) {//找4个就够了（还有自己）
						int curRow = row - i * y;//y为正往上走
						int curCol = col - i * x;

						if (curRow > -1 && curRow<size &&
							curCol>-1 && curCol < size &&
							chess->getChessData(curRow, curCol) == -1) {
							personNum++;
						}
						else if (curRow > -1 && curRow<size &&
							curCol>-1 && curCol < size &&
							chess->getChessData(curRow, curCol) == 0) {
							emptyNum++;
							break;
						}
						else {
							break;
						}
					}
					if (aiNum == 0) {
						scoreMap[row][col] += 5;
					}
					else if (aiNum == 1) {//连2
						scoreMap[row][col] += 10;
					}
					else if (aiNum == 2) {
						if (emptyNum == 1) {//死3
							scoreMap[row][col] += 25;
						}
						else if (emptyNum == 2) {//活3
							scoreMap[row][col] += 50;
						}
					}
					else if (aiNum == 3) {
						if (emptyNum == 1) {//死4
							scoreMap[row][col] += 45;
						}
						else if (emptyNum == 2) {//活4
							scoreMap[row][col] += 200000;
						}
					}
					else if (aiNum == 4) {//5
						scoreMap[row][col] += 500000;
					}
				}
			}
		}

	}
}

ChessPos AI::think()
{
	calculateScore();

	std::vector<ChessPos> maxPoints;//暂存最大分数对应的点
	int maxScore = 0;
	for (int row = 0; row < size; row++) {
		for (int col = 0; col < size; col++) {
			if (chess->getChessData(row, col) == 0) {
				if (scoreMap[row][col] > maxScore) {
					maxPoints.clear();
					maxScore = scoreMap[row][col];
					maxPoints.emplace_back(row, col);
				}
				else if (scoreMap[row][col] == maxScore) {
					maxPoints.emplace_back(row, col);
				}
			}
		}
	}
	int index = rand() % maxPoints.size();
	printf("(%d,%d)\n", maxPoints[index].row, maxPoints[index].col);
	return maxPoints[index];
}

void AI::go()
{
	auto start = std::chrono::high_resolution_clock::now();
	ChessPos pos = think();
	auto end= std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	std::cout << "AI决策用时：" << duration.count() << "us" << std::endl;
	Sleep(1000);//停顿1s
	chess->chessDown(&pos,CHESS_WHITE);
}

