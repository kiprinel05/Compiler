using MiniCompiler;
using System;
using System.Collections.Generic;
using System.IO;

public class SemanticAnalyzer
{
    private readonly List<Token> _tokens;
    private Dictionary<string, int> _symbolTable = new Dictionary<string, int>();
    private int _current = 0;

    public SemanticAnalyzer(List<Token> tokens)
    {
        _tokens = tokens;
    }

    /// <summary>
    /// Analizează semantic și execută codul, returnând valoarea finală pentru instrucțiunea return.
    /// </summary>
    public int AnalyzeAndExecute(StreamWriter debugWriter)
    {
        int returnValue = 0;

        while (!Match(TokenType.EndOfFile))
        {
            Token currentToken = _tokens[_current];

            // ✅ Tratarea declarațiilor de variabile: int a = 5;
            if (Match(TokenType.Keyword) && Previous().Lexeme == "int")
            {
                Token identifier = _tokens[_current];
                if (Match(TokenType.Identifier))
                {
                    if (Match(TokenType.Operator) && Previous().Lexeme == "=")
                    {
                        if (Match(TokenType.Number))
                        {
                            int value = int.Parse(Previous().Lexeme);
                            _symbolTable[identifier.Lexeme] = value;
                            debugWriter.WriteLine($"Variable '{identifier.Lexeme}' initialized with value {value}.");
                            if (!Match(TokenType.Delimiter) || Previous().Lexeme != ";")
                            {
                                debugWriter.WriteLine($"Error: Missing ';' after variable declaration.");
                            }
                        }
                        else
                        {
                            debugWriter.WriteLine($"Error: Invalid value assigned to variable '{identifier.Lexeme}'.");
                        }
                    }
                    else
                    {
                        debugWriter.WriteLine($"Error: Missing '=' in variable declaration.");
                    }
                }
                else
                {
                    debugWriter.WriteLine($"Error: Invalid variable name.");
                }
            }

            // ✅ Tratarea instrucțiunii return: return a;
            else if (Match(TokenType.Keyword) && Previous().Lexeme == "return")
            {
                if (Match(TokenType.Identifier))
                {
                    string identifier = Previous().Lexeme;
                    if (_symbolTable.ContainsKey(identifier))
                    {
                        returnValue = _symbolTable[identifier];
                        debugWriter.WriteLine($"Return statement executed. Returning value: {returnValue}");
                        if (!Match(TokenType.Delimiter) || Previous().Lexeme != ";")
                        {
                            debugWriter.WriteLine($"Error: Missing ';' after return statement.");
                        }
                    }
                    else
                    {
                        debugWriter.WriteLine($"Error: Variable '{identifier}' not declared.");
                    }
                }
                // ✅ Suport pentru return direct: return 5;
                else if (Match(TokenType.Number))
                {
                    returnValue = int.Parse(Previous().Lexeme);
                    debugWriter.WriteLine($"Return statement executed. Returning value: {returnValue}");
                    if (!Match(TokenType.Delimiter) || Previous().Lexeme != ";")
                    {
                        debugWriter.WriteLine($"Error: Missing ';' after return statement.");
                    }
                }
                else
                {
                    debugWriter.WriteLine($"Error: Invalid return statement.");
                }
            }
            else
            {
                debugWriter.WriteLine($"Warning: Unrecognized statement at line {_tokens[_current].LineNumber}");
                _current++;  // Se avansează la următorul token
            }
        }

        return returnValue;
    }

    /// <summary>
    /// Verifică dacă tokenul curent corespunde tipului specificat și avansează dacă este adevărat.
    /// </summary>
    private bool Match(TokenType type)
    {
        if (_current < _tokens.Count && _tokens[_current].Type == type)
        {
            _current++;
            return true;
        }
        return false;
    }

    /// <summary>
    /// Returnează tokenul anterior celui curent.
    /// </summary>
    private Token Previous()
    {
        return _tokens[_current - 1];
    }

    /// <summary>
    /// Verifică dacă tokenul curent corespunde unui anumit tip fără a avansa.
    /// </summary>
    private bool Check(TokenType type)
    {
        if (_current >= _tokens.Count) return false;
        return _tokens[_current].Type == type;
    }
}
