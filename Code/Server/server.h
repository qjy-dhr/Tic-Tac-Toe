#pragma once
#include<iostream>
#include<time.h>
#include <WS2tcpip.h>
#pragma comment (lib,"ws2_32.lib")
#define row 3
#define col 3
#define PlayTime 60000
using namespace std;

void display();//初始化棋盘
void print();//打印棋盘
char judge_win();//判断游戏结束，赢方
int judge_full();//判断棋盘是否全被占满
int gameResult();//游戏结果
int alpha_beta(int moveNow, int moveNext, int alpha, int beta);//α，β减枝策略
void calNext();//模拟电脑下棋，选择策略最小的
DWORD WINAPI player_server(LPVOID p);//双人时的下棋方法