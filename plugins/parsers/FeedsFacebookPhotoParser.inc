<?php
// $Id$
/**
 * @file
 *  Feeds parser class for Facebook photos.
 */

feeds_facebook_require('FbGraph.php', 'fbGraph', TRUE);
feeds_facebook_require('FeedsFacebookImageResourceParser.inc', 'plugins/parsers');

fb_graph_require('FbPhoto', 'object');

/**
 * Class definition for Facebook Photo Parser.
 */
class FeedsFacebookPhotoParser extends FeedsFacebookImageResourceParser {
  
  /*****************************************************************************
   * Parser properties
   */
  
  /**
   * Override: parent::getClass().
   * 
   * Overriden in subclasses.
   */
  protected function getClass() {
    return 'FbPhoto';
  }
  
  /*****************************************************************************
   * Parser utilities
   */
  
  /**
   * Override: parent::amendItem().
   */
  /*protected function amendItem(FbPhoto $object, &$item) {
    
  }*/
  
  //----------------------------------------------------------------------------

  /**
   * Override: parent::getMappingSources().
   */
  /*public function getMappingSources() {
    $map = parent::getMappingSources();    
    return $map + array(
    
    );
  }*/
}
