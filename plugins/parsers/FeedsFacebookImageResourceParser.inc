<?php
// $Id$
/**
 * @file
 *  Feeds parser class for Facebook image resources.
 */

feeds_facebook_require('FbGraph.php', 'fbGraph', TRUE);
feeds_facebook_require('FeedsFacebookLinkedResourceParser.inc', 'plugins/parsers');

fb_graph_require('FbImageResource', 'object');

/**
 * Class definition for Facebook Image Resource Parser.
 */
class FeedsFacebookImageResourceParser extends FeedsFacebookLinkedResourceParser {
  
  /*****************************************************************************
   * Parser properties
   */
  
  /**
   * Override: parent::getClass().
   * 
   * Overriden in subclasses.
   */
  protected function getClass() {
    return 'FbImageResource';
  }
  
  /*****************************************************************************
   * Parser utilities
   */
  
  /**
   * Override: parent::amendItem().
   */
  protected function amendItem(FbImageResource $object, &$item) {
    parent::amendItem($object, $item);
    
    $item['picture'] = $object->getPicture();
    $item['source']  = $object->getSource();  
  }
  
  //----------------------------------------------------------------------------

  /**
   * Override: parent::getMappingSources().
   */
  public function getMappingSources() {
    $map = parent::getMappingSources();
        
    return $map + array(
      'picture' => array(
        'name' => t('Image link'),
        'description' => t('Link to the Facebook image.'),
      ),
      'source' => array(
        'name' => t('Image source'),
        'description' => t('Name of the source for the Facebook image.'),
      ),
    );
  }
}
