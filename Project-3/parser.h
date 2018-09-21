/*
 * Copyright (C) Rida Bazzi, 2017
 *
 * Do not share this file with anyone
 */
#ifndef __PARSER_H__
#define __PARSER_H__

#include <cstdlib>
#include <string>
#include "lexer.h"
using namespace std;

struct Use{
public:
    string name;
    int linenum;
    int declarenum;
};

class SymbolTable
{
public:
    string name;
    TokenType kind;
    TokenType base_type;
	int lineNo;
};


class Scope
{
public:
    Scope *parentScope;
    std::map<string, SymbolTable> curr_map;
};



class Parser {
  private:
    LexicalAnalyzer lexer;

    void syntax_error();
    Token expect(TokenType expected_type);
    Token peek();

    int getlinenum(Scope *temp,string name);
    void checkcurrent(Scope *temp , string lexeme, TokenType expected_type, string str);
    void checkbothvar(TokenType prev, TokenType curr, int num, int line_no);
	TokenType getdatatype(Scope *temp,string name);


    
    void parse_program();
    void parse_scope();
    void parse_scope_list();
    void parse_scope_list1();
    void parse_declaration();
    void parse_type_decl();
    void parse_type_name();
    void parse_var_decl();
    void parse_id_list();
    void parse_stmt_list();
    void parse_stmt();
    void parse_assign_stmt();
    void parse_while_stmt();
    void parse_expr();
    void parse_term();
    void parse_factor();
    void parse_condition();
    void parse_primary();
    void parse_relop();

    

  public:
    void ParseInput();
};

#endif


