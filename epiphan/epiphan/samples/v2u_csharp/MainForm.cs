//
// $Id: MainForm.cs 9987 2010-06-09 12:03:18Z monich $
//
// Copyright (C) 2008-2009 Epiphan Systems Inc. All rights reserved.
//

using System;
using System.Text;
using System.Drawing;
using System.Windows.Forms;

using Epiphan.FrmGrab;

namespace v2u_csharp
{
    public partial class MainForm : Form
    {
        FrameGrabber _grabber;

        public MainForm()
        {
            InitializeComponent();
            OpenGrabber();
        }

        unsafe bool OpenGrabber()
        {
            if (_grabber == null) {
                FrameGrabber grabber = null;
                try {
                    grabber = new FrameGrabber();
                    V2U_Property p;
                    p.key = V2UPropertyKey.V2UKey_Version;
                    grabber.GetProperty(&p);
                    TextOutputAdd("Found " + grabber + "\r\nDriver version " +
                        p.value.version.major+"."+ p.value.version.minor+"." +
                        p.value.version.micro+"."+ p.value.version.nano);
                } catch (Exception x) {
                    TextOutputAdd(x.Message);
                }
                SetGrabber(grabber);
            }
            return (_grabber != null);
        }

        void SetGrabber(FrameGrabber grabber)
        {
            _grabber = grabber;
            if (grabber != null) {
                Text = grabber.ToString();
                ConfigureButton.Enabled = true;
            } else {
                Text = "No grame grabber";
                ConfigureButton.Enabled = false;
            }
        }

        private void TextOutputAdd(String s)
        {
            TextOutput.Text = s + "\r\n" + TextOutput.Text;
        }

        private void GrabFrame()
        {
            if (OpenGrabber()) {
                VideoMode vm = _grabber.DetectVideoMode();
                if (vm != null) {
                    if (vm.IsValid()) {
                        Frame frame = _grabber.GrabFrame();
                        if (frame != null) {
                            Bitmap bitmap = frame.GetBitmap();
                            if (bitmap != null) {
                                TextOutputAdd("Frame captured (" + vm + ")");
                                PictureBox.Image = bitmap;
                            } else {
                                TextOutputAdd("Capture format error");
                                SetGrabber(null);
                            }
                        } else {
                            TextOutputAdd("Capture failed");
                            SetGrabber(null);
                        }
                    } else {
                        TextOutputAdd("No signal detected");
                    }
                } else {
                    TextOutputAdd("Failed to detect video mode");
                    SetGrabber(null);
                }
            }
        }

        private void CaptureFrame_Click(object sender, EventArgs e)
        {
            GrabFrame();
        }

        private void ConfigureButton_Click(object sender, EventArgs e)
        {
            if (OpenGrabber()) {
                GrabParams gp = _grabber.GetGrabParams();
                CaptureSettings dialog = new CaptureSettings(gp);
                if (dialog.ShowDialog(this) == DialogResult.OK) {
                    _grabber.SetGrabParams(gp);
                    GrabFrame();
                }
                dialog.Dispose();
            }
        }
    }
}
