// SPDX-FileCopyrightText: 2021-2022 Filipe Coelho <falktx@falktx.com>
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <QtWidgets/QTabWidget>

class QPushButton;
class QWebEngineView;

class KioskTabs : public QTabWidget
{
    Q_OBJECT

    QPushButton* const plusButton;
    QWebEngineView* const webBrowser;
    int oldIndex;

public:
    explicit KioskTabs(QWidget* const parent);

    void closeForeignWidgets();
    void openTerminal();
    void reloadPage();

private:
    void updatePlusButtonPosition();

private Q_SLOTS:
    void addNewClicked();
    void tabClicked(int index);
    void tabClosed(int index);
    void tabCancel();
};
