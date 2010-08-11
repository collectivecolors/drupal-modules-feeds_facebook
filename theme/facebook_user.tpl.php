<?php
// $Id$
/**
 * @file facebook_user.tpl.php
 *
 * Theme a Facebook user object.
 *
 * - $fb_user: the FbUser instance.
 */
/*
if ($fb_user instanceof FbUser) {
  $fb_user->getConnections();            // Array (String) x
  $fb_user->getId();                     // String *
  $fb_user->getName();                   // String *
  $fb_user->getFirstName();              // String *
  $fb_user->getLastName();               // String *
  $fb_user->getLink();                   // Link *
  $fb_user->getEmail();                  // Email
  $fb_user->getWebsite();                // Link
  $fb_user->getTimezoneOffset();         // Integer (+|-)
  $fb_user->getGender();                 // String | FbPage? *
  $fb_user->getBirthday();               // String
  $fb_user->getRelationshipStatus();     // String
  $fb_user->getReligion();               // String | FbPage
  $fb_user->getPoliticalAffiliation();   // String | FbPage
  $fb_user->getLocation();               // String | FbPage
  $fb_user->getHometown();               // String | FbPage
  $fb_user->getWorkHistory();            // Array (FbJob)
  $fb_user->getEducation();              // Array (FbStudy)
  $fb_user->getVerified();               // Bool
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
$log->debug('facebook_user.tpl.php', get_defined_vars());

// Run a sanity check on our variable.
// This also gives us method autocomplete functionality in Eclipse Galileo ;-)
if (!($fb_user instanceof FbUser)) {
  if (is_string($fb_user)) {
    $log->debug('Returning string...', $fb_user);
    print "<div class='fb-user fb-user-string'>$fb_user</div>";
    return;
  }
  elseif ($fb_user instanceof FbObject) {
    $log->debug('Returning themed object...', $fb_user);
    print theme('facebook_object', $fb_user);
    return;
  }
  $log->debug('Returning nothing...', $fb_user);
  return;
}
$log->debug('Starting template...', $fb_user);

// Now for the user template...
?>
<div class="fb-user">
  <div class="fb-id-section">
    <span class="fb-label">User id:</span>
    <span class="fb-id"><?php print $fb_user->getId(); ?></span>
  </div>
  <div class="fb-name-section">
    <span class="fb-label">User name:</span>
    <span class="fb-name"><?php print $fb_user->getName(); ?></span>
  </div>
  <?php if ($fb_user->getFirstName()): ?>
    <div class="fb-first-name-section">
      <span class="fb-label">First name:</span>
      <span class="fb-first-name"><?php print $fb_user->getFirstName(); ?></span>
    </div>
  <?php endif ?>
  <?php if ($fb_user->getLastName()): ?>
    <div class="fb-last-name-section">
      <span class="fb-label">Last name:</span>
      <span class="fb-last-name"><?php print $fb_user->getLastName(); ?></span>
    </div>
  <?php endif ?>
  <?php if ($fb_user->getGender()): ?>
    <div class="fb-gender-section">
      <span class="fb-label">Gender:</span>
      <span class="fb-gender"><?php print theme('facebook_page', $fb_user->getGender()); ?></span>
    </div>
  <?php endif ?>
  <?php if ($fb_user->getLink()): ?>
    <div class="fb-link-section">
      <!-- <span class="fb-label">Facebook link:</span> -->
      <span class="fb-link"><a href="<?php print $fb_user->getLink(); ?>" target="_blank">Facebook page</a></span>
    </div>
  <?php endif ?>
</div>
