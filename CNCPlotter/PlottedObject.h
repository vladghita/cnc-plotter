#if !defined(__PLOTTED_OBJECT__)
#define __PLOTTED_OBJECT__

#include "StdAfx.h"
#include <vector>
#include "Global.h"

class CPlottedObject
{
private:
	std::vector<PltPoint>	m_PrimitivePoints;    //list of points read from file
	std::vector<CPoint>		m_WiredPoints;        //list of lines drawn based on model points: two points wired together, specified by their indexes
	std::vector<PltLine>    m_ModeledLines;	  //list of lines between points computed after graphic transformations

public:
	CPlottedObject(void)
	{
	}

	~CPlottedObject(void)
	{
		m_PrimitivePoints.clear();
		m_WiredPoints.clear();
		m_ModeledLines.clear();
	}

	std::vector<PltPoint> *GetPrimitivePoints()
	{
		return &m_PrimitivePoints;
	}

	//get array as vector of CPoint
	void GetPrimitivePoints(std::vector<CPoint> &points)
	{
		for(int i = 0; i < m_PrimitivePoints.size(); i++)
		{
			points.push_back(CPoint(this->GetPrimitivePointAt(i)->xCoord, this->GetPrimitivePointAt(i)->yCoord));
		}
	}

	std::vector<CPoint> *GetWiredPoints()
	{
		return &m_WiredPoints;
	}

	std::vector<PltLine> *GetModeledLines()
	{
		return &m_ModeledLines;
	}

	long GetPrimitivePointsCount()
	{
		return m_PrimitivePoints.size();
	}

	long GetWiredPointsCount()
	{
		return m_WiredPoints.size();
	}

	long GetModeledLinesCount()
	{
		return m_ModeledLines.size();
	}

	void AddPrimitivePoint(int x, int y, BOOL penDown)
	{
		PltPoint pp;
		pp.xCoord = x;
		pp.yCoord = y;
		pp.isPenDown = penDown;
		m_PrimitivePoints.push_back(pp);
	}

	void AddWiredPoints(int first, int last)
	{
		CPoint pp;
		pp.x = first;
		pp.y = last;
		m_WiredPoints.push_back(pp);
	}

	void AddModeledLine(int x_start, int y_start, int x_end, int y_end, BOOL realPath)
	{
		PltLine line;
		line.x_start = x_start;
		line.y_start = y_start;
		line.x_end = x_end;
		line.y_end= y_end;
		line.isRealPath = realPath;

		m_ModeledLines.push_back(line);
	}

	PltPoint* GetPrimitivePointAt(unsigned long idx)
	{
		if((idx < 0)||(idx >= m_PrimitivePoints.size()))
		{
			TRACE("Debug: %s => Invalid index", __FUNCTION__);
			return NULL;
		}
		else return &m_PrimitivePoints.at(idx);
	}

	CPoint* GetWiredPointsAt(unsigned long idx)
	{
		if((idx < 0)||(idx >= m_WiredPoints.size()))
		{
			TRACE("Debug: %s => Invalid index", __FUNCTION__);
			return NULL;
		}
		else return &m_WiredPoints.at(idx);
	}

	PltLine* GetModeledLinetAt(unsigned long idx)
	{
		if((idx < 0)||(idx >= m_ModeledLines.size()))
		{
			TRACE("Debug: %s => Invalid index", __FUNCTION__);
			return NULL;
		}
		else return &m_ModeledLines.at(idx);
	}

	void RemovePrimitivePointAt(unsigned long idx)
	{
		if((idx < 0)||(idx >= m_PrimitivePoints.size()))
		{
			TRACE("Debug: %s => Invalid index", __FUNCTION__);
		}
		else m_PrimitivePoints.erase(m_PrimitivePoints.begin() + idx);
	}

	void RemoveWiredPointAt(unsigned long idx)
	{
		if((idx < 0)||(idx >= m_WiredPoints.size()))
		{
			TRACE("Debug: %s => Invalid index", __FUNCTION__);
		}
		else m_WiredPoints.erase(m_WiredPoints.begin() + idx);
	}

	void RemoveModeledLineAt(unsigned long idx)
	{
		if((idx < 0)||(idx >= m_ModeledLines.size()))
		{
			TRACE("Debug: %s => Invalid index", __FUNCTION__);
		}
		else m_ModeledLines.erase(m_ModeledLines.begin() + idx);
	}

	void ClearAllPrimitivePoints()
	{
		m_PrimitivePoints.clear();
	}

	void ClearAllWiredPoints()
	{
		m_WiredPoints.clear();
	}

	void ClearAllModeledLines()
	{
		m_ModeledLines.clear();
	}
};


#endif /*__PLOTTED_OBJECT__*/