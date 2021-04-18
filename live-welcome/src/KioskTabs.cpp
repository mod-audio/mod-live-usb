/*
 */

#include "KioskTabs.hpp"

#include <KParts/BrowserExtension>
#include <KParts/ReadWritePart>
#include <KService/KService>

#include "KioskAbout.hpp"

KioskTabs::KioskTabs(QWidget* const parent)
  : QTabWidget(parent),
    fileBrowser(nullptr),
    webBrowser(new QWebEngineView(this))
{
    webBrowser->setHtml("<html><body bgcolor='black'></body></html>");
    addTab(webBrowser, "Pedalboard");

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
