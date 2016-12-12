#if !defined(__LOGGER__)
#define __LOGGER__



class CLogger
{
private:
	CRITICAL_SECTION cs;

public:
	CLogger(void);
	~CLogger(void);
	void AppendLogMessageWithTimestamp(LPCTSTR pszText);
	void AppendLogMessage(LPCTSTR pszText);

private:
	void AppendTextToOutputLog(LPCTSTR pszText);
	void AppendTextToOutputLogWithNewLine(LPCTSTR pszText);
	
};

#endif /* __LOGGER__ */