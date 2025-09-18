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

String_ID mapid_shared;
String_ID mapid_normal;
String_ID mapid_insert;
String_ID mapid_visual;

void
set_current_mapid(Application_Links *app, Command_Map_ID mapid) {
  View_ID view = get_active_view(app, 0);
  Buffer_ID buffer = view_get_buffer(app, view, 0);
  Managed_Scope scope = buffer_get_managed_scope(app, buffer);
  Command_Map_ID *map_id_ptr = scope_attachment(app, scope, buffer_map_id, Command_Map_ID);
  *map_id_ptr = mapid;
}

CUSTOM_COMMAND_SIG(go_to_normal_mode) {
  set_current_mapid(app, mapid_normal);

  active_color_table.arrays[ defcolor_cursor ].vals[ 0 ] = 0xffff5533;
  active_color_table.arrays[ defcolor_at_cursor ].vals[ 0 ] = 0xff00aacc;
  active_color_table.arrays[ defcolor_margin_active ].vals[ 0 ] = 0xffff5533;
}

CUSTOM_COMMAND_SIG(go_to_insert_mode) {
  set_current_mapid(app, mapid_insert);

  active_color_table.arrays[ defcolor_cursor ].vals[ 0 ] = 0xff80ff80;
  active_color_table.arrays[ defcolor_at_cursor ].vals[ 0 ] = 0xff293134;
  active_color_table.arrays[ defcolor_margin_active ].vals[ 0 ] = 0xff80ff80;
}

CUSTOM_COMMAND_SIG(go_to_visual_mode) {
  set_current_mapid(app, mapid_visual);

  active_color_table.arrays[ defcolor_cursor ].vals[ 0 ] = 0xffffff00;
  active_color_table.arrays[ defcolor_at_cursor ].vals[ 0 ] = 0xff0000ff;
}

BUFFER_HOOK_SIG(custom_begin_buffer){
  go_to_normal_mode(app);
  return 0;
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
  // TODO(mdelgado): This almost worked. Something is setting the
  //  map back to file_map_id, and no longer treats buffers as code
  // set_custom_hook(app, HookID_BeginBuffer, custom_begin_buffer);

  mapping_init(tctx, &framework_mapping);
  String_ID global_map_id = vars_save_string_lit("keys_global");
  String_ID file_map_id = vars_save_string_lit("keys_file");
  String_ID code_map_id = vars_save_string_lit("keys_code");

  mapid_shared = vars_save_string_lit("mapid_shared");
  mapid_normal = vars_save_string_lit("mapid_normal");
  mapid_insert = vars_save_string_lit("mapid_insert");
  mapid_visual = vars_save_string_lit("mapid_visual");

#if OS_MAC
    setup_mac_mapping(&framework_mapping, global_map_id, file_map_id, code_map_id);
#else
    setup_default_global_mapping(&framework_mapping, global_map_id);
#endif
  setup_essential_mapping(&framework_mapping, global_map_id, file_map_id, code_map_id);

  MappingScope();
  SelectMapping(&framework_mapping);

  SelectMap(mapid_shared);
  // NOTE(mdelgado): This is needed for 4coder to properly start.
  BindCore(default_startup, CoreCode_Startup);
  // NOTE(mdelgado): This is needed for 4coder to exit without being forced.
  BindCore(default_try_exit, CoreCode_TryExit);

  Bind(go_to_normal_mode, KeyCode_Escape);
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

  SelectMap(mapid_normal);
  ParentMap(mapid_shared);
  Bind(go_to_insert_mode, KeyCode_I);
  Bind(move_down, KeyCode_J);
  Bind(move_up, KeyCode_K);
  Bind(move_left, KeyCode_H);
  Bind(move_right, KeyCode_L);
  Bind(undo, KeyCode_U);
  Bind(redo, KeyCode_R, KeyCode_Control);
  Bind(search, KeyCode_ForwardSlash);
  Bind(reverse_search, KeyCode_ForwardSlash, KeyCode_Shift);
  Bind(delete_char, KeyCode_X);
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
  // TODO(mdelgado): Needs to uppercase/lowercase and only do one char
  // Bind(to_uppercase, KeyCode_Tick, KeyCode_Shift);

  SelectMap(mapid_insert);
  ParentMap(mapid_shared);
  // NOTE(mdelgado): Some semi basic editor commands for insert mode
  BindTextInput(write_text_and_auto_indent);
  Bind(delete_char, KeyCode_Delete);
  Bind(backspace_char, KeyCode_Backspace);
  Bind(move_up, KeyCode_Up);
  Bind(move_down, KeyCode_Down);
  Bind(move_left, KeyCode_Left);
  Bind(move_right, KeyCode_Right);
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

  // NOTE(mdelgado): All file and code maps should have normal mode as parent
  SelectMap(file_map_id);
  ParentMap(mapid_normal);

  SelectMap(code_map_id);
  ParentMap(mapid_normal);
}

#endif //FCODER_DEFAULT_BINDINGS

