#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QCloseEvent>
#include <QFile>
#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QTableView>

#include "SCIMSDAO.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();

private slots:
	void closeEvent(QCloseEvent*);

	void on_btn_search_clicked();

	void on_btn_add_inventory_clicked();

	void on_btn_add_order_clicked();

	void on_btn_add_destocking_clicked();

	void on_btn_mass_destocking_clicked();

	void on_btn_cancel_order_clicked();

	void on_btn_del_record_clicked();

	void on_btn_export_search_clicked();

	void on_btn_export_inventory_clicked();

	void on_btn_export_orders_clicked();

	void on_btn_export_destocking_clicked();

	void on_btn_export_low_clicked();

	void on_btn_export_expiring_clicked();

	void on_btn_reload_inventory_clicked();

	void on_btn_reload_orders_clicked();

	void on_btn_reload_destocking_clicked();

	void on_btn_reload_low_clicked();

	void on_btn_reload_expiring_clicked();

	void on_btn_save_inventory_clicked();

	void on_btn_save_orders_clicked();

	void on_btn_save_destocking_clicked();

	void on_btn_save_low_clicked();

	void on_btn_save_expiring_clicked();

    void on_btn_modify_clicked();

    void on_btn_change_price_clicked();

    void on_btn_change_password_clicked();

private:
	Ui::MainWindow* ui;
	SCIMSDAO dao;
	void load_data();
	bool validate(QVector<QString>);
	void export_(QTableView*);
    void is_expiring();
    void is_low();
};
#endif // MAINWINDOW_H
