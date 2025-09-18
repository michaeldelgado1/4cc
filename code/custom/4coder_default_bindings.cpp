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
    setup_default_mapping(&framework_mapping, global_map_id, file_map_id, code_map_id);
#endif
  setup_essential_mapping(&framework_mapping, global_map_id, file_map_id, code_map_id);

  MappingScope();
  SelectMapping(&framework_mapping);


  SelectMap(mapid_shared);
  BindCore(default_startup, CoreCode_Startup);
  BindCore(default_try_exit, CoreCode_TryExit);
  Bind(go_to_normal_mode, KeyCode_Escape);
  Bind(change_active_panel, KeyCode_L, KeyCode_Control);
  Bind(change_active_panel_backwards, KeyCode_H, KeyCode_Control);
  Bind(exit_4coder, KeyCode_Q, KeyCode_Control);
  Bind(interactive_open_or_new, KeyCode_O, KeyCode_Control);
  Bind(quick_swap_buffer, KeyCode_6, KeyCode_Control);
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
  Bind(redo, KeyCode_R, KeyCode_Control);
  Bind(undo, KeyCode_U);
  Bind(move_down, KeyCode_J);
  Bind(move_up, KeyCode_K);
  Bind(move_left, KeyCode_H);
  Bind(move_right, KeyCode_L);
  Bind(go_to_insert_mode, KeyCode_I);
  Bind(delete_char, KeyCode_X);

  SelectMap(mapid_insert);
  ParentMap(mapid_shared);
  BindTextInput(write_text_and_auto_indent);
  Bind(delete_char, KeyCode_Delete);

  SelectMap(file_map_id);
  ParentMap(mapid_normal);

  SelectMap(code_map_id);
  ParentMap(mapid_normal);

  // TODO(mdelgado): Since we start in normal mode, we should indicate it.
  //  I'll most likely need to do this in a hook
  // active_color_table.arrays[ defcolor_cursor ].vals[ 0 ] = 0xffff5533;
  // active_color_table.arrays[ defcolor_at_cursor ].vals[ 0 ] = 0xff00aacc;
  // active_color_table.arrays[ defcolor_margin_active ].vals[ 0 ] = 0xffff5533;
}

#endif //FCODER_DEFAULT_BINDINGS

