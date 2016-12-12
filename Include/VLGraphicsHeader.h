#if !defined (__VLG_GRAPHICS_HEADER__)
#define __VLG_GRAPHICS_HEADER__

#include <vector>

#define MAX_PUNCTE 10000
#define MAX_LINII  10000

class _declspec(dllimport)vlg_Matrice
{
public:
	float element[4][4];
	vlg_Matrice(void);
	~vlg_Matrice(void);
	void ElementUnitate()
	{
		for(int i=0;i<4;i++)
			for(int j=0;j<4;j++)
			{
				if(i==j) element[i][j]=1;
				else element[i][j]=0;
			}
	}
	void ElementNul()
	{
		for(int i=0;i<4;i++)
			for(int j=0;j<4;j++)
				element[i][j]=0;
	}
	vlg_Matrice& operator*(const vlg_Matrice& rv)
	{
		vlg_Matrice buf;
		for(int i=0;i<4;i++)
			for(int j=0;j<4;j++)
			{
				float t=0;
				for(int k=0;k<4;k++)
					t+=rv.element[i][k]*element[k][j];
				buf.element[i][j]=t;
			}
		for(int i=0;i<4;i++)
			for(int j=0;j<4;j++)
				element[i][j]=buf.element[i][j];
		return *this;
	}
};

class __declspec(dllimport)vlg_Viewport
{
public:
	//Viewport(void);
	//~Viewport(void);
	int left;
	int right;
	int down;
	int up;
};

class __declspec(dllimport)vlg_Window
{
public:
	//Window(void);
	//~Window(void);
	float stanga;
	float dreapta;
	float jos;
	float sus;
};

_declspec(dllimport)int vlg_iNumarPuncte;
_declspec(dllimport)int vlg_iNumarLinii;
_declspec(dllimport)int vlg_Linie[MAX_LINII][2];
_declspec(dllimport)float vlg_fsx;
_declspec(dllimport)float vlg_fsy;
_declspec(dllimport)int vlg_height;
_declspec(dllimport)int vlg_width;
_declspec(dllimport)int vlg_hv;
_declspec(dllimport)int vlg_lv;
_declspec(dllimport)float vlg_hw;
_declspec(dllimport)float vlg_lw;
_declspec(dllimport)vlg_Matrice vlg_Punct[MAX_PUNCTE];
_declspec(dllimport)vlg_Matrice vlg_Coordonate[MAX_PUNCTE];
_declspec(dllimport)vlg_Window vlg_cw;
_declspec(dllimport)vlg_Viewport vlg_cv;
__declspec(dllimport)void vlg_CitesteFisier(void); //Trece la ecran modelul din fisier
__declspec(dllimport)void vlg_CitesteVector(std::vector<CPoint> &puncte, std::vector<CPoint> &linii); //Trece la ecran modelul citit din vectorii dati ca parametru
__declspec(dllimport)void vlg_Desenare(CDC *pDC);//
// Realizeaza transformarea punctelor prin scalarea lor cu parametrii dati fsx si fsy
__declspec(dllimport)void vlg_Scalarea(float fsx, float fsy,float fsz,CDC *pDC);//
__declspec(dllimport)void vlg_Rotatia(double numarator, double numitor,CDC *pDC);//
__declspec(dllimport)void vlg_ForfecareOX(int shy, int shz,CDC *pDC);//
__declspec(dllimport)void vlg_ForfecareOY(int shx,int shz,CDC *pDC);//
__declspec(dllimport)void vlg_Translatia(int tx, int ty, int tz,CDC *pDC);//
__declspec(dllimport)void vlg_RecalculareDimensiuni(void);//
__declspec(dllimport)void vlg_SetCurrentWindow(float x1, float x2, float y1, float y2);//
__declspec(dllimport)void vlg_SetCurrentViewport(int x1, int x2, int y1, int y2);//
__declspec(dllimport)void vlg_W2V(float x, float y, float z);//
__declspec(dllimport)void vlg_Move_To(float x, float y, float z, CDC *pDC);//
__declspec(dllimport)void vlg_Line_To(float x, float y, float z, CDC* pDC);//
__declspec(dllimport)void vlg_TrasareViewport(CDC *pDC);//
__declspec(dllimport)void vlg_Vizualizare(CDC *pDC);//
__declspec(dllimport)void vlg_Pan1(float tx, float ty,CDC *pDC);//
__declspec(dllimport)void vlg_Pan2(float px, float py,CDC *pDC);//
__declspec(dllimport)void vlg_Zoom1(float cx, float cy, float fs,CDC *pDC);//
__declspec(dllimport)void vlg_RotatieOX(double numarator, double numitor,CDC *pDC);//
__declspec(dllimport)void vlg_RotatieOY(double numarator, double numitor,CDC *pDC);//
__declspec(dllimport)void vlg_ForfecareOZ(int shx, int shy,CDC *pDC);//
__declspec(dllimport)void vlg_Elevatie(float z0,CDC *pDC);//
__declspec(dllimport)void vlg_Plan(float y0,CDC *pDC);//
__declspec(dllimport)void vlg_Profil(float x0,CDC *pDC);//
__declspec(dllimport)void vlg_CavalieraX(double numarator, double numitor,CDC *pDC);//
__declspec(dllimport)void vlg_CavalieraY(double numarator, double numitor,CDC *pDC);//
__declspec(dllimport)void vlg_CavalieraZ(double numarator, double numitor,CDC *pDC);//
__declspec(dllimport)void vlg_CabinetX(double numarator, double numitor,CDC *pDC);//
__declspec(dllimport)void vlg_CabinetY(double numarator, double numitor,CDC *pDC);//
__declspec(dllimport)void vlg_CabinetZ(double numarator,double numitor,CDC *pDC);//
__declspec(dllimport)void vlg_PerspectiveX(float x0,CDC *pDC);//
__declspec(dllimport)void vlg_PerspectiveY(float y0,CDC *pDC);//
__declspec(dllimport)void vlg_PerspectiveZ(float z0,CDC *pDC);//



#endif  /*__VLG_GRAPHICS_HEADER__*/