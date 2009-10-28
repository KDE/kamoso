#include <kdialog.h>
#include <klocale.h>

/********************************************************************************
** Form generated from reading UI file 'pictureConfig.ui'
**
** Created: Fri Oct 16 12:23:47 2009
**      by: Qt User Interface Compiler version 4.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PICTURECONFIG_H
#define UI_PICTURECONFIG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_pictureConfigWidget
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QDoubleSpinBox *kcfg_photoTime;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *pictureConfigWidget)
    {
        if (pictureConfigWidget->objectName().isEmpty())
            pictureConfigWidget->setObjectName(QString::fromUtf8("pictureConfigWidget"));
        pictureConfigWidget->resize(400, 300);
        verticalLayout = new QVBoxLayout(pictureConfigWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(pictureConfigWidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        kcfg_photoTime = new QDoubleSpinBox(pictureConfigWidget);
        kcfg_photoTime->setObjectName(QString::fromUtf8("kcfg_photoTime"));

        horizontalLayout->addWidget(kcfg_photoTime);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer = new QSpacerItem(20, 248, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(pictureConfigWidget);

        QMetaObject::connectSlotsByName(pictureConfigWidget);
    } // setupUi

    void retranslateUi(QWidget *pictureConfigWidget)
    {
        pictureConfigWidget->setWindowTitle(tr2i18n("Form", 0));
        label->setText(tr2i18n("Time until the photo is taken:", 0));
    } // retranslateUi

};

namespace Ui {
    class pictureConfigWidget: public Ui_pictureConfigWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // PICTURECONFIG_H

