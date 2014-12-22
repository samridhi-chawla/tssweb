<?php

Class Display {
	private $_message;
	private $_enable_cache;
	private $_current_page;
	private $_default_message = array(
		'home' => 'Welcome to TSS SMS Services. Please select any of the following options:
			<br/><a href="index.php?page=updates">Latest News</a><br/>
			<br/><a href="index.php?page=blog">Blog Updates</a><br/>
			<br/><a href="index.php?page=query">Queries</a><br/>
			<br/><a href="index.php?page=location">Set your location</a><br/>
			<br/><a href="index.php?page=name">Set your name</a><br/>',
		'updates' => 'Latest on TSS<br/>',
		'blog' => 'TSS Blog<br/>',
		'query' => '',
		'location' => '<form action="http://api.txtweb.com/location/set" method="POST" class="txtweb‐form">
			location<input name="txtweb‐location" type="text" />
			<input type="hidden" name="txtweb‐callback‐message" value="" />
			<input type="submit" value="Submit" />
			</form>',
		'name' => 'TSS SMS Services<br /><br />
			Set your name : 
			<form action="index.php?p=name" class="txtweb‐form" method="get">
			name<input type="text" name="txtweb‐message" />
			<input type="submit" value="Submit" />
			</form>'
		);

	public function __construct($message) {
		foreach ($message as $detail => $value) {
			$this->$detail = $value;
		}
		if(!isset($_current_page)) {
			$this->$_current_page = "home";
		}
	}

	public function __set($name, $value) {
		if("message" == $name) {
			$this->_setMessage($value);
		} else if("enable_cache" == $name) {
			$this->_setCache($value);
		} else if("current_page" == $name){
			$this->_setCurrentPage($location);
		} else {
			throw new ExceptionDisplay("Invalid property initialization via __set : ".$name, 1);
		}
	}

	public function __toString() {
		return '
		<html>
		<head>
		  <title> Check out the new releases </title>
		  <meta name="txtweb‐appkey" content="ABCDEFGH‐IJKL‐MNOP‐QRST‐UVWXYZ01234" />
		</head>
		<body>'.
		  $this->message;
		.'</body>
		</html>';
	}

	private function _setCache($time) {
		
	}

	private function _setMessage($msg) {
		$this->_message = $msg;
	}

	private function _setCurrentPage($page) {
		$this->_current_page = $page;
		$this->_setMessage($this->_getMessage().$this->_default_message[$page]);
	}

	static function getTSSUpdates($start, $count) {
		$news = new News();
		$updates = $news->fetchLatest($count);
		$html = "";
		foreach($updates as $post) {
			$html .= "<a href=\"index.php?p=news&post=".$post['post_no']."\">".$post['title']."</a><br/>";
		}
		return $html;
	}

	static function getBlogUpdates($count) {
		$blog = new Blog();
		$updates = $blog->fetchLatest($count);
		$html = "";
		foreach($updates as $post) {
			$html .= "<a href=\"index.php?p=blog&post=".$post['post_no']."\">".$post['title']."</a><br/>";
		}
		return $html;
	}
}

?>