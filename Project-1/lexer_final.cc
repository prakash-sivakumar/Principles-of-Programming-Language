/*
 * Copyright (C) Rida Bazzi, 2017
 *
 * Do not share this file with anyone
 */
#include <iostream>
#include <istream>
#include <vector>
#include <string>
#include <cctype>

#include "lexer.h"
#include "inputbuf.h"

using namespace std;

string reserved[] = { "END_OF_FILE",
    "IF", "WHILE", "DO", "THEN", "PRINT",
    "PLUS", "MINUS", "DIV", "MULT",
    "EQUAL", "COLON", "COMMA", "SEMICOLON",
    "LBRAC", "RBRAC", "LPAREN", "RPAREN",
    "NOTEQUAL", "GREATER", "LESS", "LTEQ", "GTEQ",
    "DOT", "NUM", "ID", "ERROR", "REALNUM", "BASE08NUM","BASE16NUM" // TODO: Add labels for new token types here (as string)
};

#define KEYWORDS_COUNT 5
string keyword[] = { "IF", "WHILE", "DO", "THEN", "PRINT" };

void Token::Print()
{
    cout << "{" << this->lexeme << " , "
         << reserved[(int) this->token_type] << " , "
         << this->line_no << "}\n";
}

LexicalAnalyzer::LexicalAnalyzer()
{
    this->line_no = 1;
    tmp.lexeme = "";
    tmp.line_no = 1;
    tmp.token_type = ERROR;
}

bool LexicalAnalyzer::SkipSpace()
{
    char c;
    bool space_encountered = false;

    input.GetChar(c);
    line_no += (c == '\n');

    while (!input.EndOfInput() && isspace(c)) {
        space_encountered = true;
        input.GetChar(c);
        line_no += (c == '\n');
    }

    if (!input.EndOfInput()) {
        input.UngetChar(c);
    }
    return space_encountered;
}

bool LexicalAnalyzer::IsKeyword(string s)
{
    for (int i = 0; i < KEYWORDS_COUNT; i++) {
        if (s == keyword[i]) {
            return true;
        }
    }
    return false;
}

TokenType LexicalAnalyzer::FindKeywordIndex(string s)
{
    for (int i = 0; i < KEYWORDS_COUNT; i++) {
        if (s == keyword[i]) {
            return (TokenType) (i + 1);
        }
    }
    return ERROR;
}

Token LexicalAnalyzer::ScanNumber()
{
   char c;
	char temp,temp2,temp3;
	int flag_08 = 0;
	int flag_zero = 0;
	char arr[100];
   input.GetChar(c);
    if (isdigit(c)) 
    {
        if (c == '0')
        {
            tmp.lexeme = "0";
            flag_zero = 1;
        } else {
            tmp.lexeme = "";
            while (!input.EndOfInput() && isdigit(c)) {
                tmp.lexeme += c;
                input.GetChar(c);
				if(c == '8' || c == '9')
				{
					flag_08= 1;
				}
		    }
            
            if (!input.EndOfInput()) {
                input.UngetChar(c);
            }
            
        }
	    // TODO: You can check for REALNUM, BASE08NUM and BASE16NUM here!
		input.GetChar(temp);
		if (temp == 'x')
		{
		input.GetChar(temp2);
		   if(temp2 == '0') 
		    {
		    		//cout<<"inside 0 check";
		      input.GetChar(temp3);
		   		//cout<<"temp3 = "<<temp3;
		      if (temp3 == '8' && flag_08 == 0)
		         {
		   	//	   		cout<<"inside temp3 check ";
		        tmp.lexeme = tmp.lexeme+temp+temp2+temp3;
			    tmp.token_type = BASE08NUM;
                tmp.line_no = line_no;
                return tmp;
		        }
		     else
		        {
		   		   		//cout<<"unget temp3 = ";
		        input.UngetChar(temp3);
		         }
		   		   		   		//cout<<"unget temp2 = ";
		 input.UngetChar(temp2);
		   }
	   	else if(temp2 == '1')
		{
		     input.GetChar(temp3);
		    if (temp3 == '6')
		   {
		   tmp.lexeme = tmp.lexeme+temp+temp2+temp3;
			tmp.token_type = BASE16NUM;
            tmp.line_no = line_no;
            return tmp;
		   }
		   else
		   {
		   input.UngetChar(temp3);
		   }
		 input.UngetChar(temp2);
		}
		else
		{
		input.UngetChar(temp2);
		}
		
		input.UngetChar(temp);
		} 
		 
	    else if( temp == '.')
               {
                
                         
               
                  int flagnonzero =0;
                   string abc= "";
                   input.GetChar(temp2);
                   if(isdigit(temp2))
                   {
                     while (!input.EndOfInput() && isdigit(temp2))
                    {
                     if(temp2 != '0')
                     {
                     flagnonzero =1;
                     }   
                     abc += temp2;
                     input.GetChar(temp2);
                     
                    }
                  if (!input.EndOfInput()) 
                   {
                       input.UngetChar(temp2);
                   }
                  }
                  else
                  {
                  input.UngetChar(temp2);
                  }
                  if(flagnonzero == 0 && flag_zero == 1)
                  {
                  
                   input.UngetString(abc);
                  
                  } 
                  else
                  {
                   input.UngetString(abc); 
                   input.GetChar(temp2);
					if(isdigit(temp2))
					{
                    tmp.lexeme += '.';
                    while (!input.EndOfInput() && (isdigit(temp2)))
                    {
                      tmp.lexeme += temp2;
                      input.GetChar(temp2);
                    }
                     if (!input.EndOfInput()) {
                      input.UngetChar(temp2);
                    }
					   tmp.token_type = REALNUM;
					   tmp.line_no = line_no;
                       return tmp;
                    }
					else
					{
						input.UngetChar(temp2);
					}
                   }
                
				   
                   input.UngetChar(temp);
               }
	
	    else if(temp == 'A' || temp == 'B' || temp == 'C' || temp == 'D' || temp == 'E' || temp == 'F' ) //check for strings
		{
		    int count=0;
		    int tempcount = 0;
		    char arr[100];
			arr[count]= temp;
			while(isdigit(arr[count])||(arr[count] == 'A' || arr[count] == 'B' || arr[count] == 'C' || arr[count] == 'D' || arr[count] == 'E' || arr[count] == 'F' ))
            {
               count++;
               input.GetChar(arr[count]);      
            }
            tempcount = count;
            temp= arr[count];
		    
		   
            if(temp == 'x')
		  { 
		 input.GetChar(temp2);
			if(temp2 == '1')
			{   input.GetChar(temp3);
				if (temp3 == '6')
				{
				     for(count=0;count<tempcount;count++)
				     {
				        tmp.lexeme = tmp.lexeme + arr[count];
				     }
				    tmp.lexeme = tmp.lexeme+temp+temp2+temp3;
					tmp.token_type = BASE16NUM;
					tmp.line_no = line_no;
					return tmp;
					
				}
                else 
				{
				   input.UngetChar(temp3);
				   input.UngetChar(temp2);	
				}					
			}
			else
			{   
				input.UngetChar(temp2);
			}
			
			input.UngetChar(temp);
		}
		else
		{
		input.UngetChar(temp);
		}
		
		        count--;
		        while(count>-1)
		        {
		        input.UngetChar(arr[count]);
                count--;
		        }
	
		}
		
		else
		
		{
	      input.UngetChar(temp);
		}
	
	
        tmp.token_type = NUM;
        tmp.line_no = line_no;
        
        return tmp;
    } else {
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        tmp.lexeme = "";
        tmp.token_type = ERROR;
        tmp.line_no = line_no;
       
        return tmp;
    }
}

Token LexicalAnalyzer::ScanIdOrKeyword()
{
    char c;
    input.GetChar(c);

    if (isalpha(c)) {
    //cout<<"Scanidorkeyword";
        tmp.lexeme = "";
     //   cout<<"char = "<<c;
        while (!input.EndOfInput() && isalnum(c)) {
            tmp.lexeme += c;
            input.GetChar(c);
           // cout<<"char = "<<c;
        }
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        tmp.line_no = line_no;
        if (IsKeyword(tmp.lexeme))
            tmp.token_type = FindKeywordIndex(tmp.lexeme);
        else
            tmp.token_type = ID;
    } else {
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        tmp.lexeme = "";
        tmp.token_type = ERROR;
    }
    return tmp;
}

// you should unget tokens in the reverse order in which they
// are obtained. If you execute
//
//    t1 = lexer.GetToken();
//    t2 = lexer.GetToken();
//    t3 = lexer.GetToken();
//
// in this order, you should execute
//
//    lexer.UngetToken(t3);
//    lexer.UngetToken(t2);
//    lexer.UngetToken(t1);
//
// if you want to unget all three tokens. Note that it does not
// make sense to unget t1 without first ungetting t2 and t3
//
TokenType LexicalAnalyzer::UngetToken(Token tok)
{
    tokens.push_back(tok);;
    return tok.token_type;
}

Token LexicalAnalyzer::GetToken()
{
    char c;

    // if there are tokens that were previously
    // stored due to UngetToken(), pop a token and
    // return it without reading from input
    
    if (!tokens.empty()) {
        tmp = tokens.back();
        tokens.pop_back();
        return tmp;
    }

    SkipSpace();
    tmp.lexeme = "";
    tmp.line_no = line_no;
    input.GetChar(c);
   // cout<< "inside get token "<< c;
    
    switch (c) {
        case '.':
            tmp.token_type = DOT;
            return tmp;
        case '+':
            tmp.token_type = PLUS;
            return tmp;
        case '-':
            tmp.token_type = MINUS;
            return tmp;
        case '/':
            tmp.token_type = DIV;
            return tmp;
        case '*':
            tmp.token_type = MULT;
            return tmp;
        case '=':
            tmp.token_type = EQUAL;
            return tmp;
        case ':':
            tmp.token_type = COLON;
            return tmp;
        case ',':
            tmp.token_type = COMMA;
            return tmp;
        case ';':
            tmp.token_type = SEMICOLON;
            return tmp;
        case '[':
            tmp.token_type = LBRAC;
            return tmp;
        case ']':
            tmp.token_type = RBRAC;
            return tmp;
        case '(':
            tmp.token_type = LPAREN;
            return tmp;
        case ')':
            tmp.token_type = RPAREN;
            return tmp;
        case '<':
            input.GetChar(c);
            if (c == '=') {
                tmp.token_type = LTEQ;
            } else if (c == '>') {
                tmp.token_type = NOTEQUAL;
            } else {
                if (!input.EndOfInput()) {
                    input.UngetChar(c);
                }
                tmp.token_type = LESS;
            }
            return tmp;
        case '>':
            input.GetChar(c);
            if (c == '=') {
                tmp.token_type = GTEQ;
            } else {
                if (!input.EndOfInput()) {
                    input.UngetChar(c);
                }
                tmp.token_type = GREATER;
            }
            return tmp;
        default:
            if (isdigit(c)) {
                input.UngetChar(c);
                return ScanNumber();
            } else if (isalpha(c)) {
                input.UngetChar(c);
                return ScanIdOrKeyword();
            } else if (input.EndOfInput())
                tmp.token_type = END_OF_FILE;
            else
                tmp.token_type = ERROR;

            return tmp;
    }
}

int main()
{
    LexicalAnalyzer lexer;
    Token token;

    token = lexer.GetToken();
    token.Print();
    while (token.token_type != END_OF_FILE)
    {   //cout<<"inside type";
        token = lexer.GetToken();
        token.Print();
    }
}
