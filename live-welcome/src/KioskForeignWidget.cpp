/*
 */

#include "KioskForeignWidget.hpp"

#include <QtGui/QResizeEvent>
#include <QtX11Extras/QX11Info>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

KioskForeignWidget::KioskForeignWidget(QWidget* const parent)
    : QWidget(parent, Qt::Window),
      timerId(0),
      x11Size(),
      x11Window(0)
{
    timerId = startTimer(1000, Qt::CoarseTimer);

    printf("X11 Embed Id is %lu\n", (ulong)winId());
}

KioskForeignWidget::~KioskForeignWidget()
{
    if (timerId != 0)
        killTimer(timerId);
}

QSize KioskForeignWidget::sizeHint() const
{
    return x11Size;
}

QSize KioskForeignWidget::minimumSizeHint() const
{
    if (x11Window != 0)
    {
        XSizeHints hints = {};
        long ignore = 0;
        XGetWMNormalHints(QX11Info::display(), x11Window, &hints, &ignore);

        if (hints.flags & PMinSize)
            return QSize(hints.min_width, hints.min_height);
    }

    return QSize();
}

void KioskForeignWidget::resizeEvent(QResizeEvent* const event)
{
    QWidget::resizeEvent(event);

    if (x11Window != 0)
        doResize(event->size());
}

void KioskForeignWidget::timerEvent(QTimerEvent* event)
{
    QWidget::timerEvent(event);

    if (event->timerId() != timerId || x11Window != 0)
        return;

    ::Window rootWindow, parentWindow;
    ::Window* childWindows = nullptr;
    uint numChildren = 0;

    XQueryTree(QX11Info::display(), (ulong)winId(), &rootWindow, &parentWindow, &childWindows, &numChildren);

    if (numChildren > 0 && childWindows != nullptr)
    {
        x11Window = childWindows[0];
        XFree(childWindows);

        killTimer(timerId);
        timerId = 0;

        doResize(size());
    }
}

void KioskForeignWidget::doResize(const QSize& size)
{
    x11Size = size;
    XResizeWindow(QX11Info::display(), x11Window,
                  static_cast<unsigned>(size.width()),
                  static_cast<unsigned>(size.height()));
    printf("resized %i %i\n", size.width(), size.height());
}
