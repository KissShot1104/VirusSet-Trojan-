#include"SniffingView.h"



SniffingView::SniffingView(QWidget* parent) : QDialog(parent), ui(new Ui::Sv_Dialog)
{
	ui->setupUi(this);
	HANDLE thread = NULL;
	SV_THREAD_PARAM* tp = new SV_THREAD_PARAM;
	QStringList* list = new QStringList;

	model = new QStringListModel(this);

	tp->ui = ui;
	tp->list = list;
	tp->model = model;

	thread = (HANDLE)_beginthreadex(NULL, NULL, Thread_SniffingView, (LPVOID)tp, NULL, NULL);

	//WaitForSingleObject(thread, INFINITE);


}

SniffingView::~SniffingView()
{

	delete ui;
	delete model;

}

unsigned WINAPI Thread_SniffingView(LPVOID param)
{
	HANDLE hfile=NULL;
	hfile = CreateFile(L"c:\\packet.txt", GENERIC_WRITE,FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS | CREATE_ALWAYS,NULL,NULL);
	SetFilePointer(hfile, 0, NULL, FILE_END);
	SV_THREAD_PARAM* tp = (SV_THREAD_PARAM*)param;
	DWORD write_size;
	SOCKET ssock, csock;
	SOCKADDR_IN saddr, caddr;
	ZeroMemory(&saddr, sizeof(saddr));
	QString str;
	QStringList list;
	char* packet_buf = nullptr;
	
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	saddr.sin_port = htons(PACKET_VIEW);

	ssock = socket(AF_INET, SOCK_STREAM, 0);
	bind(ssock, (LPSOCKADDR)&saddr, sizeof(saddr));
	listen(ssock, 5);
	int error = 0;
	while ((csock = accept(ssock, NULL, NULL)) == SOCKET_ERROR)
		error = WSAGetLastError();
	tp->ui->Lv->setModel(tp->model);
	tp->model->setStringList(list);
	while (1) {
		for (int i=0;i<200;i++)
		{
			
			int size = 0;
			char* log_file;
			
			//int row = tp->model->rowCount();
			//int row = tp->model->rowCount();

			//tp->model->insertRows(row, 1);
		/////	///QModelIndex index = tp->model->index(row);

			//tp->ui->Lv->setCurrentIndex(index);
			//tp->ui->Lv->edit(index);

			if (recv(csock, (char*)&size, 4, 0) == SOCKET_ERROR)
				return 1;

			packet_buf = new char[size];
			ZeroMemory(packet_buf, size);

			if (recv(csock, packet_buf, size, 0) == SOCKET_ERROR)
				return 1;


			if (recv(csock, (char*)&size, 4, 0) == SOCKET_ERROR)
				return 1;
			log_file = new char[size];
			ZeroMemory(log_file, size);

			if (recv(csock, log_file, size, 0) == SOCKET_ERROR)
				return 1;
			
			WriteFile(hfile, log_file, size, &write_size, NULL);

			//tp->list << packet_buf;
			//tp->model->insertRows(row, 1);
			//QModelIndex index = tp->model->index(row);


			//tp->ui->Lv->setCurrentIndex(index);
			//tp->ui->Lv->edit(index);
			list << packet_buf;

			
			//tp->model->setStringList(list);
			delete log_file;
			delete packet_buf;
		}
		tp->model->setStringList(list);
		list.clear();
	}

	return 0;
}