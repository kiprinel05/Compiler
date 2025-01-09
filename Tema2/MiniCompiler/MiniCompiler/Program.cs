using System;
using System.IO;
using Antlr4.Runtime;

class Program
{
    static void Main(string[] args)
    {
        string inputFile = "code.txt";
        string input = File.ReadAllText(inputFile);

        var inputStream = new AntlrInputStream(input);
        var lexer = new MiniLangLexer(inputStream);
        var tokens = new CommonTokenStream(lexer);
        var parser = new MiniLangParser(tokens);

        var errorListener = new ErrorListener();
        parser.RemoveErrorListeners();
        parser.AddErrorListener(errorListener);

        try
        {
            var tree = parser.program();

            if (!errorListener.HasErrors)
            {
                Console.WriteLine("Analiza lexicala si sintactica reusita");

                var semanticAnalyzer = new SemanticAnalyzer();
                semanticAnalyzer.Visit(tree);

                if (semanticAnalyzer.Errors.Count > 0)
                {
                    Console.WriteLine("Erori semantice detectate:");
                    foreach (var error in semanticAnalyzer.Errors)
                    {
                        Console.WriteLine(error);
                    }
                    File.WriteAllLines("semantic_errors.txt", semanticAnalyzer.Errors);
                }
                else
                {
                    Console.WriteLine("Analiza semantica reusita");
                }
            }
            else
            {
                Console.WriteLine("Erori lexicale/sintactice detectate");
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine($"[Critical Error]: {ex.Message}");
        }
    }
}
