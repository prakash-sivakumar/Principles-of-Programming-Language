/*
 * Copyright (C) Mohsen Zohrevandi, 2017
 *
 * Do not share this file with anyone
 */
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <unordered_map>
#include <string>
#include <map>
#include <unordered_set>
#include <algorithm>
#include <set>
#include <string.h>




#include "lexer.h"
using namespace std;

vector<pair<string, vector<string> > > grammar_rules; 
vector <string> nonterminals_tmp;
vector <string> terminals;
vector <string> nonterminals;
vector <string> both;


int main (int argc, char* argv[])
{
    int task;

    if (argc < 2)
    {
        cout << "Error: missing argument\n";
        return 1;
    }
    task = atoi(argv[1]);

    LexicalAnalyzer lexer;
    Token token;
    token = lexer.GetToken();
    int epsilon_count=0;
    while(token.token_type!= DOUBLEHASH)
    {     
		string lhs;
		lhs = token.lexeme;
		token= lexer.GetToken();
		if(token.token_type == ARROW)
		{
		
		   vector <string> rhs;
		   token= lexer.GetToken();
		   if(token.token_type == HASH)
			   {
			   rhs.push_back("#");
			   epsilon_count=1;
			   }
		   else
			   {
			   while(token.token_type != HASH )
				   {
				   rhs.push_back(token.lexeme);
				   token= lexer.GetToken();
				   }
			   }
		 grammar_rules.push_back( std::make_pair( lhs, rhs ) );  
		}
    }
    for(int i=0;i<grammar_rules.size();i++)
		{
				  nonterminals_tmp.push_back(grammar_rules[i].first);           
		}
    
 
    
    for(int i=0;i<grammar_rules.size();i++)
    {
	   if (find (nonterminals_tmp.begin(), nonterminals_tmp.end(), grammar_rules[i].first) != nonterminals_tmp.end()) 
	   {
		  
				 if (find (nonterminals.begin(), nonterminals.end(), grammar_rules[i].first) == nonterminals.end())
				 {
					nonterminals.push_back(grammar_rules[i].first);
				 }
		}
    
      for(int j=0;j<grammar_rules[i].second.size();j++)
      {
		  if (find (nonterminals_tmp.begin(), nonterminals_tmp.end(), grammar_rules[i].second[j]) != nonterminals_tmp.end()) 
		   {
		  
				 if (find (nonterminals.begin(), nonterminals.end(), grammar_rules[i].second[j]) == nonterminals.end())
				 {
					nonterminals.push_back(grammar_rules[i].second[j]);
				 }
		   }
		 else
		 {      
				if (find (terminals.begin(), terminals.end(), grammar_rules[i].second[j]) == terminals.end() && grammar_rules[i].second[j] != "#")
				 {
					terminals.push_back(grammar_rules[i].second[j]);
				 }
		 
		 }
      }                 
    }
    
     for (int i = 0; i < terminals.size(); i++)
	    {
                both.push_back(terminals[i]);
        }
    for (int i = 0; i < nonterminals.size(); i++) 
	    {
                both.push_back(nonterminals[i]);
        }
      
      //   *********FIRST SET ********* 
    
    int tlimit= terminals.size()+2;
    int ntlimit = nonterminals.size();
    int add=tlimit+ntlimit;
    string univ_array[add];
    univ_array[0]="#";
    univ_array[1]="$";
    int z =0;
    for(int i=2;i<tlimit;i++)
    {
        univ_array[i]=terminals[z];
        z++;
    } 
    z=0;
    for(int i=tlimit;i<add;i++)
    {
        univ_array[i]=nonterminals[z];
        z++;
    } 
    
    int size_univ = sizeof(univ_array)/sizeof(*univ_array); 
    int bool_array[size_univ][tlimit];
    cout<<endl;
	
    for(int i=0;i<size_univ;i++)
    {
        for(int j=0;j<tlimit;j++)
     {
         bool_array[i][j]=0;
     }
    }
   int x =2;
   for(int i=0;i<terminals.size();i++) 
	   {
			bool_array[x][x]=1;
			x++;
	   }
   if(epsilon_count==1)
	   {
	   bool_array[0][0]=1;
	   }
 
   vector<pair<int, vector<int> > > int_rules; 
   for(int i=0;i<grammar_rules.size();i++)
       {
           int de; 
           vector <int> arrays;
           for(int k=0;k<size_univ;k++)
           { string abc;
             abc = grammar_rules[i].first;
			   if(abc==univ_array[k])
			   {
				  de = k;
			
			   }
           }
    
		   for(int j=0;j<grammar_rules[i].second.size();j++)
			  {
				   for(int k=0;k<size_univ;k++)
				   { string abc;
					 abc = grammar_rules[i].second[j];
					   if(abc==univ_array[k])
					   {
						  arrays.push_back(k);
					   }
				   }
			  }
        int_rules.push_back( std::make_pair( de,arrays  ) );        
        }

	  bool change2= true;
	  int arrays[grammar_rules.size()];
			for(int i=0;i<grammar_rules.size();i++)
			{
			 arrays[i]=0;
		    }
			
 while(change2)
   {          //cout<<"hi"<<endl;
              change2=false;
              for(int i = 0; i < int_rules.size(); i++)
                {  
                     bool epsilon_change=true;
                     int count=0;
                      for(int j = 0; j < int_rules[i].second.size(); j++)
                       {
                        if(epsilon_change)
                        {   epsilon_change=false;
							if (find (terminals.begin(), terminals.end(), grammar_rules[i].second[j]) != terminals.end()) 
								{  if(bool_array[int_rules[i].first][int_rules[i].second[j]]==0)
                                   {
								   bool_array[int_rules[i].first][int_rules[i].second[j]]=1;
								   change2=true;
								   }//arrays[i]=1;
								   
								}
							if(grammar_rules[i].second[j]== "#")
								{  
								   //change2=true;
								   //epsilon_change=true;
								   count++;
								   if(count==int_rules[i].second.size()&& bool_array[int_rules[i].first][0]==0)
									 {
									 change2=true;
									 }
								  
								} 
							 if (find (nonterminals.begin(), nonterminals.end(), grammar_rules[i].second[j]) != nonterminals.end()) 
							{    // cout<<"bool array"<<bool_array[int_rules[i].second[j]][0];
								  if(bool_array[int_rules[i].second[j]][0]==1)
								  {
									 epsilon_change=true;
									 count++;
									 if(count==int_rules[i].second.size()&& bool_array[int_rules[i].first][0]==0)
									 {
									 change2=true;
									 }
									// cout<<"Count is"<<count<<endl;
									 
								  }
								  for(int z=1;z<tlimit;z++)
								  {
										   if(bool_array[int_rules[i].second[j]][z]== 1 && bool_array[int_rules[i].first][z]==0  )
										   {
											 bool_array[int_rules[i].first][z]= 1;
											 change2=true;    
										   }
										   
								   }
							}
                        }  
                       // cout<<"inside loop"<<change2<<endl;
                        
                       }  
                       if(count==int_rules[i].second.size())
                       {
                       bool_array[int_rules[i].first][0]=1;
                       
                       //cout<<int_rules[i].first<<endl;
                       
                       } 
                
                       }
                 // cout<<"hello"<<change2<<endl;
				 
   } 
               
                                    //***********FOLLOW SETS********
   
   
    int bool_array_follow[size_univ][tlimit];
    for(int i=0;i<size_univ;i++)
    {
     for(int j=0;j<tlimit;j++)
		 {
			 bool_array_follow[i][j]=0; 
		 }
    }
   
   
   bool_array_follow[int_rules[0].first][1]=1;
   bool change3=true;
   
   while(change3)
   {
        change3=false;
        for(int i=0;i<int_rules.size();i++)
        { bool epsilon_follow=true;
          int episi=0;
          for(int j=int_rules[i].second.size()-1; j>=0; j--)
          {
                if (find (nonterminals.begin(), nonterminals.end(), grammar_rules[i].second[j]) != nonterminals.end() ) 
                  {
                    if(episi == grammar_rules[i].second.size()-1-j)
                    {
                        for(int z=1;z<tlimit;z++)
                       {
                          if(bool_array_follow[int_rules[i].first][z]== 1 && bool_array_follow[int_rules[i].second[j]][z]==0  )
                            {   
                                  bool_array_follow[int_rules[i].second[j]][z]= 1;
                                  change3=true;    
                            }
                       }
                   }   
                 }
                 if(bool_array[int_rules[i].second[j]][0])
                   { episi++; }
         }
         
         for(int j=0; j<int_rules[i].second.size(); j++)
          {   
               int next= 1;
               for(int b=j+1; b<int_rules[i].second.size(); b++)
                  {   
                             
                       if (next) 
                            {
                                for(int z=2;z<tlimit;z++)
                                {
                                   
                                       if(bool_array[int_rules[i].second[b]][z]== 1 && bool_array_follow[int_rules[i].second[j]][z]==0 )
                                        {
                                              bool_array_follow[int_rules[i].second[j]][z]= 1;
                                              change3=true;    
                                        }
                                } 
                              
                            }
                             if(bool_array[int_rules[i].second[b]][0]==1)
                                  {  next=1; }
                               else
                                    {
                                    next=0;
                                    break;
                                    }
                  }
          
          }
         
       }
   }
   
   
   //**************************** USEFUL SYMBOLS ***********
   
    vector <string> generate;
      vector <int> useful;
      vector<pair<string, vector<string>>> reachable_result;
      for(int i=0;i<terminals.size();i++)
         {
          generate.push_back(terminals[i]);
         }
      bool change=true;
      bool generating;
      int arr[grammar_rules.size()];
      for(int i=0;i<grammar_rules.size();i++)
      {
        arr[i]=0;
      }
      while(change)
      {
                change = false;
				int count=0;
                for (int i = 0; i < grammar_rules.size(); i++) 
                {   count =0;
                    for (int j = 0; j < grammar_rules[i].second.size(); j++) 
                    {   if(arr[i]==0)
                        {
                        if (grammar_rules[i].second[j]== "#" || (find(generate.begin(), generate.end(), grammar_rules[i].second[j]) != generate.end())) 
                        {
                            generating = true;
                            count++;
                        } 
                        else
                        {
                            generating = false;
                            break;
                        }
                        }
                    }
                    
                    if (generating && count == grammar_rules[i].second.size() ) {
                        generate.push_back(grammar_rules[i].first);
                        arr[i]=1;
                        change=true;
                    }
                }
            }
       
         vector<pair<string, vector<string>>> generate_result;
           for(int i = 0; i < grammar_rules.size(); i++)
            {
                if(arr[i]==1)
                {
                generate_result.push_back(grammar_rules[i]);
                }
            }    
     
		vector <string> reachable;
		//vector<pair<string, vector<string>>> reachable_result;
		vector <int> useful_rea;
		int flag=0;
		int flagi=0;
		//x=0;
		string gram = grammar_rules[0].first;
	   
		for(int i = 0; i < generate_result.size(); i++)
			{
				string gene = generate_result[i].first ;
				if(gene.compare(gram)==0)
					{
					flag=1;
					flagi=i;
					
					}   
			 }
	   
		if(flag==1)
		 {
			int array[generate_result.size()];
			for(int i=0;i<generate_result.size();i++)
			{
			 array[i]=0;
			}
			
				
				reachable.push_back(generate_result[flagi].first);
				for (int i = 0; i < generate_result[flagi].second.size(); ++i) 
				{
					reachable.push_back(generate_result[flagi].second[i]);
					useful_rea.push_back(i);
				}
				array[flagi]=1;
				bool change1 = true;

				while (change1)
				{
					long len_rea;
					len_rea = reachable.size();
					change1=false;
					for (int i = 0; i < generate_result.size(); i++)
					{
					   
						if(array[i]==0)
						{
							if (find(reachable.begin(), reachable.end(), generate_result[i].first) != reachable.end()) 
							{
								for (int j = 0; j < generate_result[i].second.size(); ++j) 
								{
									reachable.push_back(generate_result[i].second[j]);
									useful_rea.push_back(i);
									change1=true;
									array[i]=1;
								}
							}
						}
					}
				 }
				
				
				for(int i = 0; i < generate_result.size(); i++) 
				{
					if(array[i]==1)
					{
						reachable_result.push_back(generate_result[i]);
					}
				} 
				flag=1;
				
				if(flag==1 && task==2)
			{
			     
					 for(int i = 0; i < reachable_result.size(); i++)
					{   
						cout << reachable_result[i].first << "-> " ;
						for(int j = 0; j < reachable_result[i].second.size(); j++)
						{    
							cout << reachable_result[i].second[j] << " ";
						} 
						cout << endl;
					}  
			}
				
		}
		
		//cout<<reachable_result.size();
		if(task==5)
		{
		    if(grammar_rules.size()!=reachable_result.size())
			{  //cout<<grammar_rules.size()<<" "<<reachable_result.size();
			   cout<<"NO";
			}
			else
			{  int temp;
			   int temp_count=0;
			   int temp_count2=0;
			   int conditions_passed=0;
			   for(int i=tlimit;i<size_univ;i++)
                {
                    if(bool_array[i][0]==1)
                   {    // cout<<"array name";
                         temp_count++;
                         for(int j=0;j<tlimit;j++)
                         {
                                if((bool_array[i][j]== 0 && bool_array_follow[i][j]==1)||(bool_array[i][j]== 1 && bool_array_follow[i][j]==0)||bool_array[i][j]== 0 && bool_array_follow[i][j]==0)
                                {
                                    temp=0;
                                }
                                else
                                {
                                    temp=1;
                                    break;
                                }      
                         }
                         if(temp==0)
                         {
                            temp_count2++;
                         }
                   }
                   temp=1;
                }
             
                if(temp_count2==temp_count)
                {
                conditions_passed=1;
               
                vector<pair<string,int>> unique_count;
                vector<string> unique_terminals;
                for(int i=0;i<grammar_rules.size();i++)
                {       int y=0;
                        if(find(unique_terminals.begin(),unique_terminals.end(),grammar_rules[i].first)==unique_terminals.end())
                        {       int count=0;
                                string lhs=grammar_rules[i].first;
                                unique_terminals.push_back(lhs);
                                for(int k=0;k<grammar_rules.size();k++)
                                {
                                  if(lhs==grammar_rules[k].first)
                                  {
                                  count++;
                                  }   
                                } 
                                unique_count.push_back( std::make_pair( lhs, count));
                                    
                        }
                }
                
            
                bool change=true;
                for(int i=0;i<unique_count.size();i++)
                {    int tempcount=0;
                     
                     if(unique_count[i].second>1 && change)
                     
                     {   change=false;
                         int predictive[unique_count[i].second][tlimit];
                          for(int a=0;a<unique_count[i].second;a++)
                          { 
                           for(int b=0;b<tlimit;b++)
                            { 
                               predictive[a][b]=0;
                            }  
                          }
                          
                          
                          for(int j=0;j<grammar_rules.size();j++)
                          {    
                              if(grammar_rules[j].first==unique_count[i].first)
                              {    bool epsilon_chan=true;
                                   int c=0;
                                   for(int k=0;k<grammar_rules[j].second.size();k++)
                                   {    
                                        if(epsilon_chan)
                                        {
                                           epsilon_chan=false;
                                         if (find (nonterminals.begin(), nonterminals.end(), grammar_rules[j].second[k]) != nonterminals.end())
                                         {
                                              for(int z=2;z<tlimit;z++)
								              {
										           if(bool_array[int_rules[j].second[k]][z]== 1 && predictive[tempcount][z]==0  )
										           {
											         predictive[tempcount][z]= 1;
											       }
										      }
										      if(bool_array[int_rules[j].second[k]][0]==1)
										      {
										          epsilon_chan=true;
										          c++;
										      } 
                                         }
                                         else if(find (terminals.begin(), terminals.end(), grammar_rules[j].second[k]) != terminals.end())
                                          {
                                              for(int z=2;z<tlimit;z++)
								              {
										           if(bool_array[int_rules[j].second[k]][z]== 1 && predictive[tempcount][z]==0  )
										           {
											         predictive[tempcount][z]= 1;
											       }
										      }
										      
                                          }
                                          else if(grammar_rules[j].second[k]== "#")
                                          {
                                                   predictive[tempcount][0]=1;
                                                   c++;
                                          }
                                        }
                                   }
                                   if(c==grammar_rules[j].second.size())
                                   {
                                   predictive[tempcount][0]=1;
                                   }
                                  // unique_count[i].second--;
                                   tempcount++;
                              }
                          }
                          
                     
                          int abc=0;
                          for(int m=0;m<unique_count[i].second;m++)
                          {
                                for(int n=0;n<unique_count[i].second;n++)
                                {
                                    for(int p=0;p<tlimit;p++)
                                    {
                                      if(m!=n)
                                      {
                                         if((predictive[m][p]==predictive[n][p])&& (predictive[m][p]!=0))
                                         {
                                             abc=1;
                                             break;
                                         }
                                      }
                                    }
                                    if(abc==1)
                                    {
                                    break;
                                    }
                                         
                                }
                               if(abc==1)
                               {
                               break;
                               }
                          }
                          
                          if(abc==1)
                          {
                             change=false;
                          }
                          else
                          {
                             change=true;
                          }
                          
                     }
                     
                     if(change==false)
                     {
                        cout<<"NO";
                        break;
                     }
                     
                }
                   if(change==true)
                    {
                       cout<<"YES";
                    }
                
                }
                else
                {
                cout<<"NO";
                }
			}
            
		}		
	    
   
  
    switch (task) {
        case 1:
            for (int i = 0; i < both.size(); i++)
			{
                cout << both[i] << " ";
            }
            break;

        case 2:
            break;

        case 3:
            for(int i=tlimit;i<size_univ;i++)
            {
                cout<<"FIRST("<<univ_array[i]<<") = {";
                int x=0;
                cout<<" ";
                for(int j=0;j<tlimit;j++)
                { 
        
					 if(bool_array[i][j]==1)
					 {
						if(x==0)
							{
							cout<< univ_array[j];
							x=1;
							} 
						else
							{
							cout<<", " << univ_array[j];
							}
					 }
                }
                cout<<" ";
                cout<<"}";
                cout<<endl;
            } 
			break;

        case 4:
             for(int i=tlimit;i<size_univ;i++)
            {
                cout<<"FOLLOW("<<univ_array[i]<<") = {";
                int x=0;
                cout<<" ";
                for(int j=1;j<tlimit;j++)
                { 
        
					 if(bool_array_follow[i][j]==1)
					 {
						if(x==0)
							{
							cout<< univ_array[j];
							x=1;
							} 
						else
							{
							cout<<", " << univ_array[j];
							}
					 }
                }
                cout<<" ";
                cout<<"}";
                cout<<endl;
            } 
   
            break;

        case 5:
            break;

        default:
            cout << "Error: unrecognized task number " << task << "\n";
            break;
    }
    return 0;
}

