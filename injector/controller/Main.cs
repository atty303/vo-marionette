using System;
using System.Collections;
using System.Collections.Specialized;
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
        return Path.Combine(pwd, "HakoniwaProfiler.dll");
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

      
      SetupProfiler(psi.EnvironmentVariables, PROFILER_UUID, opts.ProfileDllFileName);

      using (var process = new System.Diagnostics.Process())
      {
        process.StartInfo = psi;
        process.Start();
        process.WaitForExit();
      }

      return 0;
    }

    void SetupProfiler(StringDictionary envs, string uuid, string path)
    {
      // Settings Up a Profiling Environment
      // https://docs.microsoft.com/ja-jp/dotnet/framework/unmanaged-api/profiling/setting-up-a-profiling-environment
      envs.Add("COR_ENABLE_PROFILING", "1");
      envs.Add("COR_PROFILER", uuid);

      // Registry-Free Profiler Startup and Attach
      // https://docs.microsoft.com/en-us/previous-versions/dotnet/netframework-4.0/ee471451(v%3dvs.100)
      envs.Add("COR_PROFILER_PATH", path);

      // Force .NET 4.0
      envs.Add("COMPLUS_Version", "v4.0.30319");

      // Profiler Compatibility Settings
      // https://docs.microsoft.com/en-us/previous-versions/dotnet/netframework-4.0/dd778910(v%3dvs.100)
      envs.Add("COMPLUS_ProfAPI_ProfilerCompatibilitySetting", "EnableV2Profiler");
    }
  }
}
