#include "waveformwidget.h"

#include <QColor>
#include <QPainter>

WaveformWidget::WaveformWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(320, 300);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void WaveformWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    constexpr int channelCount = 8;
    constexpr int headerHeight = 30;
    constexpr int labelWidth = 90;

    const QColor colors[channelCount] = {
        QColor("#F87171"), QColor("#FBBF24"),
        QColor("#A3E635"), QColor("#34D399"),
        QColor("#22D3EE"), QColor("#60A5FA"),
        QColor("#A78BFA"), QColor("#F472B6")
    };

    painter.fillRect(rect(), QColor("#17191D"));

    // 모든 채널이 함께 사용할 시간축 영역
    painter.fillRect(0, 0, width(), headerHeight, QColor("#292C33"));

    painter.setPen(QColor("#BFC5CF"));
    painter.drawText(
        QRect(labelWidth + 10, 0,
              width() - labelWidth - 10, headerHeight),
        Qt::AlignLeft | Qt::AlignVCenter,
        QStringLiteral("시간축 — 수집 후 표시")
        );

    const int availableHeight = height() - headerHeight;

    for (int channel = 0; channel < channelCount; ++channel) {
        const int top =
            headerHeight + availableHeight * channel / channelCount;
        const int bottom =
            headerHeight + availableHeight * (channel + 1) / channelCount;
        const int rowHeight = bottom - top;

        // 채널 이름 영역과 채널별 색상
        painter.fillRect(
            0, top, labelWidth, rowHeight, QColor("#23262D")
            );
        painter.fillRect(0, top, 4, rowHeight, colors[channel]);

        painter.setPen(colors[channel]);
        painter.drawText(
            QRect(15, top, labelWidth - 15, rowHeight),
            Qt::AlignLeft | Qt::AlignVCenter,
            QStringLiteral("CH%1").arg(channel)
            );

        // 채널 사이 구분선
        painter.setPen(QColor("#383C45"));
        painter.drawLine(0, top, width() - 1, top);
    }

    painter.setPen(QColor("#454A55"));
    painter.drawLine(labelWidth, 0, labelWidth, height() - 1);

    painter.setPen(QColor("#929AA8"));
    painter.drawText(
        QRect(labelWidth, headerHeight,
              width() - labelWidth, availableHeight),
        Qt::AlignCenter,
        QStringLiteral("수집 데이터 없음")
        );
}