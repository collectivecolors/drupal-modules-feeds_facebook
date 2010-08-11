<?php
// $Id$

/*******************************************************************************
 * Form theme functions
 */

/**
 * Render Facebook search form.
 */
function theme_facebook_search_form($form) {
  $log = feeds_facebook_get_log();
  $log->debug('theme_facebook_search_form()', $form);
  return drupal_render($form);
}

//------------------------------------------------------------------------------

/**
 * Render Facebook object selector form.
 */
function theme_facebook_object_selector_form($form) {
  $log = feeds_facebook_get_log();
  $log->debug('theme_facebook_object_selector_form()', $form);
  return drupal_render($form);
}

//------------------------------------------------------------------------------

/**
 * Render Facebook object canvas form.
 */
function theme_facebook_object_canvas_form($form) {
  $log = feeds_facebook_get_log();
  $log->debug('theme_facebook_object_canvas_form()', $form);
  return drupal_render($form);
}

//------------------------------------------------------------------------------

/**
 * Render Facebook feed selector form.
 */
function theme_facebook_feed_selector_form($form) {
  $log = feeds_facebook_get_log();
  $log->debug('theme_facebook_feed_selector_form()', $form);
  return drupal_render($form);
}

//------------------------------------------------------------------------------

/**
 * Render Facebook object display form.
 */
function theme_facebook_object_display_form($form) {
  $log = feeds_facebook_get_log();
  $log->debug('theme_facebook_object_display_form()', $form);
  return drupal_render($form);
}
