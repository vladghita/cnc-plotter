#include "StdAfx.h"
#include "PlottingPath.h"


CPlottingPath::CPlottingPath(void)
	:	idx(0)
	,	isRealPath(FALSE)
{
	::InitializeCriticalSection(&cs);
}


CPlottingPath::~CPlottingPath(void)
{
	m_bresenhamPoints.clear();

	ClearPlottedPoints();

	::DeleteCriticalSection(&cs);
}

void CPlottingPath::Lock()
{
	::EnterCriticalSection(&cs);
}

void CPlottingPath::Unlock()
{
	::LeaveCriticalSection(&cs);
}

CPoint *CPlottingPath::GetStartPoint()
{
	return &startPoint;
}

CPoint *CPlottingPath::GetEndPoint()
{
	return &endPoint;
}

CPoint *CPlottingPath::GetCurrentPoint()
{
	if((idx < 0)||(idx > m_bresenhamPoints.size()))
	{
		TRACE("Debug: %s => Invalid index", __FUNCTION__);
		return NULL;
	}
	else return &m_bresenhamPoints.at(idx);
}

std::vector<CPoint> *CPlottingPath::GetBresenhamPoints()
{
	return &m_bresenhamPoints;
}

long CPlottingPath::GetBresenhamPointsCount()
{
	return m_bresenhamPoints.size();
}

long CPlottingPath::GetPlottedPointsCount()
{
	Lock();
	long count = m_plottedPoints.size();
	Unlock();

	return count;
}

void CPlottingPath::AddBresenhamPoint(int x, int y)
{
	CPoint pp;
	pp.x = x;
	pp.y = y;
	m_bresenhamPoints.push_back(pp);
}

void CPlottingPath::AddPlottedPoint(int x, int y, int realPoint)
{
	Lock();

	PltPoint pp;
	pp.xCoord = x;
	pp.yCoord = y;
	pp.isPenDown = realPoint;

	m_plottedPoints.push_back(pp);

	Unlock();
}

CPoint* CPlottingPath::GetBresenhamPointAt(unsigned long idx)
{
	if((idx < 0)||(idx > m_bresenhamPoints.size()))
	{
		TRACE("Debug: %s => Invalid index", __FUNCTION__);
		return NULL;
	}
	else return &m_bresenhamPoints.at(idx);
}

void CPlottingPath::GetPlottedPointAt(unsigned long idx, PltPoint *point)
{
	if((idx < 0)||(idx > m_plottedPoints.size()))
	{
		TRACE("Debug: %s => Invalid index", __FUNCTION__);
	}

	Lock();
	*point = m_plottedPoints.at(idx);
	Unlock();
}

void CPlottingPath::ClearBresenhamPoints()
{
	m_bresenhamPoints.clear();
}

void CPlottingPath::ClearPlottedPoints()
{
	Lock();
	m_plottedPoints.clear();
	Unlock();
}

//algoritm de calcul pentru a determina punctele tranzitoriiale unei linii
//returneaza true daca punctele au fost inversate sau false daca nu au fost inversate
BOOL CPlottingPath::lineBresenham(int p1x, int p1y, int p2x, int p2y)
{
   int F, x, y;

   BOOL wasSwapped = FALSE;

   if (p1x > p2x)  // Swap points if p1 is on the right of p2
   {
       swap(p1x, p2x); 
       swap(p1y, p2y);

	   wasSwapped = TRUE;
   }

   // Handle trivial cases separately for algorithm speed up.
   // Trivial case 1: m = +/-INF (Vertical line)
   if (p1x == p2x)
   {
       if (p1y > p2y)  // Swap y-coordinates if p1 is above p2
       {
           swap(p1y, p2y);

		   wasSwapped = TRUE;
       }

       x = p1x;
       y = p1y;
       while (y <= p2y)
       {
           AddBresenhamPoint(x, y);
           y++;
       }
       return wasSwapped;
   }
   // Trivial case 2: m = 0 (Horizontal line)
   else if (p1y == p2y)
   {
       x = p1x;
       y = p1y;

       while (x <= p2x)
       {
           AddBresenhamPoint(x, y);
           x++;
       }
       return wasSwapped;
   }


   int dy            = p2y - p1y;  // y-increment from p1 to p2
   int dx            = p2x - p1x;  // x-increment from p1 to p2
   int dy2           = (dy << 1);  // dy << 1 == 2*dy
   int dx2           = (dx << 1);
   int dy2_minus_dx2 = dy2 - dx2;  // precompute constant for speed up
   int dy2_plus_dx2  = dy2 + dx2;


   if (dy >= 0)    // m >= 0
   {
       // Case 1: 0 <= m <= 1 (Original case)
       if (dy <= dx)   
       {
           F = dy2 - dx;    // initial F

           x = p1x;
           y = p1y;
           while (x <= p2x)
           {
               AddBresenhamPoint(x, y);
               if (F <= 0)
               {
                   F += dy2;
               }
               else
               {
                   y++;
                   F += dy2_minus_dx2;
               }
               x++;
           }
       }
       // Case 2: 1 < m < INF (Mirror about y=x line
       // replace all dy by dx and dx by dy)
       else
       {
           F = dx2 - dy;    // initial F

           y = p1y;
           x = p1x;
           while (y <= p2y)
           {
               AddBresenhamPoint(x, y);
               if (F <= 0)
               {
                   F += dx2;
                }
                else
                {
                    x++;
                    F -= dy2_minus_dx2;
                }
                y++;
            }
        }
    }
    else    // m < 0
    {
        // Case 3: -1 <= m < 0 (Mirror about x-axis, replace all dy by -dy)
        if (dx >= -dy)
        {
            F = -dy2 - dx;    // initial F

            x = p1x;
            y = p1y;
            while (x <= p2x)
            {
                AddBresenhamPoint(x, y);
                if (F <= 0)
                {
                    F -= dy2;
                }
                else
                {
                    y--;
                    F -= dy2_plus_dx2;
                }
                x++;
            }
        }
        // Case 4: -INF < m < -1 (Mirror about x-axis and mirror 
        // about y=x line, replace all dx by -dy and dy by dx)
        else    
        {
            F = dx2 + dy;    // initial F

            y = p1y;
            x = p1x;
            while (y >= p2y)
            {
                AddBresenhamPoint(x, y);
                if (F <= 0)
                {
                    F += dx2;
                }
                else
                {
                    x++;
                    F += dy2_plus_dx2;
                }
                y--;
            }
        }
    }

   return wasSwapped;
}

void CPlottingPath::swap(int &a, int &b)
{
	int c = a;
	a = b;
	b = c;
}

void CPlottingPath::ReverseOrder()
{
   std::reverse(m_bresenhamPoints.begin(),m_bresenhamPoints.end());
}

void CPlottingPath::SetCurrentPlottingPath(int x_start, int y_start, int x_end, int y_end, BOOL realPath)
{
	idx = 0;
	ClearBresenhamPoints();
	isRealPath = realPath;

	startPoint.SetPoint(x_start, y_start);
	endPoint.SetPoint(x_end, y_end);

	BOOL wasSwapped = lineBresenham(x_start, y_start, x_end, y_end);
    if(wasSwapped)
		this->ReverseOrder();
}

BOOL CPlottingPath::HasNextStep()
{
	return idx+1 < m_bresenhamPoints.size() ? TRUE : FALSE;
}

void CPlottingPath::StepForward()
{
	if(m_bresenhamPoints.size() > 0)
	{
		if(idx + 1 < m_bresenhamPoints.size())
			idx ++;
	}
}

BOOL CPlottingPath::IsRealPath()
{
	return isRealPath;
}