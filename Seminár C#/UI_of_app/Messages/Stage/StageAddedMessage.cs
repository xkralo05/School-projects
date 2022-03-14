using System;

namespace UI_of_app.Messages
{
    public class StageAddedMessage : IMessage
    {
        public Guid Id { get; set; }
    }
}