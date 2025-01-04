using System;
using System.IO;
using System.Collections.Generic;
using MiniCompiler;

class Program
{
    static void Main(string[] args)
    {
        Console.WriteLine("Introduceți calea către fișierul sursă:");
        string filePath = Console.ReadLine();

        try
        {
            // Citirea conținutului fișierului
            string sourceCode = File.ReadAllText(filePath);
            Console.WriteLine("Codul sursă a fost încărcat cu succes!");

            // Inițializarea lexerului și tokenizarea
            Lexer lexer = new Lexer(sourceCode);
            List<Token> tokens = lexer.Tokenize();

            // Afișarea tokenilor
            Console.WriteLine("Tokenii identificați:");
            foreach (var token in tokens)
            {
                Console.WriteLine(token);
            }

            // Inițializarea parserului și analiza sintactică
            Parser parser = new Parser(tokens);
            parser.Parse();
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Eroare la citirea fișierului: {ex.Message}");
        }
    }
}
