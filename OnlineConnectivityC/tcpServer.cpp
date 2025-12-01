#include "TcpServer.h"
#include <string>


std::vector<SOCKET> TcpServer::m_sockets;
SOCKET TcpServer::m_listenSocket;
std::vector<std::thread> TcpServer::m_threads;
SOCKET TcpServer::m_udpSocket;
std::mutex TcpServer::m_udpMutex;
bool TcpServer::debug = false;
std::vector<std::string> TcpServer::m_playerId;
std::vector<bool> TcpServer::m_activeSocket;

int TcpServer::setUpServer() 
{
    int success = setUpListenSocket();
    if (success != 0) return success;

    std::cout << "TCP Echo Server listening on all interfaces (0.0.0.0) port " << DEFAULT_PORT << "..." << std::endl;
    std::cout << "Server is ready to accept external connections." << std::endl;

    m_sockets.emplace_back();
    m_activeSocket.emplace_back(true);
    std::thread listenThread(listenForConnections);
    std::thread udpThread(listenForUDP);
    udpThread.detach();
    listenThread.join();
    
    closesocket(m_listenSocket);
    WSACleanup();
    return 0;
}

int TcpServer::setUpListenSocket()
{
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (result != 0) 
    {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return 1;
    }

    addrinfo hints = {}, * addrResult = nullptr;
    hints.ai_family = AF_INET;        // IPv4
    hints.ai_socktype = SOCK_STREAM;  // TCP
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    result = getaddrinfo("0.0.0.0", DEFAULT_PORT, &hints, &addrResult);
    if (result != 0) 
    {
        std::cerr << "getaddrinfo failed: " << result << std::endl;
        WSACleanup();
        return 1;
    }

    m_listenSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol);
    if (m_listenSocket == INVALID_SOCKET) 
    {
        std::cerr << "Socket creation failed" << std::endl;
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }

    result = bind(m_listenSocket, addrResult->ai_addr, (int)addrResult->ai_addrlen);
    if (result == SOCKET_ERROR) 
    {
        std::cerr << "Bind failed for address 0.0.0.0:" << DEFAULT_PORT << std::endl;
        closesocket(m_listenSocket);
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(addrResult);

    result = listen(m_listenSocket, SOMAXCONN);
    if (result == SOCKET_ERROR) 
    {
        std::cerr << "Listen failed" << std::endl;
        closesocket(m_listenSocket);
        WSACleanup();
        return 1;
    }

    setsockopt(m_listenSocket, SOL_SOCKET, SO_RCVTIMEO, "5000", sizeof("5000"));

    return 0;
}

void TcpServer::initialiseSocket(int t_pos)
{
    char buffer[BUFFER_SIZE];
    int bytesReceived;

    std::string clientID; clientID += std::to_string(t_pos);
    send(m_sockets.at(t_pos), clientID.c_str(), clientID.size(), 0);

    bytesReceived = recv(m_sockets.at(t_pos), buffer, BUFFER_SIZE, 0);
    if (bytesReceived > 0)
    {
        m_playerId.emplace_back(std::string(buffer, bytesReceived));
    }

    std::string helloMessage = "Hello client #";
    helloMessage += std::to_string(t_pos);
    helloMessage += " Player id: ";
    helloMessage += std::string(buffer, bytesReceived);
    helloMessage += " ~ Connection Established";
    send(m_sockets.at(t_pos), helloMessage.c_str(), helloMessage.size(), 0);

    m_activeSocket.at(t_pos) = true;
}

void TcpServer::listenForConnections()
{
    int pos = 0;
    bool reused = false;
    while (true) 
    {
        reused = false;
        for (int i = 0; i < m_sockets.size(); i++)
        {
            if (!m_activeSocket.at(i))
            {
                pos = i;
                reused = true;
                break;
            }
        }
        m_sockets.at(pos) = accept(m_listenSocket, nullptr, nullptr);

        if (m_sockets.at(m_sockets.size() - 1) == INVALID_SOCKET)
        {
            std::cerr << "Accept failed" << std::endl;
        }
        std::cout << "Client connected!" << std::endl;

        initialiseSocket(pos);

        if(!reused)
        {
            m_threads.push_back(std::thread(listenOnSocket, pos));
            m_threads.at(m_threads.size() - 1).detach();

            m_sockets.emplace_back();
        }
        else
        {
            bool joined = false;
            for (int i = 0; i < m_threads.size();i++)
            {
                if (m_threads.at(i).joinable())
                {
                    m_threads.at(i) = std::thread(listenOnSocket, pos);
                    m_threads.at(i).detach();
                    m_sockets.emplace_back();
                    joined = true;
                    break;
                }
            }
            if (!joined)
            {
                m_threads.push_back(std::thread(listenOnSocket, pos));
                m_threads.at(m_threads.size() - 1).detach();

            }
            
        }
        pos = m_sockets.size() - 1;
        if (!reused)
            m_activeSocket.push_back(false);
    }
}

void TcpServer::listenOnSocket(int socketNum)
{
    char buffer[BUFFER_SIZE];
    int bytesReceived;
    int pos = socketNum;

    do
    {
        bytesReceived = recv(m_sockets.at(pos), buffer, BUFFER_SIZE, 0);

        if (bytesReceived > 0)
        {
            if(debug)
                std::cout << "Received: " << std::string(buffer, bytesReceived) << std::endl;
            // add interpretation here for commands and such
            std::string spitBack = recieveData(std::string(buffer, bytesReceived));

            if (spitBack.size() <= 0) continue;

            for (int i = 0; i < m_sockets.size(); i++)
            {
                int result = send(m_sockets.at(i), spitBack.c_str(), spitBack.size(), 0); // Echo back

                if (result == SOCKET_ERROR)
                {
                    m_activeSocket.at(i) = false;
                }
            }
        }
        else if (bytesReceived == SOCKET_ERROR)
        {
            m_activeSocket.at(pos) = false;
        }
    } while (bytesReceived > 0);

    std::cout << "Client disconnected." << std::endl;
    closesocket(m_sockets.at(pos));
    m_activeSocket.at(pos) = false;
}

std::string TcpServer::recieveData(std::string t_data)
{
    int senderNum = -1;
    std::string itemSent = "";
    std::string accumulate = "";
    bool reached = false;
    for (auto& l : t_data)
    {
        if (l != '~' && !reached)
        {
            accumulate += l;
        }
        if (l == '~' && !reached)
        {
            reached = true;
            continue;
        }
        if (reached)
        {
            itemSent += l;
        }
    }
    senderNum = std::stoi(accumulate);
    if (itemSent.at(0) == '/')
    {
        // handle commands
        bool arg1 = false;
        std::string command = "";
        std::string modifier = "";
        for (auto& l : itemSent)
        {
            if (l != ' ')
            {
                if (arg1)
                {
                    modifier += l;
                }
                else
                {
                    command += l;
                }
            }
            else
            {
                if (arg1) break;

                arg1 = true;
            }
        }

        if (command == "/CHANGENAME" || command == "/changename")
        {
            m_playerId.at(senderNum) = modifier;
        }
        else if (command == "/planetMod")
        {
            std::string output;
            output += m_playerId.at(senderNum);
            output += "~";
            output += itemSent;
            return output;
        }
        else if (command == "/DEBUG" || command == "/debug")
        {
            debug = !debug;
        }
        return std::string();
    }
    else
    {
        std::string output;
        output += m_playerId.at(senderNum);
        output += "~";
        output += itemSent;
        return output;
    }
}

int TcpServer::listenForUDP()
{
    struct sockaddr_in server, si_other;
    int slen, recv_len;
    char buf[BUFLEN];

    std::vector< sockaddr_in> ports;

    slen = sizeof(si_other);

    printf("\nUDP Server starting...\n");

    //Create a socket
    if ((m_udpSocket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d", WSAGetLastError());
        return 1;
    }
    printf("Socket created.\n");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    //Bind
    if (bind(m_udpSocket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
    {
        printf("Bind failed with error code : %d", WSAGetLastError());
        return 1;
    }
    puts("Bind done");

    fflush(stdout);

    memset(buf, '\0', BUFLEN);

    //try to receive some data, this is a blocking call
    //if ((recv_len = recvfrom(m_udpSocket, buf, BUFLEN, 0, (struct sockaddr*)&si_other, &slen)) == SOCKET_ERROR)
    //{
    //    printf("recvfrom() failed with error code : %d", WSAGetLastError());
    //    //return 1;
    //}

    while (1)
    {
        if (debug)
        {
            std::cout << "Waiting for data..." << std::endl;
        }

        // clear buffer data
        memset(buf, '\0', BUFLEN);

        //try to receive some data, this is a blocking call
        if ((recv_len = recvfrom(m_udpSocket, buf, BUFLEN, 0, (struct sockaddr*)&si_other, &slen)) == SOCKET_ERROR)
        {
            int error = WSAGetLastError();
            if (error == WSAECONNRESET)
            {
                if (debug) printf("Client disconnected (connection reset)\n");
                continue;
            }
            printf("recvfrom() failed with error code : %d", error);
            continue;
        }

        // Only process if we actually received data
        if (recv_len > 0)
        {
            // Lock mutex to protect ports vector from race conditions
            std::lock_guard<std::mutex> lock(m_udpMutex);
            
            // add new port if not in sendable ports yet
            bool foundPort = false;
            for(int i = 0; i < ports.size(); i++)
            { 
                if (ports.at(i).sin_port == si_other.sin_port && ports.at(i).sin_addr.S_un.S_addr == si_other.sin_addr.S_un.S_addr)
                {
                    foundPort = true;
                    break;
                }
            }
            if(!foundPort)
                ports.emplace_back(si_other);

            //print details of the client/peer and the data received
            if (debug)
            {
                printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
                printf("Data: %s\n", buf);
            }

            // send back to all ports, removing failed ones
            for(auto it = ports.begin(); it != ports.end();)
            {
                if (sendto(m_udpSocket, buf, recv_len, 0, (struct sockaddr*)&(*it), slen) == SOCKET_ERROR)
                {
                    int error = WSAGetLastError();
                    if (error == WSAECONNRESET || error == WSAEFAULT)
                    {
                        if (debug) printf("Removing disconnected client port %d\n", ntohs(it->sin_port));
                        it = ports.erase(it);
                    }
                    else
                    {
                        std::cerr << "sendto() failed with error code : " << error << " Port: " << ntohs(it->sin_port) << std::endl;
                        ++it;
                    }
                }
                else
                {
                    ++it;
                }
            }
        }
    }

    closesocket(m_udpSocket);
    WSACleanup();

    return 0;
}
