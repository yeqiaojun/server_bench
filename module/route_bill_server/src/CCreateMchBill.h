/*
 * CCreateMchBill.h
 *
 *  Created on: 2017年7月21日
 *      Author: hawrkchen
 *      Desc:日切产生第一次清分记录并入账
 */

#ifndef _CCREATEMCHBILL_H_
#define _CCREATEMCHBILL_H_

#include "IUrlProtocolTask.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "../Base/Comm/clib_mysql.h"
#include "../Base/common/comm_tools.h"
#include "CheckParameters.h"
#include "CRouteBillBase.h"


class CCreateMchBill : public IUrlProtocolTask
{
public:
	CCreateMchBill();
	virtual ~CCreateMchBill();

    INT32 Init()
    {
        m_bInited = true;
        return 0;
    }

	void LogProcess();

    INT32 Execute( NameValueMap& mapInput, char** outbuf, int& outlen );

    void Reset()
    {
        IUrlProtocolTask::Reset();
		m_InParams.clear();
		m_RetMap.clear();
		m_ContentJsonMap.clear();
		m_pBillDB = Singleton<CSpeedPosConfig>::GetInstance()->GetBaseDB();
    }

protected:
    void FillReq( NameValueMap& mapInput);
    void CheckInput();
    void BuildResp( CHAR** outbuf, INT32& outlen );

    INT32 CalcEffectiveTimeBill();

    void CheckMchBillTask();

    void CreateMchBill();

    void CreateChanBill();

    void InserIntoMchBill(MchBillSum& mch_bill,const char* fund_type);

    void AccountingCheckin();

    void UpdateCheckStatus(const string& order_no,const string& fund_id,const string& account_no,const string& acc_status,const string& acc_desc);

    std::string GetAccountSeqNo();

	void SetRetParam();

	std::string m_start_time;
	std::string m_end_time;

	NameValueMap m_InParams;
	NameValueMap m_RetMap;
	JsonMap m_ContentJsonMap;

	//vector<ChannelInfo> vec_channel;

	std::map<std::string, MchBillSum> mchPayBillMap;
	std::map<std::string, MchBillSum> mchRefundBillMap;
	std::map<std::string, OrderPayLiquidate> orderChannelMap;
	std::map<std::string, OrderPayLiquidate> orderChannelRefMap;

	CMySQL m_mysql;

	ostringstream sqlss;

	clib_mysql * m_pBillDB = NULL;

};



#endif /* _CCREATEMCHBILL_H_ */
