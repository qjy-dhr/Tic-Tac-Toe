#pragma once
#include<iostream>
#include<time.h>
#include <WS2tcpip.h>
#define row 3 //棋盘的行
#define col 3 //棋盘的列
#define PlayTime 60000 //下一步棋的时间 60s
#pragma comment (lib,"ws2_32.lib")
using namespace std;

void display();//初始化棋盘
void print();//打印棋盘
char judge_win();//判断游戏结束，赢方
int judge_full();//判断棋盘是否全被占满
DWORD WINAPI player_client(LPVOID p);//用户下棋函数。多线程函数，用于判断时间（60s）