# Shared Shelf API module

**Created by Than Grove (May, 2014)**

This module adapts code from Jack Kelly's Shared Shelf Media module extension into a Drupal API service to call Shared Shelf for data in JSON or XML format. 

## Module Set Up

There are no dependencies for this module other than having an account on Shared Shelf. At present, only one account can be references per Drupal installation. After installing the module you will need to go to /admin/config/services/sharedshelf_api and enter in information about the Shared Shelf account to which this will be connected. By default, Admins can set Shared Shelf authorization data but this permission can be changed at /admin/people/permissions#module-sharedshelf_api 

## Module Use

Once set up, the module will enable GET calls for information from the associated Shared Shelf account. The data format for the returned information is specified by the extension: '.json' returns JSON and '.xml' returns XML. The format of the JSON or XML is specific to Shared Shelf. The following is a list of calls and their description:

<table>
	<tr><th>API Call</th><th>Description</th></tr>
	<tr>
		<td>
			/sharedshelf/api/assets/{asset ID #}.json<br/>
			/sharedshelf/api/assets/{asset ID #}.xml<br/>
			<em>e.g.,</em> /sharedshelf/api/assets/2652868
		</td>
		<td>
			Returns information about a single asset
		</td>
	</tr>
	<tr>
		<td>
			/sharedshelf/api/projects.json<br/>			
			/sharedshelf/api/projects.xml
		</td>
		<td>
			This returns a list of projects associated with the account
		<td>
	</tr>
	<tr>
		<td>
			/sharedshelf/api/projects/{project ID #}/assets.json<br/>
			/sharedshelf/api/projects/{project ID #}/assets.xml<br/>
			<em>e.g.,</em> /sharedshelf/api/projects/534/assets.xml
		</td>
		<td>Returns a list of assets for project number with the ID number of #.</td>
	</tr>
	<tr>
		<td valign="top">
			/sharedshelf/api/projects/###/assets/filter/{field_name}/{field_value}.json <br/>
			/sharedshelf/api/projects/###/assets/filter/{field_name}/{field_value}.xml <br/>
			<em>i.e.,</em> /sharedshelf/api/projects/534/assets/filter/fd_24809_lookup.links.source_id/1300.json
		</td>
		<td>
			Returns assets from project ### which have a {field_name} whose value is {field_value}.<br/>
			fd_24809_lookup.links.source_id = Kmaps Subject (for Shanti Shared Shelf)<br/>
			fd_24803_lookup.links.source_id = Kmaps Places
		</td>
	</tr>
</table>

The format of the returned is quite elaborated and cannot be fully described here. The best way to understand the return data for processing it to look at a few examples once the module is installed. However, a few comments can be made. In XML the top-level or root element is always <results>; in JSON this is represented simply as curly brackets. All returned result sets have the following fields:

<table>
	<tr><th>Field Name</th><th>Description</th></tr>
	<tr>
		<td>total</td><td>The total number of results returned</td>
	</tr>
	<tr>
		<td>assets (or items)</td><td>An array of returned assets or items (for project list). Each asset or item is marked up according to Shared Shelf's unique and somewhat complex markup scheme.</td>
	</tr>
	<tr>
		<td>success</td><td>A boolean set to true if the call was successful</td>
	</tr>
	<tr>
		<td>api_host</td><td>The host server url for the account, usually http://catalog.sharedshelf.artstor.org</td>
	</tr>
	<tr>
		<td>api_path</td><td>The service path of the call without the extension, e.g. /projects/534/assets</td>
	</tr>
</table>

