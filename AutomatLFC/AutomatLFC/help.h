#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>
bool isOperator(char c)
{
    std::string operators = "|.*()";
    return operators.find(c) != std::string::npos;
}

int operatorPriority(char c)
{
    switch (c)
    {
    case '*':
        return 3;
    case '.':
        return 2;
    case '|':
        return 1;
    case '(':
    case ')':
        return 0;
    }

    throw std::runtime_error("Not an operator");
}

std::string polishPostfixForm(std::string_view regex)
{
    std::string result{};
    std::stack<char> opStack;

    for (char c : regex)
    {
        if (!isOperator(c))
        {
            result.push_back(c);
        }
        else
        {
            if (c == '(')
            {
                opStack.push(c);
            }
            else if (c == ')')
            {
                while (!opStack.empty() && opStack.top() != '(')
                {
                    result.push_back(opStack.top());
                    opStack.pop();
                }

                if (opStack.empty())
                {
                    throw std::runtime_error("Invalid regex: unmatched ')'");
                }

                opStack.pop();
            }
            else
            {
                while (!opStack.empty() && operatorPriority(opStack.top()) >= operatorPriority(c))
                {
                    result.push_back(opStack.top());
                    opStack.pop();
                }

                opStack.push(c);
            }
        }
    }

    while (!opStack.empty())
    {
        result.push_back(opStack.top());
        opStack.pop();
    }

    return result;
}