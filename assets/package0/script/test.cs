using System;

public class HelloWorld
{
    public void onUpdate()
    {
        Console.WriteLine("Hello, World0!");
        Console.WriteLine("Current date and time is: " + DateTime.Now);
    }

    public void onPhysicsUpdate()
    {
        Console.WriteLine("Hello, World1!");
        Console.WriteLine("Current date and time is: " + DateTime.Now);
    }
}