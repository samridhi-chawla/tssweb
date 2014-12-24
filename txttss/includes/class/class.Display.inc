<?php

Class Display {
	private $_message;
	private $_current_page;
	private $_default_message = array(
		'home' => 'Welcome to TSS SMS Services. Please select any of the following options:
			<br/><a href="http://tss-gndu.com/txttss/?page=updates" class="txtweb-menu-for">Latest News</a><br/>
			<br/><a href="http://tss-gndu.com/txttss/?page=blog" class="txtweb-menu-for">Blog Updates</a><br/>
			<br/><a href="http://tss-gndu.com/txttss/?page=query" class="txtweb-menu-for">Queries</a><br/>
			<br/><a href="http://tss-gndu.com/txttss/?page=location" class="txtweb-menu-for">Set your location</a><br/>
			<br/><a href="http://tss-gndu.com/txttss/?page=name" class="txtweb-menu-for">Set your name</a><br/>',
		'updates' => 'Latest on TSS<br/>',
		'blog' => 'TSS Blog<br/>',
		'query' => 'TSS SMS Services<br /><br />
			Please reply with your contact details and query : 
			<form action="http://tss-gndu.com/txttss/?page=query" class="txtweb‐form" method="get">
			 query<input type="text" name="txtweb‐message"/>
			<input type="submit" value="Submit" />
			</form>
			e.g. someone@domain.com How to join TSS?',
		'location' => '<form action="http://api.txtweb.com/location/set" method="POST" class="txtweb‐form">
			location<input name="txtweb‐location" type="text" />
			<input type="submit" value="Submit" />
			</form>',
		'name' => 'TSS SMS Services<br /><br />
			Set your name : 
			<form action="http://tss-gndu.com/txttss/?page=name" class="txtweb‐form" method="get">
			name<input type="text" name="txtweb‐message" />
			<input type="submit" value="Submit" />
			</form>'
		);

	public function __construct($message) {
		foreach ($message as $detail => $value) {
			$this->$detail = $value;
		}
	}

	public function __set($name, $value) {
		if("message" == $name) {
			$this->_setMessage($value);
		} else if("enable_cache" == $name) {
			$this->_setCache($value);
		} else if("current_page" == $name){
			$this->_setCurrentPage($value);
		}
	}

	public function __toString() {
		return '
		<html>
		<head>
		  <title> TSS Text Services </title>
		   <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
		  <meta name="txtweb-appkey" content="73684a1f-0c3b-4144-8b0b-072950eb2b03" />
		</head>
		<body>'.
		  $this->_message
		.'</body>
		</html>';
	}

	private function _setMessage($msg) {
		$this->_message = $msg;
	}

	private function _setCurrentPage($page) {
		if(!empty($page) || !in_array($page, $this->_default_message)) {
			$this->_current_page = $page;
		} else {
			$this->_current_page = "home";
		}
		$this->_setMessage($this->_getMessage()["before"].$this->_default_message[$this->_current_page].$this->_getMessage()["after"]);
	}

	private function _getMessage() {
		return $this->_message;
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
			$html .= "<a href=\"http://tss-gndu.com/txttss/?page=blog&post=".$post['post_no']."\">".$post['title']."</a><br/>";
		}
		return $html;
	}
	static function getBlogPost($post_no) {
		$post = Blog::fetchPostContent($post_no);
		$html = $post['title']."<br/>".$post['content'];
		return $html;
	}
}

?>