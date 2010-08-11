<?php
// $Id$

/**
 * @file
 * Home of the FeedsFacebookGraphFetcher.
 */

feeds_facebook_require('FbGraph.php', 'fbGraph', TRUE);
fb_graph_require('FbObject', 'object', TRUE);

/**
 * Definition of the import batch object created on the fetching stage by
 * FeedsFacebookGraphFetcher.
 */
class FeedsFacebookBatch extends FeedsImportBatch {

  /**
   * Constructor.
   */
  public function __construct($request_id, FeedsConfigurable $importer) {
  	parent::__construct();

  	$log = feeds_facebook_get_log();
    $log->debug('new FeedsFacebookBatch()', $request_id);

    $this->setGraph(feeds_facebook_get_graph());

    if ($request_id) {
      list($id, $connection) = $this->graph->splitRequestId($request_id);

      $this->setId($id);
      $this->setConnection($connection);
    }
    $this->setImporter($importer);

    $this->title = "Facebook feed"; // Dumy text to keep feeds from throwing a validation error.
  }

  /*****************************************************************************
   * FeedsFacebookBatch properties
   */

  protected $importer;

  public function setImporter(FeedsConfigurable $importer) {
    $this->importer = $importer;
  }

  //----------------------------------------------------------------------------

  protected $graph;

  public function setGraph(FbGraph $graph) {
    if (is_null($graph)) {
      throw new Exception("A FbGraph instance is required to request Facebook data.");
    }
    $this->graph = $graph;
  }

  //----------------------------------------------------------------------------

  protected $request_id;

  public function getRequestId() {
  	$this->request_id = $request_id;
  }

  //----------------------------------------------------------------------------

  protected $id;
  
  public function setId($id) {
  	$this->id = $id;
  }
  
  //----------------------------------------------------------------------------
  
  public function getObject() {
    return $this->graph->request($this->id, '', NULL, $this->getExpiration());  
  }

  //----------------------------------------------------------------------------

  protected $connection;

  public function setConnection($connection) {
  	$this->connection = (is_string($connection) ? $connection : '');
  }
  
  //----------------------------------------------------------------------------
  
  protected function getExpiration() {
    $log = feeds_facebook_get_log();
  	$log->debug('getExpiration()');
  	
    if (!is_null($this->importer)) {
  		$expire_period = $this->importer->getSchedulePeriod('import');
  		// Import period can be set to -1 for NEVER.
  		$expire_period = ($expire_period > 0 ? $expire_period : 0);
  		$log->debug("Expire period: $expire_period");

  		// Make sure we expire before we run the recurring process of checking
  		// for updates, but don't expire the cache too quickly.
  		$expire = time() + (int)($expire_period - ($expire_period / 10));
  		$log->debug("Expiration: $expire");
  	}
  	return $expire;    
  }

  //----------------------------------------------------------------------------

  /**
   * Implementation of FeedsImportBatch::getRaw();
   */
  public function getRaw() {
  	$log = feeds_facebook_get_log();
  	$log->debug('getRaw()', $this->id, $this->connection);

  	if (!$this->id) {
  	  return serialize(array()); // So parsers can expect standardized output.
  	}
  	$data = serialize($this->graph->request($this->id, $this->connection, NULL, $this->getExpiration()));
  	$log->debug("Data returned:", $data);
  	return $data;
  }
}


/**
 * Fetches Facebook activity stream entries for a user, group, or page.
 *
 * This class basically exists to make creating feed nodes that fetch Facebook
 * information easier for the end user.
 */
class FeedsFacebookGraphFetcher extends FeedsFetcher {

  /**
   * Constructor.
   *
   * Initialize class variables.
   */
  protected function __construct($id) {
    // This needs to come before the parent constructor.
    $this->graph = feeds_facebook_get_graph();
    parent::__construct($id);
  }

  /*****************************************************************************
   * Fetcher properties
   */

  /**
   * Holds the graph connectivity class used to query Facebook.
   */
  protected $graph;

  /*****************************************************************************
   * Fetcher importer configuration form
   */

  /**
   * Override parent::configDefaults().
   */
  public function configDefaults() {
    $defaults = parent::configDefaults();

    $defaults['search_url']      = FbGraph::DEFAULT_SEARCH_URL;
    $defaults['search_results']  = 30;
    $defaults['expiration_time'] = 3600;

    return $defaults;
  }

  /**
   * Override parent::configForm().
   */
  public function configForm(&$form_state) {
    $form = parent::configForm($form_state);

    $form['search_url'] = array(
      '#type' => 'textfield',
      '#title' => t('Facebook object search endpoint'),
      '#description' => t('If you change this URL be sure that the same query parameters are used.  If you empty this input then submit the default URL will be used'),
      '#default_value' => $this->config['search_url'],
    );

    $form['search_results'] = array(
      '#type' => 'textfield',
      '#title' => t('Maximum number of results to return from Facebook searches'),
      '#description' => t('Setting this to a high number does not guarantee that amount of results.  It all depends upon Facebooks allowed limit.'),
      '#default_value' => $this->config['search_results'],
    );

    $form['expiration_time'] = array(
      '#type' => 'textfield',
      '#title' => t('Facebook cache expiration time'),
      '#description' => t('After the specified number of seconds, the Facebook object is refreshed.'),
      '#default_value' => $this->config['expiration_time'],
    );

    return $form;
  }

  /**
   * Override parent::configFormValidate().
   */
  public function configFormValidate(&$values) {
    parent::configFormValidate($values);

    if (!$values['search_url']) { // TODO: This should really be a check for a valid URL.
      $defaults = $this->configDefaults();
      $values['search_url'] = $defaults['search_url'];
    }

    // TODO: Test validation.
    if (!$values['search_results']
      || !is_numeric($values['search_results'])
      || $values['search_results'] < 0) {
      form_set_error('search_results', t('The number of search results returned must be a positive number.'));
    }

    // TODO: Test validation.
    if (!$values['expiration_time']
      || !is_numeric($values['expiration_time'])
      || $values['expiration_time'] < 0) {
      form_set_error('expiration_time', t('Expiration time (in seconds) must be a positive number.'));
    }
  }

  //----------------------------------------------------------------------------

  /**
   * Override parent::setConfig().
   */
  public function setConfig($config) {
    parent::setConfig($config);
    $this->graph->setSearchUrl($this->config['search_url']);
  }

  /**
   * Override parent::addConfig().
   */
  public function addConfig($config) {
    parent::addConfig($config);
    $this->graph->setSearchUrl($this->config['search_url']);
  }

  /*****************************************************************************
   * Fetcher source configuration settings.
   */

  /**
   * Override: parent::hasSourceConfig().
   */
  public function hasSourceConfig() {
    return TRUE; // Eliminate calls to $this->sourceForm().
  }

  //----------------------------------------------------------------------------

  /**
   * Override: parent::sourceDefaults().
   */
  public function sourceDefaults() {
    // No default values for now.
    // And we eliminate calls to $this->sourceForm().
    return array();
  }

  /*****************************************************************************
   * Fetcher source form
   */

  /**
   * Holds an instance copy of the feed form saved/default variables.
   *
   * This allows us to reference these values in multiple methods without having
   * to pass them around.  Since feeds module set the feeds form fieldset to
   * "tree" we can not easily access variables in other sections unless we pass
   * the global object around.  This variable solves our problems.
   */
  protected $source_values;

  /**
   * Expose source form.
   */
  public function sourceForm($source_config) {
  	$log = feeds_facebook_get_log();
    $log->debug('sourceForm()', $source_config);

    if ($this->validated_values) {
      $source_config = $this->validated_values;
    }
    $log->debug('Validated values:', $this->validated_values);

    static $files_added;
    if (!isset($files_added)) {
      $module_path = drupal_get_path('module', 'feeds_facebook');
      drupal_add_js("$module_path/js/feeds_facebook_ahah.js", 'footer');

      drupal_add_css("$module_path/css/feeds_facebook_form.css");
      drupal_add_css("$module_path/css/feeds_facebook_object.css");
      $files_added = TRUE;
    }
    $this->source_values = $source_config;

  	$search_results = $this->searchForm($form, -10);
  	$this->objectSelectorForm($form, $search_results);

    $log->debug('Graph Fetcher form:', $form);
    return $form;
  }

  //----------------------------------------------------------------------------

  /**
   * Holds an instance copy of the feed form submitted variables.
   *
   * This allows us to reference these values in multiple methods without having
   * to pass them around.  Since feeds module set the feeds form fieldset to
   * "tree" we can not easily access variables in other sections unless we pass
   * the global object around.  This variable solves our problems.
   */
  protected $validated_values;

  /**
   * Override parent::sourceFormValidate().
   */
  public function sourceFormValidate(&$values) {
    $log = feeds_facebook_get_log();
    $log->debug('sourceFormValidate()', $values, $_POST);

    $this->validated_values = &$values;

    if ($this->validateSearchForm($values)) {
      $this->validateObjectSelectorForm($values);
    }
    $log->debug('Amended values:', $values);
  }

  //----------------------------------------------------------------------------

  /**
   * Element ids of the search form wrappers.
   */
  const SEARCH_FORM_ID   = 'facebook_search_form';

  /**
   * Name of the search forms.
   */
  const SEARCH_FORM_NAME   = 'search';

  /**
   * Search field names.
   */
  const SEARCH_QUERY_NAME = 'search_query';
  const SEARCH_TYPE_NAME  = 'search_type';

  /**
   * Attach a search form onto an existing feeds form.
   */
  protected function searchForm(&$form, $weight = 0) {
  	$log = feeds_facebook_get_log();
    $log->debug('searchForm()', $form, $weight);

    $values = &$this->source_values[self::SEARCH_FORM_NAME];
    $log->debug('Search values:', $values);

  	// Container for the search form.
    $form[self::SEARCH_FORM_NAME] = array(
      '#prefix' => '<div id="' . self::SEARCH_FORM_ID . '" style="clear: both;">',
      '#suffix' => '</div>',
      '#theme' => 'facebook_search_form',
      '#weight' => $weight,
    );

    // Search query.
    $values[self::SEARCH_QUERY_NAME] = ($values[self::SEARCH_QUERY_NAME]
                                     ? $values[self::SEARCH_QUERY_NAME]
                                     : '');

  	$form[self::SEARCH_FORM_NAME][self::SEARCH_QUERY_NAME] = array(
      '#type' => 'textfield',
  	  '#id' => self::SEARCH_QUERY_NAME,
      '#title' => t('Search query'),
      '#default_value' => $values[self::SEARCH_QUERY_NAME],
      '#prefix' => '<div style="float: left;">',
      '#suffix' => '</div>',
    );

    $search_options = $this->graph->getSearchTypes();
    unset($search_options[FbGraph::SEARCH_TYPE_POST]); // Posts not supported right now.

    // Facebook search type.
    $values[self::SEARCH_TYPE_NAME] = ($values[self::SEARCH_TYPE_NAME]
                                    ? $values[self::SEARCH_TYPE_NAME]
                                    : FbGraph::SEARCH_TYPE_PAGE);

    $form[self::SEARCH_FORM_NAME][self::SEARCH_TYPE_NAME] = array(
      '#type' => 'select',
      '#id' => self::SEARCH_TYPE_NAME,
      '#title' => t('Facebook type'),
      '#options' => $search_options,
      '#default_value' => $values[self::SEARCH_TYPE_NAME],
      '#prefix' => '<div style="float: left;">',
      '#suffix' => '</div>',
    );

    // Search submit. (AJAX enabled)
    $form[self::SEARCH_FORM_NAME]['search_submit'] = array(
      '#type' => 'submit',
      '#id' => 'search_submit',
      '#value' => t('Search Facebook'),
      '#submit' => array('feeds_facebook_nojs_submit'),
      '#ahah' => array(
        'path' => 'feeds_fb/search/ahah/feeds,' . __CLASS__ . ',' . self::OBJECT_FORM_NAME,
        'wrapper' => self::OBJECT_SELECTOR_ID,
        'method' => 'replace',
        'effect' => 'fade',
      ),
      '#prefix' => '<div style="float: left;">',
      '#suffix' => '</div>',
    );

    $log->debug('Search form:', $form[self::SEARCH_FORM_NAME]);

    // If we have an existing query, get the search results, else NULL.
    return $this->getSearchResults($values[self::SEARCH_QUERY_NAME],
                                   $values[self::SEARCH_TYPE_NAME]);
  }

  /**
   * Validate a search form submission.
   */
  protected function validateSearchForm(&$values) {
    $log = feeds_facebook_get_log();
    $log->debug('validateSearchForm()', $values);

    if ($_POST['op'] == t('Search Facebook')) { // I hate to do this but I don't see any other way right now.
      $search_values = &$values[self::SEARCH_FORM_NAME];
      $log->debug('Search values:', $search_values);

      if (!$search_values[self::SEARCH_QUERY_NAME]) {
        form_set_error('feeds][' . self::SEARCH_QUERY_NAME, t('A search query is required to search Facebook.'));
      }
      unset($this->validated_values[self::OBJECT_FORM_NAME]);
      return FALSE;
    }
    return TRUE;
  }

  //----------------------------------------------------------------------------

  /**
   * Element ids of the object selector form wrappers.
   */
  const OBJECT_SELECTOR_ID = 'facebook_object_selector';
  const OBJECT_CANVAS_ID   = 'facebook_object_canvas';

  /**
   * Name of the object selection form.
   */
  const OBJECT_FORM_NAME   = 'object_selection';
  const OBJECT_CANVAS_NAME = 'canvas';

  /**
   * Object selector field names.
   */
  const FACEBOOK_ID_NAME = 'facebook_id';

  /**
   * Attach a object selector form onto an existing feeds form.
   */
  protected function objectSelectorForm(&$form, $search_results, $weight = 0) {
  	$log = feeds_facebook_get_log();
    $log->debug('objectSelectorForm()', $form, $search_results, $weight);

    $values = &$this->source_values[self::OBJECT_FORM_NAME];
    $log->debug('Object values:', $values);

    // Container for the object selector form.
    $form[self::OBJECT_FORM_NAME] = array(
      '#prefix' => '<div id="' . self::OBJECT_SELECTOR_ID . '" style="clear: both;">',
      '#suffix' => '</div>',
      '#theme' => 'facebook_object_selector_form',
      '#weight' => $weight,
    );
    // Make sure our wrapper gets transmitted to the client.
    $form[self::OBJECT_FORM_NAME]['placeholder'] = array(
      '#type' => 'hidden',
      '#value' => 'Placeholder',
    );

    if ($values[self::FACEBOOK_ID_NAME] || is_array($search_results)) {
      if (count($search_results)) {
        $object_options = array();
        foreach ($search_results as $item) {
          if ($item instanceof FbObject) {
            $object_options[$item->getId()] = $item->getName();
          }
        }
        $default_id = key($object_options);
        $log->debug("Object options (Default: $default_id) :", $object_options);

        $values[self::FACEBOOK_ID_NAME] = ($values[self::FACEBOOK_ID_NAME]
                                          && in_array($values[self::FACEBOOK_ID_NAME], array_keys($object_options))
                                        ? $values[self::FACEBOOK_ID_NAME]
                                        : $default_id);

        $form[self::OBJECT_FORM_NAME][self::FACEBOOK_ID_NAME] = array(
        	'#type' => 'select',
        	'#id' => self::FACEBOOK_ID_NAME,
        	'#title' => t('Facebook @entity', array('@entity' => $this->source_values[self::SEARCH_FORM_NAME][self::SEARCH_TYPE_NAME])),
        	'#options' => $object_options,
        	'#default_value' => $values[self::FACEBOOK_ID_NAME],
        	'#ahah' => array(
          	'path' => 'feeds_fb/search/ahah/feeds,'
            	      . __CLASS__ . ',' . self::OBJECT_FORM_NAME . ',canvas',
          	'event' => 'change',
          	'wrapper' => self::OBJECT_CANVAS_ID,
          	'method' => 'replace',
          	'effect' => 'fade',
          ),
        	'#weight' => -10,
        );

        // Container for object subforms.
        $form[self::OBJECT_FORM_NAME][self::OBJECT_CANVAS_NAME] = array(
        	'#prefix' => '<div id="' . self::OBJECT_CANVAS_ID . '" style="clear: both;">',
        	'#suffix' => '</div>',
        	'#theme' => 'facebook_object_canvas_form',
        	'#weight' => -5,
        );
        // Make sure our wrapper gets transmitted to the client.
        $form[self::OBJECT_FORM_NAME][self::OBJECT_CANVAS_NAME]['placeholder'] = array(
        	'#type' => 'hidden',
        	'#value' => 'Placeholder',
        );

        $object_canvas = &$form[self::OBJECT_FORM_NAME][self::OBJECT_CANVAS_NAME];
        $object = $this->graph->request($values[self::FACEBOOK_ID_NAME], '', 1, $this->config['expiration_time']);

        // Add some object subforms.
        $this->feedSelectorForm($object_canvas, $values[self::OBJECT_CANVAS_NAME], $object, -10);
        $this->objectDisplay($object_canvas, $object, -5);
      }
      else {
        // No search results.
        $form[self::OBJECT_FORM_NAME]['no_results'] = array(
      		'#type' => 'markup',
      		'#value' => t('Facebook search returned no results.'),
        );  
      }
    }
    $log->debug('Object selector form:', $form[self::OBJECT_FORM_NAME]);
  }

  /**
   * Validate an object selection form submission.
   */
  protected function validateObjectSelectorForm(&$values) {
    $log = feeds_facebook_get_log();
    $log->debug('validateObjectSelectorForm()', $values);

    $object_values = &$values[self::OBJECT_FORM_NAME];
    $log->debug('Object values:', $object_values);

    if (!$object_values[self::FACEBOOK_ID_NAME]) {
      form_set_error('', t('A valid Facebook id must be specified to submit this feed.  If you have not searched yet, do that first.  You will then be presented with Facebook feed choices.'));
    }
    else {
      $this->validateFeedSelectorForm($object_values);
    }
  }

  //----------------------------------------------------------------------------

  /**
   * Element id of the feed selector form wrapper.
   */
  const FEED_SELECTOR_ID = 'facebook_feed_selector';

  /**
   * Name of the feed selection form.
   */
  const FEED_FORM_NAME = 'feed_selection';

  /**
   * Feed selector field names.
   */
  const FACEBOOK_CONNECTION_NAME = 'facebook_connection';

  /**
   * Attach a feed selector form onto an existing feeds form.
   */
  protected function feedSelectorForm(&$form, &$parent_values, $object, $weight = 0) {
    $log = feeds_facebook_get_log();
    $log->debug('feedSelectorForm()', $form, $parent_values, $object, $weight);

    $values = &$parent_values[self::FEED_FORM_NAME];
    $log->debug('Feed values:', $values);

    // Container for the feed selector form.
    $form[self::FEED_FORM_NAME] = array(
      '#prefix' => '<div id="' . self::FEED_SELECTOR_ID . '" style="clear: both;">',
      '#suffix' => '</div>',
      '#theme' => 'facebook_feed_selector_form',
      '#weight' => $weight,
    );

    if ($object instanceof FbObject) {
      $connection_options = drupal_map_assoc($object->getConnections(), 'ucfirst');
    }
    if (!is_array($connection_options)) {
      $connection_options = array('feed' => 'Feed');
    }
    $default_connection = key($connection_options);
    $log->debug("Feed options (Default: $default_connection) :", $connection_options);

    $values[self::FACEBOOK_CONNECTION_NAME] = ($values[self::FACEBOOK_CONNECTION_NAME]
                                            ? $values[self::FACEBOOK_CONNECTION_NAME]
                                            : $default_connection);

    $form[self::FEED_FORM_NAME][self::FACEBOOK_CONNECTION_NAME] = array(
      '#type' => 'select',
      '#id' => self::FACEBOOK_CONNECTION_NAME,
      '#title' => t('@entity connection', array('@entity' => ucfirst($this->source_values[self::SEARCH_FORM_NAME][self::SEARCH_TYPE_NAME]))),
      '#options' => $connection_options,
      '#default_value' => $values[self::FACEBOOK_CONNECTION_NAME],
    );
    $log->debug('Feed selector form:', $form[self::FEED_FORM_NAME]);
  }

  /**
   * Validate an object feed selection form submission.
   */
  protected function validateFeedSelectorForm(&$values) {
    $log = feeds_facebook_get_log();
    $log->debug('validateFeedSelectorForm()', $values);

    $feed_values = &$values[self::OBJECT_CANVAS_NAME][self::FEED_FORM_NAME];
    $log->debug('Feed values:', $feed_values);

    $facebook_id = $values[self::FACEBOOK_ID_NAME];
    $log->debug('Facebook id:', $facebook_id);

    $facebook_connection = $feed_values[self::FACEBOOK_CONNECTION_NAME];
    $log->debug('Facebook connection:', $facebook_connection);

    if ($facebook_id && $facebook_connection) {
      $request_id = $this->graph->getRequestId($facebook_id, $facebook_connection);
      $log->debug('Request id:', $request_id);

      $this->validated_values['source'] = $request_id;
    }
  }

  //----------------------------------------------------------------------------

  /**
   * Element id of the object display form wrapper.
   */
  const OBJECT_DISPLAY_ID = 'facebook_object_display';

  /**
   * Name of the object display form.
   */
  const OBJECT_DISPLAY_NAME = 'object_display';

  /**
   * Display information about a Facebook object.
   */
  protected function objectDisplay(&$form, FbObject $object, $weight = 0) {
    $log = feeds_facebook_get_log();
    $log->fdebug('objectDisplay()', $object);

    // Container for the feed selector form.
    $form[self::OBJECT_DISPLAY_NAME] = array(
      '#prefix' => '<div id="' . self::OBJECT_DISPLAY_ID . '" style="clear: both;">',
      '#suffix' => '</div>',
      '#theme' => 'facebook_object_display_form',
      '#weight' => $weight,
    );
    // Get object type so we can select theme function.
    $object_type = $object->getType();
    $log->debug('Original object type:', $object_type);

    if (!in_array($object_type, array('user', 'group', 'event', 'page'))) {
      $object_type = 'object';
    }
    $log->fdebug('Theme object type:', $object_type);

    $form[self::OBJECT_DISPLAY_NAME]['info'] = array(
    	'#type' => 'markup',
    	'#value' => '<div class="info">' . theme("facebook_$object_type", $object) . '</div>',
    );
    $log->fdebug('Object display form:', $form[self::OBJECT_DISPLAY_NAME]);
  }

  /*****************************************************************************
   * Fetcher downloader
   */

  /**
   * Fetch new Facebook entries.
   */
  public function fetch(FeedsSource $source) {
  	$log = feeds_facebook_get_log();
    $log->debug('fetch()', $source);

    $source_config = $source->getConfigFor($this);
    $log->debug('Source configuration:', $source_config);

    $log->debug('Feed importer: ' . $this->id);
    $importer = feeds_importer($this->id);

    // TODO: Integrate with Facebooks data push service as soon as it supports
    // more Facebook object types.
    return new FeedsFacebookBatch($source_config['source'], $importer);
  }

  /*****************************************************************************
   * Utilities
   */

  /**
   * Return Facebook results for a given query.
   */
  protected function getSearchResults($query, $type) {
    $log = feeds_facebook_get_log();
    $log->debug('getSearchResults()', $query, $type);

    static $search_results;

    if ($query) {
    	$log->debug("Search query: $query $type");

      $search_id = $this->graph->getRequestId($query, $type);
      $log->debug("Search id: $search_id");

      if (!isset($search_results[$search_id])) {
        $search_results[$search_id] = $this->graph->search($query, $type, 30, 0, 1, $this->config['expiration_time']);
      }
      $log->debug('Search results:', $search_results[$search_id]);
      return $search_results[$search_id];
    }
    $log->debug('No query to run.');
    return NULL;
  }

}