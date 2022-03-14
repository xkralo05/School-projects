using System;

namespace UI_of_app.Messages
{
    public class TimeSlotUpdatedMessage : IMessage
    {
        public Guid Id { get; set; }
    }
}
