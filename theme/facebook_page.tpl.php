<?php
// $Id$
/**
 * @file facebook_page.tpl.php
 *
 * Theme a Facebook page object.
 *
 * - $fb_page: the FbPage instance.
 */
/*
if ($fb_page instanceof FbPage) {
  $fb_page->getConnections();            // Array (String) x
  $fb_page->getId();                     // String *
  $fb_page->getName();                   // String *
  $fb_page->getLikes();                  // Integer (+) *
  $fb_page->getLink();                   // Link *
  $fb_page->getPicture();                // Link *
  $fb_page->getCategory();               // String *
  $fb_page->getFanCount();               // Integer (+) *
}
*/
$log = feeds_facebook_get_log();
$log->debug('facebook_page.tpl.php', get_defined_vars());

// Run a sanity check on our variable.
// This also gives us method autocomplete functionality in Eclipse Galileo ;-)
if (!($fb_page instanceof FbPage)) {
  if (is_string($fb_page)) {
    $log->debug('Returning string...', $fb_page);
    print "<span class='fb-page fb-page-string'>$fb_page</span>";
    return;
  }
  elseif ($fb_page instanceof FbObject) {
    $log->debug('Returning themed object...', $fb_page);
    print theme('facebook_object', $fb_page);
    return;
  }
  $log->debug('Returning nothing...', $fb_page);
  return;
}
$log->debug('Starting template...', $fb_page);

// Now for the page template...
?>
<div class="fb-page">
  <div class="fb-id-section">
    <span class="fb-label">Page id:</span>
    <span class="fb-id"><?php print $fb_page->getId(); ?></span>
  </div>
  <div class="fb-name-section">
    <span class="fb-label">Page name:</span>
    <span class="fb-name"><?php print $fb_page->getName(); ?></span>
  </div>

  <?php if ($fb_page->getCategory()): ?>
    <div class="fb-category-section">
      <span class="fb-label">Category:</span>
      <span class="fb-category"><?php print $fb_page->getCategory(); ?></span>
    </div>
  <?php endif ?>

  <?php if ($fb_page->getPicture()): ?>
    <div class="fb-picture-section">
      <!-- <span class="fb-label">Picture:</span> -->
      <div class="fb-picture"><img href="<?php print $fb_page->getPicture(); ?>" /></div>
    </div>
  <?php endif ?>
  <?php if ($fb_page->getLink()): ?>
    <div class="fb-link-section">
      <!-- <span class="fb-label">Facebook link:</span> -->
      <span class="fb-link"><a href="<?php print $fb_page->getLink(); ?>" target="_blank">Facebook page</a></span>
    </div>
  <?php endif ?>

  <?php if ($fb_page->getFanCount()): ?>
    <div class="fb-fan-count-section">
      <span class="fb-label">Fan count:</span>
      <span class="fb-fan-count"><?php print $fb_page->getFanCount(); ?></span>
    </div>
  <?php endif ?>
  <?php if ($fb_page->getLikes()): ?>
    <div class="fb-likes-section">
      <span class="fb-label">Likes:</span>
      <span class="fb-likes"><?php print $fb_page->getLikes(); ?></span>
    </div>
  <?php endif ?>
</div>
