using System;
using System.Collections;
using System.Collections.Generic;

namespace test {

    public class test1 {
        
        public static Dictionary<string, object> GetDictionary() {
            return new Dictionary<string, object>() {
                { "a", 1 },
                { "b", -2 },
                { "c", -3.0 },
                { "d", 5.5 },
                { "int", (int) 3 },
                { "e", null },
                { "f", "7" },
                { "g", new DateTime() },
                { "h", true },
                { "i", false },
                { "j", new int[] { 1, 2, 3, 4, 5 } },
                { "doubles", new double[] { 0.0, 1.1, 2.2, 3.3, 5.5, 7.7 } },
                { "floats", new float[] { 0.0f, 1.1f, 2.2f, 3.3f, 5.5f, 7.7f } },
                { "booleans", new bool[] { true, false, true, false, true, false } },
                { "strings", new string[] { "one", "two", "three" } },
                { "mix 1", new object[] { 1, "two", 3.3, 4.4f, false, true, null } },
                { "exception", new Exception("foo") },
                { "bytes", new byte[] { 0, 1, 2, 3, 4, 5 } }
            };
        }
        
        public static Boolean GetBoolean() {
            return true;
        }

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