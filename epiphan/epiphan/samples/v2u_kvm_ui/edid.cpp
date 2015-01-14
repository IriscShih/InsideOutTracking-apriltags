/****************************************************************************
 *
 * $Id: edid.cpp 20586 2013-02-11 21:21:49Z pzeldin $
 *
 * Copyright (C) 2013 Epiphan Systems Inc. All rights reserved.
 *
 * EDID read/write
 *
 ****************************************************************************/

#include <errno.h>

#include <QtGui>

#include "edid.h"
#include "grabthread.h"

#include "v2u_util.h"

#define TRACE_PREFIX "edid: "
#include "debugtrace.h"


#define EDID_FILTER_ASCII "ASCII EDID files (*.edid *.edid.dat)"
#define EDID_FILTER_BIN   "Binary EDID files (*.bin)"


// time to display status messages
static const int ProgressTime =     0;
static const int InfoTime     = 10000;
static const int ErrorTime    = 10000;

void EdidDialog::error(const QString &msg)
{
    if (status) status->showMessage(msg, ErrorTime);
}

void EdidDialog::info(const QString &msg)
{
    if (status) status->showMessage(msg, InfoTime);
}

void EdidDialog::progress(const QString &msg)
{
    if (status) status->showMessage(msg, ProgressTime);
}



EdidDialog::EdidDialog(GrabThread *g, QWidget *parent)
  : QDialog(parent), edid(int(V2U_EDID_SIZE), '\0'), edidDisplay(0),
    status(0), grabber(g)
{
    QVBoxLayout *layout = new QVBoxLayout;
    edidDisplay = new QTextEdit(this);
    edidDisplay->setReadOnly(true);
    layout->addWidget(edidDisplay);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok,
                                                       Qt::Horizontal);
    
    QPushButton *acquireButton = new QPushButton(tr("&Acquire"));
    acquireButton->setToolTip(tr("Read EDID from device"));
    buttonBox->addButton(acquireButton, QDialogButtonBox::ActionRole);
    
    QPushButton *uploadButton = new QPushButton(tr("&Upload"));
    uploadButton->setDisabled(true);
    uploadButton->setToolTip(tr("Write EDID to device"));
    buttonBox->addButton(uploadButton, QDialogButtonBox::ApplyRole);
    
    QPushButton *readFileButton = new QPushButton(tr("&Read"));
    readFileButton->setToolTip(tr("Read EDID from file"));
    buttonBox->addButton(readFileButton, QDialogButtonBox::ActionRole);
    
    QPushButton *writeFileButton = new QPushButton(tr("&Save"));
    writeFileButton->setDisabled(true);
    writeFileButton->setToolTip(tr("Write EDID to file"));
    buttonBox->addButton(writeFileButton, QDialogButtonBox::ActionRole);
    
    layout->addWidget(buttonBox);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    
    connect(acquireButton,   SIGNAL(clicked()), this, SLOT(acquire()));
    connect(uploadButton,    SIGNAL(clicked()), this, SLOT(upload()));
    connect(readFileButton,  SIGNAL(clicked()), this, SLOT(readFile()));
    connect(writeFileButton, SIGNAL(clicked()), this, SLOT(writeFile()));
    
    connect(this, SIGNAL(gotEdid(bool)), this,            SLOT(displayEdid(bool)));
    connect(this, SIGNAL(gotEdid(bool)), uploadButton,    SLOT(setEnabled(bool)));
    connect(this, SIGNAL(gotEdid(bool)), writeFileButton, SLOT(setEnabled(bool)));
    status = new QStatusBar(this);
    layout->addWidget(status);
    status->showMessage(tr("Press Acquire or Read button to get EDID"));
    setLayout(layout);
}


static int writer(const char* txt, V2U_OUT_STREAM* stream)
{
    QTextStream *ts = (QTextStream *)stream;
    *ts << txt;
    return 1;
}


static bool formatEdid(const char *edidBin, QString& edidTxt)
{
    QTextStream ts(&edidTxt, QIODevice::WriteOnly);
    // looks like there is no clean way to set font with fixed spacing
    ts << "<pre>";
    bool rc = v2u_edid_write_text((V2U_OUT_STREAM*)&ts, writer,
                                  (const V2U_UINT8*)edidBin) == V2U_TRUE;
    ts << "</pre>";
    return rc;
}

void EdidDialog::acquire()
{
    V2U_Property prop;
    prop.key = V2UKey_EDID;
    progress(tr("acquiring EDID.."));
    
    if (grabber->getGrabProperty(prop)) {
        edid.resize(V2U_EDID_SIZE);
        memcpy(edid.data(), prop.value.edid, V2U_EDID_SIZE);
        info(tr("EDID acquired"));
        emit gotEdid(true);
    } else {
        error(tr("error acquiring EDID"));
        emit gotEdid(false);
    }
}


void EdidDialog::displayEdid(bool b)
{
    if (b) {
        QString edidTxt;
        if (formatEdid(edid.constData(), edidTxt))
            edidDisplay->setText(edidTxt);
        else
            error(tr("error formatting EDID"));
    } else {
        edidDisplay->clear();
    }
}



void EdidDialog::writeFile()
{
    QString selectedFilter;
    QString fName = QFileDialog::getSaveFileName(this, tr("Save EDID File"),
                                                 QString(),
                                                 tr(EDID_FILTER_ASCII ";;" EDID_FILTER_BIN),
                                                 &selectedFilter);
    if (!fName.isNull()) {
        if (selectedFilter == EDID_FILTER_ASCII) {
            FILE *f = fopen(qPrintable(fName), "w");
            if (f) {
                if (v2u_edid_write(f, (V2U_UINT8*)edid.data()))
                    info(tr("EDID saved"));
                else
                    error(tr("Error writing EDID to ASCII ") +  qPrintable(fName));
                fclose(f);
            } else {
                error(tr("Error creating ") + QString().sprintf("%s: %s", qPrintable(fName), strerror(errno)));
            }
        } else if (selectedFilter == EDID_FILTER_BIN) {
            QFile f(fName);
            if (f.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
                if (f.write(edid) == V2U_EDID_SIZE) {
                    info(tr("EDID saved"));
                } else {
                    error(tr("Error writing ") + QString().sprintf("%s: %s", qPrintable(fName), strerror(errno)));
                }
            } else {
                error(tr("Error creating ") + QString().sprintf("%s: %s", qPrintable(fName), strerror(errno)));
            }
        } else {
            error("Oops filter..");
        }
    }
}


void EdidDialog::readFile()
{
    QString selectedFilter;
    QString fName = QFileDialog::getOpenFileName(this, tr("Open EDID File"),
                                                 QString(),
                                                 tr(EDID_FILTER_ASCII ";;" EDID_FILTER_BIN),
                                                 &selectedFilter);
    if (!fName.isNull()) {
        bool rc = false;
        if (selectedFilter == EDID_FILTER_ASCII) {
            FILE *f = fopen(qPrintable(fName), "r");
            if (f) {
                rc = v2u_edid_read(f, (V2U_UINT8*)edid.data());
                fclose(f);
                if (!rc) error(tr("Error reading EDID from ASCII ") +  qPrintable(fName));
            } else {
                error(tr("Error opening ") + QString().sprintf("%s: %s", qPrintable(fName), strerror(errno)));
            }
        } else if (selectedFilter == EDID_FILTER_BIN) {
            QFile f(fName);
            if (QFileInfo(f).size() == V2U_EDID_SIZE) {
                if (f.open(QIODevice::ReadOnly)) {
                    if (f.read(edid.data(), V2U_EDID_SIZE) == V2U_EDID_SIZE) {
                        rc = true;
                    } else {
                        error(tr("Error reading ") + QString().sprintf("%s: %s", qPrintable(fName), strerror(errno)));
                    }
                } else {
                    error(tr("Error opening ") + QString().sprintf("%s: %s", qPrintable(fName), strerror(errno)));
                }
            } else {
                error(tr("Invalid binary EDID file ") + qPrintable(fName));
            }
        } else {
            error("Oops filter..");
        }
        emit gotEdid(rc);
    }
}


void EdidDialog::upload()
{
    V2U_Property prop;
    prop.key = V2UKey_EDID;
    memcpy(prop.value.edid, edid.constData(), V2U_EDID_SIZE);
    progress(tr("uploading EDID.."));
    if (grabber->setGrabProperty(prop)) {
        info(tr("EDID uploaded OK"));
    } else {
        error("Error uploading EDID");
    }
}

  
/*
 * Local Variables:
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * End:
 */
