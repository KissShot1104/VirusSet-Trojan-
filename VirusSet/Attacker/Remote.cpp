#include"Remote.h"


Remote::Remote(QWidget* parent) : QDialog(parent),ui(new Ui::Re_Dialog) {
	
	
	ui->setupUi(this);
	
	DISPLAYPARAM* dis = new DISPLAYPARAM;
	//QString img_path = "d:/test.jpeg";
	//QImage img(img_path);
	//QGraphicsScene* scene = new QGraphicsScene(ui->Gv);
	//QGraphicsScene scene;

	//QGraphicsItem* item = new QGraphicsItem(QPixmap::fromImage(img));
	//QGraphicsView* view = new QGraphicsView(scene);
	
	//QGraphicsPixmapItem item(QPixmap::fromImage(img));
	//QPixmap buf = QPixmap::fromImage(img);
	//scene.addPixmap(buf);
	//QPixmap buf = ("d:/test.jpg");
	QDesktopWidget desk;
	px_width = desk.width();
	px_height = desk.height();
	ui->LB->resize(px_width, px_height);
	//buf = buf.scaled(px_width - 50, px_height - 50);
	//buf = buf.scaled(50, 50);
	
	//ui->Gv->setGeometry(0, 0, px_width - 50, px_height - 50);
	//scene->addItem(item);
	//scene->addPixmap(buf);
	/*QDesktopWidget size;
	*pix = pix->scaled(size.width(), size.height());
	ui->Gv->setScene(scene);
	scene->addPixmap(*pix);*/
	

	
	
	dis->width = px_width;
	dis->height = px_height;
	dis->ui = ui;
	//dis->pix = pix;





	//la->show();
	thread[0] = (HANDLE)_beginthreadex(NULL, NULL, Thread_MouseMove, NULL, NULL, NULL); 
	thread[1] = (HANDLE)_beginthreadex(NULL, NULL, Thread_MouseClick, NULL, NULL,NULL);
	thread[2] = (HANDLE)_beginthreadex(NULL, NULL, Thread_DisplayView, (LPVOID)dis, NULL, NULL);

	//WaitForMultipleObjects(3, thread, TRUE, INFINITE);
	//WaitForMultipleObjects(3, thread, TRUE, INFINITE);
	/*for (int i = 0; i < 3; i++) {
		CloseHandle(thread[i]);
	}*/
	/*for (int i = 0; i < 2; i++)
		CloseHandle(thread[i]);*/

}

void MouseMove ()
{
		char* mouse_info = new char[sizeof(MOUSE_INFO)];
		SOCKET ssock;
		SOCKET csock;
		WSADATA wsadata;
		MOUSE_INFO mouse_infos = { 0, };
		POINT p = { 0, };

		if (WSAStartup(MAKEWORD(2, 2), &wsadata) != NULL)
			;// cout << "에러" << endl;

		SOCKADDR_IN saddr = { 0, };
		ssock = socket(AF_INET, SOCK_STREAM, 0);
		saddr.sin_family = AF_INET;
		saddr.sin_addr.s_addr = htonl(INADDR_ANY);
		saddr.sin_port = htons(7001);

		
		
		if (bind(ssock, (LPSOCKADDR)&saddr, sizeof(saddr)) == SOCKET_ERROR)
			;// cout << "bind Error" << endl;

		if (listen(ssock, 5) == SOCKET_ERROR)
			;// cout << "listen error" << " : " << WSAGetLastError() << endl;
		
		csock = accept(ssock, NULL, NULL);
		int err = WSAGetLastError();
		
		//cout << "mouse move connect" << endl;
		while (1) {


		//	ZeroMemory(&mouse_info, sizeof(MOUSE_INFO));

			GetCursorPos(&mouse_infos.xy);

			if (send(csock, (char*)&mouse_infos, sizeof(MOUSE_INFO), NULL) == SOCKET_ERROR)
			{
				//cout << WSAGetLastError() << endl;
			}
			
			Sleep(20);
		}
}
	//마우스 클릭

void MouseClick() 
{
	SOCKET ssock;
	SOCKET csock;
	WSADATA wsadata;
	MOUSE_INFO mouse_infos = { 0, };
	bool click = 0;

	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != NULL)
		;

	SOCKADDR_IN saddr = { 0, };
	ssock = socket(AF_INET, SOCK_STREAM, 0);
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	saddr.sin_port = htons(7002);

	if (bind(ssock, (LPSOCKADDR)&saddr, sizeof(saddr)) ==SOCKET_ERROR)
		;

	if (listen(ssock, 5) == SOCKET_ERROR)
		;

	csock = accept(ssock, NULL, NULL);
	
	while (1) {


		if (GetKeyState(VK_LBUTTON) < 0 && click == FALSE)
		{
			
			click = TRUE;
			mouse_infos.left_click_check = TRUE;
			mouse_infos.right_click_check = FALSE;
			mouse_infos.flag = 1;
			send(csock, (char*)&mouse_infos, sizeof(MOUSE_INFO), 0);
			while (!(GetKeyState(VK_LBUTTON) >= 0 && click == TRUE))
				Sleep(5);
			
			click = FALSE;
			mouse_infos.left_click_check = FALSE;
			mouse_infos.right_click_check = FALSE;
			send(csock, (char*)&mouse_infos, sizeof(MOUSE_INFO), 0);
		}//눌림
			//안눌림
		else if (GetKeyState(VK_RBUTTON) < 0 && click == FALSE)
		{
			//cout << "오른쪽 눌림" << endl;
			click = TRUE;
			mouse_infos.right_click_check = TRUE;
			mouse_infos.left_click_check = FALSE;
			mouse_infos.flag = 2;
			send(csock, (char*)&mouse_infos, sizeof(MOUSE_INFO), 0);
			while (!(GetKeyState(VK_RBUTTON) >= 0 && click == TRUE))
				Sleep(5);
			//cout << "오른쪽 안눌림" << endl;
			click = FALSE;
			mouse_infos.right_click_check = FALSE;
			mouse_infos.left_click_check = FALSE;
			send(csock, (char*)&mouse_infos, sizeof(MOUSE_INFO), 0);
		}//눌림
		//안눌림
		Sleep(5);
		
	}
}


unsigned WINAPI Thread_MouseMove(LPVOID v) 
{
	MouseMove();
	return 0;
}
unsigned WINAPI Thread_MouseClick(LPVOID v) 
{
	MouseClick();
	return 0;
}
unsigned WINAPI Thread_DisplayView(LPVOID v)
{
	
	DISPLAYPARAM* dis = (DISPLAYPARAM*)v;
	SOCKET ssock, csock;


	SOCKADDR_IN saddr = { 0, };
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	saddr.sin_port = htons(7000);
	ssock = socket(AF_INET, SOCK_STREAM, 0);
	bind(ssock, (LPSOCKADDR)&saddr, sizeof(saddr));
	listen(ssock, 64);
	csock = Remote_Accept(ssock);
	while (1)
	{
		//MessageBox(NULL, L"dd", L"ffff", MB_OK);
		char* buf;
		int total_size = 0;
		int size = 0;
		recv(csock, (char*)&total_size, 4, 0);
		buf = new char[total_size];
		ZeroMemory(buf, total_size);
		int recv_len = 0;
		HANDLE hfile = CreateFile(
			L"c:\\recv.jpeg",
			GENERIC_WRITE,
			FILE_SHARE_WRITE,
			NULL,
			CREATE_ALWAYS,
			NULL,
			NULL
		);
		DWORD write_size = 0;

		while (size < total_size) {
			recv_len = recv(csock, buf, total_size, 0);
			WriteFile(hfile, buf, recv_len, &write_size, NULL);
			size += recv_len;
		}
		CloseHandle(hfile);
		
		
		//QGraphicsScene* scene = new QGraphicsScene;
		//QGraphicsView* view = new QGraphicsView(scene);
		//QPixmap* pix = new QPixmap("c:\\recv.jpeg");
		
		QPixmap pix("c:\\recv.jpeg");
		dis->ui->LB->move(0, 0);
		dis->ui->LB->setPixmap(pix);
		//dis->ui->LB->close();
		delete buf;
	}


	return 0;
}

Remote::~Remote() 
{
	delete ui;
}
SOCKET Remote_Accept(SOCKET ssock)
{
	return accept(ssock, NULL, NULL);
}