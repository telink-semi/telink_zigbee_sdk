# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'bletest.ui'
##
## Created by: Qt User Interface Compiler version 6.4.2
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
from PySide6.QtWidgets import (QApplication, QComboBox, QGridLayout, QHBoxLayout,
    QLabel, QLineEdit, QListWidget, QListWidgetItem, QCheckBox,
    QMainWindow, QMenuBar, QPushButton, QScrollArea,
    QSizePolicy, QSpacerItem, QSplitter, QStatusBar,
    QTabWidget, QTextEdit, QVBoxLayout, QWidget)

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        if not MainWindow.objectName():
            MainWindow.setObjectName(u"MainWindow")
        MainWindow.resize(1277, 809)
        self.centralwidget = QWidget(MainWindow)
        self.centralwidget.setObjectName(u"centralwidget")
        self.gridLayout_6 = QGridLayout(self.centralwidget)
        self.gridLayout_6.setObjectName(u"gridLayout_6")
        self.label_connState = QLabel(self.centralwidget)
        self.label_connState.setObjectName(u"label_connState")
        palette = QPalette()
        brush = QBrush(QColor(255, 85, 0, 255))
        brush.setStyle(Qt.SolidPattern)
        palette.setBrush(QPalette.Active, QPalette.WindowText, brush)
        palette.setBrush(QPalette.Inactive, QPalette.WindowText, brush)
        brush1 = QBrush(QColor(120, 120, 120, 255))
        brush1.setStyle(Qt.SolidPattern)
        palette.setBrush(QPalette.Disabled, QPalette.WindowText, brush1)
        self.label_connState.setPalette(palette)
        self.label_connState.setCursor(QCursor(Qt.ArrowCursor))
        self.label_connState.setTextFormat(Qt.PlainText)

        self.gridLayout_6.addWidget(self.label_connState, 0, 0, 1, 1)

        self.horizontalLayout_23 = QHBoxLayout()
        self.horizontalLayout_23.setObjectName(u"horizontalLayout_23")
        self.pushButton_startScan = QPushButton(self.centralwidget)
        self.pushButton_startScan.setObjectName(u"pushButton_startScan")

        self.horizontalLayout_23.addWidget(self.pushButton_startScan)

        self.pushButton_startConnect = QPushButton(self.centralwidget)
        self.pushButton_startConnect.setObjectName(u"pushButton_startConnect")

        self.horizontalLayout_23.addWidget(self.pushButton_startConnect)

        self.pushButton_disconnect = QPushButton(self.centralwidget)
        self.pushButton_disconnect.setObjectName(u"pushButton_disconnect")
        self.pushButton_disconnect.setEnabled(True)

        self.horizontalLayout_23.addWidget(self.pushButton_disconnect)

        self.pushButton_enNotify = QPushButton(self.centralwidget)
        self.pushButton_enNotify.setObjectName(u"pushButton_enNotify")

        self.horizontalLayout_23.addWidget(self.pushButton_enNotify)

        self.pushButton_disNotify = QPushButton(self.centralwidget)
        self.pushButton_disNotify.setObjectName(u"pushButton_disNotify")

        self.horizontalLayout_23.addWidget(self.pushButton_disNotify)

        self.pushButton_attrRead = QPushButton(self.centralwidget)
        self.pushButton_attrRead.setObjectName(u"pushButton_attrRead")

        self.horizontalLayout_23.addWidget(self.pushButton_attrRead)


        self.gridLayout_6.addLayout(self.horizontalLayout_23, 1, 0, 1, 1)

        self.splitter_2 = QSplitter(self.centralwidget)
        self.splitter_2.setObjectName(u"splitter_2")
        self.splitter_2.setOrientation(Qt.Horizontal)
        self.lineEdit_attrWrite = QLineEdit(self.splitter_2)
        self.lineEdit_attrWrite.setObjectName(u"lineEdit_attrWrite")
        self.lineEdit_attrWrite.setMinimumSize(QSize(0, 30))
        self.splitter_2.addWidget(self.lineEdit_attrWrite)
        self.pushButton_attrWrite = QPushButton(self.splitter_2)
        self.pushButton_attrWrite.setObjectName(u"pushButton_attrWrite")
        self.splitter_2.addWidget(self.pushButton_attrWrite)

        self.gridLayout_6.addWidget(self.splitter_2, 2, 0, 1, 1)

        self.tabWidget = QTabWidget(self.centralwidget)
        self.tabWidget.setObjectName(u"tabWidget")
        self.GATT = QWidget()
        self.GATT.setObjectName(u"GATT")
        self.gridLayout_3 = QGridLayout(self.GATT)
        self.gridLayout_3.setObjectName(u"gridLayout_3")
        self.listWidget_gatt = QListWidget(self.GATT)
        self.listWidget_gatt.setObjectName(u"listWidget_gatt")

        self.gridLayout_3.addWidget(self.listWidget_gatt, 0, 0, 1, 1)

        self.tabWidget.addTab(self.GATT, "")
        self.Security = QWidget()
        self.Security.setObjectName(u"Security")
        self.gridLayout_5 = QGridLayout(self.Security)
        self.gridLayout_5.setObjectName(u"gridLayout_5")
        self.scrollArea = QScrollArea(self.Security)
        self.scrollArea.setObjectName(u"scrollArea")
        self.scrollArea.setWidgetResizable(True)
        self.scrollAreaWidgetContents_2 = QWidget()
        self.scrollAreaWidgetContents_2.setObjectName(u"scrollAreaWidgetContents_2")
        self.scrollAreaWidgetContents_2.setGeometry(QRect(0, 0, 1225, 588))
        self.gridLayout = QGridLayout(self.scrollAreaWidgetContents_2)
        self.gridLayout.setObjectName(u"gridLayout")
        self.label_seServiceuuid = QLabel(self.scrollAreaWidgetContents_2)
        self.label_seServiceuuid.setObjectName(u"label_seServiceuuid")
        font = QFont()
        font.setFamilies([u"Arial"])
        font.setPointSize(10)
        font.setBold(True)
        self.label_seServiceuuid.setFont(font)
        self.label_seServiceuuid.setLineWidth(1)

        self.gridLayout.addWidget(self.label_seServiceuuid, 0, 0, 1, 1)

        self.horizontalLayout_7 = QHBoxLayout()
        self.horizontalLayout_7.setObjectName(u"horizontalLayout_7")
        self.label_seCharaUuid = QLabel(self.scrollAreaWidgetContents_2)
        self.label_seCharaUuid.setObjectName(u"label_seCharaUuid")
        font1 = QFont()
        font1.setFamilies([u"Arial"])
        self.label_seCharaUuid.setFont(font1)

        self.horizontalLayout_7.addWidget(self.label_seCharaUuid)

        self.horizontalSpacer_7 = QSpacerItem(88, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_7.addItem(self.horizontalSpacer_7)

        self.label_sessionState = QLabel(self.scrollAreaWidgetContents_2)
        self.label_sessionState.setObjectName(u"label_sessionState")
        palette1 = QPalette()
        palette1.setBrush(QPalette.Active, QPalette.WindowText, brush)
        palette1.setBrush(QPalette.Inactive, QPalette.WindowText, brush)
        palette1.setBrush(QPalette.Disabled, QPalette.WindowText, brush1)
        self.label_sessionState.setPalette(palette1)

        self.horizontalLayout_7.addWidget(self.label_sessionState)

        self.horizontalSpacer_8 = QSpacerItem(318, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_7.addItem(self.horizontalSpacer_8)


        self.gridLayout.addLayout(self.horizontalLayout_7, 1, 0, 1, 1)

        self.horizontalLayout_4 = QHBoxLayout()
        self.horizontalLayout_4.setObjectName(u"horizontalLayout_4")
        self.label_keyMethod = QLabel(self.scrollAreaWidgetContents_2)
        self.label_keyMethod.setObjectName(u"label_keyMethod")

        self.horizontalLayout_4.addWidget(self.label_keyMethod)

        self.comboBox_keyMethod = QComboBox(self.scrollAreaWidgetContents_2)
        self.comboBox_keyMethod.addItem("")
        self.comboBox_keyMethod.addItem("")
        self.comboBox_keyMethod.addItem("")
        self.comboBox_keyMethod.setObjectName(u"comboBox_keyMethod")

        self.horizontalLayout_4.addWidget(self.comboBox_keyMethod)

        self.horizontalSpacer = QSpacerItem(562, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_4.addItem(self.horizontalSpacer)


        self.gridLayout.addLayout(self.horizontalLayout_4, 2, 0, 1, 1)

        self.horizontalLayout = QHBoxLayout()
        self.horizontalLayout.setObjectName(u"horizontalLayout")
        self.label_keyEnum = QLabel(self.scrollAreaWidgetContents_2)
        self.label_keyEnum.setObjectName(u"label_keyEnum")

        self.horizontalLayout.addWidget(self.label_keyEnum)

        self.comboBox_keyEnum = QComboBox(self.scrollAreaWidgetContents_2)
        self.comboBox_keyEnum.addItem("")
        self.comboBox_keyEnum.addItem("")
        self.comboBox_keyEnum.addItem("")
        self.comboBox_keyEnum.addItem("")
        self.comboBox_keyEnum.addItem("")
        self.comboBox_keyEnum.addItem("")
        self.comboBox_keyEnum.setObjectName(u"comboBox_keyEnum")

        self.horizontalLayout.addWidget(self.comboBox_keyEnum)

        self.horizontalSpacer_2 = QSpacerItem(398, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout.addItem(self.horizontalSpacer_2)


        self.gridLayout.addLayout(self.horizontalLayout, 3, 0, 1, 1)

        self.horizontalLayout_2 = QHBoxLayout()
        self.horizontalLayout_2.setObjectName(u"horizontalLayout_2")
        self.label_preKey = QLabel(self.scrollAreaWidgetContents_2)
        self.label_preKey.setObjectName(u"label_preKey")

        self.horizontalLayout_2.addWidget(self.label_preKey)

        self.lineEdit_preKey = QLineEdit(self.scrollAreaWidgetContents_2)
        self.lineEdit_preKey.setObjectName(u"lineEdit_preKey")
        self.lineEdit_preKey.setMinimumSize(QSize(0, 30))

        self.horizontalLayout_2.addWidget(self.lineEdit_preKey)

        self.comboBox_derived = QComboBox(self.scrollAreaWidgetContents_2)
        self.comboBox_derived.addItem("")
        self.comboBox_derived.addItem("")
        self.comboBox_derived.setObjectName(u"comboBox_derived")

        self.horizontalLayout_2.addWidget(self.comboBox_derived)

        self.horizontalSpacer_3 = QSpacerItem(78, 18, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_2.addItem(self.horizontalSpacer_3)


        self.gridLayout.addLayout(self.horizontalLayout_2, 4, 0, 1, 1)

        self.horizontalLayout_3 = QHBoxLayout()
        self.horizontalLayout_3.setObjectName(u"horizontalLayout_3")
        self.label_seqNum = QLabel(self.scrollAreaWidgetContents_2)
        self.label_seqNum.setObjectName(u"label_seqNum")

        self.horizontalLayout_3.addWidget(self.label_seqNum)

        self.lineEdit_keySeqNum = QLineEdit(self.scrollAreaWidgetContents_2)
        self.lineEdit_keySeqNum.setObjectName(u"lineEdit_keySeqNum")
        self.lineEdit_keySeqNum.setMinimumSize(QSize(0, 30))

        self.horizontalLayout_3.addWidget(self.lineEdit_keySeqNum)

        self.horizontalSpacer_4 = QSpacerItem(638, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_3.addItem(self.horizontalSpacer_4)


        self.gridLayout.addLayout(self.horizontalLayout_3, 5, 0, 1, 1)

        self.horizontalLayout_5 = QHBoxLayout()
        self.horizontalLayout_5.setObjectName(u"horizontalLayout_5")
        self.pushButton_startSession = QPushButton(self.scrollAreaWidgetContents_2)
        self.pushButton_startSession.setObjectName(u"pushButton_startSession")

        self.horizontalLayout_5.addWidget(self.pushButton_startSession)

        self.checkBox_test = QCheckBox(self.scrollAreaWidgetContents_2)
        self.checkBox_test.setObjectName(u"checkBox_test")

        self.horizontalLayout_5.addWidget(self.checkBox_test)
        self.checkBox_test.setText(QCoreApplication.translate("MainWindow", u"Test Vector", None))

        self.horizontalSpacer_5 = QSpacerItem(798, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_5.addItem(self.horizontalSpacer_5)


        self.gridLayout.addLayout(self.horizontalLayout_5, 6, 0, 1, 1)

        self.horizontalLayout_6 = QHBoxLayout()
        self.horizontalLayout_6.setObjectName(u"horizontalLayout_6")
        self.label_dataSec = QLabel(self.scrollAreaWidgetContents_2)
        self.label_dataSec.setObjectName(u"label_dataSec")

        self.horizontalLayout_6.addWidget(self.label_dataSec)

        self.horizontalSpacer_6 = QSpacerItem(808, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_6.addItem(self.horizontalSpacer_6)

        self.pushButton_secClear = QPushButton(self.scrollAreaWidgetContents_2)
        self.pushButton_secClear.setObjectName(u"pushButton_secClear")

        self.horizontalLayout_6.addWidget(self.pushButton_secClear)


        self.gridLayout.addLayout(self.horizontalLayout_6, 7, 0, 1, 1)

        self.textEdit_dataSecurity = QTextEdit(self.scrollAreaWidgetContents_2)
        self.textEdit_dataSecurity.setObjectName(u"textEdit_dataSecurity")

        self.gridLayout.addWidget(self.textEdit_dataSecurity, 8, 0, 1, 1)

        self.scrollArea.setWidget(self.scrollAreaWidgetContents_2)

        self.gridLayout_5.addWidget(self.scrollArea, 0, 0, 1, 1)

        self.tabWidget.addTab(self.Security, "")
        self.Commission = QWidget()
        self.Commission.setObjectName(u"Commission")
        self.gridLayout_4 = QGridLayout(self.Commission)
        self.gridLayout_4.setObjectName(u"gridLayout_4")
        self.scrollArea_2 = QScrollArea(self.Commission)
        self.scrollArea_2.setObjectName(u"scrollArea_2")
        self.scrollArea_2.setEnabled(True)
        self.scrollArea_2.setWidgetResizable(True)
        self.scrollAreaWidgetContents_3 = QWidget()
        self.scrollAreaWidgetContents_3.setObjectName(u"scrollAreaWidgetContents_3")
        self.scrollAreaWidgetContents_3.setGeometry(QRect(0, 0, 1225, 588))
        self.gridLayout_2 = QGridLayout(self.scrollAreaWidgetContents_3)
        self.gridLayout_2.setObjectName(u"gridLayout_2")
        self.label_coServiceuuid = QLabel(self.scrollAreaWidgetContents_3)
        self.label_coServiceuuid.setObjectName(u"label_coServiceuuid")
        self.label_coServiceuuid.setFont(font)
        self.label_coServiceuuid.setLineWidth(1)

        self.gridLayout_2.addWidget(self.label_coServiceuuid, 0, 0, 1, 1)

        self.horizontalLayout_22 = QHBoxLayout()
        self.horizontalLayout_22.setObjectName(u"horizontalLayout_22")
        self.splitter_4 = QSplitter(self.scrollAreaWidgetContents_3)
        self.splitter_4.setObjectName(u"splitter_4")
        self.splitter_4.setOrientation(Qt.Vertical)
        self.label_exPanid = QLabel(self.splitter_4)
        self.label_exPanid.setObjectName(u"label_exPanid")
        self.splitter_4.addWidget(self.label_exPanid)
        self.label_shortPanid = QLabel(self.splitter_4)
        self.label_shortPanid.setObjectName(u"label_shortPanid")
        self.splitter_4.addWidget(self.label_shortPanid)
        self.label_nwkChannel = QLabel(self.splitter_4)
        self.label_nwkChannel.setObjectName(u"label_nwkChannel")
        self.splitter_4.addWidget(self.label_nwkChannel)
        self.label_nwkKey = QLabel(self.splitter_4)
        self.label_nwkKey.setObjectName(u"label_nwkKey")
        self.splitter_4.addWidget(self.label_nwkKey)
        self.label_linkkey = QLabel(self.splitter_4)
        self.label_linkkey.setObjectName(u"label_linkkey")
        self.splitter_4.addWidget(self.label_linkkey)
        self.label_27 = QLabel(self.splitter_4)
        self.label_27.setObjectName(u"label_27")
        self.splitter_4.addWidget(self.label_27)
        self.label_28 = QLabel(self.splitter_4)
        self.label_28.setObjectName(u"label_28")
        self.splitter_4.addWidget(self.label_28)
        self.label_devType = QLabel(self.splitter_4)
        self.label_devType.setObjectName(u"label_devType")
        self.splitter_4.addWidget(self.label_devType)
        self.label_nwkAddr = QLabel(self.splitter_4)
        self.label_nwkAddr.setObjectName(u"label_nwkAddr")
        self.splitter_4.addWidget(self.label_nwkAddr)
        self.label_joinMethod = QLabel(self.splitter_4)
        self.label_joinMethod.setObjectName(u"label_joinMethod")
        self.splitter_4.addWidget(self.label_joinMethod)
        self.label_ieeeAddr = QLabel(self.splitter_4)
        self.label_ieeeAddr.setObjectName(u"label_ieeeAddr")
        self.splitter_4.addWidget(self.label_ieeeAddr)
        self.label_txAddr = QLabel(self.splitter_4)
        self.label_txAddr.setObjectName(u"label_txAddr")
        self.splitter_4.addWidget(self.label_txAddr)
        self.label_updateId = QLabel(self.splitter_4)
        self.label_updateId.setObjectName(u"label_updateId")
        self.splitter_4.addWidget(self.label_updateId)
        self.label_keySeqNum = QLabel(self.splitter_4)
        self.label_keySeqNum.setObjectName(u"label_keySeqNum")
        self.splitter_4.addWidget(self.label_keySeqNum)
        self.label_adminKey = QLabel(self.splitter_4)
        self.label_adminKey.setObjectName(u"label_adminKey")
        self.splitter_4.addWidget(self.label_adminKey)

        self.horizontalLayout_22.addWidget(self.splitter_4)

        self.splitter_13 = QSplitter(self.scrollAreaWidgetContents_3)
        self.splitter_13.setObjectName(u"splitter_13")
        self.splitter_13.setOrientation(Qt.Vertical)
        self.lineEdit_exPanid = QLineEdit(self.splitter_13)
        self.lineEdit_exPanid.setObjectName(u"lineEdit_exPanid")
        sizePolicy = QSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.lineEdit_exPanid.sizePolicy().hasHeightForWidth())
        self.lineEdit_exPanid.setSizePolicy(sizePolicy)
        self.splitter_13.addWidget(self.lineEdit_exPanid)
        self.lineEdit_shortPanid = QLineEdit(self.splitter_13)
        self.lineEdit_shortPanid.setObjectName(u"lineEdit_shortPanid")
        sizePolicy.setHeightForWidth(self.lineEdit_shortPanid.sizePolicy().hasHeightForWidth())
        self.lineEdit_shortPanid.setSizePolicy(sizePolicy)
        self.splitter_13.addWidget(self.lineEdit_shortPanid)
        self.lineEdit_nwkChannel = QLineEdit(self.splitter_13)
        self.lineEdit_nwkChannel.setObjectName(u"lineEdit_nwkChannel")
        sizePolicy.setHeightForWidth(self.lineEdit_nwkChannel.sizePolicy().hasHeightForWidth())
        self.lineEdit_nwkChannel.setSizePolicy(sizePolicy)
        self.splitter_13.addWidget(self.lineEdit_nwkChannel)
        self.lineEdit_nwkKey = QLineEdit(self.splitter_13)
        self.lineEdit_nwkKey.setObjectName(u"lineEdit_nwkKey")
        sizePolicy.setHeightForWidth(self.lineEdit_nwkKey.sizePolicy().hasHeightForWidth())
        self.lineEdit_nwkKey.setSizePolicy(sizePolicy)
        self.splitter_13.addWidget(self.lineEdit_nwkKey)
        self.comboBox_linkkType = QComboBox(self.splitter_13)
        self.comboBox_linkkType.addItem("")
        self.comboBox_linkkType.addItem("")
        self.comboBox_linkkType.setObjectName(u"comboBox_linkkType")
        sizePolicy1 = QSizePolicy(QSizePolicy.Preferred, QSizePolicy.Expanding)
        sizePolicy1.setHorizontalStretch(0)
        sizePolicy1.setVerticalStretch(0)
        sizePolicy1.setHeightForWidth(self.comboBox_linkkType.sizePolicy().hasHeightForWidth())
        self.comboBox_linkkType.setSizePolicy(sizePolicy1)
        self.splitter_13.addWidget(self.comboBox_linkkType)
        self.comboBox_linkkLife = QComboBox(self.splitter_13)
        self.comboBox_linkkLife.addItem("")
        self.comboBox_linkkLife.addItem("")
        self.comboBox_linkkLife.setObjectName(u"comboBox_linkkLife")
        sizePolicy1.setHeightForWidth(self.comboBox_linkkLife.sizePolicy().hasHeightForWidth())
        self.comboBox_linkkLife.setSizePolicy(sizePolicy1)
        self.splitter_13.addWidget(self.comboBox_linkkLife)
        self.lineEdit_linkkey = QLineEdit(self.splitter_13)
        self.lineEdit_linkkey.setObjectName(u"lineEdit_linkkey")
        sizePolicy.setHeightForWidth(self.lineEdit_linkkey.sizePolicy().hasHeightForWidth())
        self.lineEdit_linkkey.setSizePolicy(sizePolicy)
        self.splitter_13.addWidget(self.lineEdit_linkkey)
        self.comboBox_devType = QComboBox(self.splitter_13)
        self.comboBox_devType.addItem("")
        self.comboBox_devType.addItem("")
        self.comboBox_devType.addItem("")
        self.comboBox_devType.setObjectName(u"comboBox_devType")
        sizePolicy1.setHeightForWidth(self.comboBox_devType.sizePolicy().hasHeightForWidth())
        self.comboBox_devType.setSizePolicy(sizePolicy1)
        self.splitter_13.addWidget(self.comboBox_devType)
        self.lineEdit_nwkAddr = QLineEdit(self.splitter_13)
        self.lineEdit_nwkAddr.setObjectName(u"lineEdit_nwkAddr")
        sizePolicy.setHeightForWidth(self.lineEdit_nwkAddr.sizePolicy().hasHeightForWidth())
        self.lineEdit_nwkAddr.setSizePolicy(sizePolicy)
        self.splitter_13.addWidget(self.lineEdit_nwkAddr)
        self.comboBox_joinMethod = QComboBox(self.splitter_13)
        self.comboBox_joinMethod.addItem("")
        self.comboBox_joinMethod.addItem("")
        self.comboBox_joinMethod.addItem("")
        self.comboBox_joinMethod.setObjectName(u"comboBox_joinMethod")
        self.comboBox_joinMethod.setEnabled(True)
        sizePolicy1.setHeightForWidth(self.comboBox_joinMethod.sizePolicy().hasHeightForWidth())
        self.comboBox_joinMethod.setSizePolicy(sizePolicy1)
        self.splitter_13.addWidget(self.comboBox_joinMethod)
        self.lineEdit_ieeeAddr = QLineEdit(self.splitter_13)
        self.lineEdit_ieeeAddr.setObjectName(u"lineEdit_ieeeAddr")
        sizePolicy.setHeightForWidth(self.lineEdit_ieeeAddr.sizePolicy().hasHeightForWidth())
        self.lineEdit_ieeeAddr.setSizePolicy(sizePolicy)
        self.splitter_13.addWidget(self.lineEdit_ieeeAddr)
        self.lineEdit_tcAddr = QLineEdit(self.splitter_13)
        self.lineEdit_tcAddr.setObjectName(u"lineEdit_tcAddr")
        sizePolicy.setHeightForWidth(self.lineEdit_tcAddr.sizePolicy().hasHeightForWidth())
        self.lineEdit_tcAddr.setSizePolicy(sizePolicy)
        self.splitter_13.addWidget(self.lineEdit_tcAddr)
        self.lineEdit_updateId = QLineEdit(self.splitter_13)
        self.lineEdit_updateId.setObjectName(u"lineEdit_updateId")
        sizePolicy.setHeightForWidth(self.lineEdit_updateId.sizePolicy().hasHeightForWidth())
        self.lineEdit_updateId.setSizePolicy(sizePolicy)
        self.splitter_13.addWidget(self.lineEdit_updateId)
        self.lineEdit_keySeqNum_2 = QLineEdit(self.splitter_13)
        self.lineEdit_keySeqNum_2.setObjectName(u"lineEdit_keySeqNum_2")
        sizePolicy.setHeightForWidth(self.lineEdit_keySeqNum_2.sizePolicy().hasHeightForWidth())
        self.lineEdit_keySeqNum_2.setSizePolicy(sizePolicy)
        self.splitter_13.addWidget(self.lineEdit_keySeqNum_2)
        self.lineEdit_adminKey = QLineEdit(self.splitter_13)
        self.lineEdit_adminKey.setObjectName(u"lineEdit_adminKey")
        sizePolicy.setHeightForWidth(self.lineEdit_adminKey.sizePolicy().hasHeightForWidth())
        self.lineEdit_adminKey.setSizePolicy(sizePolicy)
        self.splitter_13.addWidget(self.lineEdit_adminKey)

        self.horizontalLayout_22.addWidget(self.splitter_13)


        self.gridLayout_2.addLayout(self.horizontalLayout_22, 1, 0, 1, 1)

        self.verticalLayout_2 = QVBoxLayout()
        self.verticalLayout_2.setObjectName(u"verticalLayout_2")
        self.horizontalLayout_8 = QHBoxLayout()
        self.horizontalLayout_8.setObjectName(u"horizontalLayout_8")
        self.label_permitJoin = QLabel(self.scrollAreaWidgetContents_3)
        self.label_permitJoin.setObjectName(u"label_permitJoin")
        font2 = QFont()
        font2.setFamilies([u"Calibri"])
        font2.setBold(True)
        self.label_permitJoin.setFont(font2)

        self.horizontalLayout_8.addWidget(self.label_permitJoin)

        self.lineEdit_perjoinTime = QLineEdit(self.scrollAreaWidgetContents_3)
        self.lineEdit_perjoinTime.setObjectName(u"lineEdit_perjoinTime")
        self.lineEdit_perjoinTime.setMinimumSize(QSize(0, 30))

        self.horizontalLayout_8.addWidget(self.lineEdit_perjoinTime)

        self.pushButton_setPer = QPushButton(self.scrollAreaWidgetContents_3)
        self.pushButton_setPer.setObjectName(u"pushButton_setPer")

        self.horizontalLayout_8.addWidget(self.pushButton_setPer)

        self.horizontalSpacer_10 = QSpacerItem(158, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_8.addItem(self.horizontalSpacer_10)


        self.verticalLayout_2.addLayout(self.horizontalLayout_8)

        self.horizontalLayout_11 = QHBoxLayout()
        self.horizontalLayout_11.setObjectName(u"horizontalLayout_11")
        self.label_leave = QLabel(self.scrollAreaWidgetContents_3)
        self.label_leave.setObjectName(u"label_leave")
        font3 = QFont()
        font3.setFamilies([u"Cambria"])
        font3.setBold(True)
        self.label_leave.setFont(font3)

        self.horizontalLayout_11.addWidget(self.label_leave)

        self.lineEdit_leaveRemove = QLineEdit(self.scrollAreaWidgetContents_3)
        self.lineEdit_leaveRemove.setObjectName(u"lineEdit_leaveRemove")
        self.lineEdit_leaveRemove.setMinimumSize(QSize(0, 30))
        self.lineEdit_leaveRemove.setCursorMoveStyle(Qt.LogicalMoveStyle)

        self.horizontalLayout_11.addWidget(self.lineEdit_leaveRemove)

        self.lineEdit_leaveRejoin = QLineEdit(self.scrollAreaWidgetContents_3)
        self.lineEdit_leaveRejoin.setObjectName(u"lineEdit_leaveRejoin")
        self.lineEdit_leaveRejoin.setMinimumSize(QSize(0, 30))

        self.horizontalLayout_11.addWidget(self.lineEdit_leaveRejoin)

        self.pushButton_leave = QPushButton(self.scrollAreaWidgetContents_3)
        self.pushButton_leave.setObjectName(u"pushButton_leave")

        self.horizontalLayout_11.addWidget(self.pushButton_leave)

        self.horizontalSpacer_11 = QSpacerItem(40, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_11.addItem(self.horizontalSpacer_11)


        self.verticalLayout_2.addLayout(self.horizontalLayout_11)

        self.label_manaJoin = QLabel(self.scrollAreaWidgetContents_3)
        self.label_manaJoin.setObjectName(u"label_manaJoin")
        self.label_manaJoin.setFont(font2)

        self.verticalLayout_2.addWidget(self.label_manaJoin)

        self.horizontalLayout_12 = QHBoxLayout()
        self.horizontalLayout_12.setObjectName(u"horizontalLayout_12")
        self.label_proLinkkey = QLabel(self.scrollAreaWidgetContents_3)
        self.label_proLinkkey.setObjectName(u"label_proLinkkey")

        self.horizontalLayout_12.addWidget(self.label_proLinkkey)

        self.lineEdit_proLinkkey = QLineEdit(self.scrollAreaWidgetContents_3)
        self.lineEdit_proLinkkey.setObjectName(u"lineEdit_proLinkkey")
        self.lineEdit_proLinkkey.setMinimumSize(QSize(0, 30))

        self.horizontalLayout_12.addWidget(self.lineEdit_proLinkkey)

        self.horizontalSpacer_9 = QSpacerItem(168, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_12.addItem(self.horizontalSpacer_9)


        self.verticalLayout_2.addLayout(self.horizontalLayout_12)

        self.horizontalLayout_13 = QHBoxLayout()
        self.horizontalLayout_13.setObjectName(u"horizontalLayout_13")
        self.label_manaIeee = QLabel(self.scrollAreaWidgetContents_3)
        self.label_manaIeee.setObjectName(u"label_manaIeee")

        self.horizontalLayout_13.addWidget(self.label_manaIeee)

        self.lineEdit_manaIeee = QLineEdit(self.scrollAreaWidgetContents_3)
        self.lineEdit_manaIeee.setObjectName(u"lineEdit_manaIeee")
        self.lineEdit_manaIeee.setMinimumSize(QSize(0, 30))

        self.horizontalLayout_13.addWidget(self.lineEdit_manaIeee)

        self.horizontalSpacer_12 = QSpacerItem(198, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_13.addItem(self.horizontalSpacer_12)


        self.verticalLayout_2.addLayout(self.horizontalLayout_13)

        self.horizontalLayout_14 = QHBoxLayout()
        self.horizontalLayout_14.setObjectName(u"horizontalLayout_14")
        self.label_manaCom = QLabel(self.scrollAreaWidgetContents_3)
        self.label_manaCom.setObjectName(u"label_manaCom")

        self.horizontalLayout_14.addWidget(self.label_manaCom)

        self.comboBox_manaType = QComboBox(self.scrollAreaWidgetContents_3)
        self.comboBox_manaType.addItem("")
        self.comboBox_manaType.addItem("")
        self.comboBox_manaType.addItem("")
        self.comboBox_manaType.setObjectName(u"comboBox_manaType")

        self.horizontalLayout_14.addWidget(self.comboBox_manaType)

        self.pushButton_manaCom = QPushButton(self.scrollAreaWidgetContents_3)
        self.pushButton_manaCom.setObjectName(u"pushButton_manaCom")

        self.horizontalLayout_14.addWidget(self.pushButton_manaCom)

        self.horizontalSpacer_16 = QSpacerItem(40, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_14.addItem(self.horizontalSpacer_16)


        self.verticalLayout_2.addLayout(self.horizontalLayout_14)

        self.horizontalLayout_15 = QHBoxLayout()
        self.horizontalLayout_15.setObjectName(u"horizontalLayout_15")
        self.label_identify = QLabel(self.scrollAreaWidgetContents_3)
        self.label_identify.setObjectName(u"label_identify")
        self.label_identify.setFont(font2)

        self.horizontalLayout_15.addWidget(self.label_identify)

        self.lineEdit_identifyTime = QLineEdit(self.scrollAreaWidgetContents_3)
        self.lineEdit_identifyTime.setObjectName(u"lineEdit_identifyTime")
        self.lineEdit_identifyTime.setMinimumSize(QSize(0, 30))

        self.horizontalLayout_15.addWidget(self.lineEdit_identifyTime)

        self.pushButton_identify = QPushButton(self.scrollAreaWidgetContents_3)
        self.pushButton_identify.setObjectName(u"pushButton_identify")

        self.horizontalLayout_15.addWidget(self.pushButton_identify)

        self.horizontalSpacer_13 = QSpacerItem(188, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_15.addItem(self.horizontalSpacer_13)


        self.verticalLayout_2.addLayout(self.horizontalLayout_15)

        self.label_findBind = QLabel(self.scrollAreaWidgetContents_3)
        self.label_findBind.setObjectName(u"label_findBind")
        self.label_findBind.setFont(font2)

        self.verticalLayout_2.addWidget(self.label_findBind)

        self.horizontalLayout_9 = QHBoxLayout()
        self.horizontalLayout_9.setObjectName(u"horizontalLayout_9")
        self.label_fbEndpoint = QLabel(self.scrollAreaWidgetContents_3)
        self.label_fbEndpoint.setObjectName(u"label_fbEndpoint")

        self.horizontalLayout_9.addWidget(self.label_fbEndpoint)

        self.lineEdit_fbEndpoint = QLineEdit(self.scrollAreaWidgetContents_3)
        self.lineEdit_fbEndpoint.setObjectName(u"lineEdit_fbEndpoint")
        self.lineEdit_fbEndpoint.setMinimumSize(QSize(0, 30))

        self.horizontalLayout_9.addWidget(self.lineEdit_fbEndpoint)

        self.horizontalSpacer_14 = QSpacerItem(268, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_9.addItem(self.horizontalSpacer_14)


        self.verticalLayout_2.addLayout(self.horizontalLayout_9)

        self.horizontalLayout_10 = QHBoxLayout()
        self.horizontalLayout_10.setObjectName(u"horizontalLayout_10")
        self.label_fbRole = QLabel(self.scrollAreaWidgetContents_3)
        self.label_fbRole.setObjectName(u"label_fbRole")

        self.horizontalLayout_10.addWidget(self.label_fbRole)

        self.comboBox_fbRole = QComboBox(self.scrollAreaWidgetContents_3)
        self.comboBox_fbRole.addItem("")
        self.comboBox_fbRole.addItem("")
        self.comboBox_fbRole.setObjectName(u"comboBox_fbRole")

        self.horizontalLayout_10.addWidget(self.comboBox_fbRole)

        self.pushButton_fbStart = QPushButton(self.scrollAreaWidgetContents_3)
        self.pushButton_fbStart.setObjectName(u"pushButton_fbStart")

        self.horizontalLayout_10.addWidget(self.pushButton_fbStart)

        self.horizontalSpacer_15 = QSpacerItem(228, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_10.addItem(self.horizontalSpacer_15)


        self.verticalLayout_2.addLayout(self.horizontalLayout_10)

        self.horizontalLayout_17 = QHBoxLayout()
        self.horizontalLayout_17.setObjectName(u"horizontalLayout_17")
        self.label_dataTran = QLabel(self.scrollAreaWidgetContents_3)
        self.label_dataTran.setObjectName(u"label_dataTran")

        self.horizontalLayout_17.addWidget(self.label_dataTran)

        self.horizontalSpacer_18 = QSpacerItem(433, 25, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_17.addItem(self.horizontalSpacer_18)

        self.pushButton_commClear = QPushButton(self.scrollAreaWidgetContents_3)
        self.pushButton_commClear.setObjectName(u"pushButton_commClear")

        self.horizontalLayout_17.addWidget(self.pushButton_commClear)


        self.verticalLayout_2.addLayout(self.horizontalLayout_17)

        self.textEdit_dataComm = QTextEdit(self.scrollAreaWidgetContents_3)
        self.textEdit_dataComm.setObjectName(u"textEdit_dataComm")

        self.verticalLayout_2.addWidget(self.textEdit_dataComm)


        self.gridLayout_2.addLayout(self.verticalLayout_2, 1, 1, 2, 1)

        self.horizontalLayout_16 = QHBoxLayout()
        self.horizontalLayout_16.setObjectName(u"horizontalLayout_16")
        self.pushButton_form = QPushButton(self.scrollAreaWidgetContents_3)
        self.pushButton_form.setObjectName(u"pushButton_form")
        sizePolicy2 = QSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)
        sizePolicy2.setHorizontalStretch(38)
        sizePolicy2.setVerticalStretch(0)
        sizePolicy2.setHeightForWidth(self.pushButton_form.sizePolicy().hasHeightForWidth())
        self.pushButton_form.setSizePolicy(sizePolicy2)
        self.pushButton_form.setMinimumSize(QSize(0, 38))
        self.pushButton_form.setFont(font2)
        self.pushButton_form.setIconSize(QSize(20, 30))

        self.horizontalLayout_16.addWidget(self.pushButton_form)

        self.pushButton_join = QPushButton(self.scrollAreaWidgetContents_3)
        self.pushButton_join.setObjectName(u"pushButton_join")
        sizePolicy3 = QSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)
        sizePolicy3.setHorizontalStretch(0)
        sizePolicy3.setVerticalStretch(0)
        sizePolicy3.setHeightForWidth(self.pushButton_join.sizePolicy().hasHeightForWidth())
        self.pushButton_join.setSizePolicy(sizePolicy3)
        self.pushButton_join.setMinimumSize(QSize(0, 38))
        self.pushButton_join.setFont(font2)

        self.horizontalLayout_16.addWidget(self.pushButton_join)

        self.horizontalSpacer_17 = QSpacerItem(108, 38, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_16.addItem(self.horizontalSpacer_17)

        self.pushButton_statusRead = QPushButton(self.scrollAreaWidgetContents_3)
        self.pushButton_statusRead.setObjectName(u"pushButton_statusRead")
        sizePolicy3.setHeightForWidth(self.pushButton_statusRead.sizePolicy().hasHeightForWidth())
        self.pushButton_statusRead.setSizePolicy(sizePolicy3)
        self.pushButton_statusRead.setMinimumSize(QSize(0, 38))
        self.pushButton_statusRead.setFont(font2)

        self.horizontalLayout_16.addWidget(self.pushButton_statusRead)


        self.gridLayout_2.addLayout(self.horizontalLayout_16, 2, 0, 1, 1)

        self.scrollArea_2.setWidget(self.scrollAreaWidgetContents_3)

        self.gridLayout_4.addWidget(self.scrollArea_2, 0, 0, 1, 1)

        self.tabWidget.addTab(self.Commission, "")
        self.Tunnel = QWidget()
        self.Tunnel.setObjectName(u"Tunnel")
        self.gridLayout_7 = QGridLayout(self.Tunnel)
        self.gridLayout_7.setObjectName(u"gridLayout_7")
        self.label_tunnelServiceuuid = QLabel(self.Tunnel)
        self.label_tunnelServiceuuid.setObjectName(u"label_tunnelServiceuuid")
        self.label_tunnelServiceuuid.setFont(font)
        self.label_tunnelServiceuuid.setLineWidth(1)

        self.gridLayout_7.addWidget(self.label_tunnelServiceuuid, 0, 0, 1, 1)

        self.label_tunnelCharaUuid = QLabel(self.Tunnel)
        self.label_tunnelCharaUuid.setObjectName(u"label_tunnelCharaUuid")
        self.label_tunnelCharaUuid.setFont(font1)

        self.gridLayout_7.addWidget(self.label_tunnelCharaUuid, 1, 0, 1, 1)

        self.horizontalLayout_18 = QHBoxLayout()
        self.horizontalLayout_18.setObjectName(u"horizontalLayout_18")
        self.label_zvdjoin = QLabel(self.Tunnel)
        self.label_zvdjoin.setObjectName(u"label_zvdjoin")

        self.horizontalLayout_18.addWidget(self.label_zvdjoin)

        self.lineEdit_zvdIeee = QLineEdit(self.Tunnel)
        self.lineEdit_zvdIeee.setObjectName(u"lineEdit_zvdIeee")
        self.lineEdit_zvdIeee.setMinimumSize(QSize(0, 30))

        self.horizontalLayout_18.addWidget(self.lineEdit_zvdIeee)

        self.lineEdit_zvdNwkAddr = QLineEdit(self.Tunnel)
        self.lineEdit_zvdNwkAddr.setObjectName(u"lineEdit_zvdNwkAddr")
        self.lineEdit_zvdNwkAddr.setMinimumSize(QSize(0, 30))

        self.horizontalLayout_18.addWidget(self.lineEdit_zvdNwkAddr)

        self.lineEdit_parentIeee = QLineEdit(self.Tunnel)
        self.lineEdit_parentIeee.setObjectName(u"lineEdit_parentIeee")
        self.lineEdit_parentIeee.setMinimumSize(QSize(0, 30))

        self.horizontalLayout_18.addWidget(self.lineEdit_parentIeee)

        self.lineEdit_parentshort = QLineEdit(self.Tunnel)
        self.lineEdit_parentshort.setObjectName(u"lineEdit_parentshort")
        self.lineEdit_parentshort.setMinimumSize(QSize(0, 30))

        self.horizontalLayout_18.addWidget(self.lineEdit_parentshort)

        self.pushButton_startJoin = QPushButton(self.Tunnel)
        self.pushButton_startJoin.setObjectName(u"pushButton_startJoin")

        self.horizontalLayout_18.addWidget(self.pushButton_startJoin)

        self.horizontalSpacer_19 = QSpacerItem(88, 25, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_18.addItem(self.horizontalSpacer_19)


        self.gridLayout_7.addLayout(self.horizontalLayout_18, 2, 0, 1, 1)

        self.horizontalLayout_21 = QHBoxLayout()
        self.horizontalLayout_21.setObjectName(u"horizontalLayout_21")
        self.pushButton_zvdLeave = QPushButton(self.Tunnel)
        self.pushButton_zvdLeave.setObjectName(u"pushButton_zvdLeave")

        self.horizontalLayout_21.addWidget(self.pushButton_zvdLeave)

        self.horizontalSpacer_22 = QSpacerItem(708, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_21.addItem(self.horizontalSpacer_22)


        self.gridLayout_7.addLayout(self.horizontalLayout_21, 3, 0, 1, 1)

        self.horizontalLayout_19 = QHBoxLayout()
        self.horizontalLayout_19.setObjectName(u"horizontalLayout_19")
        self.label_zvdOnoff = QLabel(self.Tunnel)
        self.label_zvdOnoff.setObjectName(u"label_zvdOnoff")

        self.horizontalLayout_19.addWidget(self.label_zvdOnoff)

        self.lineEdit_onoffAddr = QLineEdit(self.Tunnel)
        self.lineEdit_onoffAddr.setObjectName(u"lineEdit_onoffAddr")
        self.lineEdit_onoffAddr.setMinimumSize(QSize(0, 30))

        self.horizontalLayout_19.addWidget(self.lineEdit_onoffAddr)

        self.comboBox_commId = QComboBox(self.Tunnel)
        self.comboBox_commId.addItem("")
        self.comboBox_commId.addItem("")
        self.comboBox_commId.addItem("")
        self.comboBox_commId.setObjectName(u"comboBox_commId")

        self.horizontalLayout_19.addWidget(self.comboBox_commId)

        self.pushButton_onoffSend = QPushButton(self.Tunnel)
        self.pushButton_onoffSend.setObjectName(u"pushButton_onoffSend")

        self.horizontalLayout_19.addWidget(self.pushButton_onoffSend)

        self.horizontalSpacer_20 = QSpacerItem(428, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_19.addItem(self.horizontalSpacer_20)


        self.gridLayout_7.addLayout(self.horizontalLayout_19, 4, 0, 1, 1)

        self.horizontalLayout_20 = QHBoxLayout()
        self.horizontalLayout_20.setObjectName(u"horizontalLayout_20")
        self.label_tunnelData = QLabel(self.Tunnel)
        self.label_tunnelData.setObjectName(u"label_tunnelData")

        self.horizontalLayout_20.addWidget(self.label_tunnelData)

        self.horizontalSpacer_21 = QSpacerItem(688, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_20.addItem(self.horizontalSpacer_21)

        self.pushButton_tunnalClear = QPushButton(self.Tunnel)
        self.pushButton_tunnalClear.setObjectName(u"pushButton_tunnalClear")

        self.horizontalLayout_20.addWidget(self.pushButton_tunnalClear)


        self.gridLayout_7.addLayout(self.horizontalLayout_20, 5, 0, 1, 1)

        self.textEdit_tunnelData = QTextEdit(self.Tunnel)
        self.textEdit_tunnelData.setObjectName(u"textEdit_tunnelData")

        self.gridLayout_7.addWidget(self.textEdit_tunnelData, 6, 0, 1, 1)

        self.tabWidget.addTab(self.Tunnel, "")

        self.gridLayout_6.addWidget(self.tabWidget, 3, 0, 1, 1)

        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QMenuBar(MainWindow)
        self.menubar.setObjectName(u"menubar")
        self.menubar.setGeometry(QRect(0, 0, 1277, 26))
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QStatusBar(MainWindow)
        self.statusbar.setObjectName(u"statusbar")
        MainWindow.setStatusBar(self.statusbar)

        self.retranslateUi(MainWindow)

        self.tabWidget.setCurrentIndex(0)
        self.comboBox_keyEnum.setCurrentIndex(5)


        QMetaObject.connectSlotsByName(MainWindow)
    # setupUi

    def retranslateUi(self, MainWindow):
        MainWindow.setWindowTitle(QCoreApplication.translate("MainWindow", u"MainWindow", None))
        self.label_connState.setText(QCoreApplication.translate("MainWindow", u"Connect State: FALSE", None))
        self.pushButton_startScan.setText(QCoreApplication.translate("MainWindow", u"Start Scan", None))
        self.pushButton_startConnect.setText(QCoreApplication.translate("MainWindow", u"Start Connect", None))
        self.pushButton_disconnect.setText(QCoreApplication.translate("MainWindow", u"Disconnect", None))
        self.pushButton_enNotify.setText(QCoreApplication.translate("MainWindow", u"Enable Notify", None))
        self.pushButton_disNotify.setText(QCoreApplication.translate("MainWindow", u"Disable Notify", None))
        self.pushButton_attrRead.setText(QCoreApplication.translate("MainWindow", u"Read", None))
        self.lineEdit_attrWrite.setPlaceholderText(QCoreApplication.translate("MainWindow", u"hex", None))
        self.pushButton_attrWrite.setText(QCoreApplication.translate("MainWindow", u"Write", None))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.GATT), QCoreApplication.translate("MainWindow", u"GATT", None))
        self.label_seServiceuuid.setText(QCoreApplication.translate("MainWindow", u"Security Service UUID: 29144AF4-0000-4481-BFE9-6D0299B429E3", None))
        self.label_seCharaUuid.setText(QCoreApplication.translate("MainWindow", u"    Characteristic UUID: 29144AF4-000X-4481-BFE9-6D0299B429E3", None))
        self.label_sessionState.setText(QCoreApplication.translate("MainWindow", u"Session Build State: FALSE", None))
        self.label_keyMethod.setText(QCoreApplication.translate("MainWindow", u"Key Nogotiation method:", None))
        self.comboBox_keyMethod.setItemText(0, QCoreApplication.translate("MainWindow", u"1--ECDHE using Curve25519 with Hash AES-MMO-128", None))
        self.comboBox_keyMethod.setItemText(1, QCoreApplication.translate("MainWindow", u"2--ECDHE using Curve25519 with Hash SHA-256", None))
        self.comboBox_keyMethod.setItemText(2, QCoreApplication.translate("MainWindow", u"3--ECDHE using Curve P-256 with Hash SHA-256", None))

        self.label_keyEnum.setText(QCoreApplication.translate("MainWindow", u"Pre-shared Secret Enum:", None))
        self.comboBox_keyEnum.setItemText(0, QCoreApplication.translate("MainWindow", u"0--Symmetric Authentication Token", None))
        self.comboBox_keyEnum.setItemText(1, QCoreApplication.translate("MainWindow", u"1--Pre-configured link-key derived from installation code", None))
        self.comboBox_keyEnum.setItemText(2, QCoreApplication.translate("MainWindow", u"2--Variable-length pass code (for PAKE protocols)", None))
        self.comboBox_keyEnum.setItemText(3, QCoreApplication.translate("MainWindow", u"3--Basic Authorization Key", None))
        self.comboBox_keyEnum.setItemText(4, QCoreApplication.translate("MainWindow", u"4--Administrative Authorization Key", None))
        self.comboBox_keyEnum.setItemText(5, QCoreApplication.translate("MainWindow", u"255--Anonymous Well-Known Secret", None))

        self.label_preKey.setText(QCoreApplication.translate("MainWindow", u"Pre-shared Secret(16Bytes):", None))
        self.comboBox_derived.setItemText(0, QCoreApplication.translate("MainWindow", u"0-derived", None))
        self.comboBox_derived.setItemText(1, QCoreApplication.translate("MainWindow", u"1-unchanged", None))

        self.label_seqNum.setText(QCoreApplication.translate("MainWindow", u"Nwkkey seqNum(1Byte):", None))
        self.pushButton_startSession.setText(QCoreApplication.translate("MainWindow", u"Start Session", None))
        self.label_dataSec.setText(QCoreApplication.translate("MainWindow", u"Data In/Out:", None))
        self.pushButton_secClear.setText(QCoreApplication.translate("MainWindow", u"clear", None))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.Security), QCoreApplication.translate("MainWindow", u"Security", None))
        self.label_coServiceuuid.setText(QCoreApplication.translate("MainWindow", u"Security Service UUID: 0000fff7-0000-1000-8000-00805f9b34fb", None))
        self.label_exPanid.setText(QCoreApplication.translate("MainWindow", u"0x00 Extended PANID(8Bytes):", None))
        self.label_shortPanid.setText(QCoreApplication.translate("MainWindow", u"0x01 Short PANID(2Bytes):", None))
        self.label_nwkChannel.setText(QCoreApplication.translate("MainWindow", u"0x02 NWK Channel(5Bytes):", None))
        self.label_nwkKey.setText(QCoreApplication.translate("MainWindow", u"0x03 NWK Key(16Bytes):", None))
        self.label_linkkey.setText(QCoreApplication.translate("MainWindow", u"0x04 Link Key(17Bytes):", None))
        self.label_27.setText("")
        self.label_28.setText("")
        self.label_devType.setText(QCoreApplication.translate("MainWindow", u"0x05 Device Type(1Byte):", None))
        self.label_nwkAddr.setText(QCoreApplication.translate("MainWindow", u"0x06 NWK Address(2Bytes):", None))
        self.label_joinMethod.setText(QCoreApplication.translate("MainWindow", u"0x07 Joining Method(1Byte):", None))
        self.label_ieeeAddr.setText(QCoreApplication.translate("MainWindow", u"0x08 IEEE Address(8Bytes):", None))
        self.label_txAddr.setText(QCoreApplication.translate("MainWindow", u"0x09 TC Address(8Bytes):", None))
        self.label_updateId.setText(QCoreApplication.translate("MainWindow", u"0x0b NWK UpdateID(1Byte):", None))
        self.label_keySeqNum.setText(QCoreApplication.translate("MainWindow", u"0x0c NWK Active Key Seq(1Byte):", None))
        self.label_adminKey.setText(QCoreApplication.translate("MainWindow", u"0x0d Admin Key(16Bytes):", None))
        self.comboBox_linkkType.setItemText(0, QCoreApplication.translate("MainWindow", u"0 Global Key", None))
        self.comboBox_linkkType.setItemText(1, QCoreApplication.translate("MainWindow", u"1 Unique Key", None))

        self.comboBox_linkkLife.setItemText(0, QCoreApplication.translate("MainWindow", u"0 Permanent Key", None))
        self.comboBox_linkkLife.setItemText(1, QCoreApplication.translate("MainWindow", u"1 Provisional Key", None))

        self.comboBox_devType.setItemText(0, QCoreApplication.translate("MainWindow", u"0x00 Coordinator", None))
        self.comboBox_devType.setItemText(1, QCoreApplication.translate("MainWindow", u"0x01 Router", None))
        self.comboBox_devType.setItemText(2, QCoreApplication.translate("MainWindow", u"0x02 End device", None))

        self.comboBox_joinMethod.setItemText(0, QCoreApplication.translate("MainWindow", u"0x00 MAC association ", None))
        self.comboBox_joinMethod.setItemText(1, QCoreApplication.translate("MainWindow", u"0x01 secure NWK rejoining", None))
        self.comboBox_joinMethod.setItemText(2, QCoreApplication.translate("MainWindow", u"0x02 Out-of-band Commissioning", None))

        self.label_permitJoin.setText(QCoreApplication.translate("MainWindow", u"Permit Joining(1Byte):", None))
        self.pushButton_setPer.setText(QCoreApplication.translate("MainWindow", u"Set", None))
        self.label_leave.setText(QCoreApplication.translate("MainWindow", u"Leave network(2Bytes):", None))
        self.lineEdit_leaveRemove.setPlaceholderText(QCoreApplication.translate("MainWindow", u"Remove children", None))
        self.lineEdit_leaveRejoin.setText("")
        self.lineEdit_leaveRejoin.setPlaceholderText(QCoreApplication.translate("MainWindow", u"rejoin", None))
        self.pushButton_leave.setText(QCoreApplication.translate("MainWindow", u"Leave", None))
        self.label_manaJoin.setText(QCoreApplication.translate("MainWindow", u"Manager Joiners:", None))
        self.label_proLinkkey.setText(QCoreApplication.translate("MainWindow", u"   0x00 Provi Linkkey(16Bytes):", None))
        self.label_manaIeee.setText(QCoreApplication.translate("MainWindow", u"   0x01 IEEE Address(8Bytes):", None))
        self.label_manaCom.setText(QCoreApplication.translate("MainWindow", u"   0x02 Manage Command", None))
        self.comboBox_manaType.setItemText(0, QCoreApplication.translate("MainWindow", u"0x00 Drop all joiners' Provisional Linkkeys", None))
        self.comboBox_manaType.setItemText(1, QCoreApplication.translate("MainWindow", u"0x01 Add a joiner's Provisional Linkkey", None))
        self.comboBox_manaType.setItemText(2, QCoreApplication.translate("MainWindow", u"0x02 Remove a joiner's Provisional Linkkey", None))

        self.pushButton_manaCom.setText(QCoreApplication.translate("MainWindow", u"Manager", None))
        self.label_identify.setText(QCoreApplication.translate("MainWindow", u"Identify time(2Bytes):", None))
        self.pushButton_identify.setText(QCoreApplication.translate("MainWindow", u"Identify", None))
        self.label_findBind.setText(QCoreApplication.translate("MainWindow", u"Finding&Binding:", None))
        self.label_fbEndpoint.setText(QCoreApplication.translate("MainWindow", u"  Endpoint(1Byte):", None))
        self.label_fbRole.setText(QCoreApplication.translate("MainWindow", u"          Role:", None))
        self.comboBox_fbRole.setItemText(0, QCoreApplication.translate("MainWindow", u"0 Target", None))
        self.comboBox_fbRole.setItemText(1, QCoreApplication.translate("MainWindow", u"1 Inatiator", None))

        self.pushButton_fbStart.setText(QCoreApplication.translate("MainWindow", u"start FB", None))
        self.label_dataTran.setText(QCoreApplication.translate("MainWindow", u"Data In/Out:", None))
        self.pushButton_commClear.setText(QCoreApplication.translate("MainWindow", u"clear", None))
        self.pushButton_form.setText(QCoreApplication.translate("MainWindow", u"Form network", None))
        self.pushButton_join.setText(QCoreApplication.translate("MainWindow", u"Join Network", None))
        self.pushButton_statusRead.setText(QCoreApplication.translate("MainWindow", u"commission status read", None))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.Commission), QCoreApplication.translate("MainWindow", u"Commission", None))
        self.label_tunnelServiceuuid.setText(QCoreApplication.translate("MainWindow", u"Security Service UUID: 8BD178FD-0000-45F4-8120-B2378BD5313F", None))
        self.label_tunnelCharaUuid.setText(QCoreApplication.translate("MainWindow", u"    Characteristic UUID: 8BD178FD-0001-45F4-8120-B2378BD5313F", None))
        self.label_zvdjoin.setText(QCoreApplication.translate("MainWindow", u"ZVD Join:", None))
        self.lineEdit_zvdIeee.setPlaceholderText(QCoreApplication.translate("MainWindow", u"zvd ieee(8Bytes)", None))
        self.lineEdit_zvdNwkAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"zvd short(2Bytes)", None))
        self.lineEdit_parentIeee.setPlaceholderText(QCoreApplication.translate("MainWindow", u"parent ieee(8Bytes)", None))
        self.lineEdit_parentshort.setPlaceholderText(QCoreApplication.translate("MainWindow", u"parent short(2Bytes)", None))
        self.pushButton_startJoin.setText(QCoreApplication.translate("MainWindow", u"StartJoin", None))
        self.pushButton_zvdLeave.setText(QCoreApplication.translate("MainWindow", u"ZVD leave", None))
        self.label_zvdOnoff.setText(QCoreApplication.translate("MainWindow", u"On/Off:", None))
        self.lineEdit_onoffAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"nwk address(2Bytes)", None))
        self.comboBox_commId.setItemText(0, QCoreApplication.translate("MainWindow", u"0-off", None))
        self.comboBox_commId.setItemText(1, QCoreApplication.translate("MainWindow", u"1-on", None))
        self.comboBox_commId.setItemText(2, QCoreApplication.translate("MainWindow", u"2-toggle", None))

        self.pushButton_onoffSend.setText(QCoreApplication.translate("MainWindow", u"send", None))
        self.label_tunnelData.setText(QCoreApplication.translate("MainWindow", u"Data In/Out:", None))
        self.pushButton_tunnalClear.setText(QCoreApplication.translate("MainWindow", u"Clear", None))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.Tunnel), QCoreApplication.translate("MainWindow", u"Tunnel", None))
    # retranslateUi

