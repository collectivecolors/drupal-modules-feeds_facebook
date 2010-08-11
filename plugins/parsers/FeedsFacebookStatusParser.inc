<?php
// $Id$
/**
 * @file
 *  Feeds parser class for Facebook statuses.
 */

feeds_facebook_require('FbGraph.php', 'fbGraph', TRUE);
feeds_facebook_require('FeedsFacebookUserSubmissionParser.inc', 'plugins/parsers');

fb_graph_require('FbStatus', 'object');

/**
 * Class definition for Facebook Status Parser.
 */
class FeedsFacebookStatusParser extends FeedsFacebookUserSubmissionParser {
  
  /*****************************************************************************
   * Parser properties
   */
  
  /**
   * Override: parent::getClass().
   * 
   * Overriden in subclasses.
   */
  protected function getClass() {
    return 'FbStatus';
  }
  
  /*****************************************************************************
   * Parser utilities
   */
  
  /**
   * Override: parent::amendItem().
   */
  /*protected function amendItem(FbStatus $object, &$item) {
    
  }*/
  
  //----------------------------------------------------------------------------

  /**
   * Override: parent::getMappingSources().
   */
  public function getMappingSources() {
    $map = parent::getMappingSources();

    // Status messages are lacking some common fields.
    unset($map['name']);
    unset($map['link']);
    
    return $map;
  }
}
