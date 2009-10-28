#include <kdialog.h>
#include <klocale.h>

/********************************************************************************
** Form generated from reading UI file 'videoInfo.ui'
**
** Created: Fri Oct 16 12:23:37 2009
**      by: Qt User Interface Compiler version 4.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIDEOINFO_H
#define UI_VIDEOINFO_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFormLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_videoForm
{
public:
    QLabel *label;
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLabel *label_2;
    QLineEdit *titleText;
    QLabel *label_4;
    QLineEdit *tagText;
    QLabel *label_3;
    QPlainTextEdit *descriptionText;

    void setupUi(QWidget *videoForm)
    {
        if (videoForm->objectName().isEmpty())
            videoForm->setObjectName(QString::fromUtf8("videoForm"));
        videoForm->resize(405, 266);
        label = new QLabel(videoForm);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 10, 221, 16));
        QFont font;
        font.setPointSize(14);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        formLayoutWidget = new QWidget(videoForm);
        formLayoutWidget->setObjectName(QString::fromUtf8("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(0, 30, 401, 231));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
        formLayout->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(formLayoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label_2);

        titleText = new QLineEdit(formLayoutWidget);
        titleText->setObjectName(QString::fromUtf8("titleText"));

        formLayout->setWidget(0, QFormLayout::FieldRole, titleText);

        label_4 = new QLabel(formLayoutWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_4);

        tagText = new QLineEdit(formLayoutWidget);
        tagText->setObjectName(QString::fromUtf8("tagText"));

        formLayout->setWidget(1, QFormLayout::FieldRole, tagText);

        label_3 = new QLabel(formLayoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_3);

        descriptionText = new QPlainTextEdit(formLayoutWidget);
        descriptionText->setObjectName(QString::fromUtf8("descriptionText"));

        formLayout->setWidget(2, QFormLayout::FieldRole, descriptionText);


        retranslateUi(videoForm);

        QMetaObject::connectSlotsByName(videoForm);
    } // setupUi

    void retranslateUi(QWidget *videoForm)
    {
        videoForm->setWindowTitle(tr2i18n("Form", 0));
        label->setText(tr2i18n("Video Information:", 0));
        label_2->setText(tr2i18n("Title", 0));
        label_4->setText(tr2i18n("Tags:", 0));
        label_3->setText(tr2i18n("Desscription", 0));
    } // retranslateUi

};

namespace Ui {
    class videoForm: public Ui_videoForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // VIDEOINFO_H

