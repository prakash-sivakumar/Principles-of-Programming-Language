#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <cstring>
#include <stdlib.h>
#include <string.h>
#include "compiler.h"
#include "lexer.h"
#include "ir_debug.h"

struct StatementNode* parsing_cases(struct StatementNode* b,struct ValueNode* a );
struct StatementNode* assignment(StatementNode* start,int a);
struct StatementNode* oper(StatementNode* start);
struct StatementNode* assign_parsing();
struct StatementNode* switchcase(struct StatementNode* s,struct ValueNode* a);
struct StatementNode* check(struct StatementNode* s,struct StatementNode* b,struct ValueNode* a);
struct StatementNode* parsing_statements();
struct StatementNode* parse_generate_intermediate_representation();
struct StatementNode* body_statement();
struct ValueNode* findvalue();
int x = 0,count=0;
struct ValueNode* arr[50];
LexicalAnalyzer lexer;
 
struct StatementNode* assignment(StatementNode* start,int a)
 {   int te=0;
	 Token t = lexer.GetToken();
     if(t.token_type== NUM)
	 {
			te = count+x;
			arr[te] = new ValueNode{};
			arr[te]->value = atoi((t.lexeme).c_str());
			arr[te]->name = "";    
			if(a==1)	 
					start->assign_stmt->operand1 = arr[te];
			else if (a==2)
					start->assign_stmt->operand2 = arr[te];
			else if (a==3)
				    start->if_stmt->condition_operand1 = arr[te];
			else if (a==4)
				    start->if_stmt->condition_operand2 = arr[te];
			x++;
	 }
	 else if (t.token_type == ID)
	 {
			 for(int i =0; i< count; i++)
			 {
				 if(arr[i]->name == t.lexeme)
				 {
				  if(a==1)	 
						start->assign_stmt->operand1 = arr[i];
				  else if (a==2)
						start->assign_stmt->operand2 = arr[i];
				  else if (a==3)
						start->if_stmt->condition_operand1 = arr[i];
				  else if (a==4)
						start->if_stmt->condition_operand2 = arr[i];
				 }
			 }
	 }
	 return start;
 }
 
 struct StatementNode* oper(StatementNode* start)
 {  
    Token t = lexer.GetToken(); 
	if (t.token_type ==  PLUS)
	{
		start->assign_stmt->op = OPERATOR_PLUS;
	}
	else if(t.token_type == MINUS)
	{
		start->assign_stmt->op =  OPERATOR_MINUS;
	}
	else if(t.token_type == MULT)
	{
		start->assign_stmt->op =  OPERATOR_MULT;
	}
	else if(t.token_type == DIV)
	{
		start->assign_stmt->op =  OPERATOR_DIV;
	}
	else if(t.token_type == LESS )
	{
		start->if_stmt->condition_op = CONDITION_LESS;
	}
	else if(t.token_type == GREATER)
	{
		start->if_stmt->condition_op = CONDITION_GREATER;
	}
	else if(t.token_type == NOTEQUAL)
	{
		start->if_stmt->condition_op =  CONDITION_NOTEQUAL;
	}
	return start;
 }
 
 
 struct StatementNode* assign_parsing()
{   
    struct StatementNode* sta = (StatementNode*) calloc(1,sizeof(StatementNode));
    Token t = lexer.GetToken();
    if (t.token_type == ID)
    {
        sta->type = ASSIGN_STMT;
        sta->assign_stmt = (AssignmentStatement*) calloc(1,sizeof(AssignmentStatement));
        for(int i =0; i< count; i++)
        {
           if(arr[i]->name == t.lexeme)
           {
           sta->assign_stmt->left_hand_side = arr[i];
           }
        }
    }
    t = lexer.GetToken(); 
    if (t.token_type == EQUAL)
    {   sta = assignment(sta,1); 
        t = lexer.GetToken();
        if (t.token_type != SEMICOLON)
        {   lexer.UngetToken(t);
            sta= oper(sta);
            sta = assignment(sta,2); 
            t = lexer.GetToken();
        }
        else
        {
            sta->assign_stmt->op = OPERATOR_NONE;
			sta->assign_stmt->operand2 = NULL;
        }
    }
	sta->next = NULL;
    return sta;
}

struct StatementNode* switchcase(struct StatementNode* s,struct ValueNode* a)
{
   Token t = lexer.GetToken();  
   if(t.token_type== DEFAULT)
      s -> if_stmt -> condition_operand2 = 													;
   else if (t.token_type==CASE)
   {
        t = lexer.GetToken();
        arr[count] = new ValueNode{};
        arr[count] -> name = "";
        arr[count] -> value = atoi(t.lexeme.c_str());
        s->if_stmt->condition_operand2 = arr[count];
        count++;
   }
   return s;
}

struct StatementNode* check(struct StatementNode* s,struct StatementNode* b,struct ValueNode* a)
{
	Token t = lexer.GetToken(); 
   if(t.token_type == DEFAULT || t.token_type == CASE)
   {
      lexer.UngetToken(t);
      s->if_stmt->true_branch = parsing_cases(b, a);
   }  
   else
   {
      lexer.UngetToken(t);
      s->if_stmt->true_branch = b;
   }
   return s;
}

struct StatementNode* parsing_cases(struct StatementNode* b,struct ValueNode* a )
{
   struct StatementNode* start = (StatementNode*) calloc(1,sizeof(StatementNode));
   start->if_stmt = (IfStatement*) calloc(1,sizeof(IfStatement));
   start->type = IF_STMT;
   start->if_stmt->condition_operand1 = a;
   start->if_stmt->condition_op = CONDITION_NOTEQUAL;
   start = switchcase(start,a);
   Token t = lexer.GetToken();  
   start->if_stmt->false_branch = body_statement();
   struct StatementNode* temp2 = start->if_stmt->false_branch;
   while(temp2->next != NULL)
   {temp2 = temp2 -> next;}
   temp2->next = b;
   start = check(start,b,a);
   return start;
}

struct ValueNode* findvalue()
{
	  Token t = lexer.GetToken();   
	  struct ValueNode* a;
	  if(t.token_type == NUM)
	  {
		 arr[count] = new ValueNode{};
		 arr[count] -> value = atoi(t.lexeme.c_str());
		 arr[count] -> name = "";
		 a = arr[count];
		 count++;
	  }
	  else if(t.token_type == ID)
	  {
		for(int i = 0; i< count; i++)
		 {if(t.lexeme == arr[i]->name)
			  a = arr[i];}
	  }
	  return a;
}

struct StatementNode* parsing_statements()
{
    struct StatementNode *start =  (StatementNode*) calloc(1,sizeof(StatementNode));
	struct StatementNode *stl;
    Token t = lexer.GetToken();
    TokenType type = t.token_type;
    lexer.UngetToken(t);
    Token t1 = lexer.GetToken();
	if(t1.token_type== ID)
	{
			lexer.UngetToken(t1);
			start = assign_parsing();
	}
	else if(t1.token_type== PRINT)
    {
            Token t;
			t = lexer.GetToken();
			start->type = PRINT_STMT;
			start->print_stmt = (PrintStatement*) calloc(1,sizeof(PrintStatement));
			 for(int i =0; i< count; i++)
			{
			   if(arr[i]->name == t.lexeme)
			      start->print_stmt->id = arr[i];
			}
			t = lexer.GetToken();
	}
	else if(t1.token_type== IF)
	{
            struct StatementNode* st_end = (StatementNode*) calloc(1,sizeof(StatementNode));
			st_end->type = NOOP_STMT;
			start->type = IF_STMT;
			start->if_stmt = (IfStatement*) calloc(1,sizeof(IfStatement));
			start->next = st_end;
			st_end->next = NULL;
			start=assignment(start,3);
			start = oper(start);
			start=assignment(start,4);
			start->if_stmt->true_branch = body_statement();
			start->if_stmt->false_branch = st_end;  
			struct StatementNode* cur = start->if_stmt->true_branch;
			while(cur->next != NULL)
		    {cur = cur-> next;}
			cur->next = st_end; 
	}
	else if(t1.token_type== WHILE)
	{	
			struct StatementNode* end = (StatementNode*) calloc(1,sizeof(StatementNode));
			struct StatementNode* iterate =(StatementNode*) calloc(1,sizeof(StatementNode));
			start->if_stmt = (IfStatement*) calloc(1,sizeof(IfStatement));
			start->next = end;
			iterate->next = NULL;
			end->next = NULL;
			iterate->goto_stmt = (GotoStatement*) calloc(1,sizeof(GotoStatement));
		    start= assignment(start,3);
		    start = oper(start);
		    start = assignment(start,4); 		
		    start->if_stmt->true_branch = body_statement();
		    start->if_stmt->false_branch = end;
		    iterate->goto_stmt->target = start;
		    struct StatementNode* temp = start->if_stmt->true_branch;
		    while(temp->next != NULL)
		    {temp = temp-> next;}
			temp->next = iterate;
			iterate->type = GOTO_STMT;
			end->type = NOOP_STMT;
			start->type = IF_STMT;
	}
	else if(t1.token_type== FOR)
	{	  struct StatementNode* end = (StatementNode*) calloc(1,sizeof(StatementNode));
		  struct StatementNode* recurse = (StatementNode*) calloc(1,sizeof(StatementNode));
		  struct StatementNode* st1 = (StatementNode*) calloc(1,sizeof(StatementNode));
		  struct StatementNode* li;
		  st1->if_stmt = (IfStatement*) calloc(1,sizeof(IfStatement));
		  st1->next =end;
		  recurse ->goto_stmt = (GotoStatement*) calloc(1,sizeof(GotoStatement));
		  recurse->goto_stmt->target = st1;
		  recurse->next = end;
		  Token t;  
		  t = lexer.GetToken();        
		  start = assign_parsing();         
		  start ->next = st1;
		  st1 = assignment(st1,3);
		  st1 = oper(st1);
		  st1 = assignment(st1,4);
	      t = lexer.GetToken();  
          struct StatementNode* ab;		  
	      ab = assign_parsing();   
	      t = lexer.GetToken();    
	      li = body_statement();
	      struct StatementNode* temp;
	      temp = li;
	      while(temp->next != NULL)
	      {temp = temp-> next;}
	      temp->next = ab;
		  st1->if_stmt->true_branch = li;
		  temp = st1->if_stmt->true_branch;
		  st1->if_stmt->false_branch = end;
		  while(temp->next != NULL)
		  {temp = temp->next;}
		  temp->next = recurse;
		  st1->type = IF_STMT;
		  end->type = NOOP_STMT;
		  recurse->type = GOTO_STMT;
    }
	else  if(t1.token_type== SWITCH)
	{		  struct StatementNode* temp = (StatementNode*) calloc(1,sizeof(StatementNode));
			  struct StatementNode* end = (StatementNode*) calloc(1,sizeof(StatementNode));
			  temp->goto_stmt = (GotoStatement*) calloc(1,sizeof(GotoStatement));
			  temp->goto_stmt->target = end;
			  end->type = NOOP_STMT;
			  temp->type = GOTO_STMT;
			  struct ValueNode* a;
			  a = findvalue();
			  t = lexer.GetToken(); 
			  start = parsing_cases(temp, a);
			  start -> next = end;
			  t = lexer.GetToken();
	}
    t = lexer.GetToken();
    if ((t.token_type == ID) || (t.token_type == WHILE) || (t.token_type == PRINT) || (t.token_type == IF) || (t.token_type == SWITCH) || (t.token_type == FOR))
    {
        lexer.UngetToken(t);
        stl = parsing_statements();
	    if (type == FOR || start->type == IF_STMT )
	    {
            struct StatementNode *cur = start->next;	  
            while (cur->type != NOOP_STMT)
            { cur = cur->next;}
            cur->next = stl;
	    }
	    else
	      start->next = stl;
	}
    else
        lexer.UngetToken(t);
    return start;
}

struct StatementNode* body_statement()
{
	struct StatementNode* ab;
	Token t = lexer.GetToken(); 
	if (t.token_type == LBRACE)
	{ab = parsing_statements();} 
	t = lexer.GetToken(); 
	return ab;
}

struct StatementNode* parse_generate_intermediate_representation()
{
   struct StatementNode* result;
   Token t = lexer.GetToken();
   while (t.token_type != SEMICOLON)     
  {
   arr[count] = new ValueNode{};
   arr[count] -> value = 0;
   arr[count] -> name = t.lexeme;
   count++;
   t = lexer.GetToken();
   }
  result = body_statement();  
  return  result; 
}
