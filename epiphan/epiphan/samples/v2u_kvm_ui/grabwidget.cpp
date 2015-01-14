/****************************************************************************
 *
 * $Id: grabwidget.cpp 17596 2012-07-14 11:38:37Z monich $
 *
 * Copyright (C) 2009-2012 Epiphan Systems Inc. All rights reserved.
 *
 * Widget that paints captured frames
 *
 ****************************************************************************/

#include <QtCore>
#include <QtGui>

#include "grabwidget.h"
#include "grabframe.h"
#include "grabthread.h"
#include "graboptions.h"
#include "grabmainwindow.h"

#define TRACE_PREFIX "GrabWidget: "
#include "debugtrace.h"

const int GrabWidget::Gap = 5;

#define SUPER QWidget

GrabWidget::GrabWidget(GrabMainWindow* aParent) :
    SUPER(aParent),
    iLogo(":/images/logo.png"),
    iBlankCursor(Qt::BlankCursor),
    iMainWindow(aParent),
    iGrabFrame(NULL),
    iGrabImage(NULL),
    iHaveGrabber(false),
    iHaveKVMGrabber(false),
    iKVMMode(false),
    iKVMLeftButtonDown(false),
    iKVMRightButtonDown(false),
    iSkipKVMMousePressEvent(false),
    iSkipKVMMouseReleaseEvent(false)
{
    iDefaultCursor = cursor();
    setMinimumSize(320,240);

    // Select font for the status message
    QFont f = font();
    f.setPointSize(f.pointSize()*3/2);
    f.setStyleStrategy(QFont::PreferAntialias);
    f.setStyle(QFont::StyleItalic);
    f.setWeight(QFont::Bold);
    setFont(f);

    setFocus(Qt::OtherFocusReason);
}

GrabWidget::~GrabWidget()
{
}

// Discards the current frame and frees all associated resources
void GrabWidget::discardCurrentFrame()
{
    iGrabFrame = NULL;
    if (iGrabImage) {
        delete iGrabImage;
        iGrabImage = NULL;
        repaint();
    }
}

// Handles grab events
bool GrabWidget::event(QEvent* aEvent)
{
    switch ((GrabEventType)aEvent->type()) {
    case GrabEventOpen:
        TRACE("GrabEventOpen");
        grabOpenEvent((GrabEvent*)aEvent);
        return true;
    case GrabEventClose:
        TRACE("GrabEventClose");
        grabCloseEvent((GrabEvent*)aEvent);
        return true;
    case GrabEventVideoMode:
        TRACE("GrabEventVideoMode");
        grabVideoModeEvent((GrabVideoModeEvent*)aEvent);
        return true;
    case GrabEventFrame:
        grabFrameEvent((GrabFrameEvent*)aEvent);
        return true;
    default:
        // Send everything else to the base class
        return SUPER::event(aEvent);
    }
}

// Handles GrabEventOpen
void GrabWidget::grabOpenEvent(GrabEvent* aEvent)
{
    iHaveGrabber = true;
    iHaveKVMGrabber = aEvent->grabber()->KVMCapable();
    repaint();
}

// Handles GrabEventClose
void GrabWidget::grabCloseEvent(GrabEvent* /*aEvent*/)
{
    iHaveGrabber = false;
    iHaveKVMGrabber = false;
    discardCurrentFrame();
    leaveKVMMode();
}

// Handles GrabEventVideoMode event
void GrabWidget::grabVideoModeEvent(GrabVideoModeEvent* /*aEvent*/)
{
    discardCurrentFrame();
}

// Handles GrabEventFrame event
void GrabWidget::grabFrameEvent(GrabFrameEvent* aEvent)
{
    iGrabFrame = aEvent->frame();
    if (iGrabFrame) {
        const V2U_GrabFrame2* f = iGrabFrame->frame();
        const int width = iGrabFrame->width();
        const int height = iGrabFrame->height();

        // Select QImage format
        QImage::Format format;
        switch (V2U_GRABFRAME_FORMAT(f->palette)) {
        case V2U_GRABFRAME_FORMAT_RGB24:
            format = QImage::Format_RGB888;
            break;
        case V2U_GRABFRAME_FORMAT_RGB16:
            format = QImage::Format_RGB16;
            break;
        default:
            // All other formats seem to be incompatible with QImage
            Q_ASSERT(FALSE);
            return;
        }

        if (iGrabImage && (iGrabImage->width() != width ||
                           iGrabImage->height() != height ||
                           iGrabImage->format() != format)) {
            // Image size or format mismatch, delete the old one
            delete iGrabImage;
            iGrabImage = NULL;
        }

        if (!iGrabImage) {
            // Allocate a new uninitialized image
            iGrabImage = new QImage(width, height, format);
        }

        if (iGrabImage) {
            // Copy data into the image
            const int bpp = V2UPALETTE_2_BPP(f->palette);
            const int srcLineSize = width*bpp/8;
            Q_ASSERT(!(bpp % 8));
            if (iGrabImage->bytesPerLine() == srcLineSize) {
                const uchar* data = iGrabImage->bits();
                memcpy((void*)data, f->pixbuf, srcLineSize*height);
            } else {
                const uchar* src = (const uchar*)f->pixbuf;
                for (int i=0; i<height; i++, src += srcLineSize) {
                    // Make sure to use the const variant of scanLine(int)
                    // It gives us a pointer to the actual image memory
                    const uchar* scanLine = iGrabImage->scanLine(i);
                    memcpy((void*)scanLine, src, srcLineSize);
                }
            }

            repaint();
        }

    } else {
        if (iGrabImage) {
            delete iGrabImage;
            iGrabImage = NULL;
            repaint();
        }
    }
}

// Handles paint events
void GrabWidget::paintEvent(QPaintEvent* /*aEvent*/)
{
    QPainter painter(this);

    if (iGrabImage) {

        // We have a captured image to paint
        const int imageWidth = iGrabImage->width();
        const int imageHeight = iGrabImage->height();
        int x = 0, y = 0, w = width(), h = height();
        if (w > imageWidth) {
            x = (w - imageWidth)/2;
            w = imageWidth;
        }
        if (h > imageHeight) {
            y = (h - imageHeight)/2;
            h = imageHeight;
        }

        painter.drawImage(QRect(x, y, w, h), *iGrabImage);

    } else {

        // Fill background
        painter.fillRect(rect(), palette().background());

        // Draw logo
        const int y = rect().height() - iLogo.height() - Gap;
        painter.drawPixmap(Gap, y, iLogo);

        // Draw status text
        QRect r = rect();
        r.setHeight((rect().height() + y)/2);
        const QString& status = iHaveGrabber ?
            tr("No signal detected") :
            tr("No frame grabber found");
        painter.drawText(r, status, QTextOption(Qt::AlignCenter));
    }
}

// Centers cursor
void GrabWidget::centerCursor()
{
    QCursor::setPos(mapToGlobal(QPoint(width()/2, height()/2)));
}

// Enters KVM mode
void GrabWidget::enterKVMMode()
{
    if (iHaveKVMGrabber && !iKVMMode) {
        iKVMMode = true;
        TRACE("KVM mode is on");
        setCursor(iBlankCursor);
        centerCursor();
        setMouseTracking(true);
        iSkipKVMMousePressEvent = true;
        iSkipKVMMouseReleaseEvent = true;

        // Press and unpress all modifier keys that may be used to enter
        // KVM mode to avoid the "stickiness" effect
        static const PS2KeyPress mods[] = {
            { Qt::Key_Control, true }, { Qt::Key_Control, false },
            { Qt::Key_Shift,   true }, { Qt::Key_Shift,   false },
            { Qt::Key_Alt,     true }, { Qt::Key_Alt,     false },
        };
        iMainWindow->grabThread()->sendKeyEvents(mods,
            sizeof(mods)/sizeof(mods[0]));
    }
}

// Leaves KVM mode
void GrabWidget::leaveKVMMode()
{
    if (iKVMMode) {
        iKVMMode = false;
        TRACE("KVM mode is off");
        setCursor(iDefaultCursor);
        setMouseTracking(false);
    }
}

// Checks whether or not this is a KVM trigger event and toggles
// KVM mode if necessary
void GrabWidget::toggleKVMMode(const QMouseEvent* aEvent)
{
    if (iHaveGrabber) {
        const GrabOptions* options = iMainWindow->options();
        if (aEvent->button() == options->kvmEvent &&
            (Qt::KeyboardModifierMask & aEvent->modifiers()) ==
            (Qt::KeyboardModifierMask & options->kvmModifier)) {

            // Toggle KVM mode
            if (iKVMMode) {
                leaveKVMMode();
            } else {
                enterKVMMode();
            }
        }
    }
}

// Sends mouse event to the frame grabber. Invoked in KVM mode
void GrabWidget::sendPS2MouseEvent(const QMouseEvent* aEvent)
{
    const int dx = aEvent->x() - width()/2;
    const int dy = height()/2 - aEvent->y();
    bool left = (aEvent->buttons() & Qt::LeftButton) != 0;
    bool right = (aEvent->buttons() & Qt::RightButton) != 0;
    if (dx || dy ||
        iKVMLeftButtonDown != left ||
        iKVMRightButtonDown != right) {
        iKVMLeftButtonDown = left;
        iKVMRightButtonDown = right;
        iMainWindow->grabThread()->sendMouseEvent(dx, dy, left, right);
        centerCursor();
    }
}

// Mouse events
void GrabWidget::mousePressEvent(QMouseEvent* aEvent)
{
    TRACE2("mousePressEvent(%d,%d)",aEvent->x(),aEvent->y());
    toggleKVMMode(aEvent);
    if (iKVMMode) {
        if (iSkipKVMMousePressEvent) {
            iSkipKVMMousePressEvent = false;
            TRACE("mousePressEvent skipped");
        } else {
            sendPS2MouseEvent(aEvent);
        }
    }
}

void GrabWidget::mouseReleaseEvent(QMouseEvent* aEvent)
{
    TRACE2("mouseReleaseEvent(%d,%d)",aEvent->x(),aEvent->y());
    if (iKVMMode) {
        if (iSkipKVMMouseReleaseEvent) {
            iSkipKVMMouseReleaseEvent = false;
            TRACE("mouseReleaseEvent skipped");
        } else {
            sendPS2MouseEvent(aEvent);
        }
    }
}

void GrabWidget::mouseMoveEvent(QMouseEvent* aEvent)
{
    //TRACE2("mouseMoveEvent(%d,%d)",aEvent->x(),aEvent->y());
    if (iKVMMode) {
        sendPS2MouseEvent(aEvent);
    }
}

// Key events
void GrabWidget::keyPressEvent(QKeyEvent* aEvent)
{
    TRACE1("keyPressEvent(0x%08x)",aEvent->key());
    if (iKVMMode) {
        iMainWindow->grabThread()->sendKeyEvent(aEvent->key(), true);
    }
}

void GrabWidget::keyReleaseEvent(QKeyEvent* aEvent)
{
    TRACE1("keyReleaseEvent(0x%08x)",aEvent->key());
    if (iKVMMode) {
        iMainWindow->grabThread()->sendKeyEvent(aEvent->key(), false);
    }
}

/*
 * Local Variables:
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * End:
 */
