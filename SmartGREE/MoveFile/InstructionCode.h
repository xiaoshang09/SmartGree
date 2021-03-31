#ifndef INSTRUCTIONCODE_H
#define INSTRUCTIONCODE_H
#include <QString>

//#pragram pack(1)

namespace InstructionCode {

    enum EM_PLC_STATUS
    {
        Em_MachineRunning,     //机台运行中
        Em_MachineAlarm,       //机台报警中
        Em_MachineStandby,     //机台待机中

        Em_MaterialLack,        //机台缺料中
        Em_AirSupplyAlarm,      //气源供气不足报警
        Em_RobotWarming,        //机器人报警中
        Em_SafetyLigthCurtains, //安全光栅报警
        Em_FixtureServoAlarm,   //夹具伺服报警
        Em_MeterialFalling,     //物料掉落报警
    };
    Q_DECLARE_METATYPE(InstructionCode::EM_PLC_STATUS);

    enum EM_CAMERA_MODE
    {
        EM_TRIGGER_CAMERA_TITO,   //底拖
        EM_TRIGGER_CAMERA_METAL   //钣金
    };
    Q_DECLARE_METATYPE(InstructionCode::EM_CAMERA_MODE);

    //端口号，功能码，数据段，结束符
    const int ID_PLC                        = 0x01;              //PLC端口
    const int ID_PC                         = 0x02;              //上位机端口
    const int ID_ROBOT                      = 0x03;              //机器人端口

    const int GET_PLC_STATUS                = 0x03;              //获取PLC状态
    const int GET_PLC_DATA                  = 0x04;              //获取PLC生产数据
    const int GET_ROBOT_POS                 = 0x05;              //获取机器人当前位置

    const int TRIGGER_CAMERA_TIDO           = 0x20;              //相机拍照，底托
    const int TRIGGER_CAMERA_METAL          = 0x21;              //相机拍照，钣金

    const char STR_END_FLAG[]               = "\\r\\n";            //指令结束符

    //查询指令码  PC-PLC
    const char INST_GET_PLC_STATUS[]        = "0103,\\r\\n";       //获取PLC状态
    const char INST_GET_PLC_DATAS[]         = "0104,\\r\\n";       //获取PLC生产数据
    const char INST_GET_ROBOT_POS[]         = "0105,\\r\\n";       //获取PLC, 机器人状态

    //命令指令码  PLC-PC  req-request
    const char REQ_TRIGGER_CAMERA_TITO[]    = "0220,\\r\\n";       //相机拍照，底托
    const char REQ_TRIGGER_CAMERA_METAL[]   = "0221,\\r\\n";       //相机拍照, 钣金

    const char REQ_CHANGE_PROGRAM[]         = "0222";            //设置程序,协议头  "0222,xxx,\r\n"

    //反馈消息的状态码     res-response
    const char RES_PLC_STATUS[]             = "0103";
    const char RES_PLC_DATAS[]              = "0104";
    const char RES_ROBOT_POS[]              = "0105";

    struct PLCStatus
    {
        unsigned int MachineRunning      : 1;  //机台运行中
        unsigned int MachineAlarm        : 1;  //机台报警中
        unsigned int MachineStandby      : 1;  //机台待机中

        unsigned int MaterialLack        : 1;  //机台缺料中
        unsigned int AirSupplyAlarm      : 1;  //气源供气不足报警
        unsigned int RobotWarming        : 1;  //机器人报警中
        unsigned int SafetyLigthCurtains : 1;  //安全光栅报警
        unsigned int FixtureServoAlarm   : 1;  //夹具伺服报警
        unsigned int MeterialFalling     : 1;  //物料掉落报警

        unsigned int Reserve             : 23; //预留
    };

    union U_PLCStatus{
        unsigned int state;
        PLCStatus STS;
    };

    struct RobotStatus
    {
         RobotStatus() {}

         QString name;
         double X, Y, Z, Rx, Ry, Rz;
    };
}
#endif // INSTRUCTIONCODE_H
