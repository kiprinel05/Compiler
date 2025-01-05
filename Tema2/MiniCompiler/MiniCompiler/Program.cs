using System;
using System.IO;
using Antlr4.Runtime;
using Antlr4.Runtime.Misc;

public class ErrorListener : BaseErrorListener
{
    // Personalizează modul în care sunt afișate erorile de sintaxă
    public override void SyntaxError(TextWriter output, IRecognizer recognizer, IToken offendingSymbol, int line, int charPositionInLine, string msg, RecognitionException e)
    {
        // Afișează eroarea de sintaxă cu linia și poziția
        Console.WriteLine($"[Error] Line {line}, Position {charPositionInLine}: {msg}");

        // Afișează detalii suplimentare, dacă există
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
        // Încarcă fișierul de input (code.txt)
        string inputFile = "code.txt";
        string input = File.ReadAllText(inputFile);

        // Creează un stream de input pentru ANTLR
        var inputStream = new AntlrInputStream(input);
        var lexer = new MiniLangLexer(inputStream); // Folosește lexer-ul generat
        var tokens = new CommonTokenStream(lexer);
        var parser = new MiniLangParser(tokens);

        // Îndepărtează listener-ii de erori implicați și adaugă-l pe cel personalizat
        parser.RemoveErrorListeners();
        parser.AddErrorListener(new ErrorListener());

        try
        {
            // Începe procesul de parsing la regula de început 'program'
            var tree = parser.program();
            Console.WriteLine("Programul a fost analizat cu succes!");

        }
        catch (Exception ex)
        {
            // Capturează orice altă excepție care poate apărea
            Console.WriteLine($"A apărut o eroare: {ex.Message}");
        }
    }
}
