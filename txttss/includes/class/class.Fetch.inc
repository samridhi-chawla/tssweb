<?php

class Fetch {
	private $_REQUEST;
	private $_message;
	private $_verify_id;
	private $_agg_id;
	private $_protocol;
	private $_id;

	private $_GET;
	private $_current_page;
	private $_current_page_requests;
	private $_display_message;

	private $_user;

	public function __construct($REQUEST, $GET) {
		$this->_setREQUEST($REQUEST);
		$this->_setGET($GET);
		$this->_display_message = array("before" => "", "after" => "");
		if(isset($this->REQUEST['txtweb-mobile']) && isset($this->REQUEST['txtweb-verifyid']) && isset($this->REQUEST['txtweb-aggid']) && isset($this->REQUEST['txtweb-id']) && isset($this->REQUEST['txtweb-protocol'])) {
			$this->_setMessage($this->REQUEST['txtweb-message']);
			$this->_setUser($this->REQUEST['txtweb-mobile']);
			$this->_setVerifyID($this->REQUEST['txtweb-verifyid']);
			$this->_setAggID($this->REQUEST['txtweb-aggid']);
			$this->_setID($this->REQUEST['txtweb-id']);
			$this->_setProtocol($this->REQUEST['txtweb-protocol']);
			@$this->_setCurrentPage($this->GET['page']);
			$this->_setCurrentPageRequests($this->GET);
		} else {
			throw new ExceptionFetch("invalid requests from user side", 1);
		}
	}

	public function __get($name) {
		if("display_message" == $name) {
			return $this->_display_message;
		} else if("REQUEST" == $name) {
			return $this->_REQUEST;
		} else if("GET" == $name) {
			return $this->_GET;
		}
	}

	public function display() {
		$message = array("message" => $this->_display_message, "current_page" => $this->_current_page);
		$display = new Display($message);
		return (string)$display;
	}

	private function _setREQUEST($array) {
		if(is_array($array)) {
			$this->_REQUEST = $array;
		} else
			throw new ExceptionFetch("Invalid REQUEST requests passed to ".get_class(self), 2);	
	}

	private function _setGET($array) {
		if(is_array($array)) {
			$this->_GET = $array;
		} else
			throw new ExceptionFetch("Invalid GET requests passed to ".get_class(self), 3);	
	}

	private function _setMessage($msg) {
		$this->_message = $msg;
	}

	private function _setUser($mobile_hash) {
		$this->_user = Client::load($mobile_hash);
	}

	private function _setVerifyID($verify_id) {
		$this->_verify_id = $verify_id;
	}

	private function _setAggID($agg_id) {
		$this->_agg_id = $agg_id;
	}

	private function _setID($id) {
		$this->_id = $id;
	}

	private function _setProtocol($protocol) {
		$this->_protocol = $protocol;
	}

	private function _setCurrentPage() {
		if(isset($this->_GET['page'])) {
			$this->_current_page = $this->_GET['page'];
		} else {
			$this->_current_page = "home";
			$this->_GET['page'] = "home";
		}
	}

	private function _setCurrentPageRequests() {
		if("home" == $this->GET['page']) {
			$this->_display_message["before"] = "Hello";
			if ($this->_user->name) {
				$this->_display_message["before"] .= " ".$this->_user->name;
			} else{
				$this->_display_message["before"] .= " User";
			}
			$this->_display_message["before"] .= ". ";
		} else if("updates" == $this->GET['page']){
			//$this->display_message = Display::getTSSUpdates(4);
		} else if("blog" == $this->GET['page']) {
			if(isset($this->GET['post']) && !empty($this->GET['post'])) {
				$this->_display_message["after"] = Display::getBlogPost($this->GET['post']);
			} else {
				$this->_display_message["after"] = Display::getBlogUpdates(4); 
			}
		} else if("query" == $this->GET['page']) {
			$this->_display_message["before"] = $this->REQUEST['txtweb-message'];
			if(isset($this->REQUEST['txtweb-message']) && !empty($this->REQUEST['txtweb-message'])) {
				$query = trim($this->GET['query']);
				$val_contact = "/^(\w+([\.\-_]\w+)*@\w+(\-\w+)*(\.\w{2,5})+)( (\w+([\.\-_]\w+)*@\w+(\-\w+)*(\.\w{2,5})+))*/";
				preg_match($val_contact, $query, $contact);
				$query = trim(preg_split($val_contact, $query)[0]);
				if(strlen($contact[0]) && strlen($query[0])) {
					$contact = explode(" ", $contact[0]);
					$query = "INSERT INTO `query` (`query`, `mobile_hash`, `email`) VALUES ('$query', '$this->_mobile_hash', '$contact')";
					$db = Database::getInstance();
					$mysqli = Database::getConnection();
					if(!$mysqli->query($query))
						throw new ExceptionFetch("unable to post your query. Please try again later.", 4);
					$this->_display_message["before"] = "we have received your query and will respond to it within 3 working days.";
				} else {
					$this->_display_message["before"] = "Please use the correct syntax to post your query.";
				}
			}
		} else if("name" == $this->GET['page']) {
			if(isset($this->REQUEST['txtweb-message']) && !empty($this->REQUEST['txtweb-message'])) {
				$val_name = "/^\w+(( |\. )\w+)*$/";
				if(preg_match($val_name, $this->GET['name'])) {
					$this->_user->name = $this->GET['name'];
					$this->_user->save();
					$this->_display_message["before"] = "name has been successfully updated.";
				} else {
					$this->_display_message["before"] = "Only alphabets, . and space are allowed in name!";
				}
			}
		}
	}
}

?>