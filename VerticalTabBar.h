//
// Created by Admin on May 29 2022.
//

#ifndef SCIMS_REMASTER_VERTICALTABBAR_H
#define SCIMS_REMASTER_VERTICALTABBAR_H

#include <QTabBar>

class VerticalTabBar : public QTabBar {
public:
	QSize tabSizeHint(int index) const override;

protected:
	void paintEvent(QPaintEvent* /*event*/) override;

};


#endif //SCIMS_REMASTER_VERTICALTABBAR_H
