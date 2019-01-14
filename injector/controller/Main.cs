using System;
using System.Collections;
using System.Collections.Specialized;
using System.Diagnostics;
using System.IO;
using System.Net;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;
using McMaster.Extensions.CommandLineUtils;

namespace VoMarionette {
  [Command(Name = "vo:Marionette", Description = "Yet another CLI for Voiceroid")]
  [HelpOption("-h|--help")]
  public class Controller
  {
    public static void Main(string[] args) => CommandLineApplication.Execute<Controller>(args);

    [Argument(0, Description = "Text to speech")]
    private string SpeechText { get; }

    [Option("-e|--executable")]
    private string ExecutableFileName { get; } = "C:\\Program Files (x86)\\AHS\\VOICEROID+\\ZunkoEX\\VOICEROID.exe";

    private string ProfileDllFileName { get; } = Path.Combine(Directory.GetCurrentDirectory(), "HakoniwaProfiler.dll");

    private const string PROFILER_UUID = "{9992F2A6-DF35-472B-AD3E-317F85D958D7}";

    private void OnExecute(CommandLineApplication app)
    {
      var outputFileName = Path.GetTempFileName();

      var psi = new ProcessStartInfo
      {
        FileName = ExecutableFileName,
        UseShellExecute = false
      };

      SetupProfiler(psi.EnvironmentVariables, PROFILER_UUID, ProfileDllFileName, outputFileName);

      using (var process = new System.Diagnostics.Process())
      {
        process.StartInfo = psi;
        process.Start();

        var automator = new Automator(process, outputFileName);
        var server = new HttpServer(automator);
        server.Start();

        process.WaitForExit();
      }
    }

    private void SetupProfiler(StringDictionary envs, string uuid, string path, string tempFileName)
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

      envs.Add("VOMARIONETTE_OUTPUT_FILENAME", tempFileName);
    }
  }

  public class HttpServer
  {
    public HttpServer(Automator automator)
    {
      Automator = automator;
    }

    public void Start()
    {
      var listener = new HttpListener();
      listener.Prefixes.Add("http://localhost:18080/");

      listener.Start();

      while (listener.IsListening)
      {
        var context = listener.GetContext();
        ProcessRequest(context);
              }
      listener.Close();
    }

    private void ProcessRequest(HttpListenerContext context)
    {
      var req = context.Request;

      var reqBody = new StreamReader(req.InputStream, Encoding.Default).ReadToEnd();

      var wav = Automator.TextToSpeech(reqBody);

      context.Response.StatusCode = 200;
      context.Response.OutputStream.Write(wav, 0, wav.Length);
      context.Response.Close();
    }

    private Automator Automator { get; }
  }
}
