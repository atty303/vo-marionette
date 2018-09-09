using System;
using System.Diagnostics;
using System.IO;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using CommandLine;

namespace VoMarionette {
  public class Options
  {
    [Option('e', "executable", Default = "C:\\Program Files (x86)\\AHS\\VOICEROID+\\ZunkoEX\\VOICEROID.exe")]
    public string ExecutableFileName { get; set; }

    public string ProfileDllFileName
    {
      get
      {
        var pwd = System.IO.Directory.GetCurrentDirectory();
        return Path.Combine(pwd, "Profiler.dll");
      }
    }
  }

  public class Controller {
    const string PROFILER_UUID = "{9992F2A6-DF35-472B-AD3E-317F85D958D7}";

    public static void Main(string[] args) {
      //System.Diagnostics.Debug.WriteLine("hogehoge");
      CommandLine.Parser.Default.ParseArguments<Options>(args)
        .WithParsed(o => new Controller().Run(o));
    }

    private int Run(Options opts)
    {
      var psi = new ProcessStartInfo
      {
        FileName = opts.ExecutableFileName,
        UseShellExecute = false
      };

      var envs = psi.EnvironmentVariables;
      // Profiler API を有効にする
      envs.Add("COR_ENABLE_PROFILING", "1");
      envs.Add("COR_PROFILER", PROFILER_UUID);
      envs.Add("COR_PROFILER_PATH", opts.ProfileDllFileName);
      // .NET のバージョンを強制的に 4.0 に固定する
      envs.Add("COMPLUS_Version", "v4.0.30319");
      // 
      envs.Add("COMPLUS_ProfAPI_ProfilerCompatibilitySetting", "EnableV2Profiler");

      using (var process = new System.Diagnostics.Process())
      {
        process.StartInfo = psi;
        process.Start();
      }

      return 0;
    }
  }
}
