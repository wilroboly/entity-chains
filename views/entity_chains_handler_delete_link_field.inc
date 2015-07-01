<?php

/**
 * @file
 * Contains a Views field handler to take care of displaying deletes links
 * as fields
 */


class entity_chain_handler_delete_link_field extends entity_chain_handler_link_field {
  function construct() {
    parent::construct();
    $this->additional_fields['type'] = 'type';
  }


  function render($values) {
    $type = $values->{$this->aliases['type']};
    
    //Creating a dummy chain to check access against
    $dummy_chain = (object) array('type' => $type);
    if (!entity_chain_access('edit', $dummy_chain)) {
      return;
    }
    
    $text = !empty($this->options['text']) ? $this->options['text'] : t('delete');
    $entity_chain_id = $values->{$this->aliases['entity_chain_id']};
    
    return l($text, 'admin/content/chains/chain/' . $entity_chain_id . '/delete');
  }
}
