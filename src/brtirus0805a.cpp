#include "brtirus0805a.h"
#include "socket.h"
#include <nlohmann/json.hpp>
#include <iostream>
#include <unistd.h>

using json = nlohmann::json;
using namespace  robot;

CBrtirus0805A::CBrtirus0805A()
{
    m_pSocket = nullptr;
    m_pSocket = new CSocket();
}

CBrtirus0805A::~CBrtirus0805A()
{
    Stop();
    if(m_pSocket != nullptr)
    {
        delete m_pSocket;
        m_pSocket = nullptr;
    }
}
/** @brief       socket链接。
  * @param[in]   addr 机械臂ip
  * @param[in]   port 机械臂远程端口
  * @return      true|false
  */
bool CBrtirus0805A::Connect(std::string addr, uint16_t port)
{
    return m_pSocket->Connect(addr, port);
}
/** @brief       断开socket链接。
  * @warning     断开链接前一定要确保机械臂已经停止运行。
  */
void CBrtirus0805A::Disconnect()
{
    m_pSocket->Shutdown();
}

/** @brief       开始运行机械臂程序。
  * @return      true|false
  * @note        此函数只是让机械臂进入等待命令状态，并不是立即动作。
  */
bool CBrtirus0805A::Start()
{
    if(!StartButton())
    {
        return false;
    }
    return ReadReply();
}

/** @brief       停止机械手臂。
  * @return      true|false
  * @note        运行此函数机械手臂停止动作，可能会有一定的延迟。
  */
bool CBrtirus0805A::Stop()
{
    /*! singe */
    if( (!StopButton()) ||(!ReadReply()) )
    {
        return false;
    }

    /*! stop */
    if((!StopButton()) ||(!ReadReply()))
    {
        return false;
    }
    return true;
}

/** @brief       停止机械手臂。
  * @param[in]   std::vector<float> axisAngle 数组长度下标0-6对应机械手臂j1-j6，可以缺省，Angle范围根据每个轴有所不同。
  * @return      true|false
  */
bool robot::CBrtirus0805A::SetAxisAngle(std::vector<float> axisAngle)
{
    std::vector<std::string> dataList;
    std::cout<<__FILE__<<__LINE__<<"axisAngle: ";
    for (float it : axisAngle)
    {
        std::string strAngle = std::to_string((int)it*1000);
        std::cout<<strAngle<<"  ";
        dataList.push_back(strAngle);
    }
    std::cout<<std::endl;

    if( (!RewriteDataList(dataList)) && (!ReadReply()) )
    {
        return false;
    }
    return true;
}

bool CBrtirus0805A::ReadReply(int timeout)
{
    char buf[256] = {0};
    int size = m_pSocket->Read(buf, 256, timeout);
    //std::cout<<__FILE__<<__LINE__<<" "<<size << std::endl;
    if( 0 >= size)
    {
        std::cout<<__FILE__<<__LINE__<<"# error # ";
        return false;
    }

    json j = json :: parse(buf);
    //std::cout<<__FILE__<<__LINE__<<j<< std::endl;

    std::string cmdReply = j["cmdReply"].dump();

    if( cmdReply.find("ok") == std::string::npos)
    {
        std::cout<<__FILE__<<__LINE__
                <<"# error # "
               << cmdReply<<std::endl;
        return false;
    }

    return true;
}

bool CBrtirus0805A::WaitStop(int timeout)
{
    if(timeout <=0 )
    {
        return true;
    }

    char buf[256] = {0};
    int size = m_pSocket->Read(buf, 256, timeout);
    //std::cout<<__FILE__<<__LINE__<<" "<<size << std::endl;
    if( 0 >= size)
    {
        std::cout<<__FILE__<<__LINE__<<" "<<size << std::endl;
        return false;
    }

    json j = json :: parse(buf);
    //std::cout<<__FILE__<<__LINE__<<j<< std::endl;

    std::string reqType = j["reqType"].dump();
    std::string verifyType = R"("photo")";
    if( reqType.compare(verifyType) != 0)
    {
        std::cout<<__FILE__<<__LINE__
                <<"# error # "
               << reqType<<" "<<verifyType<<std::endl;
        return false;
    }

    return true;
}

/** @brief       运行此函数机械臂立即开始动作
  * @param[in]   timeout 此函数会阻塞等待机械臂动作完成，指导机械臂停止或超时时间到， 当timeout=-1时，立即返回。
  * @return      true|false
  */
bool CBrtirus0805A::StartMove(int timeout)
{
    if( (!ModifyOutput(4, 1, false)) ||(!ReadReply()) )
    {
        return false;
    }
    usleep(50000);

    if( (!ModifyOutput(4, 1, true)) || (!ReadReply()) )
    {
        return false;
    }
    usleep(50000);
    if( (!ModifyOutput(4, 1, false)) ||(!ReadReply()) )
    {
        return false;
    }

    if( !WaitStop(timeout))
    {
       return false;
    }
    return true;
}

bool CBrtirus0805A::StartButton()
{
    json j;
    j["dsID"] = "HCRemoteMonitor";
    j["cmdType"] = "command";
    j["cmdData"] ={"startButton"};
    std :: string s = j.dump();

    if( 0 >= m_pSocket->Write((char*)s.c_str(), s.size()))
    {
        return false;
    }
    return true;
}

bool CBrtirus0805A::StopButton()
{
    json j;
    j["dsID"] = "HCRemoteMonitor";
    j["cmdType"] = "command";
    j["cmdData"] ={"stopButton"};
    std :: string s = j.dump();

    if( 0 >= m_pSocket->Write((char*)s.c_str(), s.size()))
    {
        return false;
    }
    return true;
}
/** @brief       设计机械手臂轨迹模式
  * @param[in]   robot::TrajectoryModel 运动轨迹
  * @return      true|false
  */
bool CBrtirus0805A::SetTrajectoryModel(TrajectoryModel model)
{
    switch (model)
    {
    case TARJ_MODEL_ORIGIN:
    {
        if( (!ModifyOutput(4, 0, false)) &&(!ReadReply()) )
        {
            return false;
        }
    }break;
    case TARJ_MODEL_LAST:
    {
        if( (!ModifyOutput(4, 0, true)) &&(!ReadReply()) )
        {
            return false;
        }
    }break;
    default:
        return false;
    }
    return ReadReply();
}

/** @brief       设置机械臂修改输出点状态。
  * @param[in]   d1 输出板 ID（0～3： IO 板， 4～6： M 值， 7： EUIO）
  * @param[in]   输出点 ID
  * @param[in]   enable 输出状态（0： OFF， 1： ON）
  * @return      true|false
  */
bool CBrtirus0805A::ModifyOutput(int d1, int d2, bool enable)
{
    json j;
    j["dsID"] = "HCRemoteMonitor";
    j["cmdData"] = {"modifyOutput",std::to_string(d1),std::to_string(d2), enable ? "1" : "0"};
            j["cmdType"] = "command";
    std :: string s = j.dump();
    if( 0 >= m_pSocket->Write((char*)s.c_str(), s.size()))
    {
        return false;
    }

    return true;
}

bool CBrtirus0805A::RewriteDataList(std::vector<std::string> axisAngle)
{
    json dataList(axisAngle);
    dataList.insert(dataList.begin(), {"rewriteDataList", "800", std::to_string(axisAngle.size()), "0"});

    json j;
    j["dsID"] = "HCRemoteMonitor";
    j["cmdType"] = "command";
    j["cmdData"] =dataList;

    std::string s = j.dump();
    if( 0 <= m_pSocket->Write((char*)s.c_str(), s.size()))
    {
        return false;
    }
    return true;
}
