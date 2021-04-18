/*
 */

#include "welcome.hpp"

#include <QtGui/QKeyEvent>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QTabBar>

#include <KParts/BrowserExtension>
#include <KParts/ReadWritePart>
#include <KService/KService>

KioskTabs::KioskTabs(QWidget* const parent)
  : QTabWidget(parent),
    webBrowser(this),
    fileBrowser(nullptr),
    clockRect(),
    clockTimer(-1)
{
    webBrowser.setHtml("<html><body bgcolor='black'></body></html>");
    addTab(&webBrowser, "Pedalboard");

#if 0
    if (const KService::Ptr service = KService::serviceByDesktopName("okular_part"))
    {
        if (KParts::ReadOnlyPart* const p = service->createInstance<KParts::ReadOnlyPart>(nullptr))
        {
            p->openUrl(QUrl("file:///home/falktx/Documents/LU_04-15_DigiSub.pdf"));
            addTab(p->widget(), "Documentation");
        }
    }
#endif

    if (const KService::Ptr service = KService::serviceByDesktopName("katepart"))
    {
        if (KParts::ReadWritePart* const p = service->createInstance<KParts::ReadWritePart>(nullptr, {}))
        {
            // p->openUrl(QUrl("file:///tmp/notes.txt"));
            addTab(p->widget(), "Notes");
        }
    }

#if 0
    if (const KService::Ptr service = KService::serviceByDesktopName("dolphinpart"))
    {
        if (KParts::ReadOnlyPart* const p = service->createInstance<KParts::ReadOnlyPart>(nullptr))
        {
            addTab(p->widget(), "Dolphin");
            p->openUrl(QUrl("file:///home/falktx/"));

        }
    }
#endif

    QFont monofont(font());
    monofont.setFamily("Monospace");
    QFontMetrics metrics(monofont);
    const int height = tabBar()->height();
    const int fontheight = metrics.height();

    clockRect = QRect(0, height/2 - fontheight/2, metrics.horizontalAdvance("00:00:00"), fontheight);
    clockTimer = startTimer(1000);
}

void KioskTabs::openKonsole()
{
    if (const KService::Ptr service = KService::serviceByDesktopName("konsolepart"))
        if (KParts::ReadOnlyPart* const p = service->createInstance<KParts::ReadOnlyPart>(nullptr))
            setCurrentIndex(addTab(p->widget(), "Konsole"));
}

void KioskTabs::paintEvent(QPaintEvent* const event)
{
    QTabWidget::paintEvent(event);

    if (clockRect.x() == 0)
        return;

    QPainter painter(this);
    painter.drawText(clockRect, QTime::currentTime().toString("hh:mm:ss"));
}

void KioskTabs::resizeEvent(QResizeEvent* const event)
{
    QTabWidget::resizeEvent(event);

    clockRect.moveTo(width() - clockRect.width(), clockRect.y());
    update(clockRect);
}

void KioskTabs::timerEvent(QTimerEvent* const event)
{
    QTabWidget::timerEvent(event);

    if (event->timerId() != clockTimer)
        return;
    if (clockRect.x() == 0)
        return;

    update(clockRect);
}

class KioskWindow : public QMainWindow
{
    KioskTabs tabWidget;

public:
    KioskWindow()
      : QMainWindow(),
        tabWidget(this)
    {
        setCentralWidget(&tabWidget);
        setWindowTitle("MOD Live USB");

        // TODO full screen mode
        resize(1200, 800);
    }

    ~KioskWindow()
    {
    }

protected:
    void keyPressEvent(QKeyEvent * event) override
    {
        QMainWindow::keyPressEvent(event);

        const Qt::KeyboardModifiers modifiers = event->modifiers();

        if ((modifiers & Qt::Modifier::ALT) == 0x0)
            return;
        if ((modifiers & Qt::Modifier::CTRL) == 0x0)
            return;

        if (event->key() == Qt::Key::Key_R)
            tabWidget.reloadPage();
        if (event->key() == Qt::Key::Key_T)
            tabWidget.openKonsole();
    }
};

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    // app.setApplicationDisplayName("MOD Live USB Welcome");
    app.setApplicationName("MOD Live USB Welcome");

    QFont font(app.font());
    font.setPixelSize(16);
    app.setFont(font);

#if 1
    QPalette palette;
    palette.setColor(QPalette::Disabled, QPalette::Window, QColor(14, 14, 14));
    palette.setColor(QPalette::Active,   QPalette::Window, QColor(17, 17, 17));
    palette.setColor(QPalette::Inactive, QPalette::Window, QColor(17, 17, 17));
    palette.setColor(QPalette::Disabled, QPalette::WindowText, QColor(83, 83, 83));
    palette.setColor(QPalette::Active,   QPalette::WindowText, QColor(240, 240, 240));
    palette.setColor(QPalette::Inactive, QPalette::WindowText, QColor(240, 240, 240));
    palette.setColor(QPalette::Disabled, QPalette::Base, QColor(6, 6, 6));
    palette.setColor(QPalette::Active,   QPalette::Base, QColor(7, 7, 7));
    palette.setColor(QPalette::Inactive, QPalette::Base, QColor(7, 7, 7));
    palette.setColor(QPalette::Disabled, QPalette::AlternateBase, QColor(12, 12, 12));
    palette.setColor(QPalette::Active,   QPalette::AlternateBase, QColor(14, 14, 14));
    palette.setColor(QPalette::Inactive, QPalette::AlternateBase, QColor(14, 14, 14));
    palette.setColor(QPalette::Disabled, QPalette::ToolTipBase, QColor(4, 4, 4));
    palette.setColor(QPalette::Active,   QPalette::ToolTipBase, QColor(4, 4, 4));
    palette.setColor(QPalette::Inactive, QPalette::ToolTipBase, QColor(4, 4, 4));
    palette.setColor(QPalette::Disabled, QPalette::ToolTipText, QColor(230, 230, 230));
    palette.setColor(QPalette::Active,   QPalette::ToolTipText, QColor(230, 230, 230));
    palette.setColor(QPalette::Inactive, QPalette::ToolTipText, QColor(230, 230, 230));
    palette.setColor(QPalette::Disabled, QPalette::Text, QColor(74, 74, 74));
    palette.setColor(QPalette::Active,   QPalette::Text, QColor(230, 230, 230));
    palette.setColor(QPalette::Inactive, QPalette::Text, QColor(230, 230, 230));
    palette.setColor(QPalette::Disabled, QPalette::Button, QColor(24, 24, 24));
    palette.setColor(QPalette::Active,   QPalette::Button, QColor(28, 28, 28));
    palette.setColor(QPalette::Inactive, QPalette::Button, QColor(28, 28, 28));
    palette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(90, 90, 90));
    palette.setColor(QPalette::Active,   QPalette::ButtonText, QColor(240, 240, 240));
    palette.setColor(QPalette::Inactive, QPalette::ButtonText, QColor(240, 240, 240));
    palette.setColor(QPalette::Disabled, QPalette::BrightText, QColor(255, 255, 255));
    palette.setColor(QPalette::Active,   QPalette::BrightText, QColor(255, 255, 255));
    palette.setColor(QPalette::Inactive, QPalette::BrightText, QColor(255, 255, 255));
    palette.setColor(QPalette::Disabled, QPalette::Light, QColor(191, 191, 191));
    palette.setColor(QPalette::Active,   QPalette::Light, QColor(191, 191, 191));
    palette.setColor(QPalette::Inactive, QPalette::Light, QColor(191, 191, 191));
    palette.setColor(QPalette::Disabled, QPalette::Midlight, QColor(155, 155, 155));
    palette.setColor(QPalette::Active,   QPalette::Midlight, QColor(155, 155, 155));
    palette.setColor(QPalette::Inactive, QPalette::Midlight, QColor(155, 155, 155));
    palette.setColor(QPalette::Disabled, QPalette::Dark, QColor(129, 129, 129));
    palette.setColor(QPalette::Active,   QPalette::Dark, QColor(129, 129, 129));
    palette.setColor(QPalette::Inactive, QPalette::Dark, QColor(129, 129, 129));
    palette.setColor(QPalette::Disabled, QPalette::Mid, QColor(94, 94, 94));
    palette.setColor(QPalette::Active,   QPalette::Mid, QColor(94, 94, 94));
    palette.setColor(QPalette::Inactive, QPalette::Mid, QColor(94, 94, 94));
    palette.setColor(QPalette::Disabled, QPalette::Shadow, QColor(155, 155, 155));
    palette.setColor(QPalette::Active,   QPalette::Shadow, QColor(155, 155, 155));
    palette.setColor(QPalette::Inactive, QPalette::Shadow, QColor(155, 155, 155));
    palette.setColor(QPalette::Disabled, QPalette::Highlight, QColor(14, 14, 14));
    palette.setColor(QPalette::Active,   QPalette::Highlight, QColor(60, 60, 60));
    palette.setColor(QPalette::Inactive, QPalette::Highlight, QColor(34, 34, 34));
    palette.setColor(QPalette::Disabled, QPalette::HighlightedText, QColor(83, 83, 83));
    palette.setColor(QPalette::Active,   QPalette::HighlightedText, QColor(255, 255, 255));
    palette.setColor(QPalette::Inactive, QPalette::HighlightedText, QColor(240, 240, 240));
    palette.setColor(QPalette::Disabled, QPalette::Link, QColor(34, 34, 74));
    palette.setColor(QPalette::Active,   QPalette::Link, QColor(100, 100, 230));
    palette.setColor(QPalette::Inactive, QPalette::Link, QColor(100, 100, 230));
    palette.setColor(QPalette::Disabled, QPalette::LinkVisited, QColor(74, 34, 74));
    palette.setColor(QPalette::Active,   QPalette::LinkVisited, QColor(230, 100, 230));
    palette.setColor(QPalette::Inactive, QPalette::LinkVisited, QColor(230, 100, 230));
    app.setPalette(palette);
#endif

    KioskWindow win;
    win.show();

    return app.exec();
}
