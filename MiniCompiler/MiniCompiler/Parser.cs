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
            // ✅ Procesarea instrucțiunii și oprirea după ce a fost validată
            if (ParseStatement(debugWriter))
            {
                continue;  // Se trece la următoarea instrucțiune
            }

            // ✅ Dacă instrucțiunea nu este validă, avansăm controlat
            Token current = _tokens[_current];
            debugWriter.WriteLine($"Syntax error: Unrecognized statement at line {current.LineNumber}");
            _current++;
        }
    }

    /// <summary>
    /// ✅ Recunoaște și procesează o instrucțiune completă
    /// </summary>
    private bool ParseStatement(StreamWriter debugWriter)
    {
        Token currentToken = _tokens[_current];

        // ✅ Dacă se detectează o declarație de variabilă
        if (Check(TokenType.Keyword) && currentToken.Lexeme == "int")
        {
            return ParseVariableDeclaration(debugWriter);
        }

        // ✅ Dacă se detectează un return statement
        if (Check(TokenType.Keyword) && currentToken.Lexeme == "return")
        {
            return ParseReturnStatement(debugWriter);
        }

        return false;  // Tokenul nu corespunde niciunei instrucțiuni
    }

    /// <summary>
    /// ✅ Procesarea declarațiilor de variabile (ex: int x = 5;)
    /// </summary>
    private bool ParseVariableDeclaration(StreamWriter debugWriter)
    {
        Match(TokenType.Keyword);  // Consuma 'int'

        if (!Match(TokenType.Identifier))
        {
            debugWriter.WriteLine($"Syntax error: Expected identifier after 'int'.");
            return false;
        }

        Token identifier = Previous();

        if (!Match(TokenType.Operator) || Previous().Lexeme != "=")
        {
            debugWriter.WriteLine($"Syntax error: Missing '=' after '{identifier.Lexeme}'.");
            return false;
        }

        if (!Match(TokenType.Number))
        {
            debugWriter.WriteLine($"Syntax error: Expected a numeric value after '='.");
            return false;
        }

        Token number = Previous();

        if (!Match(TokenType.Delimiter) || Previous().Lexeme != ";")
        {
            debugWriter.WriteLine($"Syntax error: Missing ';' after variable declaration.");
            return false;
        }

        debugWriter.WriteLine($"Parsed variable declaration: {identifier.Lexeme} with value {number.Lexeme}");
        return true;
    }

    /// <summary>
    /// ✅ Procesarea Return Statement (ex: return x;)
    /// </summary>
    private bool ParseReturnStatement(StreamWriter debugWriter)
    {
        Match(TokenType.Keyword);  // Consuma 'return'

        if (!(Match(TokenType.Identifier) || Match(TokenType.Number)))
        {
            debugWriter.WriteLine($"Syntax error: Expected a variable or numeric value after 'return'.");
            return false;
        }

        Token returnValue = Previous();

        if (!Match(TokenType.Delimiter) || Previous().Lexeme != ";")
        {
            debugWriter.WriteLine($"Syntax error: Missing ';' after return statement.");
            return false;
        }

        debugWriter.WriteLine($"Parsed return statement: Returning value {returnValue.Lexeme}");
        return true;
    }

    /// <summary>
    /// ✅ Avansarea în lista de tokeni dacă tipul corespunde
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
    /// ✅ Verifică fără a avansa dacă tokenul curent este de un anumit tip
    /// </summary>
    private bool Check(TokenType type)
    {
        if (_current >= _tokens.Count) return false;
        return _tokens[_current].Type == type;
    }

    /// <summary>
    /// ✅ Returnează tokenul anterior
    /// </summary>
    private Token Previous()
    {
        return _tokens[_current - 1];
    }

    /// <summary>
    /// ✅ Verifică dacă s-a ajuns la sfârșitul fișierului
    /// </summary>
    private bool IsAtEnd()
    {
        return _current >= _tokens.Count || _tokens[_current].Type == TokenType.EndOfFile;
    }
}
