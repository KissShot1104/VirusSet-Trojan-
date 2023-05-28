#pragma once
//#include"SniffingDeviceView.h"
#include"SniffingDeviceView.h"
#include"SniffingView.h"
SOCKET ssock;
SOCKET csocks;
SniffingDeviceView::SniffingDeviceView(QWidget* parent) : QDialog(parent),ui(new Ui::Dv_Dialog)
{

	ui->setupUi(this);
	QStringList* list = new QStringList;
	LPS* lp=new LPS;
	
	
	model = new QStringListModel(this);

	ui->Lv->setEditTriggers(QAbstractItemView::NoEditTriggers);
	lp->qdialog = this;
	lp->model = model;
	lp->ui = ui;
	SniffingDeviceViews((LPVOID)lp);
	lp->ui->Lv->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(lp->ui->Lv, SIGNAL(customContextMenuRequested(QPoint)), SLOT(CustomMenuSlots(QPoint)));

}

SniffingDeviceView::~SniffingDeviceView() 
{
	delete ui;
}
void SniffingDeviceViews(LPVOID param) 
{
	
	
	LPS* lp = (LPS*)param;
	
	ssock = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN saddr = { 0, };
	QString str;
	QStringList list;
	char buf[256] = { 0, };
	int size = 0;
	int count = 0;

	ssock = socket(AF_INET, SOCK_STREAM, 0);
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	saddr.sin_port = htons(SNIFFING_DEVICE_VIEW);

	bind(ssock, (LPSOCKADDR)&saddr, sizeof(saddr));
	listen(ssock, 64);

	/*while ((csocks = accept(ssock, NULL, NULL)) == INVALID_SOCKET)
		;*/
	csocks = accept(ssock, NULL, NULL);

	
	recv(csocks, (char*)&count, 4, 0);

	for (int i = 1; i < count; i++) 
	{

		recv(csocks, (char*)&size, 4, 0);
		recv(csocks, buf, size, 0);
		
		list << buf;
		ZeroMemory(&size, 4);
		ZeroMemory(buf, sizeof(buf));
	}
	lp->model->setStringList(list);
	lp->ui->Lv->setModel(lp->model);


	//lp->ui->Lv->setContextMenuPolicy(Qt::CustomContextMenu);
	//connect(lp->ui->Lv, SIGNAL(customContextMenuRequested(QPoint)), SLOT(CustomMenuSlots(QPoint)));
	//connect(lp->ui->Lv, SIGNAL(customContextMenuRequested(QPoint)) ,SLOT(CustomMenuSlots(QPoint)));

	
	//HANDLE thread = (HANDLE)_beginthreadex(NULL, NULL, Thread_Sniffing, NULL, NULL, NULL);
}

void SniffingDeviceView::CustomMenuSlots(QPoint pos)
{
	QModelIndex Mindex = ui->Lv->indexAt(pos);

	QMenu* menu = new QMenu(this);

	QAction* action1 = new QAction();
	action1->setStatusTip(tr("연결"));




	connect(action1, SIGNAL(triggered()), this, SLOT(context()));
	connect(action1, SIGNAL(triggered()), this, SLOT(PacketView()));
	menu->popup(ui->Lv->viewport()->mapToGlobal(pos));


	menu->addAction(action1);

	QPoint point;
	point = menu->mapToParent(pos);

	
}

void SniffingDeviceView::context()
{


	QModelIndexList list = ui->Lv->selectionModel()->selectedIndexes();
	QStringList slist;

	QString str;
	int i;
	i = list.begin()->row();//인덱스 값
	
	foreach(const QModelIndex Mindex, list)
	{
		//텍스트 형식으로 렌더링 할 키 데이터
		
		str = Mindex.data(Qt::DisplayRole).toString();

	}
	//QMessageBox::about(NULL, str, str);
	//char pname[100] = { 0, };
	//qsnprintf(pname, sizeof(pname), str.toUtf8().constData());
	send(csocks, (char*)&i,4, 0);
	//qDebug() << slist.join(",");
	
	closesocket(csocks);
	closesocket(ssock);
	
	//
	
	close();
	
	
}
void SniffingDeviceView::PacketView() 
{

	SniffingView* vui = new SniffingView;

	vui->show();
	vui->exec();


}
