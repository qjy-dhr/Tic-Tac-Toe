#include "client.h"

//���������ڿͻ��������䴫��
char chess_client[row][col];
//����ṹ�崫���û�ѡ�����Ϸ����
struct Play {
	int ai;//˫�˶�ս��0�� �˻���ս��1��
	int first; //���֣�0�� ���֣�1��
};

void display()
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			chess_client[i][j] = col * i + j + '0';
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
				if (chess_client[i][j] == 'y') {
					cout << "__\u2605_" << endl;
				}
				else {
					cout << "__" << chess_client[i][j] << "__" << endl;
				}
			}
			else {
				if (chess_client[i][j] == 'y') {
					cout << "__\u2605_" << "|";;
				}
				else {
					cout << "__" << chess_client[i][j] << "__" << "|";
				}
			}
		}
	}
	cout << endl;
}

char judge_win()
{
	for (int i = 0; i < row; i++)
	{
		if ((chess_client[i][0] == chess_client[i][1]) && (chess_client[i][1] == chess_client[i][2]))
		{
			return chess_client[i][0];
		}
	}
	for (int j = 0; j < col; j++)
	{
		if ((chess_client[0][j] == chess_client[1][j]) && (chess_client[1][j] == chess_client[2][j]))
		{
			return chess_client[0][j];
		}
	}
	if ((chess_client[0][0] == chess_client[1][1]) && (chess_client[1][1] == chess_client[2][2]))
	{
		return chess_client[0][0];
	}
	else if ((chess_client[0][2] == chess_client[1][1]) && (chess_client[1][1] == chess_client[2][0]))
	{
		return chess_client[0][2];
	}
	return ' ';
}

int judge_full()
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			if (chess_client[i][j] == col * i + j + '0')
			{
				return 0;
			}
		}
	}
	return 1;
}
DWORD WINAPI player_client(LPVOID p)
{
	int x, y;
	while (1)
	{
		cout << "������λ�ã�0~8��:";
		int temp;
		cin >> temp;
		x = temp / col + 1; y = temp % col + 1;
		if (chess_client[x - 1][y - 1] == ((x - 1) * col + y - 1 + '0') && x > 0 && x <= row && y > 0 && y <= col)
		{
			chess_client[x - 1][y - 1] = 'x';
			break;
		}
		else {
			cout << "����ʧ�ܣ�����������" << endl;
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
		cout << "��ʼ���׽��ֿ�ʧ�ܣ�" << WSAGetLastError << endl;
		return 1;
	}
	//�����׽���
	SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (client == INVALID_SOCKET)
	{
		cout << "�׽��ְ�ʧ�ܣ�" << WSAGetLastError << endl;
		WSACleanup();
		return 1;
	}
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &serveraddr.sin_addr.s_addr);
	serveraddr.sin_port = htons(2222);
	int connectresult = connect(client, (sockaddr*)&serveraddr, sizeof(serveraddr));
	if (connectresult == SOCKET_ERROR)
	{
		cout << "����ʧ�ܣ�" << WSAGetLastError << endl;
		closesocket(client);
		WSACleanup();
		return 1;
	}
	cout << "���ӳɹ���" << endl;
	struct Play play;
	char sendmessage[1024] = { 0 };
	char recvmessage[1024] = { 0 };
	while (1)
	{
		cout << "ѡ��ģʽ:˫�˶�ս��0�� �˻���ս��1��:";
		cin >> play.ai;//ʹ��ʹ��AI
		cout << "ѡ���Ⱥ���:���֣�0�� ���֣�1��:";
		cin >> play.first;
		int sendresult = send(client, (char*)&play, sizeof(Play), 0);
		cout << "�Ծֿ�ʼ" << endl;
		display();
		print();
		cout << "x=>�����Լ�" << endl;
		cout << "��=>����Է��Ѿ��µĸ���" << endl;
		cout << "0~8����=>�������ӵĵط�" << endl;
		cout << endl;
		if (play.first == 0) {
		while (1)
		{
			cout << "�뾡�����ӣ�����ʱ60��..." << endl;
			HANDLE hThread1;
			hThread1 = CreateThread(NULL, 0, player_client, NULL, 0, NULL);
			int signal = WaitForSingleObject(hThread1, PlayTime);//��������60s
			//�������60s���������ú���
			if (signal != WAIT_OBJECT_0)
			{
				cout << "��ʱ���Զ��˳�" << endl;
				TerminateProcess(hThread1, -1);
				CloseHandle(hThread1);
				break;
			}
			CloseHandle(hThread1);
			print();
			for (int i = 0; i < row; i++)
			{
				for (int j = 0; j < col; j++)
				{
					if (chess_client[i][j] == 'y' || chess_client[i][j] == 'x')
					{
						sendmessage[col * i + j] = chess_client[i][j];
					}
					else {
						sendmessage[col * i + j] = col * i + j + '0';
					}
				}
			}
			int sendresult = send(client, sendmessage, strlen(sendmessage), 0);
			if (sendresult == SOCKET_ERROR)
			{
				cout << "����ʧ��" << WSAGetLastError << endl;
				break;
			}
			else {
				if (judge_win() == 'x')
				{
					cout << "clientʤ��" << endl;
					break;
				}
				else if (judge_full() == 1)
				{
					cout << "���˻�ʤ" << endl;
					break;
				}
				cout << "�ȴ��Է����ӣ�����ʱ60��..." << endl;
				int nNetTimeout = PlayTime;
				setsockopt(client, SOL_SOCKET, SO_RCVTIMEO, (char*)&nNetTimeout, sizeof(int));
				int recvresult = recv(client, recvmessage, sizeof(recvmessage), 0);
				if (recvresult < 0)
				{
					cout << "�Է�Ӧ��ʱ���˳�" << recvresult << endl;
					break;
				}
				if (recvresult == SOCKET_ERROR)
				{
					cout << "����ʧ��" << WSAGetLastError << endl;
					break;
				}
				else if (recvresult == 0)
				{
					cout << "���ӹر�" << endl;
					break;
				}
				else {
					for (int i = 0; i < row; i++)
					{
						for (int j = 0; j < col; j++)
						{
							if (recvmessage[col * i + j] != 'y' && recvmessage[col * i + j] != 'x')
							{
								chess_client[i][j] = col * i + j + '0';
							}
							else {
								chess_client[i][j] = recvmessage[col * i + j];
							}
						}
					}
					print();
					if (judge_win() == 'y')
					{
						cout << "serverʤ��" << endl;
						break;
					}
					else if (judge_full() == 1)
					{
						cout << "���˻�ʤ" << endl;
						break;
					}
				}
			}
			memset(sendmessage, 0, sizeof(sendmessage));
			memset(recvmessage, 0, sizeof(recvmessage));
		}	
		}	
		else {
			while (1)
			{
				cout << "�ȴ��Է����ӣ�����ʱ60��..." << endl;
				int nNetTimeout = PlayTime;
				setsockopt(client, SOL_SOCKET, SO_RCVTIMEO, (char*)&nNetTimeout, sizeof(int));
				int recvresult = recv(client, recvmessage, sizeof(recvmessage), 0);
				if (recvresult < 0)
				{
					cout << "�Է�Ӧ��ʱ���˳�" << recvresult << endl;
					break;
				}
				if (recvresult == SOCKET_ERROR)
				{
					cout << "����ʧ��" << WSAGetLastError << endl;
					break;
				}
				else if (recvresult == 0)
				{
					cout << "���ӹر�" << endl;
					break;
				}
				else {
					for (int i = 0; i < row; i++)
					{
						for (int j = 0; j < col; j++)
						{
							if (recvmessage[col * i + j] != 'y' && recvmessage[col * i + j] != 'x')
							{
								chess_client[i][j] = col * i + j + '0';
							}
							else {
								chess_client[i][j] = recvmessage[col * i + j];
							}
						}
					}
					print();
					if (judge_win() == 'y')
					{
						cout << "serverʤ��" << endl;
						break;
					}
					else if (judge_full() == 1)
					{
						cout << "���˻�ʤ" << endl;
						break;
					}
					cout << "�뾡�����ӣ�����ʱ60��..." << endl;
					HANDLE hThread1;
					hThread1 = CreateThread(NULL, 0, player_client, NULL, 0, NULL);
					int signal = WaitForSingleObject(hThread1, PlayTime);//��������60s
					//�������60s���������ú���
					if (signal != WAIT_OBJECT_0)
					{
						cout << "��ʱ���Զ��˳�" << endl;
						TerminateProcess(hThread1, -1);
						CloseHandle(hThread1);
						break;
					}
					CloseHandle(hThread1);
					print();
					for (int i = 0; i < row; i++)
					{
						for (int j = 0; j < col; j++)
						{
							if (chess_client[i][j] == 'y' || chess_client[i][j] == 'x')
							{
								sendmessage[col * i + j] = chess_client[i][j];
							}
							else {
								sendmessage[col * i + j] = col * i + j + '0';
							}
						}
					}
					int sendresult = send(client, sendmessage, strlen(sendmessage), 0);
					if (sendresult == SOCKET_ERROR)
					{
						cout << "����ʧ��" << WSAGetLastError << endl;
						break;
					}
					if (judge_win() == 'x')
					{
						cout << "clientʤ��" << endl;
						break;
					}
					else if (judge_full() == 1)
					{
						cout << "���˻�ʤ" << endl;
						break;
					}
				}
				memset(sendmessage, 0, sizeof(sendmessage));
				memset(recvmessage, 0, sizeof(recvmessage));
			}
		}
		cout << "������Ϸ��0�� ������Ϸ��1��:";
		int a;
		cin >> a;
		if (a == 0)
		{
			play.ai = -1;
			send(client, (char*)&play, sizeof(Play), 0);
			cout << "��Ϸ����" << endl;
			break;
		}
	}

	closesocket(client);
	WSACleanup();

	return 0;
}
