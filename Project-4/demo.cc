#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include "compiler.h"

struct StatementNode * parse_generate_intermediate_representation()
{
    // Sample program for demonstration purpose only
    // Replace the following with a parser that reads the program from stdin &
    // creates appropriate data structures to be executed by execute_program()
    // This is the imaginary input for the following construction:
    // a, b;
    // {
    //    a = 10;
    //    b = 1;
    //    WHILE a > 0
    //    {
    //        b = b * a;
    //        a = a - 1;
    //    }
    //    print b;
    // }

    struct ValueNode * A = new ValueNode;
    struct ValueNode * B = new ValueNode;
    struct ValueNode * ONE = new ValueNode;
    struct ValueNode * TEN = new ValueNode;
    struct ValueNode * ZERO = new ValueNode;
    struct StatementNode * s1 = new StatementNode;
    struct StatementNode * s2 = new StatementNode;
    struct StatementNode * s3 = new StatementNode;
    struct StatementNode * s4 = new StatementNode;
    struct StatementNode * s5 = new StatementNode;
    struct StatementNode * s6 = new StatementNode;
    struct StatementNode * s7 = new StatementNode;
    struct StatementNode * s8 = new StatementNode;

    A->name = "a";      A->value = 0;
    B->name = "b";      B->value = 0;
    ONE->name = "";     ONE->value = 1;
    TEN->name = "";     TEN->value = 10;
    ZERO->name = "";    ZERO->value = 0;

    s1->type = ASSIGN_STMT;
    s1->assign_stmt = new AssignmentStatement;
    s1->assign_stmt->left_hand_side = A;
    s1->assign_stmt->op = OPERATOR_NONE;
    s1->assign_stmt->operand1 = TEN;
    s1->assign_stmt->operand2 = NULL;
    s1->next = s2;

    s2->type = ASSIGN_STMT;
    s2->assign_stmt = new AssignmentStatement;
    s2->assign_stmt->left_hand_side = B;
    s2->assign_stmt->op = OPERATOR_NONE;
    s2->assign_stmt->operand1 = ONE;
    s2->assign_stmt->operand2 = NULL;
    s2->next = s3;

    s3->type = IF_STMT;
    s3->if_stmt = new IfStatement;
    s3->if_stmt->condition_op = CONDITION_GREATER;
    s3->if_stmt->condition_operand1 = A;
    s3->if_stmt->condition_operand2 = ZERO;
    s3->if_stmt->true_branch = s4;
    s3->if_stmt->false_branch = s7;
    s3->next = s7;

    s4->type = ASSIGN_STMT;
    s4->assign_stmt = new AssignmentStatement;
    s4->assign_stmt->left_hand_side = B;
    s4->assign_stmt->op = OPERATOR_MULT;
    s4->assign_stmt->operand1 = B;
    s4->assign_stmt->operand2 = A;
    s4->next = s5;

    s5->type = ASSIGN_STMT;
    s5->assign_stmt = new AssignmentStatement;
    s5->assign_stmt->left_hand_side = A;
    s5->assign_stmt->op = OPERATOR_MINUS;
    s5->assign_stmt->operand1 = A;
    s5->assign_stmt->operand2 = ONE;
    s5->next = s6;

    s6->type = GOTO_STMT;
    s6->goto_stmt = new GotoStatement;
    s6->goto_stmt->target = s3;    // Jump to the if statement
    s6->next = s7;

    s7->type = NOOP_STMT;
    s7->next = s8;

    s8->type = PRINT_STMT;
    s8->print_stmt = new PrintStatement;
    s8->print_stmt->id = B;
    s8->next = NULL;

    return s1;
}
