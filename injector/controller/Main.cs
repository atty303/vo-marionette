using System;
using System.Collections;
using System.Collections.Specialized;
using System.Diagnostics;
using System.IO;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using McMaster.Extensions.CommandLineUtils;

namespace VoMarionette {
  [Command(Name = "vo:Marionette", Description = "Yet another CLI for Voiceroid")]
  [HelpOption("-h|--help")]
  public class Controller
  {
    public static void Main(string[] args) => CommandLineApplication.Execute<Controller>(args);
    
    [Option("-e|--executable")]
    private string ExecutableFileName { get; } = "C:\\Program Files (x86)\\AHS\\VOICEROID+\\ZunkoEX\\VOICEROID.exe";

    private string ProfileDllFileName { get; } = Path.Combine(Directory.GetCurrentDirectory(), "HakoniwaProfiler.dll");

    const string PROFILER_UUID = "{9992F2A6-DF35-472B-AD3E-317F85D958D7}";

    private void OnExecute(CommandLineApplication app)
    {
      var psi = new ProcessStartInfo
      {
        FileName = ExecutableFileName,
        UseShellExecute = false
      };

      
      SetupProfiler(psi.EnvironmentVariables, PROFILER_UUID, ProfileDllFileName);

      using (var process = new System.Diagnostics.Process())
      {
        process.StartInfo = psi;
        process.Start();
        process.WaitForExit();
      }
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
