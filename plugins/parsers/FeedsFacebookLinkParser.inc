<?php
// $Id$
/**
 * @file
 *  Feeds parser class for Facebook links.
 */

feeds_facebook_require('FbGraph.php', 'fbGraph', TRUE);
feeds_facebook_require('FeedsFacebookImageResourceParser.inc', 'plugins/parsers');

fb_graph_require('FbLink', 'object');

/**
 * Class definition for Facebook Link Parser.
 */
class FeedsFacebookLinkParser extends FeedsFacebookImageResourceParser {
  
  /*****************************************************************************
   * Parser properties
   */
  
  /**
   * Override: parent::getClass().
   * 
   * Overriden in subclasses.
   */
  protected function getClass() {
    return 'FbLink';
  }
  
  /*****************************************************************************
   * Parser utilities
   */
  
  /**
   * Override: parent::amendItem().
   */
  /*protected function amendItem(FbLink $object, &$item) {
    
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
