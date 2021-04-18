/*
 */

#pragma once

#include <QtGui/QCloseEvent>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>

#include "AudioDiscovery.hpp"

#include <cstring>

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

class KioskSettingsPopup : public QDialog
{
    Q_OBJECT

    bool allowClose;
    bool firstRun;
    bool validRates;
    std::stringlist inputNames;
    std::stringlist outputNames;
    std::stringlist inputIds;
    std::stringlist outputIds;

    QVBoxLayout mainLayout;
    QLabel labelWelcome;
    QLabel labelStart;

    QHBoxLayout deviceLayout;
    QLabel deviceLabel;
    QComboBox deviceBox;

    CollapsibleBox advancedBox;
    QGridLayout advancedLayout;

    QLabel sampleRateLabel;
    QComboBox sampleRateBox;

    QLabel bufferSizeLabel;
    QComboBox bufferSizeBox;

    QDialogButtonBox okBox;

public:
    explicit KioskSettingsPopup(QWidget* const parent = nullptr)
      : QDialog(parent),
        allowClose(false),
        firstRun(true),
        validRates(false),
        mainLayout(this),
        labelWelcome(this),
        labelStart(this),
        deviceLayout(),
        deviceLabel(this),
        deviceBox(this),
        advancedBox("Advanced", this),
        advancedLayout(),
        sampleRateLabel(this),
        sampleRateBox(this),
        bufferSizeBox(this),
        okBox(this)
    {
        setWindowFlags(Qt::Tool);

        QFont welcomefont(labelWelcome.font());
        welcomefont.setPointSize(16);
        welcomefont.setBold(true);
        welcomefont.setWeight(75);

        // setup widgets
        labelWelcome.setAlignment(Qt::AlignCenter);
        labelWelcome.setMinimumHeight(50);
        labelWelcome.setFont(welcomefont);
        labelWelcome.setText("Welcome to MOD Live-USB");
        labelStart.setText("Please select your audio device from the list below.");
        deviceLabel.setText("Device:");
        sampleRateLabel.setText("Sample Rate:");
        bufferSizeLabel.setText("Buffer Size:");
        okBox.setCenterButtons(true);
        okBox.setOrientation(Qt::Horizontal);
        okBox.setStandardButtons(QDialogButtonBox::Ok);

        // no focus please
        deviceBox.setFocusPolicy(Qt::FocusPolicy::NoFocus);
        sampleRateBox.setFocusPolicy(Qt::FocusPolicy::NoFocus);
        bufferSizeBox.setFocusPolicy(Qt::FocusPolicy::NoFocus);
        okBox.setFocusPolicy(Qt::FocusPolicy::NoFocus);

        // setup device layout
        deviceLabel.setAlignment(Qt::AlignRight);
        deviceLabel.setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        deviceBox.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        deviceLayout.addWidget(&deviceLabel);
        deviceLayout.addWidget(&deviceBox);

        // setup advanced box layout
        sampleRateLabel.setAlignment(Qt::AlignRight);
        bufferSizeLabel.setAlignment(Qt::AlignRight);
        advancedLayout.addWidget(&sampleRateLabel, 0, 0, 1, 1);
        advancedLayout.addWidget(&sampleRateBox, 0, 1, 1, 1);
        advancedLayout.addWidget(&bufferSizeLabel, 1, 0, 1, 1);
        advancedLayout.addWidget(&bufferSizeBox, 1, 1, 1, 1);
        advancedBox.getContentLayout().addLayout(&advancedLayout);

        // place widgets
        mainLayout.addWidget(&labelWelcome);
        mainLayout.addWidget(&labelStart);
        mainLayout.addLayout(&deviceLayout);
        mainLayout.addWidget(&advancedBox);
        mainLayout.addWidget(&okBox);
        mainLayout.setSizeConstraint(QLayout::SetFixedSize);

        // connections
        connect(&deviceBox, SIGNAL(currentIndexChanged(int)), this, SLOT(deviceIndexChanged(int)));
        connect(&sampleRateBox, SIGNAL(currentIndexChanged(int)), this, SLOT(sampleRateBoxIndexChanged(int)));
        connect(&bufferSizeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(bufferSizeBoxIndexChanged(int)));
        connect(&okBox, SIGNAL(accepted()), this, SLOT(setupDone()));

        // fill up default values
        reenumerateSoundcards(false);
    }

    int exec() override
    {
        const bool wasFirstRun = firstRun;

        if (firstRun)
        {
            firstRun = false;
            setWindowTitle("Welcome");
        }
        else
        {
            labelWelcome.hide();
            setWindowTitle("Settings");
        }

        const int ret = QDialog::exec();

        // if selection is unsucessful, dont allow to close
        if (! isValid())
        {
            allowClose = false;
            firstRun = wasFirstRun;
        }

        return ret;
    }

    bool isValid() const
    {
        if (deviceBox.currentText().isEmpty())
            return false;
        if (sampleRateBox.count() == 0 || sampleRateBox.currentIndex() < 0)
            return false;
        if (bufferSizeBox.count() == 0 || bufferSizeBox.currentIndex() < 0)
            return false;
        return true;
    }

    bool getSelected(QString& device, unsigned& rate, unsigned& bufsize)
    {
        if (! isValid())
            return false;

        device = deviceBox.currentData().toString();
        rate = sampleRateBox.currentData().toUInt();
        bufsize = bufferSizeBox.currentData().toUInt();
        return true;
    }

    void reenumerateSoundcards(const bool restore)
    {
        validRates = false;

        QString olddevice;

        if (restore)
            olddevice = deviceBox.currentText();

        deviceBox.clear();
        inputNames.clear();
        inputIds.clear();
        outputNames.clear();
        outputIds.clear();

        enumerateSoundcards(inputNames, outputNames, inputIds, outputIds);

#if 0
        printf("Got %lu inputs %lu outputs\n", inputNames.size(), outputNames.size());
        for (size_t i=0; i<inputNames.size(); ++i)
            printf("input #%lu: %s\n", i+1, inputNames[i].c_str());
        for (size_t i=0; i<outputNames.size(); ++i)
            printf("output #%lu: %s\n", i+1, outputNames[i].c_str());
#endif

        // inputNames.appendNumbersToDuplicates(false, true);
        // outputNames.appendNumbersToDuplicates(false, true);

        int i, numNames = outputNames.size();

        // 1st pass, add USB cards
        i=0;
        for (; i<numNames; ++i)
        {
            const QString& name(QString::fromStdString(outputNames[i]));

            if (name.startsWith("Dummy, Dummy PCM"))
                continue;
            if (name.startsWith("Loopback, Loopback PCM"))
                continue;

            if (name.contains("USB"))
                deviceBox.addItem(name, QString::fromStdString(outputIds[i]));
        }

        // 2nd pass, add non-HDMI cards
        i=0;
        for (; i<numNames; ++i)
        {
            const QString& name(QString::fromStdString(outputNames[i]));

            if (name.startsWith("Dummy, Dummy PCM"))
                continue;
            if (name.startsWith("Loopback, Loopback PCM"))
                continue;
            if (name.contains("USB"))
                continue;
            if (name.contains("HDMI"))
                continue;

            deviceBox.addItem(name, QString::fromStdString(outputIds[i]));
        }

        // 2rd pass, add HDMI ones
        i=0;
        for (; i<numNames; ++i)
        {
            const QString& name(QString::fromStdString(outputNames[i]));

            if (name.startsWith("Dummy, Dummy PCM"))
                continue;
            if (name.startsWith("Loopback, Loopback PCM"))
                continue;
            if (name.contains("USB"))
                continue;

            if (name.contains("HDMI"))
                deviceBox.addItem(name, QString::fromStdString(outputIds[i]));
        }

        if (i == 0)
        {
            outputNames.push_back("Default");
            deviceBox.addItem("Default", QString("default"));
            sampleRateBox.clear();
            sampleRateBox.addItem("48000", -1);
            bufferSizeBox.clear();
            bufferSizeBox.addItem("256", -1);
            return;
        }

        validRates = true;
        refillDeviceOptions(restore, 0);
    }

private:
    void refillDeviceOptions(const bool restore, const int index)
    {
        unsigned oldrate;
        unsigned oldbufsize;
        unsigned minChansOut;
        unsigned maxChansOut;
        unsigned minChansIn;
        unsigned maxChansIn;
        std::vector<unsigned> bufsizes;
        std::vector<unsigned> rates;

        if (restore)
        {
            oldrate = sampleRateBox.currentData().toUInt();
            oldbufsize = bufferSizeBox.currentData().toUInt();
        }
        else
        {
            oldrate = oldbufsize = 0;
        }

        sampleRateBox.clear();
        bufferSizeBox.clear();

        getDeviceProperties(deviceBox.itemData(index).toString().toUtf8(),
                            minChansOut, maxChansOut, minChansIn, maxChansIn,
                            bufsizes, rates,
                            true, true);

        for (const unsigned rate : rates)
        {
            sampleRateBox.addItem(QString::number(rate), rate);

            if (oldrate > 1 && rate == oldrate)
            {
                oldrate = 1;
                sampleRateBox.blockSignals(true);
                sampleRateBox.setCurrentIndex(sampleRateBox.count()-1);
                sampleRateBox.blockSignals(false);
            }
        }

        for (const unsigned bufsize : bufsizes)
        {
            bufferSizeBox.addItem(QString::number(bufsize), bufsize);

            if (oldbufsize > 1 && bufsize == oldbufsize)
            {
                oldbufsize = 1;
                bufferSizeBox.blockSignals(true);
                bufferSizeBox.setCurrentIndex(bufferSizeBox.count()-1);
                bufferSizeBox.blockSignals(false);
            }
        }

        // no options set yet, try to find best available
        if (oldrate == 0)
        {
            unsigned nextrate = 0;
            for (unsigned tryrate : { 48000, 441000, 96000, 88200 })
            {
                if (std::find(rates.begin(), rates.end(), tryrate) != rates.end())
                {
                    nextrate = tryrate;
                    break;
                }
            }

            sampleRateBox.blockSignals(true);
            if (nextrate != 0)
                sampleRateBox.setCurrentIndex(sampleRateBox.findData(nextrate));
            else
                sampleRateBox.setCurrentIndex(0);
            sampleRateBox.blockSignals(false);
        }

        if (oldbufsize == 0)
        {
            unsigned nextbufsize = 0;
            std::vector<unsigned> trybufsizes;

            // prefer bufsize depending on device name
            const QString devicename(deviceBox.currentText());

             if (devicename.contains("HDMI"))
                trybufsizes = { 512, 1024, 256, 2048 };
             else if (devicename.contains("Intel"))
                trybufsizes = { 1024, 512, 2048, 256, 128 };
             else if (devicename.contains("PCH"))
                trybufsizes = { 256, 512, 128, 1024, 2048 };
             else
                trybufsizes = { 128, 256, 512, 64, 1024, 32, 2048, 4096, 16 };

            for (unsigned trybufsize : trybufsizes)
            {
                if (std::find(bufsizes.begin(), bufsizes.end(), trybufsize) != bufsizes.end())
                {
                    nextbufsize = trybufsize;
                    break;
                }
            }

            bufferSizeBox.blockSignals(true);
            if (nextbufsize != 0)
                bufferSizeBox.setCurrentIndex(bufferSizeBox.findData(nextbufsize));
            else
                bufferSizeBox.setCurrentIndex(0);
            bufferSizeBox.blockSignals(false);
        }
    }

protected:
    // do not allow to close as error/rejection
    void closeEvent(QCloseEvent* const event) override
    {
        if (allowClose)
            QDialog::closeEvent(event);
        else
            event->ignore();
    }

    void reject() override
    {
        if (allowClose)
            QDialog::reject();
    }

private Q_SLOTS:
    void deviceIndexChanged(const int index)
    {
        if (! validRates)
            return;
        refillDeviceOptions(true, index);
    }

    void sampleRateBoxIndexChanged(const int index)
    {
        if (! validRates)
            return;
    }

    void bufferSizeBoxIndexChanged(const int index)
    {
        if (! validRates)
            return;
    }

    void setupDone()
    {
        allowClose = true;
        accept();
    }
};
