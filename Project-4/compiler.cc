/*
 * Copyright (C) Rida Bazzi, 2017
 *
 * Do not share this file with anyone
 */
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <cctype>
#include <cstring>
#include "compiler.h"
#include "ir_debug.h"

#define DEBUG 1     // 1 => Turn ON debugging, 0 => Turn OFF debugging

void debug(const char* format, ...)
{
    va_list args;
    if (DEBUG)
    {
        va_start (args, format);
        vfprintf (stdout, format, args);
        va_end (args);
    }
}

void execute_program(struct StatementNode * program)
{
    struct StatementNode * pc = program;
    int op1, op2, result;

    while (pc != NULL)
    {
        switch (pc->type)
        {
            case NOOP_STMT:
                pc = pc->next;
                break;

            case PRINT_STMT:
                if (pc->print_stmt == NULL)
                {
                    debug("Error: pc points to a print statement but pc->print_stmt is null.\n");
                    exit(1);
                }
                if (pc->print_stmt->id == NULL)
                {
                    debug("Error: print_stmt->id is null.\n");
                    exit(1);
                }
                printf("%d\n", pc->print_stmt->id->value);
                pc = pc->next;
                break;

            case ASSIGN_STMT:
                if (pc->assign_stmt == NULL)
                {
                    debug("Error: pc points to an assignment statement but pc->assign_stmt is null.\n");
                    exit(1);
                }
                if (pc->assign_stmt->operand1 == NULL)
                {
                    debug("Error: assign_stmt->operand1 is null.\n");
                    exit(1);
                }
                if (pc->assign_stmt->op != OPERATOR_NONE)
                {
                    if (pc->assign_stmt->operand2 == NULL)
                    {
                        debug("Error: assign_stmt->op requires two operands but assign_stmt->operand2 is null.\n");
                        exit(1);
                    }
                }
                if (pc->assign_stmt->left_hand_side == NULL)
                {
                    debug("Error: assign_stmt->left_hand_side is null.\n");
                    exit(1);
                }
                switch (pc->assign_stmt->op)
                {
                    case OPERATOR_PLUS:
                        op1 = pc->assign_stmt->operand1->value;
                        op2 = pc->assign_stmt->operand2->value;
                        result = op1 + op2;
                        break;
                    case OPERATOR_MINUS:
                        op1 = pc->assign_stmt->operand1->value;
                        op2 = pc->assign_stmt->operand2->value;
                        result = op1 - op2;
                        break;
                    case OPERATOR_MULT:
                        op1 = pc->assign_stmt->operand1->value;
                        op2 = pc->assign_stmt->operand2->value;
                        result = op1 * op2;
                        break;
                    case OPERATOR_DIV:
                        op1 = pc->assign_stmt->operand1->value;
                        op2 = pc->assign_stmt->operand2->value;
                        result = op1 / op2;
                        break;
                    case OPERATOR_NONE:
                        op1 = pc->assign_stmt->operand1->value;
                        result = op1;
                        break;
                    default:
                        debug("Error: invalid value for assign_stmt->op (%d).\n", pc->assign_stmt->op);
                        exit(1);
                        break;
                }
                pc->assign_stmt->left_hand_side->value = result;
                pc = pc->next;
                break;

            case IF_STMT:
                if (pc->if_stmt == NULL)
                {
                    debug("Error: pc points to an if statement but pc->if_stmt is null.\n");
                    exit(1);
                }
                if (pc->if_stmt->true_branch == NULL)
                {
                    debug("Error: if_stmt->true_branch is null.\n");
                    exit(1);
                }
                if (pc->if_stmt->false_branch == NULL)
                {
                    debug("Error: if_stmt->false_branch is null.\n");
                    exit(1);
                }
                if (pc->if_stmt->condition_operand1 == NULL)
                {
                    debug("Error: if_stmt->condition_operand1 is null.\n");
                    exit(1);
                }
                if (pc->if_stmt->condition_operand2 == NULL)
                {
                    debug("Error: if_stmt->condition_operand2 is null.\n");
                    exit(1);
                }
                op1 = pc->if_stmt->condition_operand1->value;
                op2 = pc->if_stmt->condition_operand2->value;
                switch (pc->if_stmt->condition_op)
                {
                    case CONDITION_GREATER:
                        if (op1 > op2)
                            pc = pc->if_stmt->true_branch;
                        else
                            pc = pc->if_stmt->false_branch;
                        break;
                    case CONDITION_LESS:
                        if (op1 < op2)
                            pc = pc->if_stmt->true_branch;
                        else
                            pc = pc->if_stmt->false_branch;
                        break;
                    case CONDITION_NOTEQUAL:
                        if (op1 != op2)
                            pc = pc->if_stmt->true_branch;
                        else
                            pc = pc->if_stmt->false_branch;
                        break;
                    default:
                        debug("Error: invalid value for if_stmt->condition_op (%d).\n", pc->if_stmt->condition_op);
                        exit(1);
                        break;
                }
                break;

            case GOTO_STMT:
                if (pc->goto_stmt == NULL)
                {
                    debug("Error: pc points to a goto statement but pc->goto_stmt is null.\n");
                    exit(1);
                }
                if (pc->goto_stmt->target == NULL)
                {
                    debug("Error: goto_stmt->target is null.\n");
                    exit(1);
                }
                pc = pc->goto_stmt->target;
                break;

            default:
                debug("Error: invalid value for pc->type (%d).\n", pc->type);
                exit(1);
                break;
        }
    }
}

int main()
{
    struct StatementNode * program;
    program = parse_generate_intermediate_representation();
    if (DEBUG)
    {
        printf("\n-------------------------------------------\n");
        print_program(program);
        printf("\n-------------------------------------------\n");
    }
    execute_program(program);
    return 0;
}
