#include"ProcessView.h"
SOCKET csock;
ProcessView::ProcessView(QWidget* parent) : QDialog(parent),ui(new Ui::Pl_Dialog)
{
	THREAD_PARAM* tp = new THREAD_PARAM;
	HANDLE thread;
	ui->setupUi(this);

	QStringList *list = new QStringList;

	model = new QStringListModel(this);

	ui->Lv->setContextMenuPolicy(Qt::CustomContextMenu);

	tp->ui = ui;
	tp->model = model;
	tp->list = list;
	thread = (HANDLE)_beginthreadex(NULL, NULL, ProcessListView, (LPVOID)tp, NULL,NULL);
	
	//list << "Clair de lune" << "Reverie" << "prelude";

	//model->setStringList(list);

	//ui->Lv->setModel(model);

	//model->columnCount();

	//int col = model->columnCount();

	
	//ui->Lv->setModelColumn(3);
	


	//ui->Lv->setSelectionMode(QAbstractItemView::NoSelection);//못건드림
	//ui->Lv->setEditTriggers(QAbstractItemView::AnyKeyPressed| QAbstractItemView::DoubleClicked);//리스트 구정 가능
	ui->Lv->setEditTriggers(QAbstractItemView::NoEditTriggers);

	connect(ui->Lv, SIGNAL(customContextMenuRequested(QPoint)), SLOT(CustomMenuSlots(QPoint)));
	//connect(ui->ADD, SIGNAL(clicked()), this, SLOT(push_add_click1()));
	WaitForSingleObject(thread, INFINITY);
	
	CloseHandle(thread);
}

unsigned WINAPI ProcessListView(LPVOID param) 
{

	SOCKET ssock;
	SOCKADDR_IN saddr = { 0, };
	char buf[100] = { 0, };
	
	ssock = socket(AF_INET, SOCK_STREAM, 0);
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(7003);
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(ssock, (LPSOCKADDR)& saddr, sizeof(saddr));
	listen(ssock, 64);
	csock = accept(ssock, NULL, NULL);
	THREAD_PARAM* tp = (THREAD_PARAM*)param;
	int size = 0;
	int i = 0;
	QStringList list;
	while (1) {
		while (1) 
		{
			ZeroMemory(buf, sizeof(buf));
			ZeroMemory(&size, 4);
			recv(csock, (char*)& size, 4, 0);
			if (size == EOF)
				break;
			recv(csock, buf, size, 0);
			size = 0;
			list << buf;
			
		}
		tp->model->setStringList(list);
		tp->ui->Lv->setModel(tp->model);
		list.clear();
		
	}


	closesocket(ssock);
	closesocket(csock);
	return 0;
}

void ProcessView::CustomMenuSlots(QPoint pos)
{
	QModelIndex Mindex = ui->Lv->indexAt(pos);

	QMenu* menu = new QMenu(this);

	QAction* action1 = new QAction();

	action1->setStatusTip(tr("KILL"));



	connect(action1, SIGNAL(triggered()), this, SLOT(context()));

	menu->popup(ui->Lv->viewport()->mapToGlobal(pos));


	menu->addAction(action1);

	QPoint point;
	point = menu->mapToParent(pos);
	


}


ProcessView::~ProcessView()
{
	delete ui;
}


/*void ProcessView::push_add_click1()
{

	int row = model->rowCount();

	model->insertRows(row, 1);
	QModelIndex index = model->index(row);


	ui->Lv->setCurrentIndex(index);
	ui->Lv->edit(index);

}*/
void ProcessView::test()
{
	QMessageBox::aboutQt(NULL);
}
void ProcessView::context()
{


	QModelIndexList list = ui->Lv->selectionModel()->selectedIndexes();
	QStringList slist;

	QString str;

	foreach(const QModelIndex Mindex, list)
	{
		//텍스트 형식으로 렌더링 할 키 데이터
		str = Mindex.data(Qt::DisplayRole).toString();
		
	}
	QMessageBox::about(NULL, str, str);
	char pname[100] = { 0, };
	qsnprintf(pname, sizeof(pname), str.toUtf8().constData());
	if (send(csock, pname, strlen(pname), 0) == SOCKET_ERROR)
		;
	//qDebug() << slist.join(",");
}