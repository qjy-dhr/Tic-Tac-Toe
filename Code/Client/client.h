#pragma once
#include<iostream>
#include<time.h>
#include <WS2tcpip.h>
#define row 3 //���̵���
#define col 3 //���̵���
#define PlayTime 60000 //��һ�����ʱ�� 60s
#pragma comment (lib,"ws2_32.lib")
using namespace std;

void display();//��ʼ������
void print();//��ӡ����
char judge_win();//�ж���Ϸ������Ӯ��
int judge_full();//�ж������Ƿ�ȫ��ռ��
DWORD WINAPI player_client(LPVOID p);//�û����庯�������̺߳����������ж�ʱ�䣨60s��