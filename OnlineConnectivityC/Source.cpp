#include <iostream>
#include "tcpClient.h"
#include "TcpServer.h"
#include <string>

int main()
{
	char input = '1';
	
	

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