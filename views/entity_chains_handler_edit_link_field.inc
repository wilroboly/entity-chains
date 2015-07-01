<?php

/**
 * @file
 * Contains a Views field handler to take care of displaying edit links
 * as fields
 */


class entity_chains_handler_edit_link_field extends entity_chains_handler_link_field {
  function construct() {
    parent::construct();
    $this->additional_fields['type'] = 'type';
  }


  function render($values) {
    $type = $values->{$this->aliases['type']};
    
    //Creating a dummy chain to check access against
    $dummy_chain = (object) array('type' => $type);
    if (!entity_chains_access('edit', $dummy_chain)) {
      return;
    }
    
    $text = !empty($this->options['text']) ? $this->options['text'] : t('edit');
    $entity_chains_id = $values->{$this->aliases['entity_chains_id']};
    
    return l($text, 'admin/content/chains/chain/' . $entity_chains_id . '/edit');
  }
}
