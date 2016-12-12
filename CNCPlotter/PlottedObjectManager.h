#if !defined (__PLOTTED_OBJECT_MANAGER__)
#define __PLOTTED_OBJECT_MANAGER__

#include "PlottedObject.h"
#include "VLGraphicsHeader.h"


//managerul obiectului plotat pune la dispozitie diverse functii de manipulare ale punctelor/liniilor obiectului
class CPlottedObjectManager
{
private:
	CPlottedObject *m_PlottedObject;

public:

	CPlottedObjectManager()
	{
		m_PlottedObject = new CPlottedObject();
	}

	~CPlottedObjectManager(void)
	{
		delete m_PlottedObject;
	}

	//return a pointer to the plotted object
	CPlottedObject *GetPlottedObject()
	{
		return m_PlottedObject;
	}

	//parse a string content given in PLT/HPGL format and construct the m_PlottedObject obj
	BOOL ParsePltFile(CString &content)
	{
		int xCoord, yCoord;
		int idx = 0;
		int lineEndIdx = 0;
		BOOL isPenDown = FALSE;
	
		while((idx >= 0)&&(lineEndIdx >= 0))
		{
			int puIdx = content.Find("PU", idx);
			int pdIdx = content.Find("PD", idx);
			if((puIdx < 0) && (pdIdx >= 0))			//no PU command was found, only PD
			{
				idx = pdIdx;
				isPenDown = TRUE;
			}
			else if((puIdx >= 0) && (pdIdx < 0))		//no PD command was found, only PU
			{
				idx = puIdx;
				isPenDown = FALSE;
			}
			else if((puIdx >= 0) && (pdIdx >= 0))
			{
				if((puIdx < pdIdx))
				{
					idx = puIdx;
					isPenDown = FALSE;
				}
				else {
					idx = pdIdx;
					isPenDown = TRUE;
				}
			}
			else
			{
				return FALSE;
			}
			
			lineEndIdx = content.Find(";", idx);								//find index of semicolon
			CString coords = content.Mid(idx + 2, lineEndIdx - idx - 2);		//extract the coordinates part
			
			//start tokenizing coordinates
			CString resToken;
			int curPos = 0;
			resToken = coords.Tokenize(_T(" "), curPos);
			xCoord = atoi(resToken);
			resToken = coords.Tokenize(_T(" "), curPos);
			yCoord = atoi(resToken);
			
			idx = lineEndIdx;
			
			m_PlottedObject->AddPrimitivePoint(xCoord, yCoord, isPenDown);
			//TRACE("\nPoint added: (%d, %d, %d)", xCoord, yCoord, isPenDown);
			TRACE("\n%d %d %d", xCoord, yCoord, isPenDown);
	   }
	
	   return TRUE;
	}

	BOOL ReadFileFromDisk(CFile *pFile)
	{
		try
		{
		    CString mainstr = _T("");
		    char buffer[1024];
		    UINT unRead;
		
		    unRead = pFile->Read(buffer, sizeof(buffer)-1);
		    while(unRead)
		    {
		        buffer[unRead] = '\0';
	
		        mainstr += buffer;
		        unRead = pFile->Read(buffer, sizeof(buffer)-1);
		    }
	
			//parse the input file and construct the array of points
			this->ParsePltFile(mainstr);
			 
			//compute point wirings and lines
			ComputeWiredPointsFromPrimitivePoints();
	
			for(int i = 0; i < m_PlottedObject->GetWiredPointsCount(); i++)
			{
				TRACE("\n%d %d",m_PlottedObject->GetWiredPointsAt(i)->x,m_PlottedObject->GetWiredPointsAt(i)->y);
			}
	
		}
		catch(CException* e)
		{
		    AfxMessageBox("Error - unable to open file");
		    e->Delete();
	
			return FALSE;
		}
	
		return TRUE;
	}


	//calculez liniile dintre puncte, pe baza Pen Up si Pen Down
	void ComputeWiredPointsFromPrimitivePoints()
	{
		for(int i = 0; i < m_PlottedObject->GetPrimitivePointsCount() - 1; i++)
		{
			if((m_PlottedObject->GetPrimitivePointAt(i)->isPenDown == TRUE) && (m_PlottedObject->GetPrimitivePointAt(i+1)->isPenDown == FALSE))
			{
				//punctul care aterizeaza in PU (adica pct anterior lui are PD) nu-l leg de anteriorul
				//deasemena, punctul in cazua nu trebuie sa fie primul din lista
			}
			else
			{
				  m_PlottedObject->AddWiredPoints(i+1, i+2);
			}
		}

	}

	//calculez "minimum bounding box", adica dreptunghiul care incadreaza perfect textul (modelul)
	void GetBoundingBoxOfPrimitivePoints(CRect &outRect)
	{
		int min_x = INT_MAX;
		int min_y = INT_MAX;
		int max_x = -1;
		int max_y = -1;

		for(int i=0; i<m_PlottedObject->GetPrimitivePointsCount(); i++)
		{
			if(m_PlottedObject->GetPrimitivePointAt(i)->xCoord > max_x) max_x = m_PlottedObject->GetPrimitivePointAt(i)->xCoord;
			if(m_PlottedObject->GetPrimitivePointAt(i)->xCoord < min_x) min_x = m_PlottedObject->GetPrimitivePointAt(i)->xCoord;
			if(m_PlottedObject->GetPrimitivePointAt(i)->yCoord > max_y) max_y = m_PlottedObject->GetPrimitivePointAt(i)->yCoord;
			if(m_PlottedObject->GetPrimitivePointAt(i)->yCoord < min_y) min_y = m_PlottedObject->GetPrimitivePointAt(i)->yCoord;
		}

		outRect.SetRect(min_x, max_y, max_x, min_y);
	}

	//verific daca dreptunghiul minim care include modelul este inclus in viewport
	BOOL IsBoundingBoxOfModeledPointsInsideViewport()
	{
		int min_x = INT_MAX;
		int min_y = INT_MAX;
		int max_x = -1;
		int max_y = -1;

		for(int i=0; i<m_PlottedObject->GetModeledLinesCount(); i++)
		{
			if(m_PlottedObject->GetModeledLinetAt(i)->x_end > max_x) max_x = m_PlottedObject->GetModeledLinetAt(i)->x_end;
			if(m_PlottedObject->GetModeledLinetAt(i)->x_end < min_x) min_x = m_PlottedObject->GetModeledLinetAt(i)->x_end;
			if(m_PlottedObject->GetModeledLinetAt(i)->y_end > max_y) max_y = m_PlottedObject->GetModeledLinetAt(i)->y_end;
			if(m_PlottedObject->GetModeledLinetAt(i)->y_end < min_y) min_y = m_PlottedObject->GetModeledLinetAt(i)->y_end;
		}

		if((max_x > VP_AREA_RIGHT)||(min_x < VP_AREA_LEFT)||(max_y > VP_AREA_TOP)||(min_y < VP_AREA_BOTTOM))
			return FALSE;

		return TRUE;
	}

	//calculeaza vectorul de linii ale modelului grafic
	void ComputeModeledLinesFromGraphics()
	{
		//if vector is not empty (already computted), clear the vector
		if(m_PlottedObject->GetModeledLinesCount() > 0)
			m_PlottedObject->ClearAllModeledLines();

		BOOL realLine = FALSE;
		int x_start, y_start, x_end, y_end;

		//first, just add the origin (for the pen starting point)
		vlg_W2V(0, 0, 0);
		x_start = vlg_width;
		y_start = vlg_height;
		vlg_W2V(vlg_Coordonate[0].element[0][0], vlg_Coordonate[0].element[1][0], 0);
		x_end = vlg_width;
		y_end = vlg_height;
		m_PlottedObject->AddModeledLine(x_start, y_start, x_end, y_end, realLine);
		
		//add the first point
		vlg_W2V(vlg_Coordonate[0].element[0][0], vlg_Coordonate[0].element[1][0], 0);
		x_start = vlg_width;
		y_start = vlg_height;
		vlg_W2V(vlg_Coordonate[1].element[0][0], vlg_Coordonate[1].element[1][0], 0);
		x_end = vlg_width;
		y_end = vlg_height;
		m_PlottedObject->AddModeledLine(x_start, y_start, x_end, y_end, TRUE);

		for(int i = 0; i < m_PlottedObject->GetPrimitivePointsCount() - 1; i++)
		{
			if(( (i+2 < m_PlottedObject->GetPrimitivePointsCount()) && (m_PlottedObject->GetPrimitivePointAt(i)->isPenDown == TRUE) && (m_PlottedObject->GetPrimitivePointAt(i+1)->isPenDown == TRUE) && (m_PlottedObject->GetPrimitivePointAt(i+2)->isPenDown == FALSE)) || i == m_PlottedObject->GetPrimitivePointsCount() - 2)
			{
				//punctul care aterizeaza in PU (adica pct anterior lui are PD) nu-l leg de anteriorul
				//deasemena, punctul in cauza nu trebuie sa fie primul din lista
				//daca punctul e ultimul, se va intoarce la origine ca linie virtuala
				realLine = FALSE;
			}
			else
			{
				realLine = TRUE;
			}

			vlg_W2V(vlg_Coordonate[i+1].element[0][0], vlg_Coordonate[i+1].element[1][0], 0);
			x_start = vlg_width;
			y_start = vlg_height;
			vlg_W2V(vlg_Coordonate[i+2].element[0][0], vlg_Coordonate[i+2].element[1][0], 0);
			x_end = vlg_width;
			y_end = vlg_height;
			m_PlottedObject->AddModeledLine(x_start, y_start, x_end, y_end, realLine);
		}

		/*for(int i=0; i<vlg_iNumarLinii; i++)
		{
		  vlg_W2V(float(vlg_Coordonate[vlg_Linie[i][0]-1].element[0][0]), float(vlg_Coordonate[vlg_Linie[i][0]-1].element[1][0]), 0);
		  x_start = vlg_width;
		  y_start = vlg_height;
		  vlg_W2V(float(vlg_Coordonate[vlg_Linie[i][1]-1].element[0][0]), float(vlg_Coordonate[vlg_Linie[i][1]-1].element[1][0]), 0);
		  x_end = vlg_width;
		  y_end = vlg_height;

		  m_PlottedObject->AddModeledLine(x_start, y_start, x_end, y_end, realLine);
		}*/
	}
};

#endif /*__PLOTTED_OBJECT_MANAGER__*/