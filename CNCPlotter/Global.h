#ifndef __GLOBAL_H__
#define __GLOBAL_H__

//structura pentru reprezentarea unui punct plotat
struct PltPoint{
	int xCoord;
	int yCoord;
	BOOL isPenDown;   //if the plotter cursor is down or up
};

//structura ptr reprezentarea unei linii plotate
struct PltLine{
	int x_start, y_start;  //coordinates of first point
	int x_end, y_end;    //coordinates of last point
	BOOL isRealPath;    //if the line is real (with plotter pen down) or virtual (transition line: plotter pen is up)
};

//---------------------------------------------
//			GRAPHICS CONSTANTS
//--------------------------------------------
#define WND_SCROLL_SIZE								   1000

//drawing context window limits
#define WND_AREA_LEFT									  0
#define WND_AREA_BOTTOM									  0
#define WND_AREA_RIGHT								   1000
#define WND_AREA_TOP								   1000

//viewport limits
#define VP_AREA_LEFT									200
#define VP_AREA_BOTTOM									 10		//este de fapt pozitia de sus
#define VP_AREA_RIGHT									700
#define VP_AREA_TOP									    510		//este de fapt pozitia de jos

//model transformation constants
#define MODEL_ZOOM_IN									  1
#define MODEL_NO_ZOOM									  0
#define MODEL_ZOOM_OUT									 -1

//zoom and translate factors
#define RIBBON_ZOOM_FACTOR							   1.02
#define RIBBON_TRANSLATE_VAL							  2

#define DC_BACKGROUND_COLOR		COLOR_GRADIENTACTIVECAPTION
#define DC_PEN_COLOR						   RGB(0, 0, 0)   //culoarea penului care deseneaza tot modelul
#define DC_PEN_WIDTH									  2   //dimensiunea penului culoarea penului care deseneaza tot modelul

#define DC_CURSOR_PEN_SIZE								  4   //dimensiunea cursorului care deseneaza in timp real modelul						

#endif /*__GLOBAL_H__*/