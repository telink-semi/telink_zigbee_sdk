import sys
from PySide6 import QtWidgets
from hcisendcommand import Pyside6Serial


if __name__ == '__main__':
    app = QtWidgets.QApplication(sys.argv)  #初始化app
    myshow = Pyside6Serial()                 #创建主窗口
    myshow.show()                           #显示窗口
    sys.exit(app.exec())                   #消息循环结束之后返回0，接着调用sys.exit(0)退出程序
