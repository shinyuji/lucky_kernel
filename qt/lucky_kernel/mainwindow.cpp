#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTcpSocket>
#include <QHostAddress>
#include <QPlainTextEdit>
#include <QPushButton>
#include <memory>

#include "waveformwidget.h"
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

    // 설정 여닫기
    connect(ui->btnToggleSettings, &QPushButton::toggled,ui->groupSettings, &QWidget::setVisible);

    //  웨이브 폼 세팅
    if (!ui->groupWaveform->layout()) { new QVBoxLayout(ui->groupWaveform); }
    ui->groupWaveform->layout()->addWidget( new WaveformWidget(ui->groupWaveform) );

    //여백 제거
    ui->groupWaveform->layout()->setContentsMargins(0, 0, 0, 0);
    ui->groupWaveform->layout()->setSpacing(0);
    ui->groupResults->layout()->setContentsMargins(0, 0, 0, 0);
    ui->groupResults->layout()->setSpacing(0);



    // TCP 소켓은 한 번 생성해서 계속 사용합니다.
    auto *tcp = new QTcpSocket(this);

    // 현재 C 에코 서버와 송수신을 확인하기 위한 시험 데이터입니다.
    const QByteArray echoTest("QT_ECHO_TEST\n");
    auto echoReceived = std::make_shared<QByteArray>();

    // 소켓 상태에 맞춰 입력칸과 버튼 상태를 갱신합니다.
    auto updateConnectionUi = [this, tcp]() {
        const auto state = tcp->state();
        const bool idle = state == QAbstractSocket::UnconnectedState;

        ui->btnConnect->setEnabled(idle);
        ui->editIP->setEnabled(idle);
        ui->editPort->setEnabled(idle);
        ui->cmbMode->setEnabled(idle);

        ui->btnDisconnect->setEnabled(
            !idle && state != QAbstractSocket::ClosingState
            );

        if (state == QAbstractSocket::ConnectedState) {
            ui->lblConnectionStatus->setText("연결됨");
        } else if (idle) {
            ui->lblConnectionStatus->setText("연결 안 됨");
        } else if (state == QAbstractSocket::ClosingState) {
            ui->lblConnectionStatus->setText("해제 중");
        } else {
            ui->lblConnectionStatus->setText("연결 중");
        }
    };

    connect(tcp, &QTcpSocket::stateChanged, this,
            [updateConnectionUi](QAbstractSocket::SocketState) {
                updateConnectionUi();
            });

    // 연결 버튼
    connect(ui->btnConnect, &QPushButton::clicked, this,
            [this, tcp]() {
                if (tcp->state() != QAbstractSocket::UnconnectedState)
                    return;

                if (ui->cmbMode->currentText() != QStringLiteral("TCP")) {
                    ui->plainLog->appendPlainText("모드를 TCP로 선택하세요.");
                    return;
                }

                const QString ip = ui->editIP->text().trimmed();
                QHostAddress address;

                bool portOk = false;
                const quint16 port =
                    ui->editPort->text().trimmed().toUShort(&portOk);

                if (!address.setAddress(ip)) {
                    ui->plainLog->appendPlainText("올바른 IP 주소를 입력하세요.");
                    return;
                }

                if (!portOk || port == 0) {
                    ui->plainLog->appendPlainText(
                        "포트는 1~65535 범위로 입력하세요."
                        );
                    return;
                }

                ui->plainLog->appendPlainText(
                    QString("연결 요청: %1:%2").arg(ip).arg(port)
                    );

                tcp->connectToHost(address, port);
            });

    // 해제 버튼: 연결 시도 중이라면 취소합니다.
    connect(ui->btnDisconnect, &QPushButton::clicked, this,
            [this, tcp]() {
                if (tcp->state() == QAbstractSocket::ConnectedState) {
                    ui->plainLog->appendPlainText("연결 해제 요청");
                    tcp->disconnectFromHost();
                } else {
                    ui->plainLog->appendPlainText("연결 시도 취소");
                    tcp->abort();
                }
            });

    // 연결 성공 후 에코 시험 데이터를 한 번 보냅니다.
    connect(tcp, &QTcpSocket::connected, this,
            [this, tcp, echoTest, echoReceived]() {
                ui->plainLog->appendPlainText("TCP 연결 성공");
                echoReceived->clear();

                const qint64 accepted = tcp->write(echoTest);

                if (accepted == echoTest.size()) {
                    ui->plainLog->appendPlainText("TX: QT_ECHO_TEST");
                } else {
                    ui->plainLog->appendPlainText(
                        "에코 시험 데이터 전송 요청 실패"
                        );
                }
            });

    // 수신 데이터는 여러 번 나뉘어 도착할 수 있으므로 모아서 비교합니다.
    connect(tcp, &QTcpSocket::readyRead, this,
            [this, tcp, echoTest, echoReceived]() {
                const QByteArray data = tcp->readAll();
                echoReceived->append(data);

                ui->plainLog->appendPlainText(
                    QString("RX (%1 bytes): %2")
                        .arg(data.size())
                        .arg(QString::fromLatin1(data.toHex(' ')))
                    );

                if (echoReceived->size() >= echoTest.size()) {
                    if (*echoReceived == echoTest) {
                        ui->plainLog->appendPlainText(
                            "[PASS] 보낸 데이터와 받은 데이터가 일치합니다."
                            );
                    } else {
                        ui->plainLog->appendPlainText(
                            "[FAIL] 에코 데이터가 일치하지 않습니다."
                            );
                    }

                    echoReceived->clear();
                }
            });

    // 연결 종료
    connect(tcp, &QTcpSocket::disconnected, this, [this]() {
        ui->plainLog->appendPlainText("TCP 연결 종료");
    });

    // 오류 원인 표시
    connect(tcp, &QTcpSocket::errorOccurred, this,
            [this, tcp](QAbstractSocket::SocketError) {
                ui->plainLog->appendPlainText(
                    "TCP 오류: " + tcp->errorString()
                    );
            });

    updateConnectionUi();

    // 실제 캡처 명령은 아직 구현하지 않았습니다.
    ui->btnStartCapture->setEnabled(false);
    ui->btnCancelCapture->setEnabled(false);


}

MainWindow::~MainWindow()
{
    delete ui;
}
