/*
 * Copyright (C) Rida Bazzi, 2017
 *
 * Do not share this file with anyone
 *
 * Do not post this file or derivatives of 
 * of this file online
 *
 */
#include <iostream>
#include <cstdlib>
#include <istream>
#include <sstream>
#include <vector>
#include <string>
#include <cctype>
#include <algorithm>
#include <set>
#include<map>

#include "parser.h"

using namespace std;



Scope *currscope;
TokenType lastVarType , currVarType ;
TokenType expr1, expr2;
bool error, sym ;
string errorcode;
std::vector<Use> result;
bool is_type=false;
bool is_var=false;


void Parser::syntax_error()
{
    cout << "Syntax Error\n";
    exit(1);
}

TokenType Parser::getdatatype(Scope *temp,string name)
{
	if (temp->curr_map.find(name) != temp->curr_map.end())
	{
	    SymbolTable obj = temp->curr_map.find(name)->second;
		return obj.base_type;
	}
	else
	{
		if(temp->parentScope != NULL)
			return getdatatype(temp->parentScope,name);
		else
			return END_OF_FILE;	
	}
}



int Parser::getlinenum(Scope *temp,string name)
{
    if (temp->curr_map.find(name) != temp->curr_map.end())
    {
		SymbolTable obj = temp->curr_map.find(name)->second;
        return obj.lineNo;
	}
    else
    {
        if(temp->parentScope != NULL)
             return getlinenum(temp->parentScope,name);
        else
			return 0;
    
    }
}

void Parser::checkbothvar(TokenType prev, TokenType curr, int num, int line_no)
{
	if (prev != curr)
	{
			if(!error){
				if(num == 1)
					errorcode = "TYPE MISMATCH "+std::to_string(line_no)+" C1";
				else if (num == 2)
					errorcode = "TYPE MISMATCH "+std::to_string(line_no)+" C2";
				else if (num == 3)
					errorcode = "TYPE MISMATCH "+std::to_string(line_no)+" C3";
				error = true;
			}
    }
}



void Parser::checkcurrent(Scope *temp , string lexeme, TokenType expected_type, string str)
{
	if (temp->curr_map.find(lexeme) != temp->curr_map.end())
	{  
		SymbolTable obj = temp->curr_map.find(lexeme)->second;
		if (obj.kind != expected_type)
		{   bool abc =false;
	        bool d = false;
	        if(obj.kind == TYPE && expected_type == ID )
				{abc = !error && str == "u";}
		    if(obj.kind == VAR && expected_type == TYPE )
				{d = !error && str == "u";}
			if(abc){
				errorcode = "ERROR CODE 1.3 "+lexeme;
				error = true;
			}
			else if(d){
				errorcode = "ERROR CODE 2.3 "+lexeme;
				error = true;
			}
		}	
    }
	else
	{
		 bool abc = str == "u" && !error && temp->parentScope == NULL;
		if(temp->parentScope == NULL && str == "d") 
		{}
		else if (abc) 
		{  if(expected_type== TYPE)
			{errorcode = "ERROR CODE 1.4 "+lexeme;
			error = true;}
			if(expected_type== ID)
			{errorcode = "ERROR CODE 2.4 "+lexeme;
			error = true;}
		}
		else
		{
			checkcurrent(temp->parentScope,lexeme,expected_type,str);
		}
	}
}

Token Parser::expect(TokenType expected_type)
{
    Token t = lexer.GetToken();
    if (t.token_type != expected_type)
        syntax_error();
    return t;
}

Token Parser::peek()
{
    Token t = lexer.GetToken();
    lexer.UngetToken(t);
    return t;
}

// Parsing

void Parser::parse_program()
{
    // program -> scope
    
    parse_scope();
}

void Parser::parse_scope()
{
    // scope -> { scope_list }
	Scope scope ;
    scope.parentScope = currscope;
    currscope = &scope;
    expect(LBRACE);
    parse_scope_list();
    expect(RBRACE);
    currscope = currscope->parentScope;
}


void Parser::parse_scope_list()
{
   // scope_list -> stmt scope_list1    
    // scope_list -> scope scope_list1
    // scope_list -> declaration scope_list1
    // scope_list1 -> scope_list | epsilon
    
    Token t = peek();
    if (t.token_type == ID || t.token_type == WHILE)
    {
    	parse_stmt();
    	parse_scope_list1();
    }
    else if(t.token_type == TYPE || t.token_type == VAR)
    {
    	parse_declaration();
    	parse_scope_list1();
    }
    else if(t.token_type == LBRACE)
    {
    	parse_scope();
    	parse_scope_list1();
    }
    else
    {
    	syntax_error();
    }


}

void Parser::parse_scope_list1()
{
	Token t = peek();
	if(t.token_type == ID || t.token_type == WHILE || t.token_type == TYPE || t.token_type == VAR || t.token_type == LBRACE)
	{
		parse_scope_list();
	}
	else
	{
		if (t.token_type == RBRACE)
		{
		}
		else
		{syntax_error();
		}
	}
}

void Parser::parse_declaration()
{
    // declaration -> type_decl
    // declaration -> var_decl
    Token t = peek();
    if (t.token_type == TYPE)
    	parse_type_decl();
    if (t.token_type == VAR)
    	parse_var_decl();
}

void Parser::parse_type_decl()
{
    // type_decl -> TYPE id_list COLON type_name SEMICOLON
	
	is_type=true;
    expect(TYPE);
    parse_id_list();
    expect(COLON);
    parse_type_name();
    expect(SEMICOLON);
	is_type=false;
}

void Parser::parse_type_name()
{
    // type_name -> REAL
    // type_name -> INT
    // type_name -> BOOLEAN
    // type_name -> STRING
    // type_name -> LONG
    // type_name -> ID

    Token t = peek();
    Token t1;
    if(t.token_type == REAL||t.token_type == INT||t.token_type == BOOLEAN||t.token_type == STRING||t.token_type == LONG)
    {
    	expect(t.token_type);
		std::map<string,SymbolTable>::iterator i;
		for ( i=currscope->curr_map.begin(); i!=currscope->curr_map.end(); ++i)
		{
			SymbolTable temp =  i->second ;
			if (temp.base_type == END_OF_FILE)
			{
				i->second.base_type = t.token_type;
			}
	    }
		
    }
    else if(t.token_type == ID)
    {
    	expect(ID);
		checkcurrent(currscope,t.lexeme,TYPE,"u"); 
		TokenType expected_type =  getdatatype(currscope,t.lexeme);
		std::map<string,SymbolTable>::iterator i;
		for (i=currscope->curr_map.begin(); i!=currscope->curr_map.end(); ++i)
		{
			SymbolTable temp =  i->second ;
			if (temp.base_type == END_OF_FILE)
			{
				i->second.base_type = expected_type;
			}
		}
        int abc = getlinenum(currscope,t.lexeme);
		 Use temp;
		 temp.name = t.lexeme;
		 temp.declarenum = abc;
		 temp.linenum = t.line_no;
		 result.push_back(temp);
    }
    else
    {
    	syntax_error();
    }
}

void Parser::parse_var_decl()
{
    // var_decl -> VAR id_list COLON type_name SEMICOLON
	is_var=true;
    expect(VAR);
    parse_id_list();
    expect(COLON);
    parse_type_name();
    expect(SEMICOLON);
	is_var=false;
}

void Parser::parse_id_list()
{   TokenType expected_type;
    if(is_var)
		expected_type= VAR;
	else if(is_type)
		expected_type=TYPE;
    Token t1 = peek();
	expect(ID);
	SymbolTable obj;
	checkcurrent(currscope,t1.lexeme,expected_type,"d");
	obj.kind = expected_type;
	obj.lineNo = t1.line_no;
	obj.name = t1.lexeme;
	obj.base_type = END_OF_FILE;
	
    if (currscope->curr_map.find(obj.name) == currscope->curr_map.end())
	{
		currscope->curr_map[obj.name] = obj;
	}
	else
	{
		SymbolTable alreadyPresent = currscope->curr_map.find(obj.name)->second;
		
		if (alreadyPresent.kind == TYPE && expected_type == TYPE)
		{
			if(!error){
			errorcode = "ERROR CODE 1.1 "+obj.name;
			error = true;
			}
		}
		else if (alreadyPresent.kind == TYPE && expected_type == VAR)
		{
			if(!error){
				errorcode = "ERROR CODE 1.2 "+obj.name;
				error = true;
			}
        }
		if (alreadyPresent.kind == VAR && expected_type == VAR)
		{
			if(!error){
				errorcode = "ERROR CODE 2.1 "+obj.name;
				error = true;
			}
		}
		else if (alreadyPresent.kind == VAR && expected_type == TYPE)
		{
			if(!error){
				errorcode = "ERROR CODE 2.2 "+obj.name;
				error = true;
			}
        }
		
	}	
	Token t = peek();
    if (t.token_type == COMMA)
    {
    	expect(COMMA);
    	parse_id_list();
    }
}



void Parser::parse_stmt_list()
{
    // stmt_list -> stmt
    // stmt_list -> stmt stmt_list
   
    parse_stmt();
    Token t = peek();
    if (t.token_type == WHILE || t.token_type == ID)
    {
        // stmt_list -> stmt stmt_list
        parse_stmt_list();
    }
    else if (t.token_type == RBRACE)
    {
        // stmt_list -> stmt
    }
    else
    {
        syntax_error();
    }
}

void Parser::parse_stmt()
{
    // stmt -> assign_stmt
    // stmt -> while_stmt

    Token t = peek();
    if (t.token_type == ID)
         parse_assign_stmt();
    else if(t.token_type == WHILE)
         parse_while_stmt();
    else
         syntax_error();   
}

void Parser::parse_assign_stmt()
{
    // assign_stmt -> ID EQUAL expr SEMICOLON
	
    Token t = peek();
	expect(ID);
    checkcurrent(currscope,t.lexeme,ID,"u");
  	lastVarType = getdatatype(currscope, t.lexeme);
    sym = false;
    int abc = getlinenum(currscope,t.lexeme);
	Use temp;
    temp.name = t.lexeme;
    temp.declarenum = abc;
    temp.linenum = t.line_no;
	result.push_back(temp);
	expect(EQUAL);
    parse_expr();
    expect(SEMICOLON);
}

void Parser::parse_while_stmt()
{
   // while_stmt -> WHILE condition LBRACE stmt list RBRACE
    expect(WHILE);
    parse_condition();
    expect(LBRACE);
    parse_stmt_list();
    expect(RBRACE);
}

void Parser::parse_expr()
{
    // expr -> term 
    // expr -> term + expr
	
	Token t = peek();
	if (t.token_type == LPAREN || t.token_type == NUM || t.token_type == REALNUM || t.token_type == ID)
	{
		/* code */
		parse_term();
		Token a = peek();
		if(a.token_type == PLUS)
		{
			expect(PLUS);
			sym = true;
			parse_expr();
		}

	}
	else
	{
		syntax_error();
	}
}

void Parser::parse_term()
{
    // term -> factor
    // term -> factor MULT term
    
    Token t = peek();
    if (t.token_type == LPAREN || t.token_type == NUM || t.token_type == REALNUM || t.token_type == ID)
    {
    	parse_factor();
    	Token a = peek();
    	if (a.token_type == MULT)
    	{
    		expect(MULT);
    		sym = true;
    		parse_term();
    	}
    }
    else
    {
    	syntax_error();
    }
}


void Parser::parse_factor()
{
    // factor -> LPAREN expr RPAREN
    // factor -> NUM
    // factor -> REALNUM
    // factor -> ID

	Token t = peek();
	if (t.token_type == LPAREN)
	{   expect(LPAREN);
		parse_expr();
	    expect(RPAREN); }
	else if (t.token_type == NUM)
	{    expect(NUM);
		if(sym){
			expr2 = INT;
			checkbothvar(expr2, expr1,2,t.line_no);	
		    sym = false; }
		else
			expr1 = INT;
		checkbothvar(lastVarType, INT,1,t.line_no);
	}
	else if (t.token_type == REALNUM)
	{
		Token t1 = peek();
		expect(REALNUM);
		if (sym)
		{   expr2 = REAL;
			checkbothvar(expr2, expr1,2,t1.line_no);	
			sym = false;}
		else
			expr1 = REAL;
		checkbothvar(lastVarType, REAL,1,t1.line_no);
		
	}
	else if (t.token_type == ID)
	{
		Token t1 = peek();
		expect(ID);
        checkcurrent(currscope,t1.lexeme,ID,"u");
		currVarType = getdatatype(currscope,t1.lexeme);
		if(sym){
			expr2 = currVarType;
			checkbothvar(expr2, expr1,2,t1.line_no);	
			sym = false;}
		else
			expr1 = currVarType;
		checkbothvar(lastVarType, currVarType,1,t1.line_no);
		int abc = getlinenum(currscope,t1.lexeme);
        Use temp;
		temp.name = t1.lexeme;
		temp.declarenum = abc;
		temp.linenum = t1.line_no;
        result.push_back(temp);	
	}
	else{
		syntax_error();
	}
}

void Parser::parse_condition()
{
    // condition -> ID
    // condition -> primary relop primary

    Token t = peek();
    if (t.token_type == ID)
    {
    	Token t1 = peek();
		expect(ID);
    	checkcurrent(currscope,t1.lexeme,ID,"u");
    	sym = false;    	
    	expr1 = getdatatype(currscope,t1.lexeme);
		int abc = getlinenum(currscope,t1.lexeme);
        Use temp;
		temp.name = t1.lexeme;
		temp.declarenum = abc;
		temp.linenum = t1.line_no;
        result.push_back(temp);  
		Token n = peek();
		if (n.token_type == GREATER || n.token_type == GTEQ || n.token_type == LESS || n.token_type == NOTEQUAL || n.token_type == LTEQ)
    	{
    		parse_relop();
    		sym = true;
    		parse_primary();
    	}
        else
        {	expr1 = getdatatype(currscope,t1.lexeme);
            checkbothvar(expr1, BOOLEAN,4,t1.line_no);}
    }
    else if (t.token_type == NUM || t.token_type == REALNUM)
    {
    	expect(t.token_type);
        if (t.token_type == NUM)
            expr1 = INT;
        else
            expr1 = REAL;   
    	parse_relop();
    	parse_primary();
    }
    else
        syntax_error();

}

void Parser::parse_primary()
{
    // primary -> ID
    // primary -> NUM
    // primary -> REALNUM
    // TODO
    Token t = lexer.GetToken();
    if (t.token_type == ID)
    {
    	if (sym)
    	{
    		expr2 = getdatatype(currscope,t.lexeme);
            checkcurrent(currscope,t.lexeme,ID,"u");
    		checkbothvar(expr2,expr1,3,t.line_no);
			int abc = getlinenum(currscope,t.lexeme);
            Use temp;
			temp.name = t.lexeme;
			temp.declarenum = abc;
			temp.linenum = t.line_no;
			result.push_back(temp);  
    	}
    }
    else if (t.token_type == NUM)
    {   if (sym)
    	{   expr2 = INT;
    		checkbothvar(expr2,expr1,3,t.line_no);}
    }
    else if (t.token_type == REALNUM)
    {
    	if (sym)
    	{   expr2 = REAL;
    		checkbothvar(expr2,expr1,3,t.line_no);}
    }
    else
        syntax_error();

}

void Parser::parse_relop()
{
    // relop -> GREATER
    // relop -> GTEQ
    // relop -> LESS
    // relop -> NOTEQ
    // relop -> LTEQ
    Token t = peek();
    if (t.token_type == GREATER || t.token_type== GTEQ || t.token_type== LESS || t.token_type== NOTEQUAL || t.token_type== LTEQ )
    {   expect(t.token_type);
        sym=true;
    }
    else
    {
    	syntax_error();
    }
}

void Parser::ParseInput()
{
    parse_program();
    expect(END_OF_FILE);
}

int main()
{
    Parser parser;
    currscope = NULL;
    error = false;
    sym = false;

    parser.ParseInput();
    if(error)
    {
    	cout<<"\n"<<errorcode;
	}
    else
    {
        for(int i=0;i < result.size();i++)
        {
            Use abc = result[i];
            cout<<"\n"<<abc.name<<" "<<abc.linenum<<" "<<abc.declarenum;
        }
    }

}




