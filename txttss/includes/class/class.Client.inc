<?php

Class Client {
	private $_mobile_hash;
	private $_location;
	private $_name;

	public function __construct($details) {
		foreach ($details as $detail => $value) {
			$this->$detail = $value;
		}
		$this->_mobile_hash = $details['mobile_hash'];
	}

	public function __set($name, $value) {
		if("name" == $name) {
			$this->_setName($value);
		} else if("location" == $name) {
			$this->_setLocation($value);
		}
	}

	public function __get($name) {
		if("name" == $name) {
			return $this->_name;
		} else if("location" == $name) {
			return $this->_location;
		} else if("mobile_hash" == $name) {
			return $this->_mobile_hash;
		}
	}
	
	public static function getLocation($mobile_hash) {
		$loc_xml = file_get_contents("http://api.txtweb.com/v1/location/get?txtweb-mobile=$mobile_hash"); 
		$loc_dom = new DOMDocument();
		$loc_dom->loadXML($loc_xml);
		$location = $loc_dom->getElementsByTagName('city')->item(0)->nodeValue;
		return $location;
	}

	private function _setName($name) {
		$regex = "/^((aA-zZ)+[ .])+(aA-zZ)+$/";
		if(!strlen($name) || preg_match($regex, $name)) {
			$this->_name = $name;
		} else {
			throw new ExceptionClient("Name must only be a combination of alphabets, . and spaces!", 2);
		}
	}

	private function _setLocation($location) {
		$this->_location = $location;
		if(trim($this->location) == "" && isset($this->mobile_hash)) {
			$this->_location = Client::getLocation($this->mobile_hash);
		}
	}

	public static function load($mobile_hash, $autocreate = true) {
		$val_hash = "/^\w{8}\-\w{4}\-\w{4}\-\w{4}\-\w{12}$/";
		if(!preg_match($val_hash, $mobile_hash))
			throw new ExceptionClient("Request via invalid mobile hash", 3);

		$db = Database::getInstance();
		$mysqli = $db->getConnection();
		$query = "SELECT `name`, `location`, `mobile_hash` FROM `users` WHERE `mobile_hash` = '$mobile_hash'";
		if(!$result=$mysqli->query($query)) {
			throw new ExceptionClient("unable to load user details", 4);
		}

		if($row = $result->fetch_assoc()) {
			$query = "UPDATE `users` SET `last_access` = CURRENT_TIMESTAMP WHERE `mobile_hash` = '$mobile_hash'";
			if($mysqli->query($query)) {
				return new Client($row);
			} else {
				throw new Exception("Error Updating last access date", 5);
			}
		} else if($autocreate) {
			$location = Client::getLocation($mobile_hash);

			$query = "INSERT INTO `users` (`mobile_hash`, `location`, `join_date`) VALUES ('$mobile_hash', '$location', CURRENT_TIMESTAMP)";
			if(!$mysqli->query($query)) {
				throw new ExceptionClient("unable to save user details!", 6);
			}
			return new Client(array("mobile_hash" => $mobile_hash, "location" => $location));
		} else {
			throw new ExceptionClient("unable to find any such user!", 7);
		}
	}

	public function save(){
		$db = Database::getInstance();
		$mysqli = $db->getConnection();

		$query = "UPDATE `users` SET `name`='{$this->name}', `location`='{$this->location}', `last_access`=CURRENT_TIMESTAMP WHERE `mobile_hash`='{$this->_mobile_hash}'";
		if($mysql->query($query)) {
			return true;
		} else {
			throw new ExceptionClient("unable to save users details!", 8);
		}
	}
}

?>