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

    throw std::runtime_error("[ not an operator ]");
}

std::string polishPostfix(std::string_view regex)
{
    std::string result{};
    std::stack<char> oprtr;

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
                oprtr.push(c);
            }
            else if (c == ')')
            {
                while (!oprtr.empty() && oprtr.top() != '(')
                {
                    result.push_back(oprtr.top());
                    oprtr.pop();
                }

                if (oprtr.empty())
                {
                    throw std::runtime_error("[ invalid regex // unmatched ')' ]");
                }

                oprtr.pop();
            }
            else
            {
                while (!oprtr.empty() && operatorPriority(oprtr.top()) >= operatorPriority(c))
                {
                    result.push_back(oprtr.top());
                    oprtr.pop();
                }

                oprtr.push(c);
            }
        }
    }

    while (!oprtr.empty())
    {
        result.push_back(oprtr.top());
        oprtr.pop();
    }

    return result;
}