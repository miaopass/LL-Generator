#include "productionparser.h"
#include <fstream>
#include <iostream>
#include <map>

ProductionParser::ProductionParser(std::string path):pro_num(0),count_nonterminal(0)
{
     std::ifstream file(path);
     if(file)
     {
         std::string line;
         while(std::getline(file,line))
         {
             if(!line.empty())
                parse(line);
         }
     }
}

void ProductionParser::parse(std::string line)
{
    Production prod(pro_num++);
    char start = line[0];
    sym_map.insert({start,std::shared_ptr<GrammarSym>(new NonTerminalStruct(start))});
    prod.start_symbol = sym_map[start];
    for(auto it = line.begin()+2;it != line.end();it++)
    {
        if(isNonTerminal(*it))
        {
            sym_map.insert({*it,std::shared_ptr<GrammarSym>(new NonTerminalStruct(*it))});
            count_nonterminal++;
        }
        else
        {
            sym_map.insert({*it,std::shared_ptr<GrammarSym>(new TerminalStruct(*it))});
            count_terminal++;
        }
        prod.body_symbol.push_back(sym_map[*it]);
    }
    productions.push_back(std::move(prod));
}

void ProductionParser::solveNullable()
{
    bool isChanged = false;

    do
    {
        isChanged = false;

        for(auto pro : productions)
        {
            if(!pro.start_symbol->nullable)
            {
                bool nullable = true;
                for(auto sym : pro.body_symbol)
                {
                    if(!sym->nullable)
                    {
                        nullable = false;
                        break;
                    }
                }
                if(nullable)
                {
                    pro.start_symbol->nullable = true;
                    pro.nullable = true;
                    isChanged = true;
                }
            }
        }

    } while(isChanged);
}

void ProductionParser::solveFirst()
{
    for(auto sym : sym_map)
    {
        if(sym.second->getType() == TERMINAL)
        {
            sym.second->First.insert(sym.second);
        }
    }

    bool isChanged = false;

    do
    {
        isChanged = false;

        for(auto pro : productions)
        {
            auto temp_set = pro.start_symbol->First;
            for(auto sym : pro.body_symbol)
            {
                temp_set.insert(sym->First.begin(),sym->First.end());
                if(!sym->nullable)
                    break;
            }
            if(temp_set != pro.start_symbol->First)
            {
                pro.start_symbol->First = temp_set;
                isChanged = true;
            }
        }

    }while(isChanged);
}

void ProductionParser::solveFirstOfProduction()
{
    for(auto& pro : productions)
    {
        for(auto sym : pro.body_symbol)
        {
            pro.First.insert(sym->First.begin(),sym->First.end());
            if(!sym->nullable)
                break;
        }
    }
}

void ProductionParser::solveFollow()
{
    bool isChanged = false;

    do
    {
        isChanged = false;

        for(auto pro : productions)
        {
            for(auto it = pro.body_symbol.rbegin();it != pro.body_symbol.rend();it++)
            {
                auto temp_set = (*it)->Follow;
                temp_set.insert(pro.start_symbol->Follow.begin(),pro.start_symbol->Follow.end());
                if(temp_set != (*it)->Follow)
                {
                    (*it)->Follow = temp_set;
                    isChanged = true;
                }
                if(!(*it)->nullable)
                    break;
            }

            for(auto i = pro.body_symbol.begin();i + 1 < pro.body_symbol.end(); i++)
            {
                for(auto j = i + 1;j < pro.body_symbol.end();j++)
                {
                    auto temp_set = (*i)->Follow;
                    temp_set.insert((*j)->First.begin(),(*j)->First.end());
                    if(temp_set != (*i)->Follow)
                    {
                        (*i)->Follow = temp_set;
                        isChanged = true;
                    }
                    if(!(*j)->nullable)
                        break;
                }
            }
        }

    }while(isChanged);
}

void ProductionParser::createPredictingAnalysisTable()
{
    table.resize(count_nonterminal);
    for(auto row : table)
        row.resize(count_terminal);
    for(auto it = productions.begin();it != productions.end();it++)
    {

    }
}

void ProductionParser::test()
{
    solveNullable();
    solveFirst();
    solveFollow();
    solveFirstOfProduction();
    for(auto pro : productions)
    {
        std::cout<<pro.start_symbol->name<<" -> ";
        for(auto sym : pro.body_symbol)
        {
            std::cout<<sym->name<<" ";
        }
        std::cout<<"  ";
        for(auto sym : pro.First)
            std::cout<<sym->name<<" ";
        std::cout<<std::endl;
    }
    std::cout<<std::endl;
    for(auto sym : sym_map)
    {
        std::cout<<sym.second->name<<" "<<sym.second->nullable<<" ";
        for(auto s : sym.second->First)
            std::cout<<s->name<<" ";
        std::cout<<"     ";
        for(auto s : sym.second->Follow)
            std::cout<<s->name<<" ";
        std::cout<<std::endl;
    }

}
