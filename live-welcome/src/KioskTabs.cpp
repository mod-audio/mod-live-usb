/*
 */

#include "KioskTabs.hpp"

// #include <QtWebEngineWidgets/QWebEngineSettings>

#include <QtCore/QDir>
#include <QtCore/QFile>

// #include <KParts/BrowserExtension>
#include <KParts/ReadWritePart>
#include <KService/KService>

#include <dlfcn.h>

#include "KioskAbout.hpp"
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
    fileBrowser(nullptr),
    webBrowser(new QWebEngineView(this)),
    documentation(nullptr)
{
    webBrowser->setHtml(initial_html);
    addTab(webBrowser, "Pedalboard");

    QString documentationPath(findDocumentation());
    if (!documentationPath.isEmpty())
    {
        documentation = new QWebEngineView(this);
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

void KioskTabs::openTerminal()
{
    if (const KService::Ptr service = KService::serviceByDesktopName("konsolepart"))
        if (KParts::ReadOnlyPart* const p = service->createInstance<KParts::ReadOnlyPart>(nullptr))
            setCurrentIndex(addTab(p->widget(), "Terminal"));
}

void KioskTabs::reloadPage()
{
    webBrowser->setUrl(QUrl("http://localhost:8000/"));
}
