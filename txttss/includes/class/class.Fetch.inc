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

	public function __construct($user, $_REQUEST, $_GET) {
		$this->REQUEST = $_REQUEST;
		$this->GET = $_GET;
		$this->_setMessage($this->_REQUEST['txtweb-message']);
		$this->_setUser($this->_REQUEST['txtweb-mobile']);
		$this->_setVerifyID($this->_REQUEST['txtweb-verifyid']);
		$this->_setAggID($this->_REQUEST['txtweb-aggid']);
		$this->_setID($this->_REQUEST['txtweb-id']);
		$this->_setProtocol($this->_REQUEST['txtweb-protocol']);
		$this->_setCurrentPage($this->_GET['page']);
		$this->_setCurrentPageRequests($this->_GET);
	}

	public function __set($name, $value) {
		if("display_message" == $name) {
			$this->_setDisplayMessage($value);
		} else if("REQUEST" == $name) {
			$this->_setREQUEST($value);
		} else if("GET" == $name) {
			$this->_setGET($value);
		} else {
			throw new ExceptionFetch("Invalid property being initialized via __set::".$name, 2);
		}
	}

	public function display() {
		if($this->_current_page) {
			$this->
		}
	}

	private function _setREQUEST($array) {
		if(is_array($array)) {
			$this->_REQUEST = $array;
		} else
			throw new ExceptionFetch("Invalid requests passed to ".get_class(self)., 3);	
	}

	private function _setGET($array) {
		if(is_array($array)) {
			$this->_GET = $array;
		} else
			throw new ExceptionFetch("Invalid GET requests passed to ".get_class(self)., 4);	
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
		$this->_current_page = $this->_GET['page'];
	}

	private function _setCurrentPageRequests() {
		if("home" == $this->GET['page']) {
			$this->display_message = "Hello";
			if ($this->_user->name) {
				$this->display_message .= $this->_user->name;
			} else{
				$this->display_message .= "User";
			}
		} else if("updates" == $this->GET['page']){
			$this->display_message = self::getTSSUpdates(4);
		} else if("blog" == $this->GET['page']) {
			$this->display_message = self::getBlogUpdates(4); 
		} else if("query" == $this->GET['query']) {
			if(isset($this->GET['query'])) {
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
						throw new Exception("unable to post your query. Please try again later.", 1);
					$this->display_message = "we have received your query and will respond to it within 3 working days.";
				} else {
					$this->display_message = "Please use the correct syntax to post your query.";
				}
			}
		} else if("name" == $this->GET['page']) {
			if(isset($this->_GET['name'])) {
				$val_name = "/^\w+(( |\. )\w+)*$/";
				if(preg_match($val_name, $this->_GET['name'])) {
					$this->_user->name = $this->_GET['name'];
					$this->_user->save();
					$this->display_message = "name has been successfully updated.";
				} else {
					$this->display_message = "Only alphabets, . and space are allowed in name!";
				}
			}
		}
	}
}

?>