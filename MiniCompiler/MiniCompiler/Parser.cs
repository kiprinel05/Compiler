using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MiniCompiler
{
    public class Parser
    {
        private readonly List<Token> _tokens;
        private int _current = 0;

        public Parser(List<Token> tokens)
        {
            _tokens = tokens;
        }

        public void Parse()
        {
            while (!Match(TokenType.EndOfFile))
            {
                ParseStatement();
            }
        }

        private void ParseStatement()
        {
            if (Match(TokenType.Identifier))
            {
                Console.WriteLine($"Parsed identifier: {_tokens[_current - 1].Lexeme}");
            }
            else
            {
                Console.WriteLine("Syntax error");
            }
        }

        private bool Match(TokenType type)
        {
            if (_tokens[_current].Type == type)
            {
                _current++;
                return true;
            }
            return false;
        }
    }

}
