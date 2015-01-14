/****************************************************************************
 *
 * $Id: grabwidget.h 16094 2012-03-24 09:57:18Z monich $
 *
 * Copyright (C) 2009-2012 Epiphan Systems Inc. All rights reserved.
 *
 * Widget that paints captured frames
 *
 ****************************************************************************/

#ifndef EPIPHAN_GRABWIDGET_H
#define EPIPHAN_GRABWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QImage>

#include "grabevent.h"

class GrabMainWindow;

class GrabWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GrabWidget(GrabMainWindow* aParent);
    ~GrabWidget();

protected:
    // Event handlers
    bool event(QEvent* aEvent);
    void paintEvent(QPaintEvent* aEvent);
    void mousePressEvent(QMouseEvent* aEvent);
    void mouseReleaseEvent(QMouseEvent* aEvent);
    void mouseMoveEvent(QMouseEvent* aEvent);
    void keyPressEvent(QKeyEvent* aEvent);
    void keyReleaseEvent(QKeyEvent* aEvent);

private:
    void grabOpenEvent(GrabEvent* aEvent);
    void grabCloseEvent(GrabEvent* aEvent);
    void grabVideoModeEvent(GrabVideoModeEvent* aEvent);
    void grabFrameEvent(GrabFrameEvent* aEvent);
    void discardCurrentFrame();

    void centerCursor();
    void enterKVMMode();
    void leaveKVMMode();
    void toggleKVMMode(const QMouseEvent* aEvent);
    void sendPS2MouseEvent(const QMouseEvent* aEvent);

private:
    static const int Gap;

    QPixmap iLogo;
    QCursor iBlankCursor;
    QCursor iDefaultCursor;
    GrabMainWindow* iMainWindow;
    FrameRef iGrabFrame;
    QImage* iGrabImage;
    bool iHaveGrabber;
    bool iHaveKVMGrabber;
    bool iKVMMode;
    bool iKVMLeftButtonDown;
    bool iKVMRightButtonDown;
    bool iSkipKVMMousePressEvent;
    bool iSkipKVMMouseReleaseEvent;
};

#endif // EPIPHAN_GRABWIDGET_H

/*
 * Local Variables:
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * End:
 */
