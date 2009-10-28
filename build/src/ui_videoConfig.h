#include <kdialog.h>
#include <klocale.h>

/********************************************************************************
** Form generated from reading UI file 'videoConfig.ui'
**
** Created: Tue Oct 27 18:22:32 2009
**      by: Qt User Interface Compiler version 4.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIDEOCONFIG_H
#define UI_VIDEOCONFIG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSlider>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_webcamConfigWidget
{
public:
    QGroupBox *adjustEnable;
    QGridLayout *gridLayout;
    QSlider *hueSlider;
    QSlider *gammaSlider;
    QSlider *contrastSlider;
    QLabel *gammaLbl;
    QLabel *contrastLbl;
    QLabel *saturationLbl;
    QLabel *hueLbl;
    QSlider *saturationSlider;
    QSlider *brightnessSlider;
    QLabel *brightnessLbl;

    void setupUi(QWidget *webcamConfigWidget)
    {
        if (webcamConfigWidget->objectName().isEmpty())
            webcamConfigWidget->setObjectName(QString::fromUtf8("webcamConfigWidget"));
        webcamConfigWidget->resize(413, 256);
        adjustEnable = new QGroupBox(webcamConfigWidget);
        adjustEnable->setObjectName(QString::fromUtf8("adjustEnable"));
        adjustEnable->setEnabled(true);
        adjustEnable->setGeometry(QRect(0, 0, 381, 237));
        adjustEnable->setCheckable(false);
        adjustEnable->setChecked(false);
        gridLayout = new QGridLayout(adjustEnable);
        gridLayout->setSpacing(0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        hueSlider = new QSlider(adjustEnable);
        hueSlider->setObjectName(QString::fromUtf8("hueSlider"));
        hueSlider->setMaximum(360);
        hueSlider->setOrientation(Qt::Horizontal);
        hueSlider->setTickPosition(QSlider::NoTicks);
        hueSlider->setTickInterval(60);

        gridLayout->addWidget(hueSlider, 2, 1, 1, 1);

        gammaSlider = new QSlider(adjustEnable);
        gammaSlider->setObjectName(QString::fromUtf8("gammaSlider"));
        gammaSlider->setMaximum(500);
        gammaSlider->setOrientation(Qt::Horizontal);
        gammaSlider->setTickPosition(QSlider::NoTicks);
        gammaSlider->setTickInterval(50);

        gridLayout->addWidget(gammaSlider, 6, 1, 1, 1);

        contrastSlider = new QSlider(adjustEnable);
        contrastSlider->setObjectName(QString::fromUtf8("contrastSlider"));
        contrastSlider->setMaximum(200);
        contrastSlider->setOrientation(Qt::Horizontal);
        contrastSlider->setTickPosition(QSlider::NoTicks);
        contrastSlider->setTickInterval(100);

        gridLayout->addWidget(contrastSlider, 3, 1, 1, 1);

        gammaLbl = new QLabel(adjustEnable);
        gammaLbl->setObjectName(QString::fromUtf8("gammaLbl"));

        gridLayout->addWidget(gammaLbl, 6, 0, 1, 1);

        contrastLbl = new QLabel(adjustEnable);
        contrastLbl->setObjectName(QString::fromUtf8("contrastLbl"));

        gridLayout->addWidget(contrastLbl, 3, 0, 1, 1);

        saturationLbl = new QLabel(adjustEnable);
        saturationLbl->setObjectName(QString::fromUtf8("saturationLbl"));

        gridLayout->addWidget(saturationLbl, 5, 0, 1, 1);

        hueLbl = new QLabel(adjustEnable);
        hueLbl->setObjectName(QString::fromUtf8("hueLbl"));

        gridLayout->addWidget(hueLbl, 2, 0, 1, 1);

        saturationSlider = new QSlider(adjustEnable);
        saturationSlider->setObjectName(QString::fromUtf8("saturationSlider"));
        saturationSlider->setMaximum(300);
        saturationSlider->setOrientation(Qt::Horizontal);
        saturationSlider->setTickPosition(QSlider::NoTicks);
        saturationSlider->setTickInterval(100);

        gridLayout->addWidget(saturationSlider, 5, 1, 1, 1);

        brightnessSlider = new QSlider(adjustEnable);
        brightnessSlider->setObjectName(QString::fromUtf8("brightnessSlider"));
        brightnessSlider->setMaximum(200);
        brightnessSlider->setOrientation(Qt::Horizontal);
        brightnessSlider->setTickPosition(QSlider::NoTicks);
        brightnessSlider->setTickInterval(100);

        gridLayout->addWidget(brightnessSlider, 0, 1, 1, 1);

        brightnessLbl = new QLabel(adjustEnable);
        brightnessLbl->setObjectName(QString::fromUtf8("brightnessLbl"));

        gridLayout->addWidget(brightnessLbl, 0, 0, 1, 1);


        retranslateUi(webcamConfigWidget);

        QMetaObject::connectSlotsByName(webcamConfigWidget);
    } // setupUi

    void retranslateUi(QWidget *webcamConfigWidget)
    {
        webcamConfigWidget->setWindowTitle(tr2i18n("Form", 0));
        adjustEnable->setTitle(QString());
        gammaLbl->setText(tr2i18n("Gamma", 0));
        contrastLbl->setText(tr2i18n("Contrast", 0));
        saturationLbl->setText(tr2i18n("Saturation", 0));
        hueLbl->setText(tr2i18n("Hue", 0));
        brightnessLbl->setText(tr2i18n("Brightness", 0));
    } // retranslateUi

};

namespace Ui {
    class webcamConfigWidget: public Ui_webcamConfigWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // VIDEOCONFIG_H

