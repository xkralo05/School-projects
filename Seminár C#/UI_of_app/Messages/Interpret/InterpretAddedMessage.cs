using System;

namespace UI_of_app.Messages
{
    public class InterpretAddedMessage : IMessage
    {
        public Guid Id { get; set; }
    }
}