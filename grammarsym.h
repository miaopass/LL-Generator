#ifndef GRAMMARSYM_H
#define GRAMMARSYM_H

#include <set>
#include <map>
#include <memory>
#include <string>

enum SymType
{
    TERMINAL,NONTERMINAL
};

class GrammarSym
{
public:
    GrammarSym(char n) : nullable(false),name(n) { }
    virtual ~GrammarSym() = default;
    virtual SymType getType() = 0;
    bool nullable;
    std::set<std::shared_ptr<GrammarSym>> First,Follow;
    char name;
};

class TerminalStruct:public GrammarSym
{
public:
    TerminalStruct(char n) : GrammarSym(n) { }
    virtual ~TerminalStruct() = default;
    virtual SymType getType() { return TERMINAL;}
};

class NonTerminalStruct:public GrammarSym
{
public:
    NonTerminalStruct(char n) : GrammarSym(n) { }
    virtual ~NonTerminalStruct() = default;
    virtual SymType getType() { return NONTERMINAL;}
};

#endif // GRAMMARSYM_H
