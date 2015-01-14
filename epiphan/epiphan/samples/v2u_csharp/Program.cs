//
// $Id: Program.cs 7963 2009-11-09 14:49:49Z monich $
//
// Copyright (C) 2008-2009 Epiphan Systems Inc. All rights reserved.
//

using System.Windows.Forms;

namespace v2u_csharp
{
    static class Program
    {

        //--------------------------------------------------------
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new MainForm());
        }
    }
}