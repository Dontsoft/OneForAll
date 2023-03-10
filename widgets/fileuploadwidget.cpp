#include "fileuploadwidget.hpp"

#include <QDebug>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>
#include <QFileDialog>
#include <QGuiApplication>
#include <QMimeData>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QPushButton>
#include <QStandardPaths>
#include <QUrl>
#include <QVBoxLayout>

FileUploadWidget::FileUploadWidget(QWidget *parent)
    : QWidget{parent}
{
    setAcceptDrops(true);
}

void FileUploadWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        selectFile();
    }
}
void FileUploadWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->proposedAction() == Qt::CopyAction
        && event->mimeData()->hasFormat(QString("text/uri-list")))
    {
        _isCurrentlyDrag = true;
        event->acceptProposedAction();
        update();
    }
}

void FileUploadWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
    Q_UNUSED(event)
    _isCurrentlyDrag = false;
    update();
}

void FileUploadWidget::dropEvent(QDropEvent *event)
{
    if (event->dropAction() == Qt::CopyAction)
    {
        const auto fileList
            = event->mimeData()->data(QString("text/uri-list")).split('\n');
        if (!fileList.isEmpty())
        {
            const auto firstFile
                = QString::fromUtf8(fileList.constFirst()).simplified();
            emit fileChanged(QUrl(firstFile).toLocalFile());
        }
        _isCurrentlyDrag = false;
        update();
    }
}

void FileUploadWidget::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    auto palette = QGuiApplication::palette();
    QColor color;
    if (_isCurrentlyDrag)
    {
        color = QColor(Qt::green);
    }
    else
    {
        color = palette.color(QPalette::Text);
    }
    QColor colorWithTransparency = color;
    colorWithTransparency.setAlphaF(0.05);

    p.setPen(color);
    p.setBrush(colorWithTransparency);
    p.drawRect(rect());

    p.drawText(rect(), tr("Click here or drag file ..."),
               QTextOption(Qt::AlignCenter));
}

void FileUploadWidget::selectFile()
{
    const QString fileName = QFileDialog::getOpenFileName(
        nullptr, tr("Select file ..."),
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
        QString(), nullptr, QFileDialog::DontUseNativeDialog);
    if (fileName.isEmpty())
    {
        return;
    }
    emit fileChanged(fileName);
}
