using System;

class HelloWorld
{
    static void Main()  // 添加了 Main 方法作为程序的入口点
    {
        onUpdate();  // 调用 onUpdate 方法来显示信息
    }

    static void onUpdate()
    {
        Console.WriteLine("Hello, World!");
        Console.WriteLine("Current date and time is: " + DateTime.Now);
    }
}