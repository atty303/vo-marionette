using System;
using System.Diagnostics;
using System.IO;
using System.Linq;
using static PInvoke.User32;
using HWND = System.IntPtr;
using System.Windows.Automation;

namespace VoMarionette
{
  public class Automator
  {
    public Automator(Process process, string outputFileName)
    {
      TargetProcess = process;
      OutputFilename = outputFileName;
    }

    public byte[] TextToSpeech(string text)
    {
      var dialogueWindow = FindDialogueWindow(TargetProcess.MainWindowHandle);
      SetText(dialogueWindow, text);

			var element = AutomationElement.FromHandle(TargetProcess.MainWindowHandle);
			var saveButton = element.FindFirst(TreeScope.Subtree,
				new PropertyCondition(AutomationElement.AutomationIdProperty, "btnSaveWave"));
			var invokeSave = saveButton.GetCurrentPattern(InvokePattern.Pattern) as InvokePattern;
			invokeSave.Invoke();

      return File.ReadAllBytes(OutputFilename);
    }

    private Process TargetProcess { get; }
    private string OutputFilename { get; }

    // 台詞ウィンドウを見つける
    // とりあえずずんこ専用
    private static HWND FindDialogueWindow(HWND mainWindow)
    {
      var current = mainWindow;

      GetWindowCommands[] commands =
      {
        GetWindowCommands.GW_CHILD,
        GetWindowCommands.GW_CHILD,
        GetWindowCommands.GW_HWNDNEXT,
        GetWindowCommands.GW_CHILD,
        GetWindowCommands.GW_CHILD,
        GetWindowCommands.GW_CHILD,
        GetWindowCommands.GW_CHILD,
        GetWindowCommands.GW_CHILD,
        GetWindowCommands.GW_CHILD,
      };

      return commands.Aggregate(current, GetWindow);
    }

    private static unsafe void SetText(HWND dialogueWindow, string text)
    {
      fixed (char* p = "")
      {
        SendMessage(dialogueWindow, WindowMessage.WM_SETTEXT, p, null);
      }

      for (var i = 0; i < text.Length; ++i)
      {
        var code = Char.ConvertToUtf32(text, i);
        SendMessage(dialogueWindow, WindowMessage.WM_CHAR, (void*) new IntPtr(code), null);
      }
    }
  }
}
