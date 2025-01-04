using System;
using System.IO;
using System.Collections.Generic;
using MiniCompiler;

class Program
{
    static void Main(string[] args)
    {
        // Se va crea sau suprascrie fișierul debug.txt în directorul curent
        string debugFilePath = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "debug.txt");

        try
        {
            // Citirea fișierului sursă
            string sourceFilePath = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "code.txt");
            string sourceCode = File.ReadAllText(sourceFilePath);

            using (StreamWriter debugWriter = new StreamWriter(debugFilePath, false))
            {
                debugWriter.WriteLine("Starting lexical analysis...");

                // Inițializarea lexerului și tokenizarea
                Lexer lexer = new Lexer(sourceCode);
                List<Token> tokens = lexer.Tokenize();

                // Scrierea tokenilor în debug.txt
                debugWriter.WriteLine("Tokens Identified:");
                foreach (var token in tokens)
                {
                    debugWriter.WriteLine(token.ToString());
                }

                // Inițializarea parserului și analiza sintactică
                Parser parser = new Parser(tokens);
                debugWriter.WriteLine("Starting syntax analysis...");
                parser.Parse(debugWriter);

                // Inițializarea analizei semantice
                SemanticAnalyzer semanticAnalyzer = new SemanticAnalyzer(tokens);
                int result = semanticAnalyzer.AnalyzeAndExecute(debugWriter);

                // Afișarea rezultatului în consolă
                Console.WriteLine($"Result: {result}");
            }

            // Mesaj de finalizare
            Console.WriteLine("Compilation completed successfully. Check 'debug.txt' for details.");
        }
        catch (Exception ex)
        {
            Console.WriteLine($"An error occurred: {ex.Message}");
        }
    }
}
