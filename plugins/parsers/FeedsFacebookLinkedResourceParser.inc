<?php
// $Id$
/**
 * @file
 *  Feeds parser class for Facebook linked resources.
 */

feeds_facebook_require('FbGraph.php', 'fbGraph', TRUE);
feeds_facebook_require('FeedsFacebookUserSubmissionParser.inc', 'plugins/parsers');

fb_graph_require('FbLinkedResource', 'object');

/**
 * Class definition for Facebook Linked Resource Parser.
 */
class FeedsFacebookLinkedResourceParser extends FeedsFacebookUserSubmissionParser {
  
  /*****************************************************************************
   * Parser properties
   */
  
  /**
   * Override: parent::getClass().
   * 
   * Overriden in subclasses.
   */
  protected function getClass() {
    return 'FbLinkedResource';
  }
  
  /*****************************************************************************
   * Parser utilities
   */
  
  /**
   * Override: parent::amendItem().
   */
  protected function amendItem(FbLinkedResource $object, &$item) {
    parent::amendItem($object, $item);
    
    $item['caption']     = $object->getCaption();
    $item['description'] = $object->getDescription();  
  }
  
  //----------------------------------------------------------------------------

  /**
   * Override: parent::getMappingSources().
   */
  public function getMappingSources() {
    $map = parent::getMappingSources();
        
    return $map + array(
      'caption' => array(
        'name' => t('Link caption'),
        'description' => t('Link caption of the Facebook object.'),
      ),
      'description' => array(
        'name' => t('Link description'),
        'description' => t('Link description of the Facebook object.'),
      ),
    );
  }
}
