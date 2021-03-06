/*********************************************************************
 * 日志类，分类记录日志
 * **********************************************************************/
#ifndef _LOGSYS_CLIENT_H_
#define _LOGSYS_CLIENT_H_

#include <stdint.h>
#include <stdarg.h>

#include "logsys_log.h"

using namespace ULS;

namespace ULS{


#define LC_VERSION 			1
#define LC_VERSION_2 		2
#define LC_VERSION_3 		3
#define LC_VERSION_4        4

#define LOGSYS_SVR_WRITE_LOG_CMD 0x1
#define LOGSYS_SVR_BATCH_WRITE_LOG_CMD 0x10
#define LOGSYS_MAX_STR_LEN 8192

/* 日志级别定义*/
#define _LC_ERROR_ 		1	//严重错误
#define _LC_WARNING_ 	2 	//警告,比较严重，需要发到手机
#define _LC_NOTICE_ 		3 	//需要注意的问题，可能是非法的包之类，不需要发送到手机，因为可能很多
#define _LC_INFO_ 			4	//流水
#define _LC_DEBUG_ 		5	//debug


#define LOGSYS_MAX_LOGD_IF_SVR			300		//logsys_if服务器的最大个数
#define LOGSYS_LOGD_IF_IP_LEN			30           //logsys_if服务器的IP最大长度

#define LOGSYS_SENDTO_WATER			1		//发送流水日志
#define LOGSYS_SENDTO_COLOR			2	        //发送染色日志

#define LS_INVALID_DEST_ID 0 //非法的日志接口机role
#define LS_WATER_DEST_ID				1		//通用流水日志接口机在配置中心中的role
#define LS_COLOR_DEST_ID				2		//通用染色日志接口机在配置中心中的role

#define	_LC_ERR_RET_ID_		1  //修改LsPkgParam的dwErrId
#define	_LC_USER_DEF1_		2  //修改LsPkgParam的dwUserDef1
#define	_LC_USER_DEF2_		3  //修改LsPkgParam的dwUserDef2
#define	_LC_USER_DEF3_		4  //修改LsPkgParam的dwUserDef3
#define	_LC_USER_DEF4_		5  //修改LsPkgParam的dwUserDef4

#define LOGSYS_MAX_CMD_STR_LEN 64

#define LOGSYS_STATICS(fmt, args...) do { \
    LsStaticsLog(fmt, ## args); }  while (0)

#define SET_LOGSYS_ERRMSG(szErrMsg) do { \
    LsSetRetErrMsg(szErrMsg); }  while (0)

#define CLEAR_LOGSYS_ERRMSG() do { \
    LsClearRetErrMsg(); }  while (0)

/*
   要使用以下协议打印相关的宏，必须先在包含本头文件之前包含协议对应的display头文件，如"display_XXX.h"
   */
#ifdef _CPPHEAD_PROTO_DEFINE_H
#define LOGSYS_STRUCT(iLogLevel, _struct, prompt) do{\
    if (iLogLevel <= g_stLsClient.m_cWaterLogLocalLevel){\
        if((g_stLsClient.m_stWaterLog.pLogFile = fopen(g_stLsClient.m_stWaterLog.sLogFileName, "a+")) == NULL) break;\
        proto_display(g_stLsClient.m_stWaterLog.pLogFile, _struct, prompt);\
        fclose(g_stLsClient.m_stWaterLog.pLogFile);\
    }\
} while(0)
#define LOGSYS_UNION(iLogLevel, _union, prompt, id, len) do{\
    if (iLogLevel <= g_stLsClient.m_cWaterLogLocalLevel){\
        if((g_stLsClient.m_stWaterLog.pLogFile = fopen(g_stLsClient.m_stWaterLog.sLogFileName, "a+")) == NULL) break;\
        proto_display_union(g_stLsClient.m_stWaterLog.pLogFile, _union, (prompt), (id), (len));\
        fclose(g_stLsClient.m_stWaterLog.pLogFile);\
    }\
} while(0)
#else
#define LOGSYS_STRUCT(iLogLevel, _struct, prompt)
#define LOGSYS_UNION(iLogLevel, _union, prompt, id, len)
#endif

#define LOGSYS_APPID  109
#define LOGSYS_CHANNEL_API_KEY 0x10910931
#define LOGSYS_CHANNEL_API_SIZE 100 * 1024 * 1024

#define LOGSYS_CLIENT_ANTI_SNOW_SHM_KEY     0x41300
#define LOGSYS_CLIENT_COMMON_SHM_KEY     0x40400
#define LOGSYS_CLIENT_COMMON_EX_SHM_KEY	 0x40401

#define LOCAL_LOG_ID 10000
#pragma pack(1)
typedef struct 
{
    uint16_t  wLength;
    uint16_t  wVersion; 
    uint16_t  wCommand;
    uint32_t  dwCmdSequence;
    char  acReserved[4];
}Ls_PkgHead;  

typedef struct
{
	int32_t iLogDst;
	uint16_t wBuffLen;
	char sSendBuff[1024 * 10];
}Ls_ChannelPkg;
#pragma pack()


typedef struct  
{
    uint32_t dwAppId; 			//业务的AppId,由系统管理员分配,必填字段
    uint32_t dwUserDef1;       //初始化用户自定义字段1，由调用者自己填写，自己解释
    uint32_t dwProcId;
	uint16_t wMsgPort;			//可指定发送远程日志所绑定的端口，为0则随机端口
	uint16_t wRelayPort;		//可指定转发包所绑定的端口，为0则随机端口
	uint16_t cAgentLocalLog;    //是否由Agent来负责记录本地log，业务进程不再记录
}LsInitParam;   //进程初始化时可确定的信息

typedef struct  
{
    uint64_t ddwUin;   //64位uin，必填字段,同conn的qwUin
    uint32_t dwClientIp; //触发前端服务的用户IP，一般是用户所用机器的IP,网络序，同conn的dwClientIP
    uint8_t  sClientIpV6[16];  //触发前端服务的用户IP，一般是用户所用机器的IP,ipv6 格式
    uint32_t dwServiceIp;   //前端应用主机IP,conn ip认为是前端应用主机IP,网络序 ，同conn的dwServiceIp
    uint32_t dwClientVersion;  //客户端版本号,同conn的wVersion
    uint32_t dwCmd;      //命令号，同conn的wCmd
    uint32_t dwSubCmd;   //子命令号，同conn的dwSubCmd
    int32_t dwErrId;   //处理过程中的错误返回码，同conn的wErrNo，每次调用记录日志的 api时都要传入该参数  
    uint32_t dwUserDef1;  //用户自定义字段1，由调用者自己填写，自己解释，conn的cPkgType，dwLoginTime，dwLogOutTime可使用自定义字段记录并索引
    uint32_t dwUserDef2;  //用户自定义字段2，由调用者自己填写，自己解释
    uint32_t dwUserDef3;  //用户自定义字段3，由调用者自己填写，自己解释
    uint32_t dwUserDef4;  //用户自定义字段4，由调用者自己填写，自己解释  
    //char szCmdStr[LOGSYS_MAX_CMD_STR_LEN];
}LsPkgParam;   //一次请求包处理开始时才能确定的信息

typedef struct
{
	char * szLogFilePath;  /*本地日志文件名，长度不能超过55*/
	int iLogSize;
	int iLogNum;
	uint32_t dwMaxBufferBytes; /*最大能缓存的本地字节数*/
	uint32_t dwMaxBufferTime;  /*最大能缓存的时间*/
}LsLogFileInfo;

typedef struct
{
    /* 错误信息, 保留最后一次调用的错误信息，可以用于详细的返回*/	
    char m_szErrMsg[512];
    LS_LogFile  m_stWaterLog;

    /* 流水日志服务器端配置信息*/
    int m_iSvrConfiged;     //0:不启用远程记录流水日志, 非0:启用远程记录流水日志

    /* 染色日志服务器端配置信息*/
    int m_iColorSvrConfiged;   //0:不启用远程记录染色日志, 非0:启用远程记录染色日志

    /* 日志基本信息 */ 
    LsInitParam m_stLsInitParam;
    uint64_t m_ddwRandUin;

    /* 本地流水日志的日志记录级别，小于等于该级别的都记录到本地*/
    int8_t m_cWaterLogLocalLevel;
    /* 流水日志中心的日志记录级别，小于等于该级别的都发送到流水日志中心, 一般设置为_LOGSYS_WARNING_*/
    int8_t m_cWaterLogMsgLevel;

    /*统计日志*/
    LS_LogFile  m_stStaticsLog;

    /* debug 日志*/
    LS_LogFile  m_stTraceLog;
    /* 本地染色日志的日志记录级别，小于等于该级别的都记录到本地*/
    int8_t  m_cTraceLogLocalLevel;
    /* 染色日志中心的日志记录级别，小于等于该级别的都发送到染色日志中心, 一般设置为_LOGSYS_WARNING_*/
    int8_t m_cTraceLogMsgLevel;

    /*根据染色配置转发包的日志级别*/
    int8_t m_cColorLogRelayLevel;

    int m_iSocket; 		 //用进行数据包转发时所使用的套接字

    int m_iWaterDestID;  //流水日志接口机的ID,一般设置为LS_WATER_DEST_ID
    int m_iColorDestID;  //染色日志接口机的ID,一般设置为LS_COLOR_DEST_ID

	int m_bInit;		//api是否初始化标志
}LsClient;


/*
   使用API时请注意:
   一般LsCheckWriteWL()和LsWaterLog()不单独使用,为了使用方便，定义了LOGSYS_WATER宏,记录日志时请直接使用LOGSYS_WATER()宏
   */


/*
   功能说明:
   进程初始化时调用LsClientInit(),完成初始化该函数只能调用一次，因为其
   内部会attach到染色配置共享内存上

   参数说明:
iSvrConfiged:是否启用远程记录流水日志,0:不启用, 1:启用
iColorSvrConfiged:是否启用远程记录染色日志,0:不启用, 1:启用
pstLsInitParam:指向LsInitParam结构体,用于传入进程初始化是可以确定的信息:本机Appid,初始化用户自定义字段1,进程ID,不能传入空指针,否则函数调用出错
szLogFilePath:本地磁盘log文件的绝对路径,本地的流水日志和染色日志都记录在该路径下,不能传入空指针,否则函数调用出错
cWLogLevel:本地流水日志的日志记录级别(取值范围0-5,请参考"日志级别定义")，小于等于该级别的都记录到本地
cWLogMsgLevel:流水日志中心的日志记录级别(取值范围0-5,请参考"日志级别定义")，小于等于该级别的都发送到流水日志中心, 一般设置为_LC_WARNING_
cTLogLevel:本地染色日志的日志记录级别(取值范围0-5,请参考"日志级别定义")，小于等于该级别的都记录到本地
cTLogMsgLevel:染色日志中心的日志记录级别(取值范围0-5,请参考"日志级别定义")，小于等于该级别的都发送到染色日志中心, 一般设置为_LC_WARNING_
cRelayLogLevel:转发包的记录级别(取值范围0-5,请参考"日志级别定义"),小于等于该级别并且染色配置命中,会转发到指定的ip:port
iLogSize:本地磁盘log文件单个文件的大小
iLogNum:本地磁盘log文件的最大个数
iWaterDestID: 流水日志需要发送到的日志中心ID（流水日志中心可能有多套,每套一个ID）
iColorDestID: 染色日志需要发送到的日志中心ID（染色日志中心可能有多套,每套一个ID）
#define LS_WATER_DEST_ID	1		//流水日志中心ID，1为公用系统ID，如果业务需要单独搭建一套日志中心存储流水信息，那么可以申请分配新ID
#define LS_COLOR_DEST_ID	2		//染色日志中心ID， 2为公用系统ID，如果如果业务需要单独搭建一套存储染色信息，那么可以申请分配新ID


函数返回值:
-30:染色配置共享内存attach失败,遇到该错误返回值,进程可以不退出,但无法使用染色配置功能。可能是没有安装染色配置客户端导致
-40:日志中心接口机(logsy_if)配置共享内存attach失败,遇到该错误返回值,进程可以不退出,但是不能将日志通过网络发到日志中心。可能是没有安装染色配置客户端导致
<0 且不等于-30和-40:失败,逃ν顺�
=0:成功
*/
int LsClientInit(int iSvrConfiged, int iColorSvrConfiged,  
        LsInitParam *pstLsInitParam , char * szLogFilePath, 
        int8_t cWLogLevel, int8_t cWLogMsgLevel,  int8_t cTLogLevel, int8_t cTLogMsgLevel,  int8_t cRelayLogLevel, 
        int iLogSize, int iLogNum, int iWaterDestID, int iColorDestID);





/*
   功能说明:
   在进程运行过程中可通过发送信号调用LsReloadLogCfg()，更新某些统一日志中心相关的配置

   参数说明:
iSvrConfiged:是否启用远程记录流水日志,0:不启用, 1:启用
iColorSvrConfiged:是否启用远程记录染色日志,0:不启用, 1:启用
cWLogLevel:本地流水日志的日志记录级别(取值范围0-5,请参考"日志级别定义")，小于等于该级别的都记录到本地
cWLogMsgLevel:流水日志中心的日志记录级别(取值范围0-5,请参考"日志级别定义")，小于等于该级别的都发送到流水日志中心, 一般设置为_LC_WARNING_
cTLogLevel:本地染色日志的日志记录级别(取值范围0-5,请参考"日志级别定义")，小于等于该级别的都记录到本地
cTLogMsgLevel:染色日志中心的日志记录级别(取值范围0-5,请参考"日志级别定义")，小于等于该级别的都发送到染色日志中心, 一般设置为_LC_WARNING_
cRelayLogLevel:转发包的记录级别(取值范围0-5,请参考"日志级别定义"),小于等于该级别并且染色配置命中,会转发到指定的ip:port
iWaterDestID: 流水日志需要发送到的日志中心ID（流水日志中心可能有多套,每套一个ID）
iColorDestID: 染色日志需要发送到的日志中心ID（染色日志中心可能有多套,每套一个ID）
#define LS_WATER_DEST_ID	1		//流水日志中心ID，1为公用系统ID，如果业务需要单独搭建一套日志中心存储流水信息，那么可以申请分配新ID
#define LS_COLOR_DEST_ID	2		//染色日志中心ID， 2为公用系统ID，如果如果业务需要单独搭建一套存储染色信息，那么可以申请分配新ID

函数返回值:
=0:成功,该函数一定会执行成功
*/
int LsReloadLogCfg(int iSvrConfiged, int iColorSvrConfiged,  
        int8_t cWLogLevel, int8_t cWLogMsgLevel,  int8_t cTLogLevel, int8_t cTLogMsgLevel, 
        int8_t cRelayLogLevel,  int iWaterDestID, int iColorDestID);




/*
   功能说明:
           记录日志之前先调用LsClientBegin(),该API要求传入一个LsPkgParam指针,LsPkgParam
   各字段的详细含参考数据结构的定义，为了便于定位问题，要求各个字段尽量填写完整，尤其是
   ddwUin和dwCmd一定要填写正确,如果传入的ddwUin为0, 则日志中心会将为0的ddwUin 替换为一个
   随机值
            请特别注意:除ddwUin外,LsPkgParam中的字段只记录在日志中心的数据库中,用来在日志中心的
    数据库中按字段进行索引,不会在本地日志文件中进行记录。
           每收到一个请求包后，根据收包socket解析对端的IP地址，并解析包头(有些系统可能需要解析部分
   包体)便可得到LsPkgParam结构中的各个字段，然后调用LsClientBegin()函数。某些系统没有uin的
   概念,传入的ddwUin填成0即可


   参数说明:
pstLsPkgParam:指向一个LsPkgParam结构体,该结构体的各字段已填写完整,不能是空指针,否则
函数执行失败


函数返回值:
<0: 失败
=0:成功
*/
int LsPreSetParam(LsPkgParam *pstLsPkgParam);




/*
   功能说明:
   修改LsPkgParam结构中的dwErrId, dwUserDef1-4, LsPkgParam结构中的dwErrId, dwUserDef1-4比较特殊，
   一次处理过程中，可能需要多次修改dwErrId, dwUserDef1-4中的某一个或某几个字段,可调用
   LsSetParam（）进行修改

   参数说明:
dwType: 取值范围为下列的宏定义
#define	_LC_ERR_RET_ID_		1  //修改LsPkgParam的dwErrId
#define	_LC_USER_DEF1_		2  //修改LsPkgParam的dwUserDef1
#define	_LC_USER_DEF2_		3  //修改LsPkgParam的dwUserDef2
#define	_LC_USER_DEF3_		4  //修改LsPkgParam的dwUserDef3
#define	_LC_USER_DEF4_		5  //修改LsPkgParam的dwUserDef4
dwVaule: 要设置的值

函数返回值:
=0:成功,该函数一定会执行成功
*/
int LsSetParam(uint32_t dwType, uint32_t dwVaule);



/*
   功能说明:
   调用LsCheckWriteWL()判断否要记录日志

   一般LsCheckWriteWL()和LsWaterLog()不单独使用,为了使用方便，定义了LOGSYS_WATER宏,记录日志时请直接使用LOGSYS_WATER()宏

   参数说明:
iLogLevel: 要记录日志的级别 (取值范围0-5,请参考"日志级别定义")

函数返回值:
=0
*/
int LsCheckWriteWL(int iLogLevel);





/*
   功能说明:
   调用LsWaterLog()进行日志记录或发送


   一般LsCheckWriteWL()和LsWaterLog()不单独使用,为了使用方便，定义了LOGSYS_WATER宏,记录日志时请直接使用LOGSYS_WATER()宏

   参数说明:
szFile:源文件名
iLine:源代码的所在的行
szFunction:源代码的所在的函数
iLogLevel: 要记录日志的级别 (取值范围0-5,请参考"日志级别定义")
sFormat:日志的格式
函数返回值:
=0
*/
int LsWaterLog(const char * szFile, int iLine, const char * szFunction, int iLogLevel,  const char *sFormat, ...)
__attribute__ ((format(printf,5,6)));

/*
   功能说明:
   调用LsWaterLogStr()进行日志记录或发送, 用于LOGSYS_WATER_STR()宏

   参数说明:
szFile:源文件名
iLine:源代码的所在的行
szFunction:源代码的所在的函数
iLogLevel: 要记录日志的级别 (取值范围0-5,请参考"日志级别定义")
szLogStr:日志字符串
函数返回值:
=0
*/
int LsWaterLogStr(const char * szFile, int iLine, const char * szFunction, int iLogLevel, char *szLogStr);

/*
   功能说明:
   转发包函数，根据转发包的染色配置将某些uin或命令号匹配的请求包转发到指定的ip:port

   参数说明:
iLogLevel: 要转发包的级别 (取值范围1-5,请参考"日志级别定义")
sPkg:要转发的请求包,长度由iPkgLen指定,不能是空指针
iPkgLen:sPkg的长度,取值范围(0-64KB)


函数返回值:
=0:不符合转发条件, 未转发
=1:符合转发条件, 已转发
=-10:参数传入错误
<0的其他值:内部系统错误
*/
int LsColorRelayPkg(int iLogLevel, void *sPkg, int iPkgLen);

/*
   功能说明:
   转发包函数，根据转发包的染色配置将某些uin或命令号匹配的请求包转发到指定的ip:port

   参数说明:
   	iSock: 用于转发包的socket
	iLogLevel: 要转发包的级别 (取值范围1-5,请参考"日志级别定义")
	sPkg:要转发的请求包,长度由iPkgLen指定,不能是空指针
	iPkgLen:sPkg的长度,取值范围(0-64KB)


函数返回值:
	=0:不符合转发条件, 未转发
	=1:符合转发条件, 已转发
	=-1:iSock小于0
	=-10:参数传入错误
	<0的其他值:内部系统错误
*/
int LsColorRelayPkgWithSock(int iSock, int iLogLevel,  void *sPkg, int iPkgLen);

/*设置错误信息*/
int LsSetRetErrMsg(char * szErrMsg);

/*清除错误信息*/
int LsClearRetErrMsg();

/*记录统计日志*/
int LsStaticsLog(const char *sFormat, ...);

/*是否是公司相关号码*/
/*
函数返回值：
	=1：是公司相关号码
	=0：不是公司相关号码
	<0：错误
*/
int LsIsCompanyUin(uint64_t ddwUin);


/*
   API使用说明
   1.在进程初始化时调用LsClientInit()
   2.每收到一个请求包或开始一个新的处理流程,需要调用LsPreSetParam()
   3.使用LOGSYS_WATER宏记录日志
   4.在进程运营过程中,如果要修改某些配置项,可调用LsReloadLogCfg()
   5.不建议直接修改g_stLsClient的成员变量
   6.如果要将染色配置命中的请求包转发到指定的ip:port,调用LsColorRelayPkg()函数
   并判断返回值是否为1,如果返回值为1,表明转发成功,否则转发失败
   7.更多信息请参考《统一日志系统使用说明.doc》
*/

extern LsClient g_stLsClient;

#define LOGSYS_WATER(iLogLevel, fmt, args...) do { \
    if (LsCheckWriteWL(iLogLevel)) \
    LsWaterLog(__FILE__ , __LINE__ , __FUNCTION__, iLogLevel, fmt, ## args); }  while (0)

#define LOGSYS_WATER_STR(iLogLevel, szLogStr) do { \
	if (LsCheckWriteWL(iLogLevel)) \
		LsWaterLogStr(__FILE__ , __LINE__ , __FUNCTION__, iLogLevel, szLogStr); }  while (0)

#define LOGSYS_BUF(iLogLevel, title, buffer, len) do { \
    if (LsCheckWriteWL(iLogLevel)) \
    { \
        LsWaterLog(__FILE__ , __LINE__ , __FUNCTION__, iLogLevel, "%s\n", title);   \
    LsWaterLog(__FILE__ , __LINE__ , __FUNCTION__, iLogLevel, DumpPackage(buffer, len));     \
} \
}while (0)


/********************************添加对多业务支持后的新的记录日志的函数*********************************/
/*
!*保存业务的日志级别及是否使用远程日志的信息，
!*主要用于业务在运行期间自己修改配置
!*/
typedef struct
{
	int8_t cWLogLevel;    /*本地流水日志级别*/
	int8_t cWLogMsgLevel; /*远程流水日志级别*/
	int8_t cTLogLevel;
	int8_t cTLogMsgLevel;
	int8_t cRelayLogLevel;
}LsLogLevelConf;
	


/************************************************************************************************************/
/*                                          设置客户初始化参数的相关操作                                    */
/************************************************************************************************************/
/*
!*brief : 用户自定义字段，用于初始化时传递给LsLogFsOpen函数，主要包含了是否开启远程日志，远程日志系统id等信息。
!*/
typedef struct 
{
	int   iSvrConfiged;
	int   iColorSvrConfiged;
	int   m_iWaterDestID;
	int   m_iColorDestID;
    uint32_t dwUserDef1;       //初始化用户自定义字段1，由调用者自己填写，自己解释
    uint32_t dwProcId;
	uint16_t wMsgPort;			//可指定发送远程日志所绑定的端口，为0则随机端口
	uint16_t wRelayPort;		//可指定转发包所绑定的端口，为0则随机端口
	uint16_t cAgentLocalLog;    //是否由Agent来负责记录本地log，业务进程不再记录
}LOGSYS_CLIENT_INFO;


/*
!*brief : 初始化业务相关的信息
!*默认开启远程流水日志和远程染色日志(如果配置中心有该业务的日志级别相关的配置，则使用配置中心的日志级别，无则使用默认
!*的日志级别:远程日志使用LC_WARNING，本地流水级别为LC_WARNING，关闭本地染色功能
!*默认开启由agent代写本地日志的功能
!*
!*注意: 如果所有的值都使用默认值，远程日志中心会使用配置中心的配置，因此需要安装最新的agent，否则
!*需要调用LsLogFsClientInfoSetLogDestID函数设置远程日志中心的id。
!*/

int LsLogFsClientInfoInit(LOGSYS_CLIENT_INFO *pstClientInfo);

/*
!*设置进程号,在由api自己写本地日志时可用来区分不同的进程
!*/
int LsLogFsClientInfoSetProcId(LOGSYS_CLIENT_INFO *pstClientInfo, uint32_t value);

/*
!*设置初始化时的userdef项。
!*/
int LsLogFsClientInfoSetInitUserDef(LOGSYS_CLIENT_INFO *pstClientInfo, uint32_t value);

/*
!*设置是否使用远程染色日志中心，value=0表示否，其余表示是
!*/
int LsLogFsClientInfoSetUseColorSvr(LOGSYS_CLIENT_INFO *pstClientInfo, int value);

/*
!*设置是否记录远程流水日志，value=0表示否，其余表示是
!*/
int LsLogFsClientInfoSetUseWaterSvr(LOGSYS_CLIENT_INFO *pstClientInfo, int value);

/*
!*设置数据包转发时所使用的端口号
!*/
int LsLogFsClientInfoSetRelayPort(LOGSYS_CLIENT_INFO *pstClientInfo, uint16_t value);

/*设置远程染色配置的系统id和远程流水日志的系统id*/
int LsLogFsClientInfoSetLogDestID(LOGSYS_CLIENT_INFO *pstClientInfo, int iWaterDestID, int iColorDestID);




/*
!*由业务方初始化时调用，不需要携带任何配置
!*brief :只使用业务名或者ID进行初始化的函数，默认开启远程日志功能，关闭本地日志功能
!*param[in]:szAppName ,业务名称，在dwAppId不为0时可以为NULL，最长255
!*param[in]:szUserName,业务负责人名称，在dwAppId不为0时可以为NULL，最长为1024
!*param[in]:dwAppId   ,业务ID，如不知则填0
!*retval >=0 :一个正确的句柄，other初始化错误
!*本地日志相关的信息，包含了本地日志的文件数，每个日志文件的大小及日志文件的绝对路径
!*/
	int LsLogFsOpenNoConf(const char * szAppName, const char * szUserName, const uint32_t dwAppId,
								LsLogFileInfo *pstLsLogFileInfo);


/*
!*由业务方调用，在写日志之前需要调用该函数
!*brief    :根据业务名称，获取一个对应的句柄 
!*param[in]:szAppName		,业务名称，在dwAppId不为0时可以为NULL，最长255
!*param[in]:szUserName		 ,业务负责人名称，在dwAppId不为0时可以为NULL，最长为1024
!*param[in]:iSvrConfiged	  ,是否使用远程流水日志中心
!*param[in]:iColorSvrConfiged ,是否使用远程染色日志中心
!*param[in]:pstLsInitParam	  ,指向LsInitParam结构体,用于传入进程初始化是可以确定的信息:本机Appid,初始化用户自定义字段1,进程ID,不能传入空指针,否则函数调用出错
!*param[in]:pstLsLogFileInfo  ,用于传入业务的本地日志相关的配置，比如本地日志的文件名，文件大小及文件个数
!*retval >=0 :一个正确的句柄，other初始化错误
!*/
/*
!*等进一步确认了自动注册流程再来确认该函数
!*/
int LsLogFsOpen(const LOGSYS_CLIENT_INFO *pstClientInfo,
                         const char * szAppName, const char * szUserName,  const uint32_t dwAppId,
                         LsLogFileInfo *pstLsLogFileInfo);

/*
!*
!*brief : 关闭一个句柄
!*param[in] iFd : 要关闭的log file描述符
!*retval 0表示正确，other初始化错误
!*/
int LsLogFsClose(const int iFd);

/*
!*
!*brief : 向一个给定句柄中写入日志信息，首先获取与FD对应的实例，然后使用其中保存的配置信息来为业务写日志信息
!*/
int LsLogFsWrite(const int iFd, const char * szFile, int iLine, 
					 const char * szFunction, int iLogLevel, const char *sFormat, ...)
__attribute__ ((format(printf,6,7)));

/*
!*brief : 业务方提供va_list作为参数的写日志函数
!*param[in] iFd : 初始化时返回的文件描述符
!*param[in] va	: 要写入的valist参数
!*/
int LsLogFsWriteVA(const int iFd, const char * szFile, int iLine, 
						const char * szFunction, int iLogLevel, const char *sFormat, va_list va);


/*
!*brief : 转发数据包的函数
!*param[in] iFd : 由初始化函数返回的与业务对应的句柄
!*param[in] iLogLevel : 级别
!*param[in] sPkg : 需要转发的数据包
!*param[in] iPkgLen : 需要转发的数据包的长度
!*retval : 1表示成功， other 表示失败
!*/
int LsLogFsRelayPkg(const int iFd, const int iLogLevel,  const void *sPkg, const int iPkgLen);

/*
!*brief : 设置当前业务的日志信息，包括userdef，uin等等
!*param[in] : iFd 要设置的业务所对应的文件描述符
!*param[in] : pstLsPktParam 要设置的数据包的参数，该指针不能为空
!*retval	: 0 成功， other 失败 
!*/
int LsLogFsPreSetPktParam(int iFd, LsPkgParam *pstLsPktParam);

/*
!*brief : 根据类型设置数据包参数，这些参数用于在匹配染色配置时使用
!*param[in] : iFd ,open函数返回给业务的描述符
!*param[in] : dwType , 要设置的参数的类型
!*param[in] : dwValue, 要设置的参数的值
!*retval	: 0 成功， other 失败 
!*/
int LsLogFsSetPktParam(int iFd, uint32_t dwType, uint32_t dwVaule);

/*
!*brief : 获取一个业务的日志级别等配置信息
!*param[in] : iFd ,初始化时返回给业务方的文件描述符
!*param[out]: pstLsLogLevelConf, 业务的日志级别等配置
!*/
int LsLogGetConf(int iFd, LsLogLevelConf *pstLsLogLevelConf);

/*
!*brief : 设置日志级别等配置信息
!*param[in] : iFd ,初始化时返回给业务方的文件描述符
!*param[in] : pstLsLogLevelConf , 要设置的新的日志级别等信息，
!retval 	: 0 LOGSYS_EOK 成功，其余值表示失败
!*ps : 如果在设置该函数后，配置中心的业务日志级别有修改时以配置中心的配置为准
!*/
int LsLogSetConf(int iFd, LsLogLevelConf *pstLsLogLevelConf);


/*
!*brief :初步判断一条日志的日志级别是否满足发送的条件,不对染色配置进行匹配
!*param[in] iFd: 要判断的业务对应的文件描述符
!*param[in] iLogLevel 日志级别
!*retval 1 该日志级别的日志需要被进一步处理，0 该级别的日志不需要被进一步处理，other 错误
!*/
int LsLogFsCheckLevel(const int iFd, const int iLogLevel);


/*
   API使用说明
   1.在进程初始化时调用LsLogFsOpen 或者LsLogFsOpenNoCfg函数获取一个句柄
   2.每收到一个请求包或开始一个新的处理流程,需要调用LsLogFsPreSetPktParam来设置相关的参数
   3.使用LOGSYS_WATER_V2宏记录日志
   4.在进程运营过程中,如果要修改某些配置项,可调用LsLogSetConf()来设置新的配置，也可以使用LsLogGetConf来获取旧的配置
   5.不建议直接修改g_stLsClient的成员变量
   6.如果要将染色配置命中的请求包转发到指定的ip:port,调用LsLogFsRelayPkg()函数
   并判断返回值是否为1,如果返回值为1,表明转发成功,否则转发失败
   7.更多信息请参考《统一日志系统使用说明.doc》
*/

#define LOGSYS_WATER_V2(iFd, iLogLevel, fmt, args...) do{\
	if (LsLogFsCheckLevel(iFd, iLogLevel)) \
	LsLogFsWrite(iFd, __FILE__, __LINE__, __FUNCTION__, iLogLevel, fmt, ##args);\
}while(0)

/*以valist为参数*/
#define LOGSYS_WATER_VA_LIST(iFd, iLogLevel, fmt, ap) do{\
	if (LsLogFsCheckLevel(iFd, iLogLevel)) \
	LsLogFsWriteVA(iFd, __FILE__, __LINE__, __FUNCTION__, iLogLevel, fmt, ap);\
}while(0)

#define LOGSYS_WATER_STR_V2(iFd, iLogLevel, szLogStr) do{\
	if (LsLogFsCheckLevel(iFd, iLogLevel)) \
	 LsLogFsWrite(iFd, __FILE__ , __LINE__ , __FUNCTION__, iLogLevel, "%s", szLogStr);\
}while(0)

#define LOGSYS_BUF_V2(iFd, iLogLevel, title, buffer, len) do { \
	if (LsLogFsCheckLevel(iFd, iLogLevel)) {\
	LsLogFsWrite(iFd, __FILE__ , __LINE__ , __FUNCTION__, iLogLevel, "%s\n", title);   \
    LsLogFsWrite(iFd, __FILE__ , __LINE__ , __FUNCTION__, iLogLevel, DumpPackage(buffer, len));    } \
}while (0)

/*
   功能说明:
   转发包函数，根据转发包的染色配置将某些uin或命令号匹配的请求包转发到指定的ip:port

   参数说明:
iLogLevel: 要转发包的级别 (取值范围1-5,请参考"日志级别定义")
sPkg:要转发的请求包,长度由iPkgLen指定,不能是空指针
iPkgLen:sPkg的长度,取值范围(0-64KB)


函数返回值:
=0:不符合转发条件, 未转发
=1:符合转发条件, 已转发
=-10:参数传入错误
<0的其他值:内部系统错误
*/

/*********************************************end*******************************************************/
}

#endif

