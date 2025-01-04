using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MiniCompiler
{
    public enum TokenType
    {
        Identifier, Number, StringLiteral, Keyword, Operator, Delimiter,
        Comment, Error, EndOfFile
    }

    public class Token
    {
        public TokenType Type { get; }
        public string Lexeme { get; }
        public int LineNumber { get; }

        public Token(TokenType type, string lexeme, int lineNumber)
        {
            Type = type;
            Lexeme = lexeme;
            LineNumber = lineNumber;
        }

        public override string ToString() => $"{Type} {Lexeme} at line {LineNumber}";
    }

}
