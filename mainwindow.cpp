#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "login.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dao.init_connection();
    load_data();
    connect(ui->btn_quit, &QPushButton::clicked, this, &MainWindow::close);
}

void MainWindow::load_data() {
    dao.set_query(dao.INVENTORY);
    dao.read(ui->tableView_2);
    dao.set_query(dao.ORDERS);
    dao.read(ui->tableView_3);
    dao.set_query(dao.DESTOCKED);
    dao.read(ui->tableView_4);
    dao.set_query(dao.LOW_STOCK);
    dao.read(ui->tableView_7);
    dao.set_query(dao.EXPIRING);
    dao.read(ui->tableView_8);
    this->is_expiring();
    this->is_low();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::validate(QVector<QString> q) {
    foreach(QString str, q) {
        if (str.trimmed().isEmpty() || str.trimmed().isNull()) {
            QMessageBox::critical(this, this->windowTitle(), "Empty fields are not accepted.");
            return false;
        }
    }
    return true;
}

void MainWindow::export_(QTableView* table) {
    QString file_name = QFileDialog::getSaveFileName(this,
                                                     "Save File",
                                                     QDir::currentPath(),
                                                     "Comma Separated Value File (*.csv)"), file_data;
    auto model = table->model();
    if (!file_name.isNull() || !file_name.isEmpty()) {
        int row = model->rowCount(), col = model->columnCount();
        for (int i = 0; i < col; ++i) {
            file_data += model->headerData(i, Qt::Horizontal).toString();
            file_data += ", ";
        }

        file_data += "\n";
        for (int i = 0; i < row; ++i) {
            for (int j = 0; j < col; ++j) {
                file_data += table->model()->data(table->model()->index(i, j)).toString();
                file_data += ", ";
            }
            file_data += "\n";
        }
        QFile file(file_name);
        if (file.open(QFile::WriteOnly | QFile::Truncate)) {
            QTextStream data(&file);
            data << file_data;
            file.close();
            QMessageBox::information(this, this->windowTitle(), "Records exported successfully.");
        }
    }
}

void MainWindow::is_expiring() {
    int exp;
    if ((exp = dao.expiring()) > 0)
        QMessageBox::warning(this, this->windowTitle(), QString("%1 item(s) are almost expiring. "
                                                                "View them in the 'Display records' section.").arg(exp));
}

void MainWindow::is_low() {
    int low;
    if ((low = dao.low()) > 0)
        QMessageBox::warning(this, this->windowTitle(), QString("%1 item(s) need to be restocked. "
                                                                "View them in the 'Display records' section.").arg(low));
}

void MainWindow::closeEvent(QCloseEvent* event) {
    if (QMessageBox::question(this, this->windowTitle(), "Are you sure you want to quit?",
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) QApplication::quit();
    else event->ignore();
}

void MainWindow::on_btn_search_clicked()
{
    dao.set_query(dao.SEARCH);
    dao.read(ui->tableView, ui->lineEdit_2);
}


void MainWindow::on_btn_add_inventory_clicked()
{
    dao.set_query(dao.INVENTORY);
    QVector<QString> q = { ui->category->text(),ui->name->text(),ui->category->text().at(0) + QDate::currentDate().toString("yyyyMMdd")
                           + QTime::currentTime().toString("HHmmss"),
                           QString(ui->price->value()),QString(ui->quantity->value()), QString(ui->required->value()),
                           ui->expiry->date().toString("yyyy-MM-dd") };
    if (validate(q))
    {
        if (dao.insert(q))
            QMessageBox::information(this, this->windowTitle(), "Successful data entry.");
        else QMessageBox::critical(this, this->windowTitle(), "An error occurred. Data entry unsuccessful.");
    }
}


void MainWindow::on_btn_add_order_clicked()
{
    dao.set_query(dao.ORDERS);
    QVector<QString> q = { "P" + QDate::currentDate().toString("yyyymmdd") + QTime::currentTime().toString("HHmmss"),ui->code1->text(),
                           ui->supplier->text(),QString(ui->quantityPurchased->value()),ui->datePurchase->date().toString("yyyy-MM-dd") };
    if (validate(q))
    {
        if (dao.insert(q))
            QMessageBox::information(this, this->windowTitle(), "Successful data entry.");
        else QMessageBox::critical(this, this->windowTitle(), "An error occurred. Data entry unsuccessful.");
    }
}


void MainWindow::on_btn_add_destocking_clicked()
{
    dao.set_query(dao.DESTOCKED);
    QVector<QString> q = { "I" + QDate::currentDate().toString("yyyymmdd") + QTime::currentTime().toString("HHmmss"),
                           ui->code2->text(),ui->destockedTo->text(),QString(ui->quantityDestocked->value()),
                           ui->dateDestocked->date().toString("yyyy-MM-dd") };
    if (validate(q))
    {
        if (dao.insert(q))
            QMessageBox::information(this, this->windowTitle(), "Successful data entry.");
        else QMessageBox::critical(this, this->windowTitle(), "An error occurred. Data entry unsuccessful.");
    }

}


void MainWindow::on_btn_mass_destocking_clicked()
{
    dao.set_query(dao.MASS);
    QVector<QString> q = { ui->code5->text(),QString(ui->quantityDestocked1->value()) };
    if (validate(q)) {
        if (dao.update(q))
            QMessageBox::information(this, this->windowTitle(), "Successful data entry.");
        else QMessageBox::critical(this, this->windowTitle(), "An error occurred. Data entry unsuccessful.");
    }
}


void MainWindow::on_btn_cancel_order_clicked()
{
    dao.set_query(dao.ORDERS);
    QVector<QString> q = { ui->cancel_id->text() };
    if (validate(q)) {
        if (QMessageBox::question(this, this->windowTitle(), "Are you sure you want to cancel this order?",
                                  QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes){
            if (dao.delete_(q))
                QMessageBox::information(this, this->windowTitle(), "Successful data deletion.");
            else QMessageBox::critical(this, this->windowTitle(), "An error occurred. Data entry unsuccessful.");
        }
    }
}


void MainWindow::on_btn_del_record_clicked()
{
    dao.set_query(dao.DELETE);
    QVector<QString> q = { ui->code4->text() };
    if (validate(q)) {
        if (QMessageBox::question(this, this->windowTitle(), "Are you sure you want to delete this record?",
                                  QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes){
            if (dao.delete_(q))
                QMessageBox::information(this, this->windowTitle(), "Successful data deletion.");
            else QMessageBox::critical(this, this->windowTitle(), "An error occurred. Data entry unsuccessful.");
        }
    }
}


void MainWindow::on_btn_modify_clicked()
{
    dao.set_query(dao.DELIVERY);
    QVector<QString> q={QString(ui->quantityReceived->value()),ui->code6->text(),ui->dateReceived->date().toString("yyyy-MM-dd")};
    if (validate(q)) {
        if (dao.update(q))
            QMessageBox::information(this, this->windowTitle(), "Successful data entry.");
        else QMessageBox::critical(this, this->windowTitle(), "An error occurred. Data entry unsuccessful.");
    }
}


void MainWindow::on_btn_change_price_clicked()
{
    dao.set_query(dao.PRICE);
    QVector<QString> q={ui->code3->text(), QString(ui->price1->value())};
    if (validate(q)) {
        if (dao.update(q))
            QMessageBox::information(this, this->windowTitle(), "Successful data modification.");
        else QMessageBox::critical(this, this->windowTitle(), "An error occurred. Data modification unsuccessful.");
    }

}


void MainWindow::on_btn_change_password_clicked()
{
    dao.set_query(dao.PASSWORD);
    QVector<QString> q={ui->oldPassword->text(),ui->newPassword->text(),ui->newPassword2->text()};
    if (validate(q)) {
        if (dao.update(q))
            QMessageBox::information(this, this->windowTitle(), "Successful password modification.");
        else QMessageBox::critical(this, this->windowTitle(), "An error occurred. Password modification unsuccessful.");
    }

}


void MainWindow::on_btn_export_search_clicked()
{
    if (ui->tableView->model()->rowCount() > 0) export_(ui->tableView);
    else QMessageBox::critical(this, this->windowTitle(), "No records to export.");
}


void MainWindow::on_btn_export_inventory_clicked()
{
    if (ui->tableView_2->model()->rowCount() > 0) export_(ui->tableView_2);
    else QMessageBox::critical(this, this->windowTitle(), "No records to export.");
}


void MainWindow::on_btn_export_orders_clicked()
{
    if (ui->tableView_3->model()->rowCount() > 0) export_(ui->tableView_3);
    else QMessageBox::critical(this, this->windowTitle(), "No records to export.");
}


void MainWindow::on_btn_export_destocking_clicked()
{
    if (ui->tableView_4->model()->rowCount() > 0) export_(ui->tableView_4);
    else QMessageBox::critical(this, this->windowTitle(), "No records to export.");
}


void MainWindow::on_btn_export_low_clicked()
{
    if (ui->tableView_7->model()->rowCount() > 0) export_(ui->tableView_7);
    else QMessageBox::critical(this, this->windowTitle(), "No records to export.");
}


void MainWindow::on_btn_export_expiring_clicked()
{
    if (ui->tableView_8->model()->rowCount() > 0) export_(ui->tableView_8);
    else QMessageBox::critical(this, this->windowTitle(), "No records to export.");
}


void MainWindow::on_btn_reload_inventory_clicked()
{
    dao.set_query(dao.INVENTORY);
    dao.read(ui->tableView_2);
}


void MainWindow::on_btn_reload_orders_clicked()
{
    dao.set_query(dao.ORDERS);
    dao.read(ui->tableView_3);
}


void MainWindow::on_btn_reload_destocking_clicked()
{
    dao.set_query(dao.DESTOCKED);
    dao.read(ui->tableView_4);
}


void MainWindow::on_btn_reload_low_clicked()
{
    dao.set_query(dao.LOW_STOCK);
    dao.read(ui->tableView_7);
}


void MainWindow::on_btn_reload_expiring_clicked()
{
    dao.set_query(dao.EXPIRING);
    dao.read(ui->tableView_8);
}


void MainWindow::on_btn_save_inventory_clicked()
{
    if (dao.save(ui->tableView_2)) QMessageBox::information(this, this->windowTitle(), "Records saved successfully.");
    else QMessageBox::critical(this, this->windowTitle(), "An error occurred while saving records.");
}


void MainWindow::on_btn_save_orders_clicked()
{
    if (dao.save(ui->tableView_3)) QMessageBox::information(this, this->windowTitle(), "Records saved successfully.");
    else QMessageBox::critical(this, this->windowTitle(), "An error occurred while saving records.");
}


void MainWindow::on_btn_save_destocking_clicked()
{
    if (dao.save(ui->tableView_4)) QMessageBox::information(this, this->windowTitle(), "Records saved successfully.");
    else QMessageBox::critical(this, this->windowTitle(), "An error occurred while saving records.");
}


void MainWindow::on_btn_save_low_clicked()
{
    if (dao.save(ui->tableView_7)) QMessageBox::information(this, this->windowTitle(), "Records saved successfully.");
    else QMessageBox::critical(this, this->windowTitle(), "An error occurred while saving records.");
}


void MainWindow::on_btn_save_expiring_clicked()
{
    if (dao.save(ui->tableView_8)) QMessageBox::information(this, this->windowTitle(), "Records saved successfully.");
    else QMessageBox::critical(this, this->windowTitle(), "An error occurred while saving records.");
}

