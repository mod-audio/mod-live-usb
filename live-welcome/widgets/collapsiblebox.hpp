// SPDX-FileCopyrightText: 2021-2022 Filipe Coelho <falktx@falktx.com>
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <QtWidgets/QFrame>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>

class QToolButtonWithMouseTracking : public QToolButton
{
public:
    explicit QToolButtonWithMouseTracking(QWidget* const parent)
      : QToolButton(parent) {}

protected:
    void enterEvent(QEvent* const event) override
    {
        QFont nextfont(font());
        nextfont.setBold(true);
        setFont(nextfont);
        QToolButton::enterEvent(event);
    }

    void leaveEvent(QEvent* const event) override
    {
        QFont nextfont(font());
        nextfont.setBold(false);
        setFont(nextfont);
        QToolButton::leaveEvent(event);
    }
};

class CollapsibleBox : public QFrame
{
    Q_OBJECT

    QToolButtonWithMouseTracking toggleButton;
    QWidget contentArea;
    QVBoxLayout contentLayout;
    QVBoxLayout mainLayout;

public:
    explicit CollapsibleBox(const QString& title, QWidget* const parent)
      : QFrame(parent),
        toggleButton(this),
        contentArea(this),
        contentLayout(&contentArea),
        mainLayout(this)
    {
        setFrameShape(QFrame::StyledPanel);
        setFrameShadow(QFrame::Raised);

        toggleButton.setText(title);
        toggleButton.setCheckable(true);
        toggleButton.setFocusPolicy(Qt::FocusPolicy::NoFocus);
        toggleButton.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        toggleButton.setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        connect(&toggleButton, SIGNAL(toggled(bool)), this, SLOT(toolButtonPressed(bool)));

        contentLayout.setContentsMargins(6, 2, 6, 0);
        contentLayout.setSpacing(3);

        mainLayout.setContentsMargins(0, 0, 0, 4);
        mainLayout.setSpacing(1);
        mainLayout.addWidget(&toggleButton);
        mainLayout.addWidget(&contentArea);

        // initial state is hidden
        toggleButton.setChecked(false);
        toggleButton.setArrowType(Qt::RightArrow);
        contentArea.hide();
    }

    QVBoxLayout& getContentLayout() noexcept
    {
        return contentLayout;
    }

private Q_SLOTS:
    void toolButtonPressed(const bool toggled)
    {
        contentArea.setVisible(toggled);
        toggleButton.setArrowType(toggled ? Qt::DownArrow : Qt::RightArrow);
    }
};
