/*
 */

#include "KioskForeignWidget.hpp"

#include <QtGui/QResizeEvent>
#include <QtX11Extras/QX11Info>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

KioskForeignWidget::KioskForeignWidget(QWidget* const parent)
    : QWidget(parent, Qt::Window),
      process(),
      timerId(0),
      x11Size(),
      x11Display(QX11Info::display()),
      x11Window(0)
{
    process.setProcessChannelMode(QProcess::ForwardedChannels);
}

KioskForeignWidget::~KioskForeignWidget()
{
    if (timerId != 0)
        killTimer(timerId);

    if (process.state() != QProcess::NotRunning)
    {
        process.terminate();
        process.waitForFinished();
    }
}

bool KioskForeignWidget::startForeignTool(const QString& tool)
{
    if (x11Display == nullptr)
        return false;
    if (tool.isEmpty())
        return false;

    if (tool == "Cardinal")
    {
        const QStringList arguments = { "embed", QString::number(winId()) };

        process.start("Cardinal", arguments, QIODevice::ReadWrite | QIODevice::Unbuffered);
    }

    if (process.state() != QProcess::NotRunning)
    {
        process.waitForStarted();
        timerId = startTimer(1000, Qt::CoarseTimer);
        return true;
    }

    return false;
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
        XGetWMNormalHints(x11Display, x11Window, &hints, &ignore);

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

    XQueryTree(x11Display, (ulong)winId(), &rootWindow, &parentWindow, &childWindows, &numChildren);

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
    printf("resized %i %i\n", size.width(), size.height());
    x11Size = size;
    XResizeWindow(x11Display, x11Window,
                  static_cast<unsigned>(size.width()),
                  static_cast<unsigned>(size.height()));
}
