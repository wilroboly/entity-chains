<?php

/**
 * @file
 * Example tpl file for theming a single entity-chain-specific theme
 *
 * Available variables:
 * - $status: The variable to theme (while only show if you tick status)
 * 
 * Helper variables:
 * - $entity_chain: The Chain object this status is derived from
 */
?>

<div class="entity-chain-status">
  <?php print '<strong>Chain Sample Data:</strong> ' . $entity_chain_sample_data = ($entity_chain_sample_data) ? 'Switch On' : 'Switch Off' ?>
</div>