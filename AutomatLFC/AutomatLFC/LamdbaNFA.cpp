#include "LambdaNFA.h"
#include <string>
#include <stack>
#include <queue>
#include <iostream>
#include <stdexcept>

LambdaNFA::LambdaNFA()
    : m_transition{}, m_stateCounter{ 0 }, m_startState{ -1, '\0' }, m_finalState{ -1, '\0' }, m_alphabet{}
{
}

void LambdaNFA::constructFromRegex(std::string_view regex) // assume it's in postfix form
{
    std::stack<LambdaNFA> automatons;

    for (char symbol : regex)
    {
        if (!isOperator(symbol))
        {
            if (!m_alphabet.contains(symbol))
            {
                m_alphabet.insert(symbol);
            }

            State startState = { m_stateCounter++ };
            State endState = { m_stateCounter++ };

            LambdaNFA charNFA{
                constructCharNFA(
                startState,
                endState,
                symbol)
            };
            automatons.push(charNFA);
            continue;
        }

        if (symbol == '*')
        {
            LambdaNFA automaton{ automatons.top() };
            automatons.pop();

            automatons.push(kleeneStarNFA(automaton));
            continue;
        }

        LambdaNFA second{ automatons.top() };
        automatons.pop();
        LambdaNFA first{ automatons.top() };
        automatons.pop();
        if (symbol == '.')
        {
            LambdaNFA mergedNFA{ mergeNFA(first, second) };
            automatons.push(mergedNFA);
        }
        else if (symbol == '|')
        {
            LambdaNFA alternatedNFA{ alternateNFA(first, second) };
            automatons.push(alternatedNFA);
        }
    }

    LambdaNFA result = automatons.top();
    result.m_alphabet = m_alphabet;
    automatons.pop();

    *this = result;
    std::cout << "startState: " << this->m_startState.state.id << "\n";
    std::cout << "finalState: " << this->m_finalState.state.id << "\n";

    auto lambdaClosure = getLambdaClosure(std::unordered_set<State, State::Hash>{State{ 19 }});
    std::cout << "Lambda closure of q19: ";
    for (State state : lambdaClosure)
    {
        std::cout << state.id << " ";
    }
    std::cout << "\n";

    lambdaClosure = getLambdaClosure(std::unordered_set<State, State::Hash>{State{ 24 }});
    std::cout << "Lambda closure of q24: ";
    for (State state : lambdaClosure)
    {
        std::cout << state.id << " ";
    }
    std::cout << "\n";

    lambdaClosure = getLambdaClosure(std::unordered_set<State, State::Hash>{State{ 1 }});
    std::cout << "Lambda closure of q1: ";
    for (State state : lambdaClosure)
    {
        std::cout << state.id << " ";
    }
    std::cout << "\n";
}

DeterministicFiniteAutomaton LambdaNFA::convertToDFA() const
{
    std::unordered_set<State, State::Hash> states;
    std::unordered_set<char> alphabet{ m_alphabet };
    std::map<StateSymbolPair, State> resultingTransitions;
    State startState;
    std::unordered_set<State, State::Hash> finalStates;

    std::vector<std::unordered_set<State, State::Hash>> resultingStates;
    std::queue<std::unordered_set<State, State::Hash>> closuresToCheck;
    std::unordered_set<State, State::Hash> startClosure = getLambdaClosure({ m_startState.state });
    closuresToCheck.push(startClosure);
    resultingStates.push_back(closuresToCheck.front());

    if (startClosure.contains(m_finalState.state))
    {
        finalStates.insert(State{ 0 });
    }

    while (!closuresToCheck.empty())
    {
        std::unordered_set<State, State::Hash> currentClosure = closuresToCheck.front();
        closuresToCheck.pop();

        for (char symbol : m_alphabet)
        {
            std::unordered_set<State, State::Hash> currentTransitions;
            for (State state : currentClosure)
            {
                if (m_transition.contains({ state, symbol }))
                {
                    currentTransitions.insert(
                        m_transition.at({ state, symbol }).begin(),
                        m_transition.at({ state, symbol }).end()
                    );
                }
            }

            std::unordered_set<State, State::Hash> nextClosure = getLambdaClosure(currentTransitions);

            if (nextClosure.empty()) continue;

            int currentStateID = std::distance(resultingStates.begin(),
                std::find(resultingStates.begin(), resultingStates.end(), currentClosure));

            int targetStateID;
            auto it = std::find(resultingStates.begin(), resultingStates.end(), nextClosure);
            if (it == resultingStates.end())
            {
                targetStateID = resultingStates.size();
                resultingStates.push_back(nextClosure);
                closuresToCheck.push(nextClosure);

                if (nextClosure.contains(m_finalState.state))
                {
                    finalStates.insert(State{ targetStateID });
                }
            }
            else
            {
                targetStateID = std::distance(resultingStates.begin(), it);
            }

            resultingTransitions[{State{ currentStateID }, symbol}] = { State{ targetStateID } };
        }
    }

    for (int resultingStateID = 0; resultingStateID < resultingStates.size(); ++resultingStateID)
    {
        states.insert(State{ resultingStateID });
    }

    startState = State{ 0 };

    std::cout << "LAMBDANFA: RESULTING STATES\n";
    int stateID = 0;
    for (const auto& resState : resultingStates)
    {
        std::cout << "q" << stateID++ << ": ";
        for (const auto& state : resState)
        {
            std::cout << state.id << " ";
        }
        std::cout << "\n";
    }

    return DeterministicFiniteAutomaton{ states, alphabet, resultingTransitions, startState, finalStates };
}

std::unordered_set<State, State::Hash> LambdaNFA::getLambdaClosure(const std::unordered_set<State, State::Hash>& states) const
{
    std::unordered_set<State, State::Hash> lambdaClosure;
    std::stack<State> toCheck;
    for (State state : states)
    {
        toCheck.push(state);
        if (m_equivalentStates.contains(state))
        {
            toCheck.push(m_equivalentStates.at(state));
        }
    }

    while (!toCheck.empty())
    {
        State state = toCheck.top();
        lambdaClosure.insert(state);
        toCheck.pop();

        if (m_transition.find({ state, lambda }) == m_transition.end())
        {
            continue;
        }

        std::unordered_set<State, State::Hash> reachable = m_transition.at({ state, lambda });
        for (State state : reachable)
        {
            if (lambdaClosure.contains(state))
            {
                continue;
            }

            toCheck.push(state);
        }
    }

    return lambdaClosure;
}

void LambdaNFA::print() const
{
    for (const auto& [stateSymbol, states] : m_transition)
    {
        std::cout << "transition(q" << stateSymbol.state.id << ", " << stateSymbol.symbol << "): ";
        for (const auto& state : states)
        {
            std::cout << "q" << state.id << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

LambdaNFA LambdaNFA::constructCharNFA(State startSate, State finalState, char symbol)
{
    LambdaNFA result;
    result.m_transition[{startSate, symbol}].insert(finalState);

    result.m_startState = { startSate, symbol };
    result.m_finalState = { finalState, lambda };

    return result;
}

LambdaNFA LambdaNFA::mergeNFA(const LambdaNFA& first, const LambdaNFA& second)
{
    LambdaNFA result{ first };
    result.m_equivalentStates.insert(first.m_equivalentStates.begin(), first.m_equivalentStates.end());
    result.m_equivalentStates.insert(second.m_equivalentStates.begin(), second.m_equivalentStates.end());

    result.m_transition[{result.m_finalState.state, second.m_startState.symbol}] = second.m_transition.at(second.m_startState);

    for (const auto& [stateSymbol, states] : second.m_transition)
    {
        result.m_transition.insert({ stateSymbol, states });
    }

    for (const auto& [stateSymbol, states] : first.m_transition)
    {
        if (states.contains(first.m_finalState.state))
        {
            result.m_transition[stateSymbol].insert(second.m_startState.state);
        }
    }

    result.m_startState = first.m_startState;
    result.m_finalState = second.m_finalState;

    result.m_equivalentStates[first.m_finalState.state] = second.m_startState.state;
    result.m_equivalentStates[second.m_startState.state] = first.m_finalState.state;

    return result;
}

LambdaNFA LambdaNFA::alternateNFA(const LambdaNFA& first, const LambdaNFA& second)
{
    LambdaNFA result;
    result.m_equivalentStates.insert(first.m_equivalentStates.begin(), first.m_equivalentStates.end());
    result.m_equivalentStates.insert(second.m_equivalentStates.begin(), second.m_equivalentStates.end());

    result.m_startState = { m_stateCounter++, lambda };
    result.m_finalState = { m_stateCounter++, lambda };

    result.m_transition[result.m_startState].insert(first.m_startState.state);
    result.m_transition[result.m_startState].insert(second.m_startState.state);

    result.m_transition[first.m_finalState].insert(result.m_finalState.state);
    result.m_transition[second.m_finalState].insert(result.m_finalState.state);

    for (const auto& [stateSymbol, states] : first.m_transition)
    {
        result.m_transition.insert({ stateSymbol, states });
    }

    for (const auto& [stateSymbol, states] : second.m_transition)
    {
        result.m_transition.insert({ stateSymbol, states });
    }

    return result;
}

LambdaNFA LambdaNFA::kleeneStarNFA(const LambdaNFA& automaton)
{
    LambdaNFA result;
    result.m_equivalentStates.insert(automaton.m_equivalentStates.begin(), automaton.m_equivalentStates.end());

    result.m_equivalentStates = automaton.m_equivalentStates;
    result.m_startState = { m_stateCounter++, lambda };
    result.m_finalState = { m_stateCounter++, lambda };

    for (const auto& [stateSymbol, states] : automaton.m_transition)
    {
        result.m_transition.insert({ stateSymbol, states });
    }

    result.m_transition[result.m_startState].insert(automaton.m_startState.state);
    result.m_transition[result.m_startState].insert(result.m_finalState.state);
    result.m_transition[automaton.m_finalState].insert(automaton.m_startState.state);
    result.m_transition[automaton.m_finalState].insert(result.m_finalState.state);

    return result;
}

bool LambdaNFA::isOperator(char symbol)
{
    std::string operators = "|.*";
    return operators.find(symbol) != std::string::npos;
}
