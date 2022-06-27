#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>

#include <stdlib.h>

#include "loginDAO.h"
#include "mainwindow.h"

namespace Ui {
	class Login;
}

class Login : public QDialog
{
	Q_OBJECT

public:
	explicit Login(QWidget* parent = nullptr);
	~Login();

private slots:
	void on_buttonBox_accepted();

	void on_Login_rejected();

private:
	Ui::Login* ui;
};

#endif // LOGIN_H
