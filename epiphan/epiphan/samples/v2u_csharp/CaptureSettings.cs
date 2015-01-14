//
// $Id: CaptureSettings.cs 8286 2009-12-17 14:41:04Z monich $
//
// Copyright (C) 2009 Epiphan Systems Inc. All rights reserved.
//

using System;
using System.Windows.Forms;

using Epiphan.FrmGrab;

namespace v2u_csharp
{
    public partial class CaptureSettings : Form
    {
        private GrabParams _gp;

        public CaptureSettings(GrabParams gp)
        {
            _gp = gp;
            InitializeComponent();
            OffsetSlider.Value = _gp.Offset();
            GainSlider.Value = _gp.Gain();
            HorShiftSlider.Value = _gp.iGp.hshift;
            VerShiftSlider.Value = _gp.iGp.vshift;
            PhaseSlider.Value = _gp.iGp.phase;
            PLLSlider.Value = _gp.iGp.pllshift;

            UpdateOffsetGainLabel();
            HorShiftValueLabel.Text = String.Format("{0:d}", _gp.iGp.hshift);
            VerShiftValueLabel.Text = String.Format("{0:d}", _gp.iGp.vshift);
            PhaseValueLabel.Text = String.Format("{0:d}", _gp.iGp.phase);
            PLLValueLabel.Text = String.Format("{0:d}", _gp.iGp.pllshift);

            OffsetGainCheckBox.CheckState = gp.ManualOffsetGain() ?
                CheckState.Checked : CheckState.Unchecked;
            HorShiftCheckBox.CheckState = gp.ManualHShift() ?
                CheckState.Checked : CheckState.Unchecked;
            VerShiftCheckBox.CheckState = gp.ManualVShift() ?
                CheckState.Checked : CheckState.Unchecked;
            PhaseCheckBox.CheckState = gp.ManualPhase() ?
                CheckState.Checked : CheckState.Unchecked;
            PLLCheckBox.CheckState = gp.ManualPLL() ?
                CheckState.Checked : CheckState.Unchecked;

            EnableOffsetGainGroup(gp.ManualOffsetGain());
            EnableHorShiftGroup(gp.ManualHShift());
            EnableVerShiftGroup(gp.ManualVShift());
            EnablePhaseGroup(gp.ManualPhase());
            EnablePLLGroup(gp.ManualPLL());
        }

        //-------------------------------------------------------------------
        // Enable/disable groups of controls
        private void EnableOffsetGainGroup(bool enabled)
        {
            OffsetLabel.Enabled = enabled;
            OffsetSlider.Enabled = enabled;
            MinOffsetLabel.Enabled = enabled;
            GainSlider.Enabled = enabled;
            GainLabel.Enabled = enabled;
            MaxGainLabel.Enabled = enabled;
            MinGainLabel.Enabled = enabled;
            MaxOffsetLabel.Enabled = enabled;
            OffsetGainValueLabel.Enabled = enabled;
        }

        private void EnableHorShiftGroup(bool enabled)
        {
            HorShiftSlider.Enabled = enabled;
            MinHorShiftLabel.Enabled = enabled;
            MaxHorShiftLabel.Enabled = enabled;
            HorShiftValueLabel.Enabled = enabled;
        }

        private void EnableVerShiftGroup(bool enabled)
        {
            VerShiftSlider.Enabled = enabled;
            MinVerShiftLabel.Enabled = enabled;
            MaxVerShiftLabel.Enabled = enabled;
            VerShiftValueLabel.Enabled = enabled;
        }

        private void EnablePhaseGroup(bool enabled)
        {
            PhaseSlider.Enabled = enabled;
            MinPhaseLabel.Enabled = enabled;
            MaxPhaseLabel.Enabled = enabled;
            PhaseValueLabel.Enabled = enabled;
        }

        private void EnablePLLGroup(bool enabled)
        {
            PLLSlider.Enabled = enabled;
            MinPLLLabel.Enabled = enabled;
            MaxPLLLabel.Enabled = enabled;
            PLLValueLabel.Enabled = enabled;
        }

        private void UpdateOffsetGainLabel()
        {
            OffsetGainValueLabel.Text = String.Format("{0:d}/{1:d}",
                _gp.Offset(), _gp.Gain());
        }

        //-------------------------------------------------------------------
        // Slider scroll events
        private void OffsetSlider_Scroll(object sender, EventArgs e)
        {
            _gp.SetOffset((byte)((TrackBar)sender).Value);
            UpdateOffsetGainLabel();
        }

        private void GainSlider_Scroll(object sender, EventArgs e)
        {
            _gp.SetGain((byte)((TrackBar)sender).Value);
            UpdateOffsetGainLabel();
        }

        private void HorShiftSlider_Scroll(object sender, EventArgs e)
        {
            _gp.iGp.hshift = ((TrackBar)sender).Value;
            HorShiftValueLabel.Text = String.Format("{0:d}", _gp.iGp.hshift);
        }

        private void VerShiftSlider_Scroll(object sender, EventArgs e)
        {
            _gp.iGp.vshift = ((TrackBar)sender).Value;
            VerShiftValueLabel.Text = String.Format("{0:d}", _gp.iGp.vshift);
        }

        private void PhaseSlider_Scroll(object sender, EventArgs e)
        {
            _gp.iGp.phase = (byte)((TrackBar)sender).Value;
            PhaseValueLabel.Text = String.Format("{0:d}", _gp.iGp.phase);
        }

        private void PLLSlider_Scroll(object sender, EventArgs e)
        {
            _gp.iGp.pllshift = ((TrackBar)sender).Value;
            PLLValueLabel.Text = String.Format("{0:d}", _gp.iGp.pllshift);
        }

        //-------------------------------------------------------------------
        // Check box click events
        private void OffsetGainCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            bool manual = ((CheckBox)sender).Checked;
            _gp.SetManualOffsetGain(manual);
            EnableOffsetGainGroup(manual);
        }

        private void HorShiftCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            bool manual = ((CheckBox)sender).Checked;
            _gp.SetManualHShift(manual);
            EnableHorShiftGroup(manual);
        }

        private void VerShiftCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            bool manual = ((CheckBox)sender).Checked;
            _gp.SetManualVShift(manual);
            EnableVerShiftGroup(manual);
        }

        private void PhaseCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            bool manual = ((CheckBox)sender).Checked;
            _gp.SetManualPhase(manual);
            EnablePhaseGroup(manual);
        }

        private void PLLCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            bool manual = ((CheckBox)sender).Checked;
            _gp.SetManualPLL(manual);
            EnablePLLGroup(manual);
        }
    }
}
