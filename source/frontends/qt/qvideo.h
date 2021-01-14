#ifndef QVIDEO_H
#define QVIDEO_H

#include <QOpenGLWidget>

#define QVIDEO_BASECLASS QOpenGLWidget
//#define QVIDEO_BASECLASS QWidget

class QVideo : public QVIDEO_BASECLASS
{
    Q_OBJECT
public:
    explicit QVideo(QWidget *parent = nullptr);

    void loadVideoSettings(QImage * screenBuffer);
    void unloadVideoSettings();

signals:

public slots:

protected:
    virtual bool event(QEvent *event);

    virtual void paintEvent(QPaintEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

private:
    QImage * myFrameBuffer;

    int mySX;
    int mySY;
    int mySW;
    int mySH;
};

#endif // QVIDEO_H
