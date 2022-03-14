using System;

namespace UI_of_app.Messages
{
    public class TimeSlotAddedMessage : IMessage
    {
        public Guid Id { get; set; }
    }
}