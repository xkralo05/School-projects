using System;

namespace UI_of_app.Messages
{
    public class StageUpdatedMessage : IMessage
    {
        public Guid Id { get; set; }
    }
}
