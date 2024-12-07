#pragma once
#include <functional>

struct State
{
    int id;

    bool operator==(const State& other) const
    {
        return this->id == other.id;
    }

    struct Hash
    {
        size_t operator()(const State& state) const
        {
            return std::hash<int>()(state.id);
        }
    };
};

struct StateSymbolPair
{
    State state;
    char symbol;

    bool operator<(const StateSymbolPair& other) const
    {
        if (this->state.id == other.state.id)
        {
            return this->symbol < other.symbol;
        }

        return this->state.id < other.state.id;
    }
};