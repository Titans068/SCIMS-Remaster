#include "SCIMSDAO.h"

SCIMSDAO::Query SCIMSDAO::get_query()
{
	return query;
}

void SCIMSDAO::set_query(Query q)
{
	query = q;
}


void SCIMSDAO::init_connection() {
    db.setHostName("localhost");
    db.setUserName("root");
    db.setPassword("");
    db.setDatabaseName("scims");
}

void SCIMSDAO::read(QTableView* w, QLineEdit* e) {
    if (db.open()) {
        QString query;
		switch (get_query()) {
		case INVENTORY:
			query = "SELECT category, itemname, itemcode, price, quantity, delivery_status, quantity_required from item;";
			break;
		case ORDERS:
			query = "select purchase.purchase_id,purchase.itemcode,item.itemname,purchase.supplier,purchase.quantity_purchased,purchase.date_purchased,item.delivery_status,item.quantity_received,item.arrival_date from purchase left join item on purchase.itemcode=item.itemcode;";
			break;
		case DESTOCKED:
			query = "select issue.issue_id,issue.itemcode,item.itemname,issue.issued_to,issue.date_issued,item.quantity,issue.quantity_issued from issue left join item on issue.itemcode=item.itemcode;";
			break;
		case LOW_STOCK:
			query = "select item.category,item.itemcode,item.itemname,item.quantity,purchase.supplier,item.price,item.quantity_required from item left join purchase on item.itemcode=purchase.itemcode where item.quantity<=item.quantity_required;";
			break;
		case EXPIRING:
			query = "select category,itemname,itemcode,price,quantity,expiry_date from item where datediff(expiry_date,now()) <= 7";
			break;
		case SEARCH:
            if(!e->text().isEmpty())
                query = "Select item.*,purchase.purchase_id,purchase.supplier,purchase.date_purchased,issue.issue_id,issue.issued_to,issue.date_issued "
                        "from item left join purchase on item.itemcode=purchase.itemcode left join issue on item.itemcode=issue.itemcode where CONCAT"
                        " (item.category,item.itemname,item.itemcode,item.quantity) like :search;";
            else {
                QMessageBox::critical(nullptr, "SCIMS", "Empty fields not accepted.");
                return;
            }
			break;
		}
        QSqlQuery q;
        q.prepare(query);
        if(e != NULL) q.bindValue(":search", QString("%%1%").arg(e->text()));
        q.exec();
        model = new SqlTableModel();
        model->setQuery(std::move(q));
        /*
         * model->select();
         * if(e != NULL) if (!e->text().isEmpty())
         * model->setFilter(QString("CONCAT (item.category,item.itemname,item.itemcode,item.quantity) like '%%1%'").arg(e->text()));
         */
		model->setEditStrategy(QSqlTableModel::OnManualSubmit);
        w->setModel(model);
        qDebug() << q.executedQuery();
    }
}

bool SCIMSDAO::insert(QVector<QString> params) {
	if (db.open()) {
		QSqlQuery q(db);
		QString query;
		switch (get_query()) {
		case INVENTORY: {
			query = "INSERT INTO item (category, itemname, itemcode, price, quantity,quantity_required,expiry_date) "
				"VALUES (:categ,:name,:code,:price,:quantity,:quant_req,:exp_date);";
            q.prepare(query);
            q.bindValue(":categ", params[0]);
            q.bindValue(":name", params[1]);
            q.bindValue(":code", params[2]);
            q.bindValue(":price", params[3]);
            q.bindValue(":quantity", params[4]);
            q.bindValue(":quant_req", params[5]);
            q.bindValue(":exp_date", params[6]);
            break;
		}
		case ORDERS: {
			query = "INSERT INTO purchase (purchase_id, itemcode, supplier, quantity_purchased, date_purchased) "
				"VALUES (:pid,:code,:supplier,:quant_pur,:date_pur);";
            q.prepare(query);
            q.bindValue(":pid", params[0]);
            q.bindValue(":code", params[1]);
            q.bindValue(":supplier", params[2]);
            q.bindValue(":quant_pur", params[3]);
            q.bindValue(":date_pur", params[4]);

			break;
		}
		case DESTOCKED: {
			query = "INSERT INTO issue (issue_id, itemcode, issued_to, quantity_issued, date_issued) "
				"VALUES (:iid,:code,:ito,:quant_is,:date_is);UPDATE item SET quantity = quantity - :quant_is2 WHERE itemcode = :code2";
            q.prepare(query);
            q.bindValue(":iid", params[0]);
            q.bindValue(":code", params[1]);
            q.bindValue(":ito", params[2]);
            q.bindValue(":quant_is", params[3]);
            q.bindValue(":date_is", params[4]);
            q.bindValue(":quant_is2", params[3]);
            q.bindValue(":code2", params[1]);

			break;
		}
		}
        if (q.exec()) return true;
		else qDebug() << q.lastError().text();
	}
	return false;
}

bool SCIMSDAO::update(QVector<QString> params) {
	if (db.open()) {
		QSqlQuery q(db);
		QString query;
		switch (get_query()) {
		case MASS: {
			query = "UPDATE item SET quantity = (select quantity WHERE itemcode=:code)-:sold WHERE itemcode = :code1;";
			q.prepare(query);
            q.bindValue(":code", params[0]);
            q.bindValue(":sold", params[1].toInt());
            q.bindValue(":code1", params[2]);
            break;
		}
		case PRICE: {
			query = "UPDATE item SET price = :price2 WHERE itemcode = :code;";
			q.prepare(query);
            q.bindValue(":price2", params[0].toInt());
            q.bindValue(":code", params[1]);
			break;
		}
		case DELIVERY: {
			query = "UPDATE item SET quantity_received = :quant_rec WHERE itemcode = :code;"
				"UPDATE item SET arrival_date = :date WHERE itemcode = :code1;"
                "UPDATE item SET quantity=quantity+:quant_rec1 WHERE itemcode = :code2;"
                    "UPDATE item SET delivery_status='DELIVERED' where itemcode = :code3";
			q.prepare(query);
            q.bindValue(":quant_rec", params[0].toInt());
            q.bindValue(":code", params[1]);
            q.bindValue(":date", params[2]);
            q.bindValue(":code1", params[1]);
            q.bindValue(":quant_rec1", params[0].toInt());
            q.bindValue(":code2", params[1]);
            q.bindValue(":code3", params[1]);
			break;
		}
		case PASSWORD: {
            q.exec("SELECT password FROM scims_users WHERE username='admin'");
			if (q.next()) {
				//TODO: Look at decryption here
                QString oldp = params.at(0),
                    newp = params.at(1), newp2 = params.at(2);
				if (oldp == q.value(0)) {
					if (newp == newp2) {
						if (oldp != newp2) {
							//TODO: look at encryption here
							QString enc = newp2;
							if (!enc.isEmpty() || !enc.isNull()) {
                                query = "UPDATE scims_users SET password = :newp2 WHERE username = 'admin';";
								q.prepare(query);
                                q.bindValue(":newp2", newp2);
							}
						}
						else {
                            QMessageBox::critical(0, "SCIMS", "New password cannot be old password.", QMessageBox::Ok | QMessageBox::Cancel);
                            return false;
                        }
					}
					else {
                        QMessageBox::critical(0, "SCIMS", "Passwords do not match.", QMessageBox::Ok | QMessageBox::Cancel);
                        return false;
                    }
				}
				else {
                    QMessageBox::critical(0, "SCIMS", "Invalid password.", QMessageBox::Ok | QMessageBox::Cancel);
                    return false;
				}
			}
			else {
                QMessageBox::critical(0, "SCIMS", "No such user exists.", QMessageBox::Ok | QMessageBox::Cancel);
                return false;
            }
			break;

		}
		}
        if (q.exec()){
            if (q.numRowsAffected()>0)
            return true;
            else {
                QMessageBox::critical(0, "SCIMS", "No such record exists.", QMessageBox::Ok | QMessageBox::Cancel);
                return false;
            }
        }
		else qDebug() << q.lastError().text();
	}
	return false;
}


bool SCIMSDAO::delete_(QVector<QString> params) {
	if (db.open()) {
		QSqlQuery q(db);
		QString query;
		switch (get_query()) {
		case ORDERS: {
			query = "DELETE FROM purchase WHERE purchase_id = :p_id;";
			q.prepare(query);
            q.bindValue(":p_id", params[0]);
			break;
		}
		case DELETE: {
            query = "DELETE FROM item WHERE itemcode = :code;DELETE FROM purchase WHERE itemcode = :code1;"
				"DELETE FROM issue WHERE itemcode = :code2";
			q.prepare(query);
			q.bindValue(":code", params[0]);
			q.bindValue(":code1", params[0]);
			q.bindValue(":code2", params[0]);
			break;
		}
		}
        if (q.exec()){
            if (q.numRowsAffected()>0)
            return true;
            else {
                QMessageBox::critical(0, "SCIMS", "No such record exists.", QMessageBox::Ok | QMessageBox::Cancel);
                return false;
            }
        }
		else qDebug() << q.lastError().text();
	}
	return false;
}

bool SCIMSDAO::save(QTableView* table) {
	return 0;
}

int SCIMSDAO::expiring(){
    if (db.open()) {
        QSqlQuery q(
                    "select count(*) category,itemname,itemcode,price,quantity,expiry_date from item where datediff(expiry_date, now()) <= 7",
                    db
                    );
        if (q.next()){
            return q.value(0).toInt();
        }
        return 0;
    }
    return -1;
}

int SCIMSDAO::low(){
    if (db.open()) {
        QSqlQuery q("SELECT COUNT(*) FROM item WHERE quantity < quantity_required;", db);
        if (q.next()){
            return q.value(0).toInt();
        }
        return 0;
    }
    return -1;
}

SCIMSDAO::~SCIMSDAO() {
	if (db.isOpen()) db.close();
}
