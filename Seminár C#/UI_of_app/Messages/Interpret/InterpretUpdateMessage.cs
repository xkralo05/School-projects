using System;

namespace UI_of_app.Messages
{
    public class InterpretUpdatedMessage : IMessage
    {
        public Guid Id { get; set; }
    }
}
