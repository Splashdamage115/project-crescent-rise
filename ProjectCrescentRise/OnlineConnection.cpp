#include "OnlineConnection.h"
#include "OnlineDispatcher.h"
#include "CubeSphere.h"
#include "Update.h"
#include "CommandInterpreter.h"
#include "Game.h"
#include "ChatBoxText.h"
#include "Globals.h"

bool OnlineConnection::loopActive = true;
int OnlineConnection::playerId = -1;
SOCKET OnlineConnection::tcpSocket;
SOCKET OnlineConnection::udpSocket;
std::queue<std::string> OnlineConnection::tcpQueue;
std::mutex OnlineConnection::tcpMutex;
std::mutex OnlineConnection::playersMutex;
std::queue<std::string> OnlineConnection::recievedData;
std::string OnlineConnection::position;
std::vector<std::pair<int, std::shared_ptr<OnlineMover>>> OnlineConnection::m_OnlinePlayer;
std::vector<std::shared_ptr<GameObject>> OnlineConnection::m_onlinePlayers;
std::atomic<bool> OnlineConnection::m_readyToCreate = false;
std::atomic<int> OnlineConnection::m_createPosition;
std::mutex OnlineConnection::positionMutex;
std::atomic<bool> OnlineConnection::m_creating = false;

bool OnlineConnection::connected = false;
bool OnlineConnection::firstPass = true;
bool OnlineConnection::textOnce = false;

#define RECONNECT_TIME 10.0f

float OnlineConnection::reconnectTimeOut = RECONNECT_TIME;


OnlineConnection::~OnlineConnection()
{
    closesocket(tcpSocket);
    closesocket(udpSocket);
    WSACleanup();
}

void OnlineConnection::callReConnect(std::string _)
{
    std::cout << "TRYING TO RECONNECT TO SERVER\n";

    Start();
}

// start the online connection client
int OnlineConnection::Start()
{
    if (connected) return 1;
    if (firstPass)
    {
        if (ONLINE_SESSION)
            Update::append(tryReconnect);
        CommandInterpreter::append(callReConnect, "/RECONNECT");
        CommandInterpreter::append(callReConnect, "/CONNECT");
        firstPass = false;
    }

    if (!ONLINE_SESSION) return 1;

    int errorResult = SetUpConnections();
    if (errorResult != 0)
    {
        return errorResult;
    }

    initiliseDataFromServer();

    connected = true;
    ChatBoxText::SentNewText("SERVER", ". . . SUCCESFULLY CONNECTED . . .");

    std::thread IOthread(LoopConnection);
    IOthread.detach();

    Update::append(addNewPlayer);

    return 0;
}

void OnlineConnection::tryReconnect()
{
    if (!connected)
    {
        reconnectTimeOut -= Game::deltaTime;
        if (reconnectTimeOut <= 0.0f)
        {
            callReConnect("");

            reconnectTimeOut = RECONNECT_TIME;

            textOnce = false;
        }
        else if (reconnectTimeOut <= 0.5f)
        {
            if (!textOnce)
            {
                ChatBoxText::SentNewText("SERVER", ". . . TRYING TO RECONNECT TO SERVER . . .");
                textOnce = true;
            }
        }
        
    }
}

int OnlineConnection::initiliseDataFromServer()
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

    send(tcpSocket, OnlineDispatcher::playerIdentifier.c_str(), OnlineDispatcher::playerIdentifier.size(), 0);

    bytesReceived = recv(tcpSocket, buffer, BUFFER_SIZE, 0);
    if (bytesReceived > 0)
    {
        std::cout << "Server: " << std::string(buffer, bytesReceived) << std::endl;
    }

    return 0;
}

// set up the tcp & udp connections
int OnlineConnection::SetUpConnections()
{
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0)
    {
        std::cerr << "WSAStartup failed" << std::endl;
        return 1;
    }

    int errorResult = 0;
    errorResult = SetUpTcp();
    if (errorResult != 0)
    {
        std::cout << "ERROR SETTING UP TCP" << std::endl;
        WSACleanup();
        return errorResult;
    }
    errorResult = SetUpUdp();
    if (errorResult != 0)
    {
        std::cout << "ERROR SETTING UP UDP" << std::endl;
        WSACleanup();
        return errorResult;
    }
    return 0;
}

// set up the udp connection
int OnlineConnection::SetUpUdp()
{
    if ((udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
    {
        std::cout << "UDP socket() failed with error code : " << WSAGetLastError() << std::endl;
        return 1;
    }

    std::cout << "UDP socket created successfully.\n";
    return 0;
}

// set up the tcp connection
int OnlineConnection::SetUpTcp()
{
    addrinfo hints = {}, * addrResult = nullptr;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    int result = getaddrinfo(SERVER_IP, DEFAULT_PORT, &hints, &addrResult);
    if (result != 0)
    {
        std::cerr << "getaddrinfo failed" << std::endl;
        return 1;
    }

    tcpSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol);
    if (tcpSocket == INVALID_SOCKET)
    {
        std::cerr << "TCP socket creation failed" << std::endl;
        freeaddrinfo(addrResult);
        return 1;
    }

    result = connect(tcpSocket, addrResult->ai_addr, (int)addrResult->ai_addrlen);
    if (result == SOCKET_ERROR)
    {
        std::cerr << "Unable to connect to server" << std::endl;
        closesocket(tcpSocket);
        freeaddrinfo(addrResult);
        return 1;
    }

    freeaddrinfo(addrResult);
    std::cout << "TCP socket connected successfully.\n";

    return 0;
}

// safely push data, ready to send
void OnlineConnection::pushTcp(std::string& data)
{
    std::lock_guard<std::mutex> lock(tcpMutex);
    std::string sendable;
    sendable += std::to_string(playerId);
    sendable += '~';
    sendable += data;
    tcpQueue.push(sendable);
}

void OnlineConnection::pushPosition(std::string& data)
{
    std::string sendable;
    sendable += std::to_string(playerId);
    sendable += '~';
    sendable += data;
    {
        std::lock_guard<std::mutex> lock(positionMutex);
        position = sendable;
    }
}

void OnlineConnection::pullPosition(std::string positionData)
{
    if (positionData.empty() || m_creating.load() == true) return;
    
    int playerNum = -1;
    std::string placeholder = "";
    glm::vec3 pos = glm::vec3(0.0f);
    bool playerNumB = true;
    int locPos = 0;

    try
    {
        for (auto& l : positionData)
        {
            if (playerNumB && l != '~')
            {
                placeholder += l;
            }
            if (l == '~')
            {
                playerNum = std::stoi(placeholder);
                placeholder.clear();
                playerNumB = false;
                continue;
            }
            if (!playerNumB)
            {
                if (l == ',')
                {
                    if (locPos == 0)
                    {
                        pos.x = std::stof(placeholder);
                    }
                    else if (locPos == 1)
                    {
                        pos.y = std::stof(placeholder);
                    }
                    else if (locPos == 2)
                    {
                        pos.z = std::stof(placeholder);
                    }
                    locPos++;
                    placeholder.clear();
                  
                    if (locPos >= 3)
                    {
                        break;
                    }
                }
                else
                {
                    placeholder += l;
                }
            }
        }

        // Handle the last coordinate if string doesn't end with comma
        if (locPos == 2 && !placeholder.empty())
        {
            pos.z = std::stof(placeholder);
        }
        
        if (playerNum == playerId || playerNum < 0) return;
        
        {
            std::lock_guard<std::mutex> lock(playersMutex);
            bool playerNotFound = true;
            for (const auto& i : m_OnlinePlayer)
            {
                if (i.first == playerNum)
                {
                    i.second->position.store(pos);
                    playerNotFound = false;
                    return;
                }
            }
            if (playerNotFound)
            {
                m_readyToCreate.store(true);
                m_createPosition.store(playerNum);
            }
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error parsing position data: " << e.what() << std::endl;
        return;
    }
}

// safely pop the data, and send it
bool OnlineConnection::popTcp(std::string& data)
{
    std::lock_guard<std::mutex> lock(tcpMutex);
    if (tcpQueue.empty()) return false;
    data = tcpQueue.front();
    tcpQueue.pop();
    return true;
}

void OnlineConnection::addNewPlayer()
{
    if (m_readyToCreate.load() == false || m_creating.load() == true) return;

    m_creating.store(true);
    m_readyToCreate.store(false);
    int id = m_createPosition.load();
    m_createPosition.store(-1);
    
    std::pair<int, std::shared_ptr<OnlineMover>> newPlayerPair;
    newPlayerPair.first = id;
    newPlayerPair.second = std::make_shared<OnlineMover>();
    newPlayerPair.second->position.store(glm::vec3(0.0f));
    std::shared_ptr<GameObject> newPlayer = std::make_shared<GameObject>();
    newPlayer->transform->position = glm::vec3(0.0f);
    newPlayer->transform->rotation = { 0.0f, 0.0f, 0.0f };
    newPlayer->addScript(newPlayerPair.second);
    newPlayer->addScript(std::make_shared<CubeSphere>());

    std::cout << "New Object created\n";

    {
        std::lock_guard<std::mutex> lock(playersMutex);
        GameObjects::addNewObjectToPool(newPlayer);
        m_onlinePlayers.push_back(newPlayer);
        m_OnlinePlayer.push_back(newPlayerPair);
        std::cout << "New Object locked in\n";
    }
    m_creating.store(false);
    std::cout << "new object data loaded\n";
}

void OnlineConnection::LoopConnection()
{
    fd_set readFds, writeFds;

    struct sockaddr_in si_other;
    int slen = sizeof(si_other);
    memset((char*)&si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(UDP_PORT);
    si_other.sin_addr.S_un.S_addr = inet_addr(SERVER_UDP);

    char buffer[BUFFER_SIZE];
    std::string topOfQueue;
    timeval timeout{};
    timeout.tv_sec = 0;
    timeout.tv_usec = 1000;

    // loop the data transfer
    while (connected && loopActive)
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
                    pullPosition(std::string(buffer, bytesReceived));
                }
                else if (bytesReceived == SOCKET_ERROR)
                {
                    std::cout << "DISCONNECTED FROM SERVER\n";
                    connected = false;
                }
            }

            // recieve data tcp
            if (FD_ISSET(tcpSocket, &readFds))
            {
                int bytesReceived = recv(tcpSocket, buffer, BUFFER_SIZE, 0);
                if (bytesReceived > 0)
                {
                    OnlineDispatcher::RecieveDispatch(OnlineDispatcher::DispatchType::ChatText, std::string(buffer, bytesReceived));
                    //recievedData.push(std::string(buffer, bytesReceived));
                }
                else if (bytesReceived == SOCKET_ERROR)
                {
                    std::cout << "DISCONNECTED FROM SERVER\n";
                    connected = false;
                }
            }

            // send data tcp
            if (FD_ISSET(tcpSocket, &writeFds) && popTcp(topOfQueue))
            {
                int result = -1;
                result = send(tcpSocket, topOfQueue.c_str(), topOfQueue.size(), 0);

                if (result == SOCKET_ERROR)
                {
                    std::cout << "DISCONNECTED FROM SERVER\n";
                    connected = false;
                }
            }

            // send data udp
            if (FD_ISSET(udpSocket, &writeFds))
            {
                int result = -1;
                {
                    std::lock_guard<std::mutex> lock(positionMutex);
                    result = sendto(udpSocket, position.c_str(), strlen(position.c_str()), 0, (struct sockaddr*)&si_other, slen);
                }
                if (result == SOCKET_ERROR)
                {
                    std::cout << "DISCONNECTED FROM SERVER\n";
                    connected = false;
                }
            }
        }
    }
}
