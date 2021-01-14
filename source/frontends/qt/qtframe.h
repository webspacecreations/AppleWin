#ifndef QTFRAME_H
#define QTFRAME_H

#include "linux/linuxframe.h"
#include <QString>
#include <QImage>

class Emulator;
class QMdiSubWindow;

class QtFrame : public LinuxFrame
{
public:
    QtFrame(Emulator * emulator, QMdiSubWindow * window);

    virtual void VideoPresentScreen();
    virtual void FrameRefreshStatus(int drawflags);
    virtual void Initialize();
    virtual void Destroy();
    virtual void DisplayLogo();

    void SetForceRepaint(const bool force);
    void SetZoom(const int x);
    void Set43Ratio();

    bool SaveScreen(const QString & filename) const;

private:
    Emulator * myEmulator;
    QMdiSubWindow * myWindow;
    bool myForceRepaint;

    QImage myLogo;
    QImage myFrameBuffer;
    int mySX;
    int mySY;
    int mySW;
    int mySH;
};

#endif // QTFRAME_H
