<?php

Class Client {
	private $_mobile_hash;
	private $_location;
	private $_name;

	public function __construct($details) {
		foreach ($details as $detail => $value) {
			$this->$detail = $value;
		}
	}

	public function __set($name, $value) {
		if("name" == $name) {
			$this->_setName($value);
		} else if("location" == $name) {
			$this->_setLocation($value);
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
	}

	private function _setName($name) {
		$regex = "/^((aA-zZ)+[ .])+(aA-zZ)+ $/";
		if(preg_match($regex, $name)) {
			$this->_name = $name;
		} else {
			throw new ExceptionClient("Name must only be a combination of alphabets, . and spaces!", 2);
		}
	}

	public static function load($mobile_hash, $autocreate = true) {
		$val_hash = "/^\w{8}\-\w{4}\-\w{4}\-\w{4}\-\w{12}$/";
		if(preg_match($val_hash, $mobile_hash))
			$this->_mobile_hash = $mobile_hash;
		else
			throw new ExceptionFetch("Request via invalid mobile hash", 5);

		$db = Database::getInstance();

		$query = "SELECT `name`, `location`, `mobile_hash` FROM `users` WHERE `mobile_hash` = '$mobile_hash";
		$result=$db->mysqli($query);
		if($row = $result->fetch_assoc()) {
			return new Client($row);
		} else if($autocreate) {
			$loc_xml = file_get_contents("http://api.txtweb.com/v1/location/get?txtweb‐mobile={$this->mobile_hash}");
			$loc_dom = new DOMDocument();
			$loc_dom->loadXML($loc_xml);
			$this->location = $loc_dom->getElementsByTagName('city')->item(0)->nodeValue;

			$query = "INSERT INTO `users` (`mobile_hash`, `join_date`) VALUES ('$mobile_hash', CURRENT_TIMESTAMP)";
			if(!$db->mysqli($query)) {
				throw new ExceptionClient("unable to save user details!", 3);
			}
			return new Client(array($mobile_hash));
		} else {
			throw new Exception ("unable to find any such user!", 4);
		}
	}

	public function save(){
		$db = Database::getInstance();
		$mysqli = $db->getConnection();

		$query = "UPDATE `users` SET `name`='{$this->name}', `location`='{$this->location}', `last_access`=CURRENT_TIMESTAMP WHERE `mobile_hash`='{$this->_mobile_hash}'";
		if($mysql->query($query)) {
			return true;
		} else {
			throw new Exception("unable to save users details!", 5);
		}
	}
}

?>