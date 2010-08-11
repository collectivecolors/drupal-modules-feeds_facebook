<?php
// $Id$
/**
 * @file
 *  Feeds base parser class for Facebook user submissions.
 */

feeds_facebook_require('FbGraph.php', 'fbGraph', TRUE);

fb_graph_require('FbObject', 'object');
fb_graph_require('FbUserSubmission', 'object');
fb_graph_require('FbUser', 'object');

/**
 * Class definition for Facebook User Submission Parser.
 */
class FeedsFacebookUserSubmissionParser extends FeedsParser {
  
  /*****************************************************************************
   * Parser properties
   */
  
  /**
   * Return a class to test for when evaluating the feed results.
   * 
   * Overriden in subclasses.
   */
  protected function getClass() {
    return 'FbUserSubmission';
  }
  
  /*****************************************************************************
   * Parser utilities
   */
  
  /**
   * Parse the mapping sources provided by this parser.
   */
  public function parse(FeedsImportBatch $batch, FeedsSource $source) {
  	$log = feeds_facebook_get_log();
    $log->debug('parse()', $batch, $source);
    
    if (!($batch instanceof FeedsFacebookBatch)) {
      return;
    }
    // Set up initial variables needed to parse Facebook results.    
    $log->debug('Original result:', $batch->getRaw());
    $results = unserialize($batch->getRaw());
    $log->debug('Unserialized result:', $results);
    
    $parent_object = $batch->getObject();
    $log->debug('Parent object:', $parent_object);
    
    if ($results && !is_array($results)) {
      $results = array($results);
    }
    $class = $this->getClass();
    
    // Turn Facebook feed items into parsed variables ready for processor mapping.
    foreach ($results as $object) {
    	$log->debug('Result object:', $object);
      if ($object instanceof $class) {
        $item = array();
        
        // Parent object properties.
        $item['parent_type']        = ucfirst($parent_object->getType());
        $item['parent_id']          = $parent_object->getId();
        $item['parent_name']        = $parent_object->getName();
        $item['parent_link']        = $parent_object->getLink();
        $item['parent_connections'] = $parent_object->getConnections();
        
        // FbObject properties.
        $item['type']        = ucfirst($object->getType());
        $item['id']          = $object->getId();
        $item['name']        = $object->getName();
        $item['link']        = $object->getLink();
        $item['connections'] = $object->getConnections();
        
        // FbSocialObject properties.
        $item['likes'] = $object->getLikes();        
        
        // FbUserSubmission properties.
        $from = $object->getFrom();
        if ($from instanceof FbObject) {
          $item['from_connections'] = $from->getConnections();    
          $item['from_id']          = $from->getId();
          $item['from_name']        = $from->getName();
          $item['from_link']        = $from->getLink();
          if ($from instanceof FbUser) {
            $item['from_first_name']  = $from->getFirstName();
            $item['from_last_name']   = $from->getLastName();
          }          
        }
        
        $to = $object->getTo();
        if (is_array($to)) {
        	foreach ($to as $recipient) {
        		if ($recipient instanceof FbObject) {
        			$item['to_ids'][]   = $recipient->getId();
              $item['to_names'][] = $recipient->getName();
              $item["to_links"][] = $recipient->getLink();	
        		}
        	}
        }
        
        $item['message']      = $object->getMessage();
        $item['icon']         = $object->getIcon();
        $item['created_time'] = $object->getCreatedTime();
        $item['updated_time'] = $object->getUpdatedTime();
        $item['attribution']  = $object->getAttribution();
        
        $actions = $object->getActions();
        foreach ($actions as $name => $link) {
          $item['action_names'][] = $name;
          $item['action_links'][] = $link;
        }
        
        // Give submodules a chance to amend the returned item object.
        $this->amendItem($object, $item);
        
        $batch->addItem($item);
        $log->debug('Added item:', $item);
      }        
    }   
  }
  
  //----------------------------------------------------------------------------
  
  /**
   * Add or remove item elements from a parsed Facebook object item.
   * 
   * Override in subclasses.
   */
  protected function amendItem(FbObject $object, &$item) { }
  
  //----------------------------------------------------------------------------

  /**
   * Add the extra mapping sources provided by this parser.
   */
  public function getMappingSources() {    
    return array(
      // Parent object properties.
      'parent_type' => array(
        'name' => t('Parent type'),
        'description' => t('Type of the parent Facebook object.'),
      ),
      'parent_id' => array(
        'name' => t('Parent ID'),
        'description' => t('Global unique identifier of the parent Facebook object.'),
      ),
      'parent_name' => array(
        'name' => t('Parent name'),
        'description' => t('Name of the Facebook parent object.'),
      ),
      'parent_link' => array(
        'name' => t('Parent link'),
        'description' => t('Link to the Facebook parent object.'),
      ),
      'parent_connections' => array(
        'name' => t('Parent connections'),
        'description' => t('All available feeds for this Facebook parent object.'),
      ),
      
      // FbObject properties.
      'type' => array(
        'name' => t('Object type'),
        'description' => t('Type of the Facebook object.'),
      ),
      'id' => array(
        'name' => t('Object ID'),
        'description' => t('Global unique identifier of the Facebook object.'),
      ),
      'name' => array(
        'name' => t('Object name'),
        'description' => t('Name of the Facebook object.  Not all Facebook types have a name.'),
      ),
      'link' => array(
        'name' => t('Object reference link'),
        'description' => t('Link to the Facebook object.  Not all Facebook types have a link.'),
      ),
      'connections' => array(
        'name' => t('Object connections'),
        'description' => t('All available feeds for this Facebook object.'),
      ),
      
      // FbSocialObject properties.
      'likes' => array(
        'name' => t('Object likes'),
        'description' => t('Number of times this Facebook object has been liked by Facebook users.'),
      ),
      
      // FbUserSubmission properties.
      'message' => array(
        'name' => t('Object message'),
        'description' => t('Facebook object message.'),
      ),
      'created_time' => array(
        'name' => t('Object created time'),
        'description' => t('Facebook object creation time.'),
      ),
      'updated_time' => array(
        'name' => t('Object updated time'),
        'description' => t('Facebook object update time.'),
      ),
      'attribution' => array(
        'name' => t('Facebook application attribution'),
        'description' => t('Application attribution label.'),
      ),
      'icon' => array(
        'name' => t('Object icon'),
        'description' => t('Facebook object icon.'),
      ),
      'from_id' => array(
        'name' => t('Object ID of creator'),
        'description' => t('Global unique identifier of the Facebook object creator.'),
      ),
      'from_name' => array(
        'name' => t('Name of creator'),
        'description' => t('Facebook object name.'),
      ),
      'from_connections' => array(
        'name' => t('Connections of creator'),
        'description' => t('All available feeds for the Facebook object creator.'),
      ),
      'from_first_name' => array(
        'name' => t('User first name of creator'),
        'description' => t('First name of the Facebook user (if the creator is a user).'),
      ),
      'from_last_name' => array(
        'name' => t('User last name of creator'),
        'description' => t('Last name of the Facebook user (if the creator is a user).'),
      ),
      'from_link' => array(
        'name' => t('Link to creator'),
        'description' => t('Profile link to Facebook object creator.'),
      ),
      'to_ids' => array(
        'name' => t('Object IDs of recipients'),
        'description' => t('Global unique Identifier of the Facebook user.'),
      ),
      'to_names' => array(
        'name' => t('Object names of recipients'),
        'description' => t('Facebook object names.'),
      ),
      'to_links' => array(
        'name' => t('Links to message recipients'),
        'description' => t('Profile links to Facebook recipients.'),
      ),
      'action_names' => array(
        'name' => t('Names of object actions'),
        'description' => t('Names of potential actions for this Facebook object.'),
      ),
      'action_links' => array(
        'name' => t('Links of object actions'),
        'description' => t('Links to potential actions for this Facebook object.'),
      ),
    );
  }
}
