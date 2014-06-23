<?php

/**
 * Helper functions from Jack Kelly's sharedshelf module
 * These are included here in slightly modified form so this API module can be a stand alone one
 * Should it be incorporated into the above module the calls to these functions should be replaced by calls to the original functions
 * 
 * (ndg8f, May 9, 2014)
 */
 

/**
 * (Originally called: _media_sharedshelf_api_account_post)
 * Establish ARTStor SharedShelf connection and return cookies for future use.
 *
 * This makes a POST /account request to SharedShelf.
 *
 * @param string $email
 *   The email address used to log into SharedShelf.
 * @param string $password
 *   The password for the SharedShelf account.
 *
 * @return string
 *   The SharedShelf cookies needed for subsequent requests as name/value pairs
 *   with path options separated by semicolons.
 */
function _sharedshelf_api_account_post($email, $password) {
  $sharedshelf_cookies = '';
  $options = array(
    'data' => "email=$email&password=$password",
    'method' => 'POST',
    'timeout' => 15,
    'headers' => array('Content-Type' => 'application/x-www-form-urlencoded'),
  );
  $response = drupal_http_request(url(variable_get('sharedshelf_api_settings_host', SSHOST) . '/account'), $options);
  switch ($response->code) {
    case 200:
      $data = json_decode($response->data, TRUE);
      // Successful access is indicated in a flag passed back in the JSON data.
      if (isset($data['success']) && $data['success']) {
        if (isset($response->headers['set-cookie'])) {
          $sharedshelf_cookies = $response->headers['set-cookie'];
        }
      }
      else {
        // If the credentials did not work, first check to see if the email
        // and password are not empty.
        if (($email != '') && ($password != '')) {
          $error = check_plain($data['message']) . ' Please check your authentication settings';
        }
        else {
          $error = 'You have not set your SharedShelf credentials. Please enter your email address and password';
        }
        if ($error) {
          $error_msg = t("@error in the !settings before accessing SharedShelf content.",
              array(
                '@error' => $error,
                '!settings' => l(t('SharedShelf API administration page'), check_plain('admin/config/media/media_sharedshelf/api')),
              )
          );
          if (user_access('administer site configuration')) {
            drupal_set_message(filter_xss($error_msg), 'error');
          }
          watchdog('sharedshelf_api', $error_msg);
        }
      }
      break;

    default:
      // If we got here there is an issue, so we want to log the error.
      $error_message = isset($response->error) ? $response->error : '';
      $status_message = isset($response->status_message) ? $response->status_message : '';
      drupal_set_message($error_message . " (" . $response->code . "): " . $status_message);
  }
  return $sharedshelf_cookies;
}

/**
 *  Adapted from _media_sharedshelf_api_projects_get
 * 
 *  This is a generic function to make a API call with a certain path
 *  Defaults to /account to check if logged in.
 */
function _sharedshelf_api_get_path($path='/acccount') {
  global $_sharedshelf_cookie;
  $results = array();
  $options = array('headers' => array('Cookie' => $_sharedshelf_cookie), 'method' => 'GET');
  $sharedshelf_url = url(variable_get('sharedshelf_api_settings_host', SSHOST) . $path);
  $response = drupal_http_request($sharedshelf_url, $options);
  //dpm($response, 'response');
  switch ($response->code) {
    case 401:
    case 403:
      // For access denied issues, try authenticating again with SharedShelf
      // and get new cookies. The session may have timed out. Then try the
      // request again.
      $_sharedshelf_cookie = _sharedshelf_api_account_post(variable_get('sharedshelf_api_settings_email', ''), variable_get('sharedshelf_api_settings_password', ''));
      $options['headers']['Cookie'] = $_sharedshelf_cookie;
      $response = drupal_http_request($sharedshelf_url, $options);
    case 302:
      drupal_goto($sharedshelf_url);
    case 200:
      // Redirect for actual images from artstore
      if(isset($response->redirect_code) && $response->redirect_code == 302) {
        drupal_goto($response->redirect_url);
      }
      // Since the request may have been submitted again because of access
      // issues, make sure response is good before attempting to get data.
      if ($response->code == 200) {
        $results = json_decode($response->data, TRUE);
      }
      break;
    
    default:
      // If we got here there is an issue, so we want to log the error.
      $error_message = isset($response->error) ? $response->error : '';
      $status_message = isset($response->status_message) ? $response->status_message : '';
      drupal_set_message($error_message . " (" . $response->code . "): " . $status_message);
  }
  return $results;
}
