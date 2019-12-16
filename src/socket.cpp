#include "socket.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
CSocket::CSocket()
{
    m_sockFd = -1;
}

CSocket::~CSocket()
{
    Shutdown();
}

bool CSocket::Connect(std::__cxx11::string addr, uint16_t port)
{
    sockaddr_in  servaddr;
    if( (m_sockFd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);
        m_sockFd = -1;
        return false;
    }
    memset(&servaddr, 0, sizeof(sockaddr_in));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    if( inet_pton(AF_INET, addr.c_str(), &servaddr.sin_addr) <= 0)
    {
        printf("inet_pton error for %s\n", addr.c_str());
        m_sockFd = -1;
        return false;
    }

    if( connect(m_sockFd, (sockaddr*)&servaddr, sizeof(servaddr)) < 0)
    {
        printf("connect error: %s(errno: %d)\n",strerror(errno),errno);
        m_sockFd = -1;
        return false;
    }
    return true;
}

void CSocket::Shutdown()
{
    if(m_sockFd > 0)
    {
        close(m_sockFd);
        m_sockFd = -1;
    }
}

int CSocket::Write(char *pBuf, size_t size, int timeout_ms)
{
    //printf("CSocket::Write: %s \n", pBuf);
    timeval timeout = {timeout_ms/1000, (timeout_ms%1000)*1000};
    setsockopt(m_sockFd,SOL_SOCKET,SO_SNDTIMEO,(const char*)&timeout,sizeof(timeout));

    int ret = send(m_sockFd, pBuf, size, 0);
    if(  ret <= 0)
    {
        printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
    }
    return ret;
}

int CSocket::Read(char *pBuf, size_t size, int timeout_ms)
{
    timeval timeout = {timeout_ms/1000, (timeout_ms%1000)*1000};
    setsockopt(m_sockFd,SOL_SOCKET,SO_RCVTIMEO,(const char*)&timeout, sizeof(timeout));

    int ret = recv(m_sockFd, pBuf, size, 0);
    if(  ret <= 0)
    {
        printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
    }

    return ret;
}
