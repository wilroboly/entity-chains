<?php

/**
 * @file
 * Chain type editing UI.
 */

/**
 * UI controller.
 */
class EntityChainTypeUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
		$items[$this->path]['description'] = 'Manage chain entity types, including adding
		and removing fields and the display of fields.';
    return $items;
  }
}

/**
 * Generates the chain type editing form.
 */
function entity_chain_type_form($form, &$form_state, $entity_chain_type, $op = 'edit') {

  if ($op == 'clone') {
    $entity_chain_type->label .= ' (cloned)';
    $entity_chain_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $entity_chain_type->label,
    '#description' => t('The human-readable name of this chain type.'),
    '#required' => TRUE,
    '#size' => 30,
  );
  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($entity_chain_type->type) ? $entity_chain_type->type : '',
    '#maxlength' => 32,
//    '#disabled' => $entity_chain_type->isLocked() && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'entity_chain_get_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this chain type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['data']['#tree'] = TRUE;
  $form['data']['sample_data'] = array(
    '#type' => 'checkbox',
    '#title' => t('An interesting chain switch'),
    '#default_value' => !empty($entity_chain_type->data['sample_data']),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save chain type'),
    '#weight' => 40,
  );

  //Locking not supported yet
  /*if (!$entity_chain_type->isLocked() && $op != 'add') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete chain type'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('entity_chain_type_form_submit_delete')
    );
  }*/
  return $form;
}

/**
 * Form API submit callback for the type form.
 */
function entity_chain_type_form_submit(&$form, &$form_state) {
  $entity_chain_type = entity_ui_form_submit_build_entity($form, $form_state);
  $entity_chain_type->save();
  $form_state['redirect'] = 'admin/structure/chain_types';
}

/**
 * Form API submit callback for the delete button.
 */
function entity_chain_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/chain_types/manage/' . $form_state['entity_chain_type']->type . '/delete';
}
