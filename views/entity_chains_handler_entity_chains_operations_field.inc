<?php

/**
 * This field handler aggregates operations that can be done on a chain
 * under a single field providing a more flexible way to present them in a view
 */
class entity_chain_handler_entity_chain_operations_field extends views_handler_field {
  function construct() {
    parent::construct();

    $this->additional_fields['entity_chain_id'] = 'entity_chain_id';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {

    $links = menu_contextual_links('entity_chain', 'admin/content/chains/chain', array($this->get_value($values, 'entity_chain_id')));
    
    if (!empty($links)) {
      return theme('links', array('links' => $links, 'attributes' => array('class' => array('links', 'inline', 'operations'))));
    }
  }
}
