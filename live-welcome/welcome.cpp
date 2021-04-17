/*
 */

#include "welcome.hpp"

#include <QtGui/QKeyEvent>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>

#include <KParts/BrowserExtension>
#include <KParts/ReadWritePart>
#include <KService/KService>

KioskTabs::KioskTabs(QWidget* const parent)
  : QTabWidget(parent),
    webBrowser(this),
    fileBrowser(nullptr)
{
    webBrowser.setHtml("<html><body bgcolor='black'></body></html>");
    addTab(&webBrowser, "Pedalboard");
    // webBrowser.setUrl(QUrl("http://192.168.51.1/"));

    if (const KService::Ptr service = KService::serviceByDesktopName("okular_part"))
    {
        if (KParts::ReadOnlyPart* const p = service->createInstance<KParts::ReadOnlyPart>(nullptr))
        {
            p->openUrl(QUrl("file:///home/falktx/Documents/LU_04-15_DigiSub.pdf"));
            addTab(p->widget(), "Documentation");
        }
    }

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
}

void KioskTabs::openKonsole()
{
    if (const KService::Ptr service = KService::serviceByDesktopName("konsolepart"))
        if (KParts::ReadOnlyPart* const p = service->createInstance<KParts::ReadOnlyPart>(nullptr))
            setCurrentIndex(addTab(p->widget(), "Konsole"));
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

        if (event->key() == Qt::Key::Key_T)
            tabWidget.openKonsole();
    }
};

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    // app.setApplicationDisplayName("MOD Live USB Welcome");
    app.setApplicationName("MOD Live USB Welcome");

    KioskWindow win;
//     win.showMinimized();
//     app.processEvents();

    win.showNormal();
    return app.exec();
}
