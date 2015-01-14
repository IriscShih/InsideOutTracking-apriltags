namespace v2u_csharp
{
    partial class MainForm
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
            if (disposing && (components != null))
            {
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
            this.TextOutput = new System.Windows.Forms.RichTextBox();
            this.CaptureFrame = new System.Windows.Forms.Button();
            this.PictureBox = new System.Windows.Forms.PictureBox();
            this.ConfigureButton = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.PictureBox)).BeginInit();
            this.SuspendLayout();
            // 
            // TextOutput
            // 
            this.TextOutput.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.TextOutput.Location = new System.Drawing.Point(198, 11);
            this.TextOutput.Name = "TextOutput";
            this.TextOutput.ReadOnly = true;
            this.TextOutput.ScrollBars = System.Windows.Forms.RichTextBoxScrollBars.ForcedVertical;
            this.TextOutput.Size = new System.Drawing.Size(529, 87);
            this.TextOutput.TabIndex = 6;
            this.TextOutput.Text = "";
            // 
            // CaptureFrame
            // 
            this.CaptureFrame.Location = new System.Drawing.Point(12, 11);
            this.CaptureFrame.Name = "CaptureFrame";
            this.CaptureFrame.Size = new System.Drawing.Size(180, 23);
            this.CaptureFrame.TabIndex = 7;
            this.CaptureFrame.Text = "Capture Frame";
            this.CaptureFrame.UseVisualStyleBackColor = true;
            this.CaptureFrame.Click += new System.EventHandler(this.CaptureFrame_Click);
            // 
            // PictureBox
            // 
            this.PictureBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.PictureBox.Location = new System.Drawing.Point(12, 104);
            this.PictureBox.Name = "PictureBox";
            this.PictureBox.Size = new System.Drawing.Size(715, 408);
            this.PictureBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.PictureBox.TabIndex = 8;
            this.PictureBox.TabStop = false;
            this.PictureBox.WaitOnLoad = true;
            // 
            // ConfigureButton
            // 
            this.ConfigureButton.Enabled = false;
            this.ConfigureButton.Location = new System.Drawing.Point(12, 41);
            this.ConfigureButton.Name = "ConfigureButton";
            this.ConfigureButton.Size = new System.Drawing.Size(180, 23);
            this.ConfigureButton.TabIndex = 9;
            this.ConfigureButton.Text = "Configure Frame Grabber...";
            this.ConfigureButton.UseVisualStyleBackColor = true;
            this.ConfigureButton.Click += new System.EventHandler(this.ConfigureButton_Click);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(739, 524);
            this.Controls.Add(this.ConfigureButton);
            this.Controls.Add(this.PictureBox);
            this.Controls.Add(this.CaptureFrame);
            this.Controls.Add(this.TextOutput);
            this.Name = "MainForm";
            this.Text = "Epiphan Frame Grabber Test";
            ((System.ComponentModel.ISupportInitialize)(this.PictureBox)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        internal System.Windows.Forms.RichTextBox TextOutput;
        internal System.Windows.Forms.Button CaptureFrame;
        internal System.Windows.Forms.PictureBox PictureBox;
        private System.Windows.Forms.Button ConfigureButton;

    }
}

