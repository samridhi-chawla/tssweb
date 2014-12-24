<?php

class Database{
	private static $_instance;
	private $_connection;

	function __construct() {
		if(!$this->_connection = new mysqli("localhost", "tssgndu_txtweb", '1$$GNDUtxtweb'	, "tssgndu_txtweb")) {
			throw new ExceptionDatabase("unable to connect to database", 1);
		}
		return $this->_connection;
	}

	public static function getInstance() {
		if(!self::$_instance) {
			self::$_instance = new self();
		}
		return self::$_instance;
	}

	public function getConnection() {
		return $this->_connection;
	}

	function __clone() {}
}

?>