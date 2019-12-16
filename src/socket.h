#pragma once
#include <string>
#include<sys/socket.h>

class CSocket
{
public:
    CSocket();
    ~CSocket();

public:
    bool Connect(std::string addr, uint16_t port);
    void Shutdown();
    int  Write(char *pBuf, size_t size, int timeout_ms = 2000);
    int  Read(char *pBuf,  size_t size, int timeout_ms = 2000);
private:
    int   m_sockFd;
};
