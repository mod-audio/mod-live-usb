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

void KioskTabs::openKonsole()
{
    if (const KService::Ptr service = KService::serviceByDesktopName("konsolepart"))
        if (KParts::ReadOnlyPart* const p = service->createInstance<KParts::ReadOnlyPart>(nullptr))
            setCurrentIndex(addTab(p->widget(), "Konsole"));
}

void KioskTabs::reloadPage()
{
    webBrowser->setUrl(QUrl("http://localhost:8000/"));
}
