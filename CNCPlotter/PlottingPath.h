#if !defined (__PLOTTING_PATH__)
#define __PLOTTING_PATH__

#include <vector>
#include "Global.h"


//reprezinta o singura linie care se va trasa la un moment dat intre 2 puncte
class CPlottingPath
{
private:
	CPoint startPoint;							//starting point of the current line/path
	CPoint endPoint;							//ending point of the current line/path
	std::vector<CPoint> m_bresenhamPoints;		//lista completa de puncte tranzitorii, componente ale liniei curente
	int idx;									//retine indexul cursorului (punctul curent desenat/plotat)
	BOOL isRealPath;							//daca linia curenta e reala (pen down) sau tranzitorie (pen up)
	std::vector<PltPoint> m_plottedPoints;		//lista de puncte deja plotate

	CRITICAL_SECTION cs;

public:
	CPlottingPath(void);
	~CPlottingPath(void);

	CPoint *GetStartPoint();
	CPoint *GetEndPoint();
	CPoint *GetCurrentPoint();				   //returneaza pointer la cursor/punctul curent
	std::vector<CPoint> *GetBresenhamPoints();
	long GetBresenhamPointsCount();
	void ClearBresenhamPoints();
	CPoint* GetBresenhamPointAt(unsigned long idx);
	void SetCurrentPlottingPath(int x_start, int y_start, int x_end, int y_end, BOOL realPath);  //setez linia curenta
	BOOL HasNextStep();															  //verific daca exista un pas urmator in path
	void StepForward();															  //mut cursorul (punctul curent si urmator de desenat)
	BOOL IsRealPath();										//daca linia e reala (pen down) sau virtuala (pen up)

	void Lock();
	void Unlock();

	long GetPlottedPointsCount();
	void AddPlottedPoint(int x, int y, int realPoint);
	void GetPlottedPointAt(unsigned long idx, PltPoint *point);
	void ClearPlottedPoints();

private:
	void swap(int &a, int &b);
	void AddBresenhamPoint(int x, int y);
	BOOL lineBresenham(int p1x, int p1y, int p2x, int p2y);
	void ReverseOrder();									//inverseaza ordinea in vectorul de puncte brsenham
};

#endif /*__PLOTTING_PATH__*/