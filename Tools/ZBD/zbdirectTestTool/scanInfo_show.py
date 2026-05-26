from logging import exception
from PySide6 import QtWidgets
from PySide6.QtWidgets import QWidget ,QFormLayout

class ScanLayout(QWidget):
    def __init__(self,parent=None):
        super(ScanLayout,self).__init__(parent)
        self.setWindowTitle("Scan Info") 
        self.resize(400, 500)  
        self.fromlayout = QFormLayout()
        self.infoList = QtWidgets.QListWidget()

        self.fromlayout.addWidget(self.infoList)

        self.setLayout(self.fromlayout)

        self.dev_choose = {}
        self.infoList.itemClicked.connect(self.choose_ble_dev)

    def choose_ble_dev(self, item):
        dev_choose_str = item.text()
        index_str = dev_choose_str[:4]
        try:
            index = int(index_str)
            self.dev_choose = self.scan_info[index]
        except Exception as e:
            print(e)


    def showInfo(self, showInfo):
        self.scan_info = showInfo
        index = 0
        self.infoList.clear()
        for info in self.scan_info:
            self.infoList.addItem('%04d:' % index + info['address'] + ' ' + info['name'] + ' '+ str(info['rssi']))
            index += 1
