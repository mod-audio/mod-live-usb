/*
 */

#pragma once

#include <QtGui/QPainter>
#include <QtWidgets/QWidget>

class KioskAbout : public QWidget
{
public:
    explicit KioskAbout(QWidget* const parent)
        : QWidget(parent) {}

protected:
    void paintEvent(QPaintEvent* const event)
    {
        QWidget::paintEvent(event);

        QPainter painter(this);
        painter.drawText(0, 0, 400, 500, 0x0, "Hello There");
    }
};
