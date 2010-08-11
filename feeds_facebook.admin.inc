<?php
// $Id$

/**
 * Set Facebook authorization information.
 */
function feeds_facebook_admin_settings() {  
  $form['facebook_api_key'] = array(
    '#type' => 'textfield',
  	'#title' => t('Facebook API key'),
    '#default_value' => variable_get('facebook_api_key', ''),
  );
  $form['facebook_app_secret'] = array(
    '#type' => 'textfield',
  	'#title' => t('Facebook application secret'),
    '#default_value' => variable_get('facebook_app_secret', ''),
  );
  return system_settings_form($form);
}
