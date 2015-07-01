<?php

/**
 * @file
 * Chain editing UI.
 *
 * We make very little use of the EntityAPI interface for this - preferring instead to use
 * views. That offers more flexibility to change a UI that will, more often than not,
 * be end-user facing.
 */

/**
 * UI controller.
 */
class EntityChainUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults. Main reason for doing this is that
   * parent class hook_menu() is optimized for entity type administration.
   */
  public function hook_menu() {
    
    $items = array();
    $id_count = count(explode('/', $this->path));
    $wildcard = isset($this->entityInfo['admin ui']['menu wildcard']) ? $this->entityInfo['admin ui']['menu wildcard'] : '%' . $this->entityType;

    $items[$this->path] = array(
      'title' => 'Chains',
      'description' => 'Add edit and update chains.',
      'page callback' => 'system_admin_menu_block_page',
      'access arguments' => array('access administration pages'),
      'file path' => drupal_get_path('module', 'system'),
      'file' => 'system.admin.inc',
    );
    
    // Change the overview menu type for the list of chains.
    $items[$this->path]['type'] = MENU_LOCAL_TASK;
    
    // Change the add page menu to multiple types of entities
    $items[$this->path . '/add'] = array(
      'title' => 'Add a chain',
      'description' => 'Add a new chain',
      'page callback'  => 'entity_chain_add_page',
      'access callback'  => 'entity_chain_access',
      'access arguments' => array('edit'),
      'type' => MENU_NORMAL_ITEM,
      'weight' => 20,
      'file' => 'entity_chains.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])

    );
    
    // Add menu items to add each different type of entity.
    foreach (entity_chain_get_types() as $type) {
      $items[$this->path . '/add/' . $type->type] = array(
        'title' => 'Add ' . $type->label,
        'page callback' => 'entity_chain_form_wrapper',
        'page arguments' => array(entity_chain_create(array('type' => $type->type))),
        'access callback' => 'entity_chain_access',
        'access arguments' => array('edit', 'edit ' . $type->type),
        'file' => 'entity_chains.admin.inc',
        'file path' => drupal_get_path('module', $this->entityInfo['module'])
      );
    }

    // Loading and editing chain entities
    $items[$this->path . '/chain/' . $wildcard] = array(
      'page callback' => 'entity_chain_form_wrapper',
      'page arguments' => array($id_count + 1),
      'access callback' => 'entity_chain_access',
      'access arguments' => array('edit', $id_count + 1),
      'weight' => 0,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
      'file' => 'entity_chains.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );
    $items[$this->path . '/chain/' . $wildcard . '/edit'] = array(
      'title' => 'Edit',
      'type' => MENU_DEFAULT_LOCAL_TASK,
      'weight' => -10,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    );
    
    $items[$this->path . '/chain/' . $wildcard . '/delete'] = array(
      'title' => 'Delete',
      'page callback' => 'entity_chain_delete_form_wrapper',
      'page arguments' => array($id_count + 1),
      'access callback' => 'entity_chain_access',
      'access arguments' => array('edit', $id_count + 1),
      'type' => MENU_LOCAL_TASK,
      'context' => MENU_CONTEXT_INLINE,
      'weight' => 10,
      'file' => 'entity_chains.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );
    
    // Menu item for viewing chains
    $items['chain/' . $wildcard] = array(
      //'title' => 'Title',
      'title callback' => 'entity_chain_page_title',
      'title arguments' => array(1),
      'page callback' => 'entity_chain_page_view',
      'page arguments' => array(1),
      'access callback' => 'entity_chain_access',
      'access arguments' => array('view', 1),
      'type' => MENU_CALLBACK,
    );
    return $items;
  }
  
  
  /**
   * Create the markup for the add Chain Entities page within the class
   * so it can easily be extended/overriden.
   */ 
  public function addPage() {
    $item = menu_get_item();
    $content = system_admin_menu_block($item);

    if (count($content) == 1) {
      $item = array_shift($content);
      drupal_goto($item['href']);
    }    
        
    return theme('entity_chain_add_list', array('content' => $content));
  }
  
}


/**
 * Form callback wrapper: create or edit a entity_chain.
 *
 * @param $entity_chain
 *   The chain object being edited by this form.
 *
 * @see entity_chain_edit_form()
 */
function entity_chain_form_wrapper($entity_chain) {
  // Add the breadcrumb for the form's location.
  entity_chain_set_breadcrumb();
  return drupal_get_form('entity_chain_edit_form', $entity_chain);
}


/**
 * Form callback wrapper: delete a entity_chain.
 *
 * @param $entity_chain
 *   The chain object being edited by this form.
 *
 * @see entity_chain_edit_form()
 */
function entity_chain_delete_form_wrapper($entity_chain) {
  // Add the breadcrumb for the form's location.
  //chain_set_breadcrumb();
  return drupal_get_form('entity_chain_delete_form', $entity_chain);
}


/**
 * Form callback: create or edit a entity_chain.
 *
 * @param $entity_chain
 *   The chain object to edit or for a create form an empty chain object
 *     with only a chain type defined.
 */
function entity_chain_edit_form($form, &$form_state, $entity_chain) {
  // Add the default field elements.
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Chain Name'),
    '#default_value' => isset($entity_chain->name) ? $entity_chain->name : '',
    '#maxlength' => 255,
    '#required' => TRUE,
    '#weight' => -5,
  );
  
  $form['data']['#tree'] = TRUE;
  $form['data']['sample_data'] = array(
    '#type' => 'checkbox',
    '#title' => t('An interesting chain switch'),
    '#default_value' => isset($entity_chain->data['sample_data']) ? $entity_chain->data['sample_data'] : 1,
  );

  // Add the field related form elements.
  $form_state['entity_chain'] = $entity_chain;
  field_attach_form('entity_chain', $entity_chain, $form, $form_state);

  $form['actions'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('form-actions')),
    '#weight' => 400,
  );

  // We add the form's #submit array to this button along with the actual submit
  // handler to preserve any submit handlers added by a form callback_wrapper.
  $submit = array();

  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save chain'),
    '#submit' => $submit + array('entity_chain_edit_form_submit'),
  );
  
  if (!empty($entity_chain->name)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete chain'),
      '#suffix' => l(t('Cancel'), 'admin/content/chains'),
      '#submit' => $submit + array('entity_chain_form_submit_delete'),
      '#weight' => 45,
    );
  }

  // We append the validate handler to #validate in case a form callback_wrapper
  // is used to add validate handlers earlier.
  $form['#validate'][] = 'entity_chain_edit_form_validate';
  return $form;
}


/**
 * Form API validate callback for the chain form
 */
function entity_chain_edit_form_validate(&$form, &$form_state) {
  $entity_chain = $form_state['entity_chain'];
  
  // Notify field widgets to validate their data.
  field_attach_form_validate('entity_chain', $entity_chain, $form, $form_state);
}


/**
 * Form API submit callback for the chain form.
 * 
 * @todo remove hard-coded link
 */
function entity_chain_edit_form_submit(&$form, &$form_state) {
  
  $entity_chain = entity_ui_controller('entity_chain')->entityFormSubmitBuildEntity($form, $form_state);
  // Save the chain and go back to the list of chains
  
  // Add in created and changed times.
  if ($entity_chain->is_new = isset($entity_chain->is_new) ? $entity_chain->is_new : 0){
    $entity_chain->created = time();
  }

  $entity_chain->changed = time();
  
  $entity_chain->save();
  $form_state['redirect'] = 'admin/content/chains';
}

/**
 * Form API submit callback for the delete button.
 * 
 * @todo Remove hard-coded path
 */
function entity_chain_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/content/chains/chain/' . $form_state['entity_chain']->entity_chain_id . '/delete';
}


/**
 * Form callback: confirmation form for deleting a entity_chain.
 *
 * @param $entity_chain
 *   The chain to delete
 *
 * @see confirm_form()
 */
function entity_chain_delete_form($form, &$form_state, $entity_chain) {
  $form_state['entity_chain'] = $entity_chain;

  $form['#submit'][] = 'entity_chain_delete_form_submit';

  $form = confirm_form($form,
    t('Are you sure you want to delete chain %name?', array('%name' => $entity_chain->name)),
    'admin/content/chains/chain',
    '<p>' . t('This action cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );
  
  return $form;
}

/**
 * Submit callback for entity_chain_delete_form
 */
function entity_chain_delete_form_submit($form, &$form_state) {
  $entity_chain = $form_state['entity_chain'];

  entity_chain_delete($entity_chain);

  drupal_set_message(t('The chain %name has been deleted.', array('%name' => $entity_chain->name)));
  watchdog('entity_chain', 'Deleted chain %name.', array('%name' => $entity_chain->name));

  $form_state['redirect'] = 'admin/content/chains';
}



/**
 * Page to add Chain Entities.
 *
 * @todo Pass this through a proper theme function
 */
function entity_chain_add_page() {
  $controller = entity_ui_controller('entity_chain');
  return $controller->addPage();
}


/**
 * Displays the list of available chain types for chain creation.
 *
 * @ingroup themeable
 */
function theme_entity_chain_add_list($variables) {
  $content = $variables['content'];
  $output = '';
  if ($content) {
    $output = '<dl class="entity-chain-type-list">';
    foreach ($content as $item) {
      $output .= '<dt>' . l($item['title'], $item['href']) . '</dt>';
      $output .= '<dd>' . filter_xss_admin($item['description']) . '</dd>';
    }
    $output .= '</dl>';
  }
  else {
    if (user_access('administer chain types')) {
      $output = '<p>' . t('Chain Entities cannot be added because you have not created any chain types yet. Go to the <a href="@create-entity-chain-type">chain type creation page</a> to add a new chain type.', array('@create-entity-chain-type' => url('admin/structure/chain_types/add'))) . '</p>';
    }
    else {
      $output = '<p>' . t('No chain types have been created yet for you to use.') . '</p>';
    }
  }

  return $output;
}





/**
 * Sets the breadcrumb for administrative chain pages.
 */
function entity_chain_set_breadcrumb() {
  $breadcrumb = array(
    l(t('Home'), '<front>'),
    l(t('Administration'), 'admin'),
    l(t('Content'), 'admin/content'),
    l(t('Chains'), 'admin/content/chains'),
  );

  drupal_set_breadcrumb($breadcrumb);
}



