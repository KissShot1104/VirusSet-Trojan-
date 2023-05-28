#include"Attacker.h"
#include"ProcessView.h"
#include"Remote.h"
#include"SniffingDeviceView.h"
#include"VictimList.h"
#include"PacketSignal.h"
Attacker::Attacker(QWidget *parent)
	: QMainWindow(parent)
{
	ui->setupUi(this);
	//ui->REMOTE->setIcon(QIcon("c:\\television.png"));
	/*QPixmap pix("c:/television.png");
	QIcon icon;
	icon.addPixmap(pix, QIcon::Normal, QIcon::Off);

	ui->REMOTE->setIcon(icon);
	ui->REMOTE->setIconSize(QSize(100,100));*/
	connect(ui->PROCESSVIEW, SIGNAL(clicked()), this, SLOT(ShowProcess()));
	connect(ui->REMOTE, SIGNAL(clicked()), this, SLOT(ShowDisplay()));
	connect(ui->SNIFFING_DEVICE_VIEWS, SIGNAL(clicked()), this, SLOT(ShowDevice()));
	connect(ui->VICTIMLIST, SIGNAL(clicked()), this, SLOT(ShowVictim()));
	connect(ui->PACKETCONTROL, SIGNAL(clicked()), this, SLOT(ShowPacketControl()));
}

Attacker::~Attacker()
{
	delete ui;
}

void Attacker::ShowVictim() 
{

	VictimList* ui = new VictimList;

	ui->show();
	ui->exec();

}

void Attacker::ShowProcess()
{

	ProcessView* ui = new ProcessView;

	ui->show();
	ui->exec();

}
void Attacker::ShowDisplay() 
{

	Remote* ui = new Remote;

	
	
	//ui->setFixedWidth(desk.width());
	//ui->setFixedHeight(desk.height());
	
	ui->showFullScreen();
	ui->exec();

}

void Attacker::ShowDevice() 
{

	SniffingDeviceView* ui = new SniffingDeviceView;

	ui->show();
	ui->exec();

	

}
void Attacker::ShowPacketControl() 
{

	PacketSignal* ui = new PacketSignal;

	ui->show();
	ui->exec();

}
