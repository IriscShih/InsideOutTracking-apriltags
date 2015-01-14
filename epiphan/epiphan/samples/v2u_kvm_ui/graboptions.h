/****************************************************************************
 *
 * $Id: graboptions.h 16108 2012-03-24 16:39:21Z monich $
 *
 * Copyright (C) 2009-2012 Epiphan Systems Inc. All rights reserved.
 *
 * Options dialog
 *
 ****************************************************************************/

#ifndef EPIPHAN_GRABOPTIONSDIALOG_H
#define EPIPHAN_GRABOPTIONSDIALOG_H

#include <QDialog>
#include <QSlider>
#include <QComboBox>
#include <QCheckBox>
#include <QGroupBox>

#include "v2u_defs.h"

typedef struct _GrabOptions {
    Qt::MouseButton kvmEvent;
    Qt::KeyboardModifiers kvmModifier;
    V2U_GrabParameters gp;
} GrabOptions;

typedef struct _AdjustGroup {
    QGroupBox* iGroupBox;
    QSlider* iSlider;
} AdjustGroup;

// Adjustments tab
class AdjustTab : public QWidget
{
    Q_OBJECT
public:
    explicit AdjustTab(const GrabOptions* aOptions,
        const V2UAdjRange* aRange, QWidget* aParent = NULL);
    void accept(GrabOptions* aOptions);

private:
    void createOffsetGainGroup(const V2U_GrabParameters* aGp,
        const V2UAdjRange* aRange);
    static void createGroup(AdjustGroup* aGroup, const QString& aTitle,
        int aMinValue, int aMaxValue, int aValue, bool aChecked);

private:
    AdjustGroup iHShiftGroup;
    AdjustGroup iVShiftGroup;
    AdjustGroup iPLLGroup;
    AdjustGroup iPhaseGroup;

    // Offset/gain group
    QGroupBox* iOffsetGainGroupBox;
    QSlider* iOffsetSlider;
    QSlider* iGainSlider;

    QCheckBox* iWideVideoModeCheckBox;
};

// KVM tab
class KVMTab : public QWidget
{
    Q_OBJECT
public:
    explicit KVMTab(const GrabOptions* aOptions, QWidget* aParent = NULL);
    void accept(GrabOptions* aOptions);

private:
    enum { NUM_MODIFIERS = 3 };
    QCheckBox* iModifierCheckBox[NUM_MODIFIERS];
    QComboBox* iMouseEventComboBox;
};

// Options dialog
class GrabOptionsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit GrabOptionsDialog(GrabOptions* aOptions,
        const V2UAdjRange* aRange, QWidget* aParent);

private slots:
    void accept();

private:
    GrabOptions* iOptions;
    KVMTab* iKVMTab;
    AdjustTab* iAdjustTab;
};

#endif // EPIPHAN_GRABOPTIONSDIALOG_H

/*
 * Local Variables:
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * End:
 */
