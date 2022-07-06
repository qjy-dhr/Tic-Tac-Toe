#include "server.h"
//棋盘
char chess_server[row][col];
//接收结构体
struct Play {
	int ai;
	int first; //先手（1） 后手（0）
};

void display()
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			chess_server[i][j] = col * i + j + '0';
		}
	}
}

void print()
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			if (j == col - 1) {
				if (chess_server[i][j] == 'y') {
					cout << "__\u2605_" << endl;
				}
				else {
					cout << "__" << chess_server[i][j] << "__" << endl;
				}
			}
			else {
				if (chess_server[i][j] == 'y') {
					cout << "__\u2605_" << "|";
				}
				else {
					cout << "__" << chess_server[i][j] << "__" << "|";
				}
			}
		}
	}
	cout << endl;
}
int judge_full()
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			if (chess_server[i][j] == col * i + j + '0')
			{
				return 0;
			}
		}
	}
	return 1;
}
char judge_win()
{
	for (int i = 0; i < row; i++)
	{
		if ((chess_server[i][0] == chess_server[i][1]) && (chess_server[i][1] == chess_server[i][2]))
		{
			return chess_server[i][0];
		}
	}
	for (int j = 0; j < col; j++)
	{
		if ((chess_server[0][j] == chess_server[1][j]) && (chess_server[1][j] == chess_server[2][j]))
		{
			return chess_server[0][j];
		}
	}
	if ((chess_server[0][0] == chess_server[1][1]) && (chess_server[1][1] == chess_server[2][2]))
	{
		return chess_server[0][0];
	}
	else if ((chess_server[0][2] == chess_server[1][1]) && (chess_server[1][1] == chess_server[2][0]))
	{
		return chess_server[0][2];
	}
	return ' ';
}
int gameResult() {
	if (judge_win() == 'x') {
		return 1; //人获胜 
	}
	if (judge_win() == 'y') {
		return -1; //电脑获胜 
	}
	if (judge_full() == 1) {
		return -2;//平局
	}
	return 0;
}
int alpha_beta(int moveNow, int moveNext, int alpha, int beta) {
	// 判断是否结束   
	int gmresult = gameResult();
	if (gmresult != 0) {
		return gmresult;
	}
	//开始下一层
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			if (chess_server[i][j] == i * col + j + '0') {
				if (moveNow == -1)
					chess_server[i][j] = 'y';
				else if (moveNow == 1)
					chess_server[i][j] = 'x';
				int tem = alpha_beta(moveNext, moveNow, alpha, beta);
				chess_server[i][j] = i * col + j + '0';
				if (moveNow == 1) {  //max层，更新alpha,返回beta 
					if (tem > alpha) {
						alpha = tem;
					}
					if (alpha >= beta) {
						return beta;
					}
				}
				else {  //min层，更新beta,返回alpha
					if (tem < beta) {
						beta = tem;
					}
					if (alpha >= beta) {
						return alpha;
					}
				}
			}
		}
	}
	if (moveNow == 1) {  //max层，返回alpha
		return alpha;
	}
	return beta;
}
void calNext() {
	int i = 0;
	int bestValue = 1000, x = 0, y = 0;
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			//如果可以下在这个位置
			if (chess_server[i][j] == i * col + j + '0') {
				chess_server[i][j] = 'y';
				int tem = alpha_beta(1, -1, -100, 100);
				chess_server[i][j] = i * col + j + '0';
				if (tem < bestValue) {
					bestValue = tem;
					x = i;
					y = j;
				}
			}
		}
	}
	if (chess_server[x][y] == (x * col + y + '0') && x >= 0 && x < row && y >= 0 && y < col)
	{
		chess_server[x][y] = 'y';
	}
}

DWORD WINAPI player_server(LPVOID p)
{
	int x, y;
	while (1)
	{
		cout << "请输入位置（0~8）:";
		int temp;
		cin >> temp;
		x = temp / col + 1; y = temp % col + 1;
		if (chess_server[x - 1][y - 1] == ((x - 1) * col + y - 1 + '0') && x > 0 && x <= row && y > 0 && y <= col)
		{
			chess_server[x - 1][y - 1] = 'y';
			break;
		}
		else {
			cout << "落子失败，请重新落子" << endl;
		}
	}
	return 0;
}

int main()
{
	WSADATA wsadata;
	int WSAresult = WSAStartup(MAKEWORD(2, 2), &wsadata);
	if (WSAresult != 0)
	{
		cout << "初始化套接字库失败：" << WSAGetLastError << endl;
		return 1;
	}

	SOCKET server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (server == INVALID_SOCKET)
	{
		cout << "套接字绑定失败：" << WSAGetLastError << endl;
		WSACleanup();
		return 1;
	}

	struct sockaddr_in serveradd;
	memset(&serveradd, 0, sizeof(serveradd));
	serveradd.sin_family = AF_INET;
	serveradd.sin_addr.s_addr = htonl(INADDR_ANY);
	serveradd.sin_port = htons(2222);

	int bindresult = bind(server, (sockaddr*)&serveradd, sizeof(serveradd));
	if (bindresult == SOCKET_ERROR)
	{
		cout << "绑定失败" << WSAGetLastError << endl;
		closesocket(server);
		WSACleanup();
		return 1;
	}

	int listenresult = listen(server, SOMAXCONN);
	if (listenresult == SOCKET_ERROR)
	{
		cout << "listen failed with error" << WSAGetLastError << endl;
		closesocket(server);
		WSACleanup();
		return 1;
	}

	SOCKET conn;
	sockaddr_in connadd;
	int connaddlen;
	while (1)
	{
		cout << "等待用户连接..." << endl;
		connaddlen = sizeof(connadd);
		conn = accept(server, (sockaddr*)&connadd, &connaddlen);
		if (conn == INVALID_SOCKET)
		{
			cout << "连接失败：" << WSAGetLastError << endl;
			continue;
		}
		else {
			char connIP[20] = { 0 };
			char message[1024] = { 0 };
			struct Play play;
			inet_ntop(AF_INET, &connadd.sin_addr, connIP, 16);
			cout << "连接 IP：" << connIP << " 端口： " << ntohs(connadd.sin_port) << " 成功" << endl;
			while (1)
			{
				play.ai = -2;
				recv(conn, (char*)&play, sizeof(Play), 0);
				if (play.ai == 0) {
					cout << "双人模式" << endl;
					display();
					print();
					int over = 0;

					if (play.first == 0) {		
					while (1)
					{
						cout << "等待落子，倒计时60秒..." << endl;
						int nNetTimeout = PlayTime;
						setsockopt(conn, SOL_SOCKET, SO_RCVTIMEO, (char*)&nNetTimeout, sizeof(int));
						int recvresult = recv(conn, message, sizeof(message), 0);
						if (recvresult < 0)
						{
							cout << "响应超时，直接退出" << endl;
							break;
						}
						else if (recvresult == SOCKET_ERROR)
						{
							cout << "接受失败：" << WSAGetLastError << endl;
							over = 1;
							break;
						}
						else if (recvresult == 0)
						{
							cout << "连接关闭" << endl;
							over = 1;
							break;
						}
						else {
							for (int i = 0; i < row; i++)
							{
								for (int j = 0; j < col; j++)
								{
									if (message[col * i + j] != 'y' && message[col * i + j] != 'x')
									{
										chess_server[i][j] = col * i + j + '0';
									}
									else {
										chess_server[i][j] = message[col * i + j];
									}
								}
							}
							print();
							if (judge_win() == 'x')
							{
								cout << "client胜利" << endl;
								break;
							}
							else if (judge_full() == 1)
							{
								cout << "无人获胜" << endl;
								break;
							}
							cout << "请尽快落子，倒计时60秒..." << endl;
							HANDLE hThread2;
							hThread2 = CreateThread(NULL, 0, player_server, NULL, 0, NULL);
							int signal = WaitForSingleObject(hThread2, PlayTime);//设置运行60s
							//如果超出60s，则跳过该函数
							if (signal != WAIT_OBJECT_0)
							{
								cout << endl;
								cout << "超时，自动退出" << endl;
								TerminateProcess(hThread2, -1);
								CloseHandle(hThread2);
								break;
							}
							CloseHandle(hThread2);
							print();
							for (int i = 0; i < row; i++)
							{
								for (int j = 0; j < col; j++)
								{
									if (chess_server[i][j] == 'y' || chess_server[i][j] == 'x')
									{
										message[col * i + j] = chess_server[i][j];
									}
									else {
										message[col * i + j] = col * i + j + '0';
									}
								}
							}
							int sendresult = send(conn, message, strlen(message), 0);
							if (sendresult == SOCKET_ERROR)
							{
								cout << "发送失败：" << WSAGetLastError << endl;
								break;
							}
							if (judge_win() == 'y')
							{
								cout << "server胜利" << endl;
								break;
							}
							else if (judge_full() == 1)
							{
								cout << "无人获胜" << endl;
								break;
							}
						}
						memset(message, 0, sizeof(message));
					}
					}
					else {
					while (1)
					{
						cout << "请尽快落子，倒计时60秒..." << endl;
						HANDLE hThread2;
						hThread2 = CreateThread(NULL, 0, player_server, NULL, 0, NULL);
						int signal = WaitForSingleObject(hThread2, PlayTime);//设置运行60s
						//如果超出60s，则跳过该函数
						if (signal != WAIT_OBJECT_0)
						{
							cout << endl;
							cout << "超时，自动退出" << endl;
							TerminateProcess(hThread2, -1);
							CloseHandle(hThread2);
							break;
						}
						CloseHandle(hThread2);
						print();
						for (int i = 0; i < row; i++)
						{
							for (int j = 0; j < col; j++)
							{
								if (chess_server[i][j] == 'y' || chess_server[i][j] == 'x')
								{
									message[col * i + j] = chess_server[i][j];
								}
								else {
									message[col * i + j] = col * i + j + '0';
								}
							}
						}
						int sendresult = send(conn, message, strlen(message), 0);
						if (sendresult == SOCKET_ERROR)
						{
							cout << "发送失败" << WSAGetLastError << endl;
							break;
						}
						else {
							if (judge_win() == 'y')
							{
								cout << "server胜利" << endl;
								break;
							}
							else if (judge_full() == 1)
							{
								cout << "无人获胜" << endl;
								break;
							}
							cout << "等待落子，倒计时60秒..." << endl;
							int nNetTimeout = PlayTime;
							setsockopt(conn, SOL_SOCKET, SO_RCVTIMEO, (char*)&nNetTimeout, sizeof(int));
							int recvresult = recv(conn, message, sizeof(message), 0);
							if (recvresult < 0)
							{
								cout << "响应超时，直接退出" << endl;
								break;
							}
							else if (recvresult == SOCKET_ERROR)
							{
								cout << "接受失败：" << WSAGetLastError << endl;
								over = 1;
								break;
							}
							else if (recvresult == 0)
							{
								cout << "连接关闭" << endl;
								over = 1;
								break;
							}
							else {
								for (int i = 0; i < row; i++)
								{
									for (int j = 0; j < col; j++)
									{
										if (message[col * i + j] != 'y' && message[col * i + j] != 'x')
										{
											chess_server[i][j] = col * i + j + '0';
										}
										else {
											chess_server[i][j] = message[col * i + j];
										}
									}
								}
								print();
								if (judge_win() == 'x')
								{
									cout << "client胜利" << endl;
									break;
								}
								else if (judge_full() == 1)
								{
									cout << "无人获胜" << endl;
									break;
								}
							}
						}
						memset(message, 0, sizeof(message));					}
                     }
					 if (over == 1)	break;
				}
				else if (play.ai == 1) {
				    cout << "AI模式" << endl;
					display();
					print();
					int over = 0;
					if (play.first == 0) {			
					while (1)
					{
						cout << "等待落子，倒计时60秒..." << endl;
						int nNetTimeout = PlayTime;
						setsockopt(conn, SOL_SOCKET, SO_RCVTIMEO, (char*)&nNetTimeout, sizeof(int));
						int recvresult = recv(conn, message, sizeof(message), 0);
						if (recvresult < 0)
						{
							cout << "响应超时，退出" << endl;
							break;
						}
						else if (recvresult == SOCKET_ERROR)
						{
							cout << "接受失败：" << WSAGetLastError << endl;
							over = 1;
							break;
						}
						else if (recvresult == 0)
						{
							cout << "连接关闭" << endl;
							over = 1;
							break;
						}
						else {
							for (int i = 0; i < row; i++)
							{
								for (int j = 0; j < col; j++)
								{
									if (message[col * i + j] != 'y' && message[col * i + j] != 'x')
									{
										chess_server[i][j] = col * i + j + '0';
									}
									else {
										chess_server[i][j] = message[col * i + j];
									}
								}
							}
							print();
							if (judge_win() == 'x')
							{
								cout << "client胜利" << endl;
								break;
							}
							else if (judge_full() == 1)
							{
								cout << "无人获胜" << endl;
								break;
							}
							calNext();
							print();
							for (int i = 0; i < row; i++)
							{
								for (int j = 0; j < col; j++)
								{
									if (chess_server[i][j] == 'y' || chess_server[i][j] == 'x')
									{
										message[col * i + j] = chess_server[i][j];
									}
									else {
										message[col * i + j] = col * i + j + '0';
									}
								}
							}
							int sendresult = send(conn, message, strlen(message), 0);
							if (sendresult == SOCKET_ERROR)
							{
								cout << "发送失败：" << WSAGetLastError << endl;
								break;
							}
							if (judge_win() == 'y')
							{
								cout << "AI胜利" << endl;
								break;
							}
							else if (judge_full() == 1)
							{
								cout << "无人获胜" << endl;
								break;
							}
						}
						memset(message, 0, sizeof(message));
					}
					}	
					else {
						while (1)
						{
							calNext();
							print();
							for (int i = 0; i < row; i++)
							{
								for (int j = 0; j < col; j++)
								{
									if (chess_server[i][j] == 'y' || chess_server[i][j] == 'x')
									{
										message[col * i + j] = chess_server[i][j];
									}
									else {
										message[col * i + j] = col * i + j + '0';
									}
								}
							}
							int sendresult = send(conn, message, strlen(message), 0);
							if (sendresult == SOCKET_ERROR)
							{
								cout << "发送失败" << WSAGetLastError << endl;
								break;
							}
							else {
								if (judge_win() == 'y')
								{
									cout << "server胜利" << endl;
									break;
								}
								else if (judge_full() == 1)
								{
									cout << "无人获胜" << endl;
									break;
								}
								cout << "等待落子，倒计时60秒..." << endl;
								int nNetTimeout = PlayTime;
								setsockopt(conn, SOL_SOCKET, SO_RCVTIMEO, (char*)&nNetTimeout, sizeof(int));
								int recvresult = recv(conn, message, sizeof(message), 0);
								if (recvresult < 0)
								{
									cout << "响应超时，直接退出" << endl;
									break;
								}
								else if (recvresult == SOCKET_ERROR)
								{
									cout << "接受失败：" << WSAGetLastError << endl;
									over = 1;
									break;
								}
								else if (recvresult == 0)
								{
									cout << "连接关闭" << endl;
									over = 1;
									break;
								}
								else {
									for (int i = 0; i < row; i++)
									{
										for (int j = 0; j < col; j++)
										{
											if (message[col * i + j] != 'y' && message[col * i + j] != 'x')
											{
												chess_server[i][j] = col * i + j + '0';
											}
											else {
												chess_server[i][j] = message[col * i + j];
											}
										}
									}
									print();
									if (judge_win() == 'x')
									{
										cout << "client胜利" << endl;
										break;
									}
									else if (judge_full() == 1)
									{
										cout << "无人获胜" << endl;
										break;
									}
								}
							}
							memset(message, 0, sizeof(message));
						}
					}
					if (over == 1)	break;			

				}
				else if (play.ai == -1) {
					break;
				}
			}
		}
		closesocket(conn);
	}

	closesocket(server);
	WSACleanup();

	return 0;
}
