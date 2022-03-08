/*
 */

#pragma once

#include <QtGui/QCloseEvent>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
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
    QPushButton deviceButton;

    CollapsibleBox advancedBox;
    QGridLayout advancedLayout;

    QLabel sampleRateLabel;
    QComboBox sampleRateBox;

    QLabel bufferSizeLabel;
    QComboBox bufferSizeBox;

    QDialogButtonBox okBox;

    QString cachedDeviceName;
    DeviceProperties cachedDeviceProps;

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
        deviceButton(this),
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
        deviceButton.setIcon(QIcon::fromTheme("view-refresh"));
        sampleRateLabel.setText("Sample Rate:");
        bufferSizeLabel.setText("Buffer Size:");
        okBox.setCenterButtons(true);
        okBox.setOrientation(Qt::Horizontal);
        okBox.setStandardButtons(QDialogButtonBox::Ok);

        // no focus please
        deviceBox.setFocusPolicy(Qt::FocusPolicy::NoFocus);
        deviceButton.setFocusPolicy(Qt::FocusPolicy::NoFocus);
        sampleRateBox.setFocusPolicy(Qt::FocusPolicy::NoFocus);
        bufferSizeBox.setFocusPolicy(Qt::FocusPolicy::NoFocus);
        okBox.setFocusPolicy(Qt::FocusPolicy::NoFocus);

        // setup device layout
        deviceLabel.setAlignment(Qt::AlignRight);
        deviceLabel.setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        deviceBox.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        deviceButton.setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        deviceLayout.addWidget(&deviceLabel);
        deviceLayout.addWidget(&deviceBox);
        deviceLayout.addWidget(&deviceButton);

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
        connect(&deviceBox, SIGNAL(currentIndexChanged(int)), this, SLOT(deviceIndexChanged()));
        connect(&deviceButton, SIGNAL(clicked()), this, SLOT(reenumerateSoundcards()));
        connect(&okBox, SIGNAL(accepted()), this, SLOT(setupDone()));
        connect(&okBox, SIGNAL(rejected()), this, SLOT(reject()));

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
        if (wasFirstRun && ! isValid())
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

    // NOTE you MUST use this device settings if this returns successful
    bool getSelected(QString& device, unsigned& rate, unsigned& bufsize)
    {
        if (! isValid())
            return false;

        device = deviceBox.currentData().toString();
        rate = sampleRateBox.currentData().toUInt();
        bufsize = bufferSizeBox.currentData().toUInt();

        if (cachedDeviceName != device)
        {
            // store current device info, as we cannot query current running device later on
            cachedDeviceName = device;
            getDeviceProperties(device.toUtf8(), true, true, cachedDeviceProps);
        }

        return true;
    }

    void setCancellable(const bool cancellable)
    {
        if (cancellable)
        {
            allowClose = true;
            firstRun = false;
            okBox.setStandardButtons(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
        }
        else
        {
            okBox.setStandardButtons(QDialogButtonBox::Ok);
        }
    }

public Q_SLOTS:
    void reenumerateSoundcards(bool restore = true)
    {
        validRates = false;

        QString olddevice;

        if (restore)
            olddevice = deviceBox.currentText();

        deviceBox.blockSignals(true);
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

        // 3rd pass, add HDMI ones
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
            deviceBox.blockSignals(false);
            return;
        }

        validRates = true;

        int index;
        if (restore && (index = deviceBox.findText(olddevice)) >= 0)
        {
            deviceBox.setCurrentIndex(index);
        }
        else
        {
            restore = false;
            deviceBox.setCurrentIndex(0);
        }

        deviceBox.blockSignals(false);

        refillDeviceOptions(restore);
    }

private:
    void refillDeviceOptions(const bool restore)
    {
        const QString device(deviceBox.currentData().toString());
        unsigned oldrate;
        unsigned oldbufsize;
        DeviceProperties props;
        bool hasrate = false, hasbufsize = false;

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

        getDeviceProperties(device.toUtf8(), true, true, props);

        if (props.rates.size() == 0 && cachedDeviceName == device)
            props = cachedDeviceProps;

        for (const unsigned rate : props.rates)
        {
            sampleRateBox.addItem(QString::number(rate), rate);

            if (rate == oldrate)
            {
                hasrate = true;
                sampleRateBox.setCurrentIndex(sampleRateBox.count()-1);
            }
        }

        for (const unsigned bufsize : props.bufsizes)
        {
            bufferSizeBox.addItem(QString::number(bufsize), bufsize);

            if (bufsize == oldbufsize)
            {
                hasbufsize = true;
                bufferSizeBox.setCurrentIndex(bufferSizeBox.count()-1);
            }
        }

        // no options set yet, try to find best available
        if (! hasrate)
        {
            unsigned nextrate = 0;
            for (unsigned tryrate : { 48000, 441000, 96000, 88200 })
            {
                if (std::find(props.rates.begin(), props.rates.end(), tryrate) != props.rates.end())
                {
                    nextrate = tryrate;
                    break;
                }
            }

            if (nextrate != 0)
                sampleRateBox.setCurrentIndex(sampleRateBox.findData(nextrate));
            else
                sampleRateBox.setCurrentIndex(0);
        }

        if (! hasbufsize)
        {
            unsigned nextbufsize = 0;
            std::vector<unsigned> trybufsizes;

            // prefer bufsize depending on device name
            const QString devicename(deviceBox.currentText());

             if (devicename.contains("HDMI"))
                trybufsizes = { 512, 1024, 256, 2048 };
             else if (devicename.contains("HDA"))
                trybufsizes = { 1024, 512, 2048, 256, 128 };
             else if (devicename.contains("PCH"))
                trybufsizes = { 256, 512, 128, 1024, 2048 };
             else
                trybufsizes = { 128, 256, 512, 64, 1024, 32, 2048, 4096, 16 };

            for (unsigned trybufsize : trybufsizes)
            {
                if (std::find(props.bufsizes.begin(), props.bufsizes.end(), trybufsize) != props.bufsizes.end())
                {
                    nextbufsize = trybufsize;
                    break;
                }
            }

            if (nextbufsize != 0)
                bufferSizeBox.setCurrentIndex(bufferSizeBox.findData(nextbufsize));
            else
                bufferSizeBox.setCurrentIndex(0);
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

protected Q_SLOTS:
    void reject() override
    {
        if (allowClose)
            QDialog::reject();
    }

private Q_SLOTS:
    void deviceIndexChanged()
    {
        if (! validRates)
            return;
        refillDeviceOptions(true);
    }

    void setupDone()
    {
        allowClose = true;
        accept();
    }
};
