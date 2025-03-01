# AFD & Regex Compiler 🤖

This repository contains two main components:
1. **A Deterministic Finite Automaton (DFA)** implemented in C++.
2. **A Regex Compiler** implemented in C#.

## Deterministic Finite Automaton (DFA) 🤖

The Deterministic Finite Automaton (DFA) is a C++ implementation that allows you to check whether a string of characters is accepted by a specific deterministic automaton.

### Features:
- Implemented in C++.
- Uses efficient data structures for representing states and transitions.
- Allows reading and defining an automaton from a configuration file.
- Supports string validation based on the defined automaton.

## Regex Compiler (C#) 💻

The Regex Compiler is a C# application that transforms a regex into an equivalent finite automaton.

### Features:
- Implemented in C# .NET.
- Supports standard regular expressions.
- Converts a regex into an equivalent finite automaton.
- Generates C++ code for the resulting DFA.
