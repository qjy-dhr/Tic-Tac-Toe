#pragma once
#include<iostream>
#include<time.h>
#include <WS2tcpip.h>
#pragma comment (lib,"ws2_32.lib")
#define row 3
#define col 3
#define PlayTime 60000
using namespace std;

void display();//��ʼ������
void print();//��ӡ����
char judge_win();//�ж���Ϸ������Ӯ��
int judge_full();//�ж������Ƿ�ȫ��ռ��
int gameResult();//��Ϸ���
int alpha_beta(int moveNow, int moveNext, int alpha, int beta);//�����¼�֦����
void calNext();//ģ��������壬ѡ�������С��
DWORD WINAPI player_server(LPVOID p);//˫��ʱ�����巽��