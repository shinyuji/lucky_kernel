#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    /* 파일 열기 btnOpenFile
    connect(ui->btnOpenFile, &QPushButton::clicked, this, [this]() {
        const QString filePath = QFileDialog::getOpenFileName(
            this,
            "샘플 파일 열기",
            QString(),
            "Raw 샘플 (*.raw *.bin);;모든 파일 (*)"
            );

        if (filePath.isEmpty())
            return;  // 선택을 취소한 경우

        QMessageBox::information(this, "선택한 파일", filePath);
    });
    */

}

MainWindow::~MainWindow()
{
    delete ui;
}
