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
#if OS_MAC
    setup_mac_mapping(&framework_mapping, global_map_id, file_map_id, code_map_id);
#else
    setup_default_mapping(&framework_mapping, global_map_id, file_map_id, code_map_id);
#endif
  setup_essential_mapping(&framework_mapping, global_map_id, file_map_id, code_map_id);

  MappingScope();
  SelectMapping(&framework_mapping);

  SelectMap(global_map_id);
  Bind(exit_4coder, KeyCode_Q, KeyCode_Control);
  Bind(change_active_panel, KeyCode_L, KeyCode_Control);
  Bind(change_active_panel_backwards, KeyCode_H, KeyCode_Control);

  SelectMap(file_map_id);
  ParentMap(global_map_id);
  // NOTE(mdelgado): I don't love that I have to do this on both
  //  a global_map and file map. Since ctrl-q seems to be set in
  //  the file_map my quit map will get consumed by the file version,
  //  When we fully reset these bindings, I may not have to do this.
  Bind(exit_4coder, KeyCode_Q, KeyCode_Control);
  // NOTE(mdelgado): Also pagued by having a global binding and file binding
  Bind(change_active_panel, KeyCode_L, KeyCode_Control);
}

#endif //FCODER_DEFAULT_BINDINGS

