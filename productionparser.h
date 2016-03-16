#ifndef PRODUCTIONPARSER_H
#define PRODUCTIONPARSER_H

#include <string>
#include <map>
#include <set>
#include <memory>
#include <vector>
#include "grammarsym.h"

typedef char Terminal;
typedef char NonTerminal;


struct Production
{
    std::shared_ptr<GrammarSym> start_symbol;
    std::vector<std::shared_ptr<GrammarSym>> body_symbol;

    int number;

    bool nullable;
    std::set<std::shared_ptr<GrammarSym>> First;
    Production() = default;
    Production(int n) : number(n){ }
    Production(Production&&) = default;
    Production(const Production&) = default;
    ~Production() = default;
};

class ProductionParser
{
public:
    ProductionParser(std::string);
    void test();
private:
    std::map<char,std::shared_ptr<GrammarSym>> sym_map;
    std::vector<Production> productions;
    int pro_num;
    int count_terminal;
    int count_nonterminal;
    std::vector<std::vector<int>> table;

    void parse(std::string);
    bool isNonTerminal(char ch)  { return ch <= 'Z' && ch >= 'A'; }
    void solveNullable();
    void solveFirst();
    void solveFollow();
    void solveFirstOfProduction();
    void createPredictingAnalysisTable();
};

#endif // PRODUCTIONPARSER_H
