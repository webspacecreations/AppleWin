#include "StdAfx.h"
#include "qtframe.h"
#include "emulator.h"

#include "Core.h"
#include "Utilities.h"
#include "Interface.h"

#include <QMdiSubWindow>
#include <QPainter>

QtFrame::QtFrame(Emulator * emulator, QMdiSubWindow * window) : myEmulator(emulator), myWindow(window), myForceRepaint(false)
{
    myLogo = QImage(":/resources/APPLEWINLOGO.BMP").mirrored(false, true);
}

void QtFrame::SetForceRepaint(const bool force)
{
    myForceRepaint = force;
}

void QtFrame::DisplayLogo()
{
    QPainter painter(&myFrameBuffer);
    painter.drawImage(mySX, mySY, myLogo);
}

void QtFrame::VideoPresentScreen()
{
    myEmulator->refreshScreen(myForceRepaint);
}

void QtFrame::FrameRefreshStatus(int drawflags)
{
    if (drawflags & DRAW_TITLE)
    {
        GetAppleWindowTitle();
        myWindow->setWindowTitle(QString::fromStdString(g_pAppTitle));
    }
}

void QtFrame::Initialize()
{
    static_assert(sizeof(bgra_t) == 4, "Invalid size of bgra_t");
    Video & video = GetVideo();

    mySX = video.GetFrameBufferBorderWidth();
    mySY = video.GetFrameBufferBorderHeight();
    mySW = video.GetFrameBufferBorderlessWidth();
    mySH = video.GetFrameBufferBorderlessHeight();

    const int width = video.GetFrameBufferWidth();
    const int height = video.GetFrameBufferHeight();

    myFrameBuffer = QImage(width, height, QImage::Format_ARGB32_Premultiplied);

    uchar * bits = myFrameBuffer.bits();
    video.Initialize(bits);

    FrameRefreshStatus(DRAW_TITLE);
    myEmulator->loadVideoSettings(&myFrameBuffer);
    DisplayLogo();
}

void QtFrame::Destroy()
{
    LinuxFrame::Destroy();
    myEmulator->unloadVideoSettings();
}

void QtFrame::SetZoom(const int x)
{
    myEmulator->setZoom(myWindow, x);
}

void QtFrame::Set43Ratio()
{
    myEmulator->set43AspectRatio(myWindow);
}

bool QtFrame::SaveScreen(const QString & filename) const
{
    QImage screen = myFrameBuffer.copy(mySX, mySY, mySW, mySH);
    return screen.save(filename);
}
