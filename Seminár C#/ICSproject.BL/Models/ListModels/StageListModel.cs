using System;
using System.Collections.Generic;

namespace ICSproject.BL.Models.ListModels
{
    public class StageListModel : IModel
    {
        public Guid Id { get; set; }

        public string Name { get; set; }
    }
}