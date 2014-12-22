<?php
// This code is for updating recent blog posts. Please don't remove it.
class Blog {
	private $_xml;

	public function __construct($feed = 'http://blog.tss-gndu.com/?feed=rss2') {
		$this->_xml = new DOMDocument();
		if(!$this->_xml->load($feed)) {
			$this->_xml->load('error.xml');
		}
	}

	public function fetchLatest($start = 0, $count = NULL) {
		$latestPost = $this->_xml->getElementsByTagName('item');
		
		$blogPost = array(
			'post_no' => NULL,
			'title' => "no posts found",
			'desc' => "no posts found",
			'content' => "no posts found",
			'link' => "http://blog.tss-gndu.com",
			'author' => "n/a",
			'date' => "Day, date, month, year"
		);

		$i = 0;
		foreach ($latestPost as $node) {
			$link = $node->getElementsByTagName('link')->item(0)->nodeValue;
			$post_no = (int) substr($link, strpos($link, "?p=") + 3);
			$desc = $node->getElementsByTagName('description')->item(0)->nodeValue;
			$desc = substr($desc, 0, strlen($desc)-10)."... ";
			$date = new DateTime($node->getElementsByTagName('pubDate')->item(0)->nodeValue);
			$date->setTimezone(new DateTimeZone("Asia/Kolkata"));
			$date = $date->format('h:i A, l, d F, Y');
			$blogPost[$i] = array ( 
				'post_no' => $post_no,
				'title' => $node->getElementsByTagName('title')->item(0)->nodeValue,
				'desc' => $desc,
				'content' => $node->getElementsByTagName('encoded')->item(0)->nodeValue,
				'link' => $link,
				'author' => $node->getElementsByTagName('creator')->item(0)->nodeValue,
				'date' => $date
			);

			$i++;
			if(!is_null($count) && $i >= $count)
				break;
		}

		return $blogPost;
	}

	static function fetchPostContent($post_no=NULL) {
		$xml = new DOMDocument();
		$xml->loadXML("http://blog.tss-gndu.com/?feed=rss2&p=".$post_no);
		$xml->
	}
}

?>