#include <kdialog.h>
#include <klocale.h>

/********************************************************************************
** Form generated from reading UI file 'mainWidget.ui'
**
** Created: Fri Oct 16 12:23:47 2009
**      by: Qt User Interface Compiler version 4.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWIDGET_H
#define UI_MAINWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "kcombobox.h"
#include "timedpushbutton.h"

QT_BEGIN_NAMESPACE

class Ui_mainWidget
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *rightHorizSpacer;
    QLabel *chooseWebcamLbl;
    KComboBox *webcamCombo;
    QSpacerItem *sleftZeroSpacer;
    QHBoxLayout *firstRow;
    QWidget *leftSpot;
    QSpacerItem *horizontalSpacer;
    QWidget *centralSpot;
    QSpacerItem *horizontalSpacer_2;
    QWidget *rightSpot;
    QHBoxLayout *secondRow;
    QGroupBox *modes;
    QGroupBox *actions;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *pushButton;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *configure;
    QHBoxLayout *thirdRow;
    TimedPushButton *scrollLeft;
    TimedPushButton *scrollRight;

    void setupUi(QWidget *mainWidget)
    {
        if (mainWidget->objectName().isEmpty())
            mainWidget->setObjectName(QString::fromUtf8("mainWidget"));
        mainWidget->resize(737, 679);
        verticalLayout = new QVBoxLayout(mainWidget);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        rightHorizSpacer = new QSpacerItem(40, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(rightHorizSpacer);

        chooseWebcamLbl = new QLabel(mainWidget);
        chooseWebcamLbl->setObjectName(QString::fromUtf8("chooseWebcamLbl"));

        horizontalLayout->addWidget(chooseWebcamLbl);

        webcamCombo = new KComboBox(mainWidget);
        webcamCombo->setObjectName(QString::fromUtf8("webcamCombo"));

        horizontalLayout->addWidget(webcamCombo);

        sleftZeroSpacer = new QSpacerItem(40, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(sleftZeroSpacer);


        verticalLayout->addLayout(horizontalLayout);

        firstRow = new QHBoxLayout();
        firstRow->setObjectName(QString::fromUtf8("firstRow"));
        firstRow->setSizeConstraint(QLayout::SetFixedSize);
        leftSpot = new QWidget(mainWidget);
        leftSpot->setObjectName(QString::fromUtf8("leftSpot"));

        firstRow->addWidget(leftSpot);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        firstRow->addItem(horizontalSpacer);

        centralSpot = new QWidget(mainWidget);
        centralSpot->setObjectName(QString::fromUtf8("centralSpot"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(centralSpot->sizePolicy().hasHeightForWidth());
        centralSpot->setSizePolicy(sizePolicy);
        centralSpot->setMinimumSize(QSize(640, 480));
        centralSpot->setMaximumSize(QSize(640, 480));

        firstRow->addWidget(centralSpot);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        firstRow->addItem(horizontalSpacer_2);

        rightSpot = new QWidget(mainWidget);
        rightSpot->setObjectName(QString::fromUtf8("rightSpot"));

        firstRow->addWidget(rightSpot);


        verticalLayout->addLayout(firstRow);

        secondRow = new QHBoxLayout();
        secondRow->setObjectName(QString::fromUtf8("secondRow"));
        secondRow->setContentsMargins(-1, 5, -1, -1);
        modes = new QGroupBox(mainWidget);
        modes->setObjectName(QString::fromUtf8("modes"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(modes->sizePolicy().hasHeightForWidth());
        modes->setSizePolicy(sizePolicy1);

        secondRow->addWidget(modes);

        actions = new QGroupBox(mainWidget);
        actions->setObjectName(QString::fromUtf8("actions"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(actions->sizePolicy().hasHeightForWidth());
        actions->setSizePolicy(sizePolicy2);
        horizontalLayout_2 = new QHBoxLayout(actions);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_3 = new QSpacerItem(245, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        pushButton = new QPushButton(actions);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        horizontalLayout_2->addWidget(pushButton);

        horizontalSpacer_4 = new QSpacerItem(245, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);


        secondRow->addWidget(actions);

        configure = new QPushButton(mainWidget);
        configure->setObjectName(QString::fromUtf8("configure"));
        configure->setFocusPolicy(Qt::NoFocus);
        configure->setIconSize(QSize(20, 20));

        secondRow->addWidget(configure);


        verticalLayout->addLayout(secondRow);

        thirdRow = new QHBoxLayout();
        thirdRow->setObjectName(QString::fromUtf8("thirdRow"));
        scrollLeft = new TimedPushButton(mainWidget);
        scrollLeft->setObjectName(QString::fromUtf8("scrollLeft"));
        QSizePolicy sizePolicy3(QSizePolicy::Maximum, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(scrollLeft->sizePolicy().hasHeightForWidth());
        scrollLeft->setSizePolicy(sizePolicy3);

        thirdRow->addWidget(scrollLeft);

        scrollRight = new TimedPushButton(mainWidget);
        scrollRight->setObjectName(QString::fromUtf8("scrollRight"));
        sizePolicy3.setHeightForWidth(scrollRight->sizePolicy().hasHeightForWidth());
        scrollRight->setSizePolicy(sizePolicy3);

        thirdRow->addWidget(scrollRight);


        verticalLayout->addLayout(thirdRow);


        retranslateUi(mainWidget);

        QMetaObject::connectSlotsByName(mainWidget);
    } // setupUi

    void retranslateUi(QWidget *mainWidget)
    {
        mainWidget->setWindowTitle(tr2i18n("Form", 0));
        chooseWebcamLbl->setText(tr2i18n("Choose the webcam to use:", 0));
        modes->setTitle(QString());
        actions->setTitle(QString());
        pushButton->setText(tr2i18n("PushButton", 0));
        configure->setText(QString());
        scrollLeft->setText(tr2i18n("<", 0));
        scrollRight->setText(tr2i18n(">", 0));
    } // retranslateUi

};

namespace Ui {
    class mainWidget: public Ui_mainWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // MAINWIDGET_H

