//
// Created by Admin on May 29 2022.
//

#include "VerticalTabWidget.h"
#include "VerticalTabBar.h"

VerticalTabWidget::VerticalTabWidget(QWidget* parent) : QTabWidget(parent) {
	this->setTabBar(new VerticalTabBar);
	this->setTabPosition(QTabWidget::West);
}
