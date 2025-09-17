/*
4coder_custom.cpp
*/

extern "C" b32
get_version(i32 maj, i32 min, i32 patch){
  return(maj == MAJOR && min == MINOR && patch == PATCH);
}

/*
NOTE(mdelgado): This throws compile errors because functions like
default_framework_init are not found. Not sure what the problem is
but i'll have to untangle that eventually

#include "4coder_default_include.cpp"
void
md_custom_layer_init(Application_Links *app){
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
}
*/

// TODO(mdelgado): Does this TODO comment highlight?
extern "C" Custom_Layer_Init_Type*
init_apis(API_VTable_custom *custom_vtable, API_VTable_system *system_vtable){
  custom_api_read_vtable(custom_vtable);
  system_api_read_vtable(system_vtable);
  return(custom_layer_init);
}

