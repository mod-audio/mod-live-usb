/*
 */

#include "KioskTabs.hpp"

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QTimer>
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabBar>
#include <QtWebEngineWidgets/QWebEngineView>
// #include <QtWebEngineWidgets/QWebEngineSettings>

// #include <KParts/BrowserExtension>
#include <KParts/ReadWritePart>
#include <KService/KService>

#include "KioskAbout.hpp"
#include "KioskForeignWidget.hpp"
#include "Utils.hpp"

static const char* const initial_html = "<html><body><style>body { background: black url(data:image/png;base64,"
#include "../resources/watermark.txt"
") no-repeat scroll calc(100vw - 506px) calc(100vh - 145px) }</style></body></html>";

static const char* const initial_notes = ""
"This is a space dedicated to writing text, feel free to reuse as copy & paste area.\n"
"\n"
"PS: You can use Ctrl+Alt+T keyboard combination to open a terminal in a new tab.\n"
"\n";

KioskTabs::KioskTabs(QWidget* const parent)
  : QTabWidget(parent),
    // fileBrowser(nullptr),
    plusButton(new QPushButton(this)),
    webBrowser(new QWebEngineView(this)),
    oldIndex(0)
{
    setTabsClosable(true);

    QTabBar* const tabBar = this->tabBar();

    plusButton->setFixedSize(tabBar->height(), tabBar->height());
    plusButton->setText("+");

    webBrowser->setHtml(initial_html);
    addTab(webBrowser, "Pedalboard");

    const QString documentationPath(findDocumentation());
    if (!documentationPath.isEmpty())
    {
        QWebEngineView* const documentation = new QWebEngineView(this);
        documentation->setUrl(QUrl("file://" + documentationPath));
        addTab(documentation, "Documentation");
    }

    if (const KService::Ptr service = KService::serviceByDesktopName("katepart"))
    {
        if (KParts::ReadWritePart* const p = service->createInstance<KParts::ReadWritePart>(nullptr, {}))
        {
            if (!documentationPath.isEmpty())
            {
                QDir notesDir(documentationPath);
                notesDir.cdUp();

                const QString notesFilename(notesDir.absoluteFilePath("notes.txt"));

                QFile notesFile(notesFilename);
                if (!notesFile.exists() && notesFile.open(QFile::WriteOnly))
                {
                    notesFile.write(initial_notes);
                    notesFile.close();
                }

                p->openUrl(QUrl::fromLocalFile(notesFilename));
            }

            addTab(p->widget(), "Notes");
        }
    }

    addTab(new KioskAbout(this), "About");

    const int count = this->count();

    for (int i = 0; i < count; ++i)
        tabBar->setTabButton(i, QTabBar::RightSide, nullptr);

    updatePlusButtonPosition();

    connect(plusButton, SIGNAL(clicked()), this, SLOT(addNewClicked()));
    connect(this, SIGNAL(tabBarClicked(int)), this, SLOT(tabClicked(int)));
    connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(tabClosed(int)));
}

void KioskTabs::openTerminal()
{
    if (const KService::Ptr service = KService::serviceByDesktopName("konsolepart"))
        if (KParts::ReadOnlyPart* const p = service->createInstance<KParts::ReadOnlyPart>(nullptr))
            setCurrentIndex(addTab(p->widget(), "Terminal"));
}

void KioskTabs::reloadPage()
{
    webBrowser->setUrl(QUrl("http://localhost:8888/"));
}

void KioskTabs::updatePlusButtonPosition()
{
    const qreal pixelRatio = devicePixelRatioF();
    plusButton->move(tabBar()->tabRect(count() - 1).topRight().x() + 2 * pixelRatio, 0);
}

void KioskTabs::addNewClicked()
{
    const QStringList items = {
        // "Cardinal",
        "Terminal"
    };
    bool ok = false;
    const QString ret = QInputDialog::getItem(this, "Add", "Which new element to add:", items, 0, false, &ok);

    if (ok && !ret.isEmpty())
    {
        oldIndex = count();

        /**/ if (ret == "Cardinal")
            {}
        else if (ret == "Terminal")
            openTerminal();

        updatePlusButtonPosition();
    }

    QTimer::singleShot(0, this, SLOT(tabCancel()));
}

void KioskTabs::tabClicked(int index)
{
    oldIndex = index;
}

void KioskTabs::tabClosed(int index)
{
    if (currentIndex() == index)
    {
        oldIndex = 0;
        setCurrentIndex(oldIndex);
    }

    QWidget* const widget = this->widget(index);
    removeTab(index);
    delete widget;

    updatePlusButtonPosition();
}

void KioskTabs::tabCancel()
{
    setCurrentIndex(oldIndex);
}
