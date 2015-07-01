<?php

/**
 * @file
 * Providing extra functionality for the Chain UI via views.
 */


/**
 * Implements hook_views_data()
 */
function entity_chain_views_data_alter(&$data) { 
  $data['entity_chain']['link_chain'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('Provide a link to the entity_chain.'),
      'handler' => 'entity_chain_handler_link_field',
    ),
  );
  $data['entity_chain']['edit_chain'] = array(
    'field' => array(
      'title' => t('Edit Link'),
      'help' => t('Provide a link to the edit form for the entity_chain.'),
      'handler' => 'entity_chain_handler_edit_link_field',
    ),
  );
  $data['entity_chain']['delete_chain'] = array(
    'field' => array(
      'title' => t('Delete Link'),
      'help' => t('Provide a link to delete the entity_chain.'),
      'handler' => 'entity_chain_handler_delete_link_field',
    ),
  );
  // This content of this field are decided based on the menu structure that
  // follows chains/chain/%entity_chain_id/op
  $data['entity_chain']['operations'] = array(
    'field' => array(
      'title' => t('Operations links'),
      'help' => t('Display all operations available for this entity_chain.'),
      'handler' => 'entity_chain_handler_entity_chain_operations_field',
    ),
  );
}


/**
 * Implements hook_views_default_views().
 */
function entity_chain_views_default_views() {
  $view = new view();
  $view->name = 'chains';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'entity_chain';
  $view->human_name = 'Chains';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'entity_chain_id' => 'entity_chain_id',
    'name' => 'name',
    'operations' => 'operations',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'entity_chain_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'operations' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Field: Chain: Chain ID */
  $handler->display->display_options['fields']['entity_chain_id']['id'] = 'entity_chain_id';
  $handler->display->display_options['fields']['entity_chain_id']['table'] = 'entity_chain';
  $handler->display->display_options['fields']['entity_chain_id']['field'] = 'entity_chain_id';
  /* Field: Chain: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'entity_chain';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  /* Field: Chain: Operations links */
  $handler->display->display_options['fields']['operations']['id'] = 'operations';
  $handler->display->display_options['fields']['operations']['table'] = 'entity_chain';
  $handler->display->display_options['fields']['operations']['field'] = 'operations';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'chains_view_page');
  $handler->display->display_options['path'] = 'admin/content/chains/list';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'Chains';
  $handler->display->display_options['menu']['description'] = 'Manage Chains';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 1;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'tab';
  $handler->display->display_options['tab_options']['title'] = 'Chains';
  $handler->display->display_options['tab_options']['description'] = 'Manage chains';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $views[$view->name] = $view;
  return $views;

}
