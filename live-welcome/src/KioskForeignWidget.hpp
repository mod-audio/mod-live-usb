/*
 */

#pragma once

#include <QtCore/QProcess>
#include <QtWidgets/QWidget>

typedef struct _XDisplay Display;

class KioskForeignWidget : public QWidget
{
    QProcess process;
    int timerId;
    QSize x11Size;
    Display* x11Display;
    uintptr_t x11Window;

public:
    explicit KioskForeignWidget(QWidget* parent);
    ~KioskForeignWidget() override;

    bool startForeignTool(const QString& tool);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

protected:
    void resizeEvent(QResizeEvent* event) override;
    void timerEvent(QTimerEvent* event) override;

private:
    void doResize(const QSize& size);
};
