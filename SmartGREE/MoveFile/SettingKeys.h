#ifndef SETTINGKEYS_H
#define SETTINGKEYS_H


//配置文件 //工程配置是单独的
#define D_FILE_WORKSPACE  "./worksp.ini"
#define D_FILE_SETTINGS   "settings.ini"
#define D_FILE_CODE       "utf-8"

///比较字符串
#define DK_COMP(a,b) (strcmp(a,b)==0)

///其他目录全部依赖DK_ProjectDir
//Project
#define DK_ProjectDir     "Project/Dir"
#define DK_ProjectName    "Project/Name"
#define DK_ProjectStart   "Project/OpenAtStart"

//PLC
#define DK_PlcIp          "PLC/IP"
#define DK_PlcPort        "PLC/Port"
#define DK_PlcAutoConet   "PLC/AutoConnect"
#define DK_PlcState       "PLC/State"

//状态
#define DK_StateSTS        "State/STS"
#define DK_StateCamera     "State/Camera"

//camera
#define DK_CameraDir      "Camera/Dir"
#define DK_CameraImgDir   "Camera/ImgDir"
#define DK_CameraImgType  "Camera/ImgType"

//log
#define DK_LogDir           "Log/Dir"
#define DK_LogFType         "Log/FileType"
#define DK_LogDateType      "Log/DateType"
#define DK_LogAutoCreatDir  "Log/AutoCreatDir"

//system
#define DK_SysLang         "Sys/Lang"
#define DK_SysName         "Sys/name"
#define DK_SysStart        "Sys/start"

//保养//更换//操作记录是个表,用sql保存吧 //直接生成下次保养时间?
#define DK_MaintenanceInv       "Maintenance/Inv"
#define DK_MaintenanceTimeLast  "Maintenance/TimeLast"
#define DK_MaintenanceTimeNext  "Maintenance/TimeNext"

#define DK_ReplacementInv       "Replacement/Inv"
#define DK_ReplacementTimeLast  "Replacement/TimeLast"
#define DK_ReplacementTimeNext  "Replacement/TimeNext"





//robot


//TODO 更多配置



#endif // SETTINGKEYS_H
