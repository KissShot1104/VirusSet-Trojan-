#pragma once

#include "ui_SniffingView.h"
//#include<qdialog.h>
#include"dia.h"
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
#include<qmetaobject.h>
#include<qlistview.h>

#define PACKET_VIEW					7004
#pragma comment(lib,"ws2_32.lib")
typedef struct sv_threadparam
{

	Ui::Sv_Dialog* ui;
	QStringListModel* model;
	QStringList* list;
}SV_THREAD_PARAM;

class SniffingView : public QDialog
{

	Q_OBJECT


private:
	Ui::Sv_Dialog* ui = Q_NULLPTR;

public:
	virtual ~SniffingView();

	SniffingView(QWidget* parent = Q_NULLPTR);
	QStringListModel* model;


};

unsigned WINAPI Thread_SniffingView(LPVOID);

/*unsigned WINAPI Thread_Sniffing(LPVOID v)
{
	SniffingView* ui = new SniffingView;

	ui->show();
	ui->exec();
	return 0;
}*/
