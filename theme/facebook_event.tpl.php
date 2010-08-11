<?php
// $Id$
/**
 * @file facebook_event.tpl.php
 *
 * Theme a Facebook event object.
 *
 * - $fb_event: the FbEvent instance.
 */
/*
if ($fb_event instanceof FbEvent) {
  $fb_event->getConnections();           // Array (String) x
  $fb_event->getId();                    // String *
  $fb_event->getName();                  // String *
  $fb_event->getOwner();                 // FbUser *
  $fb_event->getDescription();           // String *
  $fb_event->getPrivacy();               // String *
  $fb_event->getUpdatedTime();           // Timestamp
  $fb_event->getLikes();                 // Integer (+) *
  $fb_event->getStartTime();             // Timestamp *
  $fb_event->getEndTime();               // Timestamp *
  $fb_event->getRsvpStatus();            // String *
  $fb_event->getLocation();              // String | FbPage *
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
$log->debug('facebook_event.tpl.php', get_defined_vars());

// Run a sanity check on our variable.
// This also gives us method autocomplete functionality in Eclipse Galileo ;-)
if (!($fb_event instanceof FbEvent)) {
  if (is_string($fb_event)) {
    $log->debug('Returning string...', $fb_event);
    print "<div class='fb-event fb-event-string'>$fb_event</div>";
    return;
  }
  elseif ($fb_event instanceof FbObject) {
    $log->debug('Returning themed object...', $fb_event);
    print theme('facebook_object', $fb_event);
    return;
  }
  $log->debug('Returning nothing...', $fb_event);
  return;
}
$log->debug('Starting template...', $fb_event);

// Now for the event template...
?>
<div class="fb-event">
  <div class="fb-id-section">
    <span class="fb-label">Event id:</span>
    <span class="fb-id"><?php print $fb_event->getId(); ?></span>
  </div>
  <div class="fb-name-section">
    <span class="fb-label">Event name:</span>
    <span class="fb-name"><?php print $fb_event->getName(); ?></span>
  </div>

  <?php if ($fb_event->getDescription()): ?>
    <div class="fb-description-section">
      <span class="fb-label">Description:</span>
      <div class="fb-description"><?php print $fb_event->getDescription(); ?></div>
    </div>
  <?php endif ?>
  <?php if ($fb_event->getPrivacy()): ?>
    <div class="fb-privacy-section">
      <span class="fb-label">Privacy:</span>
      <span class="fb-privacy"><?php print $fb_event->getPrivacy(); ?></span>
    </div>
  <?php endif ?>

  <?php if ($fb_event->getStartTime()): ?>
    <div class="fb-start-time-section">
      <span class="fb-label">Start time:</span>
      <span class="fb-start-time"><?php print $fb_event->getStartTime(); ?></span>
    </div>
  <?php endif ?>
  <?php if ($fb_event->getEndTime()): ?>
    <div class="fb-end-time-section">
      <span class="fb-label">End time:</span>
      <span class="fb-end-time"><?php print $fb_event->getEndTime(); ?></span>
    </div>
  <?php endif ?>
  <?php if ($fb_event->getRsvpStatus()): ?>
    <div class="fb-rsvp-status-section">
      <span class="fb-label">RSVP Status:</span>
      <span class="fb-rsvp-status"><?php print $fb_event->getRsvpStatus(); ?></span>
    </div>
  <?php endif ?>

  <?php if ($fb_event->getOwner()): ?>
    <div class="fb-owner-section">
      <span class="fb-label">Event owner:</span>
      <div class="fb-owner"><?php print theme('facebook_user', $fb_event->getOwner()); ?></div>
    </div>
  <?php endif ?>
  <?php if ($fb_event->getLocation()): ?>
    <div class="fb-location-section">
      <span class="fb-label">Event location:</span>
      <div class="fb-location"><?php print theme('facebook_page', $fb_event->getLocation()); ?></div>
    </div>
  <?php endif ?>

  <?php if ($fb_event->getLikes()): ?>
    <div class="fb-likes-section">
      <span class="fb-label">Likes:</span>
      <span class="fb-likes"><?php print $fb_event->getLikes(); ?></span>
    </div>
  <?php endif ?>
</div>
