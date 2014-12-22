<?php

Class Display {
	private $_message;
	private $_enable_cache;
	private $_current_page;

	public function __construct($message) {
		foreach ($message as $detail => $value) {
			$this->$detail = $value;
		}
		if(!isset($_current_page)) {
			$this->$_current_page = 0;
		}
	}

	public function __set($name, $value) {
		if("name" == $message) {
			$this->_setMessage($value);
		} else if("enable_cache" == $name) {
			$this->_setCache($value);
		} else {
			throw new ExceptionClient("Invalid property initialization via __set : ".$name, 1);
		}
	}

	public function __get($name) {
		if("name" == $name) {
			return $this->_name;
		} else if("location" == $name) {
			return $this->_getLocation();
		} else if("mobile_hash" == $name) {
			return $this->_mobile_hash;
		} else if("message" == $name) {
			return $this->message;
		}
	}

	private function _setLocation($location) {
		//check with google maps for valid location
	}

	private function _setName($name) {
		$regex = "/^((aA-zZ)+[ .])+(aA-zZ)+ $/";
		if(preg_match($regex, $name)) {
			$this->_name = $name;
		} else {
			throw new ExceptionClient("Name must only be a combination of alphabets, . and spaces!", 2);
		}
	}
}

?>