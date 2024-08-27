#include <string.h>
#include <stdio.h>
#include <tree_sitter/api.h>
#include <assert.h>

TSLanguage *tree_sitter_vhdl();

int main() {

  TSParser *parser = ts_parser_new();
  ts_parser_set_language(parser, tree_sitter_vhdl());


  char *source = "architecture STRUCTURE of X_GATE is\n"
"    -- global declarations of signal:\n"
"    signal A, B : BIT;\n"
"begin\n"
"    LEVEL1 : block\n"
"        -- local declaration of generic parameters\n"
"        generic (GB1, GB2 : Time);\n"
"            -- local binding of generic parameters\n"
"        generic map (GB1 => LongTime, GB2 => ShortTime);\n"
"            -- local declaration of ports\n"
"        port (PB1: in BIT; PB2 : inout BIT );\n"
"            -- local binding of ports and signals\n"
"        port map (PB1 => P1, PB2 => B);\n"
"        -- local declarations:\n"
"        constant Delay : Time := 1 ms;\n"
"        signal S1 : BIT;\n"
"    begin\n"
"        S1 <= PB1 after Delay;\n"
"        PB2 <= S1 after GB1, P1 after GB2;\n"
"    end block LEVEL1;\n"
"end architecture STRUCTURE;\n";


  TSTree *tree = ts_parser_parse_string(parser, NULL, source, strlen(source));

  TSNode root = ts_tree_root_node(tree);
  TSTreeCursor cursor = ts_tree_cursor_new (root);
  TSNode node = ts_tree_cursor_current_node(&cursor);
  // -> cursor at design_file 0-722
  assert(ts_tree_cursor_goto_first_child (&cursor) == true);
  // -> cursor at design_unit 0-722
  assert(ts_tree_cursor_goto_first_child (&cursor) == true);
  // -> cursor at architecture_body 0-722
  assert(ts_tree_cursor_goto_first_child (&cursor) == true);
  // -> cursor at architecture 0-12
  assert(ts_tree_cursor_goto_next_sibling (&cursor) == true);
  // -> cursor at identifier 13-22
  assert(ts_tree_cursor_goto_next_sibling (&cursor) == true);
  // -> cursor at of 23-25
  assert(ts_tree_cursor_goto_next_sibling (&cursor) == true);
  // -> cursor at simple_name 26-32
  assert(ts_tree_cursor_goto_next_sibling (&cursor) == true);
  // -> cursor at is 33-35
  assert(ts_tree_cursor_goto_next_sibling (&cursor) == true);
  // -> cursor at comment 40-73
  assert(ts_tree_cursor_goto_next_sibling (&cursor) == true);
  // -> cursor at declarative_part 78-96
  assert(ts_tree_cursor_goto_next_sibling (&cursor) == true);
  // -> cursor at begin 97-102
  assert(ts_tree_cursor_goto_next_sibling (&cursor) == true);
  // -> cursor at concurrent_statement_part 107-694
  assert(ts_tree_cursor_goto_first_child (&cursor) == true);
  // -> cursor at block_statement 107-694
  assert(ts_tree_cursor_goto_first_child (&cursor) == true);
  // -> cursor at label 107-115
  assert(ts_tree_cursor_goto_next_sibling (&cursor) == true);
  // -> cursor at block 116-121
  assert(ts_tree_cursor_goto_next_sibling (&cursor) == true);
  // -> cursor at comment 130-172
  assert(ts_tree_cursor_goto_next_sibling (&cursor) == true);
  // -> cursor at block_header 181-493

  // If I remove these two lines, I get the expected result.
  node = ts_tree_cursor_current_node(&cursor);
  cursor = ts_tree_cursor_new (node);

  assert(ts_tree_cursor_goto_first_child (&cursor) == true);
  assert(0 == strcmp("generic_clause", ts_node_type(ts_tree_cursor_current_node (&cursor))));
  // -> cursor at generic_clause 181-207
  assert(ts_tree_cursor_goto_next_sibling (&cursor) == true);
  // Expecting: -> cursor at comment 220-258
  // Actually get: -> cursor at block_header 220-258
  // block_header is actually the parent of generic_clause!

  // Let's see what we get:
  node = ts_tree_cursor_current_node(&cursor);
  printf ("node=%s %d-%d\n", ts_node_type (node), ts_node_start_byte (node), ts_node_end_byte (node));
  return 0;
}
