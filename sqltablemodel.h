#ifndef SQLTABLEMODEL_H
#define SQLTABLEMODEL_H

#include <QSqlTableModel>
#include <QSqlQuery>

class SqlTableModel : public QSqlTableModel
{
public:
	explicit SqlTableModel(QObject* parent = nullptr);
	virtual void setQuery(const QSqlQuery& query);
	typedef QSqlTableModel base;
};

#endif // SQLTABLEMODEL_H
