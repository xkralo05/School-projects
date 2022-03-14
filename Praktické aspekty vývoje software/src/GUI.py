# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'GUI.ui'
#
# Created by: PyQt5 UI code generator 5.10.1
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_Calculator(object):
    def setupUi(self, Calculator):
        Calculator.setObjectName("Calculator")
        Calculator.setEnabled(True)
        Calculator.resize(408, 719)
        Calculator.setMinimumSize(QtCore.QSize(408, 718))
        Calculator.setContextMenuPolicy(QtCore.Qt.CustomContextMenu)
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(":/icons/icon_result.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        Calculator.setWindowIcon(icon)
        Calculator.setAutoFillBackground(False)
        Calculator.setStyleSheet("background-color: rgb(71, 68, 60);\n"
"alternate-background-color: rgb(46, 52, 54);\n"
"border-color: rgb(0, 0, 0);\n"
"color: rgb(255, 255, 255);\n"
"")
        Calculator.setLocale(QtCore.QLocale(QtCore.QLocale.Czech, QtCore.QLocale.CzechRepublic))
        Calculator.setAnimated(True)
        self.centralwidget = QtWidgets.QWidget(Calculator)
        self.centralwidget.setLayoutDirection(QtCore.Qt.LeftToRight)
        self.centralwidget.setAutoFillBackground(False)
        self.centralwidget.setStyleSheet("")
        self.centralwidget.setLocale(QtCore.QLocale(QtCore.QLocale.Czech, QtCore.QLocale.CzechRepublic))
        self.centralwidget.setObjectName("centralwidget")
        self.gridLayout = QtWidgets.QGridLayout(self.centralwidget)
        self.gridLayout.setContentsMargins(9, 9, 9, 9)
        self.gridLayout.setSpacing(6)
        self.gridLayout.setObjectName("gridLayout")
        self.Button_3 = QtWidgets.QPushButton(self.centralwidget)
        self.Button_3.setLayoutDirection(QtCore.Qt.LeftToRight)
        self.Button_3.setStyleSheet(" QPushButton {\n"
"      border: 2px solid rgb(46, 52, 54);\n"
"      border-radius: 6px;     \n"
"    background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0.990925, y2:0, stop:0 rgba(0, 0, 0, 255));\n"
"  }\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: qlineargradient(spread:reflect, x1:0.579, y1:0.38561, x2:0.019, y2:0.963, stop:0.263682 rgba(0, 0, 0, 255), stop:0.698565 rgba(50, 50, 50, 255), stop:0.855721 rgba(81, 83, 79, 255))\n"
"  }")
        self.Button_3.setLocale(QtCore.QLocale(QtCore.QLocale.Czech, QtCore.QLocale.CzechRepublic))
        self.Button_3.setText("")
        icon1 = QtGui.QIcon()
        icon1.addPixmap(QtGui.QPixmap(":/icons/icon_num_3.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.Button_3.setIcon(icon1)
        self.Button_3.setIconSize(QtCore.QSize(100, 100))
        self.Button_3.setCheckable(False)
        self.Button_3.setAutoDefault(True)
        self.Button_3.setDefault(True)
        self.Button_3.setFlat(True)
        self.Button_3.setObjectName("Button_3")
        self.buttonGroup_numbers = QtWidgets.QButtonGroup(Calculator)
        self.buttonGroup_numbers.setObjectName("buttonGroup_numbers")
        self.buttonGroup_numbers.addButton(self.Button_3)
        self.gridLayout.addWidget(self.Button_3, 5, 2, 1, 1)
        self.Button_result = QtWidgets.QPushButton(self.centralwidget)
        self.Button_result.setLayoutDirection(QtCore.Qt.LeftToRight)
        self.Button_result.setStyleSheet(" QPushButton {\n"
"      border: 2px solid rgb(46, 52, 54);\n"
"      border-radius: 6px;     \n"
"    background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0.990925, y2:0, stop:0 rgba(0, 0, 0, 255));\n"
"  }\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: qlineargradient(spread:reflect, x1:0.579, y1:0.38561, x2:0.019, y2:0.963, stop:0.263682 rgba(0, 0, 0, 255), stop:0.698565 rgba(50, 50, 50, 255), stop:0.855721 rgba(81, 83, 79, 255))\n"
"  }")
        self.Button_result.setLocale(QtCore.QLocale(QtCore.QLocale.Czech, QtCore.QLocale.CzechRepublic))
        self.Button_result.setText("")
        self.Button_result.setIcon(icon)
        self.Button_result.setIconSize(QtCore.QSize(100, 100))
        self.Button_result.setCheckable(False)
        self.Button_result.setAutoDefault(True)
        self.Button_result.setDefault(True)
        self.Button_result.setFlat(True)
        self.Button_result.setObjectName("Button_result")
        self.buttonGroup_other = QtWidgets.QButtonGroup(Calculator)
        self.buttonGroup_other.setObjectName("buttonGroup_other")
        self.buttonGroup_other.addButton(self.Button_result)
        self.gridLayout.addWidget(self.Button_result, 6, 3, 1, 1)
        self.Button_delete = QtWidgets.QPushButton(self.centralwidget)
        self.Button_delete.setCursor(QtGui.QCursor(QtCore.Qt.ArrowCursor))
        self.Button_delete.setLayoutDirection(QtCore.Qt.LeftToRight)
        self.Button_delete.setAutoFillBackground(False)
        self.Button_delete.setStyleSheet(" QPushButton {\n"
"      border: 2px solid rgb(46, 52, 54);\n"
"      border-radius: 6px;     \n"
"    background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0.990925, y2:0, stop:0 rgba(0, 0, 0, 255));\n"
"  }\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: qlineargradient(spread:reflect, x1:0.579, y1:0.38561, x2:0.019, y2:0.963, stop:0.263682 rgba(0, 0, 0, 255), stop:0.698565 rgba(50, 50, 50, 255), stop:0.855721 rgba(81, 83, 79, 255))\n"
"  }")
        self.Button_delete.setLocale(QtCore.QLocale(QtCore.QLocale.Czech, QtCore.QLocale.CzechRepublic))
        self.Button_delete.setText("")
        icon2 = QtGui.QIcon()
        icon2.addPixmap(QtGui.QPixmap(":/icons/icon_delete.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.Button_delete.setIcon(icon2)
        self.Button_delete.setIconSize(QtCore.QSize(100, 100))
        self.Button_delete.setCheckable(False)
        self.Button_delete.setAutoDefault(True)
        self.Button_delete.setDefault(True)
        self.Button_delete.setFlat(True)
        self.Button_delete.setObjectName("Button_delete")
        self.buttonGroup_other.addButton(self.Button_delete)
        self.gridLayout.addWidget(self.Button_delete, 1, 1, 1, 1)
        self.Button_comma = QtWidgets.QPushButton(self.centralwidget)
        self.Button_comma.setLayoutDirection(QtCore.Qt.LeftToRight)
        self.Button_comma.setStyleSheet(" QPushButton {\n"
"      border: 2px solid rgb(46, 52, 54);\n"
"      border-radius: 6px;     \n"
"    background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0.990925, y2:0, stop:0 rgba(0, 0, 0, 255));\n"
"  }\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: qlineargradient(spread:reflect, x1:0.579, y1:0.38561, x2:0.019, y2:0.963, stop:0.263682 rgba(0, 0, 0, 255), stop:0.698565 rgba(50, 50, 50, 255), stop:0.855721 rgba(81, 83, 79, 255))\n"
"  }")
        self.Button_comma.setLocale(QtCore.QLocale(QtCore.QLocale.Czech, QtCore.QLocale.CzechRepublic))
        self.Button_comma.setText("")
        icon3 = QtGui.QIcon()
        icon3.addPixmap(QtGui.QPixmap(":/icons/icon_comma.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.Button_comma.setIcon(icon3)
        self.Button_comma.setIconSize(QtCore.QSize(100, 100))
        self.Button_comma.setCheckable(False)
        self.Button_comma.setAutoDefault(True)
        self.Button_comma.setDefault(True)
        self.Button_comma.setFlat(True)
        self.Button_comma.setObjectName("Button_comma")
        self.buttonGroup_other.addButton(self.Button_comma)
        self.gridLayout.addWidget(self.Button_comma, 6, 2, 1, 1)
        self.Button_clear_all = QtWidgets.QPushButton(self.centralwidget)
        self.Button_clear_all.setLayoutDirection(QtCore.Qt.LeftToRight)
        self.Button_clear_all.setStyleSheet(" QPushButton {\n"
"      border: 2px solid rgb(46, 52, 54);\n"
"      border-radius: 6px;     \n"
"    background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0.990925, y2:0, stop:0 rgba(0, 0, 0, 255));\n"
"  }\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: qlineargradient(spread:reflect, x1:0.579, y1:0.38561, x2:0.019, y2:0.963, stop:0.263682 rgba(0, 0, 0, 255), stop:0.698565 rgba(50, 50, 50, 255), stop:0.855721 rgba(81, 83, 79, 255))\n"
"  }")
        self.Button_clear_all.setLocale(QtCore.QLocale(QtCore.QLocale.Czech, QtCore.QLocale.CzechRepublic))
        self.Button_clear_all.setText("")
        icon4 = QtGui.QIcon()
        icon4.addPixmap(QtGui.QPixmap(":/icons/icon_clear_all.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.Button_clear_all.setIcon(icon4)
        self.Button_clear_all.setIconSize(QtCore.QSize(100, 100))
        self.Button_clear_all.setCheckable(False)
        self.Button_clear_all.setAutoDefault(True)
        self.Button_clear_all.setDefault(True)
        self.Button_clear_all.setFlat(True)
        self.Button_clear_all.setObjectName("Button_clear_all")
        self.buttonGroup_other.addButton(self.Button_clear_all)
        self.gridLayout.addWidget(self.Button_clear_all, 1, 0, 1, 1)
        self.Button_square_root = QtWidgets.QPushButton(self.centralwidget)
        self.Button_square_root.setLayoutDirection(QtCore.Qt.LeftToRight)
        self.Button_square_root.setStyleSheet(" QPushButton {\n"
"      border: 2px solid rgb(46, 52, 54);\n"
"      border-radius: 6px;     \n"
"    background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0.990925, y2:0, stop:0 rgba(0, 0, 0, 255));\n"
"  }\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: qlineargradient(spread:reflect, x1:0.579, y1:0.38561, x2:0.019, y2:0.963, stop:0.263682 rgba(0, 0, 0, 255), stop:0.698565 rgba(50, 50, 50, 255), stop:0.855721 rgba(81, 83, 79, 255))\n"
"  }")
        self.Button_square_root.setLocale(QtCore.QLocale(QtCore.QLocale.Czech, QtCore.QLocale.CzechRepublic))
        self.Button_square_root.setText("")
        icon5 = QtGui.QIcon()
        icon5.addPixmap(QtGui.QPixmap(":/icons/icon_square_root.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.Button_square_root.setIcon(icon5)
        self.Button_square_root.setIconSize(QtCore.QSize(100, 100))
        self.Button_square_root.setCheckable(False)
        self.Button_square_root.setAutoDefault(True)
        self.Button_square_root.setDefault(True)
        self.Button_square_root.setFlat(True)
        self.Button_square_root.setObjectName("Button_square_root")
        self.buttonGroup_other.addButton(self.Button_square_root)
        self.gridLayout.addWidget(self.Button_square_root, 2, 0, 1, 1)
        self.Button_8 = QtWidgets.QPushButton(self.centralwidget)
        self.Button_8.setLayoutDirection(QtCore.Qt.LeftToRight)
        self.Button_8.setStyleSheet(" QPushButton {\n"
"      border: 2px solid rgb(46, 52, 54);\n"
"      border-radius: 6px;     \n"
"    background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0.990925, y2:0, stop:0 rgba(0, 0, 0, 255));\n"
"  }\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: qlineargradient(spread:reflect, x1:0.579, y1:0.38561, x2:0.019, y2:0.963, stop:0.263682 rgba(0, 0, 0, 255), stop:0.698565 rgba(50, 50, 50, 255), stop:0.855721 rgba(81, 83, 79, 255))\n"
"  }")
        self.Button_8.setLocale(QtCore.QLocale(QtCore.QLocale.Czech, QtCore.QLocale.CzechRepublic))
        self.Button_8.setText("")
        icon6 = QtGui.QIcon()
        icon6.addPixmap(QtGui.QPixmap(":/icons/icon_num_8.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.Button_8.setIcon(icon6)
        self.Button_8.setIconSize(QtCore.QSize(100, 100))
        self.Button_8.setCheckable(False)
        self.Button_8.setAutoDefault(True)
        self.Button_8.setDefault(True)
        self.Button_8.setFlat(True)
        self.Button_8.setObjectName("Button_8")
        self.buttonGroup_numbers.addButton(self.Button_8)
        self.gridLayout.addWidget(self.Button_8, 3, 1, 1, 1)
        self.Button_6 = QtWidgets.QPushButton(self.centralwidget)
        self.Button_6.setLayoutDirection(QtCore.Qt.LeftToRight)
        self.Button_6.setStyleSheet(" QPushButton {\n"
"      border: 2px solid rgb(46, 52, 54);\n"
"      border-radius: 6px;     \n"
"    background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0.990925, y2:0, stop:0 rgba(0, 0, 0, 255));\n"
"  }\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: qlineargradient(spread:reflect, x1:0.579, y1:0.38561, x2:0.019, y2:0.963, stop:0.263682 rgba(0, 0, 0, 255), stop:0.698565 rgba(50, 50, 50, 255), stop:0.855721 rgba(81, 83, 79, 255))\n"
"  }")
        self.Button_6.setLocale(QtCore.QLocale(QtCore.QLocale.Czech, QtCore.QLocale.CzechRepublic))
        self.Button_6.setText("")
        icon7 = QtGui.QIcon()
        icon7.addPixmap(QtGui.QPixmap(":/icons/icon_num_6.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.Button_6.setIcon(icon7)
        self.Button_6.setIconSize(QtCore.QSize(100, 100))
        self.Button_6.setCheckable(False)
        self.Button_6.setAutoDefault(True)
        self.Button_6.setDefault(True)
        self.Button_6.setFlat(True)
        self.Button_6.setObjectName("Button_6")
        self.buttonGroup_numbers.addButton(self.Button_6)
        self.gridLayout.addWidget(self.Button_6, 4, 2, 1, 1)
        self.Button_1 = QtWidgets.QPushButton(self.centralwidget)
        self.Button_1.setLayoutDirection(QtCore.Qt.LeftToRight)
        self.Button_1.setStyleSheet(" QPushButton {\n"
"      border: 2px solid rgb(46, 52, 54);\n"
"      border-radius: 6px;     \n"
"    background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0.990925, y2:0, stop:0 rgba(0, 0, 0, 255));\n"
"  }\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: qlineargradient(spread:reflect, x1:0.579, y1:0.38561, x2:0.019, y2:0.963, stop:0.263682 rgba(0, 0, 0, 255), stop:0.698565 rgba(50, 50, 50, 255), stop:0.855721 rgba(81, 83, 79, 255))\n"
"  }")
        self.Button_1.setLocale(QtCore.QLocale(QtCore.QLocale.Czech, QtCore.QLocale.CzechRepublic))
        self.Button_1.setText("")
        icon8 = QtGui.QIcon()
        icon8.addPixmap(QtGui.QPixmap(":/icons/icon_num_1.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.Button_1.setIcon(icon8)
        self.Button_1.setIconSize(QtCore.QSize(100, 100))
        self.Button_1.setCheckable(False)
        self.Button_1.setAutoDefault(True)
        self.Button_1.setDefault(True)
        self.Button_1.setFlat(True)
        self.Button_1.setObjectName("Button_1")
        self.buttonGroup_numbers.addButton(self.Button_1)
        self.gridLayout.addWidget(self.Button_1, 5, 0, 1, 1)
        self.Button_ln = QtWidgets.QPushButton(self.centralwidget)
        self.Button_ln.setLayoutDirection(QtCore.Qt.LeftToRight)
        self.Button_ln.setStyleSheet(" QPushButton {\n"
"      border: 2px solid rgb(46, 52, 54);\n"
"      border-radius: 6px;     \n"
"    background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0.990925, y2:0, stop:0 rgba(0, 0, 0, 255));\n"
"  }\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: qlineargradient(spread:reflect, x1:0.579, y1:0.38561, x2:0.019, y2:0.963, stop:0.263682 rgba(0, 0, 0, 255), stop:0.698565 rgba(50, 50, 50, 255), stop:0.855721 rgba(81, 83, 79, 255))\n"
"  }")
        self.Button_ln.setLocale(QtCore.QLocale(QtCore.QLocale.Czech, QtCore.QLocale.CzechRepublic))
        self.Button_ln.setText("")
        icon9 = QtGui.QIcon()
        icon9.addPixmap(QtGui.QPixmap(":/icons/icon_log.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.Button_ln.setIcon(icon9)
        self.Button_ln.setIconSize(QtCore.QSize(100, 100))
        self.Button_ln.setCheckable(False)
        self.Button_ln.setAutoDefault(True)
        self.Button_ln.setDefault(True)
        self.Button_ln.setFlat(True)
        self.Button_ln.setObjectName("Button_ln")
        self.buttonGroup_other.addButton(self.Button_ln)
        self.gridLayout.addWidget(self.Button_ln, 2, 2, 1, 1)
        self.Button_pow = QtWidgets.QPushButton(self.centralwidget)
        self.Button_pow.setLayoutDirection(QtCore.Qt.LeftToRight)
        self.Button_pow.setStyleSheet(" QPushButton {\n"
"      border: 2px solid rgb(46, 52, 54);\n"
"      border-radius: 6px;     \n"
"    background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0.990925, y2:0, stop:0 rgba(0, 0, 0, 255));\n"
"  }\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: qlineargradient(spread:reflect, x1:0.579, y1:0.38561, x2:0.019, y2:0.963, stop:0.263682 rgba(0, 0, 0, 255), stop:0.698565 rgba(50, 50, 50, 255), stop:0.855721 rgba(81, 83, 79, 255))\n"
"  }")
        self.Button_pow.setLocale(QtCore.QLocale(QtCore.QLocale.Czech, QtCore.QLocale.CzechRepublic))
        self.Button_pow.setText("")
        icon10 = QtGui.QIcon()
        icon10.addPixmap(QtGui.QPixmap(":/icons/icon_pow.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.Button_pow.setIcon(icon10)
        self.Button_pow.setIconSize(QtCore.QSize(100, 100))
        self.Button_pow.setCheckable(False)
        self.Button_pow.setAutoDefault(True)
        self.Button_pow.setDefault(True)
        self.Button_pow.setFlat(True)
        self.Button_pow.setObjectName("Button_pow")
        self.buttonGroup_other.addButton(self.Button_pow)
        self.gridLayout.addWidget(self.Button_pow, 2, 1, 1, 1)
        self.Button_2 = QtWidgets.QPushButton(self.centralwidget)
        self.Button_2.setLayoutDirection(QtCore.Qt.LeftToRight)
        self.Button_2.setStyleSheet(" QPushButton {\n"
"      border: 2px solid rgb(46, 52, 54);\n"
"      border-radius: 6px;     \n"
"    background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0.990925, y2:0, stop:0 rgba(0, 0, 0, 255));\n"
"  }\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: qlineargradient(spread:reflect, x1:0.579, y1:0.38561, x2:0.019, y2:0.963, stop:0.263682 rgba(0, 0, 0, 255), stop:0.698565 rgba(50, 50, 50, 255), stop:0.855721 rgba(81, 83, 79, 255))\n"
"  }")
        self.Button_2.setLocale(QtCore.QLocale(QtCore.QLocale.Czech, QtCore.QLocale.CzechRepublic))
        self.Button_2.setText("")
        icon11 = QtGui.QIcon()
        icon11.addPixmap(QtGui.QPixmap(":/icons/icon_num_2.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.Button_2.setIcon(icon11)
        self.Button_2.setIconSize(QtCore.QSize(100, 100))
        self.Button_2.setCheckable(False)
        self.Button_2.setAutoDefault(True)
        self.Button_2.setDefault(True)
        self.Button_2.setFlat(True)
        self.Button_2.setObjectName("Button_2")
        self.buttonGroup_numbers.addButton(self.Button_2)
        self.gridLayout.addWidget(self.Button_2, 5, 1, 1, 1)
        self.Button_mul = QtWidgets.QPushButton(self.centralwidget)
        self.Button_mul.setLayoutDirection(QtCore.Qt.LeftToRight)
        self.Button_mul.setStyleSheet(" QPushButton {\n"
"      border: 2px solid rgb(46, 52, 54);\n"
"      border-radius: 6px;     \n"
"    background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0.990925, y2:0, stop:0 rgba(0, 0, 0, 255));\n"
"  }\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: qlineargradient(spread:reflect, x1:0.579, y1:0.38561, x2:0.019, y2:0.963, stop:0.263682 rgba(0, 0, 0, 255), stop:0.698565 rgba(50, 50, 50, 255), stop:0.855721 rgba(81, 83, 79, 255))\n"
"  }")
        self.Button_mul.setLocale(QtCore.QLocale(QtCore.QLocale.Czech, QtCore.QLocale.CzechRepublic))
        self.Button_mul.setText("")
        icon12 = QtGui.QIcon()
        icon12.addPixmap(QtGui.QPixmap(":/icons/icon_mul.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.Button_mul.setIcon(icon12)
        self.Button_mul.setIconSize(QtCore.QSize(100, 100))
        self.Button_mul.setCheckable(False)
        self.Button_mul.setAutoDefault(True)
        self.Button_mul.setDefault(True)
        self.Button_mul.setFlat(True)
        self.Button_mul.setObjectName("Button_mul")
        self.buttonGroup_other.addButton(self.Button_mul)
        self.gridLayout.addWidget(self.Button_mul, 3, 3, 1, 1)
        self.Button_sub = QtWidgets.QPushButton(self.centralwidget)
        self.Button_sub.setLayoutDirection(QtCore.Qt.LeftToRight)
        self.Button_sub.setStyleSheet(" QPushButton {\n"
"      border: 2px solid rgb(46, 52, 54);\n"
"      border-radius: 6px;     \n"
"    background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0.990925, y2:0, stop:0 rgba(0, 0, 0, 255));\n"
"  }\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: qlineargradient(spread:reflect, x1:0.579, y1:0.38561, x2:0.019, y2:0.963, stop:0.263682 rgba(0, 0, 0, 255), stop:0.698565 rgba(50, 50, 50, 255), stop:0.855721 rgba(81, 83, 79, 255))\n"
"  }")
        self.Button_sub.setLocale(QtCore.QLocale(QtCore.QLocale.Czech, QtCore.QLocale.CzechRepublic))
        self.Button_sub.setText("")
        icon13 = QtGui.QIcon()
        icon13.addPixmap(QtGui.QPixmap(":/icons/icon_sub.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.Button_sub.setIcon(icon13)
        self.Button_sub.setIconSize(QtCore.QSize(100, 100))
        self.Button_sub.setCheckable(False)
        self.Button_sub.setAutoDefault(True)
        self.Button_sub.setDefault(True)
        self.Button_sub.setFlat(True)
        self.Button_sub.setObjectName("Button_sub")
        self.buttonGroup_other.addButton(self.Button_sub)
        self.gridLayout.addWidget(self.Button_sub, 4, 3, 1, 1)
        self.Button_7 = QtWidgets.QPushButton(self.centralwidget)
        self.Button_7.setLayoutDirection(QtCore.Qt.LeftToRight)
        self.Button_7.setStyleSheet(" QPushButton {\n"
"      border: 2px solid rgb(46, 52, 54);\n"
"      border-radius: 6px;     \n"
"    background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0.990925, y2:0, stop:0 rgba(0, 0, 0, 255));\n"
"  }\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: qlineargradient(spread:reflect, x1:0.579, y1:0.38561, x2:0.019, y2:0.963, stop:0.263682 rgba(0, 0, 0, 255), stop:0.698565 rgba(50, 50, 50, 255), stop:0.855721 rgba(81, 83, 79, 255))\n"
"  }")
        self.Button_7.setLocale(QtCore.QLocale(QtCore.QLocale.Czech, QtCore.QLocale.CzechRepublic))
        self.Button_7.setText("")
        icon14 = QtGui.QIcon()
        icon14.addPixmap(QtGui.QPixmap(":/icons/icon_num_7.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.Button_7.setIcon(icon14)
        self.Button_7.setIconSize(QtCore.QSize(100, 100))
        self.Button_7.setCheckable(False)
        self.Button_7.setAutoDefault(True)
        self.Button_7.setDefault(True)
        self.Button_7.setFlat(True)
        self.Button_7.setObjectName("Button_7")
        self.buttonGroup_numbers.addButton(self.Button_7)
        self.gridLayout.addWidget(self.Button_7, 3, 0, 1, 1)
        self.Button_5 = QtWidgets.QPushButton(self.centralwidget)
        self.Button_5.setLayoutDirection(QtCore.Qt.LeftToRight)
        self.Button_5.setStyleSheet(" QPushButton {\n"
"      border: 2px solid rgb(46, 52, 54);\n"
"      border-radius: 6px;     \n"
"    background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0.990925, y2:0, stop:0 rgba(0, 0, 0, 255));\n"
"  }\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: qlineargradient(spread:reflect, x1:0.579, y1:0.38561, x2:0.019, y2:0.963, stop:0.263682 rgba(0, 0, 0, 255), stop:0.698565 rgba(50, 50, 50, 255), stop:0.855721 rgba(81, 83, 79, 255))\n"
"  }")
        self.Button_5.setLocale(QtCore.QLocale(QtCore.QLocale.Czech, QtCore.QLocale.CzechRepublic))
        self.Button_5.setText("")
        icon15 = QtGui.QIcon()
        icon15.addPixmap(QtGui.QPixmap(":/icons/icon_num_5.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.Button_5.setIcon(icon15)
        self.Button_5.setIconSize(QtCore.QSize(100, 100))
        self.Button_5.setCheckable(False)
        self.Button_5.setAutoDefault(True)
        self.Button_5.setDefault(True)
        self.Button_5.setFlat(True)
        self.Button_5.setObjectName("Button_5")
        self.buttonGroup_numbers.addButton(self.Button_5)
        self.gridLayout.addWidget(self.Button_5, 4, 1, 1, 1)
        self.Button_0 = QtWidgets.QPushButton(self.centralwidget)
        self.Button_0.setLayoutDirection(QtCore.Qt.LeftToRight)
        self.Button_0.setStyleSheet(" QPushButton {\n"
"      border: 2px solid rgb(46, 52, 54);\n"
"      border-radius: 6px;     \n"
"    background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0.990925, y2:0, stop:0 rgba(0, 0, 0, 255));\n"
"  }\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: qlineargradient(spread:reflect, x1:0.579, y1:0.38561, x2:0.019, y2:0.963, stop:0.263682 rgba(0, 0, 0, 255), stop:0.698565 rgba(50, 50, 50, 255), stop:0.855721 rgba(81, 83, 79, 255))\n"
"  }")
        self.Button_0.setLocale(QtCore.QLocale(QtCore.QLocale.Czech, QtCore.QLocale.CzechRepublic))
        self.Button_0.setText("")
        icon16 = QtGui.QIcon()
        icon16.addPixmap(QtGui.QPixmap(":/icons/icon_num_0.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.Button_0.setIcon(icon16)
        self.Button_0.setIconSize(QtCore.QSize(100, 100))
        self.Button_0.setCheckable(False)
        self.Button_0.setAutoDefault(True)
        self.Button_0.setDefault(True)
        self.Button_0.setFlat(True)
        self.Button_0.setObjectName("Button_0")
        self.buttonGroup_numbers.addButton(self.Button_0)
        self.gridLayout.addWidget(self.Button_0, 6, 1, 1, 1)
        self.Button_factorial = QtWidgets.QPushButton(self.centralwidget)
        self.Button_factorial.setLayoutDirection(QtCore.Qt.LeftToRight)
        self.Button_factorial.setStyleSheet(" QPushButton {\n"
"      border: 2px solid rgb(46, 52, 54);\n"
"      border-radius: 6px;     \n"
"    background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0.990925, y2:0, stop:0 rgba(0, 0, 0, 255));\n"
"  }\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: qlineargradient(spread:reflect, x1:0.579, y1:0.38561, x2:0.019, y2:0.963, stop:0.263682 rgba(0, 0, 0, 255), stop:0.698565 rgba(50, 50, 50, 255), stop:0.855721 rgba(81, 83, 79, 255))\n"
"  }")
        self.Button_factorial.setLocale(QtCore.QLocale(QtCore.QLocale.Czech, QtCore.QLocale.CzechRepublic))
        self.Button_factorial.setText("")
        icon17 = QtGui.QIcon()
        icon17.addPixmap(QtGui.QPixmap(":/icons/icon_factorial.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.Button_factorial.setIcon(icon17)
        self.Button_factorial.setIconSize(QtCore.QSize(100, 100))
        self.Button_factorial.setCheckable(False)
        self.Button_factorial.setAutoDefault(True)
        self.Button_factorial.setDefault(True)
        self.Button_factorial.setFlat(True)
        self.Button_factorial.setObjectName("Button_factorial")
        self.buttonGroup_other.addButton(self.Button_factorial)
        self.gridLayout.addWidget(self.Button_factorial, 6, 0, 1, 1)
        self.lineEdit = QtWidgets.QLineEdit(self.centralwidget)
        self.lineEdit.setMinimumSize(QtCore.QSize(0, 90))
        self.lineEdit.setSizeIncrement(QtCore.QSize(0, 2))
        self.lineEdit.setBaseSize(QtCore.QSize(0, 0))
        font = QtGui.QFont()
        font.setFamily("Uroob")
        font.setPointSize(50)
        font.setBold(True)
        font.setItalic(False)
        font.setWeight(75)
        font.setStrikeOut(False)
        font.setKerning(True)
        self.lineEdit.setFont(font)
        self.lineEdit.setCursor(QtGui.QCursor(QtCore.Qt.IBeamCursor))
        self.lineEdit.setLayoutDirection(QtCore.Qt.LeftToRight)
        self.lineEdit.setAutoFillBackground(False)
        self.lineEdit.setStyleSheet("  QLineEdit {\n"
"      border: 2px solid gray;\n"
"      border-radius: 10px;\n"
"      padding: 0 8px;\n"
"      background: rgb(17, 7, 1);\n"
"      selection-background-color: darkgray;\n"
"  }")
        self.lineEdit.setInputMask("")
        self.lineEdit.setText("")
        self.lineEdit.setMaxLength(25)
        self.lineEdit.setFrame(True)
        self.lineEdit.setCursorPosition(0)
        self.lineEdit.setAlignment(QtCore.Qt.AlignBottom|QtCore.Qt.AlignRight|QtCore.Qt.AlignTrailing)
        self.lineEdit.setDragEnabled(False)
        self.lineEdit.setReadOnly(False)
        self.lineEdit.setObjectName("lineEdit")
        self.gridLayout.addWidget(self.lineEdit, 0, 0, 1, 4)
        self.Button_div = QtWidgets.QPushButton(self.centralwidget)
        self.Button_div.setLayoutDirection(QtCore.Qt.LeftToRight)
        self.Button_div.setStyleSheet(" QPushButton {\n"
"      border: 2px solid rgb(46, 52, 54);\n"
"      border-radius: 6px;     \n"
"    background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0.990925, y2:0, stop:0 rgba(0, 0, 0, 255));\n"
"  }\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: qlineargradient(spread:reflect, x1:0.579, y1:0.38561, x2:0.019, y2:0.963, stop:0.263682 rgba(0, 0, 0, 255), stop:0.698565 rgba(50, 50, 50, 255), stop:0.855721 rgba(81, 83, 79, 255))\n"
"  }")
        self.Button_div.setLocale(QtCore.QLocale(QtCore.QLocale.Czech, QtCore.QLocale.CzechRepublic))
        self.Button_div.setText("")
        icon18 = QtGui.QIcon()
        icon18.addPixmap(QtGui.QPixmap(":/icons/icon_div.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.Button_div.setIcon(icon18)
        self.Button_div.setIconSize(QtCore.QSize(100, 100))
        self.Button_div.setCheckable(False)
        self.Button_div.setAutoDefault(True)
        self.Button_div.setDefault(True)
        self.Button_div.setFlat(True)
        self.Button_div.setObjectName("Button_div")
        self.buttonGroup_other.addButton(self.Button_div)
        self.gridLayout.addWidget(self.Button_div, 2, 3, 1, 1)
        self.Button_4 = QtWidgets.QPushButton(self.centralwidget)
        self.Button_4.setLayoutDirection(QtCore.Qt.LeftToRight)
        self.Button_4.setStyleSheet(" QPushButton {\n"
"      border: 2px solid rgb(46, 52, 54);\n"
"      border-radius: 6px;     \n"
"    background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0.990925, y2:0, stop:0 rgba(0, 0, 0, 255));\n"
"  }\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: qlineargradient(spread:reflect, x1:0.579, y1:0.38561, x2:0.019, y2:0.963, stop:0.263682 rgba(0, 0, 0, 255), stop:0.698565 rgba(50, 50, 50, 255), stop:0.855721 rgba(81, 83, 79, 255))\n"
"  }")
        self.Button_4.setLocale(QtCore.QLocale(QtCore.QLocale.Czech, QtCore.QLocale.CzechRepublic))
        self.Button_4.setText("")
        icon19 = QtGui.QIcon()
        icon19.addPixmap(QtGui.QPixmap(":/icons/icon_num_4.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.Button_4.setIcon(icon19)
        self.Button_4.setIconSize(QtCore.QSize(100, 100))
        self.Button_4.setCheckable(False)
        self.Button_4.setAutoDefault(True)
        self.Button_4.setDefault(True)
        self.Button_4.setFlat(True)
        self.Button_4.setObjectName("Button_4")
        self.buttonGroup_numbers.addButton(self.Button_4)
        self.gridLayout.addWidget(self.Button_4, 4, 0, 1, 1)
        self.Button_add = QtWidgets.QPushButton(self.centralwidget)
        self.Button_add.setLayoutDirection(QtCore.Qt.LeftToRight)
        self.Button_add.setStyleSheet(" QPushButton {\n"
"      border: 2px solid rgb(46, 52, 54);\n"
"      border-radius: 6px;     \n"
"    background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0.990925, y2:0, stop:0 rgba(0, 0, 0, 255));\n"
"  }\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: qlineargradient(spread:reflect, x1:0.579, y1:0.38561, x2:0.019, y2:0.963, stop:0.263682 rgba(0, 0, 0, 255), stop:0.698565 rgba(50, 50, 50, 255), stop:0.855721 rgba(81, 83, 79, 255))\n"
"  }")
        self.Button_add.setLocale(QtCore.QLocale(QtCore.QLocale.Czech, QtCore.QLocale.CzechRepublic))
        self.Button_add.setText("")
        icon20 = QtGui.QIcon()
        icon20.addPixmap(QtGui.QPixmap(":/icons/icon_add.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.Button_add.setIcon(icon20)
        self.Button_add.setIconSize(QtCore.QSize(100, 100))
        self.Button_add.setCheckable(False)
        self.Button_add.setAutoDefault(True)
        self.Button_add.setDefault(True)
        self.Button_add.setFlat(True)
        self.Button_add.setObjectName("Button_add")
        self.buttonGroup_other.addButton(self.Button_add)
        self.gridLayout.addWidget(self.Button_add, 5, 3, 1, 1)
        self.Button_9 = QtWidgets.QPushButton(self.centralwidget)
        self.Button_9.setLayoutDirection(QtCore.Qt.LeftToRight)
        self.Button_9.setStyleSheet(" QPushButton {\n"
"      border: 2px solid rgb(46, 52, 54);\n"
"      border-radius: 6px;     \n"
"    background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0.990925, y2:0, stop:0 rgba(0, 0, 0, 255));\n"
"  }\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: qlineargradient(spread:reflect, x1:0.579, y1:0.38561, x2:0.019, y2:0.963, stop:0.263682 rgba(0, 0, 0, 255), stop:0.698565 rgba(50, 50, 50, 255), stop:0.855721 rgba(81, 83, 79, 255))\n"
"  }")
        self.Button_9.setLocale(QtCore.QLocale(QtCore.QLocale.Czech, QtCore.QLocale.CzechRepublic))
        self.Button_9.setText("")
        icon21 = QtGui.QIcon()
        icon21.addPixmap(QtGui.QPixmap(":/icons/icon_num_9.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.Button_9.setIcon(icon21)
        self.Button_9.setIconSize(QtCore.QSize(100, 100))
        self.Button_9.setCheckable(False)
        self.Button_9.setAutoDefault(True)
        self.Button_9.setDefault(True)
        self.Button_9.setFlat(True)
        self.Button_9.setObjectName("Button_9")
        self.buttonGroup_numbers.addButton(self.Button_9)
        self.gridLayout.addWidget(self.Button_9, 3, 2, 1, 1)
        self.Button_help = QtWidgets.QPushButton(self.centralwidget)
        self.Button_help.setLayoutDirection(QtCore.Qt.LeftToRight)
        self.Button_help.setStyleSheet(" QPushButton {\n"
"      border: 2px solid black;\n"
"      border-radius: 6px; \n"
"        background-color: rgb(0, 0, 0);\n"
"  }\n"
"\n"
"\n"
"")
        self.Button_help.setText("")
        icon22 = QtGui.QIcon()
        icon22.addPixmap(QtGui.QPixmap(":/icons/napoveda.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.Button_help.setIcon(icon22)
        self.Button_help.setIconSize(QtCore.QSize(225, 100))
        self.Button_help.setObjectName("Button_help")
        self.gridLayout.addWidget(self.Button_help, 1, 2, 1, 2)
        Calculator.setCentralWidget(self.centralwidget)
        self.menubar = QtWidgets.QMenuBar(Calculator)
        self.menubar.setEnabled(True)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 408, 22))
        self.menubar.setLocale(QtCore.QLocale(QtCore.QLocale.Czech, QtCore.QLocale.CzechRepublic))
        self.menubar.setObjectName("menubar")
        Calculator.setMenuBar(self.menubar)

        self.retranslateUi(Calculator)
        QtCore.QMetaObject.connectSlotsByName(Calculator)
        Calculator.setTabOrder(self.Button_1, self.Button_2)
        Calculator.setTabOrder(self.Button_2, self.Button_3)
        Calculator.setTabOrder(self.Button_3, self.Button_4)
        Calculator.setTabOrder(self.Button_4, self.Button_5)
        Calculator.setTabOrder(self.Button_5, self.Button_6)
        Calculator.setTabOrder(self.Button_6, self.Button_7)
        Calculator.setTabOrder(self.Button_7, self.Button_8)
        Calculator.setTabOrder(self.Button_8, self.Button_9)
        Calculator.setTabOrder(self.Button_9, self.Button_0)
        Calculator.setTabOrder(self.Button_0, self.Button_comma)
        Calculator.setTabOrder(self.Button_comma, self.Button_div)
        Calculator.setTabOrder(self.Button_div, self.Button_mul)
        Calculator.setTabOrder(self.Button_mul, self.Button_sub)
        Calculator.setTabOrder(self.Button_sub, self.Button_add)
        Calculator.setTabOrder(self.Button_add, self.Button_result)
        Calculator.setTabOrder(self.Button_result, self.Button_factorial)
        Calculator.setTabOrder(self.Button_factorial, self.Button_square_root)
        Calculator.setTabOrder(self.Button_square_root, self.Button_pow)
        Calculator.setTabOrder(self.Button_pow, self.Button_ln)
        Calculator.setTabOrder(self.Button_ln, self.Button_clear_all)
        Calculator.setTabOrder(self.Button_clear_all, self.Button_delete)

    def retranslateUi(self, Calculator):
        _translate = QtCore.QCoreApplication.translate
        Calculator.setWindowTitle(_translate("Calculator", "SnakeJazzCalculator"))

import app_rc
