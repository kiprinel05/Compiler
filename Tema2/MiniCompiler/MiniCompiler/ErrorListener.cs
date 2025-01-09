using System;
using System.IO;
using Antlr4.Runtime;

public class ErrorListener : BaseErrorListener
{
    public bool HasErrors { get; private set; } = false;

    public override void SyntaxError(TextWriter output, IRecognizer recognizer, IToken offendingSymbol, int line, int charPositionInLine, string msg, RecognitionException e)
    {
        HasErrors = true;
        Console.WriteLine($"[Lexical Error] Line {line}, Position {charPositionInLine}: {msg}");

        if (e != null)
        {
            Console.WriteLine($"[Details] {e.Message}");
        }
    }
}
