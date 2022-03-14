using System;

namespace UI_of_app.Messages
{
    public class StageDeletedMessage : IMessage
    {
        public Guid Id { get; set; }
    }
}