#if !defined(__SA_PLOTTING_THREAD__)
#define __SA_PLOTTING_THREAD__

#include "basethread.h"
#include "PlottingPath.h"
#include "PlottedObject.h"
#include "Logger.h"

class CPlottingThread : public CBaseThread
{
private:
	long line_idx;
	long line_count;
	long step_count;
	DWORD startTime;
	CPlottingPath *m_pDrawPath;
	CPlottedObject *m_pPlottedObject;

	//CPen penDown, penUp;

public:

	CPlottingThread(void);
	~CPlottingThread(void);

	void Initialize();

	BOOL HasNext();	//check if the next point is available
	BOOL NextStep();
	CPoint *GetCurrentPoint();
	BOOL IsRealPoint();    //daca punctul e real (pen down) sau virtual (pen up)

	//void DrawCursor(int orig_x, int orig_y, BOOL isRealPoint, int raza);

	void __cdecl ThreadPreLoop(void);
	void __cdecl ThreadLoop(void);
	void __cdecl ThreadPostLoop(void);

	CPlottingPath *GetDrawPath()
	{
		return m_pDrawPath;
	}
};

#endif  /*__SA_PLOTTING_THREAD__*/