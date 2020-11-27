using Google.Apis.Auth.OAuth2;
using Google.Apis.Calendar.v3;
using Google.Apis.Calendar.v3.Data;
using Google.Apis.Services;
using Google.Apis.Util.Store;
using System;
using System.IO;
using System.Linq;
// using System.Threading;
using Microsoft.Extensions.Logging;
using Microsoft.Azure.WebJobs;

namespace OnAir.Calendar.Google
{
    public class CalendarConnection
    {
        static string[] Scopes = { CalendarService.Scope.CalendarReadonly };
        static string ApplicationName = "Google Calendar API .NET Quickstart";

        public static Status GetStatus(ILogger log, ExecutionContext context)
        {
            UserCredential credential;

            string inputPath = Path.Combine(context.FunctionAppDirectory, "credentials.json");
            string outputPath = Path.Combine(context.FunctionAppDirectory, "token");
            // string outputPath = Path.Combine(Path.GetTempPath(), "token");

            outputPath = outputPath.TrimEnd('\\');
            if (!Directory.Exists(outputPath))
            {
                log.LogInformation("Directory doesn't exist");
                Directory.CreateDirectory(outputPath);
            }

            log.LogInformation($"Input {inputPath}");
            log.LogInformation($"Output {outputPath}");

            using (var stream =
                new FileStream(inputPath, FileMode.Open, FileAccess.Read))
            {
                // The file token.json stores the user's access and refresh tokens, and is created
                // automatically when the authorization flow completes for the first time.
                credential = GoogleWebAuthorizationBroker.AuthorizeAsync(
                    GoogleClientSecrets.Load(stream).Secrets,
                    Scopes,
                    "user",
                    System.Threading.CancellationToken.None,
                    new FileDataStore(outputPath, true)).Result;
                log.LogInformation("Credential file saved to: " + outputPath);
            }


            // Create Google Calendar API service.
            CalendarService service = new CalendarService(new BaseClientService.Initializer()
            {
                HttpClientInitializer = credential,
                ApplicationName = ApplicationName,
            });

            // Define parameters of request.
            EventsResource.ListRequest request = service.Events.List("primary");
            request.TimeMin = DateTime.Now;
            request.ShowDeleted = false;
            request.SingleEvents = true;
            request.MaxResults = 10;
            request.OrderBy = EventsResource.ListRequest.OrderByEnum.StartTime;

            // Get the current event
            Events events = request.Execute();
            var currentMeeting = events.Items.FirstOrDefault(predicate);
            return currentMeeting == null ? Status.Available : string.IsNullOrEmpty(currentMeeting.Transparency) ? Status.Busy : Status.Focus;
        }

        public static bool predicate(Event e)
        {
            if (!(e.Start.DateTime.HasValue && e.End.DateTime.HasValue))
                return false;

            return e.Start.DateTime.Value.ToUniversalTime() < DateTime.Now.ToUniversalTime() && e.End.DateTime.Value.ToUniversalTime() > DateTime.Now.ToUniversalTime();
        }
    }
}