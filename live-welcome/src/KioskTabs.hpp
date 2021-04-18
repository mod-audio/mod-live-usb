/*
 */

#pragma once

#include <KParts/BrowserExtension>
#include <QtWidgets/QTabWidget>
#include <QtWebEngineWidgets/QWebEngineView>

class KioskTabs : public QTabWidget
{
    KParts::BrowserExtension* fileBrowser;
    QWebEngineView* webBrowser;

public:
    explicit KioskTabs(QWidget* const parent);

    void openKonsole();
    void reloadPage();
};
