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

        private static readonly HashSet<string> Keywords = new HashSet<string>
    {
        "int", "float", "double", "string", "void", "if", "else", "for", "while", "return"
    };

        private static readonly HashSet<char> Delimiters = new HashSet<char> { '(', ')', '{', '}', ',', ';' };
        private static readonly HashSet<string> Operators = new HashSet<string>
    {
        "+", "-", "*", "/", "%", "=", "==", "!=", "<", "<=", ">", ">=", "&&", "||", "++", "--"
    };

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
                else if (Delimiters.Contains(current))
                {
                    tokens.Add(new Token(TokenType.Delimiter, current.ToString(), _lineNumber));
                    _position++;
                }
                else if (IsOperatorStart(current))
                {
                    tokens.Add(ReadOperator());
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
            TokenType type = Keywords.Contains(lexeme) ? TokenType.Keyword : TokenType.Identifier;
            return new Token(type, lexeme, _lineNumber);
        }

        private Token ReadNumber()
        {
            int start = _position;
            while (_position < _source.Length && char.IsDigit(_source[_position]))
                _position++;
            string lexeme = _source.Substring(start, _position - start);
            return new Token(TokenType.Number, lexeme, _lineNumber);
        }

        private Token ReadOperator()
        {
            // Tratarea operatorilor care pot avea lungime de 2 caractere (ex: ==, !=, >=, ++, etc.)
            int start = _position;
            _position++;
            if (_position < _source.Length)
            {
                string twoCharOperator = _source.Substring(start, 2);
                if (Operators.Contains(twoCharOperator))
                {
                    _position++;
                    return new Token(TokenType.Operator, twoCharOperator, _lineNumber);
                }
            }
            string oneCharOperator = _source[start].ToString();
            return new Token(TokenType.Operator, oneCharOperator, _lineNumber);
        }

        private bool IsOperatorStart(char c)
        {
            return "+-*/%=!<>&|".Contains(c);
        }
    }


}
