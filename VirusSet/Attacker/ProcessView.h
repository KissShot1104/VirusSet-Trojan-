#pragma once
#include"ui_ProcessView.h"
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

//#define THREAD_COUNT 3
#ifdef PROCESSVIEW_H
#define PROCESSVIEW_H
#endif
#pragma comment(lib,"ws2_32.lib")
typedef struct threadparam 
{

	Ui::Pl_Dialog* ui;
	QStringListModel* model;
	QStringList* list;
}THREAD_PARAM;
class ProcessView : public QDialog
{
	Q_OBJECT

private:

	Ui::Pl_Dialog* ui = Q_NULLPTR;
	
public:
	virtual ~ProcessView();


	
	ProcessView(QWidget* parent = Q_NULLPTR);
	QStringListModel* model;
	SOCKET ssock;
	SOCKADDR_IN saddr;


public slots:
	void CustomMenuSlots(QPoint);
	void context();
	void test();
	
	//QStringListModel* model;


};
unsigned WINAPI ProcessListView(LPVOID);
