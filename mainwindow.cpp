#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <math.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // digitBTNs2 = {
    //     {Qt::Key_Period, ui->btnPeriod},
    //     {Qt::Key_Backspace, ui->btnDel},
    //     {Qt::Key_Plus, ui->btnPlus},
    //     {Qt::Key_Minus, ui->btnMinus},
    // };

    // foreach (auto btn, digitBTNs2)
    //     connect(btn, SIGNAL(clicked()), this, SLOT(btnNumClicked()));

    digitBTNs = {{Qt::Key_0, ui->btnNum0},
        {Qt::Key_1, ui->btnNum1},
        {Qt::Key_2, ui->btnNum2},
        {Qt::Key_3, ui->btnNum3},
        {Qt::Key_4, ui->btnNum4},
        {Qt::Key_5, ui->btnNum5},
        {Qt::Key_6, ui->btnNum6},
        {Qt::Key_7, ui->btnNum7},
        {Qt::Key_8, ui->btnNum8},
        {Qt::Key_9, ui->btnNum9},
    };

    foreach (auto btn, digitBTNs)
        connect(btn, SIGNAL(clicked()), this, SLOT(btnNumClicked()));

    // connect(ui->btnNum0, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
    // connect(ui->btnNum1, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
    // connect(ui->btnNum2, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
    // connect(ui->btnNum3, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
    // connect(ui->btnNum4, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
    // connect(ui->btnNum5, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
    // connect(ui->btnNum6, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
    // connect(ui->btnNum7, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
    // connect(ui->btnNum8, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
    // connect(ui->btnNum9, SIGNAL(clicked()), this, SLOT(btnNumClicked()));

    connect(ui->btnMultiple, SIGNAL(clicked()), this, SLOT(btnBinaryOperatorClicked()));
    connect(ui->btnPlus, SIGNAL(clicked()), this, SLOT(btnBinaryOperatorClicked()));
    connect(ui->btnDivide, SIGNAL(clicked()), this, SLOT(btnBinaryOperatorClicked()));
    connect(ui->btnMinus, SIGNAL(clicked()), this, SLOT(btnBinaryOperatorClicked()));

    connect(ui->btnPercentage, SIGNAL(clicked()), this, SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnInverse, SIGNAL(clicked()), this, SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnSquare, SIGNAL(clicked()), this, SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnSqrt, SIGNAL(clicked()), this, SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnSign, SIGNAL(clicked()), this, SLOT(btnUnaryOperatorClicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::calculation(bool *ok)
{
    double result = 0;
    if (operands.size() >= 2 && opcodes.size() > 0) {
        //取操作数
        double operand1 = operands.front().toDouble();
        operands.pop_front();
        double operand2 = operands.front().toDouble();
        operands.pop_front();

        //取操作符
        QString op = opcodes.front();
        opcodes.pop_front();

        if (op == "+") {
            result = operand1 + operand2;
        } else if (op == "-") {
            result = operand1 - operand2;
        } else if (op == "×") {
            result = operand1 * operand2;
        } else if (op == "/") {
            result = operand1 / operand2;
        }

        operands.push_back(QString::number(result));
        // opcodes.push_back(op);

        ui->statusbar->showMessage(QString("calcuation is in progress: operand is %1, opcode is %2").arg(operands.size()).arg(
                                       opcodes.size()));
    } else
        ui->statusbar->showMessage(QString("operand is %1, opcode is %2").arg(operands.size()).arg(opcodes.size()));

    return QString::number(result);
}


void MainWindow::btnNumClicked()
{
    QString digit = qobject_cast<QPushButton *>(sender())->text();

    if (digit == "0" && operand == "0")
        digit = "";

    if (operand == "0" && digit != "0")
        operand = "";

    operand += digit;

    //QString str = ui->display->text();
    //str += qobject_cast<QPushButton *>(sender())->text();

    ui->display->setText(operand);

    //ui->statusbar->showMessage( qobject_cast<QPushButton*>(sender())->text() + "btn clicked");

}

void MainWindow::on_btnPeriod_clicked()
{
    //QString str = ui->display->text();
    if (!operand.contains("."))
        operand += qobject_cast<QPushButton * >(sender())->text();
    ui->display->setText(operand);
}


void MainWindow::on_btnDel_clicked()
{
    //QString str = ui->display->text();
    operand = operand.left(operand.length() - 1);
    ui->display->setText(operand);
}

void MainWindow::on_btnClear_clicked()
{
    operand.clear();
    ui->display->setText(operand);
}


void MainWindow::on_btnClearAll_clicked()
{
    operands.clear();
    ui->display->setText(operand);
}


void MainWindow::btnBinaryOperatorClicked()
{
    ui->statusbar->showMessage("last operand" + operand);
    QString opcode = qobject_cast<QPushButton *>(sender())->text();

    if (operand != "") {
        operands.push_back(operand);
        operand = "";

        // opcodes.push_back(opcode);
    }

    QString result = calculation();

    ui->display->setText(result);
}

void MainWindow::btnUnaryOperatorClicked()
{
    if (operand != "") {
        double result = operand.toDouble();
        operand = "";

        QString op = qobject_cast<QPushButton *>(sender())->text();

        if (op == "%")
            result /= 100.0;
        else if (op == "1/x")
            result = 1 / result;
        else if (op == "x^2")
            result *= result;
        else if (op == "√")
            result = sqrt(result);
        else if (op == "±")
            result = -result;

        operand = QString::number(result);;

        operands.push(operand);
        opcodes.push(op);

        ui->display->setText(QString::number(result));
    }

}

void MainWindow::on_btnEqual_clicked()
{
    if (operand != "") {
        operands.push_back(operand);
        operand = "";
    }

    QString result = calculation();
    ui->display->setText(result);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    foreach (auto btnKey, digitBTNs.keys()) {
        if (event->key() == btnKey)
            digitBTNs[btnKey]->animateClick();
    }

    // foreach (auto btnKey, digitBTNs2.keys()) {
    //     if (event->key() == btnKey)
    //         digitBTNs2[btnKey]->animateClick();
    // }

    if (event->key() == Qt::Key_Backspace) {
        ui->btnDel->animateClick();
    } else if (event->key() == Qt::Key_Period) {
        ui->btnPeriod->animateClick();
    } else if (event->key() == Qt::Key_Minus) {
        ui->btnMinus->animateClick();
    } else if (event->key() == Qt::Key_Plus) {
        ui->btnPlus->animateClick();
    } else if (event->key() == Qt::Key_Asterisk) {
        ui->btnMultiple->animateClick();
    } else if (event->key() == Qt::Key_Slash) {
        ui->btnDivide->animateClick();
    } else if (event->key() == Qt::Key_Percent) {
        ui->btnPercentage->animateClick();
    }
}

