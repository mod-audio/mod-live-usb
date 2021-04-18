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

    QRect clockRect;
    int clockTimer;

public:
    explicit KioskTabs(QWidget* const parent);

    void openKonsole();
    void reloadPage()
    {
        webBrowser.setUrl(QUrl("http://localhost:8000/"));
    }

protected:
    void paintEvent(QPaintEvent*) override;
    void resizeEvent(QResizeEvent*) override;
    void timerEvent(QTimerEvent*) override;

private Q_SLOTS:
    // nothing yet
};
