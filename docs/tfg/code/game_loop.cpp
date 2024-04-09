while (is_app_still_running) {
  check_is_app_still_running();
  
  //input
  process_user_input();
  
  // update
  update_scene();
  
  // events
  app_events();
  scene_events();
  
  // display scene
  render_scene();
}