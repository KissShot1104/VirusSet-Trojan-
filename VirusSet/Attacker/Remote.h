#ifdef REMOTE_H
#define REMOTE_H
#endif
#include"ui_Remote.h"
#include<process.h>
#include<qdialog.h>
#include<qdesktopwidget.h>
#include<qgraphicsscene.h>
#include<qpixmap.h>
#include<qimage.h>
#include<qgraphicsview.h>
#include<qslider.h>
#include<WinSock2.h>
#include<windows.h>
#include<iostream>
#include<qfiledialog.h>
#include<qlabel.h>
#include<qgraphicsitem.h>
#pragma comment(lib,"ws2_32.lib")




//using namespace std;

typedef struct mouse
{

	POINT xy;
	short left_click_check;
	short right_click_check;
	int flag;
}MOUSE_INFO, * LPMOUSE_INFO;


typedef struct displayparam 
{
	int width;
	int height;
	Ui::Re_Dialog* ui;
	QPixmap* pix;
}DISPLAYPARAM;

class Remote : public QDialog 
{

	Q_OBJECT

private:
	//Ui::Dialog* ui = new Ui::Dialog;
	Ui::Re_Dialog* ui = Q_NULLPTR;

	int px_width;
	int px_height;



public:
	
	
	//QGraphicsPixmapItem* item = new QGraphicsPixmapItem(*pix);
	QLabel* la = new QLabel;
	//QGraphicsScene scene;
	//QGraphicsView view(&scene);
	
	Remote(QWidget* parent = Q_NULLPTR);
	virtual ~Remote();
	//Ui::Dialog* ui = new Ui::Dialog;
	


public:
	//HANDLE thread[3];
	HANDLE thread[2];
};
void MouseClick();
void MouseMove();

unsigned WINAPI Thread_MouseMove(LPVOID);
unsigned WINAPI Thread_MouseClick(LPVOID);
unsigned WINAPI Thread_DisplayView(LPVOID);
SOCKET Remote_Accept(SOCKET ssock);