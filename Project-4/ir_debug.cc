/*
 * Copyright (C) Mohsen Zohrevandi, 2017
 *
 * Do not share this file with anyone
 */
#include <iostream>
#include <string>
#include <sstream>
#include <cassert>
#include "ir_debug.h"

using namespace std;

static void print_statements(struct StatementNode* pc, struct StatementNode* last, int indent);

static void print_value_node(struct ValueNode* v)
{
    if (v != NULL)
    {
        if (v->name == "")
            cout << v->value;
        else
            cout << v->name;
        // Alternatively, you could print both:
        // cout << v->name << " (" << v->value << ")";

        // You could also print the address of the node as well:
        // cout << " @ " << v;
    }
    else
    {
        cout << "NULL";
    }
}

static void print_arithmetic_operator(ArithmeticOperatorType op)
{
    switch (op)
    {
        case OPERATOR_NONE:      break;
        case OPERATOR_PLUS:      cout << " + ";  break;
        case OPERATOR_MINUS:     cout << " - ";  break;
        case OPERATOR_MULT:      cout << " * ";  break;
        case OPERATOR_DIV:       cout << " / ";  break;
        default:                 cout << " ? ";  break;
    }
}

static void print_conditional_operator(ConditionalOperatorType op)
{
    switch (op)
    {
        case CONDITION_GREATER:  cout << " > ";  break;
        case CONDITION_LESS:     cout << " < ";  break;
        case CONDITION_NOTEQUAL: cout << " <> "; break;
        default:                 cout << " ? ";  break;
    }
}

static void print_line_prefix(struct StatementNode* st, int indent, bool right_brace)
{
    stringstream ss;
    ss << st << ": ";
    string p = ss.str();
    if (!right_brace)
    {
        cout << p;
    }
    else
    {
        cout << string(p.size() , ' ');
    }
    cout << string(indent * 4, ' ');
}

static void print_print(struct StatementNode* st, int indent)
{
    assert(st->print_stmt->id != NULL);

    print_line_prefix(st, indent, false);

    cout << "print ";
    print_value_node(st->print_stmt->id);
    cout << ";\n";
}

static void print_if(struct StatementNode* st, int indent)
{
    assert(st->if_stmt->condition_operand1 != NULL);
    assert(st->if_stmt->condition_operand2 != NULL);
    assert(st->if_stmt->true_branch != NULL);
    assert(st->if_stmt->false_branch != NULL);

    bool inverted = false;

    print_line_prefix(st, indent, false);

    if (st->if_stmt->true_branch->type == NOOP_STMT)
        inverted = true;

    cout << "if ( ";
    if (inverted)
        cout << "! ( ";
    print_value_node(st->if_stmt->condition_operand1);
    print_conditional_operator(st->if_stmt->condition_op);
    print_value_node(st->if_stmt->condition_operand2);
    if (inverted)
        cout << " ) ";
    cout << " ) {\n";

    if (inverted)       // NOTE: This is for SWITCH statements
        print_statements(st->if_stmt->false_branch, st->if_stmt->true_branch, indent + 1);
    else
        print_statements(st->if_stmt->true_branch, st->if_stmt->false_branch, indent + 1);

    print_line_prefix(st, indent, true);
    cout << "}\n";
}

static void print_assignment(struct StatementNode* st, int indent)
{
    assert(st->assign_stmt->left_hand_side != NULL);
    assert(st->assign_stmt->operand1 != NULL);
    if (st->assign_stmt->op == OPERATOR_NONE)
        assert(st->assign_stmt->operand2 == NULL);
    else
        assert(st->assign_stmt->operand2 != NULL);

    print_line_prefix(st, indent, false);

    print_value_node(st->assign_stmt->left_hand_side);
    cout << " = ";
    print_value_node(st->assign_stmt->operand1);
    print_arithmetic_operator(st->assign_stmt->op);
    if (st->assign_stmt->operand2 != NULL)
        print_value_node(st->assign_stmt->operand2);
    cout << ";\n";
}

static void print_goto(struct StatementNode* st, int indent)
{
    assert(st->goto_stmt->target != NULL);

    print_line_prefix(st, indent, false);

    cout << "goto " << st->goto_stmt->target << "\n";
}

static void print_noop(struct StatementNode* st, int indent)
{
    print_line_prefix(st, indent, false);

    cout << "noop;\n";
}

static void print_statements(struct StatementNode* pc, struct StatementNode* last, int indent)
{
    while (pc != last)
    {
        switch (pc->type)
        {
            case NOOP_STMT:
                print_noop(pc, indent);
                break;

            case GOTO_STMT:
                assert(pc->goto_stmt != NULL);
                print_goto(pc, indent);
                break;

            case ASSIGN_STMT:
                assert(pc->assign_stmt != NULL);
                print_assignment(pc, indent);
                break;

            case IF_STMT:
                assert(pc->if_stmt != NULL);
                print_if(pc, indent);
                break;

            case PRINT_STMT:
                assert(pc->print_stmt != NULL);
                print_print(pc, indent);
                break;

            default:
                print_line_prefix(pc, indent, false);
                cout << "Unknown Statement (type = " << pc->type << ")\n";
                break;
        }
        pc = pc->next;
    }
}

void print_program(struct StatementNode* program)
{
    assert(program != NULL);
    print_statements(program, NULL, 1);
}

