/*
 * Copyright (C) Rida Bazzi, 2017
 *
 * Do not share this file with anyone
 */
#ifndef _COMPILER_H_
#define _COMPILER_H_

#include <string>

struct ValueNode
{
    std::string name;
    int value;
};

struct GotoStatement
{
    struct StatementNode * target;
};

enum ArithmeticOperatorType {
    OPERATOR_NONE = 123,
    OPERATOR_PLUS,
    OPERATOR_MINUS,
    OPERATOR_MULT,
    OPERATOR_DIV
};

struct AssignmentStatement
{
    struct ValueNode * left_hand_side;

    struct ValueNode * operand1;
    struct ValueNode * operand2;

    /*
     * If op == OPERATOR_NONE then only operand1 is meaningful.
     * Otherwise both operands are meaningful
     */
    ArithmeticOperatorType op;
};

struct PrintStatement
{
    struct ValueNode * id;
};

enum ConditionalOperatorType {
    CONDITION_GREATER = 345,
    CONDITION_LESS,
    CONDITION_NOTEQUAL
};

struct IfStatement
{
    struct ValueNode * condition_operand1;
    struct ValueNode * condition_operand2;

    ConditionalOperatorType condition_op;

    struct StatementNode * true_branch;
    struct StatementNode * false_branch;
};

enum StatementType
{
    NOOP_STMT = 1000,
    PRINT_STMT,
    ASSIGN_STMT,
    IF_STMT,
    GOTO_STMT
};

struct StatementNode
{
    StatementType type;

    union
    {
        struct AssignmentStatement * assign_stmt;
        struct PrintStatement * print_stmt;
        struct IfStatement * if_stmt;
        struct GotoStatement * goto_stmt;
    };

    struct StatementNode * next; // next statement in the list or NULL
};

void debug(const char* format, ...);

//---------------------------------------------------------
// You should write the following function:

struct StatementNode * parse_generate_intermediate_representation();

/*
  NOTE:

  You need to write a function with the above signature. This function
  is supposed to parse the input program and generate an intermediate
  representation for it. The output of this function is passed to the
  execute_program function in main().

  Write your code in a separate file and include this header file in
  your code.
*/

#endif /* _COMPILER_H_ */
