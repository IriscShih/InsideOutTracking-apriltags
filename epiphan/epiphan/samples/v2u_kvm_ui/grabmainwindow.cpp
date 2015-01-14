/****************************************************************************
 *
 * $Id: grabmainwindow.cpp 20463 2013-02-04 20:58:17Z pzeldin $
 *
 * Copyright (C) 2009-2012 Epiphan Systems Inc. All rights reserved.
 *
 * Main window
 *
 ****************************************************************************/

#include <QtGui>
#include <sys/time.h>

#include "v2u_version.h"

#include "grabmainwindow.h"
#include "grabwidget.h"

#define TRACE_PREFIX "GrabMainWindow: "
#include "debugtrace.h"
#include "edid.h"

#define MAX_GRABBERS 32

// Timer ids and associated timeouts in milliseconds
#define FpsTimer(timer)     timer( iFpsTimerId,  1000 )

// Timer macros
#define _KILL(id,t) ((id) ? (killTimer(id), (id) = 0) : 0)
#define _START(id,t) ((!(id)) ? ((id) = startTimer(t)) : (id))
#define START_TIMER(timer) timer(_START)
#define KILL_TIMER(timer) timer(_KILL)

// We need QStatusBar::reformat() but it's protected
class GrabStatusBar : public QStatusBar
{
public:
    explicit GrabStatusBar(GrabMainWindow* aParent) : QStatusBar(aParent) {}
    void reformat() { QStatusBar::reformat(); }
};

#define SUPER QMainWindow

GrabMainWindow::GrabMainWindow(QWidget* aParent) :
    SUPER(aParent),
    iHistorySize(0),
    iHistoryPos(0),
    iFpsTimerId(0),
    iVideoModeStatusVisible(false),
    iFpsStatusVisible(false),
    iDefaultTitle(tr("Epiphan capture application"))
{
    // Default settings
    iOptions.kvmEvent = Qt::MidButton;
    iOptions.kvmModifier = Qt::ControlModifier;

    // Configure main window
    setWindowIcon(QIcon(":/images/icon.png"));
    setWindowTitle(iDefaultTitle);

    // Menu and toolbar
    createActions();

    // Status bar
    iStatusBar = new GrabStatusBar(this);
    if (iStatusBar) {
        iStatusBar->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
        setStatusBar(iStatusBar);

        iFpsStatus = new QLabel(),
        iVideoModeStatus = new QLabel(),
        iFpsStatus->setVisible(false);
        iVideoModeStatus->setVisible(false);

        iStatusBar->addPermanentWidget(iFpsStatus);
        iStatusBar->addPermanentWidget(iVideoModeStatus);
        iStatusBar->showMessage(tr("Ready"));
    }

    // Grab  widget
    QWidget* grabWidget = new GrabWidget(this);
    grabWidget->installEventFilter(this);
    setCentralWidget(grabWidget);

    // Start the capture thread
    iGrabThread = new GrabThread(grabWidget);
    iGrabThread->start();
}

GrabMainWindow::~GrabMainWindow()
{
    delete iGrabThread;
}

// Creates actions and connects them to the menu and toolbar
void GrabMainWindow::createActions()
{
    //QToolBar* toolBar = addToolBar(tr("ToolBar"));
    QMenuBar* mb = menuBar();
    QMenu* fileMenu = mb->addMenu(tr("&File"));
    QMenu* toolsMenu = mb->addMenu(tr("&Tools"));
    QMenu* helpMenu = mb->addMenu(tr("&Help"));
    QMenu* selectMenu = fileMenu->addMenu(tr("&Select"));

    // Selection of local grabbers
    FrmGrabber* g[MAX_GRABBERS];
    const int n = FrmGrabLocal_OpenAll(g, MAX_GRABBERS);
    if (n > 0) {
        for (int i=0; i<n; i++) {
            QString gid(QLatin1String(FrmGrab_GetId(g[i])));
            GrabberSelector* selector = new GrabberSelector(gid, this);
            QAction* action = new QAction(gid, selector);
            connect(action, SIGNAL(triggered()), selector, SLOT(open()));
            selectMenu->addAction(action);
            FrmGrab_Close(g[i]);
        }
    } else {
        QAction* none = selectMenu->addAction(tr("No frame grabbers"));
        none->setEnabled(false);
    }

    // Exit action
    QAction* exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcut(QKeySequence(tr("Ctrl+Q")));
    exitAction->setStatusTip(tr("Exit the application"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    // Connect action
    QAction* connectAction = new QAction(tr("C&onnect..."), this);
    connectAction->setShortcuts(QKeySequence::Open);
    connectAction->setStatusTip(tr("Connect other grabber"));
    connect(connectAction, SIGNAL(triggered()), this, SLOT(connectGrabber()));

    // EDID
    QAction* edidAction = new QAction(tr("&EDID..."), this);
    edidAction->setStatusTip(tr("Read/Write device EDID"));
    edidAction->setEnabled(false);
    connect(edidAction, SIGNAL(triggered()), this, SLOT(edid()));
    connect(this, SIGNAL(enableEDID(bool)), edidAction, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(grabberDisconnected(bool)), edidAction, SLOT(setDisabled(bool)));

    // Options
    QAction* optionsAction = new QAction(tr("&Options..."), this);
    optionsAction->setStatusTip(tr("Edit the application options"));
    connect(optionsAction, SIGNAL(triggered()), this, SLOT(showOptions()));

    // About action
    QAction* aboutAction = new QAction(tr("&About"), this);
    aboutAction->setStatusTip(tr("Show the application's About box"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(showAboutDialog()));

    // About Qt action
    QAction* aboutQtAction = new QAction(tr("About &Qt"), this);
    aboutQtAction->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    // Connect actions to the menu
    fileMenu->addAction(connectAction);
    fileMenu->addAction(exitAction);
    toolsMenu->addAction(edidAction);
    toolsMenu->addSeparator();
    toolsMenu->addAction(optionsAction);
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(aboutQtAction);
}

// Filters GrabWidget events
bool GrabMainWindow::eventFilter(QObject* aObject, QEvent* aEvent)
{
    switch ((GrabEventType)aEvent->type()) {
    case GrabEventOpen:
        TRACE("GrabEventOpen");
        grabOpenEvent((GrabEvent*)aEvent);
        break;
    case GrabEventClose:
        TRACE("GrabEventClose");
        grabCloseEvent((GrabEvent*)aEvent);
        break;
    case GrabEventVideoMode:
        TRACE("GrabEventVideoMode");
        grabVideoModeEvent((GrabVideoModeEvent*)aEvent);
        break;
    case GrabEventFrame:
        grabFrameEvent((GrabFrameEvent*)aEvent);
        break;
    case GrabEventConnectFailed:
        TRACE("GrabEventConnectFailed");
        grabConnectFailed((GrabConnectFailedEvent*)aEvent);
        break;
    default:
        break;
    }
    return SUPER::eventFilter(aObject, aEvent);
}

// Handles GrabEventOpen
void GrabMainWindow::grabOpenEvent(GrabEvent* aEvent)
{
    FrameGrabber* fg = aEvent->grabber();
    QString title;
    title.sprintf("%s [%s]",fg->productName(), fg->serialNumber());
    if (fg->networkGrabber()) {
        title.append(tr(" - ")).append(fg->location());
    }
    setWindowTitle(title);

    QString statusText;
    statusText.sprintf("%s %s (%s)", fg->networkGrabber() ?
        "Connected to" : "Opened", fg->serialNumber(), fg->location());
    statusBar()->showMessage(statusText);
    emit grabberConnected(true);
    emit enableEDID(fg->EDIDCapable());
}

// Handles GrabEventClose
void GrabMainWindow::grabCloseEvent(GrabEvent* /*aEvent*/)
{
    setWindowTitle(iDefaultTitle);
    setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    iHistorySize = 0;
    hideFpsStatus();
    hideVideoModeStatus();
    KILL_TIMER(FpsTimer);
    statusBar()->showMessage(tr("Ready"));
    emit grabberDisconnected(true);
}

// Handles GrabEventVideoMode
void GrabMainWindow::grabVideoModeEvent(GrabVideoModeEvent* aEvent)
{
    iHistorySize = 0;
    hideFpsStatus();

    const V2U_VideoMode* vm = aEvent->videoMode();
    if (vm->width > 0 && vm->height > 0) {
        QSize innerSize = centralWidget()->size();
        QSize outerSize = size();
        int maxWidth = vm->width + outerSize.width()-innerSize.width();
        int maxHeight = vm->height + outerSize.height()-innerSize.height();
        setMaximumSize(maxWidth, maxHeight);
        resize(maxWidth, maxHeight);

        iVideoModeStatus->setText(tr(" %1x%2 %3.%4 Hz ").
            arg(vm->width).
            arg(vm->height).
            arg((vm->vfreq+50)/1000).
            arg(((vm->vfreq+50)%1000)/100));
        showVideoModeStatus();
        emit signalDetected(true);
    } else {
        setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
        hideVideoModeStatus();
        KILL_TIMER(FpsTimer);
        emit signalLost(true);
    }
}

// Handles GrabEventFrame
void GrabMainWindow::grabFrameEvent(GrabFrameEvent* /* aEvent */)
{
    // Get current time in milliseconds since Jan 1st 1970, 00:00 GMT
    struct timeval tv;
    gettimeofday(&tv, NULL);
    iHistory[iHistoryPos++] = ((qint64)tv.tv_sec)*1000 + tv.tv_usec/1000;
    if (iHistorySize < iHistoryPos) iHistorySize = iHistoryPos;
    iHistoryPos %= FPS_HISTORY_DEPTH;
    START_TIMER(FpsTimer);
}

// Handles GrabEventConnectFailed
void GrabMainWindow::grabConnectFailed(GrabConnectFailedEvent* aEvent)
{
    statusBar()->showMessage(tr("Failed to connect to %1").
        arg(aEvent->iAddress));
}

// Timer handler
void GrabMainWindow::timerEvent(QTimerEvent* aEvent)
{
    const int timerId = aEvent->timerId();
    if (timerId == iFpsTimerId) {
        if (iHistorySize > 1) {
            qint64 from,to;
            if (iHistorySize < FPS_HISTORY_DEPTH) {
                from = iHistory[0];
                to = iHistory[iHistoryPos-1];
            } else {
                from = iHistory[iHistoryPos];
                if (iHistoryPos > 0) {
                    to = iHistory[iHistoryPos-1];
                } else {
                    to = iHistory[FPS_HISTORY_DEPTH-1];
                }
            }
            if (to > from) {
                double fps = 1000.0*iHistorySize/(to-from);
                QString text;
                text.sprintf(" %.1f fps ",fps);
                iFpsStatus->setText(text);
                showFpsStatus();
            }
        }
    } else {
        SUPER::timerEvent(aEvent);
    }
}

// Show/hide status bar widgets
void GrabMainWindow::showVideoModeStatus()
{
    if (!iVideoModeStatusVisible && iStatusBar) {
        iVideoModeStatusVisible = true;
        iVideoModeStatus->setVisible(iVideoModeStatusVisible);
        iStatusBar->reformat();
    }
}

void GrabMainWindow::hideVideoModeStatus()
{
    if (iVideoModeStatusVisible) {
        iVideoModeStatusVisible = false;
        iVideoModeStatus->setVisible(iVideoModeStatusVisible);
        iStatusBar->reformat();
    }
}

void GrabMainWindow::showFpsStatus()
{
    if (!iFpsStatusVisible && iStatusBar) {
        iFpsStatusVisible = true;
        iFpsStatus->setVisible(iFpsStatusVisible);
        iStatusBar->reformat();
    }
}

void GrabMainWindow::hideFpsStatus()
{
    if (iFpsStatusVisible) {
        iFpsStatusVisible = false;
        iFpsStatus->setVisible(iFpsStatusVisible);
        iStatusBar->reformat();
    }
}

// EDID
void GrabMainWindow::edid()
{
    // Dialog isn't responsive if grab thread is running at full speed
    int prevDelay = iGrabThread->setGrabDelay(100);
    EdidDialog eDia(iGrabThread, this);
    eDia.exec();
    iGrabThread->setGrabDelay(prevDelay);
}


// Options
void GrabMainWindow::showOptions()
{
    V2UAdjRange ar;
    GrabOptionsDialog options(&iOptions,
        (iGrabThread->getGrabParams(&iOptions.gp, &ar)) ? &ar : NULL,
        this);

    // Dialog isn't responsive if grab thread is running at full speed
    int prevDelay = iGrabThread->setGrabDelay(100);
    if (options.exec()) {
        iGrabThread->setGrabParams(iOptions.gp);
    }
    iGrabThread->setGrabDelay(prevDelay);
}

// About action
void GrabMainWindow::showAboutDialog()
{
    QString text;
    text.sprintf("<h3>Epiphan capture application %d.%d.%d</h3>"
                 "<p>Based on Qt version " QT_VERSION_STR "</p>"
                 "<p>This program demonstrates how to capture VGA/DVI "
                 "signal using Epiphan frame grabbers and control the "
                 "remote computer with a KVM enabled frame grabber.</p>"
                 "<p>Copyright (C) " V2U_PRODUCT_COPYRIGHT "<br/>"
                 "<a href=\"http://www.epiphan.com\">"
                 "http://www.epiphan.com</a></p>",
                 V2U_VERSION_MAJOR, V2U_VERSION_MINOR, V2U_VERSION_MICRO);

    // Dialog takes ages to repaint if grab thread is running at full speed
    int prevDelay = iGrabThread->setGrabDelay(100);
    QMessageBox::about(this, tr("About Epiphan capture application"), text);
    iGrabThread->setGrabDelay(prevDelay);
}

// Connect network grabber action
void GrabMainWindow::connectGrabber()
{
    bool ok = false;

    // Dialog isn't responsive if grab thread is running at full speed
    int prevDelay = iGrabThread->setGrabDelay(100);
    QString text = QInputDialog::getText(this, tr("Connect"),
        tr("Enter network address or serial number of the grabber:"),
        QLineEdit::Normal, iConnectAddress, &ok);
    iGrabThread->setGrabDelay(prevDelay);

    if (ok) {
        iConnectAddress = text;
        if (!text.isEmpty()) {
            statusBar()->showMessage(tr("Connecting to %1...").arg(text));
            iGrabThread->connectGrabber(text);
        }
    }
}

// Handler for the grabber selection menu
void GrabMainWindow::openGrabber(const QString& aSerialNumber)
{
    statusBar()->showMessage(tr("Connecting to %1...").arg(aSerialNumber));
    iGrabThread->connectGrabber(aSerialNumber);
}

/*
 * Local Variables:
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * End:
 */
