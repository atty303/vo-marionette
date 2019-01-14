using System;
using System.IO;
using System.Reflection;
using System.Windows.Forms;

namespace VoMarionette {
  public class Hook
  {
    private static readonly string DefaultOutputFilename = Path.GetTempFileName();
    
    public static DialogResult ShowDialog(CommonDialog dialog, IWin32Window owner)
    {
      var outputFilename = Environment.GetEnvironmentVariable("VOMARIONETTE_OUTPUT_FILENAME") ?? DefaultOutputFilename;

      var saveFileDialog = dialog as SaveFileDialog;
			if (saveFileDialog != null)
			{
				Type t = typeof(SaveFileDialog);
				t.InvokeMember("FileName", BindingFlags.SetProperty, null, dialog,
					new object[] {outputFilename});
			}
			return DialogResult.OK;
		}
  }
}
