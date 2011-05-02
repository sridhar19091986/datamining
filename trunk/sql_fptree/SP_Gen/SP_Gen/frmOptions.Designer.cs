namespace SP_Gen
{
    partial class frmOptions
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
            this.btnOK = new System.Windows.Forms.Button();
            this.btnCancel = new System.Windows.Forms.Button();
            this.tcOptions = new System.Windows.Forms.TabControl();
            this.tpGeneral = new System.Windows.Forms.TabPage();
            this.chkGenerateWrapperClass = new System.Windows.Forms.CheckBox();
            this.tpSQL = new System.Windows.Forms.TabPage();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.cbDelete = new System.Windows.Forms.CheckBox();
            this.cbSelectAll = new System.Windows.Forms.CheckBox();
            this.cbUpdate = new System.Windows.Forms.CheckBox();
            this.cbInsert = new System.Windows.Forms.CheckBox();
            this.cbSelectRow = new System.Windows.Forms.CheckBox();
            this.chkAutosaveSqlScript = new System.Windows.Forms.CheckBox();
            this.chkNullParamDefaultValues = new System.Windows.Forms.CheckBox();
            this.txtPrefix = new System.Windows.Forms.TextBox();
            this.lblPrefix = new System.Windows.Forms.Label();
            this.txtAuthorName = new System.Windows.Forms.TextBox();
            this.lblAuthorName = new System.Windows.Forms.Label();
            this.tpWrapperClass = new System.Windows.Forms.TabPage();
            this.chkAutosaveWrapperClass = new System.Windows.Forms.CheckBox();
            this.chkStaticMethods = new System.Windows.Forms.CheckBox();
            this.txtNamespace = new System.Windows.Forms.TextBox();
            this.lblNamespace = new System.Windows.Forms.Label();
            this.tcOptions.SuspendLayout();
            this.tpGeneral.SuspendLayout();
            this.tpSQL.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.tpWrapperClass.SuspendLayout();
            this.SuspendLayout();
            // 
            // btnOK
            // 
            this.btnOK.Location = new System.Drawing.Point(367, 228);
            this.btnOK.Name = "btnOK";
            this.btnOK.Size = new System.Drawing.Size(75, 21);
            this.btnOK.TabIndex = 4;
            this.btnOK.Text = "OK";
            this.btnOK.UseVisualStyleBackColor = true;
            this.btnOK.Click += new System.EventHandler(this.btnOK_Click);
            // 
            // btnCancel
            // 
            this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnCancel.Location = new System.Drawing.Point(286, 228);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(75, 21);
            this.btnCancel.TabIndex = 5;
            this.btnCancel.Text = "Cancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // tcOptions
            // 
            this.tcOptions.Controls.Add(this.tpGeneral);
            this.tcOptions.Controls.Add(this.tpSQL);
            this.tcOptions.Controls.Add(this.tpWrapperClass);
            this.tcOptions.Dock = System.Windows.Forms.DockStyle.Top;
            this.tcOptions.Location = new System.Drawing.Point(0, 0);
            this.tcOptions.Name = "tcOptions";
            this.tcOptions.SelectedIndex = 0;
            this.tcOptions.Size = new System.Drawing.Size(447, 226);
            this.tcOptions.TabIndex = 6;
            // 
            // tpGeneral
            // 
            this.tpGeneral.Controls.Add(this.chkGenerateWrapperClass);
            this.tpGeneral.Location = new System.Drawing.Point(4, 21);
            this.tpGeneral.Name = "tpGeneral";
            this.tpGeneral.Padding = new System.Windows.Forms.Padding(3);
            this.tpGeneral.Size = new System.Drawing.Size(439, 201);
            this.tpGeneral.TabIndex = 0;
            this.tpGeneral.Text = "General";
            this.tpGeneral.UseVisualStyleBackColor = true;
            // 
            // chkGenerateWrapperClass
            // 
            this.chkGenerateWrapperClass.AutoSize = true;
            this.chkGenerateWrapperClass.Location = new System.Drawing.Point(18, 15);
            this.chkGenerateWrapperClass.Name = "chkGenerateWrapperClass";
            this.chkGenerateWrapperClass.Size = new System.Drawing.Size(156, 16);
            this.chkGenerateWrapperClass.TabIndex = 7;
            this.chkGenerateWrapperClass.Text = "Generate Wrapper Class";
            this.chkGenerateWrapperClass.UseVisualStyleBackColor = true;
            // 
            // tpSQL
            // 
            this.tpSQL.Controls.Add(this.groupBox1);
            this.tpSQL.Controls.Add(this.chkAutosaveSqlScript);
            this.tpSQL.Controls.Add(this.chkNullParamDefaultValues);
            this.tpSQL.Controls.Add(this.txtPrefix);
            this.tpSQL.Controls.Add(this.lblPrefix);
            this.tpSQL.Controls.Add(this.txtAuthorName);
            this.tpSQL.Controls.Add(this.lblAuthorName);
            this.tpSQL.Location = new System.Drawing.Point(4, 21);
            this.tpSQL.Name = "tpSQL";
            this.tpSQL.Padding = new System.Windows.Forms.Padding(3);
            this.tpSQL.Size = new System.Drawing.Size(439, 201);
            this.tpSQL.TabIndex = 1;
            this.tpSQL.Text = "SQL Script Generation";
            this.tpSQL.UseVisualStyleBackColor = true;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.cbDelete);
            this.groupBox1.Controls.Add(this.cbSelectAll);
            this.groupBox1.Controls.Add(this.cbUpdate);
            this.groupBox1.Controls.Add(this.cbInsert);
            this.groupBox1.Controls.Add(this.cbSelectRow);
            this.groupBox1.Location = new System.Drawing.Point(123, 62);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(185, 81);
            this.groupBox1.TabIndex = 23;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Generate Procedures:";
            // 
            // cbDelete
            // 
            this.cbDelete.AutoSize = true;
            this.cbDelete.Location = new System.Drawing.Point(117, 40);
            this.cbDelete.Name = "cbDelete";
            this.cbDelete.Size = new System.Drawing.Size(60, 15);
            this.cbDelete.TabIndex = 27;
            this.cbDelete.Text = "Delete";
            this.cbDelete.UseVisualStyleBackColor = true;
            // 
            // cbSelectAll
            // 
            this.cbSelectAll.AutoSize = true;
            this.cbSelectAll.Location = new System.Drawing.Point(11, 17);
            this.cbSelectAll.Name = "cbSelectAll";
            this.cbSelectAll.Size = new System.Drawing.Size(84, 15);
            this.cbSelectAll.TabIndex = 23;
            this.cbSelectAll.Text = "Select All";
            this.cbSelectAll.UseVisualStyleBackColor = true;
            // 
            // cbUpdate
            // 
            this.cbUpdate.AutoSize = true;
            this.cbUpdate.Location = new System.Drawing.Point(117, 17);
            this.cbUpdate.Name = "cbUpdate";
            this.cbUpdate.Size = new System.Drawing.Size(60, 15);
            this.cbUpdate.TabIndex = 26;
            this.cbUpdate.Text = "Update";
            this.cbUpdate.UseVisualStyleBackColor = true;
            // 
            // cbInsert
            // 
            this.cbInsert.AutoSize = true;
            this.cbInsert.Location = new System.Drawing.Point(11, 61);
            this.cbInsert.Name = "cbInsert";
            this.cbInsert.Size = new System.Drawing.Size(60, 15);
            this.cbInsert.TabIndex = 25;
            this.cbInsert.Text = "Insert";
            this.cbInsert.UseVisualStyleBackColor = true;
            // 
            // cbSelectRow
            // 
            this.cbSelectRow.AutoSize = true;
            this.cbSelectRow.Location = new System.Drawing.Point(11, 40);
            this.cbSelectRow.Name = "cbSelectRow";
            this.cbSelectRow.Size = new System.Drawing.Size(84, 15);
            this.cbSelectRow.TabIndex = 24;
            this.cbSelectRow.Text = "Select Row";
            this.cbSelectRow.UseVisualStyleBackColor = true;
            // 
            // chkAutosaveSqlScript
            // 
            this.chkAutosaveSqlScript.AutoSize = true;
            this.chkAutosaveSqlScript.Location = new System.Drawing.Point(123, 149);
            this.chkAutosaveSqlScript.Name = "chkAutosaveSqlScript";
            this.chkAutosaveSqlScript.Size = new System.Drawing.Size(138, 15);
            this.chkAutosaveSqlScript.TabIndex = 17;
            this.chkAutosaveSqlScript.Text = "Autosave SQL Script";
            this.chkAutosaveSqlScript.UseVisualStyleBackColor = true;
            // 
            // chkNullParamDefaultValues
            // 
            this.chkNullParamDefaultValues.AutoSize = true;
            this.chkNullParamDefaultValues.CheckAlign = System.Drawing.ContentAlignment.TopLeft;
            this.chkNullParamDefaultValues.Location = new System.Drawing.Point(123, 170);
            this.chkNullParamDefaultValues.Name = "chkNullParamDefaultValues";
            this.chkNullParamDefaultValues.Size = new System.Drawing.Size(360, 26);
            this.chkNullParamDefaultValues.TabIndex = 16;
            this.chkNullParamDefaultValues.Text = "Pass \"NULL\" as default parameter values for fields which\r\nallow nulls";
            this.chkNullParamDefaultValues.UseVisualStyleBackColor = true;
            // 
            // txtPrefix
            // 
            this.txtPrefix.Location = new System.Drawing.Point(123, 36);
            this.txtPrefix.Name = "txtPrefix";
            this.txtPrefix.Size = new System.Drawing.Size(185, 21);
            this.txtPrefix.TabIndex = 15;
            this.txtPrefix.Text = "sp_";
            // 
            // lblPrefix
            // 
            this.lblPrefix.AutoSize = true;
            this.lblPrefix.Location = new System.Drawing.Point(81, 39);
            this.lblPrefix.Name = "lblPrefix";
            this.lblPrefix.Size = new System.Drawing.Size(47, 12);
            this.lblPrefix.TabIndex = 14;
            this.lblPrefix.Text = "Perfix:";
            // 
            // txtAuthorName
            // 
            this.txtAuthorName.Location = new System.Drawing.Point(123, 9);
            this.txtAuthorName.Name = "txtAuthorName";
            this.txtAuthorName.Size = new System.Drawing.Size(185, 21);
            this.txtAuthorName.TabIndex = 13;
            // 
            // lblAuthorName
            // 
            this.lblAuthorName.AutoSize = true;
            this.lblAuthorName.Location = new System.Drawing.Point(45, 12);
            this.lblAuthorName.Name = "lblAuthorName";
            this.lblAuthorName.Size = new System.Drawing.Size(77, 12);
            this.lblAuthorName.TabIndex = 12;
            this.lblAuthorName.Text = "Author Name:";
            // 
            // tpWrapperClass
            // 
            this.tpWrapperClass.Controls.Add(this.chkAutosaveWrapperClass);
            this.tpWrapperClass.Controls.Add(this.chkStaticMethods);
            this.tpWrapperClass.Controls.Add(this.txtNamespace);
            this.tpWrapperClass.Controls.Add(this.lblNamespace);
            this.tpWrapperClass.Location = new System.Drawing.Point(4, 21);
            this.tpWrapperClass.Name = "tpWrapperClass";
            this.tpWrapperClass.Padding = new System.Windows.Forms.Padding(3);
            this.tpWrapperClass.Size = new System.Drawing.Size(439, 201);
            this.tpWrapperClass.TabIndex = 2;
            this.tpWrapperClass.Text = "Wrapper Class";
            this.tpWrapperClass.UseVisualStyleBackColor = true;
            // 
            // chkAutosaveWrapperClass
            // 
            this.chkAutosaveWrapperClass.AutoSize = true;
            this.chkAutosaveWrapperClass.Location = new System.Drawing.Point(95, 64);
            this.chkAutosaveWrapperClass.Name = "chkAutosaveWrapperClass";
            this.chkAutosaveWrapperClass.Size = new System.Drawing.Size(156, 15);
            this.chkAutosaveWrapperClass.TabIndex = 3;
            this.chkAutosaveWrapperClass.Text = "Autosave Wrapper Class";
            this.chkAutosaveWrapperClass.UseVisualStyleBackColor = true;
            // 
            // chkStaticMethods
            // 
            this.chkStaticMethods.AutoSize = true;
            this.chkStaticMethods.Location = new System.Drawing.Point(95, 42);
            this.chkStaticMethods.Name = "chkStaticMethods";
            this.chkStaticMethods.Size = new System.Drawing.Size(162, 15);
            this.chkStaticMethods.TabIndex = 2;
            this.chkStaticMethods.Text = "Generate Static Methods";
            this.chkStaticMethods.UseVisualStyleBackColor = true;
            // 
            // txtNamespace
            // 
            this.txtNamespace.Location = new System.Drawing.Point(95, 17);
            this.txtNamespace.Name = "txtNamespace";
            this.txtNamespace.Size = new System.Drawing.Size(149, 21);
            this.txtNamespace.TabIndex = 1;
            // 
            // lblNamespace
            // 
            this.lblNamespace.AutoSize = true;
            this.lblNamespace.Location = new System.Drawing.Point(21, 17);
            this.lblNamespace.Name = "lblNamespace";
            this.lblNamespace.Size = new System.Drawing.Size(65, 12);
            this.lblNamespace.TabIndex = 0;
            this.lblNamespace.Text = "Namespace:";
            // 
            // frmOptions
            // 
            this.AcceptButton = this.btnOK;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.btnCancel;
            this.ClientSize = new System.Drawing.Size(447, 254);
            this.ControlBox = false;
            this.Controls.Add(this.tcOptions);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.btnOK);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "frmOptions";
            this.ShowInTaskbar = false;
            this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Options";
            this.tcOptions.ResumeLayout(false);
            this.tpGeneral.ResumeLayout(false);
            this.tpGeneral.PerformLayout();
            this.tpSQL.ResumeLayout(false);
            this.tpSQL.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.tpWrapperClass.ResumeLayout(false);
            this.tpWrapperClass.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button btnOK;
        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.TabControl tcOptions;
        private System.Windows.Forms.TabPage tpGeneral;
        private System.Windows.Forms.CheckBox chkGenerateWrapperClass;
        private System.Windows.Forms.TabPage tpSQL;
        private System.Windows.Forms.CheckBox chkNullParamDefaultValues;
        private System.Windows.Forms.TextBox txtPrefix;
        private System.Windows.Forms.Label lblPrefix;
        private System.Windows.Forms.TextBox txtAuthorName;
        private System.Windows.Forms.Label lblAuthorName;
        private System.Windows.Forms.TabPage tpWrapperClass;
        private System.Windows.Forms.TextBox txtNamespace;
        private System.Windows.Forms.Label lblNamespace;
        private System.Windows.Forms.CheckBox chkStaticMethods;
        private System.Windows.Forms.CheckBox chkAutosaveSqlScript;
        private System.Windows.Forms.CheckBox chkAutosaveWrapperClass;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.CheckBox cbDelete;
        private System.Windows.Forms.CheckBox cbSelectAll;
        private System.Windows.Forms.CheckBox cbUpdate;
        private System.Windows.Forms.CheckBox cbInsert;
        private System.Windows.Forms.CheckBox cbSelectRow;
    }
}