using System;

namespace test {

    public class test1 {

        public static Int32 ReturnInt32WithInt32(Int32 value) {
            Console.WriteLine("c# ReturnInt32WithInt32(Value {0})", value);
            return 32;
        }

        public static int Main(string[] args) {
            Console.WriteLine("test1");
            return 0;
        }
    }
}