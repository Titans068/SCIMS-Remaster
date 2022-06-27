#include "login.h"
#include "ui_login.h"
#include "SCIMSDAO.h"

Login::Login(QWidget* parent) :
	QDialog(parent),
	ui(new Ui::Login)
{
	ui->setupUi(this);
	ui->username->setFocus();
}

Login::~Login()
{
	delete ui;
}

void Login::on_buttonBox_accepted()
{
    if (LoginDAO().login(ui->password->text()) && ui->username->text() == "admin") {
        QMessageBox::information(nullptr, this->windowTitle(), "Welcome admin");
        //this->close();
        this->done(1);
    }
	else {
		QMessageBox::critical(nullptr,
			this->windowTitle(),
			"Incorrect credentials provided.");
        this->done(0);
	}
}


void Login::on_Login_rejected()
{
	exit(0);
}

