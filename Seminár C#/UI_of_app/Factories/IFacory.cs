namespace UI_of_app.Factories
{
    public interface IFactory<out T>
    {
        T Create();
    }
}