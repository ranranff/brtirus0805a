#include <iostream>
#include <unistd.h>
#include "brtirus0805a.h"
using namespace std;

int main()
{
    std::cout << "dba test start!" << endl;
    robot::CBrtirus0805A brtirus;
    brtirus.Connect("192.168.4.4", 9760);
    std::vector<float> temp0 = {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f};
    std::vector<float> temp1 = {30.0f,0.0f,0.0f,0.0f,0.0f,0.0f};
    std::vector<float> temp2 = {0.0f,30.0f,0.0f,0.0f,0.0f,0.0f};
    std::vector<float> temp3 = {0.0f,0.0f,30.0f,0.0f,0.0f,0.0f};
    std::vector<float> temp4 = {0.0f,0.0f,0.0f,30.0f,0.0f,0.0f};
    std::vector<float> temp5 = {0.0f,0.0f,0.0f,0.0f,30.0f,0.0f};
    std::vector<float> temp6 = {0.0f,0.0f,0.0f,0.0f,0.0f,30.0f};

    std::vector<float> test1 = {30.0f,30.0f,30.0f,30.0f,30.0f,30.0f};
    std::vector<float> test2 = {-30.0f,-30.0f,-30.0f,-30.0f,-30.0f,-30.0f};

    brtirus.Start();
    //! model 1
    brtirus.SetTrajectoryModel(robot::TARJ_MODEL_ORIGIN);
    brtirus.SetAxisAngle(temp1);
    brtirus.StartMove(5000);
    brtirus.SetAxisAngle(temp2);
    brtirus.StartMove(5000);
    brtirus.SetAxisAngle(temp3);
    brtirus.StartMove(5000);
    brtirus.SetAxisAngle(temp4);
    brtirus.StartMove(5000);
    brtirus.SetAxisAngle(temp5);
    brtirus.StartMove(5000);
    brtirus.SetAxisAngle(temp6);
    brtirus.StartMove(5000);
    brtirus.SetAxisAngle(test1);
    brtirus.StartMove(5000);
    brtirus.SetAxisAngle(test2);
    brtirus.StartMove(5000);
    brtirus.SetAxisAngle(temp0);
    brtirus.StartMove(5000);

    //! model 2
    brtirus.SetTrajectoryModel(robot::TARJ_MODEL_LAST);
    brtirus.SetAxisAngle(temp1);
    brtirus.StartMove(5000);
    brtirus.SetAxisAngle(temp2);
    brtirus.StartMove(5000);
    brtirus.SetAxisAngle(temp3);
    brtirus.StartMove(5000);
    brtirus.SetAxisAngle(temp4);
    brtirus.StartMove(5000);
    brtirus.SetAxisAngle(temp5);
    brtirus.StartMove(5000);
    brtirus.SetAxisAngle(temp6);
    brtirus.StartMove(5000);
    brtirus.SetAxisAngle(test1);
    brtirus.StartMove(5000);
    brtirus.SetAxisAngle(test2);
    brtirus.StartMove(5000);
    brtirus.SetAxisAngle(temp0);
    brtirus.StartMove(5000);

    brtirus.Stop();

    return 0;
}
