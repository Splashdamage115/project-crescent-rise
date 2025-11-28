#include <iostream>
#include "tcpClient.h"
#include "TcpServer.h"

int main()
{
	char input = '1';
	//std::cout << "1 : Server\n2 : Client\nq : quit\n";
	//std::cin >> input;

	if (input == '1')
	{
		return TcpServer::setUpServer();
	}
	if (input == '2')
	{
		return TcpClient::Start();
	}

	system("pause");
	return -1;
}