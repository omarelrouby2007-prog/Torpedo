# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'main_window.ui'
##
## Created by: Qt User Interface Compiler version 6.12.0
##
## WARNING! All changes made in this file will be lost when recompiling UI file!
################################################################################

from PySide6.QtCore import (QCoreApplication, QDate, QDateTime, QLocale,
    QMetaObject, QObject, QPoint, QRect,
    QSize, QTime, QUrl, Qt)
from PySide6.QtGui import (QBrush, QColor, QConicalGradient, QCursor,
    QFont, QFontDatabase, QGradient, QIcon,
    QImage, QKeySequence, QLinearGradient, QPainter,
    QPalette, QPixmap, QRadialGradient, QTransform)
from PySide6.QtWidgets import (QApplication, QLabel, QListWidget, QListWidgetItem,
    QSizePolicy, QVBoxLayout, QWidget)

class Ui_Form(object):
    def setupUi(self, Form):
        if not Form.objectName():
            Form.setObjectName(u"Form")
        Form.resize(665, 480)
        self.verticalLayout = QVBoxLayout(Form)
        self.verticalLayout.setObjectName(u"verticalLayout")
        self.videoLabel = QLabel(Form)
        self.videoLabel.setObjectName(u"videoLabel")

        self.verticalLayout.addWidget(self.videoLabel)

        self.statusLabel = QLabel(Form)
        self.statusLabel.setObjectName(u"statusLabel")

        self.verticalLayout.addWidget(self.statusLabel)

        self.frontSensor = QLabel(Form)
        self.frontSensor.setObjectName(u"frontSensor")

        self.verticalLayout.addWidget(self.frontSensor)

        self.rightSensor = QLabel(Form)
        self.rightSensor.setObjectName(u"rightSensor")

        self.verticalLayout.addWidget(self.rightSensor)

        self.leftSensor = QLabel(Form)
        self.leftSensor.setObjectName(u"leftSensor")

        self.verticalLayout.addWidget(self.leftSensor)

        self.headingDirection = QLabel(Form)
        self.headingDirection.setObjectName(u"headingDirection")

        self.verticalLayout.addWidget(self.headingDirection)

        self.batteryLevel = QLabel(Form)
        self.batteryLevel.setObjectName(u"batteryLevel")

        self.verticalLayout.addWidget(self.batteryLevel)

        self.listWidget = QListWidget(Form)
        self.listWidget.setObjectName(u"listWidget")

        self.verticalLayout.addWidget(self.listWidget)


        self.retranslateUi(Form)

        QMetaObject.connectSlotsByName(Form)
    # setupUi

    def retranslateUi(self, Form):
        Form.setWindowTitle(QCoreApplication.translate("Form", u"Form", None))
        self.videoLabel.setText(QCoreApplication.translate("Form", u"Waiting for camera...", None))
        self.statusLabel.setText(QCoreApplication.translate("Form", u"Status: not connected", None))
        self.frontSensor.setText(QCoreApplication.translate("Form", u"Front: ", None))
        self.rightSensor.setText(QCoreApplication.translate("Form", u"Right: ", None))
        self.leftSensor.setText(QCoreApplication.translate("Form", u"Left: ", None))
        self.headingDirection.setText(QCoreApplication.translate("Form", u"Direction:", None))
        self.batteryLevel.setText(QCoreApplication.translate("Form", u"Battery:", None))
    # retranslateUi

