#include<iostream>
#include<ctime>
#include<Windows.h>//gotoxy
#include<conio.h>//监听键盘输入
#define MAP_MODE "■"
#define HEAD "0" //蛇头
#define BODY "O" //蛇身
# define MAP_WIDTH 60
# define MAP_HEIGHT 30
# define MOVE_CENTER 5
# define INITLEN 3 //蛇长度初始为3
# define MAXLEN 30 //蛇长度最大为30

//将光标移动到控制台的(x,y)坐标点处
void gotoxy(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void hideCursor() {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;

	GetConsoleCursorInfo(consoleHandle, &cursorInfo);
	cursorInfo.bVisible = FALSE;  // 隐藏光标
	SetConsoleCursorInfo(consoleHandle, &cursorInfo);
}


//创建蛇身
struct Snake {
	int x[MAXLEN];
	int y[MAXLEN];
	int currentLen;//长度
}snake;

//创建食物
struct Food {
	int x;
	int y;
}food;
int direct = 'a';
int flag = 1; //是否需要创建食物

int score = 0;
int gameOver = 0;  // 游戏结束标志
//显示得分
void displayScore() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
		FOREGROUND_INTENSITY | FOREGROUND_BLUE);
	gotoxy(MAP_WIDTH + MOVE_CENTER + 5, 5);
	printf("Score: %d", score);
	gotoxy(MAP_WIDTH + MOVE_CENTER + 5, 8);
	printf("Length: %d", snake.currentLen);
}

//创建地图
void createMap() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	for (int i = 0 + MOVE_CENTER; i < MAP_WIDTH + MOVE_CENTER; i += 2) {
		gotoxy(i, 0);//改变x,最上面那一条边
		printf(MAP_MODE);
		gotoxy(i, MAP_HEIGHT - 1);//最下面那一条边
		printf(MAP_MODE);
	}
	for (int i = 0; i < MAP_HEIGHT; i++) {
		gotoxy(0 + MOVE_CENTER, i);
		printf(MAP_MODE);
		gotoxy(MAP_WIDTH + MOVE_CENTER, i);
		printf(MAP_MODE);
	}
	gotoxy(200, 0);

}
#pragma once

//生成食物
//void createFood() {
//	int isCreate = 1;
//	food.x = MOVE_CENTER+ rand() % (MAP_WIDTH - 4);
//	food.y = rand() % (MAP_HEIGHT-1);
//
//
//	//食物的坐标不能在蛇的身上
//	for (int i = 0; i < snake.currentLen; i++) {
//		if (snake.x[i] == food.x && snake.y[i] == food.y) {
//			isCreate = 0;
//		}
//	}
//	//食物的坐标不能在边框上
//	if (food.x <= MOVE_CENTER || food.x > MAP_WIDTH + MOVE_CENTER - 2 || food.y < 0 || food.y > MAP_HEIGHT) {
//		isCreate = 0;
//	}
//
//	if (isCreate) {
//		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);//使打印字体变为蓝色
//		gotoxy(food.x + MOVE_CENTER, food.y);
//		printf(MAP_MODE);
//		flag = 0;
//	}
//	else {
//		createFood();
//	}
//
//}

void createFood() {
	int maxAttempts = 1000;  // 最大尝试次数，避免无限循环
	int attempts = 0;

	while (attempts < maxAttempts) {
		// 生成随机坐标
		food.x = MOVE_CENTER + rand() % (MAP_WIDTH - 4);
		food.y = rand() % (MAP_HEIGHT - 1);

		// 检查是否在有效区域内
		if (food.x <= MOVE_CENTER || food.x >= MAP_WIDTH + MOVE_CENTER - 2 ||
			food.y <= 0 || food.y >= MAP_HEIGHT - 1) {
			attempts++;
			continue;
		}

		// 检查是否与蛇身重叠
		int overlap = 0;
		for (int i = 0; i < snake.currentLen; i++) {
			if (snake.x[i] == food.x && snake.y[i] == food.y) {
				overlap = 1;
				break;
			}
		}

		if (!overlap) {
			// 找到合适位置，绘制食物
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
				FOREGROUND_INTENSITY | FOREGROUND_BLUE);
			gotoxy(food.x, food.y);
			printf(MAP_MODE);
			flag = 0;
			return;
		}

		attempts++;
	}

	// 如果随机生成失败，可以考虑其他策略
	// 比如遍历寻找空位或者放宽条件
}

//蛇的初始化
void initSnake() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
		FOREGROUND_INTENSITY | FOREGROUND_BLUE);
	snake.currentLen = INITLEN;
	snake.x[0] = MAP_WIDTH + MOVE_CENTER - 10;
	snake.y[0] = MAP_HEIGHT - 10;
	gotoxy(snake.x[0], snake.y[0]);
	printf(HEAD);

	for (int i = 1; i < snake.currentLen; i++) {
		snake.x[i] = snake.x[i - 1] - 2;
		snake.y[i] = snake.y[i - 1];
		gotoxy(snake.x[i], snake.y[i]);
		printf(BODY);
	}
	gotoxy(0, 200);
}
//蛇的移动 
void moveSnake() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
		FOREGROUND_INTENSITY | FOREGROUND_BLUE);

	if (_kbhit()) {
		fflush(stdin);
		direct = _getch();
	}

	// 保存当前蛇尾位置
	int tailX = snake.x[snake.currentLen - 1];
	int tailY = snake.y[snake.currentLen - 1];

	// 擦除最后一节蛇尾
	gotoxy(snake.x[snake.currentLen - 1], snake.y[snake.currentLen - 1]);
	printf(" ");

	// 开始替换坐标
	for (int i = snake.currentLen - 1; i > 0; i--) {
		snake.x[i] = snake.x[i - 1];
		snake.y[i] = snake.y[i - 1];
		gotoxy(snake.x[i], snake.y[i]);
		printf(BODY);
	}

	switch (direct) {
	case 'W':
	case 'w':
		snake.y[0]--;
		break;
	case 'A':
	case 'a':
		snake.x[0] -= 1;
		break;
	case 'S':
	case 's':
		snake.y[0]++;
		break;
	case 'D':
	case 'd':
		snake.x[0] += 1;
		break;
	}

	// 移动之后，新蛇头的位置
	gotoxy(snake.x[0], snake.y[0]);
	printf(HEAD);

	// 判断蛇头是否和食物重合（只保留这一处处理）
	if (snake.x[0] == food.x && snake.y[0] == food.y) {
		snake.currentLen++;  // 增加长度
		score++;             // 增加得分
		flag = 1;            // 标记需要生成新食物

		// 立即在蛇尾位置添加新的身体节点
		snake.x[snake.currentLen - 1] = tailX;
		snake.y[snake.currentLen - 1] = tailY;
		gotoxy(tailX, tailY);
		printf(BODY);
	}

	// 删除原来重复的 if 判断代码块
}

//判断蛇头是否碰到墙壁
bool statemente() {
	if (snake.x[0] <= MOVE_CENTER || snake.x[0] > MAP_WIDTH + MOVE_CENTER - 2 || snake.y[0] < 0 || snake.y[0] > MAP_HEIGHT) {
		return 0;
	}
	else {
		return 1;
	}
}

//判断蛇头是否和蛇身重合
bool statemente2() {
	for (int i = 1; i < snake.currentLen; i++) {
		if (snake.x[0] == snake.x[i] && snake.y[0] == snake.y[i]) {
			return 0;
		}
	}
	return 1;
}

// 重置游戏状态的函数
void resetGame() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
		FOREGROUND_INTENSITY | FOREGROUND_BLUE);
	// 重置游戏变量
	score = 0;
	gameOver = 0;
	flag = 1;
	direct = 'a';

	// 清空游戏区域
	for (int i = 1; i < MAP_HEIGHT - 1; i++) {
		for (int j = MOVE_CENTER + 2; j < MAP_WIDTH + MOVE_CENTER - 2; j += 2) {
			gotoxy(j, i);
			printf(" ");
		}
	}

	// 重新初始化蛇
	initSnake();

	// 重新显示初始信息
	displayScore();
}

// 修改后的主函数
int main() {
	srand(time(NULL));
	hideCursor();

	while (1) {
		createMap();
		resetGame();  // 使用resetGame替代部分初始化代码

		// 游戏主循环
		while (!gameOver) {
			Sleep(100);

			if (flag) {
				createFood();
			}
			moveSnake();
			displayScore();

			// 检查游戏是否结束
			if (statemente() == 0 || statemente2() == 0) {
				gameOver = 1;
			}
		}

		gotoxy(MAP_WIDTH + MOVE_CENTER + 5, 15);
		printf("Game Over!\n");
		printf("按回车键重新开始");
		// 等待回车键重新开始
		while (1) {
			if (_kbhit()) {
				char key = _getch();
				if (key == '\r' || key == '\n') {  // 检测回车键
					break;
				}
			}
		}

		// 清空屏幕准备重新开始
		system("cls");
	}

	return 0;
}