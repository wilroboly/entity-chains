<?php

/**
 * This field handler aggregates operations that can be done on a chain
 * under a single field providing a more flexible way to present them in a view
 */
class entity_chains_handler_entity_chains_operations_field extends views_handler_field {
  function construct() {
    parent::construct();

    $this->additional_fields['entity_chains_id'] = 'entity_chains_id';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {

    $links = menu_contextual_links('entity_chains', 'admin/content/chains/chain', array($this->get_value($values, 'entity_chains_id')));
    
    if (!empty($links)) {
      return theme('links', array('links' => $links, 'attributes' => array('class' => array('links', 'inline', 'operations'))));
    }
  }
}
