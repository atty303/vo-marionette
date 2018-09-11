using System;
using System.Reflection;
using System.Windows.Forms;

namespace VoMarionette {
  public class Hook {
		static public DialogResult ShowDialog(CommonDialog dialog, IWin32Window owner)
		{
			var saveFileDialog = (dialog as SaveFileDialog);
			if (saveFileDialog != null)
			{
				Type t = typeof(SaveFileDialog);
				t.InvokeMember("FileName", BindingFlags.SetProperty, null, dialog,
					new object[] {"D:\\Videos\\test.wav"});
			}

			return DialogResult.OK;
		}
  }
}
