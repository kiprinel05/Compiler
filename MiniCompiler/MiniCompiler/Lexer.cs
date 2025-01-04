using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MiniCompiler
{
    public class Lexer
    {
        private readonly string _source;
        private int _position = 0;
        private int _lineNumber = 1;

        public Lexer(string source)
        {
            _source = source;
        }

        public List<Token> Tokenize()
        {
            List<Token> tokens = new List<Token>();

            while (_position < _source.Length)
            {
                char current = _source[_position];

                if (char.IsWhiteSpace(current))
                {
                    if (current == '\n') _lineNumber++;
                    _position++;
                }
                else if (char.IsLetter(current))
                {
                    tokens.Add(ReadIdentifier());
                }
                else if (char.IsDigit(current))
                {
                    tokens.Add(ReadNumber());
                }
                else
                {
                    tokens.Add(new Token(TokenType.Error, current.ToString(), _lineNumber));
                    _position++;
                }
            }

            tokens.Add(new Token(TokenType.EndOfFile, "", _lineNumber));
            return tokens;
        }

        private Token ReadIdentifier()
        {
            int start = _position;
            while (_position < _source.Length && char.IsLetterOrDigit(_source[_position]))
                _position++;
            string lexeme = _source.Substring(start, _position - start);
            return new Token(TokenType.Identifier, lexeme, _lineNumber);
        }

        private Token ReadNumber()
        {
            int start = _position;
            while (_position < _source.Length && char.IsDigit(_source[_position]))
                _position++;
            string lexeme = _source.Substring(start, _position - start);
            return new Token(TokenType.Number, lexeme, _lineNumber);
        }
    }

}
