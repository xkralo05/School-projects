using System;

namespace UI_of_app.Messages
{
    public class TimeSlotSelectedMessage : IMessage
    {
        public Guid Id { get; set; }
    }
}
