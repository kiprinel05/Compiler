using System;
using System.IO;
using System.Text.RegularExpressions;
using System.Collections.Generic;

public class MiniCompiler
{
    private List<string> keywords = new List<string>
    {
        "int", "float", "double", "void", "if", "else", "for", "while", "return"
    };

    private Dictionary<string, string> declaredVariables = new Dictionary<string, string>();
    private List<string> globalVariables = new List<string>();
    private List<string> functions = new List<string>();
    private List<string> controlStructures = new List<string>();

    public void Compile(string inputFilePath, string outputFilePath)
    {
        declaredVariables.Clear();
        globalVariables.Clear();
        functions.Clear();
        controlStructures.Clear();

        var sourceCode = File.ReadAllText(inputFilePath);

        // Handle multiline comments correctly
        sourceCode = RemoveMultilineComments(sourceCode);
        var lines = sourceCode.Split('\n');
        var tokens = new List<string>();
        int lineNumber = 0;

        foreach (var line in lines)
        {
            lineNumber++;
            string processedLine = RemoveComments(line);
            tokens.AddRange(Tokenize(processedLine, lineNumber));
        }

        if (SyntaxAnalysis(tokens) && SemanticAnalysis(tokens))
        {
            SaveTokens(outputFilePath, tokens);
            SaveSyntaxElements("syntax.txt");
            Console.WriteLine("Compilation finished. Check debug.txt for details.");
        }
        else
        {
            Console.WriteLine("Errors found. Compilation halted.");
        }
    }

    private bool SyntaxAnalysis(List<string> tokens)
    {
        int braceCount = 0;
        int parenthesisCount = 0;

        foreach (var token in tokens)
        {
            if (token.Contains("Lexeme: {"))
            {
                braceCount++;
            }
            else if (token.Contains("Lexeme: }"))
            {
                braceCount--;
                if (braceCount < 0) return false;
            }
            else if (token.Contains("Lexeme: ("))
            {
                parenthesisCount++;
            }
            else if (token.Contains("Lexeme: )"))
            {
                parenthesisCount--;
                if (parenthesisCount < 0) return false;
            }
        }

        return braceCount == 0 && parenthesisCount == 0;
    }

    private bool SemanticAnalysis(List<string> tokens)
    {
        for (int i = 0; i < tokens.Count; i++)
        {
            var token = tokens[i];
            if (token.Contains("Token: Keyword") && (token.Contains("int") || token.Contains("float") || token.Contains("double") || token.Contains("void")))
            {
                if (i + 1 < tokens.Count && tokens[i + 1].Contains("Token: Identifier"))
                {
                    var variableName = tokens[i + 1].Split(',')[1].Split(':')[1].Trim();
                    var variableType = token.Split(',')[1].Split(':')[1].Trim();

                    if (declaredVariables.ContainsKey(variableName))
                    {
                        Console.WriteLine($"Semantic error: Variable '{variableName}' already declared.");
                        return false;
                    }
                    declaredVariables[variableName] = variableType;
                    globalVariables.Add($"{variableType} {variableName}");
                }
            }
            else if (token.Contains("Token: Keyword") && (token.Contains("if") || token.Contains("else") || token.Contains("for") || token.Contains("while")))
            {
                controlStructures.Add($"{token}, Line: {tokens[i].Split(':')[2].Trim()}");
            }
            else if (token.Contains("Token: Identifier"))
            {
                var variableName = token.Split(',')[1].Split(':')[1].Trim();
                if (!declaredVariables.ContainsKey(variableName))
                {
                    Console.WriteLine($"Semantic error: Variable '{variableName}' used before declaration.");
                    return false;
                }
            }
        }
        return true;
    }

    private string RemoveComments(string line)
    {
        return Regex.Replace(line, @"//.*", "");
    }

    private string RemoveMultilineComments(string code)
    {
        return Regex.Replace(code, @"/\*.*?\*/", "", RegexOptions.Singleline);
    }

    private List<string> Tokenize(string line, int lineNumber)
    {
        var tokens = new List<string>();

        var identifierRegex = new Regex(@"[A-Za-z_]\w*");
        var numericLiteralRegex = new Regex(@"\b\d+\.\d+|\b\d+\.|\.\d+\b|\b\d+\b");
        var delimiterRegex = new Regex(@"[(){};,]");
        var arithmeticOperatorRegex = new Regex(@"[+\-*/%]");
        var relationalOperatorRegex = new Regex(@"==|!=|<=|>=|<|>");
        var logicalOperatorRegex = new Regex(@"&&|\|\||!");
        var assignmentOperatorRegex = new Regex(@"=|\+=|-=|\*=|/=|%=");
        var incrementDecrementOperatorRegex = new Regex(@"\+\+|--");

        foreach (Match match in identifierRegex.Matches(line))
            tokens.Add($"<Token: {(keywords.Contains(match.Value) ? "Keyword" : "Identifier")}, Lexeme: {match.Value}, Line: {lineNumber}>");

        foreach (Match match in numericLiteralRegex.Matches(line))
            tokens.Add($"<Token: Numeric Literal, Lexeme: {match.Value}, Line: {lineNumber}> ");

        foreach (Match match in delimiterRegex.Matches(line))
            tokens.Add($"<Token: Delimiter, Lexeme: {match.Value}, Line: {lineNumber}> ");

        return tokens;
    }

    private void SaveTokens(string outputFilePath, List<string> tokens)
    {
        File.WriteAllLines(outputFilePath, tokens);
    }

    private void SaveSyntaxElements(string outputFilePath)
    {
        using (StreamWriter writer = new StreamWriter(outputFilePath))
        {
            writer.WriteLine("Global Variables:");
            globalVariables.ForEach(v => writer.WriteLine(v));
            writer.WriteLine("\nControl Structures:");
            controlStructures.ForEach(c => writer.WriteLine(c));
        }
    }

    public static void Main()
    {
        var compiler = new MiniCompiler();
        compiler.Compile("code.txt", "debug.txt");
    }
}
