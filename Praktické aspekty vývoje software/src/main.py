## @file test_math.py
#  @brief Dokumentácia pre main
#  @author Python group
#  @date April 2020
#!/usr/bin/env python3
# python moduly
from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtWidgets import QApplication
from PyQt5.QtWidgets import QMessageBox
from mat_kniznica import Library
import sys
import os
import GUI

# @class MainWindow
class MainWindow(QtWidgets.QMainWindow, GUI.Ui_Calculator):
# Metóda na setup kalkulačky
# @param self
    def __init__(self):
        super().__init__()
        self.setupUi(self)
        self.show()
##
# @desc naviazanie funkcionality na tlačidlá
        self.Button_0.clicked.connect(lambda: self.pressed(0))
        self.Button_1.clicked.connect(lambda: self.pressed(1))
        self.Button_2.clicked.connect(lambda: self.pressed(2))
        self.Button_3.clicked.connect(lambda: self.pressed(3))
        self.Button_4.clicked.connect(lambda: self.pressed(4))
        self.Button_5.clicked.connect(lambda: self.pressed(5))
        self.Button_6.clicked.connect(lambda: self.pressed(6))
        self.Button_7.clicked.connect(lambda: self.pressed(7))
        self.Button_8.clicked.connect(lambda: self.pressed(8))
        self.Button_9.clicked.connect(lambda: self.pressed(9))
        self.Button_factorial.clicked.connect(lambda: self.pressed('!'))
        self.Button_add.clicked.connect(lambda: self.pressed('+'))
        self.Button_sub.clicked.connect(lambda: self.pressed('-'))
        self.Button_mul.clicked.connect(lambda: self.pressed('*'))
        self.Button_div.clicked.connect(lambda: self.pressed('/'))
        self.Button_comma.clicked.connect(lambda: self.pressed('.'))
        self.Button_ln.clicked.connect(lambda: self.pressed('log'))
        self.Button_pow.clicked.connect(lambda: self.pressed('^'))
        self.Button_square_root.clicked.connect(lambda: self.pressed('√'))
        self.Button_clear_all.clicked.connect(lambda: self.pressed('C'))
        self.Button_delete.clicked.connect(lambda: self.pressed('D'))
        self.Button_result.clicked.connect(lambda: self.eq())
        self.Button_help.clicked.connect(lambda: self.showPopup())
##
# @desc funkcia vytvorí popup okno s nápovedou
# @param self
# @return vytvorí popup okno s nápovedou
    def showPopup(self):
        msg = QMessageBox()
        msg.setWindowTitle("Napoveda")
        msg.setText("Jednoducha kalkulacka od tymu python.\nKalkulacka ponuka okrem klasickych funkcii ako +,-,*,/ aj  vypocet faktorialu, vypocet dekadickeho logaritmu,vypocet vseobecnej odmocniny a vypocet mocniny s prirodzenym exponentom.\nKalkulacka sa ovlada stlacanim prislusnych tlacidiel.\nPre viac informacii o ovladani si precitaje pribalenu dokumentaciu. ")
        x = msg.exec_()
##
# @desc funkcia píše výraz na "displej"
# @param self, char
# @pre požiadavka o vypísanie okna
# @return vypísanie okna
    def pressed(self,char):
        display = self.lineEdit.text()
        if char=='D':
            display = display[:-1]
            self.lineEdit.setText(display)
        elif char=='C':
            self.lineEdit.setText("")
        else:
            display = self.lineEdit.text() + str(char)
            self.lineEdit.setText(display)
##
# @desc funkcia na vypísanie výsledku v prípade stlačenia =
# @param self
# @pre požiadavka na vypísanie výsledku
# @return vypísanie výsledku
    def eq(self):
        try:
            result = self.getResult(self.lineEdit.text())
        except ValueError as err:
            result = str(err)
        if len(str(result)) > 13:
            result = "{:e}".format(result)
        self.lineEdit.setText(str(result))
##
# @desc funkcia prevedie string na float alebo int
# @param self,s
# @post prevedenie stringu
# @return vráti float alebo int
    def strToNum(self,s):
        try:
            if 'e' in s:
                return float(s)
            number = s.split(".")
            if len(number) == 1 or int(number[1]) == 0:
                return int(number[0])
            else:
                return float(s)
        except ValueError:
            raise ValueError("syntax error")
##
# @desc funkcia na parsovanie vyrazu
# @param self, expression, operators
# @pre skontroluje error
# @return vráti výsledok
    def parseExpression(self,expression, operators):
        result = [expression]
        for operator in operators: 
            expression = []
            expression.extend(result)

            for m in expression:
                if operator in m:
                    edited_exp = m.split(operator)
                    i = 1

                    while i < len(edited_exp):
                        edited_exp.insert(i, operator)
                        i += 2
                    edited_exp = (filter(lambda a: a != "", edited_exp))

                    index = result.index(m)
                    result.remove(m)

                    for n in edited_exp:
                        result.insert(index, n)
                        index += 1
##
#pripojenie - k cislam
        for index, item in enumerate(result):
            if item == "-" and (index == 0 or result[index - 1] in operators):
                result[index:index + 2] = [''.join(result[index:index + 2])]
##
#kontrola na syntax error
        for index, item in enumerate(result):
##
#osetrenie operatorov na zaciatku/na konci vyrazu
            if item in operators:
                if index == 0 and item != "log" and item != "√":
                    raise ValueError("syntax error")
                if index == len(result) - 1 and item != "!":
                    raise ValueError("syntax error")
##
#osetrenie dvojitych operatorov
            if item in operators and result[index - 1] in operators:
                if index > 0 and result[index - 1] != "!" and item != "log" and item != "√":
                    raise ValueError("syntax error")
##
#osetrenie ! a log
            if item == "!" and index < len(result) - 1 and result[index + 1] not in operators:
                raise ValueError("syntax error")

            if item == "log" and index > 0 and result[index - 1] not in operators:
                raise ValueError("syntax error")

            if item not in operators:
##
#overenie ci je to platne cislo
                try:
                    float(item)
                except ValueError:
                    raise ValueError("syntax error")
        return result
##
# @desc funkcia vypočíta už upravený výraz volaním funkcií z matematickej knižnice
# @param self, raw_expression
# @pre porovná výraz s funkciami v mat. knižnici
# @return funkcia vráti vypočítaný výraz
    def getResult(self,raw_expression):
        operators = ["log", "!", "√", "^", "*", "/", "+", "-"]
        expression = self.parseExpression(raw_expression, operators)
        order = [["log"], ["!"], ["^", "√"], ["*", "/"], ["+", "-"]]
        for operator in order:
            index = 0
            while index < len(expression):
                item = expression[index]
                if item in operator:
                    if item == "+":
                        result = [str(Library.plus(self.strToNum(expression[index - 1]),
                                                      self.strToNum(expression[index + 1])))]
                        expression[index - 1:index + 2] = result
                        index -= 1

                    elif item == "-":
                        result = [str(Library.minus(self.strToNum(expression[index - 1]),
                                                      self.strToNum(expression[index + 1])))]
                        expression[index - 1:index + 2] = result
                        index -= 1

                    elif item == "*":
                        result = [str(Library.nasobenie(self.strToNum(expression[index - 1]),
                                                      self.strToNum(expression[index + 1])))]
                        expression[index - 1:index + 2] = result
                        index -= 1

                    elif item == "/":
                        result = [str(Library.delenie(self.strToNum(expression[index - 1]),
                                                      self.strToNum(expression[index + 1])))]
                        expression[index - 1:index + 2] = result
                        index -= 1

                    elif item == "^":
                        result = [str(Library.umocnovanie(self.strToNum(expression[index - 1]),
                                                        self.strToNum(expression[index + 1])))]
                        expression[index - 1:index + 2] = result
                        index -= 1

                    elif item == "√":
                        if index > 0 and not expression[index - 1] in operators:
                            result = [str(Library.odmocnina(self.strToNum(expression[index + 1]),
                                                           self.strToNum(expression[index - 1])))]
                            expression[index - 1:index + 2] = result
                            index -= 1
                        else:
                            result = [str(Library.odmocnina(self.strToNum(expression[index + 1]), 2))]
                            expression[index:index + 2] = result

                    elif item == "!":
                        result = [str(Library.faktorial(self.strToNum(expression[index - 1])))]
                        expression[index - 1:index + 1] = result
                        index -= 1

                    elif item == "log":
                        result = [str(Library.logaritmus(self.strToNum(expression[index + 1])))]
                        expression[index:index + 2] = result

                index += 1
        return self.strToNum(expression[0])
##
# @desc vytvorí výzvu na kalkulačku
def main():
    app = QApplication(sys.argv)
    form = MainWindow()
    form.show()
    app.exec_()
##
# automatic
if __name__ == '__main__':
    main()
