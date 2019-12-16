/** @file brtirus0805a.h
* @brief  提供机械手臂brtirus0805a的远程控制库，需要搭配nest_dba模组操作
* @author 邓宝安
* @date   2019-12-14
* @version 0.0.1
* @note
* 1、提供两种机械手臂运动轨迹
* 2、控制机械手臂启动和停止
* 3、通过网络设置机械臂关节坐标。
* 4、控制机械臂按照设定的坐标运动并阻塞模式等待其运动结束。
*/

#ifndef CBRTIRUS0805A_H
#define CBRTIRUS0805A_H
#include <string>
#include <vector>
class CSocket;

namespace robot
{
enum TrajectoryModel
{
    TARJ_MODEL_ORIGIN, //!< 运动前先回到原点。
    TARJ_MODEL_LAST //!< 从上次位置开始运行。
};

class CBrtirus0805A
{
public:
    CBrtirus0805A();
    ~CBrtirus0805A();
public:
    bool Connect(std::string addr, uint16_t port);
    void Disconnect();
    bool Start();
    bool Stop();
    bool SetAxisAngle(std::vector<float> axisAngle);
    bool SetTrajectoryModel(TrajectoryModel model);
    bool StartMove(int timeout=-1);

private:
    bool StartButton();
    bool StopButton();
    bool ReadReply(int timeout = 5000);
    bool WaitStop(int timeout = 5000);
    bool ModifyOutput(int d1, int d2, bool enable);
    bool RewriteDataList(std::vector<std::string> axisAngle);
private:
    CSocket *m_pSocket;
};

}
#endif // CBRTIRUS0805A_H
