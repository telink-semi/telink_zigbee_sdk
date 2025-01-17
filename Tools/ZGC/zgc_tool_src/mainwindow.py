# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'mainwindow.ui'
##
## Created by: Qt User Interface Compiler version 6.6.0
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
from PySide6.QtWidgets import (QAbstractItemView, QApplication, QCheckBox, QComboBox,
    QFormLayout, QGridLayout, QHBoxLayout, QLabel,
    QLayout, QLineEdit, QListWidget, QListWidgetItem,
    QMainWindow, QMenuBar, QPlainTextEdit, QProgressBar,
    QPushButton, QScrollArea, QSizePolicy, QSpacerItem,
    QSplitter, QStatusBar, QTabWidget, QTextEdit,
    QToolBar, QVBoxLayout, QWidget)

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        if not MainWindow.objectName():
            MainWindow.setObjectName(u"MainWindow")
        MainWindow.resize(1229, 846)
        MainWindow.setMinimumSize(QSize(0, 0))
        self.centralWidget = QWidget(MainWindow)
        self.centralWidget.setObjectName(u"centralWidget")
        self.gridLayout_67 = QGridLayout(self.centralWidget)
        self.gridLayout_67.setSpacing(6)
        self.gridLayout_67.setContentsMargins(11, 11, 11, 11)
        self.gridLayout_67.setObjectName(u"gridLayout_67")
        self.widget = QWidget(self.centralWidget)
        self.widget.setObjectName(u"widget")
        sizePolicy = QSizePolicy(QSizePolicy.Expanding, QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.widget.sizePolicy().hasHeightForWidth())
        self.widget.setSizePolicy(sizePolicy)
        self.widget.setMinimumSize(QSize(200, 0))
        self.widget.setMaximumSize(QSize(200, 16777215))
        self.verticalLayout_5 = QVBoxLayout(self.widget)
        self.verticalLayout_5.setSpacing(6)
        self.verticalLayout_5.setContentsMargins(11, 11, 11, 11)
        self.verticalLayout_5.setObjectName(u"verticalLayout_5")
        self.horizontalLayout_portName = QHBoxLayout()
        self.horizontalLayout_portName.setSpacing(6)
        self.horizontalLayout_portName.setObjectName(u"horizontalLayout_portName")
        self.label_portName = QLabel(self.widget)
        self.label_portName.setObjectName(u"label_portName")
        self.label_portName.setAlignment(Qt.AlignJustify|Qt.AlignVCenter)

        self.horizontalLayout_portName.addWidget(self.label_portName)

        self.comboBox_portName = QComboBox(self.widget)
        self.comboBox_portName.setObjectName(u"comboBox_portName")
        self.comboBox_portName.setSizeAdjustPolicy(QComboBox.AdjustToContents)
        self.comboBox_portName.setMinimumContentsLength(0)

        self.horizontalLayout_portName.addWidget(self.comboBox_portName)

        self.horizontalLayout_portName.setStretch(0, 1)
        self.horizontalLayout_portName.setStretch(1, 2)

        self.verticalLayout_5.addLayout(self.horizontalLayout_portName)

        self.horizontalLayout_baudrate = QHBoxLayout()
        self.horizontalLayout_baudrate.setSpacing(6)
        self.horizontalLayout_baudrate.setObjectName(u"horizontalLayout_baudrate")
        self.label_baudrate = QLabel(self.widget)
        self.label_baudrate.setObjectName(u"label_baudrate")
        self.label_baudrate.setAlignment(Qt.AlignCenter)

        self.horizontalLayout_baudrate.addWidget(self.label_baudrate)

        self.comboBox_baudrate = QComboBox(self.widget)
        self.comboBox_baudrate.addItem("")
        self.comboBox_baudrate.addItem("")
        self.comboBox_baudrate.addItem("")
        self.comboBox_baudrate.addItem("")
        self.comboBox_baudrate.addItem("")
        self.comboBox_baudrate.setObjectName(u"comboBox_baudrate")

        self.horizontalLayout_baudrate.addWidget(self.comboBox_baudrate)

        self.horizontalLayout_baudrate.setStretch(0, 1)
        self.horizontalLayout_baudrate.setStretch(1, 2)

        self.verticalLayout_5.addLayout(self.horizontalLayout_baudrate)

        self.horizontalLayout_databits = QHBoxLayout()
        self.horizontalLayout_databits.setSpacing(6)
        self.horizontalLayout_databits.setObjectName(u"horizontalLayout_databits")
        self.label_databits = QLabel(self.widget)
        self.label_databits.setObjectName(u"label_databits")
        self.label_databits.setAlignment(Qt.AlignCenter)

        self.horizontalLayout_databits.addWidget(self.label_databits)

        self.comboBox_databits = QComboBox(self.widget)
        self.comboBox_databits.addItem("")
        self.comboBox_databits.addItem("")
        self.comboBox_databits.addItem("")
        self.comboBox_databits.addItem("")
        self.comboBox_databits.setObjectName(u"comboBox_databits")

        self.horizontalLayout_databits.addWidget(self.comboBox_databits)

        self.horizontalLayout_databits.setStretch(0, 1)
        self.horizontalLayout_databits.setStretch(1, 2)

        self.verticalLayout_5.addLayout(self.horizontalLayout_databits)

        self.horizontalLayout_parity = QHBoxLayout()
        self.horizontalLayout_parity.setSpacing(6)
        self.horizontalLayout_parity.setObjectName(u"horizontalLayout_parity")
        self.label_parity = QLabel(self.widget)
        self.label_parity.setObjectName(u"label_parity")
        self.label_parity.setEnabled(True)
        self.label_parity.setAlignment(Qt.AlignCenter)

        self.horizontalLayout_parity.addWidget(self.label_parity)

        self.comboBox_parity = QComboBox(self.widget)
        self.comboBox_parity.addItem("")
        self.comboBox_parity.addItem("")
        self.comboBox_parity.addItem("")
        self.comboBox_parity.addItem("")
        self.comboBox_parity.addItem("")
        self.comboBox_parity.setObjectName(u"comboBox_parity")

        self.horizontalLayout_parity.addWidget(self.comboBox_parity)

        self.horizontalLayout_parity.setStretch(0, 1)
        self.horizontalLayout_parity.setStretch(1, 2)

        self.verticalLayout_5.addLayout(self.horizontalLayout_parity)

        self.horizontalLayout_stopbits = QHBoxLayout()
        self.horizontalLayout_stopbits.setSpacing(6)
        self.horizontalLayout_stopbits.setObjectName(u"horizontalLayout_stopbits")
        self.label_stopbits = QLabel(self.widget)
        self.label_stopbits.setObjectName(u"label_stopbits")
        self.label_stopbits.setAlignment(Qt.AlignCenter)

        self.horizontalLayout_stopbits.addWidget(self.label_stopbits)

        self.comboBox_stopbits = QComboBox(self.widget)
        self.comboBox_stopbits.addItem("")
        self.comboBox_stopbits.addItem("")
        self.comboBox_stopbits.addItem("")
        self.comboBox_stopbits.setObjectName(u"comboBox_stopbits")

        self.horizontalLayout_stopbits.addWidget(self.comboBox_stopbits)

        self.horizontalLayout_stopbits.setStretch(0, 1)
        self.horizontalLayout_stopbits.setStretch(1, 2)

        self.verticalLayout_5.addLayout(self.horizontalLayout_stopbits)

        self.pushButton_openPort = QPushButton(self.widget)
        self.pushButton_openPort.setObjectName(u"pushButton_openPort")

        self.verticalLayout_5.addWidget(self.pushButton_openPort)

        self.pushButton_closePort = QPushButton(self.widget)
        self.pushButton_closePort.setObjectName(u"pushButton_closePort")

        self.verticalLayout_5.addWidget(self.pushButton_closePort)

        self.label_portState = QLabel(self.widget)
        self.label_portState.setObjectName(u"label_portState")
        self.label_portState.setAlignment(Qt.AlignLeading|Qt.AlignLeft|Qt.AlignTop)

        self.verticalLayout_5.addWidget(self.label_portState)

        self.checkBox_scroll = QCheckBox(self.widget)
        self.checkBox_scroll.setObjectName(u"checkBox_scroll")

        self.verticalLayout_5.addWidget(self.checkBox_scroll)

        self.checkBox_autoClear = QCheckBox(self.widget)
        self.checkBox_autoClear.setObjectName(u"checkBox_autoClear")
        self.checkBox_autoClear.setChecked(True)

        self.verticalLayout_5.addWidget(self.checkBox_autoClear)

        self.pushButton_clear = QPushButton(self.widget)
        self.pushButton_clear.setObjectName(u"pushButton_clear")

        self.verticalLayout_5.addWidget(self.pushButton_clear)


        self.gridLayout_67.addWidget(self.widget, 0, 0, 1, 1)

        self.verticalLayout_2 = QVBoxLayout()
        self.verticalLayout_2.setSpacing(6)
        self.verticalLayout_2.setObjectName(u"verticalLayout_2")
        self.splitter_2 = QSplitter(self.centralWidget)
        self.splitter_2.setObjectName(u"splitter_2")
        self.splitter_2.setOrientation(Qt.Horizontal)
        self.layoutWidget = QWidget(self.splitter_2)
        self.layoutWidget.setObjectName(u"layoutWidget")
        self.verticalLayout = QVBoxLayout(self.layoutWidget)
        self.verticalLayout.setSpacing(6)
        self.verticalLayout.setContentsMargins(11, 11, 11, 11)
        self.verticalLayout.setObjectName(u"verticalLayout")
        self.verticalLayout.setContentsMargins(0, 0, 0, 0)
        self.listWidget_commandData = QListWidget(self.layoutWidget)
        self.listWidget_commandData.setObjectName(u"listWidget_commandData")
        self.listWidget_commandData.setEnabled(True)
        self.listWidget_commandData.setLineWidth(4)
        self.listWidget_commandData.setVerticalScrollMode(QAbstractItemView.ScrollPerPixel)
        self.listWidget_commandData.setHorizontalScrollMode(QAbstractItemView.ScrollPerPixel)

        self.verticalLayout.addWidget(self.listWidget_commandData)

        self.horizontalLayout = QHBoxLayout()
        self.horizontalLayout.setSpacing(6)
        self.horizontalLayout.setObjectName(u"horizontalLayout")
        self.plainTextEdit_sendData = QPlainTextEdit(self.layoutWidget)
        self.plainTextEdit_sendData.setObjectName(u"plainTextEdit_sendData")
        self.plainTextEdit_sendData.setEnabled(True)
        self.plainTextEdit_sendData.setLineWidth(4)

        self.horizontalLayout.addWidget(self.plainTextEdit_sendData)

        self.pushButton_send = QPushButton(self.layoutWidget)
        self.pushButton_send.setObjectName(u"pushButton_send")
        sizePolicy1 = QSizePolicy(QSizePolicy.Minimum, QSizePolicy.Minimum)
        sizePolicy1.setHorizontalStretch(0)
        sizePolicy1.setVerticalStretch(0)
        sizePolicy1.setHeightForWidth(self.pushButton_send.sizePolicy().hasHeightForWidth())
        self.pushButton_send.setSizePolicy(sizePolicy1)
        self.pushButton_send.setIconSize(QSize(20, 20))

        self.horizontalLayout.addWidget(self.pushButton_send)


        self.verticalLayout.addLayout(self.horizontalLayout)

        self.splitter_2.addWidget(self.layoutWidget)
        self.listWidget_commandDataParse = QListWidget(self.splitter_2)
        self.listWidget_commandDataParse.setObjectName(u"listWidget_commandDataParse")
        self.listWidget_commandDataParse.setLineWidth(4)
        self.splitter_2.addWidget(self.listWidget_commandDataParse)

        self.verticalLayout_2.addWidget(self.splitter_2)

        self.horizontalLayout_2 = QHBoxLayout()
        self.horizontalLayout_2.setSpacing(6)
        self.horizontalLayout_2.setObjectName(u"horizontalLayout_2")
        self.checkBox_thread = QCheckBox(self.centralWidget)
        self.checkBox_thread.setObjectName(u"checkBox_thread")

        self.horizontalLayout_2.addWidget(self.checkBox_thread)

        self.checkBox_bleHci = QCheckBox(self.centralWidget)
        self.checkBox_bleHci.setObjectName(u"checkBox_bleHci")

        self.horizontalLayout_2.addWidget(self.checkBox_bleHci)

        self.horizontalSpacer_64 = QSpacerItem(38, 18, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_2.addItem(self.horizontalSpacer_64)

        self.checkBox_timesend = QCheckBox(self.centralWidget)
        self.checkBox_timesend.setObjectName(u"checkBox_timesend")

        self.horizontalLayout_2.addWidget(self.checkBox_timesend)

        self.lineEdit_sendInterval = QLineEdit(self.centralWidget)
        self.lineEdit_sendInterval.setObjectName(u"lineEdit_sendInterval")
        self.lineEdit_sendInterval.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_2.addWidget(self.lineEdit_sendInterval)

        self.label_sendInterval = QLabel(self.centralWidget)
        self.label_sendInterval.setObjectName(u"label_sendInterval")

        self.horizontalLayout_2.addWidget(self.label_sendInterval)

        self.horizontalSpacer_67 = QSpacerItem(38, 18, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_2.addItem(self.horizontalSpacer_67)

        self.label_recvLabel = QLabel(self.centralWidget)
        self.label_recvLabel.setObjectName(u"label_recvLabel")
        self.label_recvLabel.setAlignment(Qt.AlignRight|Qt.AlignTrailing|Qt.AlignVCenter)

        self.horizontalLayout_2.addWidget(self.label_recvLabel)

        self.lineEdit_recvInterval = QLineEdit(self.centralWidget)
        self.lineEdit_recvInterval.setObjectName(u"lineEdit_recvInterval")
        self.lineEdit_recvInterval.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_2.addWidget(self.lineEdit_recvInterval)

        self.label_recvInterval = QLabel(self.centralWidget)
        self.label_recvInterval.setObjectName(u"label_recvInterval")

        self.horizontalLayout_2.addWidget(self.label_recvInterval)

        self.horizontalSpacer_56 = QSpacerItem(558, 18, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_2.addItem(self.horizontalSpacer_56)


        self.verticalLayout_2.addLayout(self.horizontalLayout_2)


        self.gridLayout_67.addLayout(self.verticalLayout_2, 1, 0, 1, 2)

        self.tabWidget = QTabWidget(self.centralWidget)
        self.tabWidget.setObjectName(u"tabWidget")
        palette = QPalette()
        brush = QBrush(QColor(85, 85, 255, 255))
        brush.setStyle(Qt.SolidPattern)
        palette.setBrush(QPalette.Active, QPalette.Text, brush)
        palette.setBrush(QPalette.Inactive, QPalette.Text, brush)
        brush1 = QBrush(QColor(120, 120, 120, 255))
        brush1.setStyle(Qt.SolidPattern)
        palette.setBrush(QPalette.Disabled, QPalette.Text, brush1)
        self.tabWidget.setPalette(palette)
        self.tabWidget.setElideMode(Qt.ElideNone)
        self.tabWidget.setDocumentMode(False)
        self.bdb = QWidget()
        self.bdb.setObjectName(u"bdb")
        self.gridLayout_5 = QGridLayout(self.bdb)
        self.gridLayout_5.setSpacing(6)
        self.gridLayout_5.setContentsMargins(11, 11, 11, 11)
        self.gridLayout_5.setObjectName(u"gridLayout_5")
        self.scrollArea_bdb = QScrollArea(self.bdb)
        self.scrollArea_bdb.setObjectName(u"scrollArea_bdb")
        palette1 = QPalette()
        brush2 = QBrush(QColor(255, 255, 255, 255))
        brush2.setStyle(Qt.SolidPattern)
        palette1.setBrush(QPalette.Active, QPalette.Base, brush2)
        palette1.setBrush(QPalette.Inactive, QPalette.Base, brush2)
        brush3 = QBrush(QColor(240, 240, 240, 255))
        brush3.setStyle(Qt.SolidPattern)
        palette1.setBrush(QPalette.Disabled, QPalette.Base, brush3)
        self.scrollArea_bdb.setPalette(palette1)
        self.scrollArea_bdb.setWidgetResizable(True)
        self.scrollAreaWidgetContents_5 = QWidget()
        self.scrollAreaWidgetContents_5.setObjectName(u"scrollAreaWidgetContents_5")
        self.scrollAreaWidgetContents_5.setGeometry(QRect(0, 0, 949, 365))
        self.gridLayout = QGridLayout(self.scrollAreaWidgetContents_5)
        self.gridLayout.setSpacing(6)
        self.gridLayout.setContentsMargins(11, 11, 11, 11)
        self.gridLayout.setObjectName(u"gridLayout")
        self.horizontalLayout_setChannel = QHBoxLayout()
        self.horizontalLayout_setChannel.setSpacing(6)
        self.horizontalLayout_setChannel.setObjectName(u"horizontalLayout_setChannel")
        self.pushButton_BDBsetCh = QPushButton(self.scrollAreaWidgetContents_5)
        self.pushButton_BDBsetCh.setObjectName(u"pushButton_BDBsetCh")

        self.horizontalLayout_setChannel.addWidget(self.pushButton_BDBsetCh)

        self.comboBox_channelList = QComboBox(self.scrollAreaWidgetContents_5)
        self.comboBox_channelList.addItem("")
        self.comboBox_channelList.addItem("")
        self.comboBox_channelList.addItem("")
        self.comboBox_channelList.addItem("")
        self.comboBox_channelList.addItem("")
        self.comboBox_channelList.addItem("")
        self.comboBox_channelList.addItem("")
        self.comboBox_channelList.addItem("")
        self.comboBox_channelList.addItem("")
        self.comboBox_channelList.addItem("")
        self.comboBox_channelList.addItem("")
        self.comboBox_channelList.addItem("")
        self.comboBox_channelList.addItem("")
        self.comboBox_channelList.addItem("")
        self.comboBox_channelList.addItem("")
        self.comboBox_channelList.addItem("")
        self.comboBox_channelList.setObjectName(u"comboBox_channelList")
        sizePolicy2 = QSizePolicy(QSizePolicy.Minimum, QSizePolicy.Fixed)
        sizePolicy2.setHorizontalStretch(0)
        sizePolicy2.setVerticalStretch(0)
        sizePolicy2.setHeightForWidth(self.comboBox_channelList.sizePolicy().hasHeightForWidth())
        self.comboBox_channelList.setSizePolicy(sizePolicy2)
        self.comboBox_channelList.setMinimumSize(QSize(20, 0))
        self.comboBox_channelList.setMaximumSize(QSize(100, 16777215))
        self.comboBox_channelList.setSizeAdjustPolicy(QComboBox.AdjustToContents)
        self.comboBox_channelList.setMinimumContentsLength(0)

        self.horizontalLayout_setChannel.addWidget(self.comboBox_channelList)

        self.horizontalSpacer = QSpacerItem(0, 0, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_setChannel.addItem(self.horizontalSpacer)


        self.gridLayout.addLayout(self.horizontalLayout_setChannel, 0, 0, 1, 1)

        self.horizontalLayout_startNetwork = QHBoxLayout()
        self.horizontalLayout_startNetwork.setSpacing(6)
        self.horizontalLayout_startNetwork.setObjectName(u"horizontalLayout_startNetwork")
        self.pushButton_BDBstartNet = QPushButton(self.scrollAreaWidgetContents_5)
        self.pushButton_BDBstartNet.setObjectName(u"pushButton_BDBstartNet")

        self.horizontalLayout_startNetwork.addWidget(self.pushButton_BDBstartNet)

        self.horizontalSpacer_2 = QSpacerItem(0, 0, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_startNetwork.addItem(self.horizontalSpacer_2)


        self.gridLayout.addLayout(self.horizontalLayout_startNetwork, 1, 0, 1, 1)

        self.horizontalLayout_factoryReset = QHBoxLayout()
        self.horizontalLayout_factoryReset.setSpacing(6)
        self.horizontalLayout_factoryReset.setObjectName(u"horizontalLayout_factoryReset")
        self.pushButton_BDBfactoryRst = QPushButton(self.scrollAreaWidgetContents_5)
        self.pushButton_BDBfactoryRst.setObjectName(u"pushButton_BDBfactoryRst")

        self.horizontalLayout_factoryReset.addWidget(self.pushButton_BDBfactoryRst)

        self.horizontalSpacer_3 = QSpacerItem(40, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_factoryReset.addItem(self.horizontalSpacer_3)


        self.gridLayout.addLayout(self.horizontalLayout_factoryReset, 2, 0, 1, 1)

        self.horizontalLayout_preInstall = QHBoxLayout()
        self.horizontalLayout_preInstall.setSpacing(6)
        self.horizontalLayout_preInstall.setObjectName(u"horizontalLayout_preInstall")
        self.pushButton_BDBpreinstallCode = QPushButton(self.scrollAreaWidgetContents_5)
        self.pushButton_BDBpreinstallCode.setObjectName(u"pushButton_BDBpreinstallCode")

        self.horizontalLayout_preInstall.addWidget(self.pushButton_BDBpreinstallCode)

        self.lineEdit_dstAddr = QLineEdit(self.scrollAreaWidgetContents_5)
        self.lineEdit_dstAddr.setObjectName(u"lineEdit_dstAddr")
        sizePolicy3 = QSizePolicy(QSizePolicy.Preferred, QSizePolicy.Fixed)
        sizePolicy3.setHorizontalStretch(0)
        sizePolicy3.setVerticalStretch(0)
        sizePolicy3.setHeightForWidth(self.lineEdit_dstAddr.sizePolicy().hasHeightForWidth())
        self.lineEdit_dstAddr.setSizePolicy(sizePolicy3)
        self.lineEdit_dstAddr.setMinimumSize(QSize(0, 0))
        self.lineEdit_dstAddr.setMaximumSize(QSize(160, 16777215))

        self.horizontalLayout_preInstall.addWidget(self.lineEdit_dstAddr)

        self.lineEdit_installcode = QLineEdit(self.scrollAreaWidgetContents_5)
        self.lineEdit_installcode.setObjectName(u"lineEdit_installcode")
        self.lineEdit_installcode.setMinimumSize(QSize(360, 0))
        self.lineEdit_installcode.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_preInstall.addWidget(self.lineEdit_installcode)

        self.horizontalSpacer_5 = QSpacerItem(0, 0, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_preInstall.addItem(self.horizontalSpacer_5)


        self.gridLayout.addLayout(self.horizontalLayout_preInstall, 3, 0, 1, 1)

        self.horizontalLayout_setWorkingMode = QHBoxLayout()
        self.horizontalLayout_setWorkingMode.setSpacing(6)
        self.horizontalLayout_setWorkingMode.setObjectName(u"horizontalLayout_setWorkingMode")
        self.pushButton_BDBdongleMode = QPushButton(self.scrollAreaWidgetContents_5)
        self.pushButton_BDBdongleMode.setObjectName(u"pushButton_BDBdongleMode")

        self.horizontalLayout_setWorkingMode.addWidget(self.pushButton_BDBdongleMode)

        self.comboBox_mode = QComboBox(self.scrollAreaWidgetContents_5)
        self.comboBox_mode.addItem("")
        self.comboBox_mode.addItem("")
        self.comboBox_mode.setObjectName(u"comboBox_mode")
        self.comboBox_mode.setMaxVisibleItems(10)
        self.comboBox_mode.setSizeAdjustPolicy(QComboBox.AdjustToContents)
        self.comboBox_mode.setMinimumContentsLength(0)

        self.horizontalLayout_setWorkingMode.addWidget(self.comboBox_mode)

        self.horizontalSpacer_6 = QSpacerItem(0, 0, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_setWorkingMode.addItem(self.horizontalSpacer_6)


        self.gridLayout.addLayout(self.horizontalLayout_setWorkingMode, 4, 0, 1, 1)

        self.horizontalLayout_nodeDelete = QHBoxLayout()
        self.horizontalLayout_nodeDelete.setSpacing(6)
        self.horizontalLayout_nodeDelete.setObjectName(u"horizontalLayout_nodeDelete")
        self.pushButton_BDBnodeDelete = QPushButton(self.scrollAreaWidgetContents_5)
        self.pushButton_BDBnodeDelete.setObjectName(u"pushButton_BDBnodeDelete")

        self.horizontalLayout_nodeDelete.addWidget(self.pushButton_BDBnodeDelete)

        self.lineEdit_macAddr = QLineEdit(self.scrollAreaWidgetContents_5)
        self.lineEdit_macAddr.setObjectName(u"lineEdit_macAddr")
        sizePolicy3.setHeightForWidth(self.lineEdit_macAddr.sizePolicy().hasHeightForWidth())
        self.lineEdit_macAddr.setSizePolicy(sizePolicy3)
        self.lineEdit_macAddr.setMinimumSize(QSize(160, 0))
        self.lineEdit_macAddr.setMaximumSize(QSize(160, 16777215))

        self.horizontalLayout_nodeDelete.addWidget(self.lineEdit_macAddr)

        self.horizontalSpacer_7 = QSpacerItem(0, 0, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_nodeDelete.addItem(self.horizontalSpacer_7)


        self.gridLayout.addLayout(self.horizontalLayout_nodeDelete, 5, 0, 1, 1)

        self.horizontalLayout_setTxPower = QHBoxLayout()
        self.horizontalLayout_setTxPower.setSpacing(6)
        self.horizontalLayout_setTxPower.setObjectName(u"horizontalLayout_setTxPower")
        self.pushButton_BDBsetTXPower = QPushButton(self.scrollAreaWidgetContents_5)
        self.pushButton_BDBsetTXPower.setObjectName(u"pushButton_BDBsetTXPower")

        self.horizontalLayout_setTxPower.addWidget(self.pushButton_BDBsetTXPower)

        self.lineEdit_TxIndex = QLineEdit(self.scrollAreaWidgetContents_5)
        self.lineEdit_TxIndex.setObjectName(u"lineEdit_TxIndex")
        self.lineEdit_TxIndex.setMinimumSize(QSize(200, 0))
        self.lineEdit_TxIndex.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_setTxPower.addWidget(self.lineEdit_TxIndex)

        self.horizontalSpacer_25 = QSpacerItem(0, 0, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_setTxPower.addItem(self.horizontalSpacer_25)


        self.gridLayout.addLayout(self.horizontalLayout_setTxPower, 6, 0, 1, 1)

        self.horizontalLayout_9 = QHBoxLayout()
        self.horizontalLayout_9.setSpacing(6)
        self.horizontalLayout_9.setObjectName(u"horizontalLayout_9")
        self.pushButton_getLocalDevInfo = QPushButton(self.scrollAreaWidgetContents_5)
        self.pushButton_getLocalDevInfo.setObjectName(u"pushButton_getLocalDevInfo")

        self.horizontalLayout_9.addWidget(self.pushButton_getLocalDevInfo)

        self.horizontalSpacer_8 = QSpacerItem(40, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_9.addItem(self.horizontalSpacer_8)


        self.gridLayout.addLayout(self.horizontalLayout_9, 7, 0, 1, 1)

        self.horizontalLayout_7 = QHBoxLayout()
        self.horizontalLayout_7.setSpacing(6)
        self.horizontalLayout_7.setObjectName(u"horizontalLayout_7")
        self.lineEdit_srcEp = QLineEdit(self.scrollAreaWidgetContents_5)
        self.lineEdit_srcEp.setObjectName(u"lineEdit_srcEp")
        self.lineEdit_srcEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_7.addWidget(self.lineEdit_srcEp)

        self.lineEdit_clusterId = QLineEdit(self.scrollAreaWidgetContents_5)
        self.lineEdit_clusterId.setObjectName(u"lineEdit_clusterId")
        self.lineEdit_clusterId.setMinimumSize(QSize(60, 0))
        self.lineEdit_clusterId.setMaximumSize(QSize(120, 16777215))

        self.horizontalLayout_7.addWidget(self.lineEdit_clusterId)

        self.lineEdit_dstEp = QLineEdit(self.scrollAreaWidgetContents_5)
        self.lineEdit_dstEp.setObjectName(u"lineEdit_dstEp")
        self.lineEdit_dstEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_7.addWidget(self.lineEdit_dstEp)

        self.checkBox_autobind = QCheckBox(self.scrollAreaWidgetContents_5)
        self.checkBox_autobind.setObjectName(u"checkBox_autobind")

        self.horizontalLayout_7.addWidget(self.checkBox_autobind)

        self.label_bindConfigState = QLabel(self.scrollAreaWidgetContents_5)
        self.label_bindConfigState.setObjectName(u"label_bindConfigState")
        self.label_bindConfigState.setEnabled(True)
        self.label_bindConfigState.setAlignment(Qt.AlignLeading|Qt.AlignLeft|Qt.AlignTop)

        self.horizontalLayout_7.addWidget(self.label_bindConfigState)

        self.horizontalSpacer_4 = QSpacerItem(258, 17, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_7.addItem(self.horizontalSpacer_4)


        self.gridLayout.addLayout(self.horizontalLayout_7, 8, 0, 1, 1)

        self.label_bindFailList = QLabel(self.scrollAreaWidgetContents_5)
        self.label_bindFailList.setObjectName(u"label_bindFailList")
        self.label_bindFailList.setAlignment(Qt.AlignLeading|Qt.AlignLeft|Qt.AlignTop)

        self.gridLayout.addWidget(self.label_bindFailList, 9, 0, 1, 1)

        self.scrollArea_bdb.setWidget(self.scrollAreaWidgetContents_5)

        self.gridLayout_5.addWidget(self.scrollArea_bdb, 0, 0, 1, 1)

        self.tabWidget.addTab(self.bdb, "")
        self.nodesMgmt = QWidget()
        self.nodesMgmt.setObjectName(u"nodesMgmt")
        self.gridLayout_4 = QGridLayout(self.nodesMgmt)
        self.gridLayout_4.setSpacing(6)
        self.gridLayout_4.setContentsMargins(11, 11, 11, 11)
        self.gridLayout_4.setObjectName(u"gridLayout_4")
        self.scrollArea_nodesMgmt = QScrollArea(self.nodesMgmt)
        self.scrollArea_nodesMgmt.setObjectName(u"scrollArea_nodesMgmt")
        self.scrollArea_nodesMgmt.setWidgetResizable(True)
        self.scrollAreaWidgetContents = QWidget()
        self.scrollAreaWidgetContents.setObjectName(u"scrollAreaWidgetContents")
        self.scrollAreaWidgetContents.setGeometry(QRect(0, 0, 970, 316))
        self.gridLayout_11 = QGridLayout(self.scrollAreaWidgetContents)
        self.gridLayout_11.setSpacing(6)
        self.gridLayout_11.setContentsMargins(11, 11, 11, 11)
        self.gridLayout_11.setObjectName(u"gridLayout_11")
        self.horizontalLayout_nodesInfo = QHBoxLayout()
        self.horizontalLayout_nodesInfo.setSpacing(6)
        self.horizontalLayout_nodesInfo.setObjectName(u"horizontalLayout_nodesInfo")
        self.getJoindNodeListWidget = QListWidget(self.scrollAreaWidgetContents)
        self.getJoindNodeListWidget.setObjectName(u"getJoindNodeListWidget")
        self.getJoindNodeListWidget.setSelectionMode(QAbstractItemView.ExtendedSelection)

        self.horizontalLayout_nodesInfo.addWidget(self.getJoindNodeListWidget)

        self.verticalLayout_nodesCommand = QVBoxLayout()
        self.verticalLayout_nodesCommand.setSpacing(6)
        self.verticalLayout_nodesCommand.setObjectName(u"verticalLayout_nodesCommand")
        self.pushButton_getJoinedNodes = QPushButton(self.scrollAreaWidgetContents)
        self.pushButton_getJoinedNodes.setObjectName(u"pushButton_getJoinedNodes")

        self.verticalLayout_nodesCommand.addWidget(self.pushButton_getJoinedNodes)

        self.verticalSpacer_6 = QSpacerItem(0, 0, QSizePolicy.Minimum, QSizePolicy.Expanding)

        self.verticalLayout_nodesCommand.addItem(self.verticalSpacer_6)

        self.pushButton_on = QPushButton(self.scrollAreaWidgetContents)
        self.pushButton_on.setObjectName(u"pushButton_on")

        self.verticalLayout_nodesCommand.addWidget(self.pushButton_on)

        self.pushButton_off = QPushButton(self.scrollAreaWidgetContents)
        self.pushButton_off.setObjectName(u"pushButton_off")

        self.verticalLayout_nodesCommand.addWidget(self.pushButton_off)

        self.pushButton_leave = QPushButton(self.scrollAreaWidgetContents)
        self.pushButton_leave.setObjectName(u"pushButton_leave")

        self.verticalLayout_nodesCommand.addWidget(self.pushButton_leave)

        self.verticalSpacer_13 = QSpacerItem(0, 0, QSizePolicy.Minimum, QSizePolicy.Expanding)

        self.verticalLayout_nodesCommand.addItem(self.verticalSpacer_13)


        self.horizontalLayout_nodesInfo.addLayout(self.verticalLayout_nodesCommand)


        self.gridLayout_11.addLayout(self.horizontalLayout_nodesInfo, 0, 0, 1, 1)

        self.scrollArea_nodesMgmt.setWidget(self.scrollAreaWidgetContents)

        self.gridLayout_4.addWidget(self.scrollArea_nodesMgmt, 0, 0, 1, 1)

        self.tabWidget.addTab(self.nodesMgmt, "")
        self.mgmt = QWidget()
        self.mgmt.setObjectName(u"mgmt")
        self.gridLayout_9 = QGridLayout(self.mgmt)
        self.gridLayout_9.setSpacing(6)
        self.gridLayout_9.setContentsMargins(11, 11, 11, 11)
        self.gridLayout_9.setObjectName(u"gridLayout_9")
        self.scrollArea_mgmt = QScrollArea(self.mgmt)
        self.scrollArea_mgmt.setObjectName(u"scrollArea_mgmt")
        palette2 = QPalette()
        palette2.setBrush(QPalette.Active, QPalette.Base, brush2)
        palette2.setBrush(QPalette.Inactive, QPalette.Base, brush2)
        palette2.setBrush(QPalette.Disabled, QPalette.Base, brush3)
        self.scrollArea_mgmt.setPalette(palette2)
        self.scrollArea_mgmt.setWidgetResizable(True)
        self.scrollAreaWidgetContents_6 = QWidget()
        self.scrollAreaWidgetContents_6.setObjectName(u"scrollAreaWidgetContents_6")
        self.scrollAreaWidgetContents_6.setGeometry(QRect(0, 0, 949, 533))
        self.verticalLayout_3 = QVBoxLayout(self.scrollAreaWidgetContents_6)
        self.verticalLayout_3.setSpacing(6)
        self.verticalLayout_3.setContentsMargins(11, 11, 11, 11)
        self.verticalLayout_3.setObjectName(u"verticalLayout_3")
        self.horizontalLayout_permitJoin = QHBoxLayout()
        self.horizontalLayout_permitJoin.setSpacing(6)
        self.horizontalLayout_permitJoin.setObjectName(u"horizontalLayout_permitJoin")
        self.pushButton_permitJoin = QPushButton(self.scrollAreaWidgetContents_6)
        self.pushButton_permitJoin.setObjectName(u"pushButton_permitJoin")

        self.horizontalLayout_permitJoin.addWidget(self.pushButton_permitJoin)

        self.lineEdit_permit_dstAddr = QLineEdit(self.scrollAreaWidgetContents_6)
        self.lineEdit_permit_dstAddr.setObjectName(u"lineEdit_permit_dstAddr")
        sizePolicy3.setHeightForWidth(self.lineEdit_permit_dstAddr.sizePolicy().hasHeightForWidth())
        self.lineEdit_permit_dstAddr.setSizePolicy(sizePolicy3)
        self.lineEdit_permit_dstAddr.setMinimumSize(QSize(260, 0))
        self.lineEdit_permit_dstAddr.setMaximumSize(QSize(160, 16777215))

        self.horizontalLayout_permitJoin.addWidget(self.lineEdit_permit_dstAddr)

        self.lineEdit_interval = QLineEdit(self.scrollAreaWidgetContents_6)
        self.lineEdit_interval.setObjectName(u"lineEdit_interval")
        self.lineEdit_interval.setMinimumSize(QSize(100, 0))
        self.lineEdit_interval.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_permitJoin.addWidget(self.lineEdit_interval)

        self.horizontalSpacer_10 = QSpacerItem(0, 0, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_permitJoin.addItem(self.horizontalSpacer_10)


        self.verticalLayout_3.addLayout(self.horizontalLayout_permitJoin)

        self.horizontalLayout_ieeeReq = QHBoxLayout()
        self.horizontalLayout_ieeeReq.setSpacing(6)
        self.horizontalLayout_ieeeReq.setObjectName(u"horizontalLayout_ieeeReq")
        self.pushButton_ieeeReq = QPushButton(self.scrollAreaWidgetContents_6)
        self.pushButton_ieeeReq.setObjectName(u"pushButton_ieeeReq")

        self.horizontalLayout_ieeeReq.addWidget(self.pushButton_ieeeReq)

        self.lineEdit_ieeeReqdstAddr = QLineEdit(self.scrollAreaWidgetContents_6)
        self.lineEdit_ieeeReqdstAddr.setObjectName(u"lineEdit_ieeeReqdstAddr")
        sizePolicy3.setHeightForWidth(self.lineEdit_ieeeReqdstAddr.sizePolicy().hasHeightForWidth())
        self.lineEdit_ieeeReqdstAddr.setSizePolicy(sizePolicy3)
        self.lineEdit_ieeeReqdstAddr.setMinimumSize(QSize(260, 0))
        self.lineEdit_ieeeReqdstAddr.setMaximumSize(QSize(160, 16777215))

        self.horizontalLayout_ieeeReq.addWidget(self.lineEdit_ieeeReqdstAddr)

        self.lineEdit_ieeeReqInterestAddr = QLineEdit(self.scrollAreaWidgetContents_6)
        self.lineEdit_ieeeReqInterestAddr.setObjectName(u"lineEdit_ieeeReqInterestAddr")
        self.lineEdit_ieeeReqInterestAddr.setMinimumSize(QSize(160, 0))
        self.lineEdit_ieeeReqInterestAddr.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_ieeeReq.addWidget(self.lineEdit_ieeeReqInterestAddr)

        self.comboBox_ieeeReqType = QComboBox(self.scrollAreaWidgetContents_6)
        self.comboBox_ieeeReqType.addItem("")
        self.comboBox_ieeeReqType.addItem("")
        self.comboBox_ieeeReqType.setObjectName(u"comboBox_ieeeReqType")
        self.comboBox_ieeeReqType.setMinimumContentsLength(0)

        self.horizontalLayout_ieeeReq.addWidget(self.comboBox_ieeeReqType)

        self.lineEdit_ieeeReqStartIdx = QLineEdit(self.scrollAreaWidgetContents_6)
        self.lineEdit_ieeeReqStartIdx.setObjectName(u"lineEdit_ieeeReqStartIdx")
        self.lineEdit_ieeeReqStartIdx.setMinimumSize(QSize(100, 0))
        self.lineEdit_ieeeReqStartIdx.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_ieeeReq.addWidget(self.lineEdit_ieeeReqStartIdx)

        self.horizontalSpacer_28 = QSpacerItem(0, 0, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_ieeeReq.addItem(self.horizontalSpacer_28)


        self.verticalLayout_3.addLayout(self.horizontalLayout_ieeeReq)

        self.horizontalLayout_nwkReq = QHBoxLayout()
        self.horizontalLayout_nwkReq.setSpacing(6)
        self.horizontalLayout_nwkReq.setObjectName(u"horizontalLayout_nwkReq")
        self.pushButton_nwkReq = QPushButton(self.scrollAreaWidgetContents_6)
        self.pushButton_nwkReq.setObjectName(u"pushButton_nwkReq")

        self.horizontalLayout_nwkReq.addWidget(self.pushButton_nwkReq)

        self.lineEdit_nwkReqDstAddr = QLineEdit(self.scrollAreaWidgetContents_6)
        self.lineEdit_nwkReqDstAddr.setObjectName(u"lineEdit_nwkReqDstAddr")
        sizePolicy3.setHeightForWidth(self.lineEdit_nwkReqDstAddr.sizePolicy().hasHeightForWidth())
        self.lineEdit_nwkReqDstAddr.setSizePolicy(sizePolicy3)
        self.lineEdit_nwkReqDstAddr.setMinimumSize(QSize(260, 0))
        self.lineEdit_nwkReqDstAddr.setMaximumSize(QSize(160, 16777215))

        self.horizontalLayout_nwkReq.addWidget(self.lineEdit_nwkReqDstAddr)

        self.lineEdit_nwkReqInterestAddr = QLineEdit(self.scrollAreaWidgetContents_6)
        self.lineEdit_nwkReqInterestAddr.setObjectName(u"lineEdit_nwkReqInterestAddr")
        self.lineEdit_nwkReqInterestAddr.setMinimumSize(QSize(160, 0))
        self.lineEdit_nwkReqInterestAddr.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_nwkReq.addWidget(self.lineEdit_nwkReqInterestAddr)

        self.comboBox_nwkReqReqtype = QComboBox(self.scrollAreaWidgetContents_6)
        self.comboBox_nwkReqReqtype.addItem("")
        self.comboBox_nwkReqReqtype.addItem("")
        self.comboBox_nwkReqReqtype.setObjectName(u"comboBox_nwkReqReqtype")
        self.comboBox_nwkReqReqtype.setMinimumContentsLength(0)

        self.horizontalLayout_nwkReq.addWidget(self.comboBox_nwkReqReqtype)

        self.lineEdit_nwkReqStartIdx = QLineEdit(self.scrollAreaWidgetContents_6)
        self.lineEdit_nwkReqStartIdx.setObjectName(u"lineEdit_nwkReqStartIdx")
        self.lineEdit_nwkReqStartIdx.setMinimumSize(QSize(100, 0))
        self.lineEdit_nwkReqStartIdx.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_nwkReq.addWidget(self.lineEdit_nwkReqStartIdx)

        self.horizontalSpacer_30 = QSpacerItem(0, 0, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_nwkReq.addItem(self.horizontalSpacer_30)


        self.verticalLayout_3.addLayout(self.horizontalLayout_nwkReq)

        self.horizontalLayout_activeReq = QHBoxLayout()
        self.horizontalLayout_activeReq.setSpacing(6)
        self.horizontalLayout_activeReq.setObjectName(u"horizontalLayout_activeReq")
        self.pushButton_activeReq = QPushButton(self.scrollAreaWidgetContents_6)
        self.pushButton_activeReq.setObjectName(u"pushButton_activeReq")

        self.horizontalLayout_activeReq.addWidget(self.pushButton_activeReq)

        self.lineEdit_activeReqDstAddr = QLineEdit(self.scrollAreaWidgetContents_6)
        self.lineEdit_activeReqDstAddr.setObjectName(u"lineEdit_activeReqDstAddr")
        sizePolicy3.setHeightForWidth(self.lineEdit_activeReqDstAddr.sizePolicy().hasHeightForWidth())
        self.lineEdit_activeReqDstAddr.setSizePolicy(sizePolicy3)
        self.lineEdit_activeReqDstAddr.setMinimumSize(QSize(160, 0))
        self.lineEdit_activeReqDstAddr.setMaximumSize(QSize(160, 16777215))

        self.horizontalLayout_activeReq.addWidget(self.lineEdit_activeReqDstAddr)

        self.lineEdit_activeReqInterestAddr = QLineEdit(self.scrollAreaWidgetContents_6)
        self.lineEdit_activeReqInterestAddr.setObjectName(u"lineEdit_activeReqInterestAddr")
        self.lineEdit_activeReqInterestAddr.setMinimumSize(QSize(160, 0))
        self.lineEdit_activeReqInterestAddr.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_activeReq.addWidget(self.lineEdit_activeReqInterestAddr)

        self.horizontalSpacer_36 = QSpacerItem(0, 0, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_activeReq.addItem(self.horizontalSpacer_36)


        self.verticalLayout_3.addLayout(self.horizontalLayout_activeReq)

        self.horizontalLayout_nodeDescReq = QHBoxLayout()
        self.horizontalLayout_nodeDescReq.setSpacing(6)
        self.horizontalLayout_nodeDescReq.setObjectName(u"horizontalLayout_nodeDescReq")
        self.pushButton_nodeDescReq = QPushButton(self.scrollAreaWidgetContents_6)
        self.pushButton_nodeDescReq.setObjectName(u"pushButton_nodeDescReq")

        self.horizontalLayout_nodeDescReq.addWidget(self.pushButton_nodeDescReq)

        self.lineEdit_nodeReqDstAddr = QLineEdit(self.scrollAreaWidgetContents_6)
        self.lineEdit_nodeReqDstAddr.setObjectName(u"lineEdit_nodeReqDstAddr")
        sizePolicy3.setHeightForWidth(self.lineEdit_nodeReqDstAddr.sizePolicy().hasHeightForWidth())
        self.lineEdit_nodeReqDstAddr.setSizePolicy(sizePolicy3)
        self.lineEdit_nodeReqDstAddr.setMinimumSize(QSize(160, 0))
        self.lineEdit_nodeReqDstAddr.setMaximumSize(QSize(160, 16777215))

        self.horizontalLayout_nodeDescReq.addWidget(self.lineEdit_nodeReqDstAddr)

        self.lineEdit_nodeReqInterestAddr = QLineEdit(self.scrollAreaWidgetContents_6)
        self.lineEdit_nodeReqInterestAddr.setObjectName(u"lineEdit_nodeReqInterestAddr")
        self.lineEdit_nodeReqInterestAddr.setMinimumSize(QSize(160, 0))
        self.lineEdit_nodeReqInterestAddr.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_nodeDescReq.addWidget(self.lineEdit_nodeReqInterestAddr)

        self.horizontalSpacer_26 = QSpacerItem(0, 0, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_nodeDescReq.addItem(self.horizontalSpacer_26)


        self.verticalLayout_3.addLayout(self.horizontalLayout_nodeDescReq)

        self.horizontalLayout_simpleDescReq = QHBoxLayout()
        self.horizontalLayout_simpleDescReq.setSpacing(6)
        self.horizontalLayout_simpleDescReq.setObjectName(u"horizontalLayout_simpleDescReq")
        self.pushButton_simpleDescReq = QPushButton(self.scrollAreaWidgetContents_6)
        self.pushButton_simpleDescReq.setObjectName(u"pushButton_simpleDescReq")

        self.horizontalLayout_simpleDescReq.addWidget(self.pushButton_simpleDescReq)

        self.lineEdit_simpleReqDstAddr = QLineEdit(self.scrollAreaWidgetContents_6)
        self.lineEdit_simpleReqDstAddr.setObjectName(u"lineEdit_simpleReqDstAddr")
        sizePolicy3.setHeightForWidth(self.lineEdit_simpleReqDstAddr.sizePolicy().hasHeightForWidth())
        self.lineEdit_simpleReqDstAddr.setSizePolicy(sizePolicy3)
        self.lineEdit_simpleReqDstAddr.setMinimumSize(QSize(160, 0))
        self.lineEdit_simpleReqDstAddr.setMaximumSize(QSize(160, 16777215))

        self.horizontalLayout_simpleDescReq.addWidget(self.lineEdit_simpleReqDstAddr)

        self.lineEdit_simpleReqInterestAddr = QLineEdit(self.scrollAreaWidgetContents_6)
        self.lineEdit_simpleReqInterestAddr.setObjectName(u"lineEdit_simpleReqInterestAddr")
        self.lineEdit_simpleReqInterestAddr.setMinimumSize(QSize(160, 0))
        self.lineEdit_simpleReqInterestAddr.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_simpleDescReq.addWidget(self.lineEdit_simpleReqInterestAddr)

        self.lineEdit_simpleReqEndpoint = QLineEdit(self.scrollAreaWidgetContents_6)
        self.lineEdit_simpleReqEndpoint.setObjectName(u"lineEdit_simpleReqEndpoint")
        self.lineEdit_simpleReqEndpoint.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_simpleDescReq.addWidget(self.lineEdit_simpleReqEndpoint)

        self.horizontalSpacer_37 = QSpacerItem(0, 0, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_simpleDescReq.addItem(self.horizontalSpacer_37)


        self.verticalLayout_3.addLayout(self.horizontalLayout_simpleDescReq)

        self.formLayout_matchReq = QFormLayout()
        self.formLayout_matchReq.setSpacing(6)
        self.formLayout_matchReq.setObjectName(u"formLayout_matchReq")
        self.pushButton_matchDescReq = QPushButton(self.scrollAreaWidgetContents_6)
        self.pushButton_matchDescReq.setObjectName(u"pushButton_matchDescReq")

        self.formLayout_matchReq.setWidget(0, QFormLayout.LabelRole, self.pushButton_matchDescReq)

        self.horizontalLayout_80 = QHBoxLayout()
        self.horizontalLayout_80.setSpacing(6)
        self.horizontalLayout_80.setObjectName(u"horizontalLayout_80")
        self.lineEdit_matchReqDstAddr = QLineEdit(self.scrollAreaWidgetContents_6)
        self.lineEdit_matchReqDstAddr.setObjectName(u"lineEdit_matchReqDstAddr")
        sizePolicy3.setHeightForWidth(self.lineEdit_matchReqDstAddr.sizePolicy().hasHeightForWidth())
        self.lineEdit_matchReqDstAddr.setSizePolicy(sizePolicy3)
        self.lineEdit_matchReqDstAddr.setMinimumSize(QSize(260, 0))
        self.lineEdit_matchReqDstAddr.setMaximumSize(QSize(160, 16777215))

        self.horizontalLayout_80.addWidget(self.lineEdit_matchReqDstAddr)

        self.lineEdit_matchReqInterestAddr = QLineEdit(self.scrollAreaWidgetContents_6)
        self.lineEdit_matchReqInterestAddr.setObjectName(u"lineEdit_matchReqInterestAddr")
        sizePolicy3.setHeightForWidth(self.lineEdit_matchReqInterestAddr.sizePolicy().hasHeightForWidth())
        self.lineEdit_matchReqInterestAddr.setSizePolicy(sizePolicy3)
        self.lineEdit_matchReqInterestAddr.setMinimumSize(QSize(160, 0))
        self.lineEdit_matchReqInterestAddr.setMaximumSize(QSize(160, 16777215))

        self.horizontalLayout_80.addWidget(self.lineEdit_matchReqInterestAddr)

        self.lineEdit_matchReqProfileId = QLineEdit(self.scrollAreaWidgetContents_6)
        self.lineEdit_matchReqProfileId.setObjectName(u"lineEdit_matchReqProfileId")
        self.lineEdit_matchReqProfileId.setMinimumSize(QSize(100, 0))
        self.lineEdit_matchReqProfileId.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_80.addWidget(self.lineEdit_matchReqProfileId)

        self.horizontalSpacer_35 = QSpacerItem(0, 0, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_80.addItem(self.horizontalSpacer_35)


        self.formLayout_matchReq.setLayout(0, QFormLayout.FieldRole, self.horizontalLayout_80)

        self.horizontalLayout_76 = QHBoxLayout()
        self.horizontalLayout_76.setSpacing(6)
        self.horizontalLayout_76.setObjectName(u"horizontalLayout_76")
        self.lineEdit_matchReqInCluster = QLineEdit(self.scrollAreaWidgetContents_6)
        self.lineEdit_matchReqInCluster.setObjectName(u"lineEdit_matchReqInCluster")
        self.lineEdit_matchReqInCluster.setMinimumSize(QSize(260, 0))
        self.lineEdit_matchReqInCluster.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_76.addWidget(self.lineEdit_matchReqInCluster)

        self.lineEdit_matchReqOutCluster = QLineEdit(self.scrollAreaWidgetContents_6)
        self.lineEdit_matchReqOutCluster.setObjectName(u"lineEdit_matchReqOutCluster")
        self.lineEdit_matchReqOutCluster.setMinimumSize(QSize(260, 0))
        self.lineEdit_matchReqOutCluster.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_76.addWidget(self.lineEdit_matchReqOutCluster)

        self.horizontalSpacer_19 = QSpacerItem(0, 0, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_76.addItem(self.horizontalSpacer_19)


        self.formLayout_matchReq.setLayout(1, QFormLayout.FieldRole, self.horizontalLayout_76)


        self.verticalLayout_3.addLayout(self.formLayout_matchReq)

        self.horizontalLayout_bindReq = QHBoxLayout()
        self.horizontalLayout_bindReq.setSpacing(6)
        self.horizontalLayout_bindReq.setObjectName(u"horizontalLayout_bindReq")
        self.pushButton_bindReq = QPushButton(self.scrollAreaWidgetContents_6)
        self.pushButton_bindReq.setObjectName(u"pushButton_bindReq")

        self.horizontalLayout_bindReq.addWidget(self.pushButton_bindReq)

        self.lineEdit_bindReqSrcAddr = QLineEdit(self.scrollAreaWidgetContents_6)
        self.lineEdit_bindReqSrcAddr.setObjectName(u"lineEdit_bindReqSrcAddr")
        sizePolicy3.setHeightForWidth(self.lineEdit_bindReqSrcAddr.sizePolicy().hasHeightForWidth())
        self.lineEdit_bindReqSrcAddr.setSizePolicy(sizePolicy3)
        self.lineEdit_bindReqSrcAddr.setMinimumSize(QSize(160, 0))
        self.lineEdit_bindReqSrcAddr.setMaximumSize(QSize(160, 16777215))

        self.horizontalLayout_bindReq.addWidget(self.lineEdit_bindReqSrcAddr)

        self.lineEdit_bindReqSrcEp = QLineEdit(self.scrollAreaWidgetContents_6)
        self.lineEdit_bindReqSrcEp.setObjectName(u"lineEdit_bindReqSrcEp")
        self.lineEdit_bindReqSrcEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_bindReq.addWidget(self.lineEdit_bindReqSrcEp)

        self.lineEdit_bindReqClusterId = QLineEdit(self.scrollAreaWidgetContents_6)
        self.lineEdit_bindReqClusterId.setObjectName(u"lineEdit_bindReqClusterId")
        self.lineEdit_bindReqClusterId.setMinimumSize(QSize(100, 0))
        self.lineEdit_bindReqClusterId.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_bindReq.addWidget(self.lineEdit_bindReqClusterId)

        self.comboBox_bindReqDstMode = QComboBox(self.scrollAreaWidgetContents_6)
        self.comboBox_bindReqDstMode.addItem("")
        self.comboBox_bindReqDstMode.addItem("")
        self.comboBox_bindReqDstMode.setObjectName(u"comboBox_bindReqDstMode")
        self.comboBox_bindReqDstMode.setMinimumSize(QSize(95, 0))
        self.comboBox_bindReqDstMode.setSizeAdjustPolicy(QComboBox.AdjustToContents)
        self.comboBox_bindReqDstMode.setMinimumContentsLength(0)

        self.horizontalLayout_bindReq.addWidget(self.comboBox_bindReqDstMode)

        self.lineEdit_bindReqDstAddr = QLineEdit(self.scrollAreaWidgetContents_6)
        self.lineEdit_bindReqDstAddr.setObjectName(u"lineEdit_bindReqDstAddr")
        sizePolicy3.setHeightForWidth(self.lineEdit_bindReqDstAddr.sizePolicy().hasHeightForWidth())
        self.lineEdit_bindReqDstAddr.setSizePolicy(sizePolicy3)
        self.lineEdit_bindReqDstAddr.setMinimumSize(QSize(160, 0))
        self.lineEdit_bindReqDstAddr.setMaximumSize(QSize(160, 16777215))

        self.horizontalLayout_bindReq.addWidget(self.lineEdit_bindReqDstAddr)

        self.lineEdit_bindReqDstEp = QLineEdit(self.scrollAreaWidgetContents_6)
        self.lineEdit_bindReqDstEp.setObjectName(u"lineEdit_bindReqDstEp")
        self.lineEdit_bindReqDstEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_bindReq.addWidget(self.lineEdit_bindReqDstEp)

        self.horizontalSpacer_32 = QSpacerItem(80, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_bindReq.addItem(self.horizontalSpacer_32)


        self.verticalLayout_3.addLayout(self.horizontalLayout_bindReq)

        self.horizontalLayout_unbindReq = QHBoxLayout()
        self.horizontalLayout_unbindReq.setSpacing(6)
        self.horizontalLayout_unbindReq.setObjectName(u"horizontalLayout_unbindReq")
        self.pushButton_unbindReq = QPushButton(self.scrollAreaWidgetContents_6)
        self.pushButton_unbindReq.setObjectName(u"pushButton_unbindReq")

        self.horizontalLayout_unbindReq.addWidget(self.pushButton_unbindReq)

        self.lineEdit_unbindReqSrcAddr = QLineEdit(self.scrollAreaWidgetContents_6)
        self.lineEdit_unbindReqSrcAddr.setObjectName(u"lineEdit_unbindReqSrcAddr")
        sizePolicy3.setHeightForWidth(self.lineEdit_unbindReqSrcAddr.sizePolicy().hasHeightForWidth())
        self.lineEdit_unbindReqSrcAddr.setSizePolicy(sizePolicy3)
        self.lineEdit_unbindReqSrcAddr.setMinimumSize(QSize(160, 0))
        self.lineEdit_unbindReqSrcAddr.setMaximumSize(QSize(160, 16777215))

        self.horizontalLayout_unbindReq.addWidget(self.lineEdit_unbindReqSrcAddr)

        self.lineEdit_unbindReqSrcEp = QLineEdit(self.scrollAreaWidgetContents_6)
        self.lineEdit_unbindReqSrcEp.setObjectName(u"lineEdit_unbindReqSrcEp")
        self.lineEdit_unbindReqSrcEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_unbindReq.addWidget(self.lineEdit_unbindReqSrcEp)

        self.lineEdit_unbindReqClusterId = QLineEdit(self.scrollAreaWidgetContents_6)
        self.lineEdit_unbindReqClusterId.setObjectName(u"lineEdit_unbindReqClusterId")
        self.lineEdit_unbindReqClusterId.setMinimumSize(QSize(100, 0))
        self.lineEdit_unbindReqClusterId.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_unbindReq.addWidget(self.lineEdit_unbindReqClusterId)

        self.comboBox_unbindReqDstMode = QComboBox(self.scrollAreaWidgetContents_6)
        self.comboBox_unbindReqDstMode.addItem("")
        self.comboBox_unbindReqDstMode.addItem("")
        self.comboBox_unbindReqDstMode.setObjectName(u"comboBox_unbindReqDstMode")
        self.comboBox_unbindReqDstMode.setMinimumSize(QSize(95, 0))
        self.comboBox_unbindReqDstMode.setSizeAdjustPolicy(QComboBox.AdjustToContents)
        self.comboBox_unbindReqDstMode.setMinimumContentsLength(0)

        self.horizontalLayout_unbindReq.addWidget(self.comboBox_unbindReqDstMode)

        self.lineEdit_unbindReqDstAddr = QLineEdit(self.scrollAreaWidgetContents_6)
        self.lineEdit_unbindReqDstAddr.setObjectName(u"lineEdit_unbindReqDstAddr")
        sizePolicy3.setHeightForWidth(self.lineEdit_unbindReqDstAddr.sizePolicy().hasHeightForWidth())
        self.lineEdit_unbindReqDstAddr.setSizePolicy(sizePolicy3)
        self.lineEdit_unbindReqDstAddr.setMinimumSize(QSize(160, 0))
        self.lineEdit_unbindReqDstAddr.setMaximumSize(QSize(160, 16777215))

        self.horizontalLayout_unbindReq.addWidget(self.lineEdit_unbindReqDstAddr)

        self.lineEdit_unbindReqDstEp = QLineEdit(self.scrollAreaWidgetContents_6)
        self.lineEdit_unbindReqDstEp.setObjectName(u"lineEdit_unbindReqDstEp")
        self.lineEdit_unbindReqDstEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_unbindReq.addWidget(self.lineEdit_unbindReqDstEp)

        self.horizontalSpacer_38 = QSpacerItem(80, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_unbindReq.addItem(self.horizontalSpacer_38)


        self.verticalLayout_3.addLayout(self.horizontalLayout_unbindReq)

        self.horizontalLayout_lqiReq = QHBoxLayout()
        self.horizontalLayout_lqiReq.setSpacing(6)
        self.horizontalLayout_lqiReq.setObjectName(u"horizontalLayout_lqiReq")
        self.pushButton_lqiReq = QPushButton(self.scrollAreaWidgetContents_6)
        self.pushButton_lqiReq.setObjectName(u"pushButton_lqiReq")

        self.horizontalLayout_lqiReq.addWidget(self.pushButton_lqiReq)

        self.lineEdit_lqiReqDstAddr = QLineEdit(self.scrollAreaWidgetContents_6)
        self.lineEdit_lqiReqDstAddr.setObjectName(u"lineEdit_lqiReqDstAddr")
        sizePolicy3.setHeightForWidth(self.lineEdit_lqiReqDstAddr.sizePolicy().hasHeightForWidth())
        self.lineEdit_lqiReqDstAddr.setSizePolicy(sizePolicy3)
        self.lineEdit_lqiReqDstAddr.setMinimumSize(QSize(160, 0))
        self.lineEdit_lqiReqDstAddr.setMaximumSize(QSize(160, 16777215))

        self.horizontalLayout_lqiReq.addWidget(self.lineEdit_lqiReqDstAddr)

        self.lineEdit_lqiReqStartIdx = QLineEdit(self.scrollAreaWidgetContents_6)
        self.lineEdit_lqiReqStartIdx.setObjectName(u"lineEdit_lqiReqStartIdx")
        self.lineEdit_lqiReqStartIdx.setMinimumSize(QSize(100, 0))
        self.lineEdit_lqiReqStartIdx.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_lqiReq.addWidget(self.lineEdit_lqiReqStartIdx)

        self.horizontalSpacer_29 = QSpacerItem(80, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_lqiReq.addItem(self.horizontalSpacer_29)


        self.verticalLayout_3.addLayout(self.horizontalLayout_lqiReq)

        self.horizontalLayout_mgmtBindReq = QHBoxLayout()
        self.horizontalLayout_mgmtBindReq.setSpacing(6)
        self.horizontalLayout_mgmtBindReq.setObjectName(u"horizontalLayout_mgmtBindReq")
        self.pushButton_mgmtBindReq = QPushButton(self.scrollAreaWidgetContents_6)
        self.pushButton_mgmtBindReq.setObjectName(u"pushButton_mgmtBindReq")

        self.horizontalLayout_mgmtBindReq.addWidget(self.pushButton_mgmtBindReq)

        self.lineEdit_mgmtBindReqDstAddr = QLineEdit(self.scrollAreaWidgetContents_6)
        self.lineEdit_mgmtBindReqDstAddr.setObjectName(u"lineEdit_mgmtBindReqDstAddr")
        sizePolicy3.setHeightForWidth(self.lineEdit_mgmtBindReqDstAddr.sizePolicy().hasHeightForWidth())
        self.lineEdit_mgmtBindReqDstAddr.setSizePolicy(sizePolicy3)
        self.lineEdit_mgmtBindReqDstAddr.setMinimumSize(QSize(160, 0))
        self.lineEdit_mgmtBindReqDstAddr.setMaximumSize(QSize(160, 16777215))

        self.horizontalLayout_mgmtBindReq.addWidget(self.lineEdit_mgmtBindReqDstAddr)

        self.lineEdit_mgmtBindReqStartIdx = QLineEdit(self.scrollAreaWidgetContents_6)
        self.lineEdit_mgmtBindReqStartIdx.setObjectName(u"lineEdit_mgmtBindReqStartIdx")
        self.lineEdit_mgmtBindReqStartIdx.setMinimumSize(QSize(100, 0))
        self.lineEdit_mgmtBindReqStartIdx.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_mgmtBindReq.addWidget(self.lineEdit_mgmtBindReqStartIdx)

        self.horizontalSpacer_33 = QSpacerItem(80, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_mgmtBindReq.addItem(self.horizontalSpacer_33)


        self.verticalLayout_3.addLayout(self.horizontalLayout_mgmtBindReq)

        self.horizontalLayout_mgmtLeaveReq = QHBoxLayout()
        self.horizontalLayout_mgmtLeaveReq.setSpacing(6)
        self.horizontalLayout_mgmtLeaveReq.setObjectName(u"horizontalLayout_mgmtLeaveReq")
        self.pushButton_mgmtLeaveReq = QPushButton(self.scrollAreaWidgetContents_6)
        self.pushButton_mgmtLeaveReq.setObjectName(u"pushButton_mgmtLeaveReq")

        self.horizontalLayout_mgmtLeaveReq.addWidget(self.pushButton_mgmtLeaveReq)

        self.lineEdit_mgmtLeaveReqDstAddr = QLineEdit(self.scrollAreaWidgetContents_6)
        self.lineEdit_mgmtLeaveReqDstAddr.setObjectName(u"lineEdit_mgmtLeaveReqDstAddr")
        sizePolicy3.setHeightForWidth(self.lineEdit_mgmtLeaveReqDstAddr.sizePolicy().hasHeightForWidth())
        self.lineEdit_mgmtLeaveReqDstAddr.setSizePolicy(sizePolicy3)
        self.lineEdit_mgmtLeaveReqDstAddr.setMinimumSize(QSize(160, 0))
        self.lineEdit_mgmtLeaveReqDstAddr.setMaximumSize(QSize(160, 16777215))

        self.horizontalLayout_mgmtLeaveReq.addWidget(self.lineEdit_mgmtLeaveReqDstAddr)

        self.lineEdit_leaveIeeeAddr = QLineEdit(self.scrollAreaWidgetContents_6)
        self.lineEdit_leaveIeeeAddr.setObjectName(u"lineEdit_leaveIeeeAddr")
        self.lineEdit_leaveIeeeAddr.setMinimumSize(QSize(160, 0))
        self.lineEdit_leaveIeeeAddr.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_mgmtLeaveReq.addWidget(self.lineEdit_leaveIeeeAddr)

        self.lineEdit_rejoin = QLineEdit(self.scrollAreaWidgetContents_6)
        self.lineEdit_rejoin.setObjectName(u"lineEdit_rejoin")
        self.lineEdit_rejoin.setMinimumSize(QSize(160, 0))
        self.lineEdit_rejoin.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_mgmtLeaveReq.addWidget(self.lineEdit_rejoin)

        self.lineEdit_removeChild = QLineEdit(self.scrollAreaWidgetContents_6)
        self.lineEdit_removeChild.setObjectName(u"lineEdit_removeChild")
        self.lineEdit_removeChild.setMinimumSize(QSize(160, 0))
        self.lineEdit_removeChild.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_mgmtLeaveReq.addWidget(self.lineEdit_removeChild)

        self.horizontalSpacer_34 = QSpacerItem(80, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_mgmtLeaveReq.addItem(self.horizontalSpacer_34)


        self.verticalLayout_3.addLayout(self.horizontalLayout_mgmtLeaveReq)

        self.horizontalLayout_nwkUpdateReq = QHBoxLayout()
        self.horizontalLayout_nwkUpdateReq.setSpacing(6)
        self.horizontalLayout_nwkUpdateReq.setObjectName(u"horizontalLayout_nwkUpdateReq")
        self.pushButton_nwkUpdateReq = QPushButton(self.scrollAreaWidgetContents_6)
        self.pushButton_nwkUpdateReq.setObjectName(u"pushButton_nwkUpdateReq")

        self.horizontalLayout_nwkUpdateReq.addWidget(self.pushButton_nwkUpdateReq)

        self.lineEdit_nwkUpdateReqDstAddr = QLineEdit(self.scrollAreaWidgetContents_6)
        self.lineEdit_nwkUpdateReqDstAddr.setObjectName(u"lineEdit_nwkUpdateReqDstAddr")
        sizePolicy3.setHeightForWidth(self.lineEdit_nwkUpdateReqDstAddr.sizePolicy().hasHeightForWidth())
        self.lineEdit_nwkUpdateReqDstAddr.setSizePolicy(sizePolicy3)
        self.lineEdit_nwkUpdateReqDstAddr.setMinimumSize(QSize(236, 0))
        self.lineEdit_nwkUpdateReqDstAddr.setMaximumSize(QSize(160, 16777215))

        self.horizontalLayout_nwkUpdateReq.addWidget(self.lineEdit_nwkUpdateReqDstAddr)

        self.lineEdit_nwkUpdateReqMgtAddr = QLineEdit(self.scrollAreaWidgetContents_6)
        self.lineEdit_nwkUpdateReqMgtAddr.setObjectName(u"lineEdit_nwkUpdateReqMgtAddr")
        self.lineEdit_nwkUpdateReqMgtAddr.setMinimumSize(QSize(120, 0))
        self.lineEdit_nwkUpdateReqMgtAddr.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_nwkUpdateReq.addWidget(self.lineEdit_nwkUpdateReqMgtAddr)

        self.lineEdit_nwkUpdateReqChannel = QLineEdit(self.scrollAreaWidgetContents_6)
        self.lineEdit_nwkUpdateReqChannel.setObjectName(u"lineEdit_nwkUpdateReqChannel")
        self.lineEdit_nwkUpdateReqChannel.setMinimumSize(QSize(100, 0))
        self.lineEdit_nwkUpdateReqChannel.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_nwkUpdateReq.addWidget(self.lineEdit_nwkUpdateReqChannel)

        self.lineEdit_nwkUpdateReqDuration = QLineEdit(self.scrollAreaWidgetContents_6)
        self.lineEdit_nwkUpdateReqDuration.setObjectName(u"lineEdit_nwkUpdateReqDuration")
        self.lineEdit_nwkUpdateReqDuration.setMinimumSize(QSize(110, 0))
        self.lineEdit_nwkUpdateReqDuration.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_nwkUpdateReq.addWidget(self.lineEdit_nwkUpdateReqDuration)

        self.lineEdit_nwkUpdateReqCount = QLineEdit(self.scrollAreaWidgetContents_6)
        self.lineEdit_nwkUpdateReqCount.setObjectName(u"lineEdit_nwkUpdateReqCount")
        self.lineEdit_nwkUpdateReqCount.setMinimumSize(QSize(80, 0))
        self.lineEdit_nwkUpdateReqCount.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_nwkUpdateReq.addWidget(self.lineEdit_nwkUpdateReqCount)

        self.horizontalSpacer_27 = QSpacerItem(80, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_nwkUpdateReq.addItem(self.horizontalSpacer_27)


        self.verticalLayout_3.addLayout(self.horizontalLayout_nwkUpdateReq)

        self.verticalSpacer_3 = QSpacerItem(20, 40, QSizePolicy.Minimum, QSizePolicy.Expanding)

        self.verticalLayout_3.addItem(self.verticalSpacer_3)

        self.scrollArea_mgmt.setWidget(self.scrollAreaWidgetContents_6)

        self.gridLayout_9.addWidget(self.scrollArea_mgmt, 0, 0, 1, 1)

        self.tabWidget.addTab(self.mgmt, "")
        self.general = QWidget()
        self.general.setObjectName(u"general")
        self.gridLayout_3 = QGridLayout(self.general)
        self.gridLayout_3.setSpacing(6)
        self.gridLayout_3.setContentsMargins(11, 11, 11, 11)
        self.gridLayout_3.setObjectName(u"gridLayout_3")
        self.scrollArea_general = QScrollArea(self.general)
        self.scrollArea_general.setObjectName(u"scrollArea_general")
        palette3 = QPalette()
        palette3.setBrush(QPalette.Active, QPalette.Base, brush2)
        palette3.setBrush(QPalette.Inactive, QPalette.Base, brush2)
        palette3.setBrush(QPalette.Disabled, QPalette.Base, brush3)
        self.scrollArea_general.setPalette(palette3)
        self.scrollArea_general.setWidgetResizable(True)
        self.scrollAreaWidgetContents_4 = QWidget()
        self.scrollAreaWidgetContents_4.setObjectName(u"scrollAreaWidgetContents_4")
        self.scrollAreaWidgetContents_4.setGeometry(QRect(0, 0, 1020, 297))
        self.gridLayout_7 = QGridLayout(self.scrollAreaWidgetContents_4)
        self.gridLayout_7.setSpacing(6)
        self.gridLayout_7.setContentsMargins(11, 11, 11, 11)
        self.gridLayout_7.setObjectName(u"gridLayout_7")
        self.horizontalLayout_readAttr = QHBoxLayout()
        self.horizontalLayout_readAttr.setSpacing(6)
        self.horizontalLayout_readAttr.setObjectName(u"horizontalLayout_readAttr")
        self.pushButton_genRead = QPushButton(self.scrollAreaWidgetContents_4)
        self.pushButton_genRead.setObjectName(u"pushButton_genRead")

        self.horizontalLayout_readAttr.addWidget(self.pushButton_genRead)

        self.comboBox_readAttrDstMode = QComboBox(self.scrollAreaWidgetContents_4)
        self.comboBox_readAttrDstMode.addItem("")
        self.comboBox_readAttrDstMode.addItem("")
        self.comboBox_readAttrDstMode.addItem("")
        self.comboBox_readAttrDstMode.addItem("")
        self.comboBox_readAttrDstMode.addItem("")
        self.comboBox_readAttrDstMode.addItem("")
        self.comboBox_readAttrDstMode.addItem("")
        self.comboBox_readAttrDstMode.addItem("")
        self.comboBox_readAttrDstMode.setObjectName(u"comboBox_readAttrDstMode")
        self.comboBox_readAttrDstMode.setSizeAdjustPolicy(QComboBox.AdjustToContents)
        self.comboBox_readAttrDstMode.setMinimumContentsLength(0)

        self.horizontalLayout_readAttr.addWidget(self.comboBox_readAttrDstMode)

        self.lineEdit_readAttrDstAddr = QLineEdit(self.scrollAreaWidgetContents_4)
        self.lineEdit_readAttrDstAddr.setObjectName(u"lineEdit_readAttrDstAddr")
        sizePolicy3.setHeightForWidth(self.lineEdit_readAttrDstAddr.sizePolicy().hasHeightForWidth())
        self.lineEdit_readAttrDstAddr.setSizePolicy(sizePolicy3)
        self.lineEdit_readAttrDstAddr.setMinimumSize(QSize(160, 0))
        self.lineEdit_readAttrDstAddr.setMaximumSize(QSize(160, 16777215))

        self.horizontalLayout_readAttr.addWidget(self.lineEdit_readAttrDstAddr)

        self.lineEdit_readAttrSrcEp = QLineEdit(self.scrollAreaWidgetContents_4)
        self.lineEdit_readAttrSrcEp.setObjectName(u"lineEdit_readAttrSrcEp")
        self.lineEdit_readAttrSrcEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_readAttr.addWidget(self.lineEdit_readAttrSrcEp)

        self.lineEdit_readAttrDstEp = QLineEdit(self.scrollAreaWidgetContents_4)
        self.lineEdit_readAttrDstEp.setObjectName(u"lineEdit_readAttrDstEp")
        self.lineEdit_readAttrDstEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_readAttr.addWidget(self.lineEdit_readAttrDstEp)

        self.lineEdit_readAttrProfileId = QLineEdit(self.scrollAreaWidgetContents_4)
        self.lineEdit_readAttrProfileId.setObjectName(u"lineEdit_readAttrProfileId")
        self.lineEdit_readAttrProfileId.setMinimumSize(QSize(80, 0))
        self.lineEdit_readAttrProfileId.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_readAttr.addWidget(self.lineEdit_readAttrProfileId)

        self.comboBox_readAttrDirect = QComboBox(self.scrollAreaWidgetContents_4)
        self.comboBox_readAttrDirect.addItem("")
        self.comboBox_readAttrDirect.addItem("")
        self.comboBox_readAttrDirect.setObjectName(u"comboBox_readAttrDirect")
        self.comboBox_readAttrDirect.setSizeAdjustPolicy(QComboBox.AdjustToContents)

        self.horizontalLayout_readAttr.addWidget(self.comboBox_readAttrDirect)

        self.lineEdit_readAttrClusterId = QLineEdit(self.scrollAreaWidgetContents_4)
        self.lineEdit_readAttrClusterId.setObjectName(u"lineEdit_readAttrClusterId")
        self.lineEdit_readAttrClusterId.setMinimumSize(QSize(80, 0))
        self.lineEdit_readAttrClusterId.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_readAttr.addWidget(self.lineEdit_readAttrClusterId)

        self.lineEdit_readAttrAttrId = QLineEdit(self.scrollAreaWidgetContents_4)
        self.lineEdit_readAttrAttrId.setObjectName(u"lineEdit_readAttrAttrId")
        self.lineEdit_readAttrAttrId.setMinimumSize(QSize(0, 0))
        self.lineEdit_readAttrAttrId.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_readAttr.addWidget(self.lineEdit_readAttrAttrId)

        self.horizontalSpacer_50 = QSpacerItem(13, 13, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_readAttr.addItem(self.horizontalSpacer_50)


        self.gridLayout_7.addLayout(self.horizontalLayout_readAttr, 0, 0, 1, 1)

        self.formLayout_writeAttr = QFormLayout()
        self.formLayout_writeAttr.setSpacing(6)
        self.formLayout_writeAttr.setObjectName(u"formLayout_writeAttr")
        self.pushButton_writeAttr = QPushButton(self.scrollAreaWidgetContents_4)
        self.pushButton_writeAttr.setObjectName(u"pushButton_writeAttr")

        self.formLayout_writeAttr.setWidget(0, QFormLayout.LabelRole, self.pushButton_writeAttr)

        self.horizontalLayout_writeAttr = QHBoxLayout()
        self.horizontalLayout_writeAttr.setSpacing(6)
        self.horizontalLayout_writeAttr.setObjectName(u"horizontalLayout_writeAttr")
        self.comboBox_writeAttDstMode = QComboBox(self.scrollAreaWidgetContents_4)
        self.comboBox_writeAttDstMode.addItem("")
        self.comboBox_writeAttDstMode.addItem("")
        self.comboBox_writeAttDstMode.addItem("")
        self.comboBox_writeAttDstMode.addItem("")
        self.comboBox_writeAttDstMode.addItem("")
        self.comboBox_writeAttDstMode.addItem("")
        self.comboBox_writeAttDstMode.addItem("")
        self.comboBox_writeAttDstMode.addItem("")
        self.comboBox_writeAttDstMode.setObjectName(u"comboBox_writeAttDstMode")
        self.comboBox_writeAttDstMode.setMaxVisibleItems(10)
        self.comboBox_writeAttDstMode.setSizeAdjustPolicy(QComboBox.AdjustToContents)
        self.comboBox_writeAttDstMode.setMinimumContentsLength(0)

        self.horizontalLayout_writeAttr.addWidget(self.comboBox_writeAttDstMode)

        self.lineEdit_writeAttrDstAddr = QLineEdit(self.scrollAreaWidgetContents_4)
        self.lineEdit_writeAttrDstAddr.setObjectName(u"lineEdit_writeAttrDstAddr")
        sizePolicy3.setHeightForWidth(self.lineEdit_writeAttrDstAddr.sizePolicy().hasHeightForWidth())
        self.lineEdit_writeAttrDstAddr.setSizePolicy(sizePolicy3)
        self.lineEdit_writeAttrDstAddr.setMinimumSize(QSize(160, 0))
        self.lineEdit_writeAttrDstAddr.setMaximumSize(QSize(160, 16777215))

        self.horizontalLayout_writeAttr.addWidget(self.lineEdit_writeAttrDstAddr)

        self.lineEdit_writeAttrSrcEp = QLineEdit(self.scrollAreaWidgetContents_4)
        self.lineEdit_writeAttrSrcEp.setObjectName(u"lineEdit_writeAttrSrcEp")
        self.lineEdit_writeAttrSrcEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_writeAttr.addWidget(self.lineEdit_writeAttrSrcEp)

        self.lineEdit_writeAttrDstEp = QLineEdit(self.scrollAreaWidgetContents_4)
        self.lineEdit_writeAttrDstEp.setObjectName(u"lineEdit_writeAttrDstEp")
        self.lineEdit_writeAttrDstEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_writeAttr.addWidget(self.lineEdit_writeAttrDstEp)

        self.lineEdit_writeAttrProfileId = QLineEdit(self.scrollAreaWidgetContents_4)
        self.lineEdit_writeAttrProfileId.setObjectName(u"lineEdit_writeAttrProfileId")
        self.lineEdit_writeAttrProfileId.setMinimumSize(QSize(80, 0))
        self.lineEdit_writeAttrProfileId.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_writeAttr.addWidget(self.lineEdit_writeAttrProfileId)

        self.comboBox_writeAttrDirect = QComboBox(self.scrollAreaWidgetContents_4)
        self.comboBox_writeAttrDirect.addItem("")
        self.comboBox_writeAttrDirect.addItem("")
        self.comboBox_writeAttrDirect.setObjectName(u"comboBox_writeAttrDirect")
        self.comboBox_writeAttrDirect.setSizeAdjustPolicy(QComboBox.AdjustToContents)

        self.horizontalLayout_writeAttr.addWidget(self.comboBox_writeAttrDirect)

        self.lineEdit_writeAttrClusterId = QLineEdit(self.scrollAreaWidgetContents_4)
        self.lineEdit_writeAttrClusterId.setObjectName(u"lineEdit_writeAttrClusterId")
        self.lineEdit_writeAttrClusterId.setMinimumSize(QSize(80, 0))
        self.lineEdit_writeAttrClusterId.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_writeAttr.addWidget(self.lineEdit_writeAttrClusterId)

        self.lineEdit_writeAttrAttrId = QLineEdit(self.scrollAreaWidgetContents_4)
        self.lineEdit_writeAttrAttrId.setObjectName(u"lineEdit_writeAttrAttrId")
        self.lineEdit_writeAttrAttrId.setMinimumSize(QSize(80, 0))
        self.lineEdit_writeAttrAttrId.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_writeAttr.addWidget(self.lineEdit_writeAttrAttrId)

        self.horizontalSpacer_52 = QSpacerItem(13, 18, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_writeAttr.addItem(self.horizontalSpacer_52)


        self.formLayout_writeAttr.setLayout(0, QFormLayout.FieldRole, self.horizontalLayout_writeAttr)

        self.horizontalLayout_51 = QHBoxLayout()
        self.horizontalLayout_51.setSpacing(6)
        self.horizontalLayout_51.setObjectName(u"horizontalLayout_51")
        self.horizontalLayout_51.setSizeConstraint(QLayout.SetDefaultConstraint)
        self.comboBox_writeAttrDataType = QComboBox(self.scrollAreaWidgetContents_4)
        self.comboBox_writeAttrDataType.addItem("")
        self.comboBox_writeAttrDataType.addItem("")
        self.comboBox_writeAttrDataType.addItem("")
        self.comboBox_writeAttrDataType.addItem("")
        self.comboBox_writeAttrDataType.addItem("")
        self.comboBox_writeAttrDataType.addItem("")
        self.comboBox_writeAttrDataType.addItem("")
        self.comboBox_writeAttrDataType.addItem("")
        self.comboBox_writeAttrDataType.addItem("")
        self.comboBox_writeAttrDataType.addItem("")
        self.comboBox_writeAttrDataType.addItem("")
        self.comboBox_writeAttrDataType.addItem("")
        self.comboBox_writeAttrDataType.addItem("")
        self.comboBox_writeAttrDataType.addItem("")
        self.comboBox_writeAttrDataType.addItem("")
        self.comboBox_writeAttrDataType.addItem("")
        self.comboBox_writeAttrDataType.addItem("")
        self.comboBox_writeAttrDataType.addItem("")
        self.comboBox_writeAttrDataType.addItem("")
        self.comboBox_writeAttrDataType.addItem("")
        self.comboBox_writeAttrDataType.addItem("")
        self.comboBox_writeAttrDataType.addItem("")
        self.comboBox_writeAttrDataType.addItem("")
        self.comboBox_writeAttrDataType.addItem("")
        self.comboBox_writeAttrDataType.addItem("")
        self.comboBox_writeAttrDataType.addItem("")
        self.comboBox_writeAttrDataType.addItem("")
        self.comboBox_writeAttrDataType.addItem("")
        self.comboBox_writeAttrDataType.addItem("")
        self.comboBox_writeAttrDataType.addItem("")
        self.comboBox_writeAttrDataType.addItem("")
        self.comboBox_writeAttrDataType.addItem("")
        self.comboBox_writeAttrDataType.addItem("")
        self.comboBox_writeAttrDataType.addItem("")
        self.comboBox_writeAttrDataType.addItem("")
        self.comboBox_writeAttrDataType.addItem("")
        self.comboBox_writeAttrDataType.addItem("")
        self.comboBox_writeAttrDataType.addItem("")
        self.comboBox_writeAttrDataType.addItem("")
        self.comboBox_writeAttrDataType.addItem("")
        self.comboBox_writeAttrDataType.addItem("")
        self.comboBox_writeAttrDataType.addItem("")
        self.comboBox_writeAttrDataType.addItem("")
        self.comboBox_writeAttrDataType.addItem("")
        self.comboBox_writeAttrDataType.addItem("")
        self.comboBox_writeAttrDataType.addItem("")
        self.comboBox_writeAttrDataType.addItem("")
        self.comboBox_writeAttrDataType.addItem("")
        self.comboBox_writeAttrDataType.addItem("")
        self.comboBox_writeAttrDataType.addItem("")
        self.comboBox_writeAttrDataType.addItem("")
        self.comboBox_writeAttrDataType.addItem("")
        self.comboBox_writeAttrDataType.addItem("")
        self.comboBox_writeAttrDataType.addItem("")
        self.comboBox_writeAttrDataType.addItem("")
        self.comboBox_writeAttrDataType.addItem("")
        self.comboBox_writeAttrDataType.setObjectName(u"comboBox_writeAttrDataType")
        sizePolicy4 = QSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)
        sizePolicy4.setHorizontalStretch(0)
        sizePolicy4.setVerticalStretch(0)
        sizePolicy4.setHeightForWidth(self.comboBox_writeAttrDataType.sizePolicy().hasHeightForWidth())
        self.comboBox_writeAttrDataType.setSizePolicy(sizePolicy4)
        self.comboBox_writeAttrDataType.setMinimumSize(QSize(99, 0))
        self.comboBox_writeAttrDataType.setMaximumSize(QSize(600, 16777215))
        self.comboBox_writeAttrDataType.setSizeAdjustPolicy(QComboBox.AdjustToContents)

        self.horizontalLayout_51.addWidget(self.comboBox_writeAttrDataType)

        self.lineEdit_writeAttrData = QLineEdit(self.scrollAreaWidgetContents_4)
        self.lineEdit_writeAttrData.setObjectName(u"lineEdit_writeAttrData")
        sizePolicy3.setHeightForWidth(self.lineEdit_writeAttrData.sizePolicy().hasHeightForWidth())
        self.lineEdit_writeAttrData.setSizePolicy(sizePolicy3)
        self.lineEdit_writeAttrData.setMinimumSize(QSize(400, 0))

        self.horizontalLayout_51.addWidget(self.lineEdit_writeAttrData)

        self.horizontalSpacer_18 = QSpacerItem(13, 13, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_51.addItem(self.horizontalSpacer_18)


        self.formLayout_writeAttr.setLayout(1, QFormLayout.FieldRole, self.horizontalLayout_51)


        self.gridLayout_7.addLayout(self.formLayout_writeAttr, 1, 0, 1, 1)

        self.formLayout_readReportCfg = QFormLayout()
        self.formLayout_readReportCfg.setSpacing(6)
        self.formLayout_readReportCfg.setObjectName(u"formLayout_readReportCfg")
        self.pushButton_readReportCfg = QPushButton(self.scrollAreaWidgetContents_4)
        self.pushButton_readReportCfg.setObjectName(u"pushButton_readReportCfg")

        self.formLayout_readReportCfg.setWidget(0, QFormLayout.LabelRole, self.pushButton_readReportCfg)

        self.horizontalLayout_54 = QHBoxLayout()
        self.horizontalLayout_54.setSpacing(6)
        self.horizontalLayout_54.setObjectName(u"horizontalLayout_54")
        self.comboBox_readReportCfgDstMode = QComboBox(self.scrollAreaWidgetContents_4)
        self.comboBox_readReportCfgDstMode.addItem("")
        self.comboBox_readReportCfgDstMode.addItem("")
        self.comboBox_readReportCfgDstMode.addItem("")
        self.comboBox_readReportCfgDstMode.addItem("")
        self.comboBox_readReportCfgDstMode.addItem("")
        self.comboBox_readReportCfgDstMode.addItem("")
        self.comboBox_readReportCfgDstMode.addItem("")
        self.comboBox_readReportCfgDstMode.addItem("")
        self.comboBox_readReportCfgDstMode.setObjectName(u"comboBox_readReportCfgDstMode")
        self.comboBox_readReportCfgDstMode.setMaxVisibleItems(10)
        self.comboBox_readReportCfgDstMode.setSizeAdjustPolicy(QComboBox.AdjustToContents)
        self.comboBox_readReportCfgDstMode.setMinimumContentsLength(0)

        self.horizontalLayout_54.addWidget(self.comboBox_readReportCfgDstMode)

        self.lineEdit_readReportCfgDstAddr = QLineEdit(self.scrollAreaWidgetContents_4)
        self.lineEdit_readReportCfgDstAddr.setObjectName(u"lineEdit_readReportCfgDstAddr")
        sizePolicy3.setHeightForWidth(self.lineEdit_readReportCfgDstAddr.sizePolicy().hasHeightForWidth())
        self.lineEdit_readReportCfgDstAddr.setSizePolicy(sizePolicy3)
        self.lineEdit_readReportCfgDstAddr.setMinimumSize(QSize(160, 0))
        self.lineEdit_readReportCfgDstAddr.setMaximumSize(QSize(160, 16777215))

        self.horizontalLayout_54.addWidget(self.lineEdit_readReportCfgDstAddr)

        self.lineEdit_readReportCfgSrcEp = QLineEdit(self.scrollAreaWidgetContents_4)
        self.lineEdit_readReportCfgSrcEp.setObjectName(u"lineEdit_readReportCfgSrcEp")
        self.lineEdit_readReportCfgSrcEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_54.addWidget(self.lineEdit_readReportCfgSrcEp)

        self.lineEdit_readReportCfgDstEp = QLineEdit(self.scrollAreaWidgetContents_4)
        self.lineEdit_readReportCfgDstEp.setObjectName(u"lineEdit_readReportCfgDstEp")
        self.lineEdit_readReportCfgDstEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_54.addWidget(self.lineEdit_readReportCfgDstEp)

        self.lineEdit_readReportCfgProfileId = QLineEdit(self.scrollAreaWidgetContents_4)
        self.lineEdit_readReportCfgProfileId.setObjectName(u"lineEdit_readReportCfgProfileId")
        self.lineEdit_readReportCfgProfileId.setMinimumSize(QSize(80, 0))
        self.lineEdit_readReportCfgProfileId.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_54.addWidget(self.lineEdit_readReportCfgProfileId)

        self.comboBox_readReportCfgDir = QComboBox(self.scrollAreaWidgetContents_4)
        self.comboBox_readReportCfgDir.addItem("")
        self.comboBox_readReportCfgDir.addItem("")
        self.comboBox_readReportCfgDir.setObjectName(u"comboBox_readReportCfgDir")
        self.comboBox_readReportCfgDir.setSizeAdjustPolicy(QComboBox.AdjustToContents)

        self.horizontalLayout_54.addWidget(self.comboBox_readReportCfgDir)

        self.lineEdit_readReportCfgClusterId = QLineEdit(self.scrollAreaWidgetContents_4)
        self.lineEdit_readReportCfgClusterId.setObjectName(u"lineEdit_readReportCfgClusterId")
        self.lineEdit_readReportCfgClusterId.setMinimumSize(QSize(80, 0))
        self.lineEdit_readReportCfgClusterId.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_54.addWidget(self.lineEdit_readReportCfgClusterId)

        self.horizontalSpacer_54 = QSpacerItem(40, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_54.addItem(self.horizontalSpacer_54)


        self.formLayout_readReportCfg.setLayout(0, QFormLayout.FieldRole, self.horizontalLayout_54)

        self.horizontalLayout_55 = QHBoxLayout()
        self.horizontalLayout_55.setSpacing(6)
        self.horizontalLayout_55.setObjectName(u"horizontalLayout_55")
        self.comboBox_readReportCfgType = QComboBox(self.scrollAreaWidgetContents_4)
        self.comboBox_readReportCfgType.addItem("")
        self.comboBox_readReportCfgType.addItem("")
        self.comboBox_readReportCfgType.setObjectName(u"comboBox_readReportCfgType")
        self.comboBox_readReportCfgType.setSizeAdjustPolicy(QComboBox.AdjustToContents)

        self.horizontalLayout_55.addWidget(self.comboBox_readReportCfgType)

        self.lineEdit_readReportCfgAttrId = QLineEdit(self.scrollAreaWidgetContents_4)
        self.lineEdit_readReportCfgAttrId.setObjectName(u"lineEdit_readReportCfgAttrId")
        self.lineEdit_readReportCfgAttrId.setMinimumSize(QSize(80, 0))
        self.lineEdit_readReportCfgAttrId.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_55.addWidget(self.lineEdit_readReportCfgAttrId)

        self.horizontalSpacer_16 = QSpacerItem(40, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_55.addItem(self.horizontalSpacer_16)


        self.formLayout_readReportCfg.setLayout(1, QFormLayout.FieldRole, self.horizontalLayout_55)


        self.gridLayout_7.addLayout(self.formLayout_readReportCfg, 3, 0, 1, 1)

        self.horizontalLayout_reset = QHBoxLayout()
        self.horizontalLayout_reset.setSpacing(6)
        self.horizontalLayout_reset.setObjectName(u"horizontalLayout_reset")
        self.pushButton_genReset = QPushButton(self.scrollAreaWidgetContents_4)
        self.pushButton_genReset.setObjectName(u"pushButton_genReset")

        self.horizontalLayout_reset.addWidget(self.pushButton_genReset)

        self.comboBox_resetDstMode = QComboBox(self.scrollAreaWidgetContents_4)
        self.comboBox_resetDstMode.addItem("")
        self.comboBox_resetDstMode.addItem("")
        self.comboBox_resetDstMode.addItem("")
        self.comboBox_resetDstMode.addItem("")
        self.comboBox_resetDstMode.addItem("")
        self.comboBox_resetDstMode.addItem("")
        self.comboBox_resetDstMode.addItem("")
        self.comboBox_resetDstMode.addItem("")
        self.comboBox_resetDstMode.setObjectName(u"comboBox_resetDstMode")
        self.comboBox_resetDstMode.setSizeAdjustPolicy(QComboBox.AdjustToContents)
        self.comboBox_resetDstMode.setMinimumContentsLength(0)

        self.horizontalLayout_reset.addWidget(self.comboBox_resetDstMode)

        self.lineEdit_resetDstAddr = QLineEdit(self.scrollAreaWidgetContents_4)
        self.lineEdit_resetDstAddr.setObjectName(u"lineEdit_resetDstAddr")
        sizePolicy3.setHeightForWidth(self.lineEdit_resetDstAddr.sizePolicy().hasHeightForWidth())
        self.lineEdit_resetDstAddr.setSizePolicy(sizePolicy3)
        self.lineEdit_resetDstAddr.setMinimumSize(QSize(160, 0))
        self.lineEdit_resetDstAddr.setMaximumSize(QSize(160, 16777215))

        self.horizontalLayout_reset.addWidget(self.lineEdit_resetDstAddr)

        self.lineEdit_resetSrcEp = QLineEdit(self.scrollAreaWidgetContents_4)
        self.lineEdit_resetSrcEp.setObjectName(u"lineEdit_resetSrcEp")
        self.lineEdit_resetSrcEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_reset.addWidget(self.lineEdit_resetSrcEp)

        self.lineEdit_resetDstEp = QLineEdit(self.scrollAreaWidgetContents_4)
        self.lineEdit_resetDstEp.setObjectName(u"lineEdit_resetDstEp")
        self.lineEdit_resetDstEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_reset.addWidget(self.lineEdit_resetDstEp)

        self.horizontalSpacer_22 = QSpacerItem(80, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_reset.addItem(self.horizontalSpacer_22)


        self.gridLayout_7.addLayout(self.horizontalLayout_reset, 4, 0, 1, 1)

        self.formLayout_configReport = QFormLayout()
        self.formLayout_configReport.setSpacing(6)
        self.formLayout_configReport.setObjectName(u"formLayout_configReport")
        self.pushButton_genConfig = QPushButton(self.scrollAreaWidgetContents_4)
        self.pushButton_genConfig.setObjectName(u"pushButton_genConfig")

        self.formLayout_configReport.setWidget(0, QFormLayout.LabelRole, self.pushButton_genConfig)

        self.horizontalLayout_52 = QHBoxLayout()
        self.horizontalLayout_52.setSpacing(6)
        self.horizontalLayout_52.setObjectName(u"horizontalLayout_52")
        self.comboBox_configReportDstMode = QComboBox(self.scrollAreaWidgetContents_4)
        self.comboBox_configReportDstMode.addItem("")
        self.comboBox_configReportDstMode.addItem("")
        self.comboBox_configReportDstMode.addItem("")
        self.comboBox_configReportDstMode.addItem("")
        self.comboBox_configReportDstMode.addItem("")
        self.comboBox_configReportDstMode.addItem("")
        self.comboBox_configReportDstMode.addItem("")
        self.comboBox_configReportDstMode.addItem("")
        self.comboBox_configReportDstMode.setObjectName(u"comboBox_configReportDstMode")
        self.comboBox_configReportDstMode.setMaxVisibleItems(10)
        self.comboBox_configReportDstMode.setSizeAdjustPolicy(QComboBox.AdjustToContents)
        self.comboBox_configReportDstMode.setMinimumContentsLength(0)

        self.horizontalLayout_52.addWidget(self.comboBox_configReportDstMode)

        self.lineEdit_configReportDstAddr = QLineEdit(self.scrollAreaWidgetContents_4)
        self.lineEdit_configReportDstAddr.setObjectName(u"lineEdit_configReportDstAddr")
        sizePolicy3.setHeightForWidth(self.lineEdit_configReportDstAddr.sizePolicy().hasHeightForWidth())
        self.lineEdit_configReportDstAddr.setSizePolicy(sizePolicy3)
        self.lineEdit_configReportDstAddr.setMinimumSize(QSize(160, 0))
        self.lineEdit_configReportDstAddr.setMaximumSize(QSize(160, 16777215))

        self.horizontalLayout_52.addWidget(self.lineEdit_configReportDstAddr)

        self.lineEdit_configReportSrcEp = QLineEdit(self.scrollAreaWidgetContents_4)
        self.lineEdit_configReportSrcEp.setObjectName(u"lineEdit_configReportSrcEp")
        self.lineEdit_configReportSrcEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_52.addWidget(self.lineEdit_configReportSrcEp)

        self.lineEdit_configReportDstEp = QLineEdit(self.scrollAreaWidgetContents_4)
        self.lineEdit_configReportDstEp.setObjectName(u"lineEdit_configReportDstEp")
        self.lineEdit_configReportDstEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_52.addWidget(self.lineEdit_configReportDstEp)

        self.lineEdit_configReportProfileId = QLineEdit(self.scrollAreaWidgetContents_4)
        self.lineEdit_configReportProfileId.setObjectName(u"lineEdit_configReportProfileId")
        self.lineEdit_configReportProfileId.setMinimumSize(QSize(80, 0))
        self.lineEdit_configReportProfileId.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_52.addWidget(self.lineEdit_configReportProfileId)

        self.comboBox_configReportDirect = QComboBox(self.scrollAreaWidgetContents_4)
        self.comboBox_configReportDirect.addItem("")
        self.comboBox_configReportDirect.addItem("")
        self.comboBox_configReportDirect.setObjectName(u"comboBox_configReportDirect")
        self.comboBox_configReportDirect.setSizeAdjustPolicy(QComboBox.AdjustToContents)

        self.horizontalLayout_52.addWidget(self.comboBox_configReportDirect)

        self.lineEdit_configReportClusterId = QLineEdit(self.scrollAreaWidgetContents_4)
        self.lineEdit_configReportClusterId.setObjectName(u"lineEdit_configReportClusterId")
        self.lineEdit_configReportClusterId.setMinimumSize(QSize(80, 0))
        self.lineEdit_configReportClusterId.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_52.addWidget(self.lineEdit_configReportClusterId)

        self.horizontalSpacer_53 = QSpacerItem(40, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_52.addItem(self.horizontalSpacer_53)


        self.formLayout_configReport.setLayout(0, QFormLayout.FieldRole, self.horizontalLayout_52)

        self.horizontalLayout_53 = QHBoxLayout()
        self.horizontalLayout_53.setSpacing(6)
        self.horizontalLayout_53.setObjectName(u"horizontalLayout_53")
        self.comboBox_configReportSendType = QComboBox(self.scrollAreaWidgetContents_4)
        self.comboBox_configReportSendType.addItem("")
        self.comboBox_configReportSendType.addItem("")
        self.comboBox_configReportSendType.setObjectName(u"comboBox_configReportSendType")
        self.comboBox_configReportSendType.setSizeAdjustPolicy(QComboBox.AdjustToContents)

        self.horizontalLayout_53.addWidget(self.comboBox_configReportSendType)

        self.lineEdit_configReportAttrId = QLineEdit(self.scrollAreaWidgetContents_4)
        self.lineEdit_configReportAttrId.setObjectName(u"lineEdit_configReportAttrId")
        self.lineEdit_configReportAttrId.setMinimumSize(QSize(80, 0))
        self.lineEdit_configReportAttrId.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_53.addWidget(self.lineEdit_configReportAttrId)

        self.comboBox_configReportDataType = QComboBox(self.scrollAreaWidgetContents_4)
        self.comboBox_configReportDataType.addItem("")
        self.comboBox_configReportDataType.addItem("")
        self.comboBox_configReportDataType.addItem("")
        self.comboBox_configReportDataType.addItem("")
        self.comboBox_configReportDataType.addItem("")
        self.comboBox_configReportDataType.addItem("")
        self.comboBox_configReportDataType.addItem("")
        self.comboBox_configReportDataType.addItem("")
        self.comboBox_configReportDataType.addItem("")
        self.comboBox_configReportDataType.addItem("")
        self.comboBox_configReportDataType.addItem("")
        self.comboBox_configReportDataType.addItem("")
        self.comboBox_configReportDataType.addItem("")
        self.comboBox_configReportDataType.addItem("")
        self.comboBox_configReportDataType.addItem("")
        self.comboBox_configReportDataType.addItem("")
        self.comboBox_configReportDataType.addItem("")
        self.comboBox_configReportDataType.addItem("")
        self.comboBox_configReportDataType.addItem("")
        self.comboBox_configReportDataType.addItem("")
        self.comboBox_configReportDataType.addItem("")
        self.comboBox_configReportDataType.addItem("")
        self.comboBox_configReportDataType.addItem("")
        self.comboBox_configReportDataType.addItem("")
        self.comboBox_configReportDataType.addItem("")
        self.comboBox_configReportDataType.addItem("")
        self.comboBox_configReportDataType.addItem("")
        self.comboBox_configReportDataType.addItem("")
        self.comboBox_configReportDataType.addItem("")
        self.comboBox_configReportDataType.addItem("")
        self.comboBox_configReportDataType.addItem("")
        self.comboBox_configReportDataType.addItem("")
        self.comboBox_configReportDataType.addItem("")
        self.comboBox_configReportDataType.addItem("")
        self.comboBox_configReportDataType.addItem("")
        self.comboBox_configReportDataType.addItem("")
        self.comboBox_configReportDataType.addItem("")
        self.comboBox_configReportDataType.addItem("")
        self.comboBox_configReportDataType.addItem("")
        self.comboBox_configReportDataType.addItem("")
        self.comboBox_configReportDataType.addItem("")
        self.comboBox_configReportDataType.addItem("")
        self.comboBox_configReportDataType.addItem("")
        self.comboBox_configReportDataType.addItem("")
        self.comboBox_configReportDataType.addItem("")
        self.comboBox_configReportDataType.addItem("")
        self.comboBox_configReportDataType.addItem("")
        self.comboBox_configReportDataType.addItem("")
        self.comboBox_configReportDataType.addItem("")
        self.comboBox_configReportDataType.addItem("")
        self.comboBox_configReportDataType.addItem("")
        self.comboBox_configReportDataType.addItem("")
        self.comboBox_configReportDataType.addItem("")
        self.comboBox_configReportDataType.addItem("")
        self.comboBox_configReportDataType.addItem("")
        self.comboBox_configReportDataType.addItem("")
        self.comboBox_configReportDataType.setObjectName(u"comboBox_configReportDataType")
        self.comboBox_configReportDataType.setSizeAdjustPolicy(QComboBox.AdjustToContents)

        self.horizontalLayout_53.addWidget(self.comboBox_configReportDataType)

        self.lineEdit_configReportMinInt = QLineEdit(self.scrollAreaWidgetContents_4)
        self.lineEdit_configReportMinInt.setObjectName(u"lineEdit_configReportMinInt")
        self.lineEdit_configReportMinInt.setMinimumSize(QSize(100, 0))
        self.lineEdit_configReportMinInt.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_53.addWidget(self.lineEdit_configReportMinInt)

        self.lineEdit_configReportMaxInt = QLineEdit(self.scrollAreaWidgetContents_4)
        self.lineEdit_configReportMaxInt.setObjectName(u"lineEdit_configReportMaxInt")
        self.lineEdit_configReportMaxInt.setMinimumSize(QSize(100, 0))
        self.lineEdit_configReportMaxInt.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_53.addWidget(self.lineEdit_configReportMaxInt)

        self.lineEdit_configReportChange = QLineEdit(self.scrollAreaWidgetContents_4)
        self.lineEdit_configReportChange.setObjectName(u"lineEdit_configReportChange")
        self.lineEdit_configReportChange.setMinimumSize(QSize(100, 0))
        self.lineEdit_configReportChange.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_53.addWidget(self.lineEdit_configReportChange)

        self.lineEdit_configReportTimeOut = QLineEdit(self.scrollAreaWidgetContents_4)
        self.lineEdit_configReportTimeOut.setObjectName(u"lineEdit_configReportTimeOut")
        self.lineEdit_configReportTimeOut.setMinimumSize(QSize(120, 0))
        self.lineEdit_configReportTimeOut.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_53.addWidget(self.lineEdit_configReportTimeOut)

        self.horizontalSpacer_17 = QSpacerItem(40, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_53.addItem(self.horizontalSpacer_17)


        self.formLayout_configReport.setLayout(1, QFormLayout.FieldRole, self.horizontalLayout_53)


        self.gridLayout_7.addLayout(self.formLayout_configReport, 2, 0, 1, 1)

        self.verticalSpacer_2 = QSpacerItem(20, 40, QSizePolicy.Minimum, QSizePolicy.Expanding)

        self.gridLayout_7.addItem(self.verticalSpacer_2, 5, 0, 1, 1)

        self.scrollArea_general.setWidget(self.scrollAreaWidgetContents_4)

        self.gridLayout_3.addWidget(self.scrollArea_general, 0, 0, 1, 1)

        self.tabWidget.addTab(self.general, "")
        self.tab_group = QWidget()
        self.tab_group.setObjectName(u"tab_group")
        self.gridLayout_10 = QGridLayout(self.tab_group)
        self.gridLayout_10.setSpacing(6)
        self.gridLayout_10.setContentsMargins(11, 11, 11, 11)
        self.gridLayout_10.setObjectName(u"gridLayout_10")
        self.scrollArea_group = QScrollArea(self.tab_group)
        self.scrollArea_group.setObjectName(u"scrollArea_group")
        palette4 = QPalette()
        palette4.setBrush(QPalette.Active, QPalette.Base, brush2)
        palette4.setBrush(QPalette.Inactive, QPalette.Base, brush2)
        palette4.setBrush(QPalette.Disabled, QPalette.Base, brush3)
        self.scrollArea_group.setPalette(palette4)
        self.scrollArea_group.setWidgetResizable(True)
        self.scrollAreaWidgetContents_7 = QWidget()
        self.scrollAreaWidgetContents_7.setObjectName(u"scrollAreaWidgetContents_7")
        self.scrollAreaWidgetContents_7.setGeometry(QRect(0, 0, 970, 316))
        self.gridLayout_8 = QGridLayout(self.scrollAreaWidgetContents_7)
        self.gridLayout_8.setSpacing(6)
        self.gridLayout_8.setContentsMargins(11, 11, 11, 11)
        self.gridLayout_8.setObjectName(u"gridLayout_8")
        self.horizontalLayout_addGroup = QHBoxLayout()
        self.horizontalLayout_addGroup.setSpacing(6)
        self.horizontalLayout_addGroup.setObjectName(u"horizontalLayout_addGroup")
        self.pushButton_addGroup = QPushButton(self.scrollAreaWidgetContents_7)
        self.pushButton_addGroup.setObjectName(u"pushButton_addGroup")

        self.horizontalLayout_addGroup.addWidget(self.pushButton_addGroup)

        self.comboBox_addGroupDstmode = QComboBox(self.scrollAreaWidgetContents_7)
        self.comboBox_addGroupDstmode.addItem("")
        self.comboBox_addGroupDstmode.addItem("")
        self.comboBox_addGroupDstmode.addItem("")
        self.comboBox_addGroupDstmode.addItem("")
        self.comboBox_addGroupDstmode.addItem("")
        self.comboBox_addGroupDstmode.addItem("")
        self.comboBox_addGroupDstmode.addItem("")
        self.comboBox_addGroupDstmode.addItem("")
        self.comboBox_addGroupDstmode.setObjectName(u"comboBox_addGroupDstmode")
        self.comboBox_addGroupDstmode.setMinimumContentsLength(0)

        self.horizontalLayout_addGroup.addWidget(self.comboBox_addGroupDstmode)

        self.lineEdit_addGroupDstAddr = QLineEdit(self.scrollAreaWidgetContents_7)
        self.lineEdit_addGroupDstAddr.setObjectName(u"lineEdit_addGroupDstAddr")
        sizePolicy3.setHeightForWidth(self.lineEdit_addGroupDstAddr.sizePolicy().hasHeightForWidth())
        self.lineEdit_addGroupDstAddr.setSizePolicy(sizePolicy3)
        self.lineEdit_addGroupDstAddr.setMinimumSize(QSize(160, 0))
        self.lineEdit_addGroupDstAddr.setMaximumSize(QSize(160, 16777215))

        self.horizontalLayout_addGroup.addWidget(self.lineEdit_addGroupDstAddr)

        self.lineEdit_addGroupSrcEp = QLineEdit(self.scrollAreaWidgetContents_7)
        self.lineEdit_addGroupSrcEp.setObjectName(u"lineEdit_addGroupSrcEp")
        self.lineEdit_addGroupSrcEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_addGroup.addWidget(self.lineEdit_addGroupSrcEp)

        self.lineEdit_addGroupDstEp = QLineEdit(self.scrollAreaWidgetContents_7)
        self.lineEdit_addGroupDstEp.setObjectName(u"lineEdit_addGroupDstEp")
        self.lineEdit_addGroupDstEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_addGroup.addWidget(self.lineEdit_addGroupDstEp)

        self.lineEdit_addGroupGroupId = QLineEdit(self.scrollAreaWidgetContents_7)
        self.lineEdit_addGroupGroupId.setObjectName(u"lineEdit_addGroupGroupId")
        self.lineEdit_addGroupGroupId.setMinimumSize(QSize(80, 0))
        self.lineEdit_addGroupGroupId.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_addGroup.addWidget(self.lineEdit_addGroupGroupId)

        self.lineEdit_addGroupName = QLineEdit(self.scrollAreaWidgetContents_7)
        self.lineEdit_addGroupName.setObjectName(u"lineEdit_addGroupName")
        self.lineEdit_addGroupName.setMinimumSize(QSize(160, 0))
        self.lineEdit_addGroupName.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_addGroup.addWidget(self.lineEdit_addGroupName)

        self.horizontalSpacer_11 = QSpacerItem(0, 0, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_addGroup.addItem(self.horizontalSpacer_11)


        self.gridLayout_8.addLayout(self.horizontalLayout_addGroup, 0, 0, 1, 1)

        self.horizontalLayout_removeGroup = QHBoxLayout()
        self.horizontalLayout_removeGroup.setSpacing(6)
        self.horizontalLayout_removeGroup.setObjectName(u"horizontalLayout_removeGroup")
        self.pushButton_removeGroup = QPushButton(self.scrollAreaWidgetContents_7)
        self.pushButton_removeGroup.setObjectName(u"pushButton_removeGroup")

        self.horizontalLayout_removeGroup.addWidget(self.pushButton_removeGroup)

        self.comboBox_rmGroupDstMode = QComboBox(self.scrollAreaWidgetContents_7)
        self.comboBox_rmGroupDstMode.addItem("")
        self.comboBox_rmGroupDstMode.addItem("")
        self.comboBox_rmGroupDstMode.addItem("")
        self.comboBox_rmGroupDstMode.addItem("")
        self.comboBox_rmGroupDstMode.addItem("")
        self.comboBox_rmGroupDstMode.addItem("")
        self.comboBox_rmGroupDstMode.addItem("")
        self.comboBox_rmGroupDstMode.addItem("")
        self.comboBox_rmGroupDstMode.setObjectName(u"comboBox_rmGroupDstMode")
        self.comboBox_rmGroupDstMode.setMaxVisibleItems(10)
        self.comboBox_rmGroupDstMode.setMinimumContentsLength(0)

        self.horizontalLayout_removeGroup.addWidget(self.comboBox_rmGroupDstMode)

        self.lineEdit_rmGroupDstAddr = QLineEdit(self.scrollAreaWidgetContents_7)
        self.lineEdit_rmGroupDstAddr.setObjectName(u"lineEdit_rmGroupDstAddr")
        sizePolicy3.setHeightForWidth(self.lineEdit_rmGroupDstAddr.sizePolicy().hasHeightForWidth())
        self.lineEdit_rmGroupDstAddr.setSizePolicy(sizePolicy3)
        self.lineEdit_rmGroupDstAddr.setMinimumSize(QSize(160, 0))
        self.lineEdit_rmGroupDstAddr.setMaximumSize(QSize(160, 16777215))

        self.horizontalLayout_removeGroup.addWidget(self.lineEdit_rmGroupDstAddr)

        self.lineEdit_rmGroupSrcEp = QLineEdit(self.scrollAreaWidgetContents_7)
        self.lineEdit_rmGroupSrcEp.setObjectName(u"lineEdit_rmGroupSrcEp")
        self.lineEdit_rmGroupSrcEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_removeGroup.addWidget(self.lineEdit_rmGroupSrcEp)

        self.lineEdit_rmGroupDstEp = QLineEdit(self.scrollAreaWidgetContents_7)
        self.lineEdit_rmGroupDstEp.setObjectName(u"lineEdit_rmGroupDstEp")
        self.lineEdit_rmGroupDstEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_removeGroup.addWidget(self.lineEdit_rmGroupDstEp)

        self.lineEdit_rmGroupId = QLineEdit(self.scrollAreaWidgetContents_7)
        self.lineEdit_rmGroupId.setObjectName(u"lineEdit_rmGroupId")
        self.lineEdit_rmGroupId.setMinimumSize(QSize(80, 0))
        self.lineEdit_rmGroupId.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_removeGroup.addWidget(self.lineEdit_rmGroupId)

        self.horizontalSpacer_13 = QSpacerItem(0, 0, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_removeGroup.addItem(self.horizontalSpacer_13)


        self.gridLayout_8.addLayout(self.horizontalLayout_removeGroup, 3, 0, 1, 1)

        self.horizontalLayout_getGroup = QHBoxLayout()
        self.horizontalLayout_getGroup.setSpacing(6)
        self.horizontalLayout_getGroup.setObjectName(u"horizontalLayout_getGroup")
        self.pushButton_getGroup = QPushButton(self.scrollAreaWidgetContents_7)
        self.pushButton_getGroup.setObjectName(u"pushButton_getGroup")

        self.horizontalLayout_getGroup.addWidget(self.pushButton_getGroup)

        self.comboBox_getGroupDstMode = QComboBox(self.scrollAreaWidgetContents_7)
        self.comboBox_getGroupDstMode.addItem("")
        self.comboBox_getGroupDstMode.addItem("")
        self.comboBox_getGroupDstMode.addItem("")
        self.comboBox_getGroupDstMode.addItem("")
        self.comboBox_getGroupDstMode.addItem("")
        self.comboBox_getGroupDstMode.addItem("")
        self.comboBox_getGroupDstMode.addItem("")
        self.comboBox_getGroupDstMode.addItem("")
        self.comboBox_getGroupDstMode.setObjectName(u"comboBox_getGroupDstMode")
        self.comboBox_getGroupDstMode.setMinimumContentsLength(0)

        self.horizontalLayout_getGroup.addWidget(self.comboBox_getGroupDstMode)

        self.lineEdit_getGroupDstAddr = QLineEdit(self.scrollAreaWidgetContents_7)
        self.lineEdit_getGroupDstAddr.setObjectName(u"lineEdit_getGroupDstAddr")
        sizePolicy3.setHeightForWidth(self.lineEdit_getGroupDstAddr.sizePolicy().hasHeightForWidth())
        self.lineEdit_getGroupDstAddr.setSizePolicy(sizePolicy3)
        self.lineEdit_getGroupDstAddr.setMinimumSize(QSize(160, 0))
        self.lineEdit_getGroupDstAddr.setMaximumSize(QSize(160, 16777215))

        self.horizontalLayout_getGroup.addWidget(self.lineEdit_getGroupDstAddr)

        self.lineEdit_getGroupSrcEp = QLineEdit(self.scrollAreaWidgetContents_7)
        self.lineEdit_getGroupSrcEp.setObjectName(u"lineEdit_getGroupSrcEp")
        self.lineEdit_getGroupSrcEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_getGroup.addWidget(self.lineEdit_getGroupSrcEp)

        self.lineEdit_getGroupDstEp = QLineEdit(self.scrollAreaWidgetContents_7)
        self.lineEdit_getGroupDstEp.setObjectName(u"lineEdit_getGroupDstEp")
        self.lineEdit_getGroupDstEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_getGroup.addWidget(self.lineEdit_getGroupDstEp)

        self.lineEdit_getGroupList = QLineEdit(self.scrollAreaWidgetContents_7)
        self.lineEdit_getGroupList.setObjectName(u"lineEdit_getGroupList")
        self.lineEdit_getGroupList.setMinimumSize(QSize(260, 0))
        self.lineEdit_getGroupList.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_getGroup.addWidget(self.lineEdit_getGroupList)

        self.horizontalSpacer_15 = QSpacerItem(0, 0, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_getGroup.addItem(self.horizontalSpacer_15)


        self.gridLayout_8.addLayout(self.horizontalLayout_getGroup, 2, 0, 1, 1)

        self.horizontalLayout_viewGroup = QHBoxLayout()
        self.horizontalLayout_viewGroup.setSpacing(6)
        self.horizontalLayout_viewGroup.setObjectName(u"horizontalLayout_viewGroup")
        self.pushButton_viewGroup = QPushButton(self.scrollAreaWidgetContents_7)
        self.pushButton_viewGroup.setObjectName(u"pushButton_viewGroup")

        self.horizontalLayout_viewGroup.addWidget(self.pushButton_viewGroup)

        self.comboBox_viewGroupDstMode = QComboBox(self.scrollAreaWidgetContents_7)
        self.comboBox_viewGroupDstMode.addItem("")
        self.comboBox_viewGroupDstMode.addItem("")
        self.comboBox_viewGroupDstMode.addItem("")
        self.comboBox_viewGroupDstMode.addItem("")
        self.comboBox_viewGroupDstMode.addItem("")
        self.comboBox_viewGroupDstMode.addItem("")
        self.comboBox_viewGroupDstMode.addItem("")
        self.comboBox_viewGroupDstMode.addItem("")
        self.comboBox_viewGroupDstMode.setObjectName(u"comboBox_viewGroupDstMode")
        self.comboBox_viewGroupDstMode.setMinimumContentsLength(0)

        self.horizontalLayout_viewGroup.addWidget(self.comboBox_viewGroupDstMode)

        self.lineEdit_viewGroupDstAddr = QLineEdit(self.scrollAreaWidgetContents_7)
        self.lineEdit_viewGroupDstAddr.setObjectName(u"lineEdit_viewGroupDstAddr")
        sizePolicy3.setHeightForWidth(self.lineEdit_viewGroupDstAddr.sizePolicy().hasHeightForWidth())
        self.lineEdit_viewGroupDstAddr.setSizePolicy(sizePolicy3)
        self.lineEdit_viewGroupDstAddr.setMinimumSize(QSize(160, 0))
        self.lineEdit_viewGroupDstAddr.setMaximumSize(QSize(160, 16777215))

        self.horizontalLayout_viewGroup.addWidget(self.lineEdit_viewGroupDstAddr)

        self.lineEdit_viewGroupSrcEp = QLineEdit(self.scrollAreaWidgetContents_7)
        self.lineEdit_viewGroupSrcEp.setObjectName(u"lineEdit_viewGroupSrcEp")
        self.lineEdit_viewGroupSrcEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_viewGroup.addWidget(self.lineEdit_viewGroupSrcEp)

        self.lineEdit_viewGroupDstEp = QLineEdit(self.scrollAreaWidgetContents_7)
        self.lineEdit_viewGroupDstEp.setObjectName(u"lineEdit_viewGroupDstEp")
        self.lineEdit_viewGroupDstEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_viewGroup.addWidget(self.lineEdit_viewGroupDstEp)

        self.lineEdit_viewGroupGroupId = QLineEdit(self.scrollAreaWidgetContents_7)
        self.lineEdit_viewGroupGroupId.setObjectName(u"lineEdit_viewGroupGroupId")
        self.lineEdit_viewGroupGroupId.setMinimumSize(QSize(80, 0))
        self.lineEdit_viewGroupGroupId.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_viewGroup.addWidget(self.lineEdit_viewGroupGroupId)

        self.horizontalSpacer_12 = QSpacerItem(0, 0, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_viewGroup.addItem(self.horizontalSpacer_12)


        self.gridLayout_8.addLayout(self.horizontalLayout_viewGroup, 1, 0, 1, 1)

        self.verticalSpacer_5 = QSpacerItem(0, 0, QSizePolicy.Minimum, QSizePolicy.Expanding)

        self.gridLayout_8.addItem(self.verticalSpacer_5, 6, 0, 1, 1)

        self.horizontalLayout_addIden = QHBoxLayout()
        self.horizontalLayout_addIden.setSpacing(6)
        self.horizontalLayout_addIden.setObjectName(u"horizontalLayout_addIden")
        self.pushButton_addIfIdentify = QPushButton(self.scrollAreaWidgetContents_7)
        self.pushButton_addIfIdentify.setObjectName(u"pushButton_addIfIdentify")

        self.horizontalLayout_addIden.addWidget(self.pushButton_addIfIdentify)

        self.comboBox_addIdenDstMode = QComboBox(self.scrollAreaWidgetContents_7)
        self.comboBox_addIdenDstMode.addItem("")
        self.comboBox_addIdenDstMode.addItem("")
        self.comboBox_addIdenDstMode.addItem("")
        self.comboBox_addIdenDstMode.addItem("")
        self.comboBox_addIdenDstMode.addItem("")
        self.comboBox_addIdenDstMode.addItem("")
        self.comboBox_addIdenDstMode.addItem("")
        self.comboBox_addIdenDstMode.addItem("")
        self.comboBox_addIdenDstMode.setObjectName(u"comboBox_addIdenDstMode")
        self.comboBox_addIdenDstMode.setMinimumContentsLength(0)

        self.horizontalLayout_addIden.addWidget(self.comboBox_addIdenDstMode)

        self.lineEdit_addIdenDstAddr = QLineEdit(self.scrollAreaWidgetContents_7)
        self.lineEdit_addIdenDstAddr.setObjectName(u"lineEdit_addIdenDstAddr")
        sizePolicy3.setHeightForWidth(self.lineEdit_addIdenDstAddr.sizePolicy().hasHeightForWidth())
        self.lineEdit_addIdenDstAddr.setSizePolicy(sizePolicy3)
        self.lineEdit_addIdenDstAddr.setMinimumSize(QSize(160, 0))
        self.lineEdit_addIdenDstAddr.setMaximumSize(QSize(160, 16777215))

        self.horizontalLayout_addIden.addWidget(self.lineEdit_addIdenDstAddr)

        self.lineEdit_addIdenSrcEp = QLineEdit(self.scrollAreaWidgetContents_7)
        self.lineEdit_addIdenSrcEp.setObjectName(u"lineEdit_addIdenSrcEp")
        self.lineEdit_addIdenSrcEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_addIden.addWidget(self.lineEdit_addIdenSrcEp)

        self.lineEdit_addIdenDstEp = QLineEdit(self.scrollAreaWidgetContents_7)
        self.lineEdit_addIdenDstEp.setObjectName(u"lineEdit_addIdenDstEp")
        self.lineEdit_addIdenDstEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_addIden.addWidget(self.lineEdit_addIdenDstEp)

        self.lineEdit_addIdenGroupId = QLineEdit(self.scrollAreaWidgetContents_7)
        self.lineEdit_addIdenGroupId.setObjectName(u"lineEdit_addIdenGroupId")
        self.lineEdit_addIdenGroupId.setMinimumSize(QSize(80, 0))
        self.lineEdit_addIdenGroupId.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_addIden.addWidget(self.lineEdit_addIdenGroupId)

        self.lineEdit_addIdenName = QLineEdit(self.scrollAreaWidgetContents_7)
        self.lineEdit_addIdenName.setObjectName(u"lineEdit_addIdenName")
        self.lineEdit_addIdenName.setMinimumSize(QSize(160, 0))
        self.lineEdit_addIdenName.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_addIden.addWidget(self.lineEdit_addIdenName)

        self.horizontalSpacer_31 = QSpacerItem(0, 0, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_addIden.addItem(self.horizontalSpacer_31)


        self.gridLayout_8.addLayout(self.horizontalLayout_addIden, 5, 0, 1, 1)

        self.horizontalLayout_removeAll = QHBoxLayout()
        self.horizontalLayout_removeAll.setSpacing(6)
        self.horizontalLayout_removeAll.setObjectName(u"horizontalLayout_removeAll")
        self.pushButton_removeAll = QPushButton(self.scrollAreaWidgetContents_7)
        self.pushButton_removeAll.setObjectName(u"pushButton_removeAll")

        self.horizontalLayout_removeAll.addWidget(self.pushButton_removeAll)

        self.comboBox_removeAllDstMode = QComboBox(self.scrollAreaWidgetContents_7)
        self.comboBox_removeAllDstMode.addItem("")
        self.comboBox_removeAllDstMode.addItem("")
        self.comboBox_removeAllDstMode.addItem("")
        self.comboBox_removeAllDstMode.addItem("")
        self.comboBox_removeAllDstMode.addItem("")
        self.comboBox_removeAllDstMode.addItem("")
        self.comboBox_removeAllDstMode.addItem("")
        self.comboBox_removeAllDstMode.addItem("")
        self.comboBox_removeAllDstMode.setObjectName(u"comboBox_removeAllDstMode")
        self.comboBox_removeAllDstMode.setMinimumContentsLength(0)

        self.horizontalLayout_removeAll.addWidget(self.comboBox_removeAllDstMode)

        self.lineEdit_removeAllDstAddr = QLineEdit(self.scrollAreaWidgetContents_7)
        self.lineEdit_removeAllDstAddr.setObjectName(u"lineEdit_removeAllDstAddr")
        sizePolicy3.setHeightForWidth(self.lineEdit_removeAllDstAddr.sizePolicy().hasHeightForWidth())
        self.lineEdit_removeAllDstAddr.setSizePolicy(sizePolicy3)
        self.lineEdit_removeAllDstAddr.setMinimumSize(QSize(160, 0))
        self.lineEdit_removeAllDstAddr.setMaximumSize(QSize(160, 16777215))

        self.horizontalLayout_removeAll.addWidget(self.lineEdit_removeAllDstAddr)

        self.lineEdit_removeAllSrcEp = QLineEdit(self.scrollAreaWidgetContents_7)
        self.lineEdit_removeAllSrcEp.setObjectName(u"lineEdit_removeAllSrcEp")
        self.lineEdit_removeAllSrcEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_removeAll.addWidget(self.lineEdit_removeAllSrcEp)

        self.lineEdit_removeAllDstEp = QLineEdit(self.scrollAreaWidgetContents_7)
        self.lineEdit_removeAllDstEp.setObjectName(u"lineEdit_removeAllDstEp")
        self.lineEdit_removeAllDstEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_removeAll.addWidget(self.lineEdit_removeAllDstEp)

        self.horizontalSpacer_14 = QSpacerItem(0, 0, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_removeAll.addItem(self.horizontalSpacer_14)


        self.gridLayout_8.addLayout(self.horizontalLayout_removeAll, 4, 0, 1, 1)

        self.scrollArea_group.setWidget(self.scrollAreaWidgetContents_7)

        self.gridLayout_10.addWidget(self.scrollArea_group, 0, 0, 1, 1)

        self.tabWidget.addTab(self.tab_group, "")
        self.on_off = QWidget()
        self.on_off.setObjectName(u"on_off")
        self.gridLayout_13 = QGridLayout(self.on_off)
        self.gridLayout_13.setSpacing(6)
        self.gridLayout_13.setContentsMargins(11, 11, 11, 11)
        self.gridLayout_13.setObjectName(u"gridLayout_13")
        self.scrollArea_5 = QScrollArea(self.on_off)
        self.scrollArea_5.setObjectName(u"scrollArea_5")
        palette5 = QPalette()
        palette5.setBrush(QPalette.Active, QPalette.Base, brush2)
        palette5.setBrush(QPalette.Inactive, QPalette.Base, brush2)
        palette5.setBrush(QPalette.Disabled, QPalette.Base, brush3)
        self.scrollArea_5.setPalette(palette5)
        self.scrollArea_5.setWidgetResizable(True)
        self.scrollAreaWidgetContents_9 = QWidget()
        self.scrollAreaWidgetContents_9.setObjectName(u"scrollAreaWidgetContents_9")
        self.scrollAreaWidgetContents_9.setGeometry(QRect(0, 0, 970, 316))
        self.gridLayout_12 = QGridLayout(self.scrollAreaWidgetContents_9)
        self.gridLayout_12.setSpacing(6)
        self.gridLayout_12.setContentsMargins(11, 11, 11, 11)
        self.gridLayout_12.setObjectName(u"gridLayout_12")
        self.horizontalLayout_onoff = QHBoxLayout()
        self.horizontalLayout_onoff.setSpacing(6)
        self.horizontalLayout_onoff.setObjectName(u"horizontalLayout_onoff")
        self.pushButton_onOff = QPushButton(self.scrollAreaWidgetContents_9)
        self.pushButton_onOff.setObjectName(u"pushButton_onOff")

        self.horizontalLayout_onoff.addWidget(self.pushButton_onOff)

        self.comboBox_onoffDstMode = QComboBox(self.scrollAreaWidgetContents_9)
        self.comboBox_onoffDstMode.addItem("")
        self.comboBox_onoffDstMode.addItem("")
        self.comboBox_onoffDstMode.addItem("")
        self.comboBox_onoffDstMode.addItem("")
        self.comboBox_onoffDstMode.addItem("")
        self.comboBox_onoffDstMode.addItem("")
        self.comboBox_onoffDstMode.addItem("")
        self.comboBox_onoffDstMode.addItem("")
        self.comboBox_onoffDstMode.setObjectName(u"comboBox_onoffDstMode")
        self.comboBox_onoffDstMode.setMinimumContentsLength(0)

        self.horizontalLayout_onoff.addWidget(self.comboBox_onoffDstMode)

        self.lineEdit_onoffDstAddr = QLineEdit(self.scrollAreaWidgetContents_9)
        self.lineEdit_onoffDstAddr.setObjectName(u"lineEdit_onoffDstAddr")
        sizePolicy3.setHeightForWidth(self.lineEdit_onoffDstAddr.sizePolicy().hasHeightForWidth())
        self.lineEdit_onoffDstAddr.setSizePolicy(sizePolicy3)
        self.lineEdit_onoffDstAddr.setMinimumSize(QSize(160, 0))
        self.lineEdit_onoffDstAddr.setMaximumSize(QSize(160, 16777215))

        self.horizontalLayout_onoff.addWidget(self.lineEdit_onoffDstAddr)

        self.lineEdit_onoffSrcEp = QLineEdit(self.scrollAreaWidgetContents_9)
        self.lineEdit_onoffSrcEp.setObjectName(u"lineEdit_onoffSrcEp")
        self.lineEdit_onoffSrcEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_onoff.addWidget(self.lineEdit_onoffSrcEp)

        self.lineEdit_onoffDstEp = QLineEdit(self.scrollAreaWidgetContents_9)
        self.lineEdit_onoffDstEp.setObjectName(u"lineEdit_onoffDstEp")
        self.lineEdit_onoffDstEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_onoff.addWidget(self.lineEdit_onoffDstEp)

        self.comboBox_onOff = QComboBox(self.scrollAreaWidgetContents_9)
        self.comboBox_onOff.addItem("")
        self.comboBox_onOff.addItem("")
        self.comboBox_onOff.addItem("")
        self.comboBox_onOff.setObjectName(u"comboBox_onOff")
        self.comboBox_onOff.setSizeAdjustPolicy(QComboBox.AdjustToContents)

        self.horizontalLayout_onoff.addWidget(self.comboBox_onOff)

        self.horizontalSpacer_24 = QSpacerItem(0, 0, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_onoff.addItem(self.horizontalSpacer_24)


        self.gridLayout_12.addLayout(self.horizontalLayout_onoff, 0, 0, 1, 1)

        self.verticalSpacer_7 = QSpacerItem(0, 0, QSizePolicy.Minimum, QSizePolicy.Expanding)

        self.gridLayout_12.addItem(self.verticalSpacer_7, 1, 0, 1, 1)

        self.scrollArea_5.setWidget(self.scrollAreaWidgetContents_9)

        self.gridLayout_13.addWidget(self.scrollArea_5, 0, 0, 1, 1)

        self.tabWidget.addTab(self.on_off, "")
        self.level = QWidget()
        self.level.setObjectName(u"level")
        self.gridLayout_15 = QGridLayout(self.level)
        self.gridLayout_15.setSpacing(6)
        self.gridLayout_15.setContentsMargins(11, 11, 11, 11)
        self.gridLayout_15.setObjectName(u"gridLayout_15")
        self.scrollArea_6 = QScrollArea(self.level)
        self.scrollArea_6.setObjectName(u"scrollArea_6")
        palette6 = QPalette()
        palette6.setBrush(QPalette.Active, QPalette.Base, brush2)
        palette6.setBrush(QPalette.Inactive, QPalette.Base, brush2)
        palette6.setBrush(QPalette.Disabled, QPalette.Base, brush3)
        self.scrollArea_6.setPalette(palette6)
        self.scrollArea_6.setWidgetResizable(True)
        self.scrollAreaWidgetContents_10 = QWidget()
        self.scrollAreaWidgetContents_10.setObjectName(u"scrollAreaWidgetContents_10")
        self.scrollAreaWidgetContents_10.setGeometry(QRect(0, 0, 1000, 295))
        self.gridLayout_14 = QGridLayout(self.scrollAreaWidgetContents_10)
        self.gridLayout_14.setSpacing(6)
        self.gridLayout_14.setContentsMargins(11, 11, 11, 11)
        self.gridLayout_14.setObjectName(u"gridLayout_14")
        self.horizontalLayout_toLevel = QHBoxLayout()
        self.horizontalLayout_toLevel.setSpacing(6)
        self.horizontalLayout_toLevel.setObjectName(u"horizontalLayout_toLevel")
        self.pushButton_moveToLevel = QPushButton(self.scrollAreaWidgetContents_10)
        self.pushButton_moveToLevel.setObjectName(u"pushButton_moveToLevel")

        self.horizontalLayout_toLevel.addWidget(self.pushButton_moveToLevel)

        self.comboBox_toLevelDstMode = QComboBox(self.scrollAreaWidgetContents_10)
        self.comboBox_toLevelDstMode.addItem("")
        self.comboBox_toLevelDstMode.addItem("")
        self.comboBox_toLevelDstMode.addItem("")
        self.comboBox_toLevelDstMode.addItem("")
        self.comboBox_toLevelDstMode.addItem("")
        self.comboBox_toLevelDstMode.addItem("")
        self.comboBox_toLevelDstMode.addItem("")
        self.comboBox_toLevelDstMode.addItem("")
        self.comboBox_toLevelDstMode.setObjectName(u"comboBox_toLevelDstMode")
        self.comboBox_toLevelDstMode.setMinimumContentsLength(0)

        self.horizontalLayout_toLevel.addWidget(self.comboBox_toLevelDstMode)

        self.lineEdit_toLevelDstAddr = QLineEdit(self.scrollAreaWidgetContents_10)
        self.lineEdit_toLevelDstAddr.setObjectName(u"lineEdit_toLevelDstAddr")
        sizePolicy3.setHeightForWidth(self.lineEdit_toLevelDstAddr.sizePolicy().hasHeightForWidth())
        self.lineEdit_toLevelDstAddr.setSizePolicy(sizePolicy3)
        self.lineEdit_toLevelDstAddr.setMinimumSize(QSize(160, 0))
        self.lineEdit_toLevelDstAddr.setMaximumSize(QSize(160, 16777215))

        self.horizontalLayout_toLevel.addWidget(self.lineEdit_toLevelDstAddr)

        self.lineEdit_toLevelSrcEp = QLineEdit(self.scrollAreaWidgetContents_10)
        self.lineEdit_toLevelSrcEp.setObjectName(u"lineEdit_toLevelSrcEp")
        self.lineEdit_toLevelSrcEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_toLevel.addWidget(self.lineEdit_toLevelSrcEp)

        self.lineEdit_toLevelDstEp = QLineEdit(self.scrollAreaWidgetContents_10)
        self.lineEdit_toLevelDstEp.setObjectName(u"lineEdit_toLevelDstEp")
        self.lineEdit_toLevelDstEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_toLevel.addWidget(self.lineEdit_toLevelDstEp)

        self.lineEdit_toLevelLevel = QLineEdit(self.scrollAreaWidgetContents_10)
        self.lineEdit_toLevelLevel.setObjectName(u"lineEdit_toLevelLevel")
        self.lineEdit_toLevelLevel.setMinimumSize(QSize(100, 0))
        self.lineEdit_toLevelLevel.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_toLevel.addWidget(self.lineEdit_toLevelLevel)

        self.lineEdit_toLevelTime = QLineEdit(self.scrollAreaWidgetContents_10)
        self.lineEdit_toLevelTime.setObjectName(u"lineEdit_toLevelTime")
        self.lineEdit_toLevelTime.setMinimumSize(QSize(120, 0))
        self.lineEdit_toLevelTime.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_toLevel.addWidget(self.lineEdit_toLevelTime)

        self.comboBox_withOnOff = QComboBox(self.scrollAreaWidgetContents_10)
        self.comboBox_withOnOff.addItem("")
        self.comboBox_withOnOff.addItem("")
        self.comboBox_withOnOff.setObjectName(u"comboBox_withOnOff")
        self.comboBox_withOnOff.setSizeAdjustPolicy(QComboBox.AdjustToContents)

        self.horizontalLayout_toLevel.addWidget(self.comboBox_withOnOff)

        self.horizontalSpacer_40 = QSpacerItem(0, 0, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_toLevel.addItem(self.horizontalSpacer_40)


        self.gridLayout_14.addLayout(self.horizontalLayout_toLevel, 0, 0, 1, 1)

        self.horizontalLayout_stop = QHBoxLayout()
        self.horizontalLayout_stop.setSpacing(6)
        self.horizontalLayout_stop.setObjectName(u"horizontalLayout_stop")
        self.pushButton_stop = QPushButton(self.scrollAreaWidgetContents_10)
        self.pushButton_stop.setObjectName(u"pushButton_stop")

        self.horizontalLayout_stop.addWidget(self.pushButton_stop)

        self.comboBox_stopDstMode = QComboBox(self.scrollAreaWidgetContents_10)
        self.comboBox_stopDstMode.addItem("")
        self.comboBox_stopDstMode.addItem("")
        self.comboBox_stopDstMode.addItem("")
        self.comboBox_stopDstMode.addItem("")
        self.comboBox_stopDstMode.addItem("")
        self.comboBox_stopDstMode.addItem("")
        self.comboBox_stopDstMode.addItem("")
        self.comboBox_stopDstMode.addItem("")
        self.comboBox_stopDstMode.setObjectName(u"comboBox_stopDstMode")
        self.comboBox_stopDstMode.setMaxVisibleItems(10)
        self.comboBox_stopDstMode.setMinimumContentsLength(0)

        self.horizontalLayout_stop.addWidget(self.comboBox_stopDstMode)

        self.lineEdit_stopDstAddr = QLineEdit(self.scrollAreaWidgetContents_10)
        self.lineEdit_stopDstAddr.setObjectName(u"lineEdit_stopDstAddr")
        sizePolicy3.setHeightForWidth(self.lineEdit_stopDstAddr.sizePolicy().hasHeightForWidth())
        self.lineEdit_stopDstAddr.setSizePolicy(sizePolicy3)
        self.lineEdit_stopDstAddr.setMinimumSize(QSize(160, 0))
        self.lineEdit_stopDstAddr.setMaximumSize(QSize(160, 16777215))

        self.horizontalLayout_stop.addWidget(self.lineEdit_stopDstAddr)

        self.lineEdit_stopSrcEp = QLineEdit(self.scrollAreaWidgetContents_10)
        self.lineEdit_stopSrcEp.setObjectName(u"lineEdit_stopSrcEp")
        self.lineEdit_stopSrcEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_stop.addWidget(self.lineEdit_stopSrcEp)

        self.lineEdit_stopSrcEp_2 = QLineEdit(self.scrollAreaWidgetContents_10)
        self.lineEdit_stopSrcEp_2.setObjectName(u"lineEdit_stopSrcEp_2")
        self.lineEdit_stopSrcEp_2.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_stop.addWidget(self.lineEdit_stopSrcEp_2)

        self.comboBox_stopWithOnoff = QComboBox(self.scrollAreaWidgetContents_10)
        self.comboBox_stopWithOnoff.addItem("")
        self.comboBox_stopWithOnoff.addItem("")
        self.comboBox_stopWithOnoff.setObjectName(u"comboBox_stopWithOnoff")
        self.comboBox_stopWithOnoff.setSizeAdjustPolicy(QComboBox.AdjustToContents)

        self.horizontalLayout_stop.addWidget(self.comboBox_stopWithOnoff)

        self.horizontalSpacer_41 = QSpacerItem(0, 0, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_stop.addItem(self.horizontalSpacer_41)


        self.gridLayout_14.addLayout(self.horizontalLayout_stop, 3, 0, 1, 1)

        self.horizontalLayout_move = QHBoxLayout()
        self.horizontalLayout_move.setSpacing(6)
        self.horizontalLayout_move.setObjectName(u"horizontalLayout_move")
        self.pushButton_move = QPushButton(self.scrollAreaWidgetContents_10)
        self.pushButton_move.setObjectName(u"pushButton_move")

        self.horizontalLayout_move.addWidget(self.pushButton_move)

        self.comboBox_moveDstMode = QComboBox(self.scrollAreaWidgetContents_10)
        self.comboBox_moveDstMode.addItem("")
        self.comboBox_moveDstMode.addItem("")
        self.comboBox_moveDstMode.addItem("")
        self.comboBox_moveDstMode.addItem("")
        self.comboBox_moveDstMode.addItem("")
        self.comboBox_moveDstMode.addItem("")
        self.comboBox_moveDstMode.addItem("")
        self.comboBox_moveDstMode.addItem("")
        self.comboBox_moveDstMode.setObjectName(u"comboBox_moveDstMode")
        self.comboBox_moveDstMode.setMinimumContentsLength(0)

        self.horizontalLayout_move.addWidget(self.comboBox_moveDstMode)

        self.lineEdit_moveDstAddr = QLineEdit(self.scrollAreaWidgetContents_10)
        self.lineEdit_moveDstAddr.setObjectName(u"lineEdit_moveDstAddr")
        sizePolicy3.setHeightForWidth(self.lineEdit_moveDstAddr.sizePolicy().hasHeightForWidth())
        self.lineEdit_moveDstAddr.setSizePolicy(sizePolicy3)
        self.lineEdit_moveDstAddr.setMinimumSize(QSize(160, 0))
        self.lineEdit_moveDstAddr.setMaximumSize(QSize(160, 16777215))

        self.horizontalLayout_move.addWidget(self.lineEdit_moveDstAddr)

        self.lineEdit_moveSrcEp = QLineEdit(self.scrollAreaWidgetContents_10)
        self.lineEdit_moveSrcEp.setObjectName(u"lineEdit_moveSrcEp")
        self.lineEdit_moveSrcEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_move.addWidget(self.lineEdit_moveSrcEp)

        self.lineEdit_moveDstEp = QLineEdit(self.scrollAreaWidgetContents_10)
        self.lineEdit_moveDstEp.setObjectName(u"lineEdit_moveDstEp")
        self.lineEdit_moveDstEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_move.addWidget(self.lineEdit_moveDstEp)

        self.lineEdit_moveMode = QLineEdit(self.scrollAreaWidgetContents_10)
        self.lineEdit_moveMode.setObjectName(u"lineEdit_moveMode")
        self.lineEdit_moveMode.setMinimumSize(QSize(80, 0))
        self.lineEdit_moveMode.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_move.addWidget(self.lineEdit_moveMode)

        self.lineEdit_moveRate = QLineEdit(self.scrollAreaWidgetContents_10)
        self.lineEdit_moveRate.setObjectName(u"lineEdit_moveRate")
        self.lineEdit_moveRate.setMinimumSize(QSize(80, 0))
        self.lineEdit_moveRate.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_move.addWidget(self.lineEdit_moveRate)

        self.comboBox_moveWithOnoff = QComboBox(self.scrollAreaWidgetContents_10)
        self.comboBox_moveWithOnoff.addItem("")
        self.comboBox_moveWithOnoff.addItem("")
        self.comboBox_moveWithOnoff.setObjectName(u"comboBox_moveWithOnoff")
        self.comboBox_moveWithOnoff.setSizeAdjustPolicy(QComboBox.AdjustToContents)

        self.horizontalLayout_move.addWidget(self.comboBox_moveWithOnoff)

        self.horizontalSpacer_43 = QSpacerItem(0, 0, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_move.addItem(self.horizontalSpacer_43)


        self.gridLayout_14.addLayout(self.horizontalLayout_move, 1, 0, 1, 1)

        self.horizontalLayout_step = QHBoxLayout()
        self.horizontalLayout_step.setSpacing(6)
        self.horizontalLayout_step.setObjectName(u"horizontalLayout_step")
        self.pushButton_step = QPushButton(self.scrollAreaWidgetContents_10)
        self.pushButton_step.setObjectName(u"pushButton_step")

        self.horizontalLayout_step.addWidget(self.pushButton_step)

        self.comboBox_stepDstMode = QComboBox(self.scrollAreaWidgetContents_10)
        self.comboBox_stepDstMode.addItem("")
        self.comboBox_stepDstMode.addItem("")
        self.comboBox_stepDstMode.addItem("")
        self.comboBox_stepDstMode.addItem("")
        self.comboBox_stepDstMode.addItem("")
        self.comboBox_stepDstMode.addItem("")
        self.comboBox_stepDstMode.addItem("")
        self.comboBox_stepDstMode.addItem("")
        self.comboBox_stepDstMode.setObjectName(u"comboBox_stepDstMode")
        self.comboBox_stepDstMode.setMinimumContentsLength(0)

        self.horizontalLayout_step.addWidget(self.comboBox_stepDstMode)

        self.lineEdit_stepDstAddr = QLineEdit(self.scrollAreaWidgetContents_10)
        self.lineEdit_stepDstAddr.setObjectName(u"lineEdit_stepDstAddr")
        sizePolicy3.setHeightForWidth(self.lineEdit_stepDstAddr.sizePolicy().hasHeightForWidth())
        self.lineEdit_stepDstAddr.setSizePolicy(sizePolicy3)
        self.lineEdit_stepDstAddr.setMinimumSize(QSize(160, 0))
        self.lineEdit_stepDstAddr.setMaximumSize(QSize(160, 16777215))

        self.horizontalLayout_step.addWidget(self.lineEdit_stepDstAddr)

        self.lineEdit_stepSrcEp = QLineEdit(self.scrollAreaWidgetContents_10)
        self.lineEdit_stepSrcEp.setObjectName(u"lineEdit_stepSrcEp")
        self.lineEdit_stepSrcEp.setMinimumSize(QSize(60, 0))
        self.lineEdit_stepSrcEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_step.addWidget(self.lineEdit_stepSrcEp)

        self.lineEdit_stepDstEp = QLineEdit(self.scrollAreaWidgetContents_10)
        self.lineEdit_stepDstEp.setObjectName(u"lineEdit_stepDstEp")
        self.lineEdit_stepDstEp.setMinimumSize(QSize(60, 0))
        self.lineEdit_stepDstEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_step.addWidget(self.lineEdit_stepDstEp)

        self.lineEdit_stepMode = QLineEdit(self.scrollAreaWidgetContents_10)
        self.lineEdit_stepMode.setObjectName(u"lineEdit_stepMode")
        self.lineEdit_stepMode.setMinimumSize(QSize(80, 0))
        self.lineEdit_stepMode.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_step.addWidget(self.lineEdit_stepMode)

        self.lineEdit_stepSize = QLineEdit(self.scrollAreaWidgetContents_10)
        self.lineEdit_stepSize.setObjectName(u"lineEdit_stepSize")
        self.lineEdit_stepSize.setMinimumSize(QSize(80, 0))
        self.lineEdit_stepSize.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_step.addWidget(self.lineEdit_stepSize)

        self.lineEdit_stepTime = QLineEdit(self.scrollAreaWidgetContents_10)
        self.lineEdit_stepTime.setObjectName(u"lineEdit_stepTime")
        self.lineEdit_stepTime.setMinimumSize(QSize(120, 0))
        self.lineEdit_stepTime.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_step.addWidget(self.lineEdit_stepTime)

        self.comboBox_stepWithOnoff = QComboBox(self.scrollAreaWidgetContents_10)
        self.comboBox_stepWithOnoff.addItem("")
        self.comboBox_stepWithOnoff.addItem("")
        self.comboBox_stepWithOnoff.setObjectName(u"comboBox_stepWithOnoff")
        self.comboBox_stepWithOnoff.setSizeAdjustPolicy(QComboBox.AdjustToContents)

        self.horizontalLayout_step.addWidget(self.comboBox_stepWithOnoff)

        self.horizontalSpacer_42 = QSpacerItem(0, 0, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_step.addItem(self.horizontalSpacer_42)


        self.gridLayout_14.addLayout(self.horizontalLayout_step, 2, 0, 1, 1)

        self.verticalSpacer_8 = QSpacerItem(0, 0, QSizePolicy.Minimum, QSizePolicy.Expanding)

        self.gridLayout_14.addItem(self.verticalSpacer_8, 4, 0, 1, 1)

        self.scrollArea_6.setWidget(self.scrollAreaWidgetContents_10)

        self.gridLayout_15.addWidget(self.scrollArea_6, 0, 0, 1, 1)

        self.tabWidget.addTab(self.level, "")
        self.color = QWidget()
        self.color.setObjectName(u"color")
        self.gridLayout_17 = QGridLayout(self.color)
        self.gridLayout_17.setSpacing(6)
        self.gridLayout_17.setContentsMargins(11, 11, 11, 11)
        self.gridLayout_17.setObjectName(u"gridLayout_17")
        self.scrollArea_7 = QScrollArea(self.color)
        self.scrollArea_7.setObjectName(u"scrollArea_7")
        palette7 = QPalette()
        palette7.setBrush(QPalette.Active, QPalette.Base, brush2)
        palette7.setBrush(QPalette.Inactive, QPalette.Base, brush2)
        palette7.setBrush(QPalette.Disabled, QPalette.Base, brush3)
        self.scrollArea_7.setPalette(palette7)
        self.scrollArea_7.setWidgetResizable(True)
        self.scrollAreaWidgetContents_11 = QWidget()
        self.scrollAreaWidgetContents_11.setObjectName(u"scrollAreaWidgetContents_11")
        self.scrollAreaWidgetContents_11.setGeometry(QRect(0, 0, 970, 316))
        self.gridLayout_16 = QGridLayout(self.scrollAreaWidgetContents_11)
        self.gridLayout_16.setSpacing(6)
        self.gridLayout_16.setContentsMargins(11, 11, 11, 11)
        self.gridLayout_16.setObjectName(u"gridLayout_16")
        self.horizontalLayout_moveHue = QHBoxLayout()
        self.horizontalLayout_moveHue.setSpacing(6)
        self.horizontalLayout_moveHue.setObjectName(u"horizontalLayout_moveHue")
        self.pushButton_moveHue = QPushButton(self.scrollAreaWidgetContents_11)
        self.pushButton_moveHue.setObjectName(u"pushButton_moveHue")
        self.pushButton_moveHue.setMinimumSize(QSize(161, 0))

        self.horizontalLayout_moveHue.addWidget(self.pushButton_moveHue)

        self.comboBox_moveHueDstMode = QComboBox(self.scrollAreaWidgetContents_11)
        self.comboBox_moveHueDstMode.addItem("")
        self.comboBox_moveHueDstMode.addItem("")
        self.comboBox_moveHueDstMode.addItem("")
        self.comboBox_moveHueDstMode.addItem("")
        self.comboBox_moveHueDstMode.addItem("")
        self.comboBox_moveHueDstMode.addItem("")
        self.comboBox_moveHueDstMode.addItem("")
        self.comboBox_moveHueDstMode.addItem("")
        self.comboBox_moveHueDstMode.setObjectName(u"comboBox_moveHueDstMode")
        self.comboBox_moveHueDstMode.setMinimumContentsLength(0)

        self.horizontalLayout_moveHue.addWidget(self.comboBox_moveHueDstMode)

        self.lineEdit_moveHueDstAddr = QLineEdit(self.scrollAreaWidgetContents_11)
        self.lineEdit_moveHueDstAddr.setObjectName(u"lineEdit_moveHueDstAddr")
        sizePolicy3.setHeightForWidth(self.lineEdit_moveHueDstAddr.sizePolicy().hasHeightForWidth())
        self.lineEdit_moveHueDstAddr.setSizePolicy(sizePolicy3)
        self.lineEdit_moveHueDstAddr.setMinimumSize(QSize(160, 0))
        self.lineEdit_moveHueDstAddr.setMaximumSize(QSize(160, 16777215))

        self.horizontalLayout_moveHue.addWidget(self.lineEdit_moveHueDstAddr)

        self.lineEdit_moveHueSrcEp = QLineEdit(self.scrollAreaWidgetContents_11)
        self.lineEdit_moveHueSrcEp.setObjectName(u"lineEdit_moveHueSrcEp")
        self.lineEdit_moveHueSrcEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_moveHue.addWidget(self.lineEdit_moveHueSrcEp)

        self.lineEdit_moveHueDstEp = QLineEdit(self.scrollAreaWidgetContents_11)
        self.lineEdit_moveHueDstEp.setObjectName(u"lineEdit_moveHueDstEp")
        self.lineEdit_moveHueDstEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_moveHue.addWidget(self.lineEdit_moveHueDstEp)

        self.lineEdit_moveHueHue = QLineEdit(self.scrollAreaWidgetContents_11)
        self.lineEdit_moveHueHue.setObjectName(u"lineEdit_moveHueHue")
        self.lineEdit_moveHueHue.setMinimumSize(QSize(80, 0))
        self.lineEdit_moveHueHue.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_moveHue.addWidget(self.lineEdit_moveHueHue)

        self.lineEdit_moveHueDir = QLineEdit(self.scrollAreaWidgetContents_11)
        self.lineEdit_moveHueDir.setObjectName(u"lineEdit_moveHueDir")
        self.lineEdit_moveHueDir.setMinimumSize(QSize(80, 0))
        self.lineEdit_moveHueDir.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_moveHue.addWidget(self.lineEdit_moveHueDir)

        self.lineEdit_moveHueTime = QLineEdit(self.scrollAreaWidgetContents_11)
        self.lineEdit_moveHueTime.setObjectName(u"lineEdit_moveHueTime")
        self.lineEdit_moveHueTime.setMinimumSize(QSize(120, 0))
        self.lineEdit_moveHueTime.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_moveHue.addWidget(self.lineEdit_moveHueTime)

        self.horizontalSpacer_44 = QSpacerItem(0, 0, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_moveHue.addItem(self.horizontalSpacer_44)


        self.gridLayout_16.addLayout(self.horizontalLayout_moveHue, 0, 0, 1, 1)

        self.horizontalLayout_moveTemp = QHBoxLayout()
        self.horizontalLayout_moveTemp.setSpacing(6)
        self.horizontalLayout_moveTemp.setObjectName(u"horizontalLayout_moveTemp")
        self.pushButton_temperature = QPushButton(self.scrollAreaWidgetContents_11)
        self.pushButton_temperature.setObjectName(u"pushButton_temperature")

        self.horizontalLayout_moveTemp.addWidget(self.pushButton_temperature)

        self.comboBox_moveTempDstMode = QComboBox(self.scrollAreaWidgetContents_11)
        self.comboBox_moveTempDstMode.addItem("")
        self.comboBox_moveTempDstMode.addItem("")
        self.comboBox_moveTempDstMode.addItem("")
        self.comboBox_moveTempDstMode.addItem("")
        self.comboBox_moveTempDstMode.addItem("")
        self.comboBox_moveTempDstMode.addItem("")
        self.comboBox_moveTempDstMode.addItem("")
        self.comboBox_moveTempDstMode.addItem("")
        self.comboBox_moveTempDstMode.setObjectName(u"comboBox_moveTempDstMode")
        self.comboBox_moveTempDstMode.setMaxVisibleItems(10)
        self.comboBox_moveTempDstMode.setMinimumContentsLength(0)

        self.horizontalLayout_moveTemp.addWidget(self.comboBox_moveTempDstMode)

        self.lineEdit_moveTempDstAddr = QLineEdit(self.scrollAreaWidgetContents_11)
        self.lineEdit_moveTempDstAddr.setObjectName(u"lineEdit_moveTempDstAddr")
        sizePolicy3.setHeightForWidth(self.lineEdit_moveTempDstAddr.sizePolicy().hasHeightForWidth())
        self.lineEdit_moveTempDstAddr.setSizePolicy(sizePolicy3)
        self.lineEdit_moveTempDstAddr.setMinimumSize(QSize(160, 0))
        self.lineEdit_moveTempDstAddr.setMaximumSize(QSize(160, 16777215))

        self.horizontalLayout_moveTemp.addWidget(self.lineEdit_moveTempDstAddr)

        self.lineEdit_moveTempSrcEp = QLineEdit(self.scrollAreaWidgetContents_11)
        self.lineEdit_moveTempSrcEp.setObjectName(u"lineEdit_moveTempSrcEp")
        self.lineEdit_moveTempSrcEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_moveTemp.addWidget(self.lineEdit_moveTempSrcEp)

        self.lineEdit_moveTempDstEp = QLineEdit(self.scrollAreaWidgetContents_11)
        self.lineEdit_moveTempDstEp.setObjectName(u"lineEdit_moveTempDstEp")
        self.lineEdit_moveTempDstEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_moveTemp.addWidget(self.lineEdit_moveTempDstEp)

        self.lineEdit_moveTempTemp = QLineEdit(self.scrollAreaWidgetContents_11)
        self.lineEdit_moveTempTemp.setObjectName(u"lineEdit_moveTempTemp")
        self.lineEdit_moveTempTemp.setMinimumSize(QSize(100, 0))
        self.lineEdit_moveTempTemp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_moveTemp.addWidget(self.lineEdit_moveTempTemp)

        self.lineEdit_moveTempTime = QLineEdit(self.scrollAreaWidgetContents_11)
        self.lineEdit_moveTempTime.setObjectName(u"lineEdit_moveTempTime")
        self.lineEdit_moveTempTime.setMinimumSize(QSize(120, 0))
        self.lineEdit_moveTempTime.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_moveTemp.addWidget(self.lineEdit_moveTempTime)

        self.horizontalSpacer_45 = QSpacerItem(40, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_moveTemp.addItem(self.horizontalSpacer_45)


        self.gridLayout_16.addLayout(self.horizontalLayout_moveTemp, 4, 0, 1, 1)

        self.horizontalLayout_moveSat = QHBoxLayout()
        self.horizontalLayout_moveSat.setSpacing(6)
        self.horizontalLayout_moveSat.setObjectName(u"horizontalLayout_moveSat")
        self.pushButton_saturation = QPushButton(self.scrollAreaWidgetContents_11)
        self.pushButton_saturation.setObjectName(u"pushButton_saturation")
        self.pushButton_saturation.setMinimumSize(QSize(161, 0))

        self.horizontalLayout_moveSat.addWidget(self.pushButton_saturation)

        self.comboBox_moveSatDstMode = QComboBox(self.scrollAreaWidgetContents_11)
        self.comboBox_moveSatDstMode.addItem("")
        self.comboBox_moveSatDstMode.addItem("")
        self.comboBox_moveSatDstMode.addItem("")
        self.comboBox_moveSatDstMode.addItem("")
        self.comboBox_moveSatDstMode.addItem("")
        self.comboBox_moveSatDstMode.addItem("")
        self.comboBox_moveSatDstMode.addItem("")
        self.comboBox_moveSatDstMode.addItem("")
        self.comboBox_moveSatDstMode.setObjectName(u"comboBox_moveSatDstMode")
        self.comboBox_moveSatDstMode.setMinimumContentsLength(0)

        self.horizontalLayout_moveSat.addWidget(self.comboBox_moveSatDstMode)

        self.lineEdit_moveSatDstAddr = QLineEdit(self.scrollAreaWidgetContents_11)
        self.lineEdit_moveSatDstAddr.setObjectName(u"lineEdit_moveSatDstAddr")
        sizePolicy3.setHeightForWidth(self.lineEdit_moveSatDstAddr.sizePolicy().hasHeightForWidth())
        self.lineEdit_moveSatDstAddr.setSizePolicy(sizePolicy3)
        self.lineEdit_moveSatDstAddr.setMinimumSize(QSize(160, 0))
        self.lineEdit_moveSatDstAddr.setMaximumSize(QSize(160, 16777215))

        self.horizontalLayout_moveSat.addWidget(self.lineEdit_moveSatDstAddr)

        self.lineEdit_moveSatSrcEp = QLineEdit(self.scrollAreaWidgetContents_11)
        self.lineEdit_moveSatSrcEp.setObjectName(u"lineEdit_moveSatSrcEp")
        self.lineEdit_moveSatSrcEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_moveSat.addWidget(self.lineEdit_moveSatSrcEp)

        self.lineEdit_moveSatDstEp = QLineEdit(self.scrollAreaWidgetContents_11)
        self.lineEdit_moveSatDstEp.setObjectName(u"lineEdit_moveSatDstEp")
        self.lineEdit_moveSatDstEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_moveSat.addWidget(self.lineEdit_moveSatDstEp)

        self.lineEdit_moveSatSat = QLineEdit(self.scrollAreaWidgetContents_11)
        self.lineEdit_moveSatSat.setObjectName(u"lineEdit_moveSatSat")
        self.lineEdit_moveSatSat.setMinimumSize(QSize(100, 0))
        self.lineEdit_moveSatSat.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_moveSat.addWidget(self.lineEdit_moveSatSat)

        self.lineEdit_moveSatTime = QLineEdit(self.scrollAreaWidgetContents_11)
        self.lineEdit_moveSatTime.setObjectName(u"lineEdit_moveSatTime")
        self.lineEdit_moveSatTime.setMinimumSize(QSize(120, 0))
        self.lineEdit_moveSatTime.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_moveSat.addWidget(self.lineEdit_moveSatTime)

        self.horizontalSpacer_47 = QSpacerItem(0, 0, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_moveSat.addItem(self.horizontalSpacer_47)


        self.gridLayout_16.addLayout(self.horizontalLayout_moveSat, 2, 0, 1, 1)

        self.verticalSpacer_9 = QSpacerItem(0, 0, QSizePolicy.Minimum, QSizePolicy.Expanding)

        self.gridLayout_16.addItem(self.verticalSpacer_9, 5, 0, 1, 1)

        self.horizontalLayout_moveColor = QHBoxLayout()
        self.horizontalLayout_moveColor.setSpacing(6)
        self.horizontalLayout_moveColor.setObjectName(u"horizontalLayout_moveColor")
        self.pushButton_color = QPushButton(self.scrollAreaWidgetContents_11)
        self.pushButton_color.setObjectName(u"pushButton_color")
        self.pushButton_color.setMinimumSize(QSize(161, 0))

        self.horizontalLayout_moveColor.addWidget(self.pushButton_color)

        self.comboBox_moveColorDstMode = QComboBox(self.scrollAreaWidgetContents_11)
        self.comboBox_moveColorDstMode.addItem("")
        self.comboBox_moveColorDstMode.addItem("")
        self.comboBox_moveColorDstMode.addItem("")
        self.comboBox_moveColorDstMode.addItem("")
        self.comboBox_moveColorDstMode.addItem("")
        self.comboBox_moveColorDstMode.addItem("")
        self.comboBox_moveColorDstMode.addItem("")
        self.comboBox_moveColorDstMode.addItem("")
        self.comboBox_moveColorDstMode.setObjectName(u"comboBox_moveColorDstMode")
        self.comboBox_moveColorDstMode.setMinimumContentsLength(0)

        self.horizontalLayout_moveColor.addWidget(self.comboBox_moveColorDstMode)

        self.lineEdit_moveColorDstAddr = QLineEdit(self.scrollAreaWidgetContents_11)
        self.lineEdit_moveColorDstAddr.setObjectName(u"lineEdit_moveColorDstAddr")
        sizePolicy3.setHeightForWidth(self.lineEdit_moveColorDstAddr.sizePolicy().hasHeightForWidth())
        self.lineEdit_moveColorDstAddr.setSizePolicy(sizePolicy3)
        self.lineEdit_moveColorDstAddr.setMinimumSize(QSize(160, 0))
        self.lineEdit_moveColorDstAddr.setMaximumSize(QSize(160, 16777215))

        self.horizontalLayout_moveColor.addWidget(self.lineEdit_moveColorDstAddr)

        self.lineEdit_moveColorSrcEp = QLineEdit(self.scrollAreaWidgetContents_11)
        self.lineEdit_moveColorSrcEp.setObjectName(u"lineEdit_moveColorSrcEp")
        self.lineEdit_moveColorSrcEp.setMinimumSize(QSize(60, 0))
        self.lineEdit_moveColorSrcEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_moveColor.addWidget(self.lineEdit_moveColorSrcEp)

        self.lineEdit_moveColorDstEp = QLineEdit(self.scrollAreaWidgetContents_11)
        self.lineEdit_moveColorDstEp.setObjectName(u"lineEdit_moveColorDstEp")
        self.lineEdit_moveColorDstEp.setMinimumSize(QSize(60, 0))
        self.lineEdit_moveColorDstEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_moveColor.addWidget(self.lineEdit_moveColorDstEp)

        self.lineEdit_moveColorX = QLineEdit(self.scrollAreaWidgetContents_11)
        self.lineEdit_moveColorX.setObjectName(u"lineEdit_moveColorX")
        self.lineEdit_moveColorX.setMinimumSize(QSize(80, 0))
        self.lineEdit_moveColorX.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_moveColor.addWidget(self.lineEdit_moveColorX)

        self.lineEdit_moveColorY = QLineEdit(self.scrollAreaWidgetContents_11)
        self.lineEdit_moveColorY.setObjectName(u"lineEdit_moveColorY")
        self.lineEdit_moveColorY.setMinimumSize(QSize(80, 0))
        self.lineEdit_moveColorY.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_moveColor.addWidget(self.lineEdit_moveColorY)

        self.lineEdit_moveColorTime = QLineEdit(self.scrollAreaWidgetContents_11)
        self.lineEdit_moveColorTime.setObjectName(u"lineEdit_moveColorTime")
        self.lineEdit_moveColorTime.setMinimumSize(QSize(120, 0))
        self.lineEdit_moveColorTime.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_moveColor.addWidget(self.lineEdit_moveColorTime)

        self.horizontalSpacer_46 = QSpacerItem(0, 0, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_moveColor.addItem(self.horizontalSpacer_46)


        self.gridLayout_16.addLayout(self.horizontalLayout_moveColor, 1, 0, 1, 1)

        self.scrollArea_7.setWidget(self.scrollAreaWidgetContents_11)

        self.gridLayout_17.addWidget(self.scrollArea_7, 0, 0, 1, 1)

        self.tabWidget.addTab(self.color, "")
        self.identify = QWidget()
        self.identify.setObjectName(u"identify")
        self.gridLayout_19 = QGridLayout(self.identify)
        self.gridLayout_19.setSpacing(6)
        self.gridLayout_19.setContentsMargins(11, 11, 11, 11)
        self.gridLayout_19.setObjectName(u"gridLayout_19")
        self.scrollArea_8 = QScrollArea(self.identify)
        self.scrollArea_8.setObjectName(u"scrollArea_8")
        palette8 = QPalette()
        palette8.setBrush(QPalette.Active, QPalette.Base, brush2)
        palette8.setBrush(QPalette.Inactive, QPalette.Base, brush2)
        palette8.setBrush(QPalette.Disabled, QPalette.Base, brush3)
        self.scrollArea_8.setPalette(palette8)
        self.scrollArea_8.setWidgetResizable(True)
        self.scrollAreaWidgetContents_12 = QWidget()
        self.scrollAreaWidgetContents_12.setObjectName(u"scrollAreaWidgetContents_12")
        self.scrollAreaWidgetContents_12.setGeometry(QRect(0, 0, 970, 316))
        self.gridLayout_18 = QGridLayout(self.scrollAreaWidgetContents_12)
        self.gridLayout_18.setSpacing(6)
        self.gridLayout_18.setContentsMargins(11, 11, 11, 11)
        self.gridLayout_18.setObjectName(u"gridLayout_18")
        self.horizontalLayout_identifyQuery = QHBoxLayout()
        self.horizontalLayout_identifyQuery.setSpacing(6)
        self.horizontalLayout_identifyQuery.setObjectName(u"horizontalLayout_identifyQuery")
        self.pushButton_identifyQuery = QPushButton(self.scrollAreaWidgetContents_12)
        self.pushButton_identifyQuery.setObjectName(u"pushButton_identifyQuery")
        self.pushButton_identifyQuery.setMinimumSize(QSize(121, 0))

        self.horizontalLayout_identifyQuery.addWidget(self.pushButton_identifyQuery)

        self.comboBox_identifyQueryDstMode = QComboBox(self.scrollAreaWidgetContents_12)
        self.comboBox_identifyQueryDstMode.addItem("")
        self.comboBox_identifyQueryDstMode.addItem("")
        self.comboBox_identifyQueryDstMode.addItem("")
        self.comboBox_identifyQueryDstMode.addItem("")
        self.comboBox_identifyQueryDstMode.addItem("")
        self.comboBox_identifyQueryDstMode.addItem("")
        self.comboBox_identifyQueryDstMode.addItem("")
        self.comboBox_identifyQueryDstMode.addItem("")
        self.comboBox_identifyQueryDstMode.setObjectName(u"comboBox_identifyQueryDstMode")
        self.comboBox_identifyQueryDstMode.setMinimumContentsLength(0)

        self.horizontalLayout_identifyQuery.addWidget(self.comboBox_identifyQueryDstMode)

        self.lineEdit_identifyQueryDstAddr = QLineEdit(self.scrollAreaWidgetContents_12)
        self.lineEdit_identifyQueryDstAddr.setObjectName(u"lineEdit_identifyQueryDstAddr")
        sizePolicy3.setHeightForWidth(self.lineEdit_identifyQueryDstAddr.sizePolicy().hasHeightForWidth())
        self.lineEdit_identifyQueryDstAddr.setSizePolicy(sizePolicy3)
        self.lineEdit_identifyQueryDstAddr.setMinimumSize(QSize(160, 0))
        self.lineEdit_identifyQueryDstAddr.setMaximumSize(QSize(160, 16777215))

        self.horizontalLayout_identifyQuery.addWidget(self.lineEdit_identifyQueryDstAddr)

        self.lineEdit_identifyQuerySrcEp = QLineEdit(self.scrollAreaWidgetContents_12)
        self.lineEdit_identifyQuerySrcEp.setObjectName(u"lineEdit_identifyQuerySrcEp")
        self.lineEdit_identifyQuerySrcEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_identifyQuery.addWidget(self.lineEdit_identifyQuerySrcEp)

        self.lineEdit_identifyQueryDstEp = QLineEdit(self.scrollAreaWidgetContents_12)
        self.lineEdit_identifyQueryDstEp.setObjectName(u"lineEdit_identifyQueryDstEp")
        self.lineEdit_identifyQueryDstEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_identifyQuery.addWidget(self.lineEdit_identifyQueryDstEp)

        self.horizontalSpacer_51 = QSpacerItem(40, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_identifyQuery.addItem(self.horizontalSpacer_51)


        self.gridLayout_18.addLayout(self.horizontalLayout_identifyQuery, 1, 0, 1, 1)

        self.verticalSpacer_10 = QSpacerItem(0, 0, QSizePolicy.Minimum, QSizePolicy.Expanding)

        self.gridLayout_18.addItem(self.verticalSpacer_10, 2, 0, 1, 1)

        self.horizontalLayout_identify = QHBoxLayout()
        self.horizontalLayout_identify.setSpacing(6)
        self.horizontalLayout_identify.setObjectName(u"horizontalLayout_identify")
        self.pushButton_identify = QPushButton(self.scrollAreaWidgetContents_12)
        self.pushButton_identify.setObjectName(u"pushButton_identify")
        self.pushButton_identify.setMinimumSize(QSize(121, 0))

        self.horizontalLayout_identify.addWidget(self.pushButton_identify)

        self.comboBox_identifyDstMode = QComboBox(self.scrollAreaWidgetContents_12)
        self.comboBox_identifyDstMode.addItem("")
        self.comboBox_identifyDstMode.addItem("")
        self.comboBox_identifyDstMode.addItem("")
        self.comboBox_identifyDstMode.addItem("")
        self.comboBox_identifyDstMode.addItem("")
        self.comboBox_identifyDstMode.addItem("")
        self.comboBox_identifyDstMode.addItem("")
        self.comboBox_identifyDstMode.addItem("")
        self.comboBox_identifyDstMode.setObjectName(u"comboBox_identifyDstMode")
        self.comboBox_identifyDstMode.setMinimumContentsLength(0)

        self.horizontalLayout_identify.addWidget(self.comboBox_identifyDstMode)

        self.lineEdit_identifyDstAddr = QLineEdit(self.scrollAreaWidgetContents_12)
        self.lineEdit_identifyDstAddr.setObjectName(u"lineEdit_identifyDstAddr")
        sizePolicy3.setHeightForWidth(self.lineEdit_identifyDstAddr.sizePolicy().hasHeightForWidth())
        self.lineEdit_identifyDstAddr.setSizePolicy(sizePolicy3)
        self.lineEdit_identifyDstAddr.setMinimumSize(QSize(160, 0))
        self.lineEdit_identifyDstAddr.setMaximumSize(QSize(160, 16777215))

        self.horizontalLayout_identify.addWidget(self.lineEdit_identifyDstAddr)

        self.lineEdit_identifySrcEp = QLineEdit(self.scrollAreaWidgetContents_12)
        self.lineEdit_identifySrcEp.setObjectName(u"lineEdit_identifySrcEp")
        self.lineEdit_identifySrcEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_identify.addWidget(self.lineEdit_identifySrcEp)

        self.lineEdit_identifyDstEp = QLineEdit(self.scrollAreaWidgetContents_12)
        self.lineEdit_identifyDstEp.setObjectName(u"lineEdit_identifyDstEp")
        self.lineEdit_identifyDstEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_identify.addWidget(self.lineEdit_identifyDstEp)

        self.lineEdit_identifyTime = QLineEdit(self.scrollAreaWidgetContents_12)
        self.lineEdit_identifyTime.setObjectName(u"lineEdit_identifyTime")
        self.lineEdit_identifyTime.setMinimumSize(QSize(160, 0))
        self.lineEdit_identifyTime.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_identify.addWidget(self.lineEdit_identifyTime)

        self.horizontalSpacer_48 = QSpacerItem(0, 0, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_identify.addItem(self.horizontalSpacer_48)


        self.gridLayout_18.addLayout(self.horizontalLayout_identify, 0, 0, 1, 1)

        self.scrollArea_8.setWidget(self.scrollAreaWidgetContents_12)

        self.gridLayout_19.addWidget(self.scrollArea_8, 0, 0, 1, 1)

        self.tabWidget.addTab(self.identify, "")
        self.scene = QWidget()
        self.scene.setObjectName(u"scene")
        self.gridLayout_21 = QGridLayout(self.scene)
        self.gridLayout_21.setSpacing(6)
        self.gridLayout_21.setContentsMargins(11, 11, 11, 11)
        self.gridLayout_21.setObjectName(u"gridLayout_21")
        self.scrollArea_9 = QScrollArea(self.scene)
        self.scrollArea_9.setObjectName(u"scrollArea_9")
        palette9 = QPalette()
        palette9.setBrush(QPalette.Active, QPalette.Base, brush2)
        palette9.setBrush(QPalette.Inactive, QPalette.Base, brush2)
        palette9.setBrush(QPalette.Disabled, QPalette.Base, brush3)
        self.scrollArea_9.setPalette(palette9)
        self.scrollArea_9.setWidgetResizable(True)
        self.scrollAreaWidgetContents_13 = QWidget()
        self.scrollAreaWidgetContents_13.setObjectName(u"scrollAreaWidgetContents_13")
        self.scrollAreaWidgetContents_13.setGeometry(QRect(0, 0, 970, 316))
        self.gridLayout_20 = QGridLayout(self.scrollAreaWidgetContents_13)
        self.gridLayout_20.setSpacing(6)
        self.gridLayout_20.setContentsMargins(11, 11, 11, 11)
        self.gridLayout_20.setObjectName(u"gridLayout_20")
        self.formLayout_addScene = QFormLayout()
        self.formLayout_addScene.setSpacing(6)
        self.formLayout_addScene.setObjectName(u"formLayout_addScene")
        self.pushButton_addScene = QPushButton(self.scrollAreaWidgetContents_13)
        self.pushButton_addScene.setObjectName(u"pushButton_addScene")

        self.formLayout_addScene.setWidget(0, QFormLayout.LabelRole, self.pushButton_addScene)

        self.horizontalLayout_addScene1 = QHBoxLayout()
        self.horizontalLayout_addScene1.setSpacing(6)
        self.horizontalLayout_addScene1.setObjectName(u"horizontalLayout_addScene1")
        self.comboBox_addSceneDstMode = QComboBox(self.scrollAreaWidgetContents_13)
        self.comboBox_addSceneDstMode.addItem("")
        self.comboBox_addSceneDstMode.addItem("")
        self.comboBox_addSceneDstMode.addItem("")
        self.comboBox_addSceneDstMode.addItem("")
        self.comboBox_addSceneDstMode.addItem("")
        self.comboBox_addSceneDstMode.addItem("")
        self.comboBox_addSceneDstMode.addItem("")
        self.comboBox_addSceneDstMode.addItem("")
        self.comboBox_addSceneDstMode.setObjectName(u"comboBox_addSceneDstMode")
        self.comboBox_addSceneDstMode.setMaxVisibleItems(10)
        self.comboBox_addSceneDstMode.setMinimumContentsLength(0)

        self.horizontalLayout_addScene1.addWidget(self.comboBox_addSceneDstMode)

        self.lineEdit_addSceneDstAddr = QLineEdit(self.scrollAreaWidgetContents_13)
        self.lineEdit_addSceneDstAddr.setObjectName(u"lineEdit_addSceneDstAddr")
        sizePolicy3.setHeightForWidth(self.lineEdit_addSceneDstAddr.sizePolicy().hasHeightForWidth())
        self.lineEdit_addSceneDstAddr.setSizePolicy(sizePolicy3)
        self.lineEdit_addSceneDstAddr.setMinimumSize(QSize(160, 0))
        self.lineEdit_addSceneDstAddr.setMaximumSize(QSize(160, 16777215))

        self.horizontalLayout_addScene1.addWidget(self.lineEdit_addSceneDstAddr)

        self.lineEdit_addSceneSrcEp = QLineEdit(self.scrollAreaWidgetContents_13)
        self.lineEdit_addSceneSrcEp.setObjectName(u"lineEdit_addSceneSrcEp")
        self.lineEdit_addSceneSrcEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_addScene1.addWidget(self.lineEdit_addSceneSrcEp)

        self.lineEdit_addSceneDstEp = QLineEdit(self.scrollAreaWidgetContents_13)
        self.lineEdit_addSceneDstEp.setObjectName(u"lineEdit_addSceneDstEp")
        self.lineEdit_addSceneDstEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_addScene1.addWidget(self.lineEdit_addSceneDstEp)

        self.lineEdit_addSceneGroupId = QLineEdit(self.scrollAreaWidgetContents_13)
        self.lineEdit_addSceneGroupId.setObjectName(u"lineEdit_addSceneGroupId")
        self.lineEdit_addSceneGroupId.setMinimumSize(QSize(80, 0))
        self.lineEdit_addSceneGroupId.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_addScene1.addWidget(self.lineEdit_addSceneGroupId)

        self.lineEdit_addSceneSceneId = QLineEdit(self.scrollAreaWidgetContents_13)
        self.lineEdit_addSceneSceneId.setObjectName(u"lineEdit_addSceneSceneId")
        self.lineEdit_addSceneSceneId.setMinimumSize(QSize(80, 0))
        self.lineEdit_addSceneSceneId.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_addScene1.addWidget(self.lineEdit_addSceneSceneId)

        self.lineEdit_addSceneTime = QLineEdit(self.scrollAreaWidgetContents_13)
        self.lineEdit_addSceneTime.setObjectName(u"lineEdit_addSceneTime")
        self.lineEdit_addSceneTime.setMinimumSize(QSize(120, 0))
        self.lineEdit_addSceneTime.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_addScene1.addWidget(self.lineEdit_addSceneTime)

        self.horizontalSpacer_55 = QSpacerItem(0, 0, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_addScene1.addItem(self.horizontalSpacer_55)


        self.formLayout_addScene.setLayout(0, QFormLayout.FieldRole, self.horizontalLayout_addScene1)

        self.horizontalLayout_addScene2 = QHBoxLayout()
        self.horizontalLayout_addScene2.setSpacing(6)
        self.horizontalLayout_addScene2.setObjectName(u"horizontalLayout_addScene2")
        self.horizontalLayout_addScene2.setSizeConstraint(QLayout.SetDefaultConstraint)
        self.lineEdit_addSceneNameLen = QLineEdit(self.scrollAreaWidgetContents_13)
        self.lineEdit_addSceneNameLen.setObjectName(u"lineEdit_addSceneNameLen")
        self.lineEdit_addSceneNameLen.setMinimumSize(QSize(120, 0))
        self.lineEdit_addSceneNameLen.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_addScene2.addWidget(self.lineEdit_addSceneNameLen)

        self.lineEdit_addSceneName = QLineEdit(self.scrollAreaWidgetContents_13)
        self.lineEdit_addSceneName.setObjectName(u"lineEdit_addSceneName")
        self.lineEdit_addSceneName.setMinimumSize(QSize(160, 0))
        self.lineEdit_addSceneName.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_addScene2.addWidget(self.lineEdit_addSceneName)

        self.lineEdit_addSceneExtLen = QLineEdit(self.scrollAreaWidgetContents_13)
        self.lineEdit_addSceneExtLen.setObjectName(u"lineEdit_addSceneExtLen")
        self.lineEdit_addSceneExtLen.setMinimumSize(QSize(120, 0))
        self.lineEdit_addSceneExtLen.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_addScene2.addWidget(self.lineEdit_addSceneExtLen)

        self.lineEdit_addSceneExtField = QLineEdit(self.scrollAreaWidgetContents_13)
        self.lineEdit_addSceneExtField.setObjectName(u"lineEdit_addSceneExtField")
        sizePolicy3.setHeightForWidth(self.lineEdit_addSceneExtField.sizePolicy().hasHeightForWidth())
        self.lineEdit_addSceneExtField.setSizePolicy(sizePolicy3)
        self.lineEdit_addSceneExtField.setMinimumSize(QSize(400, 0))

        self.horizontalLayout_addScene2.addWidget(self.lineEdit_addSceneExtField)

        self.horizontalSpacer_9 = QSpacerItem(0, 0, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_addScene2.addItem(self.horizontalSpacer_9)


        self.formLayout_addScene.setLayout(1, QFormLayout.FieldRole, self.horizontalLayout_addScene2)


        self.gridLayout_20.addLayout(self.formLayout_addScene, 0, 0, 1, 1)

        self.horizontalLayout_storeScene = QHBoxLayout()
        self.horizontalLayout_storeScene.setSpacing(6)
        self.horizontalLayout_storeScene.setObjectName(u"horizontalLayout_storeScene")
        self.pushButton_storeScene = QPushButton(self.scrollAreaWidgetContents_13)
        self.pushButton_storeScene.setObjectName(u"pushButton_storeScene")

        self.horizontalLayout_storeScene.addWidget(self.pushButton_storeScene)

        self.comboBox_storeSceneDstMode = QComboBox(self.scrollAreaWidgetContents_13)
        self.comboBox_storeSceneDstMode.addItem("")
        self.comboBox_storeSceneDstMode.addItem("")
        self.comboBox_storeSceneDstMode.addItem("")
        self.comboBox_storeSceneDstMode.addItem("")
        self.comboBox_storeSceneDstMode.addItem("")
        self.comboBox_storeSceneDstMode.addItem("")
        self.comboBox_storeSceneDstMode.addItem("")
        self.comboBox_storeSceneDstMode.addItem("")
        self.comboBox_storeSceneDstMode.setObjectName(u"comboBox_storeSceneDstMode")
        self.comboBox_storeSceneDstMode.setMaxVisibleItems(10)
        self.comboBox_storeSceneDstMode.setMinimumContentsLength(0)

        self.horizontalLayout_storeScene.addWidget(self.comboBox_storeSceneDstMode)

        self.lineEdit_storeSceneDstAddr = QLineEdit(self.scrollAreaWidgetContents_13)
        self.lineEdit_storeSceneDstAddr.setObjectName(u"lineEdit_storeSceneDstAddr")
        sizePolicy3.setHeightForWidth(self.lineEdit_storeSceneDstAddr.sizePolicy().hasHeightForWidth())
        self.lineEdit_storeSceneDstAddr.setSizePolicy(sizePolicy3)
        self.lineEdit_storeSceneDstAddr.setMinimumSize(QSize(160, 0))
        self.lineEdit_storeSceneDstAddr.setMaximumSize(QSize(160, 16777215))

        self.horizontalLayout_storeScene.addWidget(self.lineEdit_storeSceneDstAddr)

        self.lineEdit_storeSceneSrcEp = QLineEdit(self.scrollAreaWidgetContents_13)
        self.lineEdit_storeSceneSrcEp.setObjectName(u"lineEdit_storeSceneSrcEp")
        self.lineEdit_storeSceneSrcEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_storeScene.addWidget(self.lineEdit_storeSceneSrcEp)

        self.lineEdit_storeSceneDstEp = QLineEdit(self.scrollAreaWidgetContents_13)
        self.lineEdit_storeSceneDstEp.setObjectName(u"lineEdit_storeSceneDstEp")
        self.lineEdit_storeSceneDstEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_storeScene.addWidget(self.lineEdit_storeSceneDstEp)

        self.lineEdit_storeSceneGroupId = QLineEdit(self.scrollAreaWidgetContents_13)
        self.lineEdit_storeSceneGroupId.setObjectName(u"lineEdit_storeSceneGroupId")
        self.lineEdit_storeSceneGroupId.setMinimumSize(QSize(80, 0))
        self.lineEdit_storeSceneGroupId.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_storeScene.addWidget(self.lineEdit_storeSceneGroupId)

        self.lineEdit_storeSceneId = QLineEdit(self.scrollAreaWidgetContents_13)
        self.lineEdit_storeSceneId.setObjectName(u"lineEdit_storeSceneId")
        self.lineEdit_storeSceneId.setMinimumSize(QSize(80, 0))
        self.lineEdit_storeSceneId.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_storeScene.addWidget(self.lineEdit_storeSceneId)

        self.horizontalSpacer_59 = QSpacerItem(0, 0, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_storeScene.addItem(self.horizontalSpacer_59)


        self.gridLayout_20.addLayout(self.horizontalLayout_storeScene, 4, 0, 1, 1)

        self.horizontalLayout_viewScene = QHBoxLayout()
        self.horizontalLayout_viewScene.setSpacing(6)
        self.horizontalLayout_viewScene.setObjectName(u"horizontalLayout_viewScene")
        self.pushButton_viewScene = QPushButton(self.scrollAreaWidgetContents_13)
        self.pushButton_viewScene.setObjectName(u"pushButton_viewScene")

        self.horizontalLayout_viewScene.addWidget(self.pushButton_viewScene)

        self.comboBox_viewSceneDstMode = QComboBox(self.scrollAreaWidgetContents_13)
        self.comboBox_viewSceneDstMode.addItem("")
        self.comboBox_viewSceneDstMode.addItem("")
        self.comboBox_viewSceneDstMode.addItem("")
        self.comboBox_viewSceneDstMode.addItem("")
        self.comboBox_viewSceneDstMode.addItem("")
        self.comboBox_viewSceneDstMode.addItem("")
        self.comboBox_viewSceneDstMode.addItem("")
        self.comboBox_viewSceneDstMode.addItem("")
        self.comboBox_viewSceneDstMode.setObjectName(u"comboBox_viewSceneDstMode")
        self.comboBox_viewSceneDstMode.setMinimumContentsLength(0)

        self.horizontalLayout_viewScene.addWidget(self.comboBox_viewSceneDstMode)

        self.lineEdit_viewSceneDstAddr = QLineEdit(self.scrollAreaWidgetContents_13)
        self.lineEdit_viewSceneDstAddr.setObjectName(u"lineEdit_viewSceneDstAddr")
        sizePolicy3.setHeightForWidth(self.lineEdit_viewSceneDstAddr.sizePolicy().hasHeightForWidth())
        self.lineEdit_viewSceneDstAddr.setSizePolicy(sizePolicy3)
        self.lineEdit_viewSceneDstAddr.setMinimumSize(QSize(160, 0))
        self.lineEdit_viewSceneDstAddr.setMaximumSize(QSize(160, 16777215))

        self.horizontalLayout_viewScene.addWidget(self.lineEdit_viewSceneDstAddr)

        self.lineEdit_viewSceneSrcEp = QLineEdit(self.scrollAreaWidgetContents_13)
        self.lineEdit_viewSceneSrcEp.setObjectName(u"lineEdit_viewSceneSrcEp")
        self.lineEdit_viewSceneSrcEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_viewScene.addWidget(self.lineEdit_viewSceneSrcEp)

        self.lineEdit_viewSceneDstEp = QLineEdit(self.scrollAreaWidgetContents_13)
        self.lineEdit_viewSceneDstEp.setObjectName(u"lineEdit_viewSceneDstEp")
        self.lineEdit_viewSceneDstEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_viewScene.addWidget(self.lineEdit_viewSceneDstEp)

        self.lineEdit_viewSceneGroupId = QLineEdit(self.scrollAreaWidgetContents_13)
        self.lineEdit_viewSceneGroupId.setObjectName(u"lineEdit_viewSceneGroupId")
        self.lineEdit_viewSceneGroupId.setMinimumSize(QSize(80, 0))
        self.lineEdit_viewSceneGroupId.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_viewScene.addWidget(self.lineEdit_viewSceneGroupId)

        self.lineEdit_viewSceneId = QLineEdit(self.scrollAreaWidgetContents_13)
        self.lineEdit_viewSceneId.setObjectName(u"lineEdit_viewSceneId")
        self.lineEdit_viewSceneId.setMinimumSize(QSize(80, 0))
        self.lineEdit_viewSceneId.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_viewScene.addWidget(self.lineEdit_viewSceneId)

        self.horizontalSpacer_58 = QSpacerItem(0, 0, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_viewScene.addItem(self.horizontalSpacer_58)


        self.gridLayout_20.addLayout(self.horizontalLayout_viewScene, 1, 0, 1, 1)

        self.horizontalLayout_recallScene = QHBoxLayout()
        self.horizontalLayout_recallScene.setSpacing(6)
        self.horizontalLayout_recallScene.setObjectName(u"horizontalLayout_recallScene")
        self.pushButton_recallScene = QPushButton(self.scrollAreaWidgetContents_13)
        self.pushButton_recallScene.setObjectName(u"pushButton_recallScene")

        self.horizontalLayout_recallScene.addWidget(self.pushButton_recallScene)

        self.comboBox_recallSceneDstMode = QComboBox(self.scrollAreaWidgetContents_13)
        self.comboBox_recallSceneDstMode.addItem("")
        self.comboBox_recallSceneDstMode.addItem("")
        self.comboBox_recallSceneDstMode.addItem("")
        self.comboBox_recallSceneDstMode.addItem("")
        self.comboBox_recallSceneDstMode.addItem("")
        self.comboBox_recallSceneDstMode.addItem("")
        self.comboBox_recallSceneDstMode.addItem("")
        self.comboBox_recallSceneDstMode.addItem("")
        self.comboBox_recallSceneDstMode.setObjectName(u"comboBox_recallSceneDstMode")
        self.comboBox_recallSceneDstMode.setMinimumContentsLength(0)

        self.horizontalLayout_recallScene.addWidget(self.comboBox_recallSceneDstMode)

        self.lineEdit_recallSceneDstAddr = QLineEdit(self.scrollAreaWidgetContents_13)
        self.lineEdit_recallSceneDstAddr.setObjectName(u"lineEdit_recallSceneDstAddr")
        sizePolicy3.setHeightForWidth(self.lineEdit_recallSceneDstAddr.sizePolicy().hasHeightForWidth())
        self.lineEdit_recallSceneDstAddr.setSizePolicy(sizePolicy3)
        self.lineEdit_recallSceneDstAddr.setMinimumSize(QSize(160, 0))
        self.lineEdit_recallSceneDstAddr.setMaximumSize(QSize(160, 16777215))

        self.horizontalLayout_recallScene.addWidget(self.lineEdit_recallSceneDstAddr)

        self.lineEdit_recallSceneSrcEp = QLineEdit(self.scrollAreaWidgetContents_13)
        self.lineEdit_recallSceneSrcEp.setObjectName(u"lineEdit_recallSceneSrcEp")
        self.lineEdit_recallSceneSrcEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_recallScene.addWidget(self.lineEdit_recallSceneSrcEp)

        self.lineEdit_recallSceneDstEp = QLineEdit(self.scrollAreaWidgetContents_13)
        self.lineEdit_recallSceneDstEp.setObjectName(u"lineEdit_recallSceneDstEp")
        self.lineEdit_recallSceneDstEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_recallScene.addWidget(self.lineEdit_recallSceneDstEp)

        self.lineEdit_recallSceneGroupId = QLineEdit(self.scrollAreaWidgetContents_13)
        self.lineEdit_recallSceneGroupId.setObjectName(u"lineEdit_recallSceneGroupId")
        self.lineEdit_recallSceneGroupId.setMinimumSize(QSize(80, 0))
        self.lineEdit_recallSceneGroupId.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_recallScene.addWidget(self.lineEdit_recallSceneGroupId)

        self.lineEdit_recallSceneId = QLineEdit(self.scrollAreaWidgetContents_13)
        self.lineEdit_recallSceneId.setObjectName(u"lineEdit_recallSceneId")
        self.lineEdit_recallSceneId.setMinimumSize(QSize(80, 0))
        self.lineEdit_recallSceneId.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_recallScene.addWidget(self.lineEdit_recallSceneId)

        self.horizontalSpacer_63 = QSpacerItem(0, 0, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_recallScene.addItem(self.horizontalSpacer_63)


        self.gridLayout_20.addLayout(self.horizontalLayout_recallScene, 5, 0, 1, 1)

        self.horizontalLayout_sceneRemoveAll = QHBoxLayout()
        self.horizontalLayout_sceneRemoveAll.setSpacing(6)
        self.horizontalLayout_sceneRemoveAll.setObjectName(u"horizontalLayout_sceneRemoveAll")
        self.pushButton_sceneRemoveAll = QPushButton(self.scrollAreaWidgetContents_13)
        self.pushButton_sceneRemoveAll.setObjectName(u"pushButton_sceneRemoveAll")

        self.horizontalLayout_sceneRemoveAll.addWidget(self.pushButton_sceneRemoveAll)

        self.comboBox_sceneRemoveAllDstMode = QComboBox(self.scrollAreaWidgetContents_13)
        self.comboBox_sceneRemoveAllDstMode.addItem("")
        self.comboBox_sceneRemoveAllDstMode.addItem("")
        self.comboBox_sceneRemoveAllDstMode.addItem("")
        self.comboBox_sceneRemoveAllDstMode.addItem("")
        self.comboBox_sceneRemoveAllDstMode.addItem("")
        self.comboBox_sceneRemoveAllDstMode.addItem("")
        self.comboBox_sceneRemoveAllDstMode.addItem("")
        self.comboBox_sceneRemoveAllDstMode.addItem("")
        self.comboBox_sceneRemoveAllDstMode.setObjectName(u"comboBox_sceneRemoveAllDstMode")
        self.comboBox_sceneRemoveAllDstMode.setMinimumContentsLength(0)

        self.horizontalLayout_sceneRemoveAll.addWidget(self.comboBox_sceneRemoveAllDstMode)

        self.lineEdit_sceneRemoveAllDstAddr = QLineEdit(self.scrollAreaWidgetContents_13)
        self.lineEdit_sceneRemoveAllDstAddr.setObjectName(u"lineEdit_sceneRemoveAllDstAddr")
        sizePolicy3.setHeightForWidth(self.lineEdit_sceneRemoveAllDstAddr.sizePolicy().hasHeightForWidth())
        self.lineEdit_sceneRemoveAllDstAddr.setSizePolicy(sizePolicy3)
        self.lineEdit_sceneRemoveAllDstAddr.setMinimumSize(QSize(160, 0))
        self.lineEdit_sceneRemoveAllDstAddr.setMaximumSize(QSize(160, 16777215))

        self.horizontalLayout_sceneRemoveAll.addWidget(self.lineEdit_sceneRemoveAllDstAddr)

        self.lineEdit_sceneRemoveAllSrcEp = QLineEdit(self.scrollAreaWidgetContents_13)
        self.lineEdit_sceneRemoveAllSrcEp.setObjectName(u"lineEdit_sceneRemoveAllSrcEp")
        self.lineEdit_sceneRemoveAllSrcEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_sceneRemoveAll.addWidget(self.lineEdit_sceneRemoveAllSrcEp)

        self.lineEdit_sceneRemoveAllDstEp = QLineEdit(self.scrollAreaWidgetContents_13)
        self.lineEdit_sceneRemoveAllDstEp.setObjectName(u"lineEdit_sceneRemoveAllDstEp")
        self.lineEdit_sceneRemoveAllDstEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_sceneRemoveAll.addWidget(self.lineEdit_sceneRemoveAllDstEp)

        self.lineEdit_sceneRemoveAllGroupId = QLineEdit(self.scrollAreaWidgetContents_13)
        self.lineEdit_sceneRemoveAllGroupId.setObjectName(u"lineEdit_sceneRemoveAllGroupId")
        self.lineEdit_sceneRemoveAllGroupId.setMinimumSize(QSize(80, 0))
        self.lineEdit_sceneRemoveAllGroupId.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_sceneRemoveAll.addWidget(self.lineEdit_sceneRemoveAllGroupId)

        self.horizontalSpacer_60 = QSpacerItem(0, 0, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_sceneRemoveAll.addItem(self.horizontalSpacer_60)


        self.gridLayout_20.addLayout(self.horizontalLayout_sceneRemoveAll, 3, 0, 1, 1)

        self.verticalSpacer_11 = QSpacerItem(20, 40, QSizePolicy.Minimum, QSizePolicy.Expanding)

        self.gridLayout_20.addItem(self.verticalSpacer_11, 7, 0, 1, 1)

        self.horizontalLayout_removeScene = QHBoxLayout()
        self.horizontalLayout_removeScene.setSpacing(6)
        self.horizontalLayout_removeScene.setObjectName(u"horizontalLayout_removeScene")
        self.pushButton_removeScene = QPushButton(self.scrollAreaWidgetContents_13)
        self.pushButton_removeScene.setObjectName(u"pushButton_removeScene")

        self.horizontalLayout_removeScene.addWidget(self.pushButton_removeScene)

        self.comboBox_removeSceneDstMode = QComboBox(self.scrollAreaWidgetContents_13)
        self.comboBox_removeSceneDstMode.addItem("")
        self.comboBox_removeSceneDstMode.addItem("")
        self.comboBox_removeSceneDstMode.addItem("")
        self.comboBox_removeSceneDstMode.addItem("")
        self.comboBox_removeSceneDstMode.addItem("")
        self.comboBox_removeSceneDstMode.addItem("")
        self.comboBox_removeSceneDstMode.addItem("")
        self.comboBox_removeSceneDstMode.addItem("")
        self.comboBox_removeSceneDstMode.setObjectName(u"comboBox_removeSceneDstMode")
        self.comboBox_removeSceneDstMode.setMinimumContentsLength(0)

        self.horizontalLayout_removeScene.addWidget(self.comboBox_removeSceneDstMode)

        self.lineEdit_removeSceneDstAddr = QLineEdit(self.scrollAreaWidgetContents_13)
        self.lineEdit_removeSceneDstAddr.setObjectName(u"lineEdit_removeSceneDstAddr")
        sizePolicy3.setHeightForWidth(self.lineEdit_removeSceneDstAddr.sizePolicy().hasHeightForWidth())
        self.lineEdit_removeSceneDstAddr.setSizePolicy(sizePolicy3)
        self.lineEdit_removeSceneDstAddr.setMinimumSize(QSize(160, 0))
        self.lineEdit_removeSceneDstAddr.setMaximumSize(QSize(160, 16777215))

        self.horizontalLayout_removeScene.addWidget(self.lineEdit_removeSceneDstAddr)

        self.lineEdit_removeSceneSrcEp = QLineEdit(self.scrollAreaWidgetContents_13)
        self.lineEdit_removeSceneSrcEp.setObjectName(u"lineEdit_removeSceneSrcEp")
        self.lineEdit_removeSceneSrcEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_removeScene.addWidget(self.lineEdit_removeSceneSrcEp)

        self.lineEdit_removeSceneDstEp = QLineEdit(self.scrollAreaWidgetContents_13)
        self.lineEdit_removeSceneDstEp.setObjectName(u"lineEdit_removeSceneDstEp")
        self.lineEdit_removeSceneDstEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_removeScene.addWidget(self.lineEdit_removeSceneDstEp)

        self.lineEdit_removeSceneGroupId = QLineEdit(self.scrollAreaWidgetContents_13)
        self.lineEdit_removeSceneGroupId.setObjectName(u"lineEdit_removeSceneGroupId")
        self.lineEdit_removeSceneGroupId.setMinimumSize(QSize(80, 0))
        self.lineEdit_removeSceneGroupId.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_removeScene.addWidget(self.lineEdit_removeSceneGroupId)

        self.lineEdit_removeSceneId = QLineEdit(self.scrollAreaWidgetContents_13)
        self.lineEdit_removeSceneId.setObjectName(u"lineEdit_removeSceneId")
        self.lineEdit_removeSceneId.setMinimumSize(QSize(80, 0))
        self.lineEdit_removeSceneId.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_removeScene.addWidget(self.lineEdit_removeSceneId)

        self.horizontalSpacer_61 = QSpacerItem(0, 0, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_removeScene.addItem(self.horizontalSpacer_61)


        self.gridLayout_20.addLayout(self.horizontalLayout_removeScene, 2, 0, 1, 1)

        self.horizontalLayout_getScene = QHBoxLayout()
        self.horizontalLayout_getScene.setSpacing(6)
        self.horizontalLayout_getScene.setObjectName(u"horizontalLayout_getScene")
        self.pushButton_sceneGetMembership = QPushButton(self.scrollAreaWidgetContents_13)
        self.pushButton_sceneGetMembership.setObjectName(u"pushButton_sceneGetMembership")

        self.horizontalLayout_getScene.addWidget(self.pushButton_sceneGetMembership)

        self.comboBox_getSceneDstMode = QComboBox(self.scrollAreaWidgetContents_13)
        self.comboBox_getSceneDstMode.addItem("")
        self.comboBox_getSceneDstMode.addItem("")
        self.comboBox_getSceneDstMode.addItem("")
        self.comboBox_getSceneDstMode.addItem("")
        self.comboBox_getSceneDstMode.addItem("")
        self.comboBox_getSceneDstMode.addItem("")
        self.comboBox_getSceneDstMode.addItem("")
        self.comboBox_getSceneDstMode.addItem("")
        self.comboBox_getSceneDstMode.setObjectName(u"comboBox_getSceneDstMode")
        self.comboBox_getSceneDstMode.setMinimumContentsLength(0)

        self.horizontalLayout_getScene.addWidget(self.comboBox_getSceneDstMode)

        self.lineEdit_getSceneDstAddr = QLineEdit(self.scrollAreaWidgetContents_13)
        self.lineEdit_getSceneDstAddr.setObjectName(u"lineEdit_getSceneDstAddr")
        sizePolicy3.setHeightForWidth(self.lineEdit_getSceneDstAddr.sizePolicy().hasHeightForWidth())
        self.lineEdit_getSceneDstAddr.setSizePolicy(sizePolicy3)
        self.lineEdit_getSceneDstAddr.setMinimumSize(QSize(160, 0))
        self.lineEdit_getSceneDstAddr.setMaximumSize(QSize(160, 16777215))

        self.horizontalLayout_getScene.addWidget(self.lineEdit_getSceneDstAddr)

        self.lineEdit_getSceneSrcEp = QLineEdit(self.scrollAreaWidgetContents_13)
        self.lineEdit_getSceneSrcEp.setObjectName(u"lineEdit_getSceneSrcEp")
        self.lineEdit_getSceneSrcEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_getScene.addWidget(self.lineEdit_getSceneSrcEp)

        self.lineEdit_getSceneDstEp = QLineEdit(self.scrollAreaWidgetContents_13)
        self.lineEdit_getSceneDstEp.setObjectName(u"lineEdit_getSceneDstEp")
        self.lineEdit_getSceneDstEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_getScene.addWidget(self.lineEdit_getSceneDstEp)

        self.lineEdit_getSceneGroupId = QLineEdit(self.scrollAreaWidgetContents_13)
        self.lineEdit_getSceneGroupId.setObjectName(u"lineEdit_getSceneGroupId")
        self.lineEdit_getSceneGroupId.setMinimumSize(QSize(80, 0))
        self.lineEdit_getSceneGroupId.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_getScene.addWidget(self.lineEdit_getSceneGroupId)

        self.horizontalSpacer_62 = QSpacerItem(0, 0, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_getScene.addItem(self.horizontalSpacer_62)


        self.gridLayout_20.addLayout(self.horizontalLayout_getScene, 6, 0, 1, 1)

        self.scrollArea_9.setWidget(self.scrollAreaWidgetContents_13)

        self.gridLayout_21.addWidget(self.scrollArea_9, 0, 0, 1, 1)

        self.tabWidget.addTab(self.scene, "")
        self.ota = QWidget()
        self.ota.setObjectName(u"ota")
        self.gridLayout_22 = QGridLayout(self.ota)
        self.gridLayout_22.setSpacing(6)
        self.gridLayout_22.setContentsMargins(11, 11, 11, 11)
        self.gridLayout_22.setObjectName(u"gridLayout_22")
        self.verticalLayout_4 = QVBoxLayout()
        self.verticalLayout_4.setSpacing(6)
        self.verticalLayout_4.setObjectName(u"verticalLayout_4")
        self.horizontalLayout_ota_imageNotify = QHBoxLayout()
        self.horizontalLayout_ota_imageNotify.setSpacing(6)
        self.horizontalLayout_ota_imageNotify.setObjectName(u"horizontalLayout_ota_imageNotify")
        self.pushButton_imageNotify = QPushButton(self.ota)
        self.pushButton_imageNotify.setObjectName(u"pushButton_imageNotify")

        self.horizontalLayout_ota_imageNotify.addWidget(self.pushButton_imageNotify)

        self.comboBox_otaDstMode = QComboBox(self.ota)
        self.comboBox_otaDstMode.addItem("")
        self.comboBox_otaDstMode.addItem("")
        self.comboBox_otaDstMode.addItem("")
        self.comboBox_otaDstMode.addItem("")
        self.comboBox_otaDstMode.addItem("")
        self.comboBox_otaDstMode.addItem("")
        self.comboBox_otaDstMode.addItem("")
        self.comboBox_otaDstMode.addItem("")
        self.comboBox_otaDstMode.setObjectName(u"comboBox_otaDstMode")
        self.comboBox_otaDstMode.setMinimumContentsLength(0)

        self.horizontalLayout_ota_imageNotify.addWidget(self.comboBox_otaDstMode)

        self.lineEdit_otaDstAddr = QLineEdit(self.ota)
        self.lineEdit_otaDstAddr.setObjectName(u"lineEdit_otaDstAddr")
        sizePolicy3.setHeightForWidth(self.lineEdit_otaDstAddr.sizePolicy().hasHeightForWidth())
        self.lineEdit_otaDstAddr.setSizePolicy(sizePolicy3)
        self.lineEdit_otaDstAddr.setMinimumSize(QSize(160, 0))
        self.lineEdit_otaDstAddr.setMaximumSize(QSize(160, 16777215))

        self.horizontalLayout_ota_imageNotify.addWidget(self.lineEdit_otaDstAddr)

        self.lineEdit_otaSrcEp = QLineEdit(self.ota)
        self.lineEdit_otaSrcEp.setObjectName(u"lineEdit_otaSrcEp")
        self.lineEdit_otaSrcEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_ota_imageNotify.addWidget(self.lineEdit_otaSrcEp)

        self.lineEdit_otaDstEp = QLineEdit(self.ota)
        self.lineEdit_otaDstEp.setObjectName(u"lineEdit_otaDstEp")
        self.lineEdit_otaDstEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_ota_imageNotify.addWidget(self.lineEdit_otaDstEp)

        self.comboBox_payloadType = QComboBox(self.ota)
        self.comboBox_payloadType.addItem("")
        self.comboBox_payloadType.addItem("")
        self.comboBox_payloadType.addItem("")
        self.comboBox_payloadType.addItem("")
        self.comboBox_payloadType.setObjectName(u"comboBox_payloadType")
        self.comboBox_payloadType.setSizeAdjustPolicy(QComboBox.AdjustToContents)

        self.horizontalLayout_ota_imageNotify.addWidget(self.comboBox_payloadType)

        self.lineEdit_jitter = QLineEdit(self.ota)
        self.lineEdit_jitter.setObjectName(u"lineEdit_jitter")
        sizePolicy3.setHeightForWidth(self.lineEdit_jitter.sizePolicy().hasHeightForWidth())
        self.lineEdit_jitter.setSizePolicy(sizePolicy3)
        self.lineEdit_jitter.setMinimumSize(QSize(160, 0))
        self.lineEdit_jitter.setMaximumSize(QSize(160, 16777215))

        self.horizontalLayout_ota_imageNotify.addWidget(self.lineEdit_jitter)

        self.horizontalSpacer_39 = QSpacerItem(0, 0, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_ota_imageNotify.addItem(self.horizontalSpacer_39)


        self.verticalLayout_4.addLayout(self.horizontalLayout_ota_imageNotify)

        self.verticalSpacer_14 = QSpacerItem(0, 0, QSizePolicy.Minimum, QSizePolicy.Expanding)

        self.verticalLayout_4.addItem(self.verticalSpacer_14)


        self.gridLayout_22.addLayout(self.verticalLayout_4, 0, 0, 1, 1)

        self.tabWidget.addTab(self.ota, "")
        self.zbd = QWidget()
        self.zbd.setObjectName(u"zbd")
        self.gridLayout_68 = QGridLayout(self.zbd)
        self.gridLayout_68.setSpacing(6)
        self.gridLayout_68.setContentsMargins(11, 11, 11, 11)
        self.gridLayout_68.setObjectName(u"gridLayout_68")
        self.scrollArea_23 = QScrollArea(self.zbd)
        self.scrollArea_23.setObjectName(u"scrollArea_23")
        palette10 = QPalette()
        palette10.setBrush(QPalette.Active, QPalette.Base, brush2)
        palette10.setBrush(QPalette.Inactive, QPalette.Base, brush2)
        palette10.setBrush(QPalette.Disabled, QPalette.Base, brush3)
        self.scrollArea_23.setPalette(palette10)
        self.scrollArea_23.setWidgetResizable(True)
        self.scrollAreaWidgetContents_34 = QWidget()
        self.scrollAreaWidgetContents_34.setObjectName(u"scrollAreaWidgetContents_34")
        self.scrollAreaWidgetContents_34.setGeometry(QRect(0, 0, 970, 316))
        self.gridLayout_26 = QGridLayout(self.scrollAreaWidgetContents_34)
        self.gridLayout_26.setSpacing(6)
        self.gridLayout_26.setContentsMargins(11, 11, 11, 11)
        self.gridLayout_26.setObjectName(u"gridLayout_26")
        self.gridLayout_69 = QGridLayout()
        self.gridLayout_69.setSpacing(6)
        self.gridLayout_69.setObjectName(u"gridLayout_69")
        self.horizontalLayout_identify_4 = QHBoxLayout()
        self.horizontalLayout_identify_4.setSpacing(6)
        self.horizontalLayout_identify_4.setObjectName(u"horizontalLayout_identify_4")
        self.pushButton_if_state_set = QPushButton(self.scrollAreaWidgetContents_34)
        self.pushButton_if_state_set.setObjectName(u"pushButton_if_state_set")
        self.pushButton_if_state_set.setMinimumSize(QSize(121, 0))

        self.horizontalLayout_identify_4.addWidget(self.pushButton_if_state_set)

        self.comboBox_ifSetDstMode = QComboBox(self.scrollAreaWidgetContents_34)
        self.comboBox_ifSetDstMode.addItem("")
        self.comboBox_ifSetDstMode.addItem("")
        self.comboBox_ifSetDstMode.addItem("")
        self.comboBox_ifSetDstMode.addItem("")
        self.comboBox_ifSetDstMode.addItem("")
        self.comboBox_ifSetDstMode.addItem("")
        self.comboBox_ifSetDstMode.addItem("")
        self.comboBox_ifSetDstMode.addItem("")
        self.comboBox_ifSetDstMode.setObjectName(u"comboBox_ifSetDstMode")
        self.comboBox_ifSetDstMode.setMinimumContentsLength(0)

        self.horizontalLayout_identify_4.addWidget(self.comboBox_ifSetDstMode)

        self.lineEdit_ifStateSetDstAddr = QLineEdit(self.scrollAreaWidgetContents_34)
        self.lineEdit_ifStateSetDstAddr.setObjectName(u"lineEdit_ifStateSetDstAddr")
        sizePolicy3.setHeightForWidth(self.lineEdit_ifStateSetDstAddr.sizePolicy().hasHeightForWidth())
        self.lineEdit_ifStateSetDstAddr.setSizePolicy(sizePolicy3)
        self.lineEdit_ifStateSetDstAddr.setMinimumSize(QSize(160, 0))
        self.lineEdit_ifStateSetDstAddr.setMaximumSize(QSize(160, 16777215))

        self.horizontalLayout_identify_4.addWidget(self.lineEdit_ifStateSetDstAddr)

        self.lineEdit_ifStateSrcEp = QLineEdit(self.scrollAreaWidgetContents_34)
        self.lineEdit_ifStateSrcEp.setObjectName(u"lineEdit_ifStateSrcEp")
        self.lineEdit_ifStateSrcEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_identify_4.addWidget(self.lineEdit_ifStateSrcEp)

        self.lineEdit_ifStateDstEp = QLineEdit(self.scrollAreaWidgetContents_34)
        self.lineEdit_ifStateDstEp.setObjectName(u"lineEdit_ifStateDstEp")
        self.lineEdit_ifStateDstEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_identify_4.addWidget(self.lineEdit_ifStateDstEp)

        self.lineEdit_ifState = QLineEdit(self.scrollAreaWidgetContents_34)
        self.lineEdit_ifState.setObjectName(u"lineEdit_ifState")
        self.lineEdit_ifState.setMinimumSize(QSize(160, 0))
        self.lineEdit_ifState.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_identify_4.addWidget(self.lineEdit_ifState)

        self.horizontalSpacer_182 = QSpacerItem(0, 0, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_identify_4.addItem(self.horizontalSpacer_182)


        self.gridLayout_69.addLayout(self.horizontalLayout_identify_4, 0, 0, 1, 1)

        self.horizontalLayout_18 = QHBoxLayout()
        self.horizontalLayout_18.setSpacing(6)
        self.horizontalLayout_18.setObjectName(u"horizontalLayout_18")
        self.pushButton_anj_timeout = QPushButton(self.scrollAreaWidgetContents_34)
        self.pushButton_anj_timeout.setObjectName(u"pushButton_anj_timeout")
        self.pushButton_anj_timeout.setMinimumSize(QSize(121, 0))

        self.horizontalLayout_18.addWidget(self.pushButton_anj_timeout)

        self.comboBox_anjTimeDstMode = QComboBox(self.scrollAreaWidgetContents_34)
        self.comboBox_anjTimeDstMode.addItem("")
        self.comboBox_anjTimeDstMode.addItem("")
        self.comboBox_anjTimeDstMode.addItem("")
        self.comboBox_anjTimeDstMode.addItem("")
        self.comboBox_anjTimeDstMode.addItem("")
        self.comboBox_anjTimeDstMode.addItem("")
        self.comboBox_anjTimeDstMode.addItem("")
        self.comboBox_anjTimeDstMode.addItem("")
        self.comboBox_anjTimeDstMode.setObjectName(u"comboBox_anjTimeDstMode")
        self.comboBox_anjTimeDstMode.setMinimumContentsLength(0)

        self.horizontalLayout_18.addWidget(self.comboBox_anjTimeDstMode)

        self.lineEdit_anjTimeDstAddr = QLineEdit(self.scrollAreaWidgetContents_34)
        self.lineEdit_anjTimeDstAddr.setObjectName(u"lineEdit_anjTimeDstAddr")
        sizePolicy3.setHeightForWidth(self.lineEdit_anjTimeDstAddr.sizePolicy().hasHeightForWidth())
        self.lineEdit_anjTimeDstAddr.setSizePolicy(sizePolicy3)
        self.lineEdit_anjTimeDstAddr.setMinimumSize(QSize(160, 0))
        self.lineEdit_anjTimeDstAddr.setMaximumSize(QSize(160, 16777215))

        self.horizontalLayout_18.addWidget(self.lineEdit_anjTimeDstAddr)

        self.lineEdit_anjTimeSrcEp = QLineEdit(self.scrollAreaWidgetContents_34)
        self.lineEdit_anjTimeSrcEp.setObjectName(u"lineEdit_anjTimeSrcEp")
        self.lineEdit_anjTimeSrcEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_18.addWidget(self.lineEdit_anjTimeSrcEp)

        self.lineEdit_anjTimeDstEp = QLineEdit(self.scrollAreaWidgetContents_34)
        self.lineEdit_anjTimeDstEp.setObjectName(u"lineEdit_anjTimeDstEp")
        self.lineEdit_anjTimeDstEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_18.addWidget(self.lineEdit_anjTimeDstEp)

        self.lineEdit_anjTime = QLineEdit(self.scrollAreaWidgetContents_34)
        self.lineEdit_anjTime.setObjectName(u"lineEdit_anjTime")
        self.lineEdit_anjTime.setMinimumSize(QSize(160, 0))
        self.lineEdit_anjTime.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_18.addWidget(self.lineEdit_anjTime)

        self.horizontalSpacer_181 = QSpacerItem(40, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_18.addItem(self.horizontalSpacer_181)


        self.gridLayout_69.addLayout(self.horizontalLayout_18, 1, 0, 1, 1)

        self.verticalSpacer_41 = QSpacerItem(889, 217, QSizePolicy.Minimum, QSizePolicy.Expanding)

        self.gridLayout_69.addItem(self.verticalSpacer_41, 2, 0, 1, 1)


        self.gridLayout_26.addLayout(self.gridLayout_69, 0, 0, 1, 1)

        self.scrollArea_23.setWidget(self.scrollAreaWidgetContents_34)

        self.gridLayout_68.addWidget(self.scrollArea_23, 0, 0, 1, 1)

        self.tabWidget.addTab(self.zbd, "")
        self.AF = QWidget()
        self.AF.setObjectName(u"AF")
        self.gridLayout_23 = QGridLayout(self.AF)
        self.gridLayout_23.setSpacing(6)
        self.gridLayout_23.setContentsMargins(11, 11, 11, 11)
        self.gridLayout_23.setObjectName(u"gridLayout_23")
        self.scrollArea_10 = QScrollArea(self.AF)
        self.scrollArea_10.setObjectName(u"scrollArea_10")
        palette11 = QPalette()
        palette11.setBrush(QPalette.Active, QPalette.Base, brush2)
        palette11.setBrush(QPalette.Inactive, QPalette.Base, brush2)
        palette11.setBrush(QPalette.Disabled, QPalette.Base, brush3)
        self.scrollArea_10.setPalette(palette11)
        self.scrollArea_10.setWidgetResizable(True)
        self.scrollAreaWidgetContents_14 = QWidget()
        self.scrollAreaWidgetContents_14.setObjectName(u"scrollAreaWidgetContents_14")
        self.scrollAreaWidgetContents_14.setGeometry(QRect(0, 0, 970, 316))
        self.verticalLayout_6 = QVBoxLayout(self.scrollAreaWidgetContents_14)
        self.verticalLayout_6.setSpacing(6)
        self.verticalLayout_6.setContentsMargins(11, 11, 11, 11)
        self.verticalLayout_6.setObjectName(u"verticalLayout_6")
        self.label_afTestNotes = QLabel(self.scrollAreaWidgetContents_14)
        self.label_afTestNotes.setObjectName(u"label_afTestNotes")

        self.verticalLayout_6.addWidget(self.label_afTestNotes)

        self.horizontalLayout_txrxTest = QHBoxLayout()
        self.horizontalLayout_txrxTest.setSpacing(6)
        self.horizontalLayout_txrxTest.setObjectName(u"horizontalLayout_txrxTest")
        self.pushButton_txrxTest = QPushButton(self.scrollAreaWidgetContents_14)
        self.pushButton_txrxTest.setObjectName(u"pushButton_txrxTest")

        self.horizontalLayout_txrxTest.addWidget(self.pushButton_txrxTest)

        self.lineEdit_txrxTestDstAddr = QLineEdit(self.scrollAreaWidgetContents_14)
        self.lineEdit_txrxTestDstAddr.setObjectName(u"lineEdit_txrxTestDstAddr")
        sizePolicy3.setHeightForWidth(self.lineEdit_txrxTestDstAddr.sizePolicy().hasHeightForWidth())
        self.lineEdit_txrxTestDstAddr.setSizePolicy(sizePolicy3)
        self.lineEdit_txrxTestDstAddr.setMinimumSize(QSize(260, 0))
        self.lineEdit_txrxTestDstAddr.setMaximumSize(QSize(160, 16777215))

        self.horizontalLayout_txrxTest.addWidget(self.lineEdit_txrxTestDstAddr)

        self.lineEdit_txrxTestSrcEp = QLineEdit(self.scrollAreaWidgetContents_14)
        self.lineEdit_txrxTestSrcEp.setObjectName(u"lineEdit_txrxTestSrcEp")
        self.lineEdit_txrxTestSrcEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_txrxTest.addWidget(self.lineEdit_txrxTestSrcEp)

        self.lineEdit_txrxTestDstEp = QLineEdit(self.scrollAreaWidgetContents_14)
        self.lineEdit_txrxTestDstEp.setObjectName(u"lineEdit_txrxTestDstEp")
        self.lineEdit_txrxTestDstEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_txrxTest.addWidget(self.lineEdit_txrxTestDstEp)

        self.lineEdit_txrxTestSendCount = QLineEdit(self.scrollAreaWidgetContents_14)
        self.lineEdit_txrxTestSendCount.setObjectName(u"lineEdit_txrxTestSendCount")
        self.lineEdit_txrxTestSendCount.setMinimumSize(QSize(100, 0))
        self.lineEdit_txrxTestSendCount.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_txrxTest.addWidget(self.lineEdit_txrxTestSendCount)

        self.lineEdit_txrxTestInterval = QLineEdit(self.scrollAreaWidgetContents_14)
        self.lineEdit_txrxTestInterval.setObjectName(u"lineEdit_txrxTestInterval")
        self.lineEdit_txrxTestInterval.setMinimumSize(QSize(100, 0))
        self.lineEdit_txrxTestInterval.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_txrxTest.addWidget(self.lineEdit_txrxTestInterval)

        self.lineEdit_txrxTestTxPower = QLineEdit(self.scrollAreaWidgetContents_14)
        self.lineEdit_txrxTestTxPower.setObjectName(u"lineEdit_txrxTestTxPower")
        self.lineEdit_txrxTestTxPower.setMinimumSize(QSize(100, 0))
        self.lineEdit_txrxTestTxPower.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_txrxTest.addWidget(self.lineEdit_txrxTestTxPower)

        self.horizontalSpacer_57 = QSpacerItem(0, 0, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_txrxTest.addItem(self.horizontalSpacer_57)


        self.verticalLayout_6.addLayout(self.horizontalLayout_txrxTest)

        self.horizontalLayout_afDataSend = QHBoxLayout()
        self.horizontalLayout_afDataSend.setSpacing(6)
        self.horizontalLayout_afDataSend.setObjectName(u"horizontalLayout_afDataSend")
        self.pushButton_afDataSend = QPushButton(self.scrollAreaWidgetContents_14)
        self.pushButton_afDataSend.setObjectName(u"pushButton_afDataSend")

        self.horizontalLayout_afDataSend.addWidget(self.pushButton_afDataSend)

        self.lineEdit_afDataSendDstAddr = QLineEdit(self.scrollAreaWidgetContents_14)
        self.lineEdit_afDataSendDstAddr.setObjectName(u"lineEdit_afDataSendDstAddr")
        sizePolicy3.setHeightForWidth(self.lineEdit_afDataSendDstAddr.sizePolicy().hasHeightForWidth())
        self.lineEdit_afDataSendDstAddr.setSizePolicy(sizePolicy3)
        self.lineEdit_afDataSendDstAddr.setMinimumSize(QSize(260, 0))
        self.lineEdit_afDataSendDstAddr.setMaximumSize(QSize(160, 16777215))

        self.horizontalLayout_afDataSend.addWidget(self.lineEdit_afDataSendDstAddr)

        self.lineEdit_afDataSendSrcEp = QLineEdit(self.scrollAreaWidgetContents_14)
        self.lineEdit_afDataSendSrcEp.setObjectName(u"lineEdit_afDataSendSrcEp")
        self.lineEdit_afDataSendSrcEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_afDataSend.addWidget(self.lineEdit_afDataSendSrcEp)

        self.lineEdit_afDataSendDstEp = QLineEdit(self.scrollAreaWidgetContents_14)
        self.lineEdit_afDataSendDstEp.setObjectName(u"lineEdit_afDataSendDstEp")
        self.lineEdit_afDataSendDstEp.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_afDataSend.addWidget(self.lineEdit_afDataSendDstEp)

        self.lineEdit_afDataSendClusterId = QLineEdit(self.scrollAreaWidgetContents_14)
        self.lineEdit_afDataSendClusterId.setObjectName(u"lineEdit_afDataSendClusterId")
        self.lineEdit_afDataSendClusterId.setMinimumSize(QSize(100, 0))
        self.lineEdit_afDataSendClusterId.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_afDataSend.addWidget(self.lineEdit_afDataSendClusterId)

        self.lineEdit_afDataSendData = QLineEdit(self.scrollAreaWidgetContents_14)
        self.lineEdit_afDataSendData.setObjectName(u"lineEdit_afDataSendData")
        self.lineEdit_afDataSendData.setMinimumSize(QSize(260, 0))
        self.lineEdit_afDataSendData.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_afDataSend.addWidget(self.lineEdit_afDataSendData)

        self.horizontalSpacer_65 = QSpacerItem(0, 0, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_afDataSend.addItem(self.horizontalSpacer_65)


        self.verticalLayout_6.addLayout(self.horizontalLayout_afDataSend)

        self.horizontalLayout_toggleTest = QHBoxLayout()
        self.horizontalLayout_toggleTest.setSpacing(6)
        self.horizontalLayout_toggleTest.setObjectName(u"horizontalLayout_toggleTest")
        self.pushButton_nodesToggleTest = QPushButton(self.scrollAreaWidgetContents_14)
        self.pushButton_nodesToggleTest.setObjectName(u"pushButton_nodesToggleTest")

        self.horizontalLayout_toggleTest.addWidget(self.pushButton_nodesToggleTest)

        self.comboBox_toggleTestMode = QComboBox(self.scrollAreaWidgetContents_14)
        self.comboBox_toggleTestMode.addItem("")
        self.comboBox_toggleTestMode.addItem("")
        self.comboBox_toggleTestMode.setObjectName(u"comboBox_toggleTestMode")
        self.comboBox_toggleTestMode.setSizeAdjustPolicy(QComboBox.AdjustToContents)

        self.horizontalLayout_toggleTest.addWidget(self.comboBox_toggleTestMode)

        self.lineEdit_toggleInterval = QLineEdit(self.scrollAreaWidgetContents_14)
        self.lineEdit_toggleInterval.setObjectName(u"lineEdit_toggleInterval")
        self.lineEdit_toggleInterval.setMinimumSize(QSize(460, 0))
        self.lineEdit_toggleInterval.setMaximumSize(QSize(60, 16777215))

        self.horizontalLayout_toggleTest.addWidget(self.lineEdit_toggleInterval)

        self.horizontalSpacer_66 = QSpacerItem(0, 0, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_toggleTest.addItem(self.horizontalSpacer_66)


        self.verticalLayout_6.addLayout(self.horizontalLayout_toggleTest)

        self.verticalSpacer_12 = QSpacerItem(0, 0, QSizePolicy.Minimum, QSizePolicy.Expanding)

        self.verticalLayout_6.addItem(self.verticalSpacer_12)

        self.scrollArea_10.setWidget(self.scrollAreaWidgetContents_14)

        self.gridLayout_23.addWidget(self.scrollArea_10, 0, 0, 1, 1)

        self.tabWidget.addTab(self.AF, "")
        self.hciOta = QWidget()
        self.hciOta.setObjectName(u"hciOta")
        self.gridLayout_24 = QGridLayout(self.hciOta)
        self.gridLayout_24.setSpacing(6)
        self.gridLayout_24.setContentsMargins(11, 11, 11, 11)
        self.gridLayout_24.setObjectName(u"gridLayout_24")
        self.horizontalLayout_hciOtaConf = QHBoxLayout()
        self.horizontalLayout_hciOtaConf.setSpacing(6)
        self.horizontalLayout_hciOtaConf.setObjectName(u"horizontalLayout_hciOtaConf")
        self.pushButton_hciOtaFileChoose = QPushButton(self.hciOta)
        self.pushButton_hciOtaFileChoose.setObjectName(u"pushButton_hciOtaFileChoose")

        self.horizontalLayout_hciOtaConf.addWidget(self.pushButton_hciOtaFileChoose)

        self.lineEdit_hciOtaFilePath = QLineEdit(self.hciOta)
        self.lineEdit_hciOtaFilePath.setObjectName(u"lineEdit_hciOtaFilePath")
        self.lineEdit_hciOtaFilePath.setEnabled(True)

        self.horizontalLayout_hciOtaConf.addWidget(self.lineEdit_hciOtaFilePath)

        self.pushButton_hciOtaStart = QPushButton(self.hciOta)
        self.pushButton_hciOtaStart.setObjectName(u"pushButton_hciOtaStart")

        self.horizontalLayout_hciOtaConf.addWidget(self.pushButton_hciOtaStart)


        self.gridLayout_24.addLayout(self.horizontalLayout_hciOtaConf, 0, 0, 1, 1)

        self.horizontalLayout_4 = QHBoxLayout()
        self.horizontalLayout_4.setSpacing(6)
        self.horizontalLayout_4.setObjectName(u"horizontalLayout_4")
        self.label_hciotaPercent = QLabel(self.hciOta)
        self.label_hciotaPercent.setObjectName(u"label_hciotaPercent")

        self.horizontalLayout_4.addWidget(self.label_hciotaPercent)

        self.progressBar_hciOta = QProgressBar(self.hciOta)
        self.progressBar_hciOta.setObjectName(u"progressBar_hciOta")
        self.progressBar_hciOta.setValue(0)

        self.horizontalLayout_4.addWidget(self.progressBar_hciOta)


        self.gridLayout_24.addLayout(self.horizontalLayout_4, 1, 0, 1, 1)

        self.verticalSpacer_15 = QSpacerItem(0, 0, QSizePolicy.Minimum, QSizePolicy.Expanding)

        self.gridLayout_24.addItem(self.verticalSpacer_15, 2, 0, 1, 1)

        self.tabWidget.addTab(self.hciOta, "")
        self.analyze = QWidget()
        self.analyze.setObjectName(u"analyze")
        self.gridLayout_2 = QGridLayout(self.analyze)
        self.gridLayout_2.setSpacing(6)
        self.gridLayout_2.setContentsMargins(11, 11, 11, 11)
        self.gridLayout_2.setObjectName(u"gridLayout_2")
        self.horizontalLayout_6 = QHBoxLayout()
        self.horizontalLayout_6.setSpacing(6)
        self.horizontalLayout_6.setObjectName(u"horizontalLayout_6")
        self.label_analyzeAddr = QLabel(self.analyze)
        self.label_analyzeAddr.setObjectName(u"label_analyzeAddr")
        self.label_analyzeAddr.setAlignment(Qt.AlignRight|Qt.AlignTrailing|Qt.AlignVCenter)

        self.horizontalLayout_6.addWidget(self.label_analyzeAddr)

        self.lineEdit_analyzeAddr = QLineEdit(self.analyze)
        self.lineEdit_analyzeAddr.setObjectName(u"lineEdit_analyzeAddr")
        self.lineEdit_analyzeAddr.setEnabled(True)

        self.horizontalLayout_6.addWidget(self.lineEdit_analyzeAddr)

        self.label_analyzeInterval = QLabel(self.analyze)
        self.label_analyzeInterval.setObjectName(u"label_analyzeInterval")
        self.label_analyzeInterval.setAlignment(Qt.AlignRight|Qt.AlignTrailing|Qt.AlignVCenter)

        self.horizontalLayout_6.addWidget(self.label_analyzeInterval)

        self.lineEdit_analyzeInterval = QLineEdit(self.analyze)
        self.lineEdit_analyzeInterval.setObjectName(u"lineEdit_analyzeInterval")
        self.lineEdit_analyzeInterval.setEnabled(True)
        self.lineEdit_analyzeInterval.setEchoMode(QLineEdit.Normal)

        self.horizontalLayout_6.addWidget(self.lineEdit_analyzeInterval)

        self.label_analyzeCommand = QLabel(self.analyze)
        self.label_analyzeCommand.setObjectName(u"label_analyzeCommand")
        self.label_analyzeCommand.setAlignment(Qt.AlignRight|Qt.AlignTrailing|Qt.AlignVCenter)

        self.horizontalLayout_6.addWidget(self.label_analyzeCommand)

        self.lineEdit_analyzeCommandId = QLineEdit(self.analyze)
        self.lineEdit_analyzeCommandId.setObjectName(u"lineEdit_analyzeCommandId")
        self.lineEdit_analyzeCommandId.setEnabled(True)
        self.lineEdit_analyzeCommandId.setEchoMode(QLineEdit.Normal)

        self.horizontalLayout_6.addWidget(self.lineEdit_analyzeCommandId)

        self.pushButton_startFilter = QPushButton(self.analyze)
        self.pushButton_startFilter.setObjectName(u"pushButton_startFilter")

        self.horizontalLayout_6.addWidget(self.pushButton_startFilter)


        self.gridLayout_2.addLayout(self.horizontalLayout_6, 2, 0, 1, 1)

        self.horizontalLayout_5 = QHBoxLayout()
        self.horizontalLayout_5.setSpacing(6)
        self.horizontalLayout_5.setObjectName(u"horizontalLayout_5")
        self.pushButton_startAnalyze = QPushButton(self.analyze)
        self.pushButton_startAnalyze.setObjectName(u"pushButton_startAnalyze")

        self.horizontalLayout_5.addWidget(self.pushButton_startAnalyze)

        self.textEdit_nwkAddrShow = QTextEdit(self.analyze)
        self.textEdit_nwkAddrShow.setObjectName(u"textEdit_nwkAddrShow")
        self.textEdit_nwkAddrShow.setMaximumSize(QSize(16777215, 16777215))

        self.horizontalLayout_5.addWidget(self.textEdit_nwkAddrShow)


        self.gridLayout_2.addLayout(self.horizontalLayout_5, 1, 0, 1, 1)

        self.horizontalLayout_3 = QHBoxLayout()
        self.horizontalLayout_3.setSpacing(6)
        self.horizontalLayout_3.setObjectName(u"horizontalLayout_3")
        self.pushButton_analyzePathChoose = QPushButton(self.analyze)
        self.pushButton_analyzePathChoose.setObjectName(u"pushButton_analyzePathChoose")

        self.horizontalLayout_3.addWidget(self.pushButton_analyzePathChoose)

        self.lineEdit_analyzePath = QLineEdit(self.analyze)
        self.lineEdit_analyzePath.setObjectName(u"lineEdit_analyzePath")
        self.lineEdit_analyzePath.setEnabled(True)

        self.horizontalLayout_3.addWidget(self.lineEdit_analyzePath)


        self.gridLayout_2.addLayout(self.horizontalLayout_3, 0, 0, 1, 1)

        self.tabWidget.addTab(self.analyze, "")
        self.blehci = QWidget()
        self.blehci.setObjectName(u"blehci")
        self.gridLayout_25 = QGridLayout(self.blehci)
        self.gridLayout_25.setSpacing(6)
        self.gridLayout_25.setContentsMargins(11, 11, 11, 11)
        self.gridLayout_25.setObjectName(u"gridLayout_25")
        self.gridLayout_6 = QGridLayout()
        self.gridLayout_6.setSpacing(6)
        self.gridLayout_6.setObjectName(u"gridLayout_6")
        self.horizontalLayout_10 = QHBoxLayout()
        self.horizontalLayout_10.setSpacing(6)
        self.horizontalLayout_10.setObjectName(u"horizontalLayout_10")
        self.pushButton_advParaSet = QPushButton(self.blehci)
        self.pushButton_advParaSet.setObjectName(u"pushButton_advParaSet")

        self.horizontalLayout_10.addWidget(self.pushButton_advParaSet)

        self.lineEdit_minAdvInterval = QLineEdit(self.blehci)
        self.lineEdit_minAdvInterval.setObjectName(u"lineEdit_minAdvInterval")
        sizePolicy3.setHeightForWidth(self.lineEdit_minAdvInterval.sizePolicy().hasHeightForWidth())
        self.lineEdit_minAdvInterval.setSizePolicy(sizePolicy3)

        self.horizontalLayout_10.addWidget(self.lineEdit_minAdvInterval)

        self.lineEdit_maxAdvInterval = QLineEdit(self.blehci)
        self.lineEdit_maxAdvInterval.setObjectName(u"lineEdit_maxAdvInterval")
        sizePolicy3.setHeightForWidth(self.lineEdit_maxAdvInterval.sizePolicy().hasHeightForWidth())
        self.lineEdit_maxAdvInterval.setSizePolicy(sizePolicy3)

        self.horizontalLayout_10.addWidget(self.lineEdit_maxAdvInterval)

        self.horizontalSpacer_20 = QSpacerItem(80, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_10.addItem(self.horizontalSpacer_20)


        self.gridLayout_6.addLayout(self.horizontalLayout_10, 0, 1, 1, 1)

        self.listWidget_AdvList = QListWidget(self.blehci)
        self.listWidget_AdvList.setObjectName(u"listWidget_AdvList")
        sizePolicy5 = QSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
        sizePolicy5.setHorizontalStretch(0)
        sizePolicy5.setVerticalStretch(0)
        sizePolicy5.setHeightForWidth(self.listWidget_AdvList.sizePolicy().hasHeightForWidth())
        self.listWidget_AdvList.setSizePolicy(sizePolicy5)

        self.gridLayout_6.addWidget(self.listWidget_AdvList, 0, 2, 8, 1)

        self.horizontalLayout_11 = QHBoxLayout()
        self.horizontalLayout_11.setSpacing(6)
        self.horizontalLayout_11.setObjectName(u"horizontalLayout_11")
        self.pushButton_advDataSet = QPushButton(self.blehci)
        self.pushButton_advDataSet.setObjectName(u"pushButton_advDataSet")

        self.horizontalLayout_11.addWidget(self.pushButton_advDataSet)

        self.comboBox_advOrScanRe = QComboBox(self.blehci)
        self.comboBox_advOrScanRe.addItem("")
        self.comboBox_advOrScanRe.addItem("")
        self.comboBox_advOrScanRe.setObjectName(u"comboBox_advOrScanRe")

        self.horizontalLayout_11.addWidget(self.comboBox_advOrScanRe)

        self.lineEdit_advData = QLineEdit(self.blehci)
        self.lineEdit_advData.setObjectName(u"lineEdit_advData")
        self.lineEdit_advData.setEnabled(True)
        sizePolicy3.setHeightForWidth(self.lineEdit_advData.sizePolicy().hasHeightForWidth())
        self.lineEdit_advData.setSizePolicy(sizePolicy3)
        self.lineEdit_advData.setMinimumSize(QSize(200, 0))

        self.horizontalLayout_11.addWidget(self.lineEdit_advData)

        self.horizontalSpacer_21 = QSpacerItem(35, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_11.addItem(self.horizontalSpacer_21)


        self.gridLayout_6.addLayout(self.horizontalLayout_11, 1, 1, 1, 1)

        self.horizontalLayout_12 = QHBoxLayout()
        self.horizontalLayout_12.setSpacing(6)
        self.horizontalLayout_12.setObjectName(u"horizontalLayout_12")
        self.pushButton_advStart = QPushButton(self.blehci)
        self.pushButton_advStart.setObjectName(u"pushButton_advStart")

        self.horizontalLayout_12.addWidget(self.pushButton_advStart)

        self.pushButton_advStop = QPushButton(self.blehci)
        self.pushButton_advStop.setObjectName(u"pushButton_advStop")

        self.horizontalLayout_12.addWidget(self.pushButton_advStop)

        self.horizontalSpacer_23 = QSpacerItem(40, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_12.addItem(self.horizontalSpacer_23)


        self.gridLayout_6.addLayout(self.horizontalLayout_12, 2, 1, 1, 1)

        self.horizontalLayout_15 = QHBoxLayout()
        self.horizontalLayout_15.setSpacing(6)
        self.horizontalLayout_15.setObjectName(u"horizontalLayout_15")
        self.pushButton_bleDisconnect = QPushButton(self.blehci)
        self.pushButton_bleDisconnect.setObjectName(u"pushButton_bleDisconnect")

        self.horizontalLayout_15.addWidget(self.pushButton_bleDisconnect)

        self.lineEdit_bleConnHandle = QLineEdit(self.blehci)
        self.lineEdit_bleConnHandle.setObjectName(u"lineEdit_bleConnHandle")
        sizePolicy3.setHeightForWidth(self.lineEdit_bleConnHandle.sizePolicy().hasHeightForWidth())
        self.lineEdit_bleConnHandle.setSizePolicy(sizePolicy3)

        self.horizontalLayout_15.addWidget(self.lineEdit_bleConnHandle)

        self.horizontalSpacer_70 = QSpacerItem(40, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_15.addItem(self.horizontalSpacer_70)


        self.gridLayout_6.addLayout(self.horizontalLayout_15, 6, 1, 1, 1)

        self.horizontalLayout_8 = QHBoxLayout()
        self.horizontalLayout_8.setSpacing(6)
        self.horizontalLayout_8.setObjectName(u"horizontalLayout_8")
        self.pushButton_scanParaSet = QPushButton(self.blehci)
        self.pushButton_scanParaSet.setObjectName(u"pushButton_scanParaSet")

        self.horizontalLayout_8.addWidget(self.pushButton_scanParaSet)

        self.lineEdit_scanInterval = QLineEdit(self.blehci)
        self.lineEdit_scanInterval.setObjectName(u"lineEdit_scanInterval")
        sizePolicy3.setHeightForWidth(self.lineEdit_scanInterval.sizePolicy().hasHeightForWidth())
        self.lineEdit_scanInterval.setSizePolicy(sizePolicy3)

        self.horizontalLayout_8.addWidget(self.lineEdit_scanInterval)

        self.lineEdit_scanWindow = QLineEdit(self.blehci)
        self.lineEdit_scanWindow.setObjectName(u"lineEdit_scanWindow")
        sizePolicy3.setHeightForWidth(self.lineEdit_scanWindow.sizePolicy().hasHeightForWidth())
        self.lineEdit_scanWindow.setSizePolicy(sizePolicy3)

        self.horizontalLayout_8.addWidget(self.lineEdit_scanWindow)

        self.horizontalSpacer_69 = QSpacerItem(40, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_8.addItem(self.horizontalSpacer_69)


        self.gridLayout_6.addLayout(self.horizontalLayout_8, 3, 1, 1, 1)

        self.label_bleState = QLabel(self.blehci)
        self.label_bleState.setObjectName(u"label_bleState")
        sizePolicy6 = QSizePolicy(QSizePolicy.Preferred, QSizePolicy.Preferred)
        sizePolicy6.setHorizontalStretch(0)
        sizePolicy6.setVerticalStretch(0)
        sizePolicy6.setHeightForWidth(self.label_bleState.sizePolicy().hasHeightForWidth())
        self.label_bleState.setSizePolicy(sizePolicy6)

        self.gridLayout_6.addWidget(self.label_bleState, 7, 0, 2, 2)

        self.verticalSpacer = QSpacerItem(963, 13, QSizePolicy.Minimum, QSizePolicy.Expanding)

        self.gridLayout_6.addItem(self.verticalSpacer, 8, 1, 1, 2)

        self.horizontalLayout_14 = QHBoxLayout()
        self.horizontalLayout_14.setSpacing(6)
        self.horizontalLayout_14.setObjectName(u"horizontalLayout_14")
        self.pushButton_bleScanstart = QPushButton(self.blehci)
        self.pushButton_bleScanstart.setObjectName(u"pushButton_bleScanstart")

        self.horizontalLayout_14.addWidget(self.pushButton_bleScanstart)

        self.pushButton_bleScanstop = QPushButton(self.blehci)
        self.pushButton_bleScanstop.setObjectName(u"pushButton_bleScanstop")

        self.horizontalLayout_14.addWidget(self.pushButton_bleScanstop)

        self.horizontalSpacer_68 = QSpacerItem(40, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_14.addItem(self.horizontalSpacer_68)


        self.gridLayout_6.addLayout(self.horizontalLayout_14, 4, 1, 1, 1)

        self.horizontalLayout_13 = QHBoxLayout()
        self.horizontalLayout_13.setSpacing(6)
        self.horizontalLayout_13.setObjectName(u"horizontalLayout_13")
        self.pushButton_bleConnect = QPushButton(self.blehci)
        self.pushButton_bleConnect.setObjectName(u"pushButton_bleConnect")

        self.horizontalLayout_13.addWidget(self.pushButton_bleConnect)

        self.lineEdit_bleMac = QLineEdit(self.blehci)
        self.lineEdit_bleMac.setObjectName(u"lineEdit_bleMac")
        sizePolicy3.setHeightForWidth(self.lineEdit_bleMac.sizePolicy().hasHeightForWidth())
        self.lineEdit_bleMac.setSizePolicy(sizePolicy3)

        self.horizontalLayout_13.addWidget(self.lineEdit_bleMac)

        self.pushButton_getConnList = QPushButton(self.blehci)
        self.pushButton_getConnList.setObjectName(u"pushButton_getConnList")

        self.horizontalLayout_13.addWidget(self.pushButton_getConnList)

        self.horizontalSpacer_49 = QSpacerItem(40, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)

        self.horizontalLayout_13.addItem(self.horizontalSpacer_49)


        self.gridLayout_6.addLayout(self.horizontalLayout_13, 5, 1, 1, 1)


        self.gridLayout_25.addLayout(self.gridLayout_6, 0, 0, 1, 1)

        self.tabWidget.addTab(self.blehci, "")

        self.gridLayout_67.addWidget(self.tabWidget, 0, 1, 1, 1)

        MainWindow.setCentralWidget(self.centralWidget)
        self.menuBar = QMenuBar(MainWindow)
        self.menuBar.setObjectName(u"menuBar")
        self.menuBar.setGeometry(QRect(0, 0, 1229, 26))
        MainWindow.setMenuBar(self.menuBar)
        self.statusBar = QStatusBar(MainWindow)
        self.statusBar.setObjectName(u"statusBar")
        MainWindow.setStatusBar(self.statusBar)
        self.toolBar = QToolBar(MainWindow)
        self.toolBar.setObjectName(u"toolBar")
        MainWindow.addToolBar(Qt.TopToolBarArea, self.toolBar)
#if QT_CONFIG(shortcut)
        self.label_portName.setBuddy(self.comboBox_portName)
        self.label_baudrate.setBuddy(self.comboBox_baudrate)
        self.label_databits.setBuddy(self.comboBox_databits)
        self.label_parity.setBuddy(self.comboBox_parity)
        self.label_stopbits.setBuddy(self.comboBox_stopbits)
#endif // QT_CONFIG(shortcut)
        QWidget.setTabOrder(self.comboBox_portName, self.comboBox_baudrate)
        QWidget.setTabOrder(self.comboBox_baudrate, self.comboBox_databits)
        QWidget.setTabOrder(self.comboBox_databits, self.comboBox_parity)
        QWidget.setTabOrder(self.comboBox_parity, self.comboBox_stopbits)

        self.retranslateUi(MainWindow)

        self.comboBox_baudrate.setCurrentIndex(3)
        self.comboBox_databits.setCurrentIndex(3)
        self.tabWidget.setCurrentIndex(0)
        self.comboBox_readAttrDstMode.setCurrentIndex(2)
        self.comboBox_writeAttDstMode.setCurrentIndex(2)
        self.comboBox_readReportCfgDstMode.setCurrentIndex(2)
        self.comboBox_resetDstMode.setCurrentIndex(2)
        self.comboBox_configReportDstMode.setCurrentIndex(2)
        self.comboBox_addGroupDstmode.setCurrentIndex(2)
        self.comboBox_rmGroupDstMode.setCurrentIndex(2)
        self.comboBox_getGroupDstMode.setCurrentIndex(2)
        self.comboBox_viewGroupDstMode.setCurrentIndex(2)
        self.comboBox_addIdenDstMode.setCurrentIndex(2)
        self.comboBox_removeAllDstMode.setCurrentIndex(2)
        self.comboBox_onoffDstMode.setCurrentIndex(2)
        self.comboBox_toLevelDstMode.setCurrentIndex(2)
        self.comboBox_stopDstMode.setCurrentIndex(2)
        self.comboBox_moveDstMode.setCurrentIndex(2)
        self.comboBox_stepDstMode.setCurrentIndex(2)
        self.comboBox_moveHueDstMode.setCurrentIndex(2)
        self.comboBox_moveTempDstMode.setCurrentIndex(2)
        self.comboBox_moveSatDstMode.setCurrentIndex(2)
        self.comboBox_moveColorDstMode.setCurrentIndex(2)
        self.comboBox_identifyQueryDstMode.setCurrentIndex(2)
        self.comboBox_identifyDstMode.setCurrentIndex(2)
        self.comboBox_addSceneDstMode.setCurrentIndex(2)
        self.comboBox_storeSceneDstMode.setCurrentIndex(2)
        self.comboBox_viewSceneDstMode.setCurrentIndex(2)
        self.comboBox_recallSceneDstMode.setCurrentIndex(2)
        self.comboBox_sceneRemoveAllDstMode.setCurrentIndex(2)
        self.comboBox_removeSceneDstMode.setCurrentIndex(2)
        self.comboBox_getSceneDstMode.setCurrentIndex(2)
        self.comboBox_otaDstMode.setCurrentIndex(2)
        self.comboBox_ifSetDstMode.setCurrentIndex(2)
        self.comboBox_anjTimeDstMode.setCurrentIndex(2)


        QMetaObject.connectSlotsByName(MainWindow)
    # setupUi

    def retranslateUi(self, MainWindow):
        MainWindow.setWindowTitle(QCoreApplication.translate("MainWindow", u"ZGC", None))
        self.label_portName.setText(QCoreApplication.translate("MainWindow", u"Port    ", None))
        self.label_baudrate.setText(QCoreApplication.translate("MainWindow", u"BaudRate", None))
        self.comboBox_baudrate.setItemText(0, QCoreApplication.translate("MainWindow", u"9600", None))
        self.comboBox_baudrate.setItemText(1, QCoreApplication.translate("MainWindow", u"19200", None))
        self.comboBox_baudrate.setItemText(2, QCoreApplication.translate("MainWindow", u"57600", None))
        self.comboBox_baudrate.setItemText(3, QCoreApplication.translate("MainWindow", u"115200", None))
        self.comboBox_baudrate.setItemText(4, QCoreApplication.translate("MainWindow", u"1000000", None))

        self.comboBox_baudrate.setCurrentText(QCoreApplication.translate("MainWindow", u"115200", None))
        self.label_databits.setText(QCoreApplication.translate("MainWindow", u"DataBits", None))
        self.comboBox_databits.setItemText(0, QCoreApplication.translate("MainWindow", u"5", None))
        self.comboBox_databits.setItemText(1, QCoreApplication.translate("MainWindow", u"6", None))
        self.comboBox_databits.setItemText(2, QCoreApplication.translate("MainWindow", u"7", None))
        self.comboBox_databits.setItemText(3, QCoreApplication.translate("MainWindow", u"8", None))

        self.label_parity.setText(QCoreApplication.translate("MainWindow", u"Parity  ", None))
        self.comboBox_parity.setItemText(0, QCoreApplication.translate("MainWindow", u"None", None))
        self.comboBox_parity.setItemText(1, QCoreApplication.translate("MainWindow", u"Even", None))
        self.comboBox_parity.setItemText(2, QCoreApplication.translate("MainWindow", u"Odd", None))
        self.comboBox_parity.setItemText(3, QCoreApplication.translate("MainWindow", u"Mark", None))
        self.comboBox_parity.setItemText(4, QCoreApplication.translate("MainWindow", u"Space", None))

        self.label_stopbits.setText(QCoreApplication.translate("MainWindow", u"StopBits", None))
        self.comboBox_stopbits.setItemText(0, QCoreApplication.translate("MainWindow", u"1", None))
        self.comboBox_stopbits.setItemText(1, QCoreApplication.translate("MainWindow", u"1.5", None))
        self.comboBox_stopbits.setItemText(2, QCoreApplication.translate("MainWindow", u"2", None))

        self.pushButton_openPort.setText(QCoreApplication.translate("MainWindow", u"open", None))
        self.pushButton_closePort.setText(QCoreApplication.translate("MainWindow", u"close", None))
        self.label_portState.setText("")
        self.checkBox_scroll.setText(QCoreApplication.translate("MainWindow", u"scroll to bottom", None))
        self.checkBox_autoClear.setText(QCoreApplication.translate("MainWindow", u"auto clear(500 lines)", None))
        self.pushButton_clear.setText(QCoreApplication.translate("MainWindow", u"clear", None))
        self.pushButton_send.setText(QCoreApplication.translate("MainWindow", u"Send", None))
        self.checkBox_thread.setText(QCoreApplication.translate("MainWindow", u"thread", None))
        self.checkBox_bleHci.setText(QCoreApplication.translate("MainWindow", u"bleHci", None))
        self.checkBox_timesend.setText(QCoreApplication.translate("MainWindow", u"Repeat Send", None))
        self.lineEdit_sendInterval.setInputMask("")
        self.lineEdit_sendInterval.setText(QCoreApplication.translate("MainWindow", u"1000", None))
        self.lineEdit_sendInterval.setPlaceholderText(QCoreApplication.translate("MainWindow", u"1000", None))
        self.label_sendInterval.setText(QCoreApplication.translate("MainWindow", u"ms", None))
        self.label_recvLabel.setText(QCoreApplication.translate("MainWindow", u"Recv Detection Interval", None))
        self.lineEdit_recvInterval.setInputMask("")
        self.lineEdit_recvInterval.setText(QCoreApplication.translate("MainWindow", u"5", None))
        self.lineEdit_recvInterval.setPlaceholderText(QCoreApplication.translate("MainWindow", u"1000", None))
        self.label_recvInterval.setText(QCoreApplication.translate("MainWindow", u"ms", None))
        self.pushButton_BDBsetCh.setText(QCoreApplication.translate("MainWindow", u"set channel", None))
        self.comboBox_channelList.setItemText(0, QCoreApplication.translate("MainWindow", u"11", None))
        self.comboBox_channelList.setItemText(1, QCoreApplication.translate("MainWindow", u"12", None))
        self.comboBox_channelList.setItemText(2, QCoreApplication.translate("MainWindow", u"13", None))
        self.comboBox_channelList.setItemText(3, QCoreApplication.translate("MainWindow", u"14", None))
        self.comboBox_channelList.setItemText(4, QCoreApplication.translate("MainWindow", u"15", None))
        self.comboBox_channelList.setItemText(5, QCoreApplication.translate("MainWindow", u"16", None))
        self.comboBox_channelList.setItemText(6, QCoreApplication.translate("MainWindow", u"17", None))
        self.comboBox_channelList.setItemText(7, QCoreApplication.translate("MainWindow", u"18", None))
        self.comboBox_channelList.setItemText(8, QCoreApplication.translate("MainWindow", u"19", None))
        self.comboBox_channelList.setItemText(9, QCoreApplication.translate("MainWindow", u"20", None))
        self.comboBox_channelList.setItemText(10, QCoreApplication.translate("MainWindow", u"21", None))
        self.comboBox_channelList.setItemText(11, QCoreApplication.translate("MainWindow", u"22", None))
        self.comboBox_channelList.setItemText(12, QCoreApplication.translate("MainWindow", u"23", None))
        self.comboBox_channelList.setItemText(13, QCoreApplication.translate("MainWindow", u"24", None))
        self.comboBox_channelList.setItemText(14, QCoreApplication.translate("MainWindow", u"25", None))
        self.comboBox_channelList.setItemText(15, QCoreApplication.translate("MainWindow", u"26", None))

        self.pushButton_BDBstartNet.setText(QCoreApplication.translate("MainWindow", u"start network", None))
        self.pushButton_BDBfactoryRst.setText(QCoreApplication.translate("MainWindow", u"factory reset", None))
        self.pushButton_BDBpreinstallCode.setText(QCoreApplication.translate("MainWindow", u"preinstall code", None))
#if QT_CONFIG(accessibility)
        self.lineEdit_dstAddr.setAccessibleName("")
#endif // QT_CONFIG(accessibility)
        self.lineEdit_dstAddr.setInputMask("")
        self.lineEdit_dstAddr.setText("")
        self.lineEdit_dstAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstAddr", None))
        self.lineEdit_installcode.setText("")
        self.lineEdit_installcode.setPlaceholderText(QCoreApplication.translate("MainWindow", u"install code", None))
        self.pushButton_BDBdongleMode.setText(QCoreApplication.translate("MainWindow", u"dongle working mode set", None))
        self.comboBox_mode.setItemText(0, QCoreApplication.translate("MainWindow", u"GET_MAC_ADDR_MODE", None))
        self.comboBox_mode.setItemText(1, QCoreApplication.translate("MainWindow", u"NORMAL_MODE", None))

        self.pushButton_BDBnodeDelete.setText(QCoreApplication.translate("MainWindow", u"node delete", None))
#if QT_CONFIG(accessibility)
        self.lineEdit_macAddr.setAccessibleName("")
#endif // QT_CONFIG(accessibility)
        self.lineEdit_macAddr.setInputMask("")
        self.lineEdit_macAddr.setText("")
        self.lineEdit_macAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"macAddr", None))
        self.pushButton_BDBsetTXPower.setText(QCoreApplication.translate("MainWindow", u"set TX power", None))
        self.lineEdit_TxIndex.setPlaceholderText(QCoreApplication.translate("MainWindow", u"index of tx power list", None))
        self.pushButton_getLocalDevInfo.setText(QCoreApplication.translate("MainWindow", u"get local network info", None))
        self.lineEdit_srcEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"srcEp", None))
        self.lineEdit_clusterId.setPlaceholderText(QCoreApplication.translate("MainWindow", u"clusterID", None))
        self.lineEdit_dstEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"dstEp", None))
        self.checkBox_autobind.setText(QCoreApplication.translate("MainWindow", u"auto bind when recv devannce", None))
        self.label_bindConfigState.setText("")
        self.label_bindFailList.setText("")
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.bdb), QCoreApplication.translate("MainWindow", u"BDB", None))
        self.pushButton_getJoinedNodes.setText(QCoreApplication.translate("MainWindow", u"get joined nodes", None))
        self.pushButton_on.setText(QCoreApplication.translate("MainWindow", u"on", None))
        self.pushButton_off.setText(QCoreApplication.translate("MainWindow", u"off", None))
        self.pushButton_leave.setText(QCoreApplication.translate("MainWindow", u"leave", None))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.nodesMgmt), QCoreApplication.translate("MainWindow", u"nodesMGMT", None))
        self.pushButton_permitJoin.setText(QCoreApplication.translate("MainWindow", u"permit join", None))
#if QT_CONFIG(accessibility)
        self.lineEdit_permit_dstAddr.setAccessibleName("")
#endif // QT_CONFIG(accessibility)
        self.lineEdit_permit_dstAddr.setInputMask("")
        self.lineEdit_permit_dstAddr.setText("")
        self.lineEdit_permit_dstAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstAddr:broadcast or unicast", None))
        self.lineEdit_interval.setInputMask("")
        self.lineEdit_interval.setText("")
        self.lineEdit_interval.setPlaceholderText(QCoreApplication.translate("MainWindow", u"interval", None))
        self.pushButton_ieeeReq.setText(QCoreApplication.translate("MainWindow", u"ieee req", None))
#if QT_CONFIG(accessibility)
        self.lineEdit_ieeeReqdstAddr.setAccessibleName("")
#endif // QT_CONFIG(accessibility)
        self.lineEdit_ieeeReqdstAddr.setInputMask("")
        self.lineEdit_ieeeReqdstAddr.setText("")
        self.lineEdit_ieeeReqdstAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstAddr:broadcast or unicast", None))
        self.lineEdit_ieeeReqInterestAddr.setInputMask("")
        self.lineEdit_ieeeReqInterestAddr.setText("")
        self.lineEdit_ieeeReqInterestAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"nwkAddrOfInterest", None))
        self.comboBox_ieeeReqType.setItemText(0, QCoreApplication.translate("MainWindow", u"single", None))
        self.comboBox_ieeeReqType.setItemText(1, QCoreApplication.translate("MainWindow", u"extended", None))

        self.lineEdit_ieeeReqStartIdx.setPlaceholderText(QCoreApplication.translate("MainWindow", u"startIndex", None))
        self.pushButton_nwkReq.setText(QCoreApplication.translate("MainWindow", u"nwkAddr req", None))
#if QT_CONFIG(accessibility)
        self.lineEdit_nwkReqDstAddr.setAccessibleName("")
#endif // QT_CONFIG(accessibility)
        self.lineEdit_nwkReqDstAddr.setInputMask("")
        self.lineEdit_nwkReqDstAddr.setText("")
        self.lineEdit_nwkReqDstAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstAddr:broadcast or unicast", None))
        self.lineEdit_nwkReqInterestAddr.setInputMask("")
        self.lineEdit_nwkReqInterestAddr.setText("")
        self.lineEdit_nwkReqInterestAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"ieeeAddrOfInterest", None))
        self.comboBox_nwkReqReqtype.setItemText(0, QCoreApplication.translate("MainWindow", u"single", None))
        self.comboBox_nwkReqReqtype.setItemText(1, QCoreApplication.translate("MainWindow", u"extended", None))

        self.lineEdit_nwkReqStartIdx.setPlaceholderText(QCoreApplication.translate("MainWindow", u"startIndex", None))
        self.pushButton_activeReq.setText(QCoreApplication.translate("MainWindow", u"active ep req", None))
#if QT_CONFIG(accessibility)
        self.lineEdit_activeReqDstAddr.setAccessibleName("")
#endif // QT_CONFIG(accessibility)
        self.lineEdit_activeReqDstAddr.setInputMask("")
        self.lineEdit_activeReqDstAddr.setText("")
        self.lineEdit_activeReqDstAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstAddr:unicast", None))
        self.lineEdit_activeReqInterestAddr.setInputMask("")
        self.lineEdit_activeReqInterestAddr.setText("")
        self.lineEdit_activeReqInterestAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"nwkAddrOfInterest", None))
        self.pushButton_nodeDescReq.setText(QCoreApplication.translate("MainWindow", u"node desc req", None))
#if QT_CONFIG(accessibility)
        self.lineEdit_nodeReqDstAddr.setAccessibleName("")
#endif // QT_CONFIG(accessibility)
        self.lineEdit_nodeReqDstAddr.setInputMask("")
        self.lineEdit_nodeReqDstAddr.setText("")
        self.lineEdit_nodeReqDstAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstAddr:unicast", None))
        self.lineEdit_nodeReqInterestAddr.setInputMask("")
        self.lineEdit_nodeReqInterestAddr.setText("")
        self.lineEdit_nodeReqInterestAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"nwkAddrOfInterest", None))
        self.pushButton_simpleDescReq.setText(QCoreApplication.translate("MainWindow", u"simple desc req", None))
#if QT_CONFIG(accessibility)
        self.lineEdit_simpleReqDstAddr.setAccessibleName("")
#endif // QT_CONFIG(accessibility)
        self.lineEdit_simpleReqDstAddr.setInputMask("")
        self.lineEdit_simpleReqDstAddr.setText("")
        self.lineEdit_simpleReqDstAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstAddr:unicast", None))
        self.lineEdit_simpleReqInterestAddr.setInputMask("")
        self.lineEdit_simpleReqInterestAddr.setText("")
        self.lineEdit_simpleReqInterestAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"nwkAddrOfInterest", None))
        self.lineEdit_simpleReqEndpoint.setInputMask("")
        self.lineEdit_simpleReqEndpoint.setText("")
        self.lineEdit_simpleReqEndpoint.setPlaceholderText(QCoreApplication.translate("MainWindow", u"ep", None))
        self.pushButton_matchDescReq.setText(QCoreApplication.translate("MainWindow", u"match desc req", None))
#if QT_CONFIG(accessibility)
        self.lineEdit_matchReqDstAddr.setAccessibleName("")
#endif // QT_CONFIG(accessibility)
        self.lineEdit_matchReqDstAddr.setInputMask("")
        self.lineEdit_matchReqDstAddr.setText("")
        self.lineEdit_matchReqDstAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstAddr:broadcast or unicast", None))
#if QT_CONFIG(accessibility)
        self.lineEdit_matchReqInterestAddr.setAccessibleName("")
#endif // QT_CONFIG(accessibility)
        self.lineEdit_matchReqInterestAddr.setInputMask("")
        self.lineEdit_matchReqInterestAddr.setText("")
        self.lineEdit_matchReqInterestAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"nwkAddrOfInterest", None))
        self.lineEdit_matchReqProfileId.setInputMask("")
        self.lineEdit_matchReqProfileId.setText("")
        self.lineEdit_matchReqProfileId.setPlaceholderText(QCoreApplication.translate("MainWindow", u"profileID", None))
        self.lineEdit_matchReqInCluster.setInputMask("")
        self.lineEdit_matchReqInCluster.setText("")
        self.lineEdit_matchReqInCluster.setPlaceholderText(QCoreApplication.translate("MainWindow", u"InClusterList", None))
        self.lineEdit_matchReqOutCluster.setInputMask("")
        self.lineEdit_matchReqOutCluster.setText("")
        self.lineEdit_matchReqOutCluster.setPlaceholderText(QCoreApplication.translate("MainWindow", u"OutClusterList", None))
        self.pushButton_bindReq.setText(QCoreApplication.translate("MainWindow", u"bind req", None))
#if QT_CONFIG(accessibility)
        self.lineEdit_bindReqSrcAddr.setAccessibleName("")
#endif // QT_CONFIG(accessibility)
        self.lineEdit_bindReqSrcAddr.setInputMask("")
        self.lineEdit_bindReqSrcAddr.setText("")
        self.lineEdit_bindReqSrcAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"SrcAddr:ieee", None))
        self.lineEdit_bindReqSrcEp.setInputMask("")
        self.lineEdit_bindReqSrcEp.setText("")
        self.lineEdit_bindReqSrcEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"SrcEp", None))
        self.lineEdit_bindReqClusterId.setInputMask("")
        self.lineEdit_bindReqClusterId.setText("")
        self.lineEdit_bindReqClusterId.setPlaceholderText(QCoreApplication.translate("MainWindow", u"clusterID", None))
        self.comboBox_bindReqDstMode.setItemText(0, QCoreApplication.translate("MainWindow", u"group", None))
        self.comboBox_bindReqDstMode.setItemText(1, QCoreApplication.translate("MainWindow", u"extAddr", None))

#if QT_CONFIG(accessibility)
        self.lineEdit_bindReqDstAddr.setAccessibleName("")
#endif // QT_CONFIG(accessibility)
        self.lineEdit_bindReqDstAddr.setInputMask("")
        self.lineEdit_bindReqDstAddr.setText("")
        self.lineEdit_bindReqDstAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstAddr", None))
        self.lineEdit_bindReqDstEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstEp", None))
        self.pushButton_unbindReq.setText(QCoreApplication.translate("MainWindow", u"unbind req", None))
#if QT_CONFIG(accessibility)
        self.lineEdit_unbindReqSrcAddr.setAccessibleName("")
#endif // QT_CONFIG(accessibility)
        self.lineEdit_unbindReqSrcAddr.setInputMask("")
        self.lineEdit_unbindReqSrcAddr.setText("")
        self.lineEdit_unbindReqSrcAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"SrcAddr:ieee", None))
        self.lineEdit_unbindReqSrcEp.setInputMask("")
        self.lineEdit_unbindReqSrcEp.setText("")
        self.lineEdit_unbindReqSrcEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"SrcEp", None))
        self.lineEdit_unbindReqClusterId.setInputMask("")
        self.lineEdit_unbindReqClusterId.setText("")
        self.lineEdit_unbindReqClusterId.setPlaceholderText(QCoreApplication.translate("MainWindow", u"clusterID", None))
        self.comboBox_unbindReqDstMode.setItemText(0, QCoreApplication.translate("MainWindow", u"group", None))
        self.comboBox_unbindReqDstMode.setItemText(1, QCoreApplication.translate("MainWindow", u"extAddr", None))

#if QT_CONFIG(accessibility)
        self.lineEdit_unbindReqDstAddr.setAccessibleName("")
#endif // QT_CONFIG(accessibility)
        self.lineEdit_unbindReqDstAddr.setInputMask("")
        self.lineEdit_unbindReqDstAddr.setText("")
        self.lineEdit_unbindReqDstAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstAddr", None))
        self.lineEdit_unbindReqDstEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstEp", None))
        self.pushButton_lqiReq.setText(QCoreApplication.translate("MainWindow", u"MGMT LQI req", None))
#if QT_CONFIG(accessibility)
        self.lineEdit_lqiReqDstAddr.setAccessibleName("")
#endif // QT_CONFIG(accessibility)
        self.lineEdit_lqiReqDstAddr.setInputMask("")
        self.lineEdit_lqiReqDstAddr.setText("")
        self.lineEdit_lqiReqDstAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstAddr:unicast", None))
        self.lineEdit_lqiReqStartIdx.setInputMask("")
        self.lineEdit_lqiReqStartIdx.setText("")
        self.lineEdit_lqiReqStartIdx.setPlaceholderText(QCoreApplication.translate("MainWindow", u"startIndex", None))
        self.pushButton_mgmtBindReq.setText(QCoreApplication.translate("MainWindow", u"MGMT bind req", None))
#if QT_CONFIG(accessibility)
        self.lineEdit_mgmtBindReqDstAddr.setAccessibleName("")
#endif // QT_CONFIG(accessibility)
        self.lineEdit_mgmtBindReqDstAddr.setInputMask("")
        self.lineEdit_mgmtBindReqDstAddr.setText("")
        self.lineEdit_mgmtBindReqDstAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstAddr:unicast", None))
        self.lineEdit_mgmtBindReqStartIdx.setPlaceholderText(QCoreApplication.translate("MainWindow", u"startIndex", None))
        self.pushButton_mgmtLeaveReq.setText(QCoreApplication.translate("MainWindow", u"MGMT leave req", None))
#if QT_CONFIG(accessibility)
        self.lineEdit_mgmtLeaveReqDstAddr.setAccessibleName("")
#endif // QT_CONFIG(accessibility)
        self.lineEdit_mgmtLeaveReqDstAddr.setInputMask("")
        self.lineEdit_mgmtLeaveReqDstAddr.setText("")
        self.lineEdit_mgmtLeaveReqDstAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstAddr:unicast", None))
        self.lineEdit_leaveIeeeAddr.setInputMask("")
        self.lineEdit_leaveIeeeAddr.setText("")
        self.lineEdit_leaveIeeeAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"device addr:ieee", None))
        self.lineEdit_rejoin.setPlaceholderText(QCoreApplication.translate("MainWindow", u"rejoin:0/1", None))
        self.lineEdit_removeChild.setPlaceholderText(QCoreApplication.translate("MainWindow", u"remove children:0/1", None))
        self.pushButton_nwkUpdateReq.setText(QCoreApplication.translate("MainWindow", u"MGMT nwk update req", None))
#if QT_CONFIG(accessibility)
        self.lineEdit_nwkUpdateReqDstAddr.setAccessibleName("")
#endif // QT_CONFIG(accessibility)
        self.lineEdit_nwkUpdateReqDstAddr.setInputMask("")
        self.lineEdit_nwkUpdateReqDstAddr.setText("")
        self.lineEdit_nwkUpdateReqDstAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstAddr:broadcast or unicast", None))
        self.lineEdit_nwkUpdateReqMgtAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"nwkManagerAddr", None))
        self.lineEdit_nwkUpdateReqChannel.setInputMask("")
        self.lineEdit_nwkUpdateReqChannel.setText("")
        self.lineEdit_nwkUpdateReqChannel.setPlaceholderText(QCoreApplication.translate("MainWindow", u"scanChannel", None))
        self.lineEdit_nwkUpdateReqDuration.setPlaceholderText(QCoreApplication.translate("MainWindow", u"scanDuration", None))
        self.lineEdit_nwkUpdateReqCount.setPlaceholderText(QCoreApplication.translate("MainWindow", u"scanCount", None))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.mgmt), QCoreApplication.translate("MainWindow", u"MGMT", None))
        self.pushButton_genRead.setText(QCoreApplication.translate("MainWindow", u"read attr", None))
        self.comboBox_readAttrDstMode.setItemText(0, QCoreApplication.translate("MainWindow", u"bound", None))
        self.comboBox_readAttrDstMode.setItemText(1, QCoreApplication.translate("MainWindow", u"group", None))
        self.comboBox_readAttrDstMode.setItemText(2, QCoreApplication.translate("MainWindow", u"short", None))
        self.comboBox_readAttrDstMode.setItemText(3, QCoreApplication.translate("MainWindow", u"ieee", None))
        self.comboBox_readAttrDstMode.setItemText(4, QCoreApplication.translate("MainWindow", u"broadcast", None))
        self.comboBox_readAttrDstMode.setItemText(5, QCoreApplication.translate("MainWindow", u"bound_no_ack", None))
        self.comboBox_readAttrDstMode.setItemText(6, QCoreApplication.translate("MainWindow", u"short_no_ack", None))
        self.comboBox_readAttrDstMode.setItemText(7, QCoreApplication.translate("MainWindow", u"ieee_no_ack", None))

        self.comboBox_readAttrDstMode.setCurrentText(QCoreApplication.translate("MainWindow", u"short", None))
#if QT_CONFIG(accessibility)
        self.lineEdit_readAttrDstAddr.setAccessibleName("")
#endif // QT_CONFIG(accessibility)
        self.lineEdit_readAttrDstAddr.setInputMask("")
        self.lineEdit_readAttrDstAddr.setText("")
        self.lineEdit_readAttrDstAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstAddr", None))
        self.lineEdit_readAttrSrcEp.setInputMask("")
        self.lineEdit_readAttrSrcEp.setText("")
        self.lineEdit_readAttrSrcEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"SrcEp", None))
        self.lineEdit_readAttrDstEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstEp", None))
        self.lineEdit_readAttrProfileId.setText("")
        self.lineEdit_readAttrProfileId.setPlaceholderText(QCoreApplication.translate("MainWindow", u"ProfileID", None))
        self.comboBox_readAttrDirect.setItemText(0, QCoreApplication.translate("MainWindow", u"to server", None))
        self.comboBox_readAttrDirect.setItemText(1, QCoreApplication.translate("MainWindow", u"to client", None))

        self.lineEdit_readAttrClusterId.setText("")
        self.lineEdit_readAttrClusterId.setPlaceholderText(QCoreApplication.translate("MainWindow", u"ClusterID", None))
        self.lineEdit_readAttrAttrId.setText("")
        self.lineEdit_readAttrAttrId.setPlaceholderText(QCoreApplication.translate("MainWindow", u"AttrID", None))
        self.pushButton_writeAttr.setText(QCoreApplication.translate("MainWindow", u"write attr", None))
        self.comboBox_writeAttDstMode.setItemText(0, QCoreApplication.translate("MainWindow", u"bound", None))
        self.comboBox_writeAttDstMode.setItemText(1, QCoreApplication.translate("MainWindow", u"group", None))
        self.comboBox_writeAttDstMode.setItemText(2, QCoreApplication.translate("MainWindow", u"short", None))
        self.comboBox_writeAttDstMode.setItemText(3, QCoreApplication.translate("MainWindow", u"ieee", None))
        self.comboBox_writeAttDstMode.setItemText(4, QCoreApplication.translate("MainWindow", u"broadcast", None))
        self.comboBox_writeAttDstMode.setItemText(5, QCoreApplication.translate("MainWindow", u"bound_no_ack", None))
        self.comboBox_writeAttDstMode.setItemText(6, QCoreApplication.translate("MainWindow", u"short_no_ack", None))
        self.comboBox_writeAttDstMode.setItemText(7, QCoreApplication.translate("MainWindow", u"ieee_no_ack", None))

        self.comboBox_writeAttDstMode.setCurrentText(QCoreApplication.translate("MainWindow", u"short", None))
#if QT_CONFIG(accessibility)
        self.lineEdit_writeAttrDstAddr.setAccessibleName("")
#endif // QT_CONFIG(accessibility)
        self.lineEdit_writeAttrDstAddr.setInputMask("")
        self.lineEdit_writeAttrDstAddr.setText("")
        self.lineEdit_writeAttrDstAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstAddr", None))
        self.lineEdit_writeAttrSrcEp.setInputMask("")
        self.lineEdit_writeAttrSrcEp.setText("")
        self.lineEdit_writeAttrSrcEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"SrcEp", None))
        self.lineEdit_writeAttrDstEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstEp", None))
        self.lineEdit_writeAttrProfileId.setText("")
        self.lineEdit_writeAttrProfileId.setPlaceholderText(QCoreApplication.translate("MainWindow", u"ProfileID", None))
        self.comboBox_writeAttrDirect.setItemText(0, QCoreApplication.translate("MainWindow", u"to server", None))
        self.comboBox_writeAttrDirect.setItemText(1, QCoreApplication.translate("MainWindow", u"to client", None))

        self.lineEdit_writeAttrClusterId.setText("")
        self.lineEdit_writeAttrClusterId.setPlaceholderText(QCoreApplication.translate("MainWindow", u"ClusterID", None))
        self.lineEdit_writeAttrAttrId.setPlaceholderText(QCoreApplication.translate("MainWindow", u"AttrID", None))
        self.comboBox_writeAttrDataType.setItemText(0, QCoreApplication.translate("MainWindow", u"no data", None))
        self.comboBox_writeAttrDataType.setItemText(1, QCoreApplication.translate("MainWindow", u"data8", None))
        self.comboBox_writeAttrDataType.setItemText(2, QCoreApplication.translate("MainWindow", u"data16", None))
        self.comboBox_writeAttrDataType.setItemText(3, QCoreApplication.translate("MainWindow", u"data24", None))
        self.comboBox_writeAttrDataType.setItemText(4, QCoreApplication.translate("MainWindow", u"data32", None))
        self.comboBox_writeAttrDataType.setItemText(5, QCoreApplication.translate("MainWindow", u"data40", None))
        self.comboBox_writeAttrDataType.setItemText(6, QCoreApplication.translate("MainWindow", u"data48", None))
        self.comboBox_writeAttrDataType.setItemText(7, QCoreApplication.translate("MainWindow", u"data56", None))
        self.comboBox_writeAttrDataType.setItemText(8, QCoreApplication.translate("MainWindow", u"data64", None))
        self.comboBox_writeAttrDataType.setItemText(9, QCoreApplication.translate("MainWindow", u"boolean", None))
        self.comboBox_writeAttrDataType.setItemText(10, QCoreApplication.translate("MainWindow", u"bitmap8", None))
        self.comboBox_writeAttrDataType.setItemText(11, QCoreApplication.translate("MainWindow", u"bitmap16", None))
        self.comboBox_writeAttrDataType.setItemText(12, QCoreApplication.translate("MainWindow", u"bitmap24", None))
        self.comboBox_writeAttrDataType.setItemText(13, QCoreApplication.translate("MainWindow", u"bitmap32", None))
        self.comboBox_writeAttrDataType.setItemText(14, QCoreApplication.translate("MainWindow", u"bitmap40", None))
        self.comboBox_writeAttrDataType.setItemText(15, QCoreApplication.translate("MainWindow", u"bitmap48", None))
        self.comboBox_writeAttrDataType.setItemText(16, QCoreApplication.translate("MainWindow", u"bitmap56", None))
        self.comboBox_writeAttrDataType.setItemText(17, QCoreApplication.translate("MainWindow", u"bitmap64", None))
        self.comboBox_writeAttrDataType.setItemText(18, QCoreApplication.translate("MainWindow", u"uint8", None))
        self.comboBox_writeAttrDataType.setItemText(19, QCoreApplication.translate("MainWindow", u"uint16", None))
        self.comboBox_writeAttrDataType.setItemText(20, QCoreApplication.translate("MainWindow", u"uint24", None))
        self.comboBox_writeAttrDataType.setItemText(21, QCoreApplication.translate("MainWindow", u"uint32", None))
        self.comboBox_writeAttrDataType.setItemText(22, QCoreApplication.translate("MainWindow", u"uint40", None))
        self.comboBox_writeAttrDataType.setItemText(23, QCoreApplication.translate("MainWindow", u"uint48", None))
        self.comboBox_writeAttrDataType.setItemText(24, QCoreApplication.translate("MainWindow", u"uint56", None))
        self.comboBox_writeAttrDataType.setItemText(25, QCoreApplication.translate("MainWindow", u"uint64", None))
        self.comboBox_writeAttrDataType.setItemText(26, QCoreApplication.translate("MainWindow", u"int8", None))
        self.comboBox_writeAttrDataType.setItemText(27, QCoreApplication.translate("MainWindow", u"int16", None))
        self.comboBox_writeAttrDataType.setItemText(28, QCoreApplication.translate("MainWindow", u"int24", None))
        self.comboBox_writeAttrDataType.setItemText(29, QCoreApplication.translate("MainWindow", u"int32", None))
        self.comboBox_writeAttrDataType.setItemText(30, QCoreApplication.translate("MainWindow", u"int40", None))
        self.comboBox_writeAttrDataType.setItemText(31, QCoreApplication.translate("MainWindow", u"int48", None))
        self.comboBox_writeAttrDataType.setItemText(32, QCoreApplication.translate("MainWindow", u"int56", None))
        self.comboBox_writeAttrDataType.setItemText(33, QCoreApplication.translate("MainWindow", u"int64", None))
        self.comboBox_writeAttrDataType.setItemText(34, QCoreApplication.translate("MainWindow", u"enum8", None))
        self.comboBox_writeAttrDataType.setItemText(35, QCoreApplication.translate("MainWindow", u"enum16", None))
        self.comboBox_writeAttrDataType.setItemText(36, QCoreApplication.translate("MainWindow", u"semi prec", None))
        self.comboBox_writeAttrDataType.setItemText(37, QCoreApplication.translate("MainWindow", u"single prec", None))
        self.comboBox_writeAttrDataType.setItemText(38, QCoreApplication.translate("MainWindow", u"double prec", None))
        self.comboBox_writeAttrDataType.setItemText(39, QCoreApplication.translate("MainWindow", u"octet str", None))
        self.comboBox_writeAttrDataType.setItemText(40, QCoreApplication.translate("MainWindow", u"char str", None))
        self.comboBox_writeAttrDataType.setItemText(41, QCoreApplication.translate("MainWindow", u"long octet str", None))
        self.comboBox_writeAttrDataType.setItemText(42, QCoreApplication.translate("MainWindow", u"long char str", None))
        self.comboBox_writeAttrDataType.setItemText(43, QCoreApplication.translate("MainWindow", u"array", None))
        self.comboBox_writeAttrDataType.setItemText(44, QCoreApplication.translate("MainWindow", u"struct", None))
        self.comboBox_writeAttrDataType.setItemText(45, QCoreApplication.translate("MainWindow", u"set", None))
        self.comboBox_writeAttrDataType.setItemText(46, QCoreApplication.translate("MainWindow", u"bag", None))
        self.comboBox_writeAttrDataType.setItemText(47, QCoreApplication.translate("MainWindow", u"tod", None))
        self.comboBox_writeAttrDataType.setItemText(48, QCoreApplication.translate("MainWindow", u"date", None))
        self.comboBox_writeAttrDataType.setItemText(49, QCoreApplication.translate("MainWindow", u"utc", None))
        self.comboBox_writeAttrDataType.setItemText(50, QCoreApplication.translate("MainWindow", u"cluster id", None))
        self.comboBox_writeAttrDataType.setItemText(51, QCoreApplication.translate("MainWindow", u"attr id", None))
        self.comboBox_writeAttrDataType.setItemText(52, QCoreApplication.translate("MainWindow", u"bac oid", None))
        self.comboBox_writeAttrDataType.setItemText(53, QCoreApplication.translate("MainWindow", u"ieee addr", None))
        self.comboBox_writeAttrDataType.setItemText(54, QCoreApplication.translate("MainWindow", u"128 bit sec key", None))
        self.comboBox_writeAttrDataType.setItemText(55, QCoreApplication.translate("MainWindow", u"unknown", None))

        self.lineEdit_writeAttrData.setPlaceholderText(QCoreApplication.translate("MainWindow", u"data(format:hex)", None))
        self.pushButton_readReportCfg.setText(QCoreApplication.translate("MainWindow", u"read report config", None))
        self.comboBox_readReportCfgDstMode.setItemText(0, QCoreApplication.translate("MainWindow", u"bound", None))
        self.comboBox_readReportCfgDstMode.setItemText(1, QCoreApplication.translate("MainWindow", u"group", None))
        self.comboBox_readReportCfgDstMode.setItemText(2, QCoreApplication.translate("MainWindow", u"short", None))
        self.comboBox_readReportCfgDstMode.setItemText(3, QCoreApplication.translate("MainWindow", u"ieee", None))
        self.comboBox_readReportCfgDstMode.setItemText(4, QCoreApplication.translate("MainWindow", u"broadcast", None))
        self.comboBox_readReportCfgDstMode.setItemText(5, QCoreApplication.translate("MainWindow", u"bound_no_ack", None))
        self.comboBox_readReportCfgDstMode.setItemText(6, QCoreApplication.translate("MainWindow", u"short_no_ack", None))
        self.comboBox_readReportCfgDstMode.setItemText(7, QCoreApplication.translate("MainWindow", u"ieee_no_ack", None))

        self.comboBox_readReportCfgDstMode.setCurrentText(QCoreApplication.translate("MainWindow", u"short", None))
#if QT_CONFIG(accessibility)
        self.lineEdit_readReportCfgDstAddr.setAccessibleName("")
#endif // QT_CONFIG(accessibility)
        self.lineEdit_readReportCfgDstAddr.setInputMask("")
        self.lineEdit_readReportCfgDstAddr.setText("")
        self.lineEdit_readReportCfgDstAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstAddr", None))
        self.lineEdit_readReportCfgSrcEp.setInputMask("")
        self.lineEdit_readReportCfgSrcEp.setText("")
        self.lineEdit_readReportCfgSrcEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"SrcEp", None))
        self.lineEdit_readReportCfgDstEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstEp", None))
        self.lineEdit_readReportCfgProfileId.setText("")
        self.lineEdit_readReportCfgProfileId.setPlaceholderText(QCoreApplication.translate("MainWindow", u"ProfileID", None))
        self.comboBox_readReportCfgDir.setItemText(0, QCoreApplication.translate("MainWindow", u"to server", None))
        self.comboBox_readReportCfgDir.setItemText(1, QCoreApplication.translate("MainWindow", u"to client", None))

        self.lineEdit_readReportCfgClusterId.setText("")
        self.lineEdit_readReportCfgClusterId.setPlaceholderText(QCoreApplication.translate("MainWindow", u"ClusterID", None))
        self.comboBox_readReportCfgType.setItemText(0, QCoreApplication.translate("MainWindow", u"send attr report", None))
        self.comboBox_readReportCfgType.setItemText(1, QCoreApplication.translate("MainWindow", u"expect attr report", None))

        self.lineEdit_readReportCfgAttrId.setPlaceholderText(QCoreApplication.translate("MainWindow", u"AttrID", None))
        self.pushButton_genReset.setText(QCoreApplication.translate("MainWindow", u"reset", None))
        self.comboBox_resetDstMode.setItemText(0, QCoreApplication.translate("MainWindow", u"bound", None))
        self.comboBox_resetDstMode.setItemText(1, QCoreApplication.translate("MainWindow", u"group", None))
        self.comboBox_resetDstMode.setItemText(2, QCoreApplication.translate("MainWindow", u"short", None))
        self.comboBox_resetDstMode.setItemText(3, QCoreApplication.translate("MainWindow", u"ieee", None))
        self.comboBox_resetDstMode.setItemText(4, QCoreApplication.translate("MainWindow", u"broadcast", None))
        self.comboBox_resetDstMode.setItemText(5, QCoreApplication.translate("MainWindow", u"bound_no_ack", None))
        self.comboBox_resetDstMode.setItemText(6, QCoreApplication.translate("MainWindow", u"short_no_ack", None))
        self.comboBox_resetDstMode.setItemText(7, QCoreApplication.translate("MainWindow", u"ieee_no_ack", None))

        self.comboBox_resetDstMode.setCurrentText(QCoreApplication.translate("MainWindow", u"short", None))
#if QT_CONFIG(accessibility)
        self.lineEdit_resetDstAddr.setAccessibleName("")
#endif // QT_CONFIG(accessibility)
        self.lineEdit_resetDstAddr.setInputMask("")
        self.lineEdit_resetDstAddr.setText("")
        self.lineEdit_resetDstAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstAddr", None))
        self.lineEdit_resetSrcEp.setInputMask("")
        self.lineEdit_resetSrcEp.setText("")
        self.lineEdit_resetSrcEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"SrcEp", None))
        self.lineEdit_resetDstEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstEp", None))
        self.pushButton_genConfig.setText(QCoreApplication.translate("MainWindow", u"config report", None))
        self.comboBox_configReportDstMode.setItemText(0, QCoreApplication.translate("MainWindow", u"bound", None))
        self.comboBox_configReportDstMode.setItemText(1, QCoreApplication.translate("MainWindow", u"group", None))
        self.comboBox_configReportDstMode.setItemText(2, QCoreApplication.translate("MainWindow", u"short", None))
        self.comboBox_configReportDstMode.setItemText(3, QCoreApplication.translate("MainWindow", u"ieee", None))
        self.comboBox_configReportDstMode.setItemText(4, QCoreApplication.translate("MainWindow", u"broadcast", None))
        self.comboBox_configReportDstMode.setItemText(5, QCoreApplication.translate("MainWindow", u"bound_no_ack", None))
        self.comboBox_configReportDstMode.setItemText(6, QCoreApplication.translate("MainWindow", u"short_no_ack", None))
        self.comboBox_configReportDstMode.setItemText(7, QCoreApplication.translate("MainWindow", u"ieee_no_ack", None))

        self.comboBox_configReportDstMode.setCurrentText(QCoreApplication.translate("MainWindow", u"short", None))
#if QT_CONFIG(accessibility)
        self.lineEdit_configReportDstAddr.setAccessibleName("")
#endif // QT_CONFIG(accessibility)
        self.lineEdit_configReportDstAddr.setInputMask("")
        self.lineEdit_configReportDstAddr.setText("")
        self.lineEdit_configReportDstAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstAddr", None))
        self.lineEdit_configReportSrcEp.setInputMask("")
        self.lineEdit_configReportSrcEp.setText("")
        self.lineEdit_configReportSrcEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"SrcEp", None))
        self.lineEdit_configReportDstEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstEp", None))
        self.lineEdit_configReportProfileId.setText("")
        self.lineEdit_configReportProfileId.setPlaceholderText(QCoreApplication.translate("MainWindow", u"ProfileID", None))
        self.comboBox_configReportDirect.setItemText(0, QCoreApplication.translate("MainWindow", u"to server", None))
        self.comboBox_configReportDirect.setItemText(1, QCoreApplication.translate("MainWindow", u"to client", None))

        self.lineEdit_configReportClusterId.setText("")
        self.lineEdit_configReportClusterId.setPlaceholderText(QCoreApplication.translate("MainWindow", u"ClusterID", None))
        self.comboBox_configReportSendType.setItemText(0, QCoreApplication.translate("MainWindow", u"send attr report", None))
        self.comboBox_configReportSendType.setItemText(1, QCoreApplication.translate("MainWindow", u"expect attr report", None))

        self.lineEdit_configReportAttrId.setPlaceholderText(QCoreApplication.translate("MainWindow", u"AttrID", None))
        self.comboBox_configReportDataType.setItemText(0, QCoreApplication.translate("MainWindow", u"no data", None))
        self.comboBox_configReportDataType.setItemText(1, QCoreApplication.translate("MainWindow", u"data8", None))
        self.comboBox_configReportDataType.setItemText(2, QCoreApplication.translate("MainWindow", u"data16", None))
        self.comboBox_configReportDataType.setItemText(3, QCoreApplication.translate("MainWindow", u"data24", None))
        self.comboBox_configReportDataType.setItemText(4, QCoreApplication.translate("MainWindow", u"data32", None))
        self.comboBox_configReportDataType.setItemText(5, QCoreApplication.translate("MainWindow", u"data40", None))
        self.comboBox_configReportDataType.setItemText(6, QCoreApplication.translate("MainWindow", u"data48", None))
        self.comboBox_configReportDataType.setItemText(7, QCoreApplication.translate("MainWindow", u"data56", None))
        self.comboBox_configReportDataType.setItemText(8, QCoreApplication.translate("MainWindow", u"data64", None))
        self.comboBox_configReportDataType.setItemText(9, QCoreApplication.translate("MainWindow", u"boolean", None))
        self.comboBox_configReportDataType.setItemText(10, QCoreApplication.translate("MainWindow", u"bitmap8", None))
        self.comboBox_configReportDataType.setItemText(11, QCoreApplication.translate("MainWindow", u"bitmap16", None))
        self.comboBox_configReportDataType.setItemText(12, QCoreApplication.translate("MainWindow", u"bitmap24", None))
        self.comboBox_configReportDataType.setItemText(13, QCoreApplication.translate("MainWindow", u"bitmap32", None))
        self.comboBox_configReportDataType.setItemText(14, QCoreApplication.translate("MainWindow", u"bitmap40", None))
        self.comboBox_configReportDataType.setItemText(15, QCoreApplication.translate("MainWindow", u"bitmap48", None))
        self.comboBox_configReportDataType.setItemText(16, QCoreApplication.translate("MainWindow", u"bitmap56", None))
        self.comboBox_configReportDataType.setItemText(17, QCoreApplication.translate("MainWindow", u"bitmap64", None))
        self.comboBox_configReportDataType.setItemText(18, QCoreApplication.translate("MainWindow", u"uint8", None))
        self.comboBox_configReportDataType.setItemText(19, QCoreApplication.translate("MainWindow", u"uint16", None))
        self.comboBox_configReportDataType.setItemText(20, QCoreApplication.translate("MainWindow", u"uint24", None))
        self.comboBox_configReportDataType.setItemText(21, QCoreApplication.translate("MainWindow", u"uint32", None))
        self.comboBox_configReportDataType.setItemText(22, QCoreApplication.translate("MainWindow", u"uint40", None))
        self.comboBox_configReportDataType.setItemText(23, QCoreApplication.translate("MainWindow", u"uint48", None))
        self.comboBox_configReportDataType.setItemText(24, QCoreApplication.translate("MainWindow", u"uint56", None))
        self.comboBox_configReportDataType.setItemText(25, QCoreApplication.translate("MainWindow", u"uint64", None))
        self.comboBox_configReportDataType.setItemText(26, QCoreApplication.translate("MainWindow", u"int8", None))
        self.comboBox_configReportDataType.setItemText(27, QCoreApplication.translate("MainWindow", u"int16", None))
        self.comboBox_configReportDataType.setItemText(28, QCoreApplication.translate("MainWindow", u"int24", None))
        self.comboBox_configReportDataType.setItemText(29, QCoreApplication.translate("MainWindow", u"int32", None))
        self.comboBox_configReportDataType.setItemText(30, QCoreApplication.translate("MainWindow", u"int40", None))
        self.comboBox_configReportDataType.setItemText(31, QCoreApplication.translate("MainWindow", u"int48", None))
        self.comboBox_configReportDataType.setItemText(32, QCoreApplication.translate("MainWindow", u"int56", None))
        self.comboBox_configReportDataType.setItemText(33, QCoreApplication.translate("MainWindow", u"int64", None))
        self.comboBox_configReportDataType.setItemText(34, QCoreApplication.translate("MainWindow", u"enum8", None))
        self.comboBox_configReportDataType.setItemText(35, QCoreApplication.translate("MainWindow", u"enum16", None))
        self.comboBox_configReportDataType.setItemText(36, QCoreApplication.translate("MainWindow", u"semi prec", None))
        self.comboBox_configReportDataType.setItemText(37, QCoreApplication.translate("MainWindow", u"single prec", None))
        self.comboBox_configReportDataType.setItemText(38, QCoreApplication.translate("MainWindow", u"double prec", None))
        self.comboBox_configReportDataType.setItemText(39, QCoreApplication.translate("MainWindow", u"octet str", None))
        self.comboBox_configReportDataType.setItemText(40, QCoreApplication.translate("MainWindow", u"char str", None))
        self.comboBox_configReportDataType.setItemText(41, QCoreApplication.translate("MainWindow", u"long octet str", None))
        self.comboBox_configReportDataType.setItemText(42, QCoreApplication.translate("MainWindow", u"long char str", None))
        self.comboBox_configReportDataType.setItemText(43, QCoreApplication.translate("MainWindow", u"array", None))
        self.comboBox_configReportDataType.setItemText(44, QCoreApplication.translate("MainWindow", u"struct", None))
        self.comboBox_configReportDataType.setItemText(45, QCoreApplication.translate("MainWindow", u"set", None))
        self.comboBox_configReportDataType.setItemText(46, QCoreApplication.translate("MainWindow", u"bag", None))
        self.comboBox_configReportDataType.setItemText(47, QCoreApplication.translate("MainWindow", u"tod", None))
        self.comboBox_configReportDataType.setItemText(48, QCoreApplication.translate("MainWindow", u"date", None))
        self.comboBox_configReportDataType.setItemText(49, QCoreApplication.translate("MainWindow", u"utc", None))
        self.comboBox_configReportDataType.setItemText(50, QCoreApplication.translate("MainWindow", u"cluster id", None))
        self.comboBox_configReportDataType.setItemText(51, QCoreApplication.translate("MainWindow", u"attr id", None))
        self.comboBox_configReportDataType.setItemText(52, QCoreApplication.translate("MainWindow", u"bac oid", None))
        self.comboBox_configReportDataType.setItemText(53, QCoreApplication.translate("MainWindow", u"ieee addr", None))
        self.comboBox_configReportDataType.setItemText(54, QCoreApplication.translate("MainWindow", u"128 bit sec key", None))
        self.comboBox_configReportDataType.setItemText(55, QCoreApplication.translate("MainWindow", u"unknown", None))

        self.lineEdit_configReportMinInt.setText("")
        self.lineEdit_configReportMinInt.setPlaceholderText(QCoreApplication.translate("MainWindow", u"min Interval", None))
        self.lineEdit_configReportMaxInt.setPlaceholderText(QCoreApplication.translate("MainWindow", u"max Interval", None))
        self.lineEdit_configReportChange.setPlaceholderText(QCoreApplication.translate("MainWindow", u"change", None))
        self.lineEdit_configReportTimeOut.setInputMask("")
        self.lineEdit_configReportTimeOut.setText("")
        self.lineEdit_configReportTimeOut.setPlaceholderText(QCoreApplication.translate("MainWindow", u"timeout period", None))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.general), QCoreApplication.translate("MainWindow", u"general", None))
        self.pushButton_addGroup.setText(QCoreApplication.translate("MainWindow", u"add group", None))
        self.comboBox_addGroupDstmode.setItemText(0, QCoreApplication.translate("MainWindow", u"bound", None))
        self.comboBox_addGroupDstmode.setItemText(1, QCoreApplication.translate("MainWindow", u"group", None))
        self.comboBox_addGroupDstmode.setItemText(2, QCoreApplication.translate("MainWindow", u"short", None))
        self.comboBox_addGroupDstmode.setItemText(3, QCoreApplication.translate("MainWindow", u"ieee", None))
        self.comboBox_addGroupDstmode.setItemText(4, QCoreApplication.translate("MainWindow", u"broadcast", None))
        self.comboBox_addGroupDstmode.setItemText(5, QCoreApplication.translate("MainWindow", u"bound_no_ack", None))
        self.comboBox_addGroupDstmode.setItemText(6, QCoreApplication.translate("MainWindow", u"short_no_ack", None))
        self.comboBox_addGroupDstmode.setItemText(7, QCoreApplication.translate("MainWindow", u"ieee_no_ack", None))

        self.comboBox_addGroupDstmode.setCurrentText(QCoreApplication.translate("MainWindow", u"short", None))
#if QT_CONFIG(accessibility)
        self.lineEdit_addGroupDstAddr.setAccessibleName("")
#endif // QT_CONFIG(accessibility)
        self.lineEdit_addGroupDstAddr.setInputMask("")
        self.lineEdit_addGroupDstAddr.setText("")
        self.lineEdit_addGroupDstAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstAddr", None))
        self.lineEdit_addGroupSrcEp.setInputMask("")
        self.lineEdit_addGroupSrcEp.setText("")
        self.lineEdit_addGroupSrcEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"SrcEp", None))
        self.lineEdit_addGroupDstEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstEp", None))
        self.lineEdit_addGroupGroupId.setText("")
        self.lineEdit_addGroupGroupId.setPlaceholderText(QCoreApplication.translate("MainWindow", u"groupID", None))
        self.lineEdit_addGroupName.setText("")
        self.lineEdit_addGroupName.setPlaceholderText(QCoreApplication.translate("MainWindow", u"groupName", None))
        self.pushButton_removeGroup.setText(QCoreApplication.translate("MainWindow", u"remove group", None))
        self.comboBox_rmGroupDstMode.setItemText(0, QCoreApplication.translate("MainWindow", u"bound", None))
        self.comboBox_rmGroupDstMode.setItemText(1, QCoreApplication.translate("MainWindow", u"group", None))
        self.comboBox_rmGroupDstMode.setItemText(2, QCoreApplication.translate("MainWindow", u"short", None))
        self.comboBox_rmGroupDstMode.setItemText(3, QCoreApplication.translate("MainWindow", u"ieee", None))
        self.comboBox_rmGroupDstMode.setItemText(4, QCoreApplication.translate("MainWindow", u"broadcast", None))
        self.comboBox_rmGroupDstMode.setItemText(5, QCoreApplication.translate("MainWindow", u"bound_no_ack", None))
        self.comboBox_rmGroupDstMode.setItemText(6, QCoreApplication.translate("MainWindow", u"short_no_ack", None))
        self.comboBox_rmGroupDstMode.setItemText(7, QCoreApplication.translate("MainWindow", u"ieee_no_ack", None))

        self.comboBox_rmGroupDstMode.setCurrentText(QCoreApplication.translate("MainWindow", u"short", None))
#if QT_CONFIG(accessibility)
        self.lineEdit_rmGroupDstAddr.setAccessibleName("")
#endif // QT_CONFIG(accessibility)
        self.lineEdit_rmGroupDstAddr.setInputMask("")
        self.lineEdit_rmGroupDstAddr.setText("")
        self.lineEdit_rmGroupDstAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstAddr", None))
        self.lineEdit_rmGroupSrcEp.setInputMask("")
        self.lineEdit_rmGroupSrcEp.setText("")
        self.lineEdit_rmGroupSrcEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"SrcEp", None))
        self.lineEdit_rmGroupDstEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstEp", None))
        self.lineEdit_rmGroupId.setText("")
        self.lineEdit_rmGroupId.setPlaceholderText(QCoreApplication.translate("MainWindow", u"groupID", None))
        self.pushButton_getGroup.setText(QCoreApplication.translate("MainWindow", u"get group", None))
        self.comboBox_getGroupDstMode.setItemText(0, QCoreApplication.translate("MainWindow", u"bound", None))
        self.comboBox_getGroupDstMode.setItemText(1, QCoreApplication.translate("MainWindow", u"group", None))
        self.comboBox_getGroupDstMode.setItemText(2, QCoreApplication.translate("MainWindow", u"short", None))
        self.comboBox_getGroupDstMode.setItemText(3, QCoreApplication.translate("MainWindow", u"ieee", None))
        self.comboBox_getGroupDstMode.setItemText(4, QCoreApplication.translate("MainWindow", u"broadcast", None))
        self.comboBox_getGroupDstMode.setItemText(5, QCoreApplication.translate("MainWindow", u"bound_no_ack", None))
        self.comboBox_getGroupDstMode.setItemText(6, QCoreApplication.translate("MainWindow", u"short_no_ack", None))
        self.comboBox_getGroupDstMode.setItemText(7, QCoreApplication.translate("MainWindow", u"ieee_no_ack", None))

        self.comboBox_getGroupDstMode.setCurrentText(QCoreApplication.translate("MainWindow", u"short", None))
#if QT_CONFIG(accessibility)
        self.lineEdit_getGroupDstAddr.setAccessibleName("")
#endif // QT_CONFIG(accessibility)
        self.lineEdit_getGroupDstAddr.setInputMask("")
        self.lineEdit_getGroupDstAddr.setText("")
        self.lineEdit_getGroupDstAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstAddr", None))
        self.lineEdit_getGroupSrcEp.setInputMask("")
        self.lineEdit_getGroupSrcEp.setText("")
        self.lineEdit_getGroupSrcEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"SrcEp", None))
        self.lineEdit_getGroupDstEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstEp", None))
        self.lineEdit_getGroupList.setPlaceholderText(QCoreApplication.translate("MainWindow", u"groupList", None))
        self.pushButton_viewGroup.setText(QCoreApplication.translate("MainWindow", u"view group", None))
        self.comboBox_viewGroupDstMode.setItemText(0, QCoreApplication.translate("MainWindow", u"bound", None))
        self.comboBox_viewGroupDstMode.setItemText(1, QCoreApplication.translate("MainWindow", u"group", None))
        self.comboBox_viewGroupDstMode.setItemText(2, QCoreApplication.translate("MainWindow", u"short", None))
        self.comboBox_viewGroupDstMode.setItemText(3, QCoreApplication.translate("MainWindow", u"ieee", None))
        self.comboBox_viewGroupDstMode.setItemText(4, QCoreApplication.translate("MainWindow", u"broadcast", None))
        self.comboBox_viewGroupDstMode.setItemText(5, QCoreApplication.translate("MainWindow", u"bound_no_ack", None))
        self.comboBox_viewGroupDstMode.setItemText(6, QCoreApplication.translate("MainWindow", u"short_no_ack", None))
        self.comboBox_viewGroupDstMode.setItemText(7, QCoreApplication.translate("MainWindow", u"ieee_no_ack", None))

        self.comboBox_viewGroupDstMode.setCurrentText(QCoreApplication.translate("MainWindow", u"short", None))
#if QT_CONFIG(accessibility)
        self.lineEdit_viewGroupDstAddr.setAccessibleName("")
#endif // QT_CONFIG(accessibility)
        self.lineEdit_viewGroupDstAddr.setInputMask("")
        self.lineEdit_viewGroupDstAddr.setText("")
        self.lineEdit_viewGroupDstAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstAddr", None))
        self.lineEdit_viewGroupSrcEp.setInputMask("")
        self.lineEdit_viewGroupSrcEp.setText("")
        self.lineEdit_viewGroupSrcEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"SrcEp", None))
        self.lineEdit_viewGroupDstEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstEp", None))
        self.lineEdit_viewGroupGroupId.setText("")
        self.lineEdit_viewGroupGroupId.setPlaceholderText(QCoreApplication.translate("MainWindow", u"groupID", None))
        self.pushButton_addIfIdentify.setText(QCoreApplication.translate("MainWindow", u"add group if identifying", None))
        self.comboBox_addIdenDstMode.setItemText(0, QCoreApplication.translate("MainWindow", u"bound", None))
        self.comboBox_addIdenDstMode.setItemText(1, QCoreApplication.translate("MainWindow", u"group", None))
        self.comboBox_addIdenDstMode.setItemText(2, QCoreApplication.translate("MainWindow", u"short", None))
        self.comboBox_addIdenDstMode.setItemText(3, QCoreApplication.translate("MainWindow", u"ieee", None))
        self.comboBox_addIdenDstMode.setItemText(4, QCoreApplication.translate("MainWindow", u"broadcast", None))
        self.comboBox_addIdenDstMode.setItemText(5, QCoreApplication.translate("MainWindow", u"bound_no_ack", None))
        self.comboBox_addIdenDstMode.setItemText(6, QCoreApplication.translate("MainWindow", u"short_no_ack", None))
        self.comboBox_addIdenDstMode.setItemText(7, QCoreApplication.translate("MainWindow", u"ieee_no_ack", None))

        self.comboBox_addIdenDstMode.setCurrentText(QCoreApplication.translate("MainWindow", u"short", None))
#if QT_CONFIG(accessibility)
        self.lineEdit_addIdenDstAddr.setAccessibleName("")
#endif // QT_CONFIG(accessibility)
        self.lineEdit_addIdenDstAddr.setInputMask("")
        self.lineEdit_addIdenDstAddr.setText("")
        self.lineEdit_addIdenDstAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstAddr", None))
        self.lineEdit_addIdenSrcEp.setInputMask("")
        self.lineEdit_addIdenSrcEp.setText("")
        self.lineEdit_addIdenSrcEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"SrcEp", None))
        self.lineEdit_addIdenDstEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstEp", None))
        self.lineEdit_addIdenGroupId.setText("")
        self.lineEdit_addIdenGroupId.setPlaceholderText(QCoreApplication.translate("MainWindow", u"groupID", None))
        self.lineEdit_addIdenName.setPlaceholderText(QCoreApplication.translate("MainWindow", u"groupName", None))
        self.pushButton_removeAll.setText(QCoreApplication.translate("MainWindow", u"remove all", None))
        self.comboBox_removeAllDstMode.setItemText(0, QCoreApplication.translate("MainWindow", u"bound", None))
        self.comboBox_removeAllDstMode.setItemText(1, QCoreApplication.translate("MainWindow", u"group", None))
        self.comboBox_removeAllDstMode.setItemText(2, QCoreApplication.translate("MainWindow", u"short", None))
        self.comboBox_removeAllDstMode.setItemText(3, QCoreApplication.translate("MainWindow", u"ieee", None))
        self.comboBox_removeAllDstMode.setItemText(4, QCoreApplication.translate("MainWindow", u"broadcast", None))
        self.comboBox_removeAllDstMode.setItemText(5, QCoreApplication.translate("MainWindow", u"bound_no_ack", None))
        self.comboBox_removeAllDstMode.setItemText(6, QCoreApplication.translate("MainWindow", u"short_no_ack", None))
        self.comboBox_removeAllDstMode.setItemText(7, QCoreApplication.translate("MainWindow", u"ieee_no_ack", None))

        self.comboBox_removeAllDstMode.setCurrentText(QCoreApplication.translate("MainWindow", u"short", None))
#if QT_CONFIG(accessibility)
        self.lineEdit_removeAllDstAddr.setAccessibleName("")
#endif // QT_CONFIG(accessibility)
        self.lineEdit_removeAllDstAddr.setInputMask("")
        self.lineEdit_removeAllDstAddr.setText("")
        self.lineEdit_removeAllDstAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstAddr", None))
        self.lineEdit_removeAllSrcEp.setInputMask("")
        self.lineEdit_removeAllSrcEp.setText("")
        self.lineEdit_removeAllSrcEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"SrcEp", None))
        self.lineEdit_removeAllDstEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstEp", None))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.tab_group), QCoreApplication.translate("MainWindow", u"group", None))
        self.pushButton_onOff.setText(QCoreApplication.translate("MainWindow", u"onOff", None))
        self.comboBox_onoffDstMode.setItemText(0, QCoreApplication.translate("MainWindow", u"bound", None))
        self.comboBox_onoffDstMode.setItemText(1, QCoreApplication.translate("MainWindow", u"group", None))
        self.comboBox_onoffDstMode.setItemText(2, QCoreApplication.translate("MainWindow", u"short", None))
        self.comboBox_onoffDstMode.setItemText(3, QCoreApplication.translate("MainWindow", u"ieee", None))
        self.comboBox_onoffDstMode.setItemText(4, QCoreApplication.translate("MainWindow", u"broadcast", None))
        self.comboBox_onoffDstMode.setItemText(5, QCoreApplication.translate("MainWindow", u"bound_no_ack", None))
        self.comboBox_onoffDstMode.setItemText(6, QCoreApplication.translate("MainWindow", u"short_no_ack", None))
        self.comboBox_onoffDstMode.setItemText(7, QCoreApplication.translate("MainWindow", u"ieee_no_ack", None))

        self.comboBox_onoffDstMode.setCurrentText(QCoreApplication.translate("MainWindow", u"short", None))
#if QT_CONFIG(accessibility)
        self.lineEdit_onoffDstAddr.setAccessibleName("")
#endif // QT_CONFIG(accessibility)
        self.lineEdit_onoffDstAddr.setInputMask("")
        self.lineEdit_onoffDstAddr.setText("")
        self.lineEdit_onoffDstAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstAddr", None))
        self.lineEdit_onoffSrcEp.setInputMask("")
        self.lineEdit_onoffSrcEp.setText("")
        self.lineEdit_onoffSrcEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"SrcEp", None))
        self.lineEdit_onoffDstEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstEp", None))
        self.comboBox_onOff.setItemText(0, QCoreApplication.translate("MainWindow", u"on", None))
        self.comboBox_onOff.setItemText(1, QCoreApplication.translate("MainWindow", u"off", None))
        self.comboBox_onOff.setItemText(2, QCoreApplication.translate("MainWindow", u"toggle", None))

        self.tabWidget.setTabText(self.tabWidget.indexOf(self.on_off), QCoreApplication.translate("MainWindow", u"onOff", None))
        self.pushButton_moveToLevel.setText(QCoreApplication.translate("MainWindow", u"move to level", None))
        self.comboBox_toLevelDstMode.setItemText(0, QCoreApplication.translate("MainWindow", u"bound", None))
        self.comboBox_toLevelDstMode.setItemText(1, QCoreApplication.translate("MainWindow", u"group", None))
        self.comboBox_toLevelDstMode.setItemText(2, QCoreApplication.translate("MainWindow", u"short", None))
        self.comboBox_toLevelDstMode.setItemText(3, QCoreApplication.translate("MainWindow", u"ieee", None))
        self.comboBox_toLevelDstMode.setItemText(4, QCoreApplication.translate("MainWindow", u"broadcast", None))
        self.comboBox_toLevelDstMode.setItemText(5, QCoreApplication.translate("MainWindow", u"bound_no_ack", None))
        self.comboBox_toLevelDstMode.setItemText(6, QCoreApplication.translate("MainWindow", u"short_no_ack", None))
        self.comboBox_toLevelDstMode.setItemText(7, QCoreApplication.translate("MainWindow", u"ieee_no_ack", None))

        self.comboBox_toLevelDstMode.setCurrentText(QCoreApplication.translate("MainWindow", u"short", None))
#if QT_CONFIG(accessibility)
        self.lineEdit_toLevelDstAddr.setAccessibleName("")
#endif // QT_CONFIG(accessibility)
        self.lineEdit_toLevelDstAddr.setInputMask("")
        self.lineEdit_toLevelDstAddr.setText("")
        self.lineEdit_toLevelDstAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstAddr", None))
        self.lineEdit_toLevelSrcEp.setInputMask("")
        self.lineEdit_toLevelSrcEp.setText("")
        self.lineEdit_toLevelSrcEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"SrcEp", None))
        self.lineEdit_toLevelDstEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstEp", None))
        self.lineEdit_toLevelLevel.setText("")
        self.lineEdit_toLevelLevel.setPlaceholderText(QCoreApplication.translate("MainWindow", u"level", None))
        self.lineEdit_toLevelTime.setText("")
        self.lineEdit_toLevelTime.setPlaceholderText(QCoreApplication.translate("MainWindow", u"Time(u16 0.1s)", None))
        self.comboBox_withOnOff.setItemText(0, QCoreApplication.translate("MainWindow", u"with onOff", None))
        self.comboBox_withOnOff.setItemText(1, QCoreApplication.translate("MainWindow", u"without onOff", None))

        self.pushButton_stop.setText(QCoreApplication.translate("MainWindow", u"stop", None))
        self.comboBox_stopDstMode.setItemText(0, QCoreApplication.translate("MainWindow", u"bound", None))
        self.comboBox_stopDstMode.setItemText(1, QCoreApplication.translate("MainWindow", u"group", None))
        self.comboBox_stopDstMode.setItemText(2, QCoreApplication.translate("MainWindow", u"short", None))
        self.comboBox_stopDstMode.setItemText(3, QCoreApplication.translate("MainWindow", u"ieee", None))
        self.comboBox_stopDstMode.setItemText(4, QCoreApplication.translate("MainWindow", u"broadcast", None))
        self.comboBox_stopDstMode.setItemText(5, QCoreApplication.translate("MainWindow", u"bound_no_ack", None))
        self.comboBox_stopDstMode.setItemText(6, QCoreApplication.translate("MainWindow", u"short_no_ack", None))
        self.comboBox_stopDstMode.setItemText(7, QCoreApplication.translate("MainWindow", u"ieee_no_ack", None))

#if QT_CONFIG(accessibility)
        self.lineEdit_stopDstAddr.setAccessibleName("")
#endif // QT_CONFIG(accessibility)
        self.lineEdit_stopDstAddr.setInputMask("")
        self.lineEdit_stopDstAddr.setText("")
        self.lineEdit_stopDstAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstAddr", None))
        self.lineEdit_stopSrcEp.setInputMask("")
        self.lineEdit_stopSrcEp.setText("")
        self.lineEdit_stopSrcEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"SrcEp", None))
        self.lineEdit_stopSrcEp_2.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstEp", None))
        self.comboBox_stopWithOnoff.setItemText(0, QCoreApplication.translate("MainWindow", u"with onOff", None))
        self.comboBox_stopWithOnoff.setItemText(1, QCoreApplication.translate("MainWindow", u"without onOff", None))

        self.pushButton_move.setText(QCoreApplication.translate("MainWindow", u"move", None))
        self.comboBox_moveDstMode.setItemText(0, QCoreApplication.translate("MainWindow", u"bound", None))
        self.comboBox_moveDstMode.setItemText(1, QCoreApplication.translate("MainWindow", u"group", None))
        self.comboBox_moveDstMode.setItemText(2, QCoreApplication.translate("MainWindow", u"short", None))
        self.comboBox_moveDstMode.setItemText(3, QCoreApplication.translate("MainWindow", u"ieee", None))
        self.comboBox_moveDstMode.setItemText(4, QCoreApplication.translate("MainWindow", u"broadcast", None))
        self.comboBox_moveDstMode.setItemText(5, QCoreApplication.translate("MainWindow", u"bound_no_ack", None))
        self.comboBox_moveDstMode.setItemText(6, QCoreApplication.translate("MainWindow", u"short_no_ack", None))
        self.comboBox_moveDstMode.setItemText(7, QCoreApplication.translate("MainWindow", u"ieee_no_ack", None))

        self.comboBox_moveDstMode.setCurrentText(QCoreApplication.translate("MainWindow", u"short", None))
#if QT_CONFIG(accessibility)
        self.lineEdit_moveDstAddr.setAccessibleName("")
#endif // QT_CONFIG(accessibility)
        self.lineEdit_moveDstAddr.setInputMask("")
        self.lineEdit_moveDstAddr.setText("")
        self.lineEdit_moveDstAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstAddr", None))
        self.lineEdit_moveSrcEp.setInputMask("")
        self.lineEdit_moveSrcEp.setText("")
        self.lineEdit_moveSrcEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"SrcEp", None))
        self.lineEdit_moveDstEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstEp", None))
        self.lineEdit_moveMode.setText("")
        self.lineEdit_moveMode.setPlaceholderText(QCoreApplication.translate("MainWindow", u"mode", None))
        self.lineEdit_moveRate.setText("")
        self.lineEdit_moveRate.setPlaceholderText(QCoreApplication.translate("MainWindow", u"rate", None))
        self.comboBox_moveWithOnoff.setItemText(0, QCoreApplication.translate("MainWindow", u"with onOff", None))
        self.comboBox_moveWithOnoff.setItemText(1, QCoreApplication.translate("MainWindow", u"without onOff", None))

        self.pushButton_step.setText(QCoreApplication.translate("MainWindow", u"step", None))
        self.comboBox_stepDstMode.setItemText(0, QCoreApplication.translate("MainWindow", u"bound", None))
        self.comboBox_stepDstMode.setItemText(1, QCoreApplication.translate("MainWindow", u"group", None))
        self.comboBox_stepDstMode.setItemText(2, QCoreApplication.translate("MainWindow", u"short", None))
        self.comboBox_stepDstMode.setItemText(3, QCoreApplication.translate("MainWindow", u"ieee", None))
        self.comboBox_stepDstMode.setItemText(4, QCoreApplication.translate("MainWindow", u"broadcast", None))
        self.comboBox_stepDstMode.setItemText(5, QCoreApplication.translate("MainWindow", u"bound_no_ack", None))
        self.comboBox_stepDstMode.setItemText(6, QCoreApplication.translate("MainWindow", u"short_no_ack", None))
        self.comboBox_stepDstMode.setItemText(7, QCoreApplication.translate("MainWindow", u"ieee_no_ack", None))

#if QT_CONFIG(accessibility)
        self.lineEdit_stepDstAddr.setAccessibleName("")
#endif // QT_CONFIG(accessibility)
        self.lineEdit_stepDstAddr.setInputMask("")
        self.lineEdit_stepDstAddr.setText("")
        self.lineEdit_stepDstAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstAddr", None))
        self.lineEdit_stepSrcEp.setInputMask("")
        self.lineEdit_stepSrcEp.setText("")
        self.lineEdit_stepSrcEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"SrcEp", None))
        self.lineEdit_stepDstEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstEp", None))
        self.lineEdit_stepMode.setText("")
        self.lineEdit_stepMode.setPlaceholderText(QCoreApplication.translate("MainWindow", u"mode", None))
        self.lineEdit_stepSize.setText("")
        self.lineEdit_stepSize.setPlaceholderText(QCoreApplication.translate("MainWindow", u"size", None))
        self.lineEdit_stepTime.setText("")
        self.lineEdit_stepTime.setPlaceholderText(QCoreApplication.translate("MainWindow", u"Time(u16 0.1s)", None))
        self.comboBox_stepWithOnoff.setItemText(0, QCoreApplication.translate("MainWindow", u"with onOff", None))
        self.comboBox_stepWithOnoff.setItemText(1, QCoreApplication.translate("MainWindow", u"without onOff", None))

        self.tabWidget.setTabText(self.tabWidget.indexOf(self.level), QCoreApplication.translate("MainWindow", u"level", None))
        self.pushButton_moveHue.setText(QCoreApplication.translate("MainWindow", u"move to hue", None))
        self.comboBox_moveHueDstMode.setItemText(0, QCoreApplication.translate("MainWindow", u"bound", None))
        self.comboBox_moveHueDstMode.setItemText(1, QCoreApplication.translate("MainWindow", u"group", None))
        self.comboBox_moveHueDstMode.setItemText(2, QCoreApplication.translate("MainWindow", u"short", None))
        self.comboBox_moveHueDstMode.setItemText(3, QCoreApplication.translate("MainWindow", u"ieee", None))
        self.comboBox_moveHueDstMode.setItemText(4, QCoreApplication.translate("MainWindow", u"broadcast", None))
        self.comboBox_moveHueDstMode.setItemText(5, QCoreApplication.translate("MainWindow", u"bound_no_ack", None))
        self.comboBox_moveHueDstMode.setItemText(6, QCoreApplication.translate("MainWindow", u"short_no_ack", None))
        self.comboBox_moveHueDstMode.setItemText(7, QCoreApplication.translate("MainWindow", u"ieee_no_ack", None))

        self.comboBox_moveHueDstMode.setCurrentText(QCoreApplication.translate("MainWindow", u"short", None))
#if QT_CONFIG(accessibility)
        self.lineEdit_moveHueDstAddr.setAccessibleName("")
#endif // QT_CONFIG(accessibility)
        self.lineEdit_moveHueDstAddr.setInputMask("")
        self.lineEdit_moveHueDstAddr.setText("")
        self.lineEdit_moveHueDstAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstAddr", None))
        self.lineEdit_moveHueSrcEp.setInputMask("")
        self.lineEdit_moveHueSrcEp.setText("")
        self.lineEdit_moveHueSrcEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"SrcEp", None))
        self.lineEdit_moveHueDstEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstEp", None))
        self.lineEdit_moveHueHue.setText("")
        self.lineEdit_moveHueHue.setPlaceholderText(QCoreApplication.translate("MainWindow", u"hue", None))
        self.lineEdit_moveHueDir.setText("")
        self.lineEdit_moveHueDir.setPlaceholderText(QCoreApplication.translate("MainWindow", u"direction", None))
        self.lineEdit_moveHueTime.setText("")
        self.lineEdit_moveHueTime.setPlaceholderText(QCoreApplication.translate("MainWindow", u"Time(u16 0.1s)", None))
        self.pushButton_temperature.setText(QCoreApplication.translate("MainWindow", u"move to temperature", None))
        self.comboBox_moveTempDstMode.setItemText(0, QCoreApplication.translate("MainWindow", u"bound", None))
        self.comboBox_moveTempDstMode.setItemText(1, QCoreApplication.translate("MainWindow", u"group", None))
        self.comboBox_moveTempDstMode.setItemText(2, QCoreApplication.translate("MainWindow", u"short", None))
        self.comboBox_moveTempDstMode.setItemText(3, QCoreApplication.translate("MainWindow", u"ieee", None))
        self.comboBox_moveTempDstMode.setItemText(4, QCoreApplication.translate("MainWindow", u"broadcast", None))
        self.comboBox_moveTempDstMode.setItemText(5, QCoreApplication.translate("MainWindow", u"bound_no_ack", None))
        self.comboBox_moveTempDstMode.setItemText(6, QCoreApplication.translate("MainWindow", u"short_no_ack", None))
        self.comboBox_moveTempDstMode.setItemText(7, QCoreApplication.translate("MainWindow", u"ieee_no_ack", None))

        self.comboBox_moveTempDstMode.setCurrentText(QCoreApplication.translate("MainWindow", u"short", None))
#if QT_CONFIG(accessibility)
        self.lineEdit_moveTempDstAddr.setAccessibleName("")
#endif // QT_CONFIG(accessibility)
        self.lineEdit_moveTempDstAddr.setInputMask("")
        self.lineEdit_moveTempDstAddr.setText("")
        self.lineEdit_moveTempDstAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstAddr", None))
        self.lineEdit_moveTempSrcEp.setInputMask("")
        self.lineEdit_moveTempSrcEp.setText("")
        self.lineEdit_moveTempSrcEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"SrcEp", None))
        self.lineEdit_moveTempDstEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstEp", None))
        self.lineEdit_moveTempTemp.setText("")
        self.lineEdit_moveTempTemp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"temperature", None))
        self.lineEdit_moveTempTime.setText("")
        self.lineEdit_moveTempTime.setPlaceholderText(QCoreApplication.translate("MainWindow", u"Time(u16 0.1s)", None))
        self.pushButton_saturation.setText(QCoreApplication.translate("MainWindow", u"move to saturation", None))
        self.comboBox_moveSatDstMode.setItemText(0, QCoreApplication.translate("MainWindow", u"bound", None))
        self.comboBox_moveSatDstMode.setItemText(1, QCoreApplication.translate("MainWindow", u"group", None))
        self.comboBox_moveSatDstMode.setItemText(2, QCoreApplication.translate("MainWindow", u"short", None))
        self.comboBox_moveSatDstMode.setItemText(3, QCoreApplication.translate("MainWindow", u"ieee", None))
        self.comboBox_moveSatDstMode.setItemText(4, QCoreApplication.translate("MainWindow", u"broadcast", None))
        self.comboBox_moveSatDstMode.setItemText(5, QCoreApplication.translate("MainWindow", u"bound_no_ack", None))
        self.comboBox_moveSatDstMode.setItemText(6, QCoreApplication.translate("MainWindow", u"short_no_ack", None))
        self.comboBox_moveSatDstMode.setItemText(7, QCoreApplication.translate("MainWindow", u"ieee_no_ack", None))

        self.comboBox_moveSatDstMode.setCurrentText(QCoreApplication.translate("MainWindow", u"short", None))
#if QT_CONFIG(accessibility)
        self.lineEdit_moveSatDstAddr.setAccessibleName("")
#endif // QT_CONFIG(accessibility)
        self.lineEdit_moveSatDstAddr.setInputMask("")
        self.lineEdit_moveSatDstAddr.setText("")
        self.lineEdit_moveSatDstAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstAddr", None))
        self.lineEdit_moveSatSrcEp.setInputMask("")
        self.lineEdit_moveSatSrcEp.setText("")
        self.lineEdit_moveSatSrcEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"SrcEp", None))
        self.lineEdit_moveSatDstEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstEp", None))
        self.lineEdit_moveSatSat.setText("")
        self.lineEdit_moveSatSat.setPlaceholderText(QCoreApplication.translate("MainWindow", u"saturation", None))
        self.lineEdit_moveSatTime.setText("")
        self.lineEdit_moveSatTime.setPlaceholderText(QCoreApplication.translate("MainWindow", u"Time(u16 0.1s)", None))
        self.pushButton_color.setText(QCoreApplication.translate("MainWindow", u"move to color", None))
        self.comboBox_moveColorDstMode.setItemText(0, QCoreApplication.translate("MainWindow", u"bound", None))
        self.comboBox_moveColorDstMode.setItemText(1, QCoreApplication.translate("MainWindow", u"group", None))
        self.comboBox_moveColorDstMode.setItemText(2, QCoreApplication.translate("MainWindow", u"short", None))
        self.comboBox_moveColorDstMode.setItemText(3, QCoreApplication.translate("MainWindow", u"ieee", None))
        self.comboBox_moveColorDstMode.setItemText(4, QCoreApplication.translate("MainWindow", u"broadcast", None))
        self.comboBox_moveColorDstMode.setItemText(5, QCoreApplication.translate("MainWindow", u"bound_no_ack", None))
        self.comboBox_moveColorDstMode.setItemText(6, QCoreApplication.translate("MainWindow", u"short_no_ack", None))
        self.comboBox_moveColorDstMode.setItemText(7, QCoreApplication.translate("MainWindow", u"ieee_no_ack", None))

        self.comboBox_moveColorDstMode.setCurrentText(QCoreApplication.translate("MainWindow", u"short", None))
#if QT_CONFIG(accessibility)
        self.lineEdit_moveColorDstAddr.setAccessibleName("")
#endif // QT_CONFIG(accessibility)
        self.lineEdit_moveColorDstAddr.setInputMask("")
        self.lineEdit_moveColorDstAddr.setText("")
        self.lineEdit_moveColorDstAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstAddr", None))
        self.lineEdit_moveColorSrcEp.setInputMask("")
        self.lineEdit_moveColorSrcEp.setText("")
        self.lineEdit_moveColorSrcEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"SrcEp", None))
        self.lineEdit_moveColorDstEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstEp", None))
        self.lineEdit_moveColorX.setText("")
        self.lineEdit_moveColorX.setPlaceholderText(QCoreApplication.translate("MainWindow", u"colorX", None))
        self.lineEdit_moveColorY.setText("")
        self.lineEdit_moveColorY.setPlaceholderText(QCoreApplication.translate("MainWindow", u"colorY", None))
        self.lineEdit_moveColorTime.setText("")
        self.lineEdit_moveColorTime.setPlaceholderText(QCoreApplication.translate("MainWindow", u"Time(u16 0.1s)", None))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.color), QCoreApplication.translate("MainWindow", u"color", None))
        self.pushButton_identifyQuery.setText(QCoreApplication.translate("MainWindow", u"identify query", None))
        self.comboBox_identifyQueryDstMode.setItemText(0, QCoreApplication.translate("MainWindow", u"bound", None))
        self.comboBox_identifyQueryDstMode.setItemText(1, QCoreApplication.translate("MainWindow", u"group", None))
        self.comboBox_identifyQueryDstMode.setItemText(2, QCoreApplication.translate("MainWindow", u"short", None))
        self.comboBox_identifyQueryDstMode.setItemText(3, QCoreApplication.translate("MainWindow", u"ieee", None))
        self.comboBox_identifyQueryDstMode.setItemText(4, QCoreApplication.translate("MainWindow", u"broadcast", None))
        self.comboBox_identifyQueryDstMode.setItemText(5, QCoreApplication.translate("MainWindow", u"bound_no_ack", None))
        self.comboBox_identifyQueryDstMode.setItemText(6, QCoreApplication.translate("MainWindow", u"short_no_ack", None))
        self.comboBox_identifyQueryDstMode.setItemText(7, QCoreApplication.translate("MainWindow", u"ieee_no_ack", None))

#if QT_CONFIG(accessibility)
        self.lineEdit_identifyQueryDstAddr.setAccessibleName("")
#endif // QT_CONFIG(accessibility)
        self.lineEdit_identifyQueryDstAddr.setInputMask("")
        self.lineEdit_identifyQueryDstAddr.setText("")
        self.lineEdit_identifyQueryDstAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstAddr", None))
        self.lineEdit_identifyQuerySrcEp.setInputMask("")
        self.lineEdit_identifyQuerySrcEp.setText("")
        self.lineEdit_identifyQuerySrcEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"SrcEp", None))
        self.lineEdit_identifyQueryDstEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstEp", None))
        self.pushButton_identify.setText(QCoreApplication.translate("MainWindow", u"identify", None))
        self.comboBox_identifyDstMode.setItemText(0, QCoreApplication.translate("MainWindow", u"bound", None))
        self.comboBox_identifyDstMode.setItemText(1, QCoreApplication.translate("MainWindow", u"group", None))
        self.comboBox_identifyDstMode.setItemText(2, QCoreApplication.translate("MainWindow", u"short", None))
        self.comboBox_identifyDstMode.setItemText(3, QCoreApplication.translate("MainWindow", u"ieee", None))
        self.comboBox_identifyDstMode.setItemText(4, QCoreApplication.translate("MainWindow", u"broadcast", None))
        self.comboBox_identifyDstMode.setItemText(5, QCoreApplication.translate("MainWindow", u"bound_no_ack", None))
        self.comboBox_identifyDstMode.setItemText(6, QCoreApplication.translate("MainWindow", u"short_no_ack", None))
        self.comboBox_identifyDstMode.setItemText(7, QCoreApplication.translate("MainWindow", u"ieee_no_ack", None))

#if QT_CONFIG(accessibility)
        self.lineEdit_identifyDstAddr.setAccessibleName("")
#endif // QT_CONFIG(accessibility)
        self.lineEdit_identifyDstAddr.setInputMask("")
        self.lineEdit_identifyDstAddr.setText("")
        self.lineEdit_identifyDstAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstAddr", None))
        self.lineEdit_identifySrcEp.setInputMask("")
        self.lineEdit_identifySrcEp.setText("")
        self.lineEdit_identifySrcEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"SrcEp", None))
        self.lineEdit_identifyDstEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstEp", None))
        self.lineEdit_identifyTime.setText("")
        self.lineEdit_identifyTime.setPlaceholderText(QCoreApplication.translate("MainWindow", u"time(u16 s)", None))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.identify), QCoreApplication.translate("MainWindow", u"identify", None))
        self.pushButton_addScene.setText(QCoreApplication.translate("MainWindow", u"add scene", None))
        self.comboBox_addSceneDstMode.setItemText(0, QCoreApplication.translate("MainWindow", u"bound", None))
        self.comboBox_addSceneDstMode.setItemText(1, QCoreApplication.translate("MainWindow", u"group", None))
        self.comboBox_addSceneDstMode.setItemText(2, QCoreApplication.translate("MainWindow", u"short", None))
        self.comboBox_addSceneDstMode.setItemText(3, QCoreApplication.translate("MainWindow", u"ieee", None))
        self.comboBox_addSceneDstMode.setItemText(4, QCoreApplication.translate("MainWindow", u"broadcast", None))
        self.comboBox_addSceneDstMode.setItemText(5, QCoreApplication.translate("MainWindow", u"bound_no_ack", None))
        self.comboBox_addSceneDstMode.setItemText(6, QCoreApplication.translate("MainWindow", u"short_no_ack", None))
        self.comboBox_addSceneDstMode.setItemText(7, QCoreApplication.translate("MainWindow", u"ieee_no_ack", None))

#if QT_CONFIG(accessibility)
        self.lineEdit_addSceneDstAddr.setAccessibleName("")
#endif // QT_CONFIG(accessibility)
        self.lineEdit_addSceneDstAddr.setInputMask("")
        self.lineEdit_addSceneDstAddr.setText("")
        self.lineEdit_addSceneDstAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstAddr", None))
        self.lineEdit_addSceneSrcEp.setInputMask("")
        self.lineEdit_addSceneSrcEp.setText("")
        self.lineEdit_addSceneSrcEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"SrcEp", None))
        self.lineEdit_addSceneDstEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstEp", None))
        self.lineEdit_addSceneGroupId.setText("")
        self.lineEdit_addSceneGroupId.setPlaceholderText(QCoreApplication.translate("MainWindow", u"groupID", None))
        self.lineEdit_addSceneSceneId.setPlaceholderText(QCoreApplication.translate("MainWindow", u"sceneID", None))
        self.lineEdit_addSceneTime.setText("")
        self.lineEdit_addSceneTime.setPlaceholderText(QCoreApplication.translate("MainWindow", u"Time(u16 s)", None))
        self.lineEdit_addSceneNameLen.setText("")
        self.lineEdit_addSceneNameLen.setPlaceholderText(QCoreApplication.translate("MainWindow", u"sceneNameLen", None))
        self.lineEdit_addSceneName.setText("")
        self.lineEdit_addSceneName.setPlaceholderText(QCoreApplication.translate("MainWindow", u"sceneName", None))
        self.lineEdit_addSceneExtLen.setText("")
        self.lineEdit_addSceneExtLen.setPlaceholderText(QCoreApplication.translate("MainWindow", u"extendLen", None))
        self.lineEdit_addSceneExtField.setPlaceholderText(QCoreApplication.translate("MainWindow", u"extension field sets", None))
        self.pushButton_storeScene.setText(QCoreApplication.translate("MainWindow", u"store scene", None))
        self.comboBox_storeSceneDstMode.setItemText(0, QCoreApplication.translate("MainWindow", u"bound", None))
        self.comboBox_storeSceneDstMode.setItemText(1, QCoreApplication.translate("MainWindow", u"group", None))
        self.comboBox_storeSceneDstMode.setItemText(2, QCoreApplication.translate("MainWindow", u"short", None))
        self.comboBox_storeSceneDstMode.setItemText(3, QCoreApplication.translate("MainWindow", u"ieee", None))
        self.comboBox_storeSceneDstMode.setItemText(4, QCoreApplication.translate("MainWindow", u"broadcast", None))
        self.comboBox_storeSceneDstMode.setItemText(5, QCoreApplication.translate("MainWindow", u"bound_no_ack", None))
        self.comboBox_storeSceneDstMode.setItemText(6, QCoreApplication.translate("MainWindow", u"short_no_ack", None))
        self.comboBox_storeSceneDstMode.setItemText(7, QCoreApplication.translate("MainWindow", u"ieee_no_ack", None))

#if QT_CONFIG(accessibility)
        self.lineEdit_storeSceneDstAddr.setAccessibleName("")
#endif // QT_CONFIG(accessibility)
        self.lineEdit_storeSceneDstAddr.setInputMask("")
        self.lineEdit_storeSceneDstAddr.setText("")
        self.lineEdit_storeSceneDstAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstAddr", None))
        self.lineEdit_storeSceneSrcEp.setInputMask("")
        self.lineEdit_storeSceneSrcEp.setText("")
        self.lineEdit_storeSceneSrcEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"SrcEp", None))
        self.lineEdit_storeSceneDstEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstEp", None))
        self.lineEdit_storeSceneGroupId.setText("")
        self.lineEdit_storeSceneGroupId.setPlaceholderText(QCoreApplication.translate("MainWindow", u"groupID", None))
        self.lineEdit_storeSceneId.setText("")
        self.lineEdit_storeSceneId.setPlaceholderText(QCoreApplication.translate("MainWindow", u"sceneID", None))
        self.pushButton_viewScene.setText(QCoreApplication.translate("MainWindow", u"view scene", None))
        self.comboBox_viewSceneDstMode.setItemText(0, QCoreApplication.translate("MainWindow", u"bound", None))
        self.comboBox_viewSceneDstMode.setItemText(1, QCoreApplication.translate("MainWindow", u"group", None))
        self.comboBox_viewSceneDstMode.setItemText(2, QCoreApplication.translate("MainWindow", u"short", None))
        self.comboBox_viewSceneDstMode.setItemText(3, QCoreApplication.translate("MainWindow", u"ieee", None))
        self.comboBox_viewSceneDstMode.setItemText(4, QCoreApplication.translate("MainWindow", u"broadcast", None))
        self.comboBox_viewSceneDstMode.setItemText(5, QCoreApplication.translate("MainWindow", u"bound_no_ack", None))
        self.comboBox_viewSceneDstMode.setItemText(6, QCoreApplication.translate("MainWindow", u"short_no_ack", None))
        self.comboBox_viewSceneDstMode.setItemText(7, QCoreApplication.translate("MainWindow", u"ieee_no_ack", None))

#if QT_CONFIG(accessibility)
        self.lineEdit_viewSceneDstAddr.setAccessibleName("")
#endif // QT_CONFIG(accessibility)
        self.lineEdit_viewSceneDstAddr.setInputMask("")
        self.lineEdit_viewSceneDstAddr.setText("")
        self.lineEdit_viewSceneDstAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstAddr", None))
        self.lineEdit_viewSceneSrcEp.setInputMask("")
        self.lineEdit_viewSceneSrcEp.setText("")
        self.lineEdit_viewSceneSrcEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"SrcEp", None))
        self.lineEdit_viewSceneDstEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstEp", None))
        self.lineEdit_viewSceneGroupId.setText("")
        self.lineEdit_viewSceneGroupId.setPlaceholderText(QCoreApplication.translate("MainWindow", u"groupID", None))
        self.lineEdit_viewSceneId.setText("")
        self.lineEdit_viewSceneId.setPlaceholderText(QCoreApplication.translate("MainWindow", u"sceneID", None))
        self.pushButton_recallScene.setText(QCoreApplication.translate("MainWindow", u"recall scene", None))
        self.comboBox_recallSceneDstMode.setItemText(0, QCoreApplication.translate("MainWindow", u"bound", None))
        self.comboBox_recallSceneDstMode.setItemText(1, QCoreApplication.translate("MainWindow", u"group", None))
        self.comboBox_recallSceneDstMode.setItemText(2, QCoreApplication.translate("MainWindow", u"short", None))
        self.comboBox_recallSceneDstMode.setItemText(3, QCoreApplication.translate("MainWindow", u"ieee", None))
        self.comboBox_recallSceneDstMode.setItemText(4, QCoreApplication.translate("MainWindow", u"broadcast", None))
        self.comboBox_recallSceneDstMode.setItemText(5, QCoreApplication.translate("MainWindow", u"bound_no_ack", None))
        self.comboBox_recallSceneDstMode.setItemText(6, QCoreApplication.translate("MainWindow", u"short_no_ack", None))
        self.comboBox_recallSceneDstMode.setItemText(7, QCoreApplication.translate("MainWindow", u"ieee_no_ack", None))

#if QT_CONFIG(accessibility)
        self.lineEdit_recallSceneDstAddr.setAccessibleName("")
#endif // QT_CONFIG(accessibility)
        self.lineEdit_recallSceneDstAddr.setInputMask("")
        self.lineEdit_recallSceneDstAddr.setText("")
        self.lineEdit_recallSceneDstAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstAddr", None))
        self.lineEdit_recallSceneSrcEp.setInputMask("")
        self.lineEdit_recallSceneSrcEp.setText("")
        self.lineEdit_recallSceneSrcEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"SrcEp", None))
        self.lineEdit_recallSceneDstEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstEp", None))
        self.lineEdit_recallSceneGroupId.setText("")
        self.lineEdit_recallSceneGroupId.setPlaceholderText(QCoreApplication.translate("MainWindow", u"groupID", None))
        self.lineEdit_recallSceneId.setText("")
        self.lineEdit_recallSceneId.setPlaceholderText(QCoreApplication.translate("MainWindow", u"sceneID", None))
        self.pushButton_sceneRemoveAll.setText(QCoreApplication.translate("MainWindow", u"remove all", None))
        self.comboBox_sceneRemoveAllDstMode.setItemText(0, QCoreApplication.translate("MainWindow", u"bound", None))
        self.comboBox_sceneRemoveAllDstMode.setItemText(1, QCoreApplication.translate("MainWindow", u"group", None))
        self.comboBox_sceneRemoveAllDstMode.setItemText(2, QCoreApplication.translate("MainWindow", u"short", None))
        self.comboBox_sceneRemoveAllDstMode.setItemText(3, QCoreApplication.translate("MainWindow", u"ieee", None))
        self.comboBox_sceneRemoveAllDstMode.setItemText(4, QCoreApplication.translate("MainWindow", u"broadcast", None))
        self.comboBox_sceneRemoveAllDstMode.setItemText(5, QCoreApplication.translate("MainWindow", u"bound_no_ack", None))
        self.comboBox_sceneRemoveAllDstMode.setItemText(6, QCoreApplication.translate("MainWindow", u"short_no_ack", None))
        self.comboBox_sceneRemoveAllDstMode.setItemText(7, QCoreApplication.translate("MainWindow", u"ieee_no_ack", None))

#if QT_CONFIG(accessibility)
        self.lineEdit_sceneRemoveAllDstAddr.setAccessibleName("")
#endif // QT_CONFIG(accessibility)
        self.lineEdit_sceneRemoveAllDstAddr.setInputMask("")
        self.lineEdit_sceneRemoveAllDstAddr.setText("")
        self.lineEdit_sceneRemoveAllDstAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstAddr", None))
        self.lineEdit_sceneRemoveAllSrcEp.setInputMask("")
        self.lineEdit_sceneRemoveAllSrcEp.setText("")
        self.lineEdit_sceneRemoveAllSrcEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"SrcEp", None))
        self.lineEdit_sceneRemoveAllDstEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstEp", None))
        self.lineEdit_sceneRemoveAllGroupId.setText("")
        self.lineEdit_sceneRemoveAllGroupId.setPlaceholderText(QCoreApplication.translate("MainWindow", u"groupID", None))
        self.pushButton_removeScene.setText(QCoreApplication.translate("MainWindow", u"remove scene", None))
        self.comboBox_removeSceneDstMode.setItemText(0, QCoreApplication.translate("MainWindow", u"bound", None))
        self.comboBox_removeSceneDstMode.setItemText(1, QCoreApplication.translate("MainWindow", u"group", None))
        self.comboBox_removeSceneDstMode.setItemText(2, QCoreApplication.translate("MainWindow", u"short", None))
        self.comboBox_removeSceneDstMode.setItemText(3, QCoreApplication.translate("MainWindow", u"ieee", None))
        self.comboBox_removeSceneDstMode.setItemText(4, QCoreApplication.translate("MainWindow", u"broadcast", None))
        self.comboBox_removeSceneDstMode.setItemText(5, QCoreApplication.translate("MainWindow", u"bound_no_ack", None))
        self.comboBox_removeSceneDstMode.setItemText(6, QCoreApplication.translate("MainWindow", u"short_no_ack", None))
        self.comboBox_removeSceneDstMode.setItemText(7, QCoreApplication.translate("MainWindow", u"ieee_no_ack", None))

#if QT_CONFIG(accessibility)
        self.lineEdit_removeSceneDstAddr.setAccessibleName("")
#endif // QT_CONFIG(accessibility)
        self.lineEdit_removeSceneDstAddr.setInputMask("")
        self.lineEdit_removeSceneDstAddr.setText("")
        self.lineEdit_removeSceneDstAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstAddr", None))
        self.lineEdit_removeSceneSrcEp.setInputMask("")
        self.lineEdit_removeSceneSrcEp.setText("")
        self.lineEdit_removeSceneSrcEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"SrcEp", None))
        self.lineEdit_removeSceneDstEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstEp", None))
        self.lineEdit_removeSceneGroupId.setText("")
        self.lineEdit_removeSceneGroupId.setPlaceholderText(QCoreApplication.translate("MainWindow", u"groupID", None))
        self.lineEdit_removeSceneId.setText("")
        self.lineEdit_removeSceneId.setPlaceholderText(QCoreApplication.translate("MainWindow", u"sceneID", None))
        self.pushButton_sceneGetMembership.setText(QCoreApplication.translate("MainWindow", u"get membership", None))
        self.comboBox_getSceneDstMode.setItemText(0, QCoreApplication.translate("MainWindow", u"bound", None))
        self.comboBox_getSceneDstMode.setItemText(1, QCoreApplication.translate("MainWindow", u"group", None))
        self.comboBox_getSceneDstMode.setItemText(2, QCoreApplication.translate("MainWindow", u"short", None))
        self.comboBox_getSceneDstMode.setItemText(3, QCoreApplication.translate("MainWindow", u"ieee", None))
        self.comboBox_getSceneDstMode.setItemText(4, QCoreApplication.translate("MainWindow", u"broadcast", None))
        self.comboBox_getSceneDstMode.setItemText(5, QCoreApplication.translate("MainWindow", u"bound_no_ack", None))
        self.comboBox_getSceneDstMode.setItemText(6, QCoreApplication.translate("MainWindow", u"short_no_ack", None))
        self.comboBox_getSceneDstMode.setItemText(7, QCoreApplication.translate("MainWindow", u"ieee_no_ack", None))

#if QT_CONFIG(accessibility)
        self.lineEdit_getSceneDstAddr.setAccessibleName("")
#endif // QT_CONFIG(accessibility)
        self.lineEdit_getSceneDstAddr.setInputMask("")
        self.lineEdit_getSceneDstAddr.setText("")
        self.lineEdit_getSceneDstAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstAddr", None))
        self.lineEdit_getSceneSrcEp.setInputMask("")
        self.lineEdit_getSceneSrcEp.setText("")
        self.lineEdit_getSceneSrcEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"SrcEp", None))
        self.lineEdit_getSceneDstEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstEp", None))
        self.lineEdit_getSceneGroupId.setText("")
        self.lineEdit_getSceneGroupId.setPlaceholderText(QCoreApplication.translate("MainWindow", u"groupID", None))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.scene), QCoreApplication.translate("MainWindow", u"scene", None))
        self.pushButton_imageNotify.setText(QCoreApplication.translate("MainWindow", u"image notify", None))
        self.comboBox_otaDstMode.setItemText(0, QCoreApplication.translate("MainWindow", u"bound", None))
        self.comboBox_otaDstMode.setItemText(1, QCoreApplication.translate("MainWindow", u"group", None))
        self.comboBox_otaDstMode.setItemText(2, QCoreApplication.translate("MainWindow", u"short", None))
        self.comboBox_otaDstMode.setItemText(3, QCoreApplication.translate("MainWindow", u"ieee", None))
        self.comboBox_otaDstMode.setItemText(4, QCoreApplication.translate("MainWindow", u"broadcast", None))
        self.comboBox_otaDstMode.setItemText(5, QCoreApplication.translate("MainWindow", u"bound_no_ack", None))
        self.comboBox_otaDstMode.setItemText(6, QCoreApplication.translate("MainWindow", u"short_no_ack", None))
        self.comboBox_otaDstMode.setItemText(7, QCoreApplication.translate("MainWindow", u"ieee_no_ack", None))

        self.comboBox_otaDstMode.setCurrentText(QCoreApplication.translate("MainWindow", u"short", None))
#if QT_CONFIG(accessibility)
        self.lineEdit_otaDstAddr.setAccessibleName("")
#endif // QT_CONFIG(accessibility)
        self.lineEdit_otaDstAddr.setInputMask("")
        self.lineEdit_otaDstAddr.setText("")
        self.lineEdit_otaDstAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstAddr", None))
        self.lineEdit_otaSrcEp.setInputMask("")
        self.lineEdit_otaSrcEp.setText("")
        self.lineEdit_otaSrcEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"SrcEp", None))
        self.lineEdit_otaDstEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstEp", None))
        self.comboBox_payloadType.setItemText(0, QCoreApplication.translate("MainWindow", u"JITTER", None))
        self.comboBox_payloadType.setItemText(1, QCoreApplication.translate("MainWindow", u"JITTER_MFG", None))
        self.comboBox_payloadType.setItemText(2, QCoreApplication.translate("MainWindow", u"JITTER_MFG_TYPE", None))
        self.comboBox_payloadType.setItemText(3, QCoreApplication.translate("MainWindow", u"JITTER_MFG_TYPE_VER", None))

#if QT_CONFIG(accessibility)
        self.lineEdit_jitter.setAccessibleName("")
#endif // QT_CONFIG(accessibility)
        self.lineEdit_jitter.setInputMask("")
        self.lineEdit_jitter.setText("")
        self.lineEdit_jitter.setPlaceholderText(QCoreApplication.translate("MainWindow", u"QueryJitter", None))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.ota), QCoreApplication.translate("MainWindow", u"OTA", None))
        self.pushButton_if_state_set.setText(QCoreApplication.translate("MainWindow", u"if state set", None))
        self.comboBox_ifSetDstMode.setItemText(0, QCoreApplication.translate("MainWindow", u"bound", None))
        self.comboBox_ifSetDstMode.setItemText(1, QCoreApplication.translate("MainWindow", u"group", None))
        self.comboBox_ifSetDstMode.setItemText(2, QCoreApplication.translate("MainWindow", u"short", None))
        self.comboBox_ifSetDstMode.setItemText(3, QCoreApplication.translate("MainWindow", u"ieee", None))
        self.comboBox_ifSetDstMode.setItemText(4, QCoreApplication.translate("MainWindow", u"broadcast", None))
        self.comboBox_ifSetDstMode.setItemText(5, QCoreApplication.translate("MainWindow", u"bound_no_ack", None))
        self.comboBox_ifSetDstMode.setItemText(6, QCoreApplication.translate("MainWindow", u"short_no_ack", None))
        self.comboBox_ifSetDstMode.setItemText(7, QCoreApplication.translate("MainWindow", u"ieee_no_ack", None))

#if QT_CONFIG(accessibility)
        self.lineEdit_ifStateSetDstAddr.setAccessibleName("")
#endif // QT_CONFIG(accessibility)
        self.lineEdit_ifStateSetDstAddr.setInputMask("")
        self.lineEdit_ifStateSetDstAddr.setText("")
        self.lineEdit_ifStateSetDstAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstAddr", None))
        self.lineEdit_ifStateSrcEp.setInputMask("")
        self.lineEdit_ifStateSrcEp.setText("")
        self.lineEdit_ifStateSrcEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"SrcEp", None))
        self.lineEdit_ifStateDstEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstEp", None))
        self.lineEdit_ifState.setText("")
        self.lineEdit_ifState.setPlaceholderText(QCoreApplication.translate("MainWindow", u"ifState(0/1)", None))
        self.pushButton_anj_timeout.setText(QCoreApplication.translate("MainWindow", u"anj timeout set", None))
        self.comboBox_anjTimeDstMode.setItemText(0, QCoreApplication.translate("MainWindow", u"bound", None))
        self.comboBox_anjTimeDstMode.setItemText(1, QCoreApplication.translate("MainWindow", u"group", None))
        self.comboBox_anjTimeDstMode.setItemText(2, QCoreApplication.translate("MainWindow", u"short", None))
        self.comboBox_anjTimeDstMode.setItemText(3, QCoreApplication.translate("MainWindow", u"ieee", None))
        self.comboBox_anjTimeDstMode.setItemText(4, QCoreApplication.translate("MainWindow", u"broadcast", None))
        self.comboBox_anjTimeDstMode.setItemText(5, QCoreApplication.translate("MainWindow", u"bound_no_ack", None))
        self.comboBox_anjTimeDstMode.setItemText(6, QCoreApplication.translate("MainWindow", u"short_no_ack", None))
        self.comboBox_anjTimeDstMode.setItemText(7, QCoreApplication.translate("MainWindow", u"ieee_no_ack", None))

#if QT_CONFIG(accessibility)
        self.lineEdit_anjTimeDstAddr.setAccessibleName("")
#endif // QT_CONFIG(accessibility)
        self.lineEdit_anjTimeDstAddr.setInputMask("")
        self.lineEdit_anjTimeDstAddr.setText("")
        self.lineEdit_anjTimeDstAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstAddr", None))
        self.lineEdit_anjTimeSrcEp.setInputMask("")
        self.lineEdit_anjTimeSrcEp.setText("")
        self.lineEdit_anjTimeSrcEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"SrcEp", None))
        self.lineEdit_anjTimeDstEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstEp", None))
        self.lineEdit_anjTime.setText("")
        self.lineEdit_anjTime.setPlaceholderText(QCoreApplication.translate("MainWindow", u"anjTime(u24)", None))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.zbd), QCoreApplication.translate("MainWindow", u"zbd", None))
        self.label_afTestNotes.setText(QCoreApplication.translate("MainWindow", u"Notes: The unicast addr defaults to short addr.", None))
        self.pushButton_txrxTest.setText(QCoreApplication.translate("MainWindow", u"txrx test", None))
#if QT_CONFIG(accessibility)
        self.lineEdit_txrxTestDstAddr.setAccessibleName("")
#endif // QT_CONFIG(accessibility)
        self.lineEdit_txrxTestDstAddr.setInputMask("")
        self.lineEdit_txrxTestDstAddr.setText("")
        self.lineEdit_txrxTestDstAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstAddr:broadcast or unicast", None))
        self.lineEdit_txrxTestSrcEp.setInputMask("")
        self.lineEdit_txrxTestSrcEp.setText("")
        self.lineEdit_txrxTestSrcEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"SrcEp", None))
        self.lineEdit_txrxTestDstEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstEp", None))
        self.lineEdit_txrxTestSendCount.setInputMask("")
        self.lineEdit_txrxTestSendCount.setText("")
        self.lineEdit_txrxTestSendCount.setPlaceholderText(QCoreApplication.translate("MainWindow", u"send count", None))
        self.lineEdit_txrxTestInterval.setInputMask("")
        self.lineEdit_txrxTestInterval.setText("")
        self.lineEdit_txrxTestInterval.setPlaceholderText(QCoreApplication.translate("MainWindow", u"interval", None))
        self.lineEdit_txrxTestTxPower.setInputMask("")
        self.lineEdit_txrxTestTxPower.setText("")
        self.lineEdit_txrxTestTxPower.setPlaceholderText(QCoreApplication.translate("MainWindow", u"tx power", None))
        self.pushButton_afDataSend.setText(QCoreApplication.translate("MainWindow", u"AF data send", None))
#if QT_CONFIG(accessibility)
        self.lineEdit_afDataSendDstAddr.setAccessibleName("")
#endif // QT_CONFIG(accessibility)
        self.lineEdit_afDataSendDstAddr.setInputMask("")
        self.lineEdit_afDataSendDstAddr.setText("")
        self.lineEdit_afDataSendDstAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstAddr:broadcast or unicast", None))
        self.lineEdit_afDataSendSrcEp.setInputMask("")
        self.lineEdit_afDataSendSrcEp.setText("")
        self.lineEdit_afDataSendSrcEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"SrcEp", None))
        self.lineEdit_afDataSendDstEp.setPlaceholderText(QCoreApplication.translate("MainWindow", u"DstEp", None))
        self.lineEdit_afDataSendClusterId.setInputMask("")
        self.lineEdit_afDataSendClusterId.setText("")
        self.lineEdit_afDataSendClusterId.setPlaceholderText(QCoreApplication.translate("MainWindow", u"clusterID", None))
        self.lineEdit_afDataSendData.setInputMask("")
        self.lineEdit_afDataSendData.setText("")
        self.lineEdit_afDataSendData.setPlaceholderText(QCoreApplication.translate("MainWindow", u"payload(n)", None))
        self.pushButton_nodesToggleTest.setText(QCoreApplication.translate("MainWindow", u"nodes toggle test", None))
        self.comboBox_toggleTestMode.setItemText(0, QCoreApplication.translate("MainWindow", u"broadcast", None))
        self.comboBox_toggleTestMode.setItemText(1, QCoreApplication.translate("MainWindow", u"unicast", None))

        self.lineEdit_toggleInterval.setInputMask("")
        self.lineEdit_toggleInterval.setText("")
        self.lineEdit_toggleInterval.setPlaceholderText(QCoreApplication.translate("MainWindow", u"interval, set the interval to 0 to stop, unit: 1/100s.", None))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.AF), QCoreApplication.translate("MainWindow", u"AF", None))
        self.pushButton_hciOtaFileChoose.setText(QCoreApplication.translate("MainWindow", u"OTA file path", None))
        self.lineEdit_hciOtaFilePath.setPlaceholderText(QCoreApplication.translate("MainWindow", u"HCI OTA file path", None))
        self.pushButton_hciOtaStart.setText(QCoreApplication.translate("MainWindow", u"Start OTA", None))
        self.label_hciotaPercent.setText(QCoreApplication.translate("MainWindow", u"OTA Progress Bar", None))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.hciOta), QCoreApplication.translate("MainWindow", u"HCI OTA", None))
        self.label_analyzeAddr.setText(QCoreApplication.translate("MainWindow", u"Address:", None))
        self.lineEdit_analyzeAddr.setPlaceholderText(QCoreApplication.translate("MainWindow", u"dev_addr", None))
        self.label_analyzeInterval.setText(QCoreApplication.translate("MainWindow", u"Interval(s):", None))
        self.lineEdit_analyzeInterval.setText("")
        self.lineEdit_analyzeInterval.setPlaceholderText(QCoreApplication.translate("MainWindow", u">1,<1,=1", None))
        self.label_analyzeCommand.setText(QCoreApplication.translate("MainWindow", u"CommandID:", None))
        self.lineEdit_analyzeCommandId.setText("")
        self.lineEdit_analyzeCommandId.setPlaceholderText(QCoreApplication.translate("MainWindow", u"ZBHCI_CMD_ZCL_REPORT_MSG_RCV", None))
        self.pushButton_startFilter.setText(QCoreApplication.translate("MainWindow", u"Start Filter", None))
        self.pushButton_startAnalyze.setText(QCoreApplication.translate("MainWindow", u"Start Analyze", None))
        self.pushButton_analyzePathChoose.setText(QCoreApplication.translate("MainWindow", u"Analyze Folder Path", None))
        self.lineEdit_analyzePath.setPlaceholderText(QCoreApplication.translate("MainWindow", u"analyze folder path", None))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.analyze), QCoreApplication.translate("MainWindow", u"analyze", None))
        self.pushButton_advParaSet.setText(QCoreApplication.translate("MainWindow", u"Adv Para set", None))
        self.lineEdit_minAdvInterval.setPlaceholderText(QCoreApplication.translate("MainWindow", u"Min interval(2Bytes)", None))
        self.lineEdit_maxAdvInterval.setPlaceholderText(QCoreApplication.translate("MainWindow", u"Max interval(2Bytes)", None))
        self.pushButton_advDataSet.setText(QCoreApplication.translate("MainWindow", u"Adv data set", None))
        self.comboBox_advOrScanRe.setItemText(0, QCoreApplication.translate("MainWindow", u"Adv Data", None))
        self.comboBox_advOrScanRe.setItemText(1, QCoreApplication.translate("MainWindow", u"Scan Response", None))

        self.lineEdit_advData.setPlaceholderText(QCoreApplication.translate("MainWindow", u"data", None))
        self.pushButton_advStart.setText(QCoreApplication.translate("MainWindow", u"start Adv", None))
        self.pushButton_advStop.setText(QCoreApplication.translate("MainWindow", u"stop Adv", None))
        self.pushButton_bleDisconnect.setText(QCoreApplication.translate("MainWindow", u"disconnect", None))
        self.lineEdit_bleConnHandle.setText("")
        self.lineEdit_bleConnHandle.setPlaceholderText(QCoreApplication.translate("MainWindow", u"ble handle(2Bytes)", None))
        self.pushButton_scanParaSet.setText(QCoreApplication.translate("MainWindow", u"scan para set", None))
        self.lineEdit_scanInterval.setPlaceholderText(QCoreApplication.translate("MainWindow", u"scan interval(2Bytes)", None))
        self.lineEdit_scanWindow.setPlaceholderText(QCoreApplication.translate("MainWindow", u"scan window(2Bytes)", None))
        self.label_bleState.setText("")
        self.pushButton_bleScanstart.setText(QCoreApplication.translate("MainWindow", u"start scan", None))
        self.pushButton_bleScanstop.setText(QCoreApplication.translate("MainWindow", u"stop scan", None))
        self.pushButton_bleConnect.setText(QCoreApplication.translate("MainWindow", u"connect", None))
        self.lineEdit_bleMac.setPlaceholderText(QCoreApplication.translate("MainWindow", u"ble mac addr(6Bytes)", None))
        self.pushButton_getConnList.setText(QCoreApplication.translate("MainWindow", u"get connect list ", None))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.blehci), QCoreApplication.translate("MainWindow", u"bleHci", None))
        self.toolBar.setWindowTitle(QCoreApplication.translate("MainWindow", u"toolBar", None))
    # retranslateUi

