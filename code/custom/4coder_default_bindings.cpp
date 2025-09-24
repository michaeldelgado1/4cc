/*
4coder_default_bidings.cpp - Supplies the default bindings used for default 4coder behavior.
*/

#if !defined(FCODER_DEFAULT_BINDINGS_CPP)
#define FCODER_DEFAULT_BINDINGS_CPP

#include "4coder_default_include.cpp"

// NOTE(allen): Users can declare their own managed IDs here.

#if !defined(META_PASS)
#include "generated/managed_id_metadata.cpp"
#endif

// NOTE(mdelgado): Vim modes
String_ID mapid_shared;
String_ID mapid_normal;
String_ID mapid_insert;
String_ID mapid_visual;
String_ID mapid_visual_line;

// NOTE(mdelgado): Key combo maps
String_ID mapid_delete;
String_ID mapid_delete_inner;

void
set_current_mapid(Application_Links *app, Command_Map_ID mapid) {
  View_ID view = get_active_view(app, 0);
  Buffer_ID buffer = view_get_buffer(app, view, 0);
  Managed_Scope scope = buffer_get_managed_scope(app, buffer);
  Command_Map_ID *map_id_ptr = scope_attachment(app, scope, buffer_map_id, Command_Map_ID);
  *map_id_ptr = mapid;
}

Range_i64
view_get_highlight_range(Application_Links *app, View_ID view) {
  Range_i64 result = {};
  i64 pos = view_get_cursor_pos(app, view);
  i64 mark = view_get_mark_pos(app, view);
  result.start = Min(pos, mark);
  result.end = Max(pos, mark);
  return result;
}

String_Const_u8
get_chars_in_range(Application_Links *app, Buffer_ID buffer, Range_i64 range) {
  Scratch_Block scratch(app);
  String_Const_u8 result = push_buffer_range(app, scratch, buffer, range);
  return result;
}

String_Const_u8
get_selected_chars(Application_Links *app) {
  View_ID view = get_active_view(app, 0);
  Buffer_ID buffer = view_get_buffer(app, view, 0);

  Range_i64 range = view_get_highlight_range(app, view);
  return get_chars_in_range(app, buffer, range);
}

// NOTE(mdelgado): Switch Modes
CUSTOM_COMMAND_SIG(enter_normal_mode) {
  set_current_mapid(app, mapid_normal);
  // NOTE(mdelgado): This changes the cursor to the rectangluar shape
  set_mode_to_original(app);
  enable_snap_mark_to_cursor(app);

  active_color_table.arrays[ defcolor_cursor ].vals[ 0 ] = 0xff7aa2f7;
  active_color_table.arrays[ defcolor_at_cursor ].vals[ 0 ] = 0xff4b7be3;
  active_color_table.arrays[ defcolor_margin_active ].vals[ 0 ] = 0xff7aa2f7;
}

CUSTOM_COMMAND_SIG(enter_insert_mode) {
  set_current_mapid(app, mapid_insert);
  set_mode_to_notepad_like(app);

  enable_snap_mark_to_cursor(app);

  // TODO(mdelgado): I'd rather revert back to the default color scheme in insert mode
  //  I'm not sure how I pull that in here
  active_color_table.arrays[ defcolor_cursor ].vals[ 0 ] = 0xff9ece6a;
  active_color_table.arrays[ defcolor_at_cursor ].vals[ 0 ] = 0xff68ac1e;
  active_color_table.arrays[ defcolor_margin_active ].vals[ 0 ] = 0xff9ece6a;
}

CUSTOM_COMMAND_SIG(enter_visual_mode) {
  set_current_mapid(app, mapid_visual);
  disable_snap_mark_to_cursor();

  set_mode_to_original(app);
  active_color_table.arrays[ defcolor_cursor ].vals[ 0 ] = 0xffbb9af7;
  active_color_table.arrays[ defcolor_at_cursor ].vals[ 0 ] = 0xffa675ff;
  active_color_table.arrays[ defcolor_margin_active ].vals[ 0 ] = 0xffbb9af7;
}

CUSTOM_COMMAND_SIG(enter_visual_line_mode) {
  set_current_mapid(app, mapid_visual_line);

  disable_snap_mark_to_cursor();
  seek_beginning_of_line(app);
  set_mark(app);
  seek_end_of_line(app);

  set_mode_to_original(app);
  active_color_table.arrays[ defcolor_cursor ].vals[ 0 ] = 0xffbb9af7;
  active_color_table.arrays[ defcolor_at_cursor ].vals[ 0 ] = 0xffa675ff;
  active_color_table.arrays[ defcolor_margin_active ].vals[ 0 ] = 0xffbb9af7;
}


CUSTOM_COMMAND_SIG(start_delete_combo) {
  set_current_mapid(app, mapid_delete);
}

CUSTOM_COMMAND_SIG(start_delete_inner_combo) {
  set_current_mapid(app, mapid_delete_inner);
}

// NOTE(mdelgado): Editing
CUSTOM_COMMAND_SIG(delete_to_end_of_line) {
  set_mark(app);
  seek_end_of_line(app);
  delete_range(app);
}

CUSTOM_COMMAND_SIG(edit_to_end_of_line) {
  delete_to_end_of_line(app);
  enter_insert_mode(app);
}


CUSTOM_COMMAND_SIG(edit_entire_line) {
  seek_beginning_of_line(app);
  edit_to_end_of_line(app);
}

// TODO(mdelgado): These line moves work, but are kinda heavy
CUSTOM_COMMAND_SIG(visual_line_move_up) {
  move_up(app);

  View_ID view = get_active_view(app, 0);
  i64 cursor = view_get_cursor_pos(app, view);
  i64 mark = view_get_mark_pos(app, view);

  if (cursor > mark) {
    seek_end_of_line(app);
    cursor_mark_swap(app);
    seek_beginning_of_line(app);
    cursor_mark_swap(app);
  } else if (cursor < mark) {
    seek_beginning_of_line(app);
    cursor_mark_swap(app);
    seek_end_of_line(app);
    cursor_mark_swap(app);
  } else {
    seek_beginning_of_line(app);
    cursor_mark_swap(app);
    seek_end_of_line(app);
  }
}

CUSTOM_COMMAND_SIG(visual_line_move_down) {
  move_down(app);

  View_ID view = get_active_view(app, 0);
  i64 cursor = view_get_cursor_pos(app, view);
  i64 mark = view_get_mark_pos(app, view);

  if (cursor > mark) {
    seek_end_of_line(app);
    cursor_mark_swap(app);
    seek_beginning_of_line(app);
    cursor_mark_swap(app);
  } else if (cursor < mark) {
    seek_beginning_of_line(app);
    cursor_mark_swap(app);
    seek_end_of_line(app);
    cursor_mark_swap(app);
  } else {
    seek_beginning_of_line(app);
    cursor_mark_swap(app);
    seek_end_of_line(app);
  }
}

CUSTOM_COMMAND_SIG(insert_after_cursor) {
  View_ID view = get_active_view(app, 0);
  i64 cursor = view_get_cursor_pos(app, view);
  Range_i64 range = {};
  range.start = cursor;
  range.end = cursor + 1;

  Buffer_ID buffer = view_get_buffer(app, view, 0);
  String_Const_u8 underCursor = get_chars_in_range(app, buffer, range);

  u8 curChar = underCursor.size ? underCursor.str[0] : 0;
  if (curChar != '\n' && curChar != 0) {
    move_right(app);
  }

  enter_insert_mode(app);
}

CUSTOM_COMMAND_SIG(insert_at_end_of_line) {
  seek_end_of_line(app);
  enter_insert_mode(app);
}

CUSTOM_COMMAND_SIG(insert_at_beginning_of_line) {
  seek_beginning_of_line(app);
  enter_insert_mode(app);
}

CUSTOM_COMMAND_SIG(create_new_line_below_and_insert) {
  seek_end_of_line(app);
  Scratch_Block scratch(app);
  String_Const_u8 newLine = push_stringf(scratch, "\n");
  write_string(app, newLine);
  enter_insert_mode(app);
}

CUSTOM_COMMAND_SIG(create_new_line_above_and_insert) {
  seek_beginning_of_line(app);
  Scratch_Block scratch(app);
  String_Const_u8 newLine = push_stringf(scratch, "\n");
  write_string(app, newLine);
  move_up(app);
  enter_insert_mode(app);
}

CUSTOM_COMMAND_SIG(change_range_case) {
  View_ID view = get_active_view(app, 0);
  i64 pos = view_get_cursor_pos(app, view);
  i64 mark = view_get_mark_pos(app, view);
  if (pos > mark || pos == mark) {
    move_right(app);
  } else {
    cursor_mark_swap(app);
    move_right(app);
    cursor_mark_swap(app);
  } 

  String_Const_u8 selected = get_selected_chars(app);

  for(int i = 0; i < selected.size; i++) {
    u8 curChar = selected.str[i];
    u8 updatedChar;
    if (character_is_upper(curChar)) {
      updatedChar = character_to_lower(curChar);
    } else {
      updatedChar = character_to_upper(curChar);
    }

    selected.str[i] = updatedChar;
  }


  Buffer_ID buffer = view_get_buffer(app, view, 0);
  Range_i64 range = view_get_highlight_range(app, view);
  buffer_replace_range(app, buffer, range, selected);
  view_set_cursor_and_preferred_x(app, view, seek_pos(pos));
  enter_normal_mode(app);
}

CUSTOM_COMMAND_SIG(visual_delete_range) {
  View_ID view = get_active_view(app, 0);
  i64 pos = view_get_cursor_pos(app, view);
  i64 mark = view_get_mark_pos(app, view);
  if (pos < mark) {
    cursor_mark_swap(app);
  }

  move_right(app);
  delete_range(app);
  enter_normal_mode(app);
}

CUSTOM_COMMAND_SIG(visual_edit_range) {
  visual_delete_range(app);
  enter_insert_mode(app);
}

CUSTOM_COMMAND_SIG(delete_line_to_normal_mode) {
  delete_line(app);
  enter_normal_mode(app);
}

CUSTOM_COMMAND_SIG(delete_word_to_normal_mode) {
  // TODO(mdelgado): This doesn't work correctly with whitespace chars
  delete_alpha_numeric_boundary(app);
  enter_normal_mode(app);
}

CUSTOM_COMMAND_SIG(delete_inner_word_to_normal_mode) {
  // TODO(mdelgado): There's a bug that appears when you
  //  are at the first letter of the word. You actually
  //  delete the previous word instead of deleting the
  //  word under the cursor.
  move_left_whitespace_boundary(app);
  set_mark(app);
  move_right_whitespace_boundary(app);
  delete_range(app);
  enter_normal_mode(app);
}

CUSTOM_COMMAND_SIG(noop) { }

// NOTE(mdelgado): Hooks
BUFFER_HOOK_SIG(custom_begin_buffer){
  default_begin_buffer(app, buffer_id);
  enter_normal_mode(app);

  return 0;
}

void
set_up_shared_mappings(Mapping *mapping) {
  MappingScope();
  SelectMapping(&framework_mapping);

  SelectMap(mapid_shared);
  // NOTE(mdelgado): This is needed for 4coder to properly start.
  BindCore(default_startup, CoreCode_Startup);
  // NOTE(mdelgado): This is needed for 4coder to exit without being forced.
  BindCore(default_try_exit, CoreCode_TryExit);

  Bind(enter_normal_mode, KeyCode_Escape);
  Bind(change_active_panel, KeyCode_L, KeyCode_Control);
  Bind(change_active_panel_backwards, KeyCode_H, KeyCode_Control);
  Bind(exit_4coder, KeyCode_Q, KeyCode_Control);
  Bind(interactive_open_or_new, KeyCode_O, KeyCode_Control);
  Bind(quick_swap_buffer, KeyCode_6, KeyCode_Control);
  Bind(move_down_to_blank_line_end, KeyCode_RightBracket, KeyCode_Control);
  Bind(move_up_to_blank_line_end, KeyCode_LeftBracket, KeyCode_Control);
  Bind(command_lister, KeyCode_X, KeyCode_Alt);
  /*
   * TODO(mdelgado): Make the below commented out code work. I'd like to
   *  be able to go to the next suggestion by pressing ctrl-n and previous
   *  by pressing ctrl-p. Right now that looks like I'd have to change the
   *  actual implementation.
   *  Bind(word_complete_drop_down,    KeyCode_Space, KeyCode_Control);
   */
  Bind(word_complete, KeyCode_Space, KeyCode_Control);
}

void
set_up_normal_mode_mappings(Mapping *mapping) {
  MappingScope();
  SelectMapping(mapping);

  SelectMap(mapid_normal);
  ParentMap(mapid_shared);
  Bind(enter_insert_mode, KeyCode_I);
  Bind(enter_visual_mode, KeyCode_V);
  Bind(enter_visual_line_mode, KeyCode_V, KeyCode_Shift);
  Bind(insert_at_beginning_of_line, KeyCode_I, KeyCode_Shift);
  Bind(insert_after_cursor, KeyCode_A);
  Bind(insert_at_end_of_line, KeyCode_A, KeyCode_Shift);
  Bind(create_new_line_below_and_insert, KeyCode_O);
  Bind(create_new_line_above_and_insert, KeyCode_O, KeyCode_Shift);
  Bind(move_down, KeyCode_J);
  Bind(move_up, KeyCode_K);
  Bind(move_left, KeyCode_H);
  Bind(move_right, KeyCode_L);
  /* TODO(mdelgado): This can be wrong when a command nests several
       commands. It will mostly just undo the nested commands rather
       than the entire parent command. Figure out if I can get this
       thing to do what I want.
  */
  Bind(undo, KeyCode_U);
  Bind(redo, KeyCode_R, KeyCode_Control);
  Bind(search, KeyCode_ForwardSlash);
  Bind(reverse_search, KeyCode_ForwardSlash, KeyCode_Shift);
  Bind(delete_char, KeyCode_X);
  Bind(delete_to_end_of_line, KeyCode_D, KeyCode_Shift);
  Bind(edit_to_end_of_line, KeyCode_C, KeyCode_Shift);
  Bind(edit_entire_line, KeyCode_S, KeyCode_Shift);
  Bind(move_left_whitespace_boundary, KeyCode_B);
  // NOTE(mdelgado): Both of these are technically wrong
  //  The both move forward to the white space, but I'd
  //  rather they move to the last char or first char
  Bind(move_right_whitespace_boundary, KeyCode_W);
  Bind(move_right_whitespace_boundary, KeyCode_E);

  // TODO(mdelgado): This doesn't work. We should see
  //  if we can figure out a way to make it work.
  // Bind(delete_line, KeyCode_D, KeyCode_D);
  // TODO(mdelgado): Can't do gg or goto_beginning_of_file
  Bind(goto_end_of_file, KeyCode_G, KeyCode_Shift);
  // NOTE(mdelgado): Technically the ^ command, but I
  //  Actually can't navigate past an indent
  Bind(seek_beginning_of_line, KeyCode_0);
  // NOTE(mdelgado): Can't bind the Caret Symbol, but this works
  Bind(seek_beginning_of_line, KeyCode_6, KeyCode_Shift);
  Bind(seek_end_of_line, KeyCode_4, KeyCode_Shift);
  Bind(change_range_case, KeyCode_Tick, KeyCode_Shift);
  Bind(start_delete_combo, KeyCode_D);
}

void
set_up_insert_mode_mappings(Mapping *mapping) {
  MappingScope();
  SelectMapping(&framework_mapping);

  SelectMap(mapid_insert);
  ParentMap(mapid_shared);
  BindTextInput(write_text_and_auto_indent);

  // NOTE(mdelgado): Some semi basic editor commands for insert mode
  Bind(delete_char, KeyCode_Delete);
  Bind(backspace_char, KeyCode_Backspace);
  Bind(move_up, KeyCode_Up);
  Bind(move_down, KeyCode_Down);
  Bind(move_left, KeyCode_Left);
  Bind(move_right, KeyCode_Right);
  Bind(move_up, KeyCode_Up, KeyCode_Shift);
  Bind(move_down, KeyCode_Down, KeyCode_Shift);
  Bind(move_left, KeyCode_Left, KeyCode_Shift);
  Bind(move_right, KeyCode_Right, KeyCode_Shift);
  Bind(seek_end_of_line, KeyCode_End);
  Bind(seek_beginning_of_line, KeyCode_Home);
  Bind(page_up, KeyCode_PageUp);
  Bind(page_down, KeyCode_PageDown);
  Bind(copy, KeyCode_C, KeyCode_Control);
  Bind(cut, KeyCode_X, KeyCode_Control);
  Bind(paste_and_indent, KeyCode_V, KeyCode_Control);
  Bind(search, KeyCode_F, KeyCode_Control);
  Bind(reverse_search, KeyCode_F, KeyCode_Control, KeyCode_Shift);
  Bind(save, KeyCode_S, KeyCode_Control);
  Bind(redo, KeyCode_Y, KeyCode_Control);
  Bind(undo, KeyCode_Z, KeyCode_Control);
}

void
set_up_visual_mode_mappings(Mapping *mapping) {
  MappingScope();
  SelectMapping(&framework_mapping);

  SelectMap(mapid_visual);
  ParentMap(mapid_shared);

  Bind(move_down, KeyCode_J);
  Bind(move_up, KeyCode_K);
  Bind(move_left, KeyCode_H);
  Bind(move_right, KeyCode_L);
  Bind(move_left_whitespace_boundary, KeyCode_B);
  // NOTE(mdelgado): Same as Normal Mode
  Bind(move_right_whitespace_boundary, KeyCode_W);
  Bind(move_right_whitespace_boundary, KeyCode_E);

  // NOTE(mdelgado): Same as Normal Mode
  Bind(goto_end_of_file, KeyCode_G, KeyCode_Shift);
  // NOTE(mdelgado): Same as Normal Mode
  Bind(seek_beginning_of_line, KeyCode_0);
  Bind(seek_beginning_of_line, KeyCode_6, KeyCode_Shift);
  Bind(seek_end_of_line, KeyCode_4, KeyCode_Shift);
  Bind(change_range_case, KeyCode_Tick, KeyCode_Shift);
  Bind(visual_delete_range, KeyCode_D);
  Bind(visual_delete_range, KeyCode_X);
  Bind(visual_edit_range, KeyCode_C);
  Bind(visual_edit_range, KeyCode_S);
}

void
set_up_visual_line_mode_mappings(Mapping *mapping) {
  MappingScope();
  SelectMapping(&framework_mapping);

  SelectMap(mapid_visual_line);
  ParentMap(mapid_shared);

  Bind(visual_line_move_down, KeyCode_J);
  Bind(visual_line_move_up, KeyCode_K);
  Bind(goto_end_of_file, KeyCode_G, KeyCode_Shift);
  Bind(change_range_case, KeyCode_Tick, KeyCode_Shift);
  Bind(visual_delete_range, KeyCode_D);
  Bind(visual_delete_range, KeyCode_X);
  Bind(visual_edit_range, KeyCode_C);
  Bind(visual_edit_range, KeyCode_S);
}

void
set_up_delete_combos(Mapping *mapping) {
  MappingScope();
  SelectMapping(mapping);

  SelectMap(mapid_delete);
  ParentMap(mapid_shared);

  Bind(delete_line_to_normal_mode, KeyCode_D);
  Bind(delete_word_to_normal_mode, KeyCode_W);
  Bind(start_delete_inner_combo, KeyCode_I);
}

void
set_up_delete_inner_combos(Mapping *mapping) {
  MappingScope();
  SelectMapping(mapping);

  SelectMap(mapid_delete_inner);
  ParentMap(mapid_shared);

  Bind(delete_inner_word_to_normal_mode, KeyCode_W);
}

// TODO(mdelgado): I'd like to avoid updating this function,
//  I don't think this system gives me that ability to define
//  this in my 4coder_custom.cpp file
void
custom_layer_init(Application_Links *app){
  Thread_Context *tctx = get_thread_context(app);

  // NOTE(allen): setup for default framework
  default_framework_init(app);

  // NOTE(allen): default hooks and command maps
  set_all_default_hooks(app);

  // NOTE(mdelgado): Adding in my hook overrides
  set_custom_hook(app, HookID_BeginBuffer, custom_begin_buffer);

  mapping_init(tctx, &framework_mapping);
  String_ID global_map_id = vars_save_string_lit("keys_global");
  String_ID file_map_id = vars_save_string_lit("keys_file");
  String_ID code_map_id = vars_save_string_lit("keys_code");

  mapid_shared = vars_save_string_lit("mapid_shared");
  mapid_normal = vars_save_string_lit("mapid_normal");
  mapid_insert = vars_save_string_lit("mapid_insert");
  mapid_visual = vars_save_string_lit("mapid_visual");
  mapid_visual_line = vars_save_string_lit("mapid_visual_line");

  mapid_delete = vars_save_string_lit("mapid_delete");
  mapid_delete_inner = vars_save_string_lit("mapid_delete_inner");

#if OS_MAC
    setup_mac_mapping(&framework_mapping, global_map_id, file_map_id, code_map_id);
#else
    setup_default_global_mapping(&framework_mapping, global_map_id);
#endif
  setup_essential_mapping(&framework_mapping, global_map_id, file_map_id, code_map_id);

  set_up_shared_mappings(&framework_mapping);
  set_up_normal_mode_mappings(&framework_mapping);
  set_up_insert_mode_mappings(&framework_mapping);
  set_up_visual_mode_mappings(&framework_mapping);
  set_up_visual_line_mode_mappings(&framework_mapping);

  // NOTE(mdelgad): Multi Key Combos
  set_up_delete_combos(&framework_mapping); 
  set_up_delete_inner_combos(&framework_mapping); 

  MappingScope();
  SelectMapping(&framework_mapping);

  // NOTE(mdelgado): All file and code maps should have normal mode as parent
  SelectMap(file_map_id);
  ParentMap(mapid_normal);

  SelectMap(code_map_id);
  ParentMap(mapid_normal);
}

#endif //FCODER_DEFAULT_BINDINGS

