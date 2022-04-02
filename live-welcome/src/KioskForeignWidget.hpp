/*
 */

#pragma once

#include <QtWidgets/QWidget>

class KioskForeignWidget : public QWidget
{
    int timerId;
    QSize x11Size;
    uintptr_t x11Window;

public:
    explicit KioskForeignWidget(QWidget* parent);
    ~KioskForeignWidget() override;

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

protected:
    void resizeEvent(QResizeEvent* event) override;
    void timerEvent(QTimerEvent* event) override;

private:
    void doResize(const QSize& size);
};
