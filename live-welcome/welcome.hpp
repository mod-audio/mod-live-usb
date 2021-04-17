/*
 */

#pragma once

#include <KParts/BrowserExtension>
#include <QtWidgets/QTabWidget>
#include <QtWebEngineWidgets/QWebEngineView>

class KioskTabs : public QTabWidget
{
Q_OBJECT
    QWebEngineView webBrowser;
    KParts::BrowserExtension* fileBrowser;

public:
    explicit KioskTabs(QWidget* const parent);

    void openKonsole();

private Q_SLOTS:
    // nothing yet
};
