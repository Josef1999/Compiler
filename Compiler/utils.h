#pragma once
//#define USE_DEBUG_FILE

#ifndef USE_DEBUG_FILE
#define SOURCE_CODE "../输入文件/源代码.txt"
#define LEX_RESULT "../分析结果/词法分析结果.txt"
#define GRAMMAR_INPUT "../输入文件/表达式.txt"
#else
#define SOURCE_CODE "../测试用文件/源代码.txt"
#define LEX_RESULT "../测试用文件/词法分析结果_debug.txt"
#define GRAMMAR_INPUT "../测试用文件/表达式_debug.txt"
#endif

#define PARSER_PROCESS "../分析结果/语法分析过程.txt"
#define PARSER_TREE "../分析结果/语法树.txt"
