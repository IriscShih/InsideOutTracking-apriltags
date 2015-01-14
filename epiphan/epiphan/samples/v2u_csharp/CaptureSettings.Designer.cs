namespace v2u_csharp
{
    partial class CaptureSettings
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null)) {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.OffsetGainValueLabel = new System.Windows.Forms.Label();
            this.GainLabel = new System.Windows.Forms.Label();
            this.MaxGainLabel = new System.Windows.Forms.Label();
            this.MinGainLabel = new System.Windows.Forms.Label();
            this.MaxOffsetLabel = new System.Windows.Forms.Label();
            this.MinOffsetLabel = new System.Windows.Forms.Label();
            this.GainSlider = new System.Windows.Forms.TrackBar();
            this.OffsetLabel = new System.Windows.Forms.Label();
            this.OffsetSlider = new System.Windows.Forms.TrackBar();
            this.OffsetGainCheckBox = new System.Windows.Forms.CheckBox();
            this.OkButton = new System.Windows.Forms.Button();
            this.MyCancelButton = new System.Windows.Forms.Button();
            this.HorShiftCheckBox = new System.Windows.Forms.CheckBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.HorShiftValueLabel = new System.Windows.Forms.Label();
            this.MaxHorShiftLabel = new System.Windows.Forms.Label();
            this.MinHorShiftLabel = new System.Windows.Forms.Label();
            this.HorShiftSlider = new System.Windows.Forms.TrackBar();
            this.VerShiftCheckBox = new System.Windows.Forms.CheckBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.VerShiftValueLabel = new System.Windows.Forms.Label();
            this.MaxVerShiftLabel = new System.Windows.Forms.Label();
            this.MinVerShiftLabel = new System.Windows.Forms.Label();
            this.VerShiftSlider = new System.Windows.Forms.TrackBar();
            this.PhaseCheckBox = new System.Windows.Forms.CheckBox();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.PhaseValueLabel = new System.Windows.Forms.Label();
            this.MaxPhaseLabel = new System.Windows.Forms.Label();
            this.MinPhaseLabel = new System.Windows.Forms.Label();
            this.PhaseSlider = new System.Windows.Forms.TrackBar();
            this.PLLCheckBox = new System.Windows.Forms.CheckBox();
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.PLLValueLabel = new System.Windows.Forms.Label();
            this.MaxPLLLabel = new System.Windows.Forms.Label();
            this.MinPLLLabel = new System.Windows.Forms.Label();
            this.PLLSlider = new System.Windows.Forms.TrackBar();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.GainSlider)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.OffsetSlider)).BeginInit();
            this.groupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.HorShiftSlider)).BeginInit();
            this.groupBox3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.VerShiftSlider)).BeginInit();
            this.groupBox4.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.PhaseSlider)).BeginInit();
            this.groupBox5.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.PLLSlider)).BeginInit();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.OffsetGainValueLabel);
            this.groupBox1.Controls.Add(this.GainLabel);
            this.groupBox1.Controls.Add(this.MaxGainLabel);
            this.groupBox1.Controls.Add(this.MinGainLabel);
            this.groupBox1.Controls.Add(this.MaxOffsetLabel);
            this.groupBox1.Controls.Add(this.MinOffsetLabel);
            this.groupBox1.Controls.Add(this.GainSlider);
            this.groupBox1.Controls.Add(this.OffsetLabel);
            this.groupBox1.Controls.Add(this.OffsetSlider);
            this.groupBox1.Location = new System.Drawing.Point(12, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(348, 86);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            // 
            // OffsetGainValueLabel
            // 
            this.OffsetGainValueLabel.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.OffsetGainValueLabel.Location = new System.Drawing.Point(292, 0);
            this.OffsetGainValueLabel.Name = "OffsetGainValueLabel";
            this.OffsetGainValueLabel.Size = new System.Drawing.Size(48, 15);
            this.OffsetGainValueLabel.TabIndex = 8;
            this.OffsetGainValueLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // GainLabel
            // 
            this.GainLabel.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.GainLabel.AutoSize = true;
            this.GainLabel.Location = new System.Drawing.Point(247, 60);
            this.GainLabel.Name = "GainLabel";
            this.GainLabel.Size = new System.Drawing.Size(29, 13);
            this.GainLabel.TabIndex = 7;
            this.GainLabel.Text = "Gain";
            this.GainLabel.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
            // 
            // MaxGainLabel
            // 
            this.MaxGainLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.MaxGainLabel.AutoSize = true;
            this.MaxGainLabel.Location = new System.Drawing.Point(315, 60);
            this.MaxGainLabel.Name = "MaxGainLabel";
            this.MaxGainLabel.Size = new System.Drawing.Size(25, 13);
            this.MaxGainLabel.TabIndex = 6;
            this.MaxGainLabel.Text = "255";
            this.MaxGainLabel.TextAlign = System.Drawing.ContentAlignment.BottomRight;
            // 
            // MinGainLabel
            // 
            this.MinGainLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.MinGainLabel.AutoSize = true;
            this.MinGainLabel.Location = new System.Drawing.Point(189, 60);
            this.MinGainLabel.Name = "MinGainLabel";
            this.MinGainLabel.Size = new System.Drawing.Size(13, 13);
            this.MinGainLabel.TabIndex = 5;
            this.MinGainLabel.Text = "0";
            this.MinGainLabel.TextAlign = System.Drawing.ContentAlignment.BottomLeft;
            // 
            // MaxOffsetLabel
            // 
            this.MaxOffsetLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.MaxOffsetLabel.AutoSize = true;
            this.MaxOffsetLabel.Location = new System.Drawing.Point(143, 60);
            this.MaxOffsetLabel.Name = "MaxOffsetLabel";
            this.MaxOffsetLabel.Size = new System.Drawing.Size(19, 13);
            this.MaxOffsetLabel.TabIndex = 4;
            this.MaxOffsetLabel.Text = "63";
            this.MaxOffsetLabel.TextAlign = System.Drawing.ContentAlignment.BottomRight;
            // 
            // MinOffsetLabel
            // 
            this.MinOffsetLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.MinOffsetLabel.AutoSize = true;
            this.MinOffsetLabel.Location = new System.Drawing.Point(13, 60);
            this.MinOffsetLabel.Name = "MinOffsetLabel";
            this.MinOffsetLabel.Size = new System.Drawing.Size(13, 13);
            this.MinOffsetLabel.TabIndex = 3;
            this.MinOffsetLabel.Text = "0";
            this.MinOffsetLabel.TextAlign = System.Drawing.ContentAlignment.BottomLeft;
            // 
            // GainSlider
            // 
            this.GainSlider.AutoSize = false;
            this.GainSlider.Location = new System.Drawing.Point(184, 23);
            this.GainSlider.Maximum = 255;
            this.GainSlider.Name = "GainSlider";
            this.GainSlider.Size = new System.Drawing.Size(160, 34);
            this.GainSlider.TabIndex = 2;
            this.GainSlider.TickFrequency = 10;
            this.GainSlider.Scroll += new System.EventHandler(this.GainSlider_Scroll);
            // 
            // OffsetLabel
            // 
            this.OffsetLabel.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.OffsetLabel.AutoSize = true;
            this.OffsetLabel.Location = new System.Drawing.Point(69, 60);
            this.OffsetLabel.Name = "OffsetLabel";
            this.OffsetLabel.Size = new System.Drawing.Size(35, 13);
            this.OffsetLabel.TabIndex = 1;
            this.OffsetLabel.Text = "Offset";
            this.OffsetLabel.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
            // 
            // OffsetSlider
            // 
            this.OffsetSlider.AutoSize = false;
            this.OffsetSlider.Location = new System.Drawing.Point(6, 23);
            this.OffsetSlider.Maximum = 63;
            this.OffsetSlider.Name = "OffsetSlider";
            this.OffsetSlider.Size = new System.Drawing.Size(160, 34);
            this.OffsetSlider.TabIndex = 0;
            this.OffsetSlider.TickFrequency = 10;
            this.OffsetSlider.Scroll += new System.EventHandler(this.OffsetSlider_Scroll);
            // 
            // OffsetGainCheckBox
            // 
            this.OffsetGainCheckBox.AutoSize = true;
            this.OffsetGainCheckBox.Location = new System.Drawing.Point(18, 12);
            this.OffsetGainCheckBox.Name = "OffsetGainCheckBox";
            this.OffsetGainCheckBox.Size = new System.Drawing.Size(179, 17);
            this.OffsetGainCheckBox.TabIndex = 0;
            this.OffsetGainCheckBox.Text = "Offset/gain (brightness/contrast)";
            this.OffsetGainCheckBox.UseVisualStyleBackColor = true;
            this.OffsetGainCheckBox.CheckedChanged += new System.EventHandler(this.OffsetGainCheckBox_CheckedChanged);
            // 
            // OkButton
            // 
            this.OkButton.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.OkButton.Location = new System.Drawing.Point(204, 293);
            this.OkButton.Name = "OkButton";
            this.OkButton.Size = new System.Drawing.Size(75, 24);
            this.OkButton.TabIndex = 1;
            this.OkButton.Text = "OK";
            this.OkButton.UseVisualStyleBackColor = true;
            // 
            // MyCancelButton
            // 
            this.MyCancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.MyCancelButton.Location = new System.Drawing.Point(285, 293);
            this.MyCancelButton.Name = "MyCancelButton";
            this.MyCancelButton.Size = new System.Drawing.Size(75, 24);
            this.MyCancelButton.TabIndex = 2;
            this.MyCancelButton.Text = "Cancel";
            this.MyCancelButton.UseVisualStyleBackColor = true;
            // 
            // HorShiftCheckBox
            // 
            this.HorShiftCheckBox.AutoSize = true;
            this.HorShiftCheckBox.Location = new System.Drawing.Point(18, 104);
            this.HorShiftCheckBox.Name = "HorShiftCheckBox";
            this.HorShiftCheckBox.Size = new System.Drawing.Size(95, 17);
            this.HorShiftCheckBox.TabIndex = 4;
            this.HorShiftCheckBox.Text = "Horizontal shift";
            this.HorShiftCheckBox.UseVisualStyleBackColor = true;
            this.HorShiftCheckBox.CheckedChanged += new System.EventHandler(this.HorShiftCheckBox_CheckedChanged);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.HorShiftValueLabel);
            this.groupBox2.Controls.Add(this.MaxHorShiftLabel);
            this.groupBox2.Controls.Add(this.MinHorShiftLabel);
            this.groupBox2.Controls.Add(this.HorShiftSlider);
            this.groupBox2.Location = new System.Drawing.Point(12, 104);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(170, 86);
            this.groupBox2.TabIndex = 3;
            this.groupBox2.TabStop = false;
            // 
            // HorShiftValueLabel
            // 
            this.HorShiftValueLabel.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.HorShiftValueLabel.Location = new System.Drawing.Point(130, 0);
            this.HorShiftValueLabel.Name = "HorShiftValueLabel";
            this.HorShiftValueLabel.Size = new System.Drawing.Size(32, 15);
            this.HorShiftValueLabel.TabIndex = 8;
            this.HorShiftValueLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // MaxHorShiftLabel
            // 
            this.MaxHorShiftLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.MaxHorShiftLabel.AutoSize = true;
            this.MaxHorShiftLabel.Location = new System.Drawing.Point(138, 60);
            this.MaxHorShiftLabel.Name = "MaxHorShiftLabel";
            this.MaxHorShiftLabel.Size = new System.Drawing.Size(25, 13);
            this.MaxHorShiftLabel.TabIndex = 4;
            this.MaxHorShiftLabel.Text = "200";
            this.MaxHorShiftLabel.TextAlign = System.Drawing.ContentAlignment.BottomRight;
            // 
            // MinHorShiftLabel
            // 
            this.MinHorShiftLabel.AutoSize = true;
            this.MinHorShiftLabel.Location = new System.Drawing.Point(13, 60);
            this.MinHorShiftLabel.Name = "MinHorShiftLabel";
            this.MinHorShiftLabel.Size = new System.Drawing.Size(28, 13);
            this.MinHorShiftLabel.TabIndex = 3;
            this.MinHorShiftLabel.Text = "-200";
            this.MinHorShiftLabel.TextAlign = System.Drawing.ContentAlignment.BottomLeft;
            // 
            // HorShiftSlider
            // 
            this.HorShiftSlider.AutoSize = false;
            this.HorShiftSlider.Location = new System.Drawing.Point(6, 23);
            this.HorShiftSlider.Maximum = 200;
            this.HorShiftSlider.Minimum = -200;
            this.HorShiftSlider.Name = "HorShiftSlider";
            this.HorShiftSlider.Size = new System.Drawing.Size(160, 34);
            this.HorShiftSlider.TabIndex = 0;
            this.HorShiftSlider.TickFrequency = 20;
            this.HorShiftSlider.Scroll += new System.EventHandler(this.HorShiftSlider_Scroll);
            // 
            // VerShiftCheckBox
            // 
            this.VerShiftCheckBox.AutoSize = true;
            this.VerShiftCheckBox.Location = new System.Drawing.Point(196, 104);
            this.VerShiftCheckBox.Name = "VerShiftCheckBox";
            this.VerShiftCheckBox.Size = new System.Drawing.Size(83, 17);
            this.VerShiftCheckBox.TabIndex = 6;
            this.VerShiftCheckBox.Text = "Vertical shift";
            this.VerShiftCheckBox.UseVisualStyleBackColor = true;
            this.VerShiftCheckBox.CheckedChanged += new System.EventHandler(this.VerShiftCheckBox_CheckedChanged);
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.VerShiftValueLabel);
            this.groupBox3.Controls.Add(this.MaxVerShiftLabel);
            this.groupBox3.Controls.Add(this.MinVerShiftLabel);
            this.groupBox3.Controls.Add(this.VerShiftSlider);
            this.groupBox3.Location = new System.Drawing.Point(190, 104);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(170, 86);
            this.groupBox3.TabIndex = 5;
            this.groupBox3.TabStop = false;
            // 
            // VerShiftValueLabel
            // 
            this.VerShiftValueLabel.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.VerShiftValueLabel.Location = new System.Drawing.Point(130, 0);
            this.VerShiftValueLabel.Name = "VerShiftValueLabel";
            this.VerShiftValueLabel.Size = new System.Drawing.Size(32, 15);
            this.VerShiftValueLabel.TabIndex = 8;
            this.VerShiftValueLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // MaxVerShiftLabel
            // 
            this.MaxVerShiftLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.MaxVerShiftLabel.AutoSize = true;
            this.MaxVerShiftLabel.Location = new System.Drawing.Point(138, 60);
            this.MaxVerShiftLabel.Name = "MaxVerShiftLabel";
            this.MaxVerShiftLabel.Size = new System.Drawing.Size(19, 13);
            this.MaxVerShiftLabel.TabIndex = 4;
            this.MaxVerShiftLabel.Text = "35";
            this.MaxVerShiftLabel.TextAlign = System.Drawing.ContentAlignment.BottomRight;
            // 
            // MinVerShiftLabel
            // 
            this.MinVerShiftLabel.AutoSize = true;
            this.MinVerShiftLabel.Location = new System.Drawing.Point(13, 60);
            this.MinVerShiftLabel.Name = "MinVerShiftLabel";
            this.MinVerShiftLabel.Size = new System.Drawing.Size(22, 13);
            this.MinVerShiftLabel.TabIndex = 3;
            this.MinVerShiftLabel.Text = "-35";
            this.MinVerShiftLabel.TextAlign = System.Drawing.ContentAlignment.BottomLeft;
            // 
            // VerShiftSlider
            // 
            this.VerShiftSlider.AutoSize = false;
            this.VerShiftSlider.Location = new System.Drawing.Point(6, 23);
            this.VerShiftSlider.Maximum = 35;
            this.VerShiftSlider.Minimum = -35;
            this.VerShiftSlider.Name = "VerShiftSlider";
            this.VerShiftSlider.Size = new System.Drawing.Size(160, 34);
            this.VerShiftSlider.TabIndex = 0;
            this.VerShiftSlider.TickFrequency = 5;
            this.VerShiftSlider.Scroll += new System.EventHandler(this.VerShiftSlider_Scroll);
            // 
            // PhaseCheckBox
            // 
            this.PhaseCheckBox.AutoSize = true;
            this.PhaseCheckBox.Location = new System.Drawing.Point(18, 196);
            this.PhaseCheckBox.Name = "PhaseCheckBox";
            this.PhaseCheckBox.Size = new System.Drawing.Size(101, 17);
            this.PhaseCheckBox.TabIndex = 8;
            this.PhaseCheckBox.Text = "Sampling phase";
            this.PhaseCheckBox.UseVisualStyleBackColor = true;
            this.PhaseCheckBox.CheckedChanged += new System.EventHandler(this.PhaseCheckBox_CheckedChanged);
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.PhaseValueLabel);
            this.groupBox4.Controls.Add(this.MaxPhaseLabel);
            this.groupBox4.Controls.Add(this.MinPhaseLabel);
            this.groupBox4.Controls.Add(this.PhaseSlider);
            this.groupBox4.Location = new System.Drawing.Point(12, 196);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(170, 86);
            this.groupBox4.TabIndex = 7;
            this.groupBox4.TabStop = false;
            // 
            // PhaseValueLabel
            // 
            this.PhaseValueLabel.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.PhaseValueLabel.Location = new System.Drawing.Point(130, 0);
            this.PhaseValueLabel.Name = "PhaseValueLabel";
            this.PhaseValueLabel.Size = new System.Drawing.Size(32, 15);
            this.PhaseValueLabel.TabIndex = 8;
            this.PhaseValueLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // MaxPhaseLabel
            // 
            this.MaxPhaseLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.MaxPhaseLabel.AutoSize = true;
            this.MaxPhaseLabel.Location = new System.Drawing.Point(138, 60);
            this.MaxPhaseLabel.Name = "MaxPhaseLabel";
            this.MaxPhaseLabel.Size = new System.Drawing.Size(19, 13);
            this.MaxPhaseLabel.TabIndex = 4;
            this.MaxPhaseLabel.Text = "31";
            this.MaxPhaseLabel.TextAlign = System.Drawing.ContentAlignment.BottomRight;
            // 
            // MinPhaseLabel
            // 
            this.MinPhaseLabel.AutoSize = true;
            this.MinPhaseLabel.Location = new System.Drawing.Point(13, 60);
            this.MinPhaseLabel.Name = "MinPhaseLabel";
            this.MinPhaseLabel.Size = new System.Drawing.Size(13, 13);
            this.MinPhaseLabel.TabIndex = 3;
            this.MinPhaseLabel.Text = "0";
            this.MinPhaseLabel.TextAlign = System.Drawing.ContentAlignment.BottomLeft;
            // 
            // PhaseSlider
            // 
            this.PhaseSlider.AutoSize = false;
            this.PhaseSlider.Location = new System.Drawing.Point(6, 23);
            this.PhaseSlider.Maximum = 31;
            this.PhaseSlider.Name = "PhaseSlider";
            this.PhaseSlider.Size = new System.Drawing.Size(160, 34);
            this.PhaseSlider.TabIndex = 0;
            this.PhaseSlider.TickFrequency = 2;
            this.PhaseSlider.Scroll += new System.EventHandler(this.PhaseSlider_Scroll);
            // 
            // PLLCheckBox
            // 
            this.PLLCheckBox.AutoSize = true;
            this.PLLCheckBox.Location = new System.Drawing.Point(196, 196);
            this.PLLCheckBox.Name = "PLLCheckBox";
            this.PLLCheckBox.Size = new System.Drawing.Size(99, 17);
            this.PLLCheckBox.TabIndex = 10;
            this.PLLCheckBox.Text = "PLL adjustment";
            this.PLLCheckBox.UseVisualStyleBackColor = true;
            this.PLLCheckBox.CheckedChanged += new System.EventHandler(this.PLLCheckBox_CheckedChanged);
            // 
            // groupBox5
            // 
            this.groupBox5.Controls.Add(this.PLLValueLabel);
            this.groupBox5.Controls.Add(this.MaxPLLLabel);
            this.groupBox5.Controls.Add(this.MinPLLLabel);
            this.groupBox5.Controls.Add(this.PLLSlider);
            this.groupBox5.Location = new System.Drawing.Point(190, 196);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Size = new System.Drawing.Size(170, 86);
            this.groupBox5.TabIndex = 9;
            this.groupBox5.TabStop = false;
            // 
            // PLLValueLabel
            // 
            this.PLLValueLabel.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.PLLValueLabel.Location = new System.Drawing.Point(131, 0);
            this.PLLValueLabel.Name = "PLLValueLabel";
            this.PLLValueLabel.Size = new System.Drawing.Size(31, 15);
            this.PLLValueLabel.TabIndex = 8;
            this.PLLValueLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // MaxPLLLabel
            // 
            this.MaxPLLLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.MaxPLLLabel.AutoSize = true;
            this.MaxPLLLabel.Location = new System.Drawing.Point(138, 60);
            this.MaxPLLLabel.Name = "MaxPLLLabel";
            this.MaxPLLLabel.Size = new System.Drawing.Size(25, 13);
            this.MaxPLLLabel.TabIndex = 4;
            this.MaxPLLLabel.Text = "300";
            this.MaxPLLLabel.TextAlign = System.Drawing.ContentAlignment.BottomRight;
            // 
            // MinPLLLabel
            // 
            this.MinPLLLabel.AutoSize = true;
            this.MinPLLLabel.Location = new System.Drawing.Point(13, 60);
            this.MinPLLLabel.Name = "MinPLLLabel";
            this.MinPLLLabel.Size = new System.Drawing.Size(28, 13);
            this.MinPLLLabel.TabIndex = 3;
            this.MinPLLLabel.Text = "-300";
            this.MinPLLLabel.TextAlign = System.Drawing.ContentAlignment.BottomLeft;
            // 
            // PLLSlider
            // 
            this.PLLSlider.AutoSize = false;
            this.PLLSlider.Location = new System.Drawing.Point(6, 23);
            this.PLLSlider.Maximum = 300;
            this.PLLSlider.Minimum = -300;
            this.PLLSlider.Name = "PLLSlider";
            this.PLLSlider.Size = new System.Drawing.Size(160, 34);
            this.PLLSlider.TabIndex = 0;
            this.PLLSlider.TickFrequency = 30;
            this.PLLSlider.Scroll += new System.EventHandler(this.PLLSlider_Scroll);
            // 
            // CaptureSettings
            // 
            this.AcceptButton = this.OkButton;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.MyCancelButton;
            this.ClientSize = new System.Drawing.Size(372, 328);
            this.Controls.Add(this.PLLCheckBox);
            this.Controls.Add(this.PhaseCheckBox);
            this.Controls.Add(this.groupBox5);
            this.Controls.Add(this.groupBox4);
            this.Controls.Add(this.VerShiftCheckBox);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.HorShiftCheckBox);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.MyCancelButton);
            this.Controls.Add(this.OkButton);
            this.Controls.Add(this.OffsetGainCheckBox);
            this.Controls.Add(this.groupBox1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "CaptureSettings";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "VGA Capture Settings";
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.GainSlider)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.OffsetSlider)).EndInit();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.HorShiftSlider)).EndInit();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.VerShiftSlider)).EndInit();
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.PhaseSlider)).EndInit();
            this.groupBox5.ResumeLayout(false);
            this.groupBox5.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.PLLSlider)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.CheckBox OffsetGainCheckBox;
        private System.Windows.Forms.Label OffsetLabel;
        private System.Windows.Forms.TrackBar OffsetSlider;
        private System.Windows.Forms.Label MinOffsetLabel;
        private System.Windows.Forms.TrackBar GainSlider;
        private System.Windows.Forms.Label GainLabel;
        private System.Windows.Forms.Label MaxGainLabel;
        private System.Windows.Forms.Label MinGainLabel;
        private System.Windows.Forms.Label MaxOffsetLabel;
        private System.Windows.Forms.Label OffsetGainValueLabel;
        private System.Windows.Forms.Button OkButton;
        private System.Windows.Forms.Button MyCancelButton;
        private System.Windows.Forms.CheckBox HorShiftCheckBox;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Label HorShiftValueLabel;
        private System.Windows.Forms.Label MaxHorShiftLabel;
        private System.Windows.Forms.Label MinHorShiftLabel;
        private System.Windows.Forms.TrackBar HorShiftSlider;
        private System.Windows.Forms.CheckBox VerShiftCheckBox;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Label VerShiftValueLabel;
        private System.Windows.Forms.Label MaxVerShiftLabel;
        private System.Windows.Forms.Label MinVerShiftLabel;
        private System.Windows.Forms.TrackBar VerShiftSlider;
        private System.Windows.Forms.CheckBox PhaseCheckBox;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.Label PhaseValueLabel;
        private System.Windows.Forms.Label MaxPhaseLabel;
        private System.Windows.Forms.Label MinPhaseLabel;
        private System.Windows.Forms.TrackBar PhaseSlider;
        private System.Windows.Forms.CheckBox PLLCheckBox;
        private System.Windows.Forms.GroupBox groupBox5;
        private System.Windows.Forms.Label PLLValueLabel;
        private System.Windows.Forms.Label MaxPLLLabel;
        private System.Windows.Forms.Label MinPLLLabel;
        private System.Windows.Forms.TrackBar PLLSlider;
    }
}