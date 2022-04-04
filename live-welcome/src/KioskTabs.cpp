// SPDX-FileCopyrightText: 2021-2022 Filipe Coelho <falktx@falktx.com>
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "KioskTabs.hpp"

#include "KioskAbout.hpp"
#include "KioskForeignWidget.hpp"
#include "Utils.hpp"

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QThread>
#include <QtCore/QTimer>
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabBar>
#include <QtWebEngineWidgets/QWebEngineView>

#include <KParts/ReadWritePart>
#include <KService/KService>

#include <jack/jack.h>

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
    plusButton->setFlat(true);
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

void KioskTabs::closeForeignWidgets()
{
    const int count = this->count();
    QTabBar* const tabBar = this->tabBar();

    for (int i = count; --i >= 0;)
    {
        if (tabBar->tabButton(i, QTabBar::RightSide) == nullptr)
            break;
        tabClosed(i);
    }
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
        "Cardinal",
        "Terminal"
    };
    bool ok = false;
    const QString ret = QInputDialog::getItem(this, "Add", "Which new element to add:", items, 0, false, &ok);

    if (ok && !ret.isEmpty())
    {
        oldIndex = count();

        /**/ if (ret == "Cardinal")
        {
            KioskForeignWidget* const w = new KioskForeignWidget(this);
            setCurrentIndex(addTab(w, "Cardinal"));

            if (w->startForeignTool("Cardinal"))
            {
                if (jack_client_t* const client = jack_client_open("CardinalHelper", JackNoStartServer, nullptr))
                {
                    // wait max 5s for Cardinal to appear
                    for (int i=0; i<50; ++i)
                    {
                        if (jack_port_by_name(client, "Cardinal:events-in") == nullptr)
                        {
                            QThread::msleep(100);
                            continue;
                        }

                        jack_connect(client, "mod-midi-merger:out", "Cardinal:events-in");
                        jack_connect(client, "system:capture_1", "Cardinal:audio_in_1");
                        jack_connect(client, "system:capture_2", "Cardinal:audio_in_2");
                        jack_connect(client, "system:capture_3", "Cardinal:audio_in_3");
                        jack_connect(client, "system:capture_4", "Cardinal:audio_in_4");
                        jack_connect(client, "system:capture_5", "Cardinal:audio_in_5");
                        jack_connect(client, "system:capture_6", "Cardinal:audio_in_6");
                        jack_connect(client, "system:capture_7", "Cardinal:audio_in_7");
                        jack_connect(client, "system:capture_8", "Cardinal:audio_in_8");
                        jack_connect(client, "Cardinal:midi-out", "mod-midi-broadcaster:in");
                        jack_connect(client, "Cardinal:audio_out_1", "mod-monitor:in_1");
                        jack_connect(client, "Cardinal:audio_out_2", "mod-monitor:in_2");
                        jack_connect(client, "Cardinal:audio_out_3", "system:playback_3");
                        jack_connect(client, "Cardinal:audio_out_4", "system:playback_4");
                        jack_connect(client, "Cardinal:audio_out_5", "system:playback_5");
                        jack_connect(client, "Cardinal:audio_out_6", "system:playback_6");
                        jack_connect(client, "Cardinal:audio_out_7", "system:playback_7");
                        jack_connect(client, "Cardinal:audio_out_8", "system:playback_8");
                        break;
                    }

                    jack_client_close(client);
                }
            }
            else
            {
                tabClosed(oldIndex);
            }
        }
        else if (ret == "Terminal")
        {
            openTerminal();
        }

        updatePlusButtonPosition();
        return;
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
