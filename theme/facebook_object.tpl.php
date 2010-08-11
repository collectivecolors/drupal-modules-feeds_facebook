<?php
// $Id$
/**
 * @file facebook_object.tpl.php
 *
 * Theme a generic Facebook object.
 *
 * - $fb_object: the FbObject instance.
 */
/*
if ($fb_object instanceof FbObject) {
  $fb_object->getConnections();            // Array (String) x
  $fb_object->getId();                     // String *
  $fb_object->getName();                   // String *
  $fb_object->getType();                   // String *
}
*/
$log = feeds_facebook_get_log();
$log->debug('facebook_object.tpl.php', get_defined_vars());

// Run a sanity check on our variable.
// This also gives us method autocomplete functionality in Eclipse Galileo.
if (!($fb_object instanceof FbObject)) {
  if (is_string($fb_object)) {
    $log->debug('Returning string...', $fb_object);
    print "<span class='fb-object fb-object-string'>$fb_object</span>";
    return;
  }
  $log->debug('Returning nothing...', $fb_object);
  return;
}
$log->debug('Starting template...', $fb_object);

// Now for the object template...
?>
<div class="fb-object">
  <div class="fb-id-section">
    <span class="fb-label">Object id:</span>
    <span class="fb-id"><?php print $fb_object->getId(); ?></span>
  </div>
  <div class="fb-name-section">
    <span class="fb-label">Object name:</span>
    <span class="fb-name"><?php print $fb_object->getName(); ?></span>
  </div>
  <div class="fb-type-section">
    <span class="fb-label">Facebook type:</span>
    <span class="fb-type"><?php print ucfirst($fb_object->getType()); ?></span>
  </div>
</div>

