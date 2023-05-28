#pragma once

#include"ui_SniffingDeviceView.h"
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

#define SNIFFING_DEVICE_VIEW		7005
#pragma comment(lib,"ws2_32.lib")


typedef struct lp
{

	QStringListModel* model;
	Ui::Dv_Dialog* ui;
	QDialog* qdialog;
	
}LPS;

class SniffingDeviceView : public QDialog
{

	Q_OBJECT

private:
	Ui::Dv_Dialog* ui = Q_NULLPTR;

public:
	virtual ~SniffingDeviceView();

	SniffingDeviceView(QWidget* parent = Q_NULLPTR);

	QStringListModel* model;

	

public slots:

	void CustomMenuSlots(QPoint pos);
	void context();
	void PacketView();

};

void SniffingDeviceViews(LPVOID param);


