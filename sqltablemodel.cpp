#include "sqltablemodel.h"

SqlTableModel::SqlTableModel(QObject* parent)
	: QSqlTableModel{ parent }
{

}

void SqlTableModel::setQuery(const QSqlQuery& query) {
	base::setQuery(query);
}
