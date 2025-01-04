using MiniCompiler;
using System;
using System.Collections.Generic;
using System.IO;

public class Parser
{
    private readonly List<Token> _tokens;
    private int _current = 0;

    public Parser(List<Token> tokens)
    {
        _tokens = tokens;
    }

    public void Parse(StreamWriter debugWriter)
    {
        while (!IsAtEnd())
        {
            ParseStatement(debugWriter);
        }
    }

    private void ParseStatement(StreamWriter debugWriter)
    {
        Token currentToken = _tokens[_current];

        // ✅ Tratare declarație variabilă: int x = 5;
        if (Match(TokenType.Keyword) && Previous().Lexeme == "int")
        {
            if (Match(TokenType.Identifier))
            {
                Token identifier = Previous();
                if (Match(TokenType.Operator) && Previous().Lexeme == "=")
                {
                    if (Match(TokenType.Number))
                    {
                        debugWriter.WriteLine($"Parsed variable declaration: {identifier.Lexeme} with value {Previous().Lexeme}");
                        if (!Match(TokenType.Delimiter) || Previous().Lexeme != ";")
                        {
                            debugWriter.WriteLine($"Syntax error: Missing ';' after variable declaration at line {identifier.LineNumber}");
                        }
                        return; // ✅ Se iese din metodă după o declarație validă
                    }
                    else
                    {
                        debugWriter.WriteLine($"Syntax error: Expected a number after '='.");
                        return;
                    }
                }
                else
                {
                    debugWriter.WriteLine($"Syntax error: Missing '=' after identifier '{identifier.Lexeme}'.");
                    return;
                }
            }
        }

        // ✅ Tratare Return Statement: return x;
        else if (Match(TokenType.Keyword) && Previous().Lexeme == "return")
        {
            if (Match(TokenType.Identifier) || Match(TokenType.Number))
            {
                Token returnToken = Previous();
                debugWriter.WriteLine($"Parsed return statement: Returning value {returnToken.Lexeme}");

                if (!Match(TokenType.Delimiter) || Previous().Lexeme != ";")
                {
                    debugWriter.WriteLine($"Syntax error: Missing ';' after return statement.");
                }
                return; // ✅ Ieșire după un return valid
            }
            else
            {
                debugWriter.WriteLine($"Syntax error: Expected a valid identifier or number after 'return'.");
                return;
            }
        }

        else
        {
            debugWriter.WriteLine($"Syntax error at line {currentToken.LineNumber}: Unexpected token '{currentToken.Lexeme}'.");
            _current++; // ✅ Avansarea la următorul token pentru prevenirea buclelor infinite
        }
    }

    private bool Match(TokenType type)
    {
        if (_current < _tokens.Count && _tokens[_current].Type == type)
        {
            _current++;
            return true;
        }
        return false;
    }

    private Token Previous()
    {
        return _tokens[_current - 1];
    }

    private bool IsAtEnd()
    {
        return _current >= _tokens.Count || _tokens[_current].Type == TokenType.EndOfFile;
    }
}
