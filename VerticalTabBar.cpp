//
// Created by Admin on May 29 2022.
//

#include <QStylePainter>
#include <QStyleOptionTab>

#include "VerticalTabBar.h"

QSize VerticalTabBar::tabSizeHint(int index) const {
	QSize s = QTabBar::tabSizeHint(index);
	s.transpose();
	return s;
}

void VerticalTabBar::paintEvent(QPaintEvent*) {
	QStylePainter painter(this);
	QStyleOptionTab opt;

	for (int i = 0; i < count(); i++) {
		this->initStyleOption(&opt, i);
		painter.drawControl(QStyle::CE_TabBarTabShape, opt);
		painter.save();

		QSize s = opt.rect.size();
		s.transpose();
		QRect r(QPoint(), s);
		r.moveCenter(opt.rect.center());
		opt.rect = r;

		QPoint c = tabRect(i).center();
		painter.translate(c);
		painter.rotate(90);
		painter.translate(-c);
		painter.drawControl(QStyle::CE_TabBarTabLabel, opt);
		painter.restore();
	}
}
