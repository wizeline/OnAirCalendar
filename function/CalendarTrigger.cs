using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;
using Microsoft.Azure.Devices;
using Microsoft.Azure.WebJobs;
using Microsoft.Azure.WebJobs.Host;
using Microsoft.Extensions.Logging;
using OnAir.Calendar.Google;
using Microsoft.Azure.WebJobs.Extensions.Files;
using Microsoft.Extensions.DependencyInjection;

namespace OnAir.Calendar
{
    public static class CalendarTrigger
    {
        [FunctionName("CalendarTrigger")]
        public async static Task Run([TimerTrigger("*/15 * * * * *")] TimerInfo myTimer, ILogger log, ExecutionContext context)
        {

            ServiceClient serviceClient;
            string connectionString = "HostName=OnAirCalendar.azure-devices.net;SharedAccessKeyName=iothubowner;SharedAccessKey=2840VypKT55Hbm603+5EEN7BP2FHsoaw2UftJVaJXIg=";
            string targetDevice = "arduino";

            serviceClient = ServiceClient.CreateFromConnectionString(connectionString);
            Status status = CalendarConnection.GetStatus(log, context);
            string color = GetColorString(status);

            log.LogInformation($"Status: {status}");
            var commandMessage = new Message(Encoding.ASCII.GetBytes(color));
            await serviceClient.SendAsync(targetDevice, commandMessage);
        }

        private static string GetColorString(Status status)
        {
            IDictionary<Status, string> colors = new Dictionary<Status, string>() { { Status.Available, "00FF00" }, { Status.Busy, "FF0000" }, { Status.Focus, "FFFF00" }, };
            return colors[status];
        }
    }
}
