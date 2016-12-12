#if !defined(__BASE_THREAD_H__)
#define __BASE_THREAD_H__

#include <process.h>

#define MAX_THREAD_NAME	255

class CBaseThread
{
private:
	TCHAR			m_tszThreadName[MAX_THREAD_NAME + 1];
	HANDLE			m_hThread;
	HANDLE			m_hStartEvent;
	BOOL			m_bRunning;
	BOOL			m_bSkipEntry;
	unsigned int	m_nThreadId;

	HRESULT			m_HrCoInit;

public:
	CBaseThread(const TCHAR *ptszThreadName = NULL)
		:	m_hThread(NULL)
		,	m_hStartEvent(NULL)
		,	m_bRunning(FALSE)
		,	m_bSkipEntry(FALSE)
		,	m_nThreadId(0x00)
		,	m_HrCoInit(E_FAIL)
		#if defined(__NOTIFY_H__)
		,	m_notifier(XNotifier::Create())
		#endif	/*__NOTIFY_H__*/
	{
		memset(m_tszThreadName, 0, sizeof(m_tszThreadName));
		if(ptszThreadName != NULL)
			memcpy(m_tszThreadName, ptszThreadName, min(_tcslen(ptszThreadName), MAX_THREAD_NAME) * sizeof(TCHAR));

		CreateThread();
	}

	virtual ~CBaseThread(void)
	{
		if(IsRunning())
			StopThread();

		if(m_hStartEvent != NULL)
		{
			CloseHandle(m_hStartEvent);
			m_hStartEvent = NULL;
		}
	}

	#ifdef _DEBUG
		void AssertValid(void) const
		{
			ASSERT(this != NULL);

			ASSERT(m_hStartEvent	!= NULL);
			ASSERT(m_hThread		!= NULL);
		};
		void Dump(CDumpContext& dc) const
		{
			dc <<"Thread Handle: \""<<(DWORD)m_hThread<<"\" Thread Id: \""<<m_nThreadId<<"\".";
			dc <<"Thread Name: "<<m_tszThreadName<<" at " << (void*)(this) << "\n";
			//UNUSED(dc); // unused in release build
		};
	#endif

private:
	void				__cdecl CreateThread(void)
	{
		try
		{
			m_hStartEvent	= ::CreateEvent(NULL, TRUE, FALSE, NULL);
			ResetEvent(m_hStartEvent);
			m_hThread		= (HANDLE)_beginthreadex(NULL, 0x00, StaticEntryPoint, this, 0x00, &m_nThreadId);
		}
		catch(...)
		{
			TRACE(_T("\tDebug: CBaseThread::CreateThread(). Exception. \n"));
			return;
		}
	};

	static unsigned int __stdcall StaticEntryPoint(LPVOID pParam)
	{
		unsigned int nret = 0x00;
		try
		{
			CBaseThread *pThread = (CBaseThread*)pParam;
			if(pThread != NULL)
				nret = pThread->PreEntryPoint();
		}
		catch(...)
		{
			TRACE(_T("\tDebug: CBaseThread::StaticEntryPoint(). Exception. \n"));
			return ((unsigned int) -1);
		}
		return nret;
	};

	unsigned int		__cdecl PreEntryPoint(void)
	{	/*
		*	We Have To Use A seccond caller, cause if you want to overwrite the EntryPoint(), it shall not be called, due to TypeCasting into
		*   StaticEntryPoint() procedure
		*/
		unsigned int nret = 0x00;
		try
		{
			nret = EntryPoint();
		}
		catch(...)
		{
			TRACE(_T("\tDebug: CBaseThread::PreEntryPoint(). Exception. \n"));
			return ((unsigned int) -1);
		}
		return nret;
	};

protected:
	virtual unsigned int __cdecl EntryPoint(void)
	{
		unsigned int nret = 0x00;

		ASSERT(this);

		try
		{	
			MSG msg = {0x00};
			::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE);
			
			//	Initialize COM Factory
			//
			m_HrCoInit = CoInitializeEx(0, COINIT_APARTMENTTHREADED);

			m_bRunning = TRUE;
			if(WaitForMultipleObjects(0x01, &m_hStartEvent, FALSE, INFINITE) != WAIT_OBJECT_0)
				return ((unsigned int)-1);
		
			if(!m_bSkipEntry)
				Run();
			
			//	Release COM Factory
			//
			if(m_HrCoInit == S_OK)
				CoUninitialize();
		}
		catch(...)
		{
			TRACE(_T("\tDebug: CBaseThread::EntryPoint(). Exception. \n"));
			return ((unsigned int) -1);
		}

		return nret;
	};

	virtual void __cdecl Run(void)
	{
		try
		{
			ThreadPreLoop();
			ThreadLoop(); 
			ThreadPostLoop();
		}
		catch(...)
		{
			TRACE(_T("\tDebug: CBaseThread::Run(). Exception.\n"));
			return;
		}
	};
	virtual void __cdecl ThreadPreLoop(void){};
	virtual void __cdecl ThreadLoop(void){};
	virtual void __cdecl ThreadPostLoop(void){};

public:
	BOOL __cdecl IsRunning(void)const
	{	return m_bRunning;			};

	BOOL __cdecl StartThread(void)
	{
		ASSERT(this);
		BOOL bRet = FALSE;

		if(m_hStartEvent != NULL)
		{
			SetEvent(m_hStartEvent);
			bRet = TRUE;
		}
		return bRet;
	};

	BOOL __cdecl StopThread(void)
	{
		ASSERT(this);
		BOOL bRet = FALSE;
		
		{	/* In case the thread is pending for a StartThread() Call, If the thread is allready running, the following block has no effect */
			m_bSkipEntry = TRUE;
			if(m_hStartEvent != NULL)
				SetEvent(m_hStartEvent);
		}

		if(m_hThread != NULL)
		{
			m_bRunning = FALSE;
			if(WaitForMultipleObjects(0x01, &m_hThread, FALSE, 5000) != WAIT_OBJECT_0)
				::TerminateThread(m_hThread, (DWORD)-1);
				
			CloseHandle(m_hThread); m_hThread = NULL;
			
			#if defined(_DEBUG)
				TRACE(_T("\tDebug: \"CBaseThread::StopThread()\" => \"Thread: [%s] Closed.\".\n "), CString(m_tszThreadName));
			#endif	/*_DEBUG*/

			bRet = TRUE;
		}
		else
			bRet = FALSE;
		
		return bRet;
	};

	void __cdecl PhStop(void)
	{	InterlockedExchange((LONG*)&m_bRunning, FALSE);	};

	DWORD __cdecl GetThreadID(void)const	{	return m_nThreadId;	};

	HANDLE __cdecl GetHandle()
	{
	  return m_hThread;
	}
};

#endif	/* __BASE_THREAD_H__ */