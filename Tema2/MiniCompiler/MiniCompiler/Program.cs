using System;
using System.IO;
using Antlr4.Runtime;
using Antlr4.Runtime.Misc;

public class ErrorListener : BaseErrorListener
{
    public bool HasErrors { get; private set; } = false;

    public override void SyntaxError(TextWriter output, IRecognizer recognizer, IToken offendingSymbol, int line, int charPositionInLine, string msg, RecognitionException e)
    {
        HasErrors = true; 
        Console.WriteLine($"[Error] Line {line}, Position {charPositionInLine}: {msg}");

        if (e != null)
        {
            Console.WriteLine($"[Details] {e.Message}");
        }
    }
}

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
                Console.WriteLine("Programul a fost analizat cu succes!");
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine($"A apărut o eroare: {ex.Message}");
        }
    }
}
