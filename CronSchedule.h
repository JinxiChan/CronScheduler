#ifndef _INC_CRONSCHEDULE_
#define _INC_CRONSCHEDULE_
//////////////////////////////////////////////////////////////////////////
#include "ccronexpr.h"
#include <list>
#include <string>
#include <functional>
//////////////////////////////////////////////////////////////////////////
//	if return none-zero, this job will be deleted
//typedef int (*FUNC_CRONCALLBACK)(int, int);
typedef std::function<int(std::string, void*)> FUNC_CRONCALLBACK;
//////////////////////////////////////////////////////////////////////////
class CronJobScheduler;

class CronJob
{
	friend class CronJobScheduler;

public:
	//	sorter
	class CronJobSorter
	{
	public:
		bool operator()(const CronJob* _pJobL, const CronJob* _pJobR)
		{
			return _pJobL->m_tmNext < _pJobR->m_tmNext;
		}
	};

public:
	CronJob();
	~CronJob();

public:
	bool Parse(const char* _pszCronExpr);
	void Reset();
	bool IsTriggerable();
	int Trigger();
	void GetNext();

private:
	std::string m_sJobId;
	void* m_nUserData;
	FUNC_CRONCALLBACK m_fnCallback;

	time_t m_tmNext;
	cron_expr* m_pCronParser;

	bool m_bDeleteFlag;
};
typedef std::list<CronJob*> CronJobList;

class CronJobScheduler
{
public:
	CronJobScheduler();
	~CronJobScheduler();

public:
	void Update();

	bool AddCronJob(std::string _sJobId, const char* _pszCronExpr, FUNC_CRONCALLBACK _fnCb, void* _nArg);
	int RemoveCronJob(std::string _sJobId);
	void Clear();

protected:
	void sortJobs();

protected:
	CronJobList m_xCronJobList;
	bool m_bJobWaitDelete;
};
//////////////////////////////////////////////////////////////////////////
#endif