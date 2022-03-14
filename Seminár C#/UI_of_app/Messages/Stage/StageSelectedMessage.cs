using System;

namespace UI_of_app.Messages
{
    public class StageSelectedMessage : IMessage
    {
        public Guid Id { get; set; }
    }
}
