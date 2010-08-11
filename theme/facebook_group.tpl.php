<?php
// $Id$
/**
 * @file facebook_group.tpl.php
 *
 * Theme a Facebook group object.
 *
 * - $fb_group: the FbGroup instance.
 */
/*
if ($fb_group instanceof FbGroup) {
  $fb_group->getConnections();           // Array (String) x
  $fb_group->getId();                    // String *
  $fb_group->getName();                  // String *
  $fb_group->getOwner();                 // FbUser *
  $fb_group->getDescription();           // String *
  $fb_group->getPrivacy();               // String *
  $fb_group->getUpdatedTime();           // Timestamp
  $fb_group->getLink();                  // Link *
  $fb_group->getLikes();                 // Integer (+) *
}

if ($data instanceof FbUser) {
  $data->getConnections();            // Array (String)
  $data->getId();                     // String
  $data->getName();                   // String
  $data->getFirstName();              // String
  $data->getLastName();               // String
  $data->getLink();                   // Link
  $data->getEmail();                  // Email
  $data->getWebsite();                // Link
  $data->getTimezoneOffset();         // Integer (+|-)
  $data->getGender();                 // String | FbPage?
  $data->getBirthday();               // String
  $data->getRelationshipStatus();     // String
  $data->getReligion();               // String | FbPage
  $data->getPoliticalAffiliation();   // String | FbPage
  $data->getLocation();               // String | FbPage
  $data->getHometown();               // String | FbPage
  $data->getWorkHistory();            // Array (FbJob)
  $data->getEducation();              // Array (FbStudy)
  $data->getVerified();               // Bool
}

if ($data instanceof FbJob) {
  $data->getEmployer();               // String | FbPage
  $data->getPosition();               // String | FbPage
  $data->getStartDate();              // Timestamp
  $data->getEndDate();                // Timestamp
}

if ($data instanceof FbStudy) {
  $data->getSchool();                 // String | FbPage
  $data->getYear();                   // String | FbPage
  $data->getConcentration();          // Array (String | FbPage)
}

if ($data instanceof FbPage) {
  $data->getConnections();            // Array (String)
  $data->getId();                     // String
  $data->getName();                   // String
  $data->getLikes();                  // Integer (+)
  $data->getLink();                   // Link
  $data->getPicture();                // Link
  $data->getCategory();               // String
  $data->getFanCount();               // Integer (+)
}
*/
$log = feeds_facebook_get_log();
$log->debug('facebook_group.tpl.php', get_defined_vars());

// Run a sanity check on our variable.
// This also gives us method autocomplete functionality in Eclipse Galileo ;-)
if (!($fb_group instanceof FbGroup)) {
  if (is_string($fb_group)) {
    $log->debug('Returning string...', $fb_group);
    print "<div class='fb-group fb-group-string'>$fb_group</div>";
    return;
  }
  elseif ($fb_group instanceof FbObject) {
    $log->debug('Returning themed object...', $fb_group);
    print theme('facebook_object', $fb_group);
    return;
  }
  $log->debug('Returning nothing...', $fb_group);
  return;
}
$log->debug('Starting template...', $fb_group);

// Now for the group template...
?>
<div class="fb-group">
  <div class="fb-id-section">
    <span class="fb-label">Group id:</span>
    <span class="fb-id"><?php print $fb_group->getId(); ?></span>
  </div>
  <div class="fb-name-section">
    <span class="fb-label">Group name:</span>
    <span class="fb-name"><?php print $fb_group->getName(); ?></span>
  </div>

  <?php if ($fb_group->getDescription()): ?>
    <div class="fb-description-section">
      <span class="fb-label">Description:</span>
      <div class="fb-description"><?php print $fb_group->getDescription(); ?></div>
    </div>
  <?php endif ?>
  <?php if ($fb_group->getPrivacy()): ?>
    <div class="fb-privacy-section">
      <span class="fb-label">Privacy:</span>
      <span class="fb-privacy"><?php print $fb_group->getPrivacy(); ?></span>
    </div>
  <?php endif ?>

  <?php if ($fb_group->getLink()): ?>
    <div class="fb-link-section">
      <!-- <span class="fb-label">Facebook link:</span> -->
      <span class="fb-link"><a href="<?php print $fb_group->getLink(); ?>" target="_blank">Facebook page</a></span>
    </div>
  <?php endif ?>

  <?php if ($fb_group->getOwner()): ?>
    <div class="fb-owner-section">
      <span class="fb-label">Group owner:</span>
      <div class="fb-owner"><?php print theme('facebook_user', $fb_group->getOwner()); ?></div>
    </div>
  <?php endif ?>

  <?php if ($fb_group->getLikes()): ?>
    <div class="fb-likes-section">
      <span class="fb-label">Likes:</span>
      <span class="fb-likes"><?php print $fb_group->getLikes(); ?></span>
    </div>
  <?php endif ?>
</div>
