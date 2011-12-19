using System;
using System.Collections;
using System.Collections.Generic;

namespace test {

    public class test1 {
        
        public static void PrintWhatever(object whatever) {
            if (whatever != null) {
                if (whatever is byte[]) {
                    byte[] array = whatever as byte[];
                    Console.Write("[byte[{0}] ", array.Length);
                    foreach (byte e in array) {
                        Console.Write("{0} ", e);
                    }
                    Console.Write("]");
                } else if (whatever is object[]) {
                    object[] array = whatever as object[];
                    Console.Write("[object[{0}] ", array.Length);
                    foreach (object e in array) {
                        PrintWhatever(e);
                        Console.Write(" ");
                    }
                    Console.Write("]");
                } else if (whatever is Dictionary<string, object>) {
                    Dictionary<string, object> dictionary = whatever as Dictionary<string, object>;
                    Console.WriteLine("Dictionary<string, object>:");
                    foreach (KeyValuePair<string, object> pair in dictionary) {
                        Console.WriteLine(" - {0} : {1}", pair.Key, pair.Value);
                    }
                } else if (whatever is Hashtable) {
                    Hashtable hashtable = whatever as Hashtable;
                    Console.WriteLine("Hashtable:");
                    foreach (DictionaryEntry pair in hashtable) {
                        Console.Write(" - ");
                        PrintWhatever(pair.Key);
                        Console.Write(" : ");
                        PrintWhatever(pair.Value);
                        Console.WriteLine();
                    }
                } else {
                    Console.Write("{0}", whatever);
                }
            } else {
                Console.Write("[null]");
            }
        }
        
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
                { "bytes", new byte[] { 0, 1, 2, 3, 4, 5 } },
                { "ArrayList", new ArrayList() { 1, "2", 3.3, 4.4f, false, null, true } }
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