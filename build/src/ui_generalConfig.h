#include <kdialog.h>
#include <klocale.h>

/********************************************************************************
** Form generated from reading UI file 'generalConfig.ui'
**
** Created: Fri Oct 16 12:23:47 2009
**      by: Qt User Interface Compiler version 4.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GENERALCONFIG_H
#define UI_GENERALCONFIG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "kurlrequester.h"

QT_BEGIN_NAMESPACE

class Ui_generalConfigWidget
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    KUrlRequester *kcfg_saveUrl;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *generalConfigWidget)
    {
        if (generalConfigWidget->objectName().isEmpty())
            generalConfigWidget->setObjectName(QString::fromUtf8("generalConfigWidget"));
        generalConfigWidget->resize(400, 300);
        verticalLayout = new QVBoxLayout(generalConfigWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(generalConfigWidget);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        kcfg_saveUrl = new KUrlRequester(generalConfigWidget);
        kcfg_saveUrl->setObjectName(QString::fromUtf8("kcfg_saveUrl"));

        verticalLayout->addWidget(kcfg_saveUrl);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(generalConfigWidget);

        QMetaObject::connectSlotsByName(generalConfigWidget);
    } // setupUi

    void retranslateUi(QWidget *generalConfigWidget)
    {
        generalConfigWidget->setWindowTitle(tr2i18n("Form", 0));
        label->setText(tr2i18n("Directory where kamoso saves the photos", 0));
    } // retranslateUi

};

namespace Ui {
    class generalConfigWidget: public Ui_generalConfigWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // GENERALCONFIG_H

