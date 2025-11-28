#include "TcpClient.h"
#include <string>


bool TcpClient::loopActive = true;
int TcpClient::playerId = -1;
SOCKET TcpClient::tcpSocket;
SOCKET TcpClient::udpSocket;
std::queue<std::string> TcpClient::tcpQueue;
std::mutex TcpClient::tcpMutex;
std::queue<std::string> TcpClient::recievedData;

// start the online connection client
int TcpClient::Start() 
{
    int errorResult = SetUpConnections();
    if (errorResult != 0)
    {
        return errorResult;
    }

    initiliseDataFromServer();

    std::thread IOthread(LoopConnection);
    IOthread.detach();



    std::string sendMessage = "";
    std::string message;

    while (loopActive) 
    {
        std::cout << "Enter message (type 'exit' to quit): ";
        std::cin >> message;
    
        if (message == "exit") break;

        sendMessage.clear();
        sendMessage += std::to_string(playerId);
        sendMessage += " > ";
        sendMessage += message;

        pushTcp(sendMessage);
    }


    closesocket(tcpSocket);
    closesocket(udpSocket);
    WSACleanup();
    return 0;
}

int TcpClient::initiliseDataFromServer()
{
    char buffer[BUFFER_SIZE];

    int bytesReceived = recv(tcpSocket, buffer, BUFFER_SIZE, 0);
    if (bytesReceived > 0)
    {
        playerId = std::stoi(buffer);
    }
    else
    {
        std::cout << "Didnt recieve player ID" << std::endl;
        return 1;
    }
    bytesReceived = recv(tcpSocket, buffer, BUFFER_SIZE, 0);
    if (bytesReceived > 0)
    {
        std::cout << "Server: " << std::string(buffer, bytesReceived) << std::endl;
    }

    return 0;
}

// set up the tcp & udp connections
int TcpClient::SetUpConnections()
{
    int errorResult = 0;
    errorResult = SetUpTcp();
    if (errorResult != 0)
    {
        std::cout << "ERROR SETTING UP TCP" << std::endl;
        return errorResult;
    }
    errorResult = SetUpUdp();
    if (errorResult != 0)
    {
        std::cout << "ERROR SETTING UP UDP" << std::endl;
        return errorResult;
    }
    return 0;
}

// set up the udp connection
int TcpClient::SetUpUdp()
{
    struct sockaddr_in si_other;
    int slen = sizeof(si_other);
    char buf[BUFFER_SIZE];
    char message[BUFFER_SIZE];
    WSADATA wsa;

    //Initialise winsock
    std::cout << "\nInitialising Winsock...\n";
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        std::cout << "Failed. Error Code : %d" << WSAGetLastError() << std::endl;
        return 1;
    }
    std::cout << "Initialised.\n";

    //create socket
    if ((udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
    {
        std::cout << "socket() failed with error code : %d" << WSAGetLastError() << std::endl;
        return 1;
    }

    return 0;
}

// set up the tcp connection
int TcpClient::SetUpTcp()
{
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0)
    {
        std::cerr << "WSAStartup failed" << std::endl;
        return 1;
    }

    addrinfo hints = {}, * addrResult = nullptr;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    result = getaddrinfo(SERVER_IP, DEFAULT_PORT, &hints, &addrResult);
    if (result != 0)
    {
        std::cerr << "getaddrinfo failed" << std::endl;
        WSACleanup();
        return 1;
    }

    tcpSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol);
    if (tcpSocket == INVALID_SOCKET)
    {
        std::cerr << "Socket creation failed" << std::endl;
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }

    result = connect(tcpSocket, addrResult->ai_addr, (int)addrResult->ai_addrlen);
    if (result == SOCKET_ERROR)
    {
        std::cerr << "Unable to connect to server" << std::endl;
        closesocket(tcpSocket);
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(addrResult);

    return 0;
}

// safely push data, ready to send
void TcpClient::pushTcp(std::string& data)
{
    std::lock_guard<std::mutex> lock(tcpMutex);
    tcpQueue.push(data);
}

// safely pop the data, and send it
bool TcpClient::popTcp(std::string& data)
{
    std::lock_guard<std::mutex> lock(tcpMutex);
    if (tcpQueue.empty()) return false;
    data = tcpQueue.front();
    tcpQueue.pop();
    return true;
}

void TcpClient::LoopConnection()
{
    fd_set readFds, writeFds;

    struct sockaddr_in si_other;
    int slen = sizeof(si_other);
    memset((char*)&si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(UDP_PORT);
    si_other.sin_addr.S_un.S_addr = inet_addr(SERVER_IP);

    char buffer[BUFFER_SIZE];
    std::string topOfQueue;
    timeval timeout{};
    timeout.tv_sec = 0;
    timeout.tv_usec = 1000;

    std::string position;
    position += std::to_string(playerId);
    position += " > ";
    position += " {0,0,0}";

    // loop the data transfer
    while (loopActive)
    {
        FD_ZERO(&readFds);
        FD_ZERO(&writeFds);

        FD_SET(tcpSocket, &readFds);
        FD_SET(udpSocket, &readFds);

        FD_SET(tcpSocket, &writeFds);
        FD_SET(udpSocket, &writeFds);

        // check if buffers are ready for data
        int result = select(0, &readFds, &writeFds, nullptr, &timeout);

        if (result > 0)
        {
            // recieve data udp
            if (FD_ISSET(udpSocket, &readFds))
            {
                int bytesReceived = recvfrom(udpSocket, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&si_other, &slen);
                if (bytesReceived > 0)
                {
                    // handle udp data
                    //std::cout << "UDP DATA : " << std::string(buffer, bytesReceived) << "\n";
                }
            }

            // recieve data tcp
            if (FD_ISSET(tcpSocket, &readFds))
            {
                int bytesReceived = recv(tcpSocket, buffer, BUFFER_SIZE, 0);
                if (bytesReceived > 0) 
                {
                    recievedData.push(std::string(buffer, bytesReceived));
                }
            }

            // send data tcp
            if (FD_ISSET(tcpSocket, &writeFds) && popTcp(topOfQueue))
            {
                send(tcpSocket, topOfQueue.c_str(), topOfQueue.size(), 0);
            }

            // send data udp
            if (FD_ISSET(udpSocket, &writeFds))
            {
                sendto(udpSocket, position.c_str(), strlen(position.c_str()), 0, (struct sockaddr*)&si_other, slen);
            }
        }
    }
}
