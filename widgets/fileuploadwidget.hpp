#ifndef FILEUPLOADWIDGET_HPP
#define FILEUPLOADWIDGET_HPP

#include <QWidget>

class FileUploadWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FileUploadWidget(QWidget *parent = nullptr);

    void mouseReleaseEvent(QMouseEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
signals:
    void fileChanged(const QString &fileName);

private:
    void selectFile();

    bool _isCurrentlyDrag = false;
};

#endif // FILEUPLOADWIDGET_HPP
