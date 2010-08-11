<?php

feeds_facebook_require('FbGraph.php', 'fbGraph', TRUE);
fb_graph_require('StaticCache', 'cache', TRUE);

/**
 * Static cache
 */
class DrupalCache extends StaticCache {
  
  /**
   * Constructor.
   *
   * Initialize class variables.
   */
  public function __construct($cache_table = '') {
    $this->setTable($cache_table);
  }
  
  /*****************************************************************************
   * Cache properties.
   */
  
  /**
   * Drupal cache table.
   */
  protected $cache_table;
  
  /**
   * Get the cache table.
   */
  public function getTable() {
  	return $this->cache_table;
  }
  
  /**
   * Set the cache table.
   */
  public function setTable($cache_table) {
  	$this->cache_table = ($cache_table ? $cache_table : 'cache');
  }
  
  //----------------------------------------------------------------------------
  
  /**
   * Return an entry from the cache or NULL if nothing was found.
   */
  public function get($cache_id) {
  	// First try local static cache.
    $data = parent::get($cache_id);
    
    if (is_null($data)) {
    	// If that fails, try persistent cache.
    	$cached = cache_get($cache_id, $this->getTable());
    	
    	if (is_object($cached) && $cached->data) {
        $data = $cached->data;
        parent::add($cache_id, $data); // Statically cached so expiration doesn't matter.
    	}
    }
    return $data;
  }
  
  /**
   * Add an entry to the cache.
   * 
   * If an object with the same cache id already exists, it is replaced.
   */
  public function add($cache_id, $data, $expire = NULL) {
  	parent::add($cache_id, $data, $expire);
    
  	$expire = ($expire ? $expire : CACHE_PERMANENT);
  	cache_set($cache_id, $data, $this->getTable(), $expire);
  }
  
  /**
   * Delete a cache entry.
   * 
   * The first parameter of this function should be a cache id. At this time
   * regular expressions or patterns are not supported in this delete method
   * due to difficulties reconciling the two different cache patterns.
   */
  public function delete($cache_id, $is_pattern = FALSE) {
  	parent::delete($cache_id, FALSE);
  	cache_clear_all($cache_id, $this->getTable(), FALSE);
  }
  
  /**
   * Clear the entire cache.
   */
  public function clear() {
  	parent::clear();
    cache_clear_all('*', $this->getTable(), TRUE);
  }
}