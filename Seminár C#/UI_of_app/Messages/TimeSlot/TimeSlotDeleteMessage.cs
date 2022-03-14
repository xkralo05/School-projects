using System;

namespace UI_of_app.Messages
{
    public class TimeSlotDeletedMessage : IMessage
    {
        public Guid Id { get; set; }
    }
}