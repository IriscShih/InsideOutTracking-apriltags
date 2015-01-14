/****************************************************************************
 *
 * $Id: graboptions.cpp 16107 2012-03-24 16:38:42Z monich $
 *
 * Copyright (C) 2009-2012 Epiphan Systems Inc. All rights reserved.
 *
 * Options dialog
 *
 ****************************************************************************/

#include "graboptions.h"

#include <QtGui>

//===========================================================================
// Adjustment tab
//===========================================================================

AdjustTab::AdjustTab(const GrabOptions* aOptions, const V2UAdjRange* aRange,
   QWidget* aParent) :
   QWidget(aParent),
   iOffsetGainGroupBox(NULL),
   iOffsetSlider(NULL),
   iGainSlider(NULL)
{
    const V2U_GrabParameters* gp = &aOptions->gp;

    memset(&iHShiftGroup, 0, sizeof(iHShiftGroup));
    memset(&iVShiftGroup, 0, sizeof(iVShiftGroup));
    memset(&iPLLGroup,    0, sizeof(iPLLGroup));
    memset(&iPhaseGroup,  0, sizeof(iPhaseGroup));

    createOffsetGainGroup(gp, aRange);
    createGroup(&iHShiftGroup, tr("Horizontal Shift"),
        aRange->hshift_min, aRange->hshift_max, gp->hshift,
        (gp->flags & V2U_FLAG_VALID_HSHIFT) != 0);
    createGroup(&iVShiftGroup, tr("Vertical Shift"),
        aRange->vshift_min, aRange->vshift_max, gp->vshift,
        (gp->flags & V2U_FLAG_VALID_VSHIFT) != 0);
    createGroup(&iPLLGroup, tr("PLL Shift"),
        aRange->pll_min, aRange->pll_max, gp->pllshift,
        (gp->flags & V2U_FLAG_VALID_PLLSHIFT) != 0);
    createGroup(&iPhaseGroup, tr("Phase Adjustment"),
        aRange->phase_min, aRange->phase_max, gp->phase,
        (gp->flags & V2U_FLAG_VALID_PHASE) != 0);

    QGridLayout* tabLayout = new QGridLayout(this);
    tabLayout->addWidget(iOffsetGainGroupBox,    0, 0, 1, 2);
    tabLayout->addItem(new QSpacerItem(0,0),     1, 2);
    tabLayout->setRowStretch(1, 1);
    tabLayout->addWidget(iHShiftGroup.iGroupBox, 2, 0);
    tabLayout->addWidget(iVShiftGroup.iGroupBox, 2, 1);
    tabLayout->addItem(new QSpacerItem(0,0),     3, 2);
    tabLayout->setRowStretch(3, 1);
    tabLayout->addWidget(iPLLGroup.iGroupBox,    4, 0);
    tabLayout->addWidget(iPhaseGroup.iGroupBox,  4, 1);
    tabLayout->addItem(new QSpacerItem(0,0),     5, 2);
    tabLayout->setRowStretch(5, 1);

    iWideVideoModeCheckBox = new QCheckBox(tr("Wide video mode"));
    tabLayout->addWidget(iWideVideoModeCheckBox, 6, 0, 1, 2);
    tabLayout->addItem(new QSpacerItem(0,0),     7, 2);
    tabLayout->setRowStretch(7, 1);

    iWideVideoModeCheckBox->setChecked((gp->grab_flags &
        gp->grab_flags_mask & V2U_GRAB_PREFER_WIDE_MODE) != 0);
}

void AdjustTab::createOffsetGainGroup(const V2U_GrabParameters* aGp,
    const V2UAdjRange* aRange)
{
    QString str;
    QLabel* offsetLabel = new QLabel(tr("Offset (brightness)"));
    QLabel* gainLabel = new QLabel(tr("Gain (contrast)"));
    QLabel* minOffsetLabel = new QLabel(str.setNum(aRange->offset_min));
    QLabel* maxOffsetLabel = new QLabel(str.setNum(aRange->offset_max));
    QLabel* minGainLabel = new QLabel(str.setNum(aRange->gain_min));
    QLabel* maxGainLabel = new QLabel(str.setNum(aRange->gain_max));
    offsetLabel->setAlignment(Qt::AlignCenter);
    gainLabel->setAlignment(Qt::AlignCenter);
    minOffsetLabel->setAlignment(Qt::AlignLeft|Qt::AlignTop);
    maxOffsetLabel->setAlignment(Qt::AlignRight|Qt::AlignTop);
    minGainLabel->setAlignment(Qt::AlignLeft|Qt::AlignTop);
    maxGainLabel->setAlignment(Qt::AlignRight|Qt::AlignTop);

    const int offset = (aGp->offset_r + aGp->offset_g + aGp->offset_b)/3;
    const int gain = (aGp->gain_r + aGp->gain_g + aGp->gain_b)/3;

    iOffsetSlider = new QSlider();
    iOffsetSlider->setOrientation(Qt::Horizontal);
    iOffsetSlider->setMinimum(aRange->offset_min);
    iOffsetSlider->setMaximum(aRange->offset_max);
    iOffsetSlider->setValue(offset);

    iGainSlider = new QSlider();
    iGainSlider->setOrientation(Qt::Horizontal);
    iGainSlider->setMinimum(aRange->gain_min);
    iGainSlider->setMaximum(aRange->gain_max);
    iGainSlider->setValue(gain);

    QSpinBox* offsetSpinBox = new QSpinBox();
    offsetSpinBox->setMinimum(aRange->offset_min);
    offsetSpinBox->setMaximum(aRange->offset_max);
    offsetSpinBox->setValue(offset);

    QSpinBox* gainSpinBox = new QSpinBox();
    gainSpinBox->setMinimum(aRange->gain_min);
    gainSpinBox->setMaximum(aRange->gain_max);
    gainSpinBox->setValue(gain);

    QGridLayout* grid = new QGridLayout();
    grid->addWidget(offsetLabel,   0,0,1,3);
    grid->addWidget(gainLabel,     0,4,1,3);
    grid->addWidget(iOffsetSlider, 1,0,1,3);
    grid->addWidget(iGainSlider,   1,4,1,3);
    grid->addWidget(minOffsetLabel,2,0);
    grid->addWidget(offsetSpinBox,2,1);
    grid->addWidget(maxOffsetLabel,2,2);
    grid->addWidget(minGainLabel,  2,4);
    grid->addWidget(gainSpinBox,  2,5);
    grid->addWidget(maxGainLabel,  2,6);
    grid->addItem(new QSpacerItem(10,0), 0, 3, 3, 1);
    grid->setColumnStretch(0, 1);
    grid->setColumnStretch(2, 1);
    grid->setColumnStretch(4, 1);
    grid->setColumnStretch(6, 1);

    iOffsetGainGroupBox = new QGroupBox(tr("Offset/Gain"));
    iOffsetGainGroupBox->setLayout(grid);
    iOffsetGainGroupBox->setAlignment(Qt::AlignLeft);
    iOffsetGainGroupBox->setCheckable(true);
    iOffsetGainGroupBox->setChecked((aGp->flags &
        V2U_FLAG_VALID_OFFSETGAIN) != 0);

    // Synchronize slider and spinbox
    connect(iOffsetSlider, SIGNAL(valueChanged(int)),
        offsetSpinBox, SLOT(setValue(int)));
    connect(offsetSpinBox, SIGNAL(valueChanged(int)),
        iOffsetSlider, SLOT(setValue(int)));
    connect(iGainSlider, SIGNAL(valueChanged(int)),
        gainSpinBox, SLOT(setValue(int)));
    connect(gainSpinBox, SIGNAL(valueChanged(int)),
        iGainSlider, SLOT(setValue(int)));
}

void AdjustTab::createGroup(AdjustGroup* aGroup, const QString& aTitle,
    int aMinValue, int aMaxValue, int aValue, bool aChecked)
{
    aGroup->iSlider = new QSlider();
    aGroup->iSlider->setOrientation(Qt::Horizontal);
    aGroup->iSlider->setMinimum(aMinValue);
    aGroup->iSlider->setMaximum(aMaxValue);
    aGroup->iSlider->setValue(aValue);

    QSpinBox* spinBox = new QSpinBox();
    spinBox->setMinimum(aMinValue);
    spinBox->setMaximum(aMaxValue);
    spinBox->setValue(aValue);

    QString str;
    QLabel* minLabel = new QLabel(str.setNum(aMinValue));
    QLabel* maxLabel = new QLabel(str.setNum(aMaxValue));
    minLabel->setAlignment(Qt::AlignLeft|Qt::AlignTop);
    maxLabel->setAlignment(Qt::AlignRight|Qt::AlignTop);

    QGridLayout* grid = new QGridLayout();
    grid->addWidget(aGroup->iSlider, 0, 0, 1, 3);
    grid->addWidget(minLabel, 1, 0);
    grid->addWidget(spinBox, 1, 1);
    grid->addWidget(maxLabel, 1, 2);
    grid->setColumnStretch(0, 1);
    grid->setColumnStretch(2, 1);

    aGroup->iGroupBox = new QGroupBox(aTitle);
    aGroup->iGroupBox->setLayout(grid);
    aGroup->iGroupBox->setAlignment(Qt::AlignLeft);
    aGroup->iGroupBox->setCheckable(true);
    aGroup->iGroupBox->setChecked(aChecked);

    // Synchronize slider and spinbox
    connect(aGroup->iSlider,  SIGNAL(valueChanged(int)),
        spinBox, SLOT(setValue(int)));
    connect(spinBox, SIGNAL(valueChanged(int)),
        aGroup->iSlider,SLOT(setValue(int)));
}

void AdjustTab::accept(GrabOptions* aOptions)
{
    V2U_GrabParameters* gp = &aOptions->gp;
    if (iOffsetGainGroupBox->isChecked()) {
        gp->offset_r = gp->offset_g = gp->offset_b =
            (V2U_UINT8)iOffsetSlider->value();
        gp->gain_r = gp->gain_g = gp->gain_b =
            (V2U_UINT8)iGainSlider->value();
        gp->flags |= V2U_FLAG_VALID_OFFSETGAIN;
    } else {
        gp->flags &= ~V2U_FLAG_VALID_OFFSETGAIN;
    }

    if (iHShiftGroup.iGroupBox->isChecked()) {
        gp->hshift = iHShiftGroup.iSlider->value();
        gp->flags |= V2U_FLAG_VALID_HSHIFT;
    } else {
        gp->flags &= ~V2U_FLAG_VALID_HSHIFT;
    }

    if (iVShiftGroup.iGroupBox->isChecked()) {
        gp->vshift = iVShiftGroup.iSlider->value();
        gp->flags |= V2U_FLAG_VALID_VSHIFT;
    } else {
        gp->flags &= ~V2U_FLAG_VALID_VSHIFT;
    }

    if (iPLLGroup.iGroupBox->isChecked()) {
        gp->pllshift = iPLLGroup.iSlider->value();
        gp->flags |= V2U_FLAG_VALID_PLLSHIFT;
    } else {
        gp->flags &= ~V2U_FLAG_VALID_PLLSHIFT;
    }

    if (iPhaseGroup.iGroupBox->isChecked()) {
        gp->phase = iPhaseGroup.iSlider->value();
        gp->flags |= V2U_FLAG_VALID_PHASE;
    } else {
        gp->flags &= ~V2U_FLAG_VALID_PHASE;
    }

    if (iWideVideoModeCheckBox->isChecked()) {
        gp->grab_flags |= V2U_GRAB_PREFER_WIDE_MODE;
    } else {
        gp->grab_flags &= ~V2U_GRAB_PREFER_WIDE_MODE;
    }
    gp->grab_flags_mask |= V2U_GRAB_PREFER_WIDE_MODE;
}

//===========================================================================
// KVM tab
//===========================================================================

static const struct _KVMTriggerEvents {
    Qt::MouseButton event;
    const char* label;
} kvmEvents [] = {
    { Qt::LeftButton,  "Left mouse button"   },
    { Qt::MidButton,   "Middle mouse button" },
    { Qt::RightButton, "Right mouse button"  }
};

static const struct _KVMModifiers {
    Qt::KeyboardModifier modifier;
    const char* label;
} kvmModifiers [] = {
    { Qt::ControlModifier,  "Ctrl"  },
    { Qt::AltModifier,      "Alt"   },
    { Qt::ShiftModifier,    "Shift" }
};

KVMTab::KVMTab(const GrabOptions* aOptions, QWidget* aParent) :
    QWidget(aParent),
    iMouseEventComboBox(NULL)
{
    memset(iModifierCheckBox, 0, sizeof(iModifierCheckBox));

    // Modifiers
    size_t i;
    QHBoxLayout* modifierLayout = new QHBoxLayout;
    for (i=0; i<NUM_MODIFIERS; i++) {
        iModifierCheckBox[i] = new QCheckBox(tr(kvmModifiers[i].label));
        iModifierCheckBox[i]->setChecked((kvmModifiers[i].modifier &
            aOptions->kvmModifier) != 0);
        modifierLayout->addWidget(iModifierCheckBox[i]);
    }
    modifierLayout->addStretch(1);

    QWidget* modifiers = new QWidget;
    modifiers->setLayout(modifierLayout);

    // Mouse events
    iMouseEventComboBox = new QComboBox;
    for (i=0; i<sizeof(kvmEvents)/sizeof(kvmEvents[0]); i++) {
        iMouseEventComboBox->addItem(tr(kvmEvents[i].label));
        if (kvmEvents[i].event == aOptions->kvmEvent) {
            iMouseEventComboBox->setCurrentIndex((int)i);
        }
    }

    QGridLayout* triggerLayout = new QGridLayout;
    triggerLayout->addWidget(new QLabel(tr("Mouse:")),0,0);
    triggerLayout->addWidget(new QLabel(tr("Keyboard:")),1,0);
    triggerLayout->addWidget(iMouseEventComboBox,0,1);
    triggerLayout->addWidget(modifiers,1,1);

    QGroupBox* triggerGroup = new QGroupBox(tr("Trigger event"));
    triggerGroup->setLayout(triggerLayout);

    QVBoxLayout* tabLayout = new QVBoxLayout(this);
    tabLayout->addWidget(triggerGroup);
    tabLayout->addStretch(1);
}

void KVMTab::accept(GrabOptions* aOptions)
{
    const int index = iMouseEventComboBox->currentIndex();
    if (index >= 0) aOptions->kvmEvent = kvmEvents[index].event;
    aOptions->kvmModifier = 0;
    for (int i=0; i<NUM_MODIFIERS; i++) {
        if (iModifierCheckBox[i]->checkState() == Qt::Checked) {
            aOptions->kvmModifier |= kvmModifiers[i].modifier;
        }
    }
}

//===========================================================================
// Options dialog
//===========================================================================

GrabOptionsDialog::GrabOptionsDialog(GrabOptions* aOpts,
    const V2UAdjRange* aRange, QWidget* aParent) :
    QDialog(aParent),
    iOptions(aOpts),
    iKVMTab(NULL),
    iAdjustTab(NULL)
{
    QTabWidget* tabWidget = new QTabWidget;
    iKVMTab = new KVMTab(iOptions, this);
    tabWidget->addTab(iKVMTab, tr("KVM"));
    if (aRange) {
        iAdjustTab = new AdjustTab(iOptions, aRange, this);
        tabWidget->addTab(iAdjustTab, tr("Adjustments"));
    }

    QDialogButtonBox* buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tabWidget);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    setWindowTitle(tr("Options"));
}

void GrabOptionsDialog::accept()
{
    iKVMTab->accept(iOptions);
    if (iAdjustTab) iAdjustTab->accept(iOptions);
    QDialog::accept();
}

/*
 * Local Variables:
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * End:
 */
