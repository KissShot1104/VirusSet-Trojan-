#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Attacker.h"
#include<qpixmap.h>
#include<qpushbutton.h>
#ifdef ATTACKER_H
#define ATTACKER_H
#endif
class Attacker : public QMainWindow
{
	Q_OBJECT

public:
	Attacker(QWidget *parent = Q_NULLPTR);
	virtual ~Attacker();
private:
	Ui::AttackerClass *ui = new Ui::AttackerClass;

public slots:
	void ShowProcess();
	void ShowDisplay();
	void ShowDevice();
	void ShowVictim();
	void ShowPacketControl();
};
