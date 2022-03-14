using System;

namespace UI_of_app.Messages
{
    public class InterpretDeletedMessage : IMessage
    {
        public Guid Id { get; set; }
    }
}